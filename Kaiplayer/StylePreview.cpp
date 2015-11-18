
#include "StylePreview.h"
#include "config.h"

#include "Vsfilterapi.h"


StylePreview::StylePreview(wxWindow *parent, int id, const wxPoint& pos, const wxSize& size)
	: wxWindow(parent, id, pos, size) 
	{
	wxColour kol1=Options.GetColour("Style Preview Color1");
	wxColour kol2=Options.GetColour("Style Preview Color2");
	b=kol1.Blue();
	g=kol1.Green();
	r=kol1.Red();

	b1=kol2.Blue();
	g1=kol2.Green();
	r1=kol2.Red();
	
	bmpframe=NULL;
	instance=NULL;
	//VA=NULL;
    PrevText=NULL;
	vobsub=NULL;
	styl=NULL;
	}
StylePreview::~StylePreview()
{
	if (instance) csri_close(instance);
	if (vobsub) csri_close_renderer(vobsub);//csri_renderer_byname(0,0);
	//wxDELETE(VA);
	wxDELETE(styl);
	wxDELETE(bmpframe);
}

void StylePreview::DrawPreview(Styles *style)
	{
	wxMutexLocker lock(mutex);
	if(style){
		wxDELETE(styl);
		styl=style->Copy();}
	if (instance) csri_close(instance);
	 //wxMessageBox("bitmap filled");
	instance = NULL;
	//wxDELETE(VA);
	wxDELETE(bmpframe);
	//VA=new VobsubApi();

	//csri_rend *renderer=NULL;

	// Select renderer
	//if(!vobsub){
		vobsub = csri_renderer_default();
	//}
	if(!vobsub){wxLogStatus("Csri failed.");return;}

	GetClientSize(&width,&height);
	pitch=width*4;
	std::vector<byte> dat;
	SubsText(dat);
	//if(!VA->OpenMemory( &dat[0], dat.size())){wxMessageBox("Instancja vobsuba nie utworzy�a si�.");return;}
	instance = csri_open_mem(vobsub,&dat[0],dat.size(),NULL);
	if(!instance){
		wxLogStatus("Instancja vobsuba nie utworzy�a si�.");return;}

	unsigned char *data= (unsigned char *)malloc(width * height * 4);
		

	bool ch=false;
	bool ch1=false;
	for(int i=0; i<height; i++)
	{
		if((i%10) == 0){ch1=!ch1;}
		ch=ch1;
		for(int j=0; j<width; j++)
		{
			int k=((i*width)+j)*4;
			if((j%10) == 0 && j>0){ch=!ch;}
			data[k] = (ch)? b : b1;
			data[k+1] = (ch)? g : g1;
			data[k+2] = (ch)? r : r1;
			data[k+3] = 255;
		}
			
	}
		
	csri_frame frame;
	frame.planes[0]=data;
	frame.strides[0]=pitch;
	for(int i=1;i<4;i++){
		frame.planes[i]=NULL;
		frame.strides[i]=NULL;
	}
	frame.pixfmt=CSRI_F_BGR_;

	csri_fmt format;
	format.width = width;
	format.height = height;
	format.pixfmt = frame.pixfmt;
	int error = csri_request_fmt(instance,&format);
	if (error) {wxLogStatus("Request format failed.");return;}

	// Render
	csri_render(instance,&frame,0);
	/*
	SubPicDesc spd;
	spd.w=width;
	spd.h=height;
	spd.type=MSP_RGB32;
	spd.bpp=32;
	spd.pitch=pitch;
	spd.vidrect=CRect(0,0,width,height);
	if(!VA->RequestFormat(spd,25.0f)){wxMessageBox("Request format failed.");return;}
	
	VA->RenderFrame(data,0);*/

	wxImage preview(width, height, true);
	unsigned char *data1= (unsigned char *)malloc(width * height * 3);
	//int bb,gg,rr;
	for(int i=0;i<(width * height);i++)
		{
		data1[i*3]=data[(i*4)+2];
		data1[(i*3)+1]=data[(i*4)+1];
		data1[(i*3)+2]=data[i*4];
		}
	preview.SetData(data1);
	//wxLogStatus("set data");
	bmpframe=new wxBitmap(preview);
	//wxLogStatus("make bitmap");
	Refresh(false);
	//wxLogStatus("refresh");
	free(data);
	data=0;
	}

void StylePreview::OnPaint(wxPaintEvent& event)
	{
	if (!bmpframe) return;

	wxPaintDC dc(this);

	wxMemoryDC memdc;
	memdc.SelectObject(*bmpframe);
	dc.Blit(0, 0, bmpframe->GetWidth(), bmpframe->GetHeight(), &memdc, 0, 0);

	}

void StylePreview::SubsText(std::vector<byte> &buf)
{
	styl->Alignment="5";
	//styl.MarginL="0";
	//styl.MarginR="0";
	//styl.MarginV="0";
	wxString subs((wchar_t)0xFEFF);
	subs<<_T("[Script Info]\r\nPlayResX: ")<<width<<_T("\r\nPlayResY: ")<<height<<_T("\r\nScaledBorderAndShadow: Yes\r\nScriptType: v4.00+\r\nWrapStyle: 0")
         <<_T("\r\n[V4+ Styles]\r\nFormat: Name, Fontname, Fontsize, PrimaryColour, SecondaryColour, OutlineColour, BackColour, Bold, Italic, Underline, StrikeOut, ScaleX, ScaleY, Spacing, Angle, BorderStyle, Outline, Shadow, Alignment, MarginL, MarginR, MarginV, Encoding\r\n")
		 <<styl->styletext()<<_T("\r\n \r\n[Events]\r\nFormat: Layer, Start, End, Style, Name, MarginL, MarginR, MarginV, Effect, Text\r\nDialogue: 0,0:00:00.00,0:01:26.00,")<<styl->Name<<_T(",,0000,0000,0000,,")<<Options.GetString("Preview Text");

		wxScopedCharBuffer buffer= subs.mb_str(wxConvUTF8);
		int size = strlen(buffer);
        buf.clear();
		buf.resize(size);
		memcpy(&buf[0],buffer,size);
		
}

void StylePreview::OnMouseEvent(wxMouseEvent& event)
{
	if(event.LeftDown()){
		if(PrevText){
			Options.SetString("Preview Text",PrevText->GetValue());
			DrawPreview();
			Options.SaveOptions();
			PrevText->Destroy();PrevText=NULL;
		}else{
			wxSize siz=GetClientSize();
			PrevText=new wxTextCtrl(this,-1,Options.GetString("Preview Text"),wxPoint(0,0),wxSize(siz.x,-1));
			PrevText->Show();
		}

	}
}

BEGIN_EVENT_TABLE(StylePreview, wxWindow)
	EVT_PAINT(StylePreview::OnPaint)
	EVT_MOUSE_EVENTS(StylePreview::OnMouseEvent)
END_EVENT_TABLE()
