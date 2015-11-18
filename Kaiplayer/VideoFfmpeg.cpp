#include <wx/dir.h>
#include "VideoFfmpeg.h"
#include "kainoteApp.h"
#include "config.h"
#include "mkv_wrap.h"
#include <objbase.h>

#include "include\ffmscompat.h"



class listw : public wxDialog
	{
	public:
		listw(wxWindow *parent, wxArrayString suggest);
		virtual ~listw();
		wxListBox *disperrs;
	private:
		
		void OnDoubleClick(wxCommandEvent& event);
	};

listw::listw(wxWindow *parent, wxArrayString suggest)
	: wxDialog(parent,-1,"Wybierz �cie�k�")
{
	wxBoxSizer *sizer=new wxBoxSizer(wxHORIZONTAL);
	disperrs=new wxListBox(this,29886,wxDefaultPosition,wxDefaultSize,suggest);
	sizer->Add(disperrs,1,wxEXPAND|wxALL,2);
	sizer->SetMinSize(100,100);
	SetSizerAndFit(sizer);
	CenterOnParent();
	Connect(29886,wxEVT_COMMAND_LISTBOX_DOUBLECLICKED,(wxObjectEventFunction)&listw::OnDoubleClick);
}
listw::~listw()
{
}

void listw::OnDoubleClick(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}


VideoFfmpeg::VideoFfmpeg(wxString filename, int tab, bool *success)
{
	isgetstream=false;
	atab=tab;
	filen=filename;
	//com_inited = false;
	if(!Options.AudioOpts && !Options.LoadAudioOpts()){wxMessageBox("Dupa blada, opcje si� nie wczyta�y, na audio nie podzia�asz", "B��dny b��d");}
	disccache = !Options.GetBool("Audio RAM Cache");
	blnum=0;
	Cache=0;
	lastframe=-1;
	lasttime=-1;
	Delay=0;
	audiosource=0;
	videosource=0;
	progress=0;
	//fft=0;
	
	//if (SUCCEEDED(CoInitializeEx(0,0))) 
	//{}	//com_inited = true;
	//else{
		//*success=false;
		//wxLogStatus("Dupa blada COM si� nie zainicjalizowa�");
	//}
	//CoInitializeEx(0,COINIT_APARTMENTTHREADED);
	FFMS_Init(0, 1);

	char errmsg[1024];
	errinfo.Buffer      = errmsg;
	errinfo.BufferSize  = sizeof(errmsg);
	errinfo.ErrorType   = FFMS_ERROR_SUCCESS;
	errinfo.SubType     = FFMS_ERROR_SUCCESS;
	
	*success=(Init()==1);
	if(progress){
		progress->Destroy();
		progress=0;}
	if(audiosource){FFMS_DestroyAudioSource(audiosource);audiosource=0;}
}



