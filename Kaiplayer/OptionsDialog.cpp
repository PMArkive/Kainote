

#include "OptionsDialog.h"
#include "config.h"
#include "kainoteMain.h"
#include "Hotkeys.h"
#include <wx/fontpicker.h>
#include "NumCtrl.h"

ColorButton::ColorButton(wxWindow *parent, int id, const wxColour &col, const wxPoint &pos, const wxSize &size)
	: wxButton(parent, id, "", pos, size)
{
	SetBackgroundColour(col);
	Connect(id,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ColorButton::OnClick);
}

	
wxColour ColorButton::GetColour()
{
	return GetBackgroundColour();
}
	
void ColorButton::OnClick(wxCommandEvent &evt)
{
	wxPoint pos=wxGetMousePosition();
	dcp = DialogColorPicker::Get(this,GetBackgroundColour().GetAsString(wxC2S_HTML_SYNTAX));
	wxPoint mst=wxGetMousePosition();
	int dw, dh;
	wxSize siz=dcp->GetSize();
	siz.x;
	wxDisplaySize (&dw, &dh);
	mst.x-=(siz.x/2);
	mst.x=MID(0,mst.x, dw-siz.x);
	mst.y+=15;
	dcp->Move(mst);
	if (dcp->ShowModal() == wxID_OK) {
		SetBackgroundColour(dcp->GetColor());
	}
}