int VideoFfmpeg::Init()
{

	kainoteApp *Kaia=(kainoteApp*)wxTheApp;

	
	FFMS_Indexer *Indexer = FFMS_CreateIndexer(filen.utf8_str(), &errinfo);

	int NumTracks = FFMS_GetNumTracksI(Indexer);
	int audiotrack=-1;
	wxArrayInt audiotable;
	int videotrack=-1;
	
	for (int i=0; i<NumTracks; i++) {
		if (FFMS_GetTrackTypeI(Indexer, i) == FFMS_TYPE_VIDEO && videotrack==-1) {
			videotrack=i;
		}
		else if (FFMS_GetTrackTypeI(Indexer, i) == FFMS_TYPE_AUDIO) {
			audiotable.Add(i);
		}
		//else if(audiotrack!=-1 && videotrack !=-1)
			//{break;}
	}
	bool ismkv=(filen.AfterLast('.').Lower()=="mkv");
	
	if(audiotable.size()>1 || ismkv){

		wxArrayString tracks;
		
		if(ismkv){
			MatroskaWrapper mw;mw.Open(filen,false);
			if(audiotable.size()>1){
				for (size_t j=0;j<audiotable.size();j++){
					TrackInfo* ti=mkv_GetTrackInfo(mw.file,audiotable[j]);
					
					wxString all;
					char *opis = (ti->Name)? ti->Name : ti->Language;
					all<<audiotable[j]<<": "<<opis;
					tracks.Add(all);
				}
			}
			Chapter *chap=NULL;
			UINT nchap=0;
			mkv_GetChapters(mw.file,&chap,&nchap);
			VideoRend *rend=Notebook::GetTab()->Video;
			//wxLogStatus("chap %i, %i", (int)chap, (int)nchap);
			if(chap && nchap){
				for(int i=0; i<(int)chap->nChildren; i++){
					chapter ch;
					ch.name=wxString(chap->Children[i].Display->String);
					ch.time=(int)(chap->Children[i].Start/1000000.0);
					rend->chaps.push_back(ch);
				}
			}
			mw.Close();
			if(audiotable.size()<2){audiotrack= (audiotable.size()>0)? audiotable[0] : -1; goto done;}
		}else{
			for (size_t j=0;j<audiotable.size();j++){
				wxString CodecName(FFMS_GetCodecNameI(Indexer, audiotable[j]), wxConvUTF8);
				wxString all;
				all<<audiotable[j]<<": "<<CodecName;
				tracks.Add(all);
			}
		}
		listw lwind(Kaia->Frame,tracks);

		if(lwind.ShowModal()==wxID_OK)
		{
			//wxLogStatus("wesz�o");
			wxString result=lwind.disperrs->GetString(lwind.disperrs->GetSelection());
			//wxLogStatus(result);
			audiotrack=wxAtoi(result.BeforeFirst(':'));
			
		}else{return 0;}
		
    }else if(audiotable.size()>0){
		audiotrack=audiotable[0];
	}
done:

	wxString path=Options.pathfull + "\\Indices\\" + filen.AfterLast('\\').BeforeLast('.') + wxString::Format("_%i.ffindex",audiotrack);

	FFMS_Index *index=NULL;

	
	progress = new ProgresDialog(Kaia->Frame,_("Indeksowanie pliku wideo"));
	
	//wxLogStatus("Read index");
	if(wxFileExists(path)){
		index = FFMS_ReadIndex(path.utf8_str(), &errinfo);

		if(FFMS_IndexBelongsToFile(index, filen.utf8_str(), &errinfo))
		{
			FFMS_DestroyIndex(index);
			index=NULL;
		}

	}
	
	if(!index){
	
		index =FFMS_DoIndexing(Indexer, (1<<audiotrack), 0, NULL,NULL, FFMS_IEH_IGNORE, UpdateProgress, (void*)progress, &errinfo);
		//index =FFMS_MakeIndex(filen.utf8_str(), /*(1<<audiotrack)*/0, 0, NULL,NULL, FFMS_IEH_ABORT, NULL, NULL, &errinfo);
		//wxLogStatus("vidtrack %i, audtrack %i 2",videotrack,audiotrack);
		if (index == NULL) {
			if(wxString(errinfo.Buffer).StartsWith(_("Cancelled"))){wxMessageBox(_("Indeksowanie anulowane przez u�ytkownika"),"B��d",5,Kaia->Frame);}
			else{
				wxMessageBox(wxString::Format(_("Wyst�pi� b��d indeksowania: %s"),errinfo.Buffer),"B��d",5,Kaia->Frame);}
			//FFMS_CancelIndexing(Indexer);
			return 0;
		}
		//wxLogStatus("write index "+path);
		if(!wxDir::Exists(path.BeforeLast('\\')))
		{
			wxDir::Make(path.BeforeLast('\\'));
		}
		//wxLogStatus("write index");
		if(FFMS_WriteIndex(path.utf8_str(), index, &errinfo))
		{
			wxLogStatus("Nie mo�na zapisa� indeksu, wyst�pi� b��d %s", errinfo.Buffer);
			FFMS_DestroyIndex(index);
			return 0;
		}
		//wxLogStatus("Index writed");

	}

	
	//wxLogStatus("video");
	if(videotrack!=-1){	
		//wxLogStatus("num of cores %i", (int)std::thread::hardware_concurrency());
		SYSTEM_INFO sysinfo;
		GetSystemInfo( &sysinfo );
		videosource = FFMS_CreateVideoSource(filen.utf8_str(), videotrack, index, sysinfo.dwNumberOfProcessors, FFMS_SEEK_NORMAL, &errinfo);
		//Since the index is copied into the video source object upon its creation,
		//we can and should now destroy the index object. 
			
		if (videosource == NULL) {
			wxLogStatus("Dupa bada videosource failed");
			return 0;
		}


				
		//wxLogStatus("videoprops");
		const FFMS_VideoProperties *videoprops = FFMS_GetVideoProperties(videosource);
		//wxLogStatus("numframes");
		NumFrames = videoprops->NumFrames;
		Duration=videoprops->LastTime;
		Delay=videoprops->FirstTime;
		//wxLogStatus("fps %i / %i",videoprops->FPSNumerator, videoprops->FPSDenominator);
		fps=(float)videoprops->FPSNumerator/(float)videoprops->FPSDenominator;

		const FFMS_Frame *propframe = FFMS_GetFrame(videosource, 0, &errinfo);
		//wxLogStatus("propframe");
		width=propframe->EncodedWidth;
		height=propframe->EncodedHeight; 
		arwidth=(videoprops->SARNum==0)? width : (float)width*((float)videoprops->SARNum/(float)videoprops->SARDen);
		arheight= height;
		while(1){
			bool divided=false;
			for (int i = 10; i>1; i--){
				if((arwidth % i)==0 && (arheight % i)==0){
					arwidth/=i; arheight /=i;
					divided=true;
					break;
				}
			}
			if(!divided){break;}
		}
		
	
		int pixfmt[2];
		pixfmt[0] =PIX_FMT_NV12; //PIX_FMT_YUVJ420P;//PIX_FMT_YUV411P;//PIX_FMT_YUV420P; //PIX_FMT_YUYV422;//PIX_FMT_NV12;//FFMS_GetPixFmt("bgra");PIX_FMT_YUYV422;//
		pixfmt[1] = -1;

		//wxLogStatus("Set format %i %i",width, height);
		if (FFMS_SetOutputFormatV2(videosource, pixfmt, width, height, FFMS_RESIZER_BILINEAR, &errinfo)) {
			wxLogStatus("Dupa bada nie mo�na przekonwertowa� na bgra");
			return 0;
		}
	
		FFMS_Track *FrameData = FFMS_GetTrackFromVideo(videosource);
		if (FrameData == NULL){
			wxLogStatus("Dupa bada nie mo�na pobra� �cie�ki z wideo");
			return 0;}
		const FFMS_TrackTimeBase *TimeBase = FFMS_GetTimeBase(FrameData);
		if (TimeBase == NULL){
			wxLogStatus("Dupa bada nie mo�na pobra� bazy czas�w");
			return 0;}

		const FFMS_FrameInfo *CurFrameData;

	
		// build list of keyframes and timecodes
		for (int CurFrameNum = 0; CurFrameNum < videoprops->NumFrames; CurFrameNum++) {
			CurFrameData = FFMS_GetFrameInfo(FrameData, CurFrameNum);
			if (CurFrameData == NULL) {
				continue;
			}

			// keyframe?
		
			int Timestamp = ((CurFrameData->PTS * TimeBase->Num) / TimeBase->Den);
			if (CurFrameData->KeyFrame){KeyFrames.Add(Timestamp);}
			Timecodes.push_back(Timestamp);
		

		}
	
	}
	/* Retrieve the track number of the first audio track */
	//int trackn = FFMS_GetFirstTrackOfType(index, FFMS_TYPE_AUDIO, &errinfo);
	//wxLogStatus("audio");
	if(audiotrack==-1){FFMS_DestroyIndex(index);SampleRate=-1;return 1;}
	/* We now have enough information to create the audio source object */
	audiosource = FFMS_CreateAudioSource(filen.utf8_str(), audiotrack, index, FFMS_DELAY_FIRST_VIDEO_TRACK, &errinfo);//FFMS_DELAY_FIRST_VIDEO_TRACK
	if (audiosource == NULL) {
		/* handle error (you should know what to do by now) */
		wxMessageBox(wxString::Format(_("Wyst�pi� b��d tworzenia �r�d�a audio: %s"),errinfo.Buffer),"B��d",5,Kaia->Frame);
		FFMS_DestroyIndex(index);
		return 0;
	}

	
	
	
	
	
	FFMS_DestroyIndex(index);
	//wxLogStatus("index destroyed");

	FFMS_ResampleOptions *resopts=FFMS_CreateResampleOptions(audiosource);
	resopts->ChannelLayout=FFMS_CH_FRONT_CENTER;
	resopts->SampleFormat=FFMS_FMT_S16;
	
	if (FFMS_SetOutputFormatA(audiosource, resopts, &errinfo))
	{
		wxMessageBox(wxString::Format(_("Wyst�pi� b��d konwertowania audio: %s"),errinfo.Buffer),"B��d",5,Kaia->Frame);
	}
	else
	{
		BytesPerSample=2;
		Channels=1;
	}
	FFMS_DestroyResampleOptions(resopts);
	const FFMS_AudioProperties *audioprops = FFMS_GetAudioProperties(audiosource);
	
	SampleRate=audioprops->SampleRate;
	//BytesPerSample=audioprops->BitsPerSample/8;
	//Channels=audioprops->Channels;
	Delay=audioprops->FirstTime;
	NumSamples=audioprops->NumSamples;
	audioprops = FFMS_GetAudioProperties(audiosource);
	

	if(disccache){
		diskCacheFilename="";
		diskCacheFilename<<Options.pathfull<<"\\AudioCache"<<atab<<".w64";
		if(!DiskCache()){return 0;}
		}else{if(!CacheIt()){return 0;}
	}
	
	return 1;
}