OptionsDialog::OptionsDialog(wxWindow *parent, kainoteFrame *kaiparent)
	: wxDialog(parent,-1,"Opcje",wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE,"Options")
	{

	OptionsTree= new wxTreebook(this,-1);
	
	Kai=kaiparent;
	Stylelist=NULL;
	Katlist=NULL;

	wxIcon icn;
	icn.CopyFromBitmap(CreateBitmapFromPngResource("SETTINGS"));
	SetIcon(icn);

	wxPanel *Main= new wxPanel(OptionsTree);
	wxPanel *GridColors= new wxPanel(OptionsTree);
	wxPanel *ConvOpt= new wxPanel(OptionsTree);
	wxPanel *Hotkeyss= new wxPanel(OptionsTree);
	wxPanel *AudioMain= new wxPanel(OptionsTree);
	wxPanel *Video= new wxPanel(OptionsTree);
	wxPanel *AudioCols= new wxPanel(OptionsTree);

	hkeymodif=0;
	if(!Options.AudioOpts && !Options.LoadAudioOpts()){wxMessageBox("Dupa blada, opcje si� nie wczyta�y, audio nie skonfigurujesz", "B��dny b��d");}

	//Main
	{
		wxBoxSizer *MainSizer=new wxBoxSizer(wxVERTICAL);
		wxString labels[10]={"Wczytywanie posortowanych napis�w","W��cz sprawdzanie pisowni",
			"Zaznaczaj linijk� z czasem aktywnej\nlinijki poprzedniej zak�adki","Zapisuj napisy z nazw� wideo",
			"Poka� sugestie po dwuktornym klinini�ciu na b��d","Otwieraj napisy zawsze w nowej karcie",
			"Nie przechod� do nast�pnej linii przy edycji czas�w","Zapisuj zmiany po przej�ciu na inn� lini�",
			"Pierwszy kolor podgl�du styli","Drugi kolor podgl�du styli"};
		wxString opts[10]={"Grid Load Sorted","Editbox Spellchecker","Auto Select Lines","Subs Autonaming","Editbox Sugestions On Dclick",
			"Open In New Card","Times Stop On line","Grid save without enter","Style Preview Color1","Style Preview Color2"};
	
		for(int i=0;i<8;i++)
		{
			wxCheckBox *opt=new wxCheckBox(Main,-1,labels[i]);
			opt->SetValue(Options.GetBool(opts[i]));
			ConOpt(opt,opts[i]);
			MainSizer->Add(opt,0,wxALL,2);
		}

		wxString movopts[6]={"Dwukrotnym klikni�ciu na lini� (zawsze w��czone)","Klikni�ciu na lini�","Klikni�ciu na lini� i edycji na pauzie","Klikni�ciu na lini� i edycji na pauzie i odtwarzaniu","Edycji na pauzie","Edycji na pauzie i odtwarzaniu" };
		MainSizer->Add(new wxStaticText(Main,-1,"Przesuwaj wideo do aktualnej linii po:"),0,wxALL,2);
		wxChoice *movvid=new wxChoice(Main,10000,wxDefaultPosition,wxDefaultSize,6,movopts);
		movvid->SetSelection(Options.GetInt("Move Video To Active Line"));
		ConOpt(movvid,"Move Video To Active Line");
		MainSizer->Add(movvid,0,wxALL,2);

		wxString playopts[4]={"Nic","Audio do ko�ca linii","Wideo do ko�ca linii","Wideo do pocz�tku nast�pnej linii"};
		MainSizer->Add(new wxStaticText(Main,-1,"Odtwarzaj po zmianie linii:"),0,wxALL,2);
		wxChoice *playing=new wxChoice(Main,10000,wxDefaultPosition,wxDefaultSize,4,playopts);
		playing->SetSelection(Options.GetInt("Play After Selection"));
		ConOpt(playing,"Play After Selection");
		MainSizer->Add(playing,0,wxALL,2);
	
		wxBoxSizer *MainSizer1=new wxBoxSizer(wxHORIZONTAL);
		wxGridSizer *MainSizer2=new wxGridSizer(2,5,5);
		//uwaga id 20000 ma tylko numctrl, pola tekstowe musza mie� inny id
		NumCtrl *sc = new NumCtrl(Main, 20000, Options.GetString("Offset of start time"), -100000, 100000,true, wxDefaultPosition, wxSize(60,-1), wxTE_PROCESS_ENTER);
		NumCtrl *sc1 = new NumCtrl(Main, 20000, Options.GetString("Offset of end time"), -100000, 100000,true, wxDefaultPosition, wxSize(60,-1), wxTE_PROCESS_ENTER);
		wxTextCtrl *sc2 = new wxTextCtrl(Main, 22001, Options.GetString("Grid tag changing char"), wxDefaultPosition, wxSize(60,-1), wxTE_PROCESS_ENTER);
		NumCtrl *sc3 = new NumCtrl(Main, 20000, Options.GetString("Editbox tag buttons"), 0, 9,true, wxDefaultPosition, wxSize(60,-1), wxTE_PROCESS_ENTER);
		ConOpt(sc,"Offset of start time");
		ConOpt(sc1,"Offset of end time");
		ConOpt(sc2,"Grid tag changing char");
		ConOpt(sc3,"Editbox tag buttons");
		MainSizer2->Add(new wxStaticText(Main,-1,"Op�nienie klatek pocz�tkowych w ms:"),0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,5);
		MainSizer2->Add(sc,0,wxALL,2);
		MainSizer2->Add(new wxStaticText(Main,-1,"Op�nienie klatek ko�cowych w ms:"),0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,5);
		MainSizer2->Add(sc1,0,wxALL,2);
		MainSizer2->Add(new wxStaticText(Main,-1,"Znak podmiany tag�w ASS:"),0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,5);
		MainSizer2->Add(sc2,0,wxALL,2);
		MainSizer2->Add(new wxStaticText(Main,-1,"Ilo�� przycisk�w wstawiaj�cych tagi ASS:"),0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,5);
		MainSizer2->Add(sc3,0,wxALL,2);
		MainSizer->Add(MainSizer2,0,wxALL,2);
	
		wxFontPickerCtrl *optf=new wxFontPickerCtrl(Main,-1,wxFont(Options.GetInt("Grid Font Size"),wxSWISS,wxFONTSTYLE_NORMAL,wxNORMAL,false,Options.GetString("Grid Font Name")));
		ConOpt(optf,"Grid Font");
		MainSizer1->Add(new wxStaticText(Main,-1,"Czcionka ramki z napisami:"),0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,5);
		MainSizer1->Add(optf,0,wxALL,2);
	
		MainSizer->Add(MainSizer1,0,wxLEFT|wxTOP,2);

		wxFlexGridSizer *GridColorsSizer=new wxFlexGridSizer(2,2,wxSize(5,5));
		for(int i=8;i<10;i++){
			ColorButton *optc=new ColorButton(Main,-1,Options.GetString(opts[i]));
			ConOpt(optc,opts[i]);
			GridColorsSizer->Add(new wxStaticText(Main,-1,labels[i]+":"),0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,5);
			GridColorsSizer->Add(optc,0,wxALL,2);
		}
		MainSizer->Add(GridColorsSizer,0,0,0);

		Main->SetSizerAndFit(MainSizer);
	}

		//Grid colors
	{//nale�y uwa�a� by ilo�� linii w grid sizerze si� zgadza�a ile opcji tyle ma by� linii
		wxFlexGridSizer *GridColorsSizer=new wxFlexGridSizer(13,2,wxSize(5,5));
		wxString labels[13]={"Kolor t�a","Kolor t�a dialog�w","Kolor t�a komentarzy","Kolor zaznaczonych dialog�w","Kolor zaznaczonych komentarzy","Kolor tekstu","Kolor nachodz�cych linii","Kolor linii","Kolor obramowania aktywnej linijki","Kolor normaly etykiety","Kolor etykiety modyfikacja","Kolor etykiety zapis","Kolor t�a b��d�w pisowni"};
		wxString opts[13]={"Grid Background","Grid Dialogue","Grid Comment","Grid Selected Dialogue","Grid Selected Comment","Grid Text","Grid Collisions","Grid Lines","Grid Active Line","Grid Label Normal","Grid Label Modified","Grid Label Saved","Grid Spellchecker"};
	
		for(int i=0;i<13;i++)
		{
			ColorButton *optc=new ColorButton(GridColors,-1,Options.GetString(opts[i]));
			ConOpt(optc,opts[i]);
			GridColorsSizer->Add(new wxStaticText(GridColors,-1,labels[i]+":"),0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,5);
			GridColorsSizer->Add(optc,0,wxALL,2);
		}
	

		GridColors->SetSizerAndFit(GridColorsSizer);


	}
		//Ustawienia konwersji
	{
		wxBoxSizer *ConvOptSizer1=new wxBoxSizer(wxVERTICAL);

		wxStaticBoxSizer *obr=new wxStaticBoxSizer(wxHORIZONTAL,ConvOpt,"Wybierz katalog");
		wxStaticBoxSizer *obr0=new wxStaticBoxSizer(wxHORIZONTAL,ConvOpt,"Wybierz styl");
		wxStaticBoxSizer *obr1=new wxStaticBoxSizer(wxHORIZONTAL,ConvOpt,"Wybierz fps");
		wxStaticBoxSizer *obr2=new wxStaticBoxSizer(wxHORIZONTAL,ConvOpt,"Czas w ms na jedn� liter�");
		wxStaticBoxSizer *obr3=new wxStaticBoxSizer(wxHORIZONTAL,ConvOpt,"Tagi wstawiane na pocz�tku ka�dej linijki ass");
		wxStaticBoxSizer *obr4=new wxStaticBoxSizer(wxHORIZONTAL,ConvOpt,"Rozdzielczo�� przy konwersji na ASS");
		wxArrayString styles;
		wxArrayString fpsy;

		

		fpsy.Add(_T("23.976"));fpsy.Add(_T("24"));fpsy.Add(_T("25"));fpsy.Add(_T("29.97"));fpsy.Add(_T("30"));fpsy.Add(_T("60"));

        for(int i = 0;i<2;i++){
			wxString optname=(i==0)? Options.GetString("Default Style Catalog") : Options.GetString("Default Style");
			if(i!=0){
				for(int i = 0; i<Options.StoreSize();i++){
					styles.Add(Options.GetStyle(i)->Name);
				}
			}
			wxChoice *cmb = new wxChoice(ConvOpt, (i==0)?28888 : 28889, wxDefaultPosition, wxSize(200,-1), (i==0)?Options.dirs : styles, wxVSCROLL|wxTE_PROCESS_ENTER);
		
			int sel=cmb->FindString(optname);
			
			if(sel>=0){cmb->SetSelection(sel);if(i==0 && Options.acdir!=optname){Options.LoadStyles(optname);}}
			else{
				if(i==0){sel=cmb->FindString(Options.acdir);}cmb->SetSelection(MAX(0,sel));
				wxString co=(i==0)?"katalog dla stylu" : "styl"; 
				wxMessageBox("Wybrany "+co+" konwersji nie istnieje,\nzostanie on zmieniony na domy�lny","Uwaga!!");}

			ConOpt(cmb,(i==0)? "Default Style Catalog" : "Default Style");
			if(i==0){obr->Add(cmb,0,wxCENTER);ConvOptSizer1->Add(obr,0,wxLEFT);Katlist=cmb;
			Connect(28888,wxEVT_COMMAND_CHOICE_SELECTED,(wxObjectEventFunction)&OptionsDialog::OnChangeCatalog);}
			else{obr0->Add(cmb,0,wxCENTER);ConvOptSizer1->Add(obr0,0,wxLEFT);Stylelist=cmb;}
		}

		wxComboBox *cmb = new wxComboBox(ConvOpt, -1, Options.GetString("Default FPS"), wxDefaultPosition, wxSize(200,-1), fpsy, wxCB_DROPDOWN|wxSUNKEN_BORDER|wxVSCROLL|wxTE_PROCESS_ENTER);
		int sel=cmb->FindString(Options.GetString("Default FPS"));
		if(sel>=0){cmb->SetSelection(sel);}
		else{cmb->SetValue(Options.GetString("Default FPS"));}

	    ConOpt(cmb,"Default FPS");
		obr1->Add(cmb,0,wxCENTER);ConvOptSizer1->Add(obr1,0,wxLEFT);
		

		for(int i=0;i<3;i++)
		{
		wxCheckBox *opt=new wxCheckBox(ConvOpt,-1,(i==0)?"FPS z wideo":(i==1)?"Nowe czasy ko�cowe":"Poka� okno przed konwersj�");
		wxString optname=(i==0)?"FPS from video":(i==1)?"New end times":"Show settings window";
		opt->SetValue(Options.GetBool(optname));
		ConOpt(opt,optname);
		ConvOptSizer1->Add(opt,0,wxALL,2);
		}

		NumCtrl *sc = new NumCtrl(ConvOpt, 20000, Options.GetString("Time show of letter"), 30, 1000, true, wxDefaultPosition, wxSize(250,-1), wxTE_PROCESS_ENTER);
		ConOpt(sc,"Time show of letter");
		obr2->Add(sc,0,wxALL|wxALIGN_CENTER,2);
		ConvOptSizer1->Add(obr2,0,wxALL,2);

		sc = new NumCtrl(ConvOpt, 20000, Options.GetString("Convert Resolution W"), 1, 3000, true, wxDefaultPosition, wxSize(115,-1), wxTE_PROCESS_ENTER);
		ConOpt(sc,"Convert Resolution W");
		obr4->Add(sc,0,wxALL|wxALIGN_CENTER,2);
		
		wxStaticText* txt= new wxStaticText(ConvOpt,-1," X ");
		obr4->Add(txt,0,wxALL|wxALIGN_CENTER,2);

		sc = new NumCtrl(ConvOpt, 20000, Options.GetString("Convert Resolution H"), 1, 3000, true, wxDefaultPosition, wxSize(115,-1), wxTE_PROCESS_ENTER);
		ConOpt(sc,"Convert Resolution H");
		obr4->Add(sc,0,wxALL|wxALIGN_CENTER,2);
		ConvOptSizer1->Add(obr4,0,wxALL,2);
		
		wxTextCtrl *tc = new wxTextCtrl(ConvOpt, -1, Options.GetString("Ass Conversion Prefix"), wxDefaultPosition, wxSize(250,-1),wxTE_PROCESS_ENTER);
		ConOpt(tc,"Ass Conversion Prefix");
		obr3->Add(tc,0,wxALL|wxALIGN_CENTER,2);
		ConvOptSizer1->Add(obr3,0,wxALL,2);

		ConvOpt->SetSizerAndFit(ConvOptSizer1);
	}
	//Video
	{
		wxString voptspl[]={"Otw�rz wideo z linii komend na pe�nym ekranie","Lewy przycisk myszy pauzuje wideo","Otwieraj wideo z czasem aktywnej linii","Preferowane �cie�ki audio pooddzielane �rednikiem"};
		wxString vopts[]={"Video Fullskreen on Start","Video Pause on Click","Open Video At Active Line","Accepted audio stream"};
		wxBoxSizer *MainSizer=new wxBoxSizer(wxVERTICAL);
		for(int i=0;i<3;i++)
		{
			wxCheckBox *opt=new wxCheckBox(Video,-1,voptspl[i]);
			opt->SetValue(Options.GetBool(vopts[i]));
			ConOpt(opt,vopts[i]);
			MainSizer->Add(opt,0,wxALL,2);
		}
		wxStaticBoxSizer *prefaudio=new wxStaticBoxSizer(wxHORIZONTAL,Video,voptspl[3]);
		wxTextCtrl *tc = new wxTextCtrl(Video, -1, Options.GetString(vopts[3]), wxDefaultPosition, wxSize(250,-1),wxTE_PROCESS_ENTER);
		ConOpt(tc,vopts[3]);
		prefaudio->Add(tc,0,wxALL|wxALIGN_CENTER,2);
		MainSizer->Add(prefaudio,0,wxALL,2);

		Video->SetSizerAndFit(MainSizer);
	}
		//Hotkeys
	{
		wxBoxSizer *HkeysSizer=new wxBoxSizer(wxVERTICAL);
		Shortcuts = new wxListCtrl(Hotkeyss,26667,wxDefaultPosition,wxDefaultSize,wxLC_REPORT | wxLC_SINGLE_SEL);
		Shortcuts->InsertColumn(0,_("Funkcja"),wxLIST_FORMAT_LEFT,220);
		Shortcuts->InsertColumn(1,_("Skr�t"),wxLIST_FORMAT_LEFT,120);
		Connect(26667,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,(wxObjectEventFunction)&OptionsDialog::OnMapHkey);
		Connect(26667,wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK,(wxObjectEventFunction)&OptionsDialog::OnResetHkey);		

		if(!Hkeys.AudioKeys && !Hkeys.LoadHkeys(true)){wxMessageBox("Dupa blada, skr�ty klawiszowe nie wczyta�y si�, na audio nie podzia�asz", "B��dny b��d");}
		
		long ii=0;
		//std::map<wxString,wxString>::iterator cur;

		for (auto cur = Hkeys.hkeys.begin();cur != Hkeys.hkeys.end();cur++) {
			wxString name=wxString(cur->second.Type)<<" "<<cur->second.Name;
			long pos = Shortcuts->InsertItem(ii,name);
			Shortcuts->SetItem(pos,1,cur->second.Accel);
			ii++;
		}

		
		HkeysSizer->Add(Shortcuts,1,wxALL|wxEXPAND,2);
		Hotkeyss->SetSizerAndFit(HkeysSizer);
		
	}

		//Audio main

	{
		wxBoxSizer *audio=new wxBoxSizer(wxVERTICAL);

		wxString names[]={"Wy�wietlaj czas przy kursorze","Wy�wietlaj linie sekund","Wy�wietlaj t�o zaznaczenia","Wy�wietlaj pozycj� wideo",
			"Wy�wietlaj klatki kluczowe","Przewijaj spektrum przy odtwarzaniu", "Automatyczne aktywowanie okna audio", "Przyklejaj do Klatek kluczowych",
			"Przyklejaj do pozosta�ych linii","Scalaj wszystkie \"n\" z poprzedni� sylab�","Zapisuj audio do pami�ci RAM"};
		wxString opts[]={"Audio Draw Cursor Time","Audio Draw Secondary Lines","Audio Draw Selection Background","Audio Draw Video Position",
			"Audio Draw Keyframes","Audio Lock Scroll On Cursor","Audio Autofocus","Audio Snap To Keyframes","Audio Snap To Other Lines",
			"Merge Every N With Syllable","Audio RAM Cache"};

		for(int i=0;i<11;i++)
		{
			wxCheckBox *opt=new wxCheckBox(AudioMain,-1,names[i]);
			opt->SetValue(Options.GetBool(opts[i]));
			ConOpt(opt,opts[i]);
			audio->Add(opt,0,wxALL,2);
		}

		wxString opts1[]={"Audio Mark Play Time","Audio Inactive Lines Display Mode"};

		NumCtrl *sc = new NumCtrl(AudioMain, 20000, Options.GetString(opts1[0]), 400, 5000, true, wxDefaultPosition, wxSize(300,-1), 0);
		wxString inact[3]={"Brak","Przed i po aktywnej","Wszystkie widoczne"};
		wxChoice *sc1 = new wxChoice(AudioMain, 10000, wxDefaultPosition, wxSize(300,-1), 3, inact);
		sc1->SetSelection(Options.GetInt(opts1[1]));
		ConOpt(sc,opts1[0]);
		ConOpt(sc1,opts1[1]);
		wxStaticBoxSizer *audiocols=new wxStaticBoxSizer(wxVERTICAL,AudioMain,_("Czas odtwarzania audio przed i po znaczniku w ms"));	
		wxStaticBoxSizer *audiocols1=new wxStaticBoxSizer(wxVERTICAL,AudioMain,_("Spos�b wy�wietlania nieaktywnych linijek"));	
		audiocols->Add(sc,0,wxALL,2);
		audiocols1->Add(sc1,0,wxALL,2);
		audio->Add(audiocols,0,wxALL,2);
		audio->Add(audiocols1,0,wxALL,2);
		

		AudioMain->SetSizerAndFit(audio);
	}

		//Audio colours
	{

		wxFlexGridSizer *AudioColorsSizer=new wxFlexGridSizer(13,2,wxSize(5,5));
		wxString labels[]={"Kolor t�a","Kolor znacznika start","Kolor znacznika koniec","Kolor znacznika przesuwania czas�w",
			"Kolor znacznik�w nieaktywnej linijki","Kolor kursora","Kolor znacznik�w sekund","Kolor zaznaczenia",
			"Kolor zaznaczenia po modyfikacji","Kolor wykresu audio","Kolor nieaktywnego wykresu audio",
			"Kolor zmodyfikowanego wykresu audio","Kolor zaznaczonego wykresu audio"};
		wxString opts[]={"Audio Background","Audio Line Boundary Start","Audio Line Boundary End","Audio Line Boundary Mark",
			"Audio Line Boundary Inactive Line","Audio Play Cursor","Audio Seconds Boundaries","Audio Selection Background",
			"Audio Selection Background Modified","Audio Waveform","Audio Waveform Inactive","Audio Waveform Modified","Audio Waveform Selected"};
	
		for(int i=0;i<13;i++)
		{
			ColorButton *optc=new ColorButton(AudioCols,-1,Options.GetString(opts[i]));
			ConOpt(optc,opts[i]);
			AudioColorsSizer->Add(new wxStaticText(AudioCols,-1,labels[i]+":"),0,wxRIGHT | wxALIGN_CENTRE_VERTICAL,5);
			AudioColorsSizer->Add(optc,0,wxALL,2);
		}
	

		AudioCols->SetSizerAndFit(AudioColorsSizer);
	}
		
	//Adding pages
	OptionsTree->AddPage(Main,"Edytor",true);
	OptionsTree->AddSubPage(GridColors,"Kolorystyka",true);
	OptionsTree->AddSubPage(ConvOpt,"Konwersja",true);
	OptionsTree->AddPage(Video,"Wideo",true);
	OptionsTree->AddPage(AudioMain,"Audio",true);
	OptionsTree->AddSubPage(AudioCols,"Kolorystyka",true);
	OptionsTree->AddPage(Hotkeyss,"Hotkeys",true);
	OptionsTree->Fit();
		
	//adding buttons
	wxBoxSizer *ButtonsSizer=new wxBoxSizer(wxHORIZONTAL);

	okok=new wxButton(this,wxID_OK,"OK");
	wxButton *oknow=new wxButton(this,ID_BCOMMIT,"Zastosuj");
	wxButton *cancel=new wxButton(this,wxID_CANCEL,"Anuluj");

	ButtonsSizer->Add(okok,0,wxRIGHT,2);
	ButtonsSizer->Add(oknow,0,wxRIGHT,2);
	ButtonsSizer->Add(cancel,0,wxRIGHT,2);

	wxBoxSizer *TreeSizer=new wxBoxSizer(wxVERTICAL);
	TreeSizer->Add(OptionsTree,0,wxTOP|wxLEFT,2);
	TreeSizer->Add(ButtonsSizer,0,wxBOTTOM|wxALIGN_CENTER,2);
	SetSizerAndFit(TreeSizer);

	CenterOnParent();

    Connect(ID_BCOMMIT,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&OptionsDialog::OnSaveClick);
        
	wxAcceleratorEntry entries[1];
	entries[0].Set(wxACCEL_NORMAL, WXK_RETURN, wxID_OK);
	wxAcceleratorTable accel(1, entries);
	this->SetAcceleratorTable(accel);

		
}
OptionsDialog::~OptionsDialog()
{
	if(GetReturnCode ()==wxID_OK){
		SetOptions();
		if(hkeymodif==1){Hkeys.SaveHkeys();Kai->SetAccels();}
		else if(hkeymodif==2){
			Hkeys.SaveHkeys(true);
			if(Kai->GetTab()->Edit->ABox){Kai->GetTab()->Edit->ABox->SetAccels();}
		}
	}
	handles.clear();
}

void OptionsDialog::ConOpt(wxControl *ctrl,wxString option)
{
	OptionsBind Obind;
	Obind.ctrl=ctrl;
	Obind.option=option;
	handles.push_back(Obind);
}

void OptionsDialog::OnSaveClick(wxCommandEvent& event)
{
	SetOptions(false);
	if(hkeymodif==1){Hkeys.SaveHkeys();Kai->SetAccels();}
	else if(hkeymodif==2){
		Hkeys.SaveHkeys(true);
		if(Kai->GetTab()->Edit->ABox){Kai->GetTab()->Edit->ABox->SetAccels();}
	}
}

void OptionsDialog::SetOptions(bool saveall)
{
	bool fontmod=false;
	bool colmod=false;
	for(size_t i = 0; i<handles.size(); i++)
	{
		OptionsBind OB=handles[i];

		if(OB.ctrl->IsKindOf(CLASSINFO(wxCheckBox))){
			wxCheckBox *cb=(wxCheckBox*)OB.ctrl;
			if(Options.GetBool(OB.option)!=cb->GetValue()){
				Options.SetBool(OB.option,cb->GetValue());
			}
		}
		else if(OB.ctrl->IsKindOf(CLASSINFO(wxButton))){
			ColorButton *cpc=(ColorButton*)OB.ctrl;
			wxColour kol=cpc->GetColour();
			if(Options.GetColour(OB.option)!=kol){
				Options.SetColour(OB.option,kol);colmod=true;
			}
		}
		else if(OB.ctrl->IsKindOf(CLASSINFO(wxFontPickerCtrl))){
			wxFontPickerCtrl *fpc=(wxFontPickerCtrl*)OB.ctrl;
			wxFont font=fpc->GetSelectedFont();
			wxString fontname=font.GetFaceName();
			int fontsize=font.GetPointSize();
			if(Options.GetString(OB.option+" Name")!=fontname){
				Options.SetString(OB.option+" Name",fontname);fontmod=true;
			}
			if(Options.GetInt(OB.option+" Size")!=fontsize){
				Options.SetInt(OB.option+" Size",fontsize);fontmod=true;
			}
		}
		else if(OB.ctrl->IsKindOf(CLASSINFO(wxComboBox))){
			wxComboBox *cbx=(wxComboBox*)OB.ctrl;
			wxString kol=cbx->GetValue();
			if(Options.GetString(OB.option)!=kol){
				Options.SetString(OB.option,kol);
			}
		}
		else if(OB.ctrl->IsKindOf(CLASSINFO(wxChoice))){
			wxChoice *cbx=(wxChoice*)OB.ctrl;
			if(cbx->GetId()!=10000){
				wxString kol=cbx->GetString(cbx->GetSelection());
				if(Options.GetString(OB.option)!=kol){
					Options.SetString(OB.option,kol);
				}
			}else{
				if(Options.GetInt(OB.option)!=cbx->GetSelection()){
					Options.SetInt(OB.option,cbx->GetSelection());
				}
			}
		}
		else if(OB.ctrl->IsKindOf(CLASSINFO(wxTextCtrl))){
			//wxLogStatus("txtctrl %i",OB.ctrl->GetId());
			if(OB.ctrl->GetId()!=20000){
				wxTextCtrl *sc=(wxTextCtrl*)OB.ctrl;
				wxString str=sc->GetValue();
				if(Options.GetString(OB.option)!=str){
					Options.SetString(OB.option,str);
				}
			}else{
				NumCtrl *sc=(NumCtrl*)OB.ctrl;
				int num=sc->GetInt();
				if(Options.GetInt(OB.option)!=num){
					Options.SetInt(OB.option,num);
				}
			}
		}
	}
	if(fontmod){Kai->GetTab()->Grid1->SetStyle();Kai->GetTab()->Grid1->RepaintWindow();}
	if(colmod){Kai->GetTab()->Grid1->Refresh(false);}
	Options.SaveOptions();
	Options.SaveAudioOpts();
}