VideoFfmpeg::~VideoFfmpeg()
{
	
	KeyFrames.Clear();
	Timecodes.clear();
	//wxLogStatus("videosource");
	if(videosource){FFMS_DestroyVideoSource(videosource);videosource=NULL;}
	//wxLogStatus("couni");
	//if (com_inited){
		//CoUninitialize();//}
	//wxLogStatus("counited");
	/*if(fft){
		delete fft; fft=0;
	}*/

	if(disccache){Cleardiskc();}else{Clearcache();}
}



int FFMS_CC VideoFfmpeg::UpdateProgress(int64_t Current, int64_t Total, void *ICPrivate)
{
	ProgresDialog *progress= (ProgresDialog*)ICPrivate;
	progress->Progress(((double)Current/(double)Total)*100);
	return progress->WasCancelled();
}

void VideoFfmpeg::GetFrame(int ttime, byte *buff)
{
		//wxMutexLocker lock(blockaudio);
	//int fram=FramefromTime(time);
	//if(fram<0){*powt=true;fram=lastframe;}else{*powt=false;}
	if(lastframe!=ttime){fframe=FFMS_GetFrame(videosource, ttime, &errinfo);}//fframe=FFMS_GetFrameByTime(videosource, (double)ttime/1000.0, &errinfo);}
	lastframe=ttime;
	//const FFMS_Frame *fframe=FFMS_GetFrame(videosource, frame, &errinfo);
	int fplane=height*width;
	byte* cpy= (byte *)fframe->Data[0];
	memcpy(&buff[0],cpy,fplane);
	cpy= (byte *)fframe->Data[1];
	int uvplane=fplane/2;
	//wxLogMessage("%i, %i",vheight*pitch,fplane+uvplane);
	memcpy(&buff[fplane],cpy,uvplane);
}