void OptionsDialog::OnMapHkey(wxListEvent& event)
{
	int num=event.GetIndex();
	wxListItem item=event.GetItem();
	wxString shkey=item.GetText().AfterLast(' ');
	HkeysDialog hkd(this,shkey,shkey.StartsWith("Script"));
	if(hkd.ShowModal()==0){
	
		wxString test;
		if(hkd.flag & 1){
			test<<"Alt-";}
		if(hkd.flag & 2){
			test<<"Ctrl-";}
		if(hkd.flag & 4){
			test<<"Shift-";}
    
		wxString keytxt=Hkeys.keys[hkd.hkey];
		if(keytxt==""){keytxt=wchar_t(hkd.hkey);}
		test<<keytxt;
		wxLogStatus(test);
		int id=-1;
		for(auto cur=Hkeys.hkeys.begin(); cur!=Hkeys.hkeys.end(); cur++){//wxLogStatus(cur->first);
			if(cur->second.Name == shkey){id=cur->first;}
			if(cur->second.Accel == test && (cur->second.Type == shkey[0]) ){
			
				if(wxMessageBox("Ten skr�t ju� istnieje i jest ustawiony jako skr�t do \""+cur->second.Name+
					"\".\n Wykasowa� powtarzaj�cy si� skr�t?", "Uwaga",wxYES_NO)==wxYES){
					Hkeys.hkeys.erase(cur);
					long nitem=Shortcuts->FindItem(-1,cur->first);
					if(nitem!=-1){
						Shortcuts->SetItem(nitem,1,"");
					}
				
				}else{return;}
			}
		}
		
		if(id<0){return;}
		Hkeys.SetHKey(id, shkey, hkd.flag, hkd.hkey);
		//wxLogStatus("Setitem");
		Shortcuts->SetItem(event.GetIndex(),1,Hkeys.GetMenuH(id));
		//wxLogStatus("Setmodif");
		if(item.GetText().StartsWith("A")){hkeymodif=2;}
		else{hkeymodif=1;}
	
	}
}

void OptionsDialog::OnResetHkey(wxListEvent& event)
{
	
	wxListItem item=event.GetItem();
	int id=-1;
	for(auto cur=Hkeys.hkeys.begin(); cur!=Hkeys.hkeys.end(); cur++){//wxLogStatus(cur->first);
		if(cur->second.Name == item.GetText().AfterFirst(' ')){id=cur->first;}
	}
	if(id<0){return;};
	Hkeys.ResetKey(id);
	Shortcuts->SetItem(event.GetIndex(),1,Hkeys.GetMenuH(id));
	if(item.GetText().StartsWith("A")){hkeymodif=2;}
	else{hkeymodif=1;}
}

void OptionsDialog::OnChangeCatalog(wxCommandEvent& event)
{
	Options.SaveOptions(false);
    Options.LoadStyles(Katlist->GetString(Katlist->GetSelection()));
	Stylelist->Clear();
	for(int i = 0; i<Options.StoreSize();i++){
		Stylelist->Append(Options.GetStyle(i)->Name);
	}
	Stylelist->SetSelection(0);
}