void VideoFfmpeg::GetAudio(void *buf, int64_t start, int64_t count)
	{
	//wxMutexLocker lock(blockaudio);
	//wxLogStatus(_("wesz�o"));
	
	if (count == 0 || !audiosource) return;
	if (start+count > NumSamples) {
		int64_t oldcount = count;
		count = NumSamples-start;
		if (count < 0) count = 0;

		// Fill beyond with zero
		
			short *temp = (short *) buf;
			for (int64_t i=count;i<oldcount;i++) {
				temp[i] = 0;
			}

	}

	FFMS_GetAudio(audiosource, buf, start, count, &errinfo);
	
}

void VideoFfmpeg::GetBuffer(void *buf, int64_t start, int64_t count, double volume)
{
	wxMutexLocker lock(blockaudio);

	if (start+count > NumSamples) {
		int64_t oldcount = count;
		count = NumSamples-start;
		if (count < 0) count = 0;

		
			short *temp = (short *) buf;
			for (int i=count;i<oldcount;i++) {
				temp[i] = 0;
			}
	}

	if (count) {
		if(disccache){
			if(file_cache.IsOpened()){
			file_cache.Seek(start*GetBytesPerSample());
			file_cache.Read((char*)buf,count*GetBytesPerSample());}
		}
		else{
			if(!Cache){return;}
			char *tmpbuf = (char *)buf;
			int i = (start*GetBytesPerSample()) >> 22;
			int blsize=(1<<22);
			int offset = (start*GetBytesPerSample()) & (blsize-1);
			int64_t remaining = count*GetBytesPerSample();
			int readsize=remaining;

			while(remaining){
				readsize = MIN(remaining,blsize - offset);

				memcpy(tmpbuf,(char *)(Cache[i++]+offset),readsize);
				//wxLogStatus(_("i %i, readsize %i, end %i"), i, readsize, end);
				tmpbuf+=readsize;
				offset=0;
				remaining-=readsize;
		
			}
		}
		if (volume == 1.0) return;

	
		// Read raw samples
		short *buffer = (short*) buf;
		int value;

		// Modify
		for (int64_t i=0;i<count;i++) {
			value = (int)(buffer[i]*volume+0.5);
			if (value < -0x8000) value = -0x8000;
			if (value > 0x7FFF) value = 0x7FFF;
			buffer[i] = value;
		}
	
	}
}


void VideoFfmpeg::GetWaveForm(int *min,int *peak,int64_t start,int w,int h,int samples,float scale)
{
	wxMutexLocker lock(blockaudio);
	int n = w * samples;
	for (int i=0;i<w;i++) {
		peak[i] = 0;
		min[i] = h;
	}

	// Prepare waveform
	int cur;
	int curvalue;

	// Prepare buffers
	int needLen = n*GetBytesPerSample();
	
	void *raw;
		raw = new char[needLen];
	

		short *raw_short = (short*) raw;
		GetBuffer(raw,start,n);
		int half_h = h/2;
		int half_amplitude = int(half_h * scale);
		//wxLogStatus("before for");
		// Calculate waveform
		for (int i=0;i<n;i++) {
			cur = i/samples;
			curvalue = half_h - (int(raw_short[i])*half_amplitude)/0x8000;
			if (curvalue > h) curvalue = h;
			if (curvalue < 0) curvalue = 0;
			if (curvalue < min[cur]) min[cur] = curvalue;
			if (curvalue > peak[cur]) peak[cur] = curvalue;
		}
		//wxLogStatus("after for");
	
	delete[] raw;
	//wxLogStatus("del");

}

int VideoFfmpeg::GetSampleRate()
{
	return SampleRate;
}

int VideoFfmpeg::GetBytesPerSample()
{
	return 2;
}

int VideoFfmpeg::GetChannels()
{
	return 1;
}

int64_t VideoFfmpeg::GetNumSamples()
{
	return NumSamples;
}

bool VideoFfmpeg::CacheIt()
{
	//wxLogStatus(_("bufor"));
	progress->Title("Zapisywanie do pami�ci");
	//progress->cancel->Enable(false);
	int64_t end=NumSamples*GetBytesPerSample();
	
	int blsize=(1<<22);
	blnum=((float)end/(float)blsize)+1;
	
	
	
	Cache=NULL;
	Cache=new char*[blnum];
	if(Cache==NULL){wxMessageBox("Za ma�o pami�ci ram");return false;}
	
	int64_t pos=0;
	int halfsize=(blsize/GetBytesPerSample());
	for(int i = 0; i< blnum; i++)
	{
		if(i >= blnum-1){blsize=end-pos; }
		Cache[i]= new char[blsize];
	//wxLogStatus("pos %i, size %i, end %i, pos+size %i",(int)pos, blsize, (int)end, ((int)pos+blsize));
		GetAudio(Cache[i], pos, halfsize);
		pos+=halfsize;
		progress->Progress(((float)i/(float)(blnum-1))*100);
		if(progress->WasCancelled()){blnum=i+1;Clearcache();return false;}
	}
		
	return true;
}



void VideoFfmpeg::Clearcache()
{
	if(!Cache){return;}
	for(int i=0; i<blnum; i++)
	{
		//wxLogStatus("i %i",i);
		delete[ ] Cache[i];
	}
	//wxLogStatus("del cache");
	delete[ ] Cache;
	//wxLogStatus("deleted");
	Cache=0;
	blnum=0;
}

int VideoFfmpeg::TimefromFrame(int nframe)
{
	if(nframe<0){nframe=0;}
	if(nframe>=NumFrames){nframe=NumFrames-1;}
	return Timecodes[nframe];
}

int VideoFfmpeg::FramefromTime(int time)
{
	if(time<=0){return 0;}
	int start=lastframe;
	if(lasttime>time)
	{
		start=0;
	}
	int wframe=NumFrames-1;	
	for(int i=start;i<NumFrames-1;i++)
	{
		if(Timecodes[i]>=time && time<Timecodes[i+1])
		{
			wframe= i;
			break;
		}
	}
	//if(lastframe==wframe){return-1;}
	lastframe=wframe;
	lasttime=time;	
	return lastframe;
}

bool VideoFfmpeg::DiskCache()
{
	progress->Title("Zapisywanie na dysk twardy");
	//progress->cancel->Enable(false);
	progress->Progress(0);
	//char* data;
	bool good=true;
	//if(file_cache.IsOpened()){file_cache.Close();}
	file_cache.Create(diskCacheFilename,true,wxS_DEFAULT);
	file_cache.Open(diskCacheFilename,wxFile::read_write);
	
	int block = 332768;
	//int block2=block*2
	if(Delay>0){
		int size=SampleRate*Delay*2;
		char *silence=new char[size];
		memset(silence,0,size);
		file_cache.Write(silence,size); 
		delete[] silence;
	}
	try {
		char data[332768*2];
		int all=(NumSamples/block)+1;
		int64_t pos=0;
		for (int i=0;i<all; i++) {
			if (block+pos > NumSamples) block = NumSamples - pos;
			//wxLogStatus("i %i block %i nums %i", (int)pos, block, (int)NumSamples);
			GetAudio(&data,pos,block);
			//wxLogStatus("write");
			file_cache.Write(data,block*2);
			//wxLogStatus("Progress");
			pos+=block;
			progress->Progress(((float)pos/(float)(NumSamples))*100);
		if(progress->WasCancelled()){good=false;return false;}
		}
		
		file_cache.Seek(0);
	}
	catch (...) {
		good=false;
	}
	
	if(!good){Cleardiskc();}
	
	return good;
}

void VideoFfmpeg::Cleardiskc()
{
	file_cache.Close();

	//wxRemoveFile(diskCacheFilename);
}

int VideoFfmpeg::GetMSfromFrame(int frame)
{
	return Timecodes[frame];
}
	
int VideoFfmpeg::GetFramefromMS(int MS, int seekfrom)
{
	if (MS<=0) return 0;
	//else if(MS>=Duration) return NumFrames-1;
	int result=NumFrames-1;
	for(int i=seekfrom; i<NumFrames; i++)
	{
		if(Timecodes[i]>=MS)
		{
			result = i;
			break;
		}
	}
	return result;
}
