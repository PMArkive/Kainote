
#pragma once
#include "Hotkeys.h"
#include "OpennWrite.h"
#include "KainoteMain.h"
#include "config.h"
#include <wx/regex.h>
#include <wx/log.h>
#include <wx/msgdlg.h>


Hotkeys::Hotkeys()
{
}

void Hotkeys::FillTable()
{
	keys[WXK_BACK] = _T("Backspace");
	keys[WXK_SPACE] = _T("Space");
	keys[WXK_RETURN] = _T("Enter");
	keys[WXK_TAB] = _T("Tab");
	keys[WXK_PAUSE] = _T("Pause");

	keys[WXK_LEFT] = "Left";
	keys[WXK_RIGHT] = "Right";
	keys[WXK_UP] = "Up";
	keys[WXK_DOWN] = "Down";
	
	keys[WXK_INSERT] = _T("Insert");
	keys[WXK_DELETE] = _T("Delete");
	keys[WXK_HOME] = _T("Home");
	keys[WXK_END] = _T("End");
	keys[WXK_PAGEUP] = _T("PgUp");
	keys[WXK_PAGEDOWN] = _T("PgDn");

	keys[WXK_NUMPAD0] = _T("Num 0");
	keys[WXK_NUMPAD1] = _T("Num 1");
	keys[WXK_NUMPAD2] = _T("Num 2");
	keys[WXK_NUMPAD3] = _T("Num 3");
	keys[WXK_NUMPAD4] = _T("Num 4");
	keys[WXK_NUMPAD5] = _T("Num 5");
	keys[WXK_NUMPAD6] = _T("Num 6");
	keys[WXK_NUMPAD7] = _T("Num 7");
	keys[WXK_NUMPAD8] = _T("Num 8");
	keys[WXK_NUMPAD9] = _T("Num 9");
	keys[WXK_NUMPAD_ADD] = _T("Num +");
	keys[WXK_NUMPAD_SUBTRACT] = _T("Num -");
	keys[WXK_NUMPAD_SEPARATOR] = _T("Num |");
	keys[WXK_NUMPAD_MULTIPLY] = _T("Num *");
	keys[WXK_NUMPAD_DIVIDE] = _T("Num /");
	keys[WXK_NUMPAD_DECIMAL] = _T("Num .");
	keys[WXK_NUMPAD_ENTER] = _T("Num Enter");

	keys[WXK_F1] = _T("F1");
	keys[WXK_F2] = _T("F2");
	keys[WXK_F3] = _T("F3");
	keys[WXK_F4] = _T("F4");
	keys[WXK_F5] = _T("F5");
	keys[WXK_F6] = _T("F6");
	keys[WXK_F7] = _T("F7");
	keys[WXK_F8] = _T("F8");
	keys[WXK_F9] = _T("F9");
	keys[WXK_F10] = _T("F10");
	keys[WXK_F11] = _T("F11");
	keys[WXK_F12] = _T("F12");

	AudioKeys=false;
}

void Hotkeys::LoadDefault(std::map<int, hdata> &_hkeys, bool Audio)
{
	if(!Audio){
		_hkeys[ID_CHANGETIME]= hdata('G', _("Okno zmiany czas�w"),"Ctrl-I");
		_hkeys[ID_ASS]=hdata('G', _("Konwertuj do ASS"), "F9");
		_hkeys[ID_SRT]=hdata('G', _("Konwertuj do SRT"), "F8");
		_hkeys[ID_MDVD]=hdata('G', _("Konwertuj do MDVD"), "F10");
		_hkeys[ID_MPL2]=hdata('G', _("Konwertuj do MPL2"), "F11");
		_hkeys[ID_TMP]=hdata('G', _("Konwertuj do TMP"), "Ctrl-F12");
		_hkeys[ID_STYLEMNGR]=hdata('G', _("Mened�er styl�w"), "Ctrl-M");
		_hkeys[ID_EDITOR]=hdata('G', _("W��cz / Wy��cz edytor"), "Ctrl-E");
		_hkeys[ID_OPVIDEO]=hdata('G', _("Otw�rz wideo"), "Ctrl-Shift-O");
		_hkeys[ID_FIND]=hdata('G', _("Znajd�"), "Ctrl-F");
		_hkeys[ID_FINDREP]=hdata('G', _("Znajd� i zmie�"), "Ctrl-H");
		_hkeys[ID_UNDO1]=hdata('G', _("Cofnij"), "Ctrl-Z");
		_hkeys[ID_REDO1]=hdata('G', _("Pon�w"), "Ctrl-Y");
		_hkeys[ID_OPENSUBS]=hdata('G', _("Otw�rz napisy"), "Ctrl-O");
		_hkeys[ID_SAVE]=hdata('G', _("Zapisz"), "Ctrl-S");
		_hkeys[ID_SAVEAS]=hdata('G', _("Zapisz jako..."), "Ctrl-Shift-S");
		_hkeys[ID_DELETE_TEXT]=hdata('G', _("Usu� tekst"), "Alt-Delete");
		_hkeys[ID_DELETE]=hdata('G', _("Usu� linijk�"), "Shift-Delete");
		_hkeys[ID_SETSTIME]=hdata('G', _("Wstaw czas pocz�tkowy z wideo"), "Alt-Z");
		_hkeys[ID_SETETIME]=hdata('G', _("Wstaw czas ko�cowy z wideo"), "Alt-X");
		_hkeys[ID_PAUSE]=hdata('G', _("Odtwarzaj / Pauza"), "Alt-Space");
		_hkeys[ID_M5SEC]=hdata('G', _("Wideo minus 5 sekund"), "Alt-Left");//Lewo
		_hkeys[ID_P5SEC]=hdata('G', _("Wideo plus 5 sekund"), "Alt-Right");//prawo
		_hkeys[ID_PREVFRAME]=hdata('G', _("Klatka w ty�"), "Alt-A");
		_hkeys[ID_NEXTFRAME]=hdata('G', _("Klatka w prz�d"), "Alt-S");
		_hkeys[ID_PREV_LINE]=hdata('G', _("Poprzednia linijka"), "Ctrl-Up");//g�ra
		_hkeys[ID_NEXT_LINE]=hdata('G', _("Nast�pna linijka"), "Ctrl-Down");//d�
		_hkeys[GRID_JOINWP]=hdata('G', _("Scal z porzedni� linijk�"), "F3");
		_hkeys[GRID_JOINWN]=hdata('G', _("Scal z nast�pn� linijk�"), "F4");
		_hkeys[ID_SNAP_START]=hdata('G', _("Przyklej start do klatki kluczowej"), "Shift-Left");//lewo
		_hkeys[ID_SNAP_END]=hdata('G', _("Przyklej koniec do klatki kluczowej"), "Shift-Right");//prawo
		_hkeys[ID_NEXT_TAB]=hdata('G', _("Nast�pna karta"), "Ctrl-PgDn");
		_hkeys[ID_PREV_TAB]=hdata('G', _("Poprzednia karta"), "Ctrl-PgUp");
		_hkeys[MENU_DUPLICATE]=hdata('N', _("Duplikuj linie"), "Ctrl-D");
		_hkeys[MENU_PASTECOLS]=hdata('N', _("Wklej kolumny"), "Ctrl-Shift-V");
		_hkeys[MENU_PLAYP]=hdata('W', _("Odtwarzaj"), "Space");
		_hkeys[MENU_P5SEC]=hdata('W', _("Plus 5 sekund"), "Right");//prawo
		_hkeys[MENU_M5SEC]=hdata('W', _("Minus 5 sekund"), "Left");//lewo
		_hkeys[MENU_PMIN]=hdata('W', _("Plus minuta"), "Up");
		_hkeys[MENU_MMIN]=hdata('W', _("Minus minuta"), "Down");
		_hkeys[MENU_NEXT]=hdata('W', _("Nast�pny plik"), ".");
		_hkeys[MENU_PREV]=hdata('W', _("Poprzedni plik"), ",");
		_hkeys[MENU_SPLUS]=hdata('W', _("D�wi�k g�o�niej"), "Num .");
		_hkeys[MENU_SMINUS]=hdata('W', _("D�wi�k ciszej"), "Num 0");
		_hkeys[MENU_NEXTCHAP]=hdata('W', _("Nast�pny rozdzia�"), "M");
		_hkeys[MENU_PREVCHAP]=hdata('W', _("Poprzedni rozdzia�"), "N");
		_hkeys[ID_BOLD]=hdata('E', _("Wstaw pogrubienie"), "Alt-W");
		_hkeys[ID_ITAL]=hdata('E', _("Wstaw kursyw�"), "Alt-E");
		_hkeys[ID_SPLIT]=hdata('E', _("Wstaw znak podzia�u"), "Ctrl-N");
		_hkeys[ID_STARTDIFF]=hdata('E', _("Wstaw r�nic� pocz�tkow�"), "Ctrl-Alt-,");
		_hkeys[ID_ENDDIFF]=hdata('E', _("Wstaw r�nic� ko�cow�"), "Ctrl-Alt-.");
	}else{
		_hkeys[Audio_Button_Commit]=hdata('A', _("Zatwierd�"), "Enter");
		_hkeys[Audio_Button_Commit-1000]=hdata('A', _("Zatwierd� zast"), "G");
		_hkeys[Audio_Button_Prev]=hdata('A', _("Poprzednia linijka"), "Left");
		_hkeys[Audio_Button_Prev-1000]=hdata('A', _("Poprzednia linijka zast"), "Z");
		_hkeys[Audio_Button_Next]=hdata('A', _("Nast�pna linijka"), "Right");
		_hkeys[Audio_Button_Next-1000]=hdata('A', _("Nast�pna linijka zast"), "X");
		_hkeys[Audio_Button_Play]=hdata('A', _("Odtwarzaj"), "Down");
		_hkeys[Audio_Button_Play-1000]=hdata('A', _("Odtwarzaj zast"), "S");
		_hkeys[Audio_Button_Stop]=hdata('A', _("Zatrzymaj"), "H");
		_hkeys[Audio_Button_Goto]=hdata('A', _("Przejd� do zaznaczenia"), "F");
		_hkeys[Audio_Button_Play_Before_Mark]=hdata('A', _("Odtwarzaj przed znacznikem"), "Num 0");
		_hkeys[Audio_Button_Play_After_Mark]=hdata('A', _("Odtwarzaj po znaczniku"), "Num .");
		_hkeys[Audio_Button_Play_500ms_First]=hdata('A', _("Odtwarzaj pierwsze 500ms"), "E");
		_hkeys[Audio_Button_Play_500ms_Last]=hdata('A', _("Odtwarzaj ko�cowe 500ms"), "D");
		_hkeys[Audio_Button_Play_500ms_Before]=hdata('A', _("Odtwarzaj 500ms przed"), "Q");
		_hkeys[Audio_Button_Play_500ms_After]=hdata('A', _("Odtwarzaj 500ms po"), "W");
		_hkeys[Audio_Button_Play_To_End]=hdata('A', _("Odtwarzaj do ko�ca"), "T");
		_hkeys[Audio_Button_Leadin]=hdata('A', _("Dodaj wst�p"), "C");
		_hkeys[Audio_Button_Leadout]=hdata('A', _("Dodaj zako�czenie"), "V");
	}
}

Hotkeys::~Hotkeys()
{
	keys.clear();
	hkeys.clear();

}

int Hotkeys::LoadHkeys(bool Audio)
{
	if(keys.empty()){FillTable();}
	OpenWrite ow;
	wxString hkpath = (Audio)? "\\AudioHotkeys.txt" : "\\Hotkeys.txt";
	wxString acctxt = ow.FileOpen(Options.pathfull + hkpath,true);
	//failedowa�o z tak b�achego powodu jak brak �cie�ki
	//trzeba uwa�a� na kolejno��, opcje maj� zdecydowane pierwsze�stwo
	if(acctxt==""){LoadDefault(hkeys,Audio);return 1;}

	wxStringTokenizer hk(acctxt,_T("\n"),wxTOKEN_STRTOK);
	bool checkVer=false;
	if(acctxt.StartsWith("[")){
		wxString token=hk.NextToken();
		token=token.Mid(1,token.Len()-2);
		checkVer=(token==Options.progname);
	}

	int g=0;
	while(hk.HasMoreTokens())
	{
		wxString token=hk.NextToken();
		token.Trim(false);
		token.Trim(true);


		wxString Values=token.AfterFirst(',');
		Values.Trim(false);
		Values.Trim(true);
		wxString Labels=token.BeforeFirst(',');
		Labels.Trim(false);
		Labels.Trim(true);
		if(Values!=""){		
			hkeys[wxAtoi(Labels)]=Values;g++;}		
		if(Labels.StartsWith("Script"))
		{
			lscripts.Add(Labels);
		}
	}
	if(g>10){if(Audio){AudioKeys=true;}return (checkVer)? 1 : 2;}
	LoadDefault(hkeys,Audio);
	if(hkeys.size()>10){return (checkVer)? 1 : 2;}
	return 0;

}

void Hotkeys::SaveHkeys(bool Audio)
{
	wxString Texthk=_T("["+Options.progname+"]\r\n");
	for (std::map<int, hdata>::iterator cur = hkeys.begin();cur != hkeys.end();cur++) {
		if((!Audio && cur->second.Type=='A') || (Audio && !(cur->second.Type=='A')) ) {continue;}
		Texthk<< cur->first << _T(",") << cur->second.Type << " " << cur->second.Name << "=" << cur->second.Accel << _T("\r\n");
	}
	OpenWrite ow;
	wxString hkpath=(Audio)? "\\AudioHotkeys.txt" : "\\Hotkeys.txt";
	ow.FileWrite(Options.pathfull+hkpath, Texthk);

}



wxAcceleratorEntry Hotkeys::GetHKey(int itemid)
{
	wxString accel=hkeys[itemid].Accel;
	wxAcceleratorEntry accelkey;
	if (accel==""){return accelkey;/*ResetKey(itemid);accel=hkeys[itemid];*/}
	int modif=0;
	if(accel.Find("Alt-")!=-1){
		modif|=1;
	}
	if(accel.Find("Shift-")!=-1){
		modif|=4;
	}
	if(accel.Find("Ctrl-")!=-1){
		modif|=2;
	}

	//if(modif==0){accel.Prepend("-");}
	wxString akey=accel.AfterLast('-');
	int key=0;
	//wxLogStatus("\""+akey+"\"");
	std::map<int,wxString>::iterator cur;
	for (cur = keys.begin();cur != keys.end();cur++) {
		if (cur->second == akey) {
			key = cur->first;
			break;
		}
	}

	if(key==0 && akey.Len()<2){key=static_cast<int>(akey[0]);}else if(key==0){wxLogStatus(_("Key \"%s\" nie jest prawid�owy"),akey);}
	//wxLogStatus(accname+" %i",key);

	accelkey.Set(modif,key,itemid);

	return accelkey;
}

void Hotkeys::SetHKey(int id, wxString name,int flag, int key)
{
	wxString strkey;
	if(flag & 1){
		strkey<<"Alt-";}
	if(flag & 2){
		strkey<<"Ctrl-";}
	if(flag & 4){
		strkey<<"Shift-";}

	wxString chkey=keys[key];
	if(chkey==""){chkey=wchar_t(key);}
	if(chkey!=""){strkey<<chkey;

	hkeys[id]=hdata(name[0], name.AfterFirst(' '),strkey);
	}
}

wxString Hotkeys::GetMenuH(int id)
{
	auto it=hkeys.find(id);
	if(it!=hkeys.end()){return it->second.Accel;}
	return "";
}

void Hotkeys::ResetKey(int id)
{
	std::map<int,hdata> tmphkeys;
	LoadDefault(tmphkeys);LoadDefault(tmphkeys, true);
	auto it= tmphkeys.find(id);
	if(it!= tmphkeys.end())
	{
		hkeys[id]=it->second;
	}else{wxLogStatus(_("Nie mo�na przywr�ci� skr�tu, bo nie ma domy�lnego ustawienia o idzie %i"), id);}
}

int Hotkeys::OnMapHkey(int id, wxString name,wxWindow *parent, wxString *windows, int elems)
{
	HkeysDialog hkd(parent,name,false,windows,elems);
	int resitem=-1;
	if(hkd.ShowModal()==0){

		wxString test;
		if(hkd.flag & 1){
			test<<"Alt-";}
		if(hkd.flag & 2){
			test<<"Ctrl-";}
		if(hkd.flag & 4){
			test<<"Shift-";}

		wxString keytxt=keys[hkd.hkey];
		if(keytxt==""){keytxt=wchar_t(hkd.hkey);}
		test<<keytxt;
		//wxLogStatus(test);

		for(std::map< int, hdata >::iterator cur=hkeys.begin(); cur!=hkeys.end(); cur++)
		{
			if(cur->second.Accel == test && (cur->second.Type == hkd.hkname[0]) ){

				if(wxMessageBox(wxString::Format(_("Ten skr�t ju� istnieje jako skr�t do \"%s\"\n.Usun�� powtarzaj�cy si� skr�t?"), cur->second.Name), _("Uwaga"),wxYES_NO)==wxYES)
				{
					hkeys.erase(cur->first);
					resitem=cur->first;
				}else{ return -2;}
			}
		}
		//wxLogStatus("Sethkey");
		Hkeys.SetHKey(id, hkd.hkname, hkd.flag, hkd.hkey);
		//wxLogStatus("Setitem");
	}
	return resitem;
}

wxMenuItem *Hotkeys::SetAccMenu(wxMenu *menu, int id, const wxString &txt, const wxString &help, wxItemKind kind)
{
	wxString hkey=GetMenuH(id);
	wxString mtext=(hkey!="")? txt.BeforeFirst('\t')+"\t"+hkey : txt;
	return menu->Append(id,mtext,help,kind);
}


//Okno dialogowe przechwytuj�ce skr�ty klawiszowe
//blokuj�ce przy okazji dost�p do opcji
HkeysDialog::HkeysDialog( wxWindow *parent, wxString name, bool script, wxString *windows, int elems)
	: wxDialog(parent,-1,_("Mapowanie przycisk�w"),wxDefaultPosition,wxDefaultSize,wxCAPTION|wxWANTS_CHARS|wxCLOSE_BOX)
{
	/*wxChoice *global=NULL;
	if(windows){
	global=new wxChoice(this,-1,wxDefaultPosition,wxDefaultSize,elems, windows,wxWANTS_CHARS);
	global->SetSelection(0);
	global->Connect(wxEVT_KEY_DOWN, (wxObjectEventFunction)&HkeysDialog::OnKeyPress,0,this);
	}*/
	wxStaticText *txt=new wxStaticText(this,-1,wxString::Format(_("Prosz� wcisn�� klawisze skr�tu dla \"%s\"."), name),wxDefaultPosition,wxDefaultSize,wxWANTS_CHARS);
	txt->Connect(wxEVT_KEY_DOWN, (wxObjectEventFunction)&HkeysDialog::OnKeyPress,0,this);

	wxSizer *MainSizer = new wxBoxSizer(wxVERTICAL);
	//if(global){MainSizer->Add(global, 0, wxTOP|wxLEFT|wxRIGHT|wxEXPAND, 12);}
	MainSizer->Add(txt, 0, wxALL, 12);
	MainSizer->SetSizeHints(this);
	SetSizer(MainSizer);
	//CenterOnParent();
	SetPosition(wxGetMousePosition());
	if(elems){hkname<<windows[0][0]<<" ";}
	hkname<<name;
	scr=script;
}

HkeysDialog::~HkeysDialog()
{
}

void HkeysDialog::OnKeyPress(wxKeyEvent& event)
{
	int key = event.GetKeyCode();
	if (key==0){key=event.GetUnicodeKey();}

	if(key!=WXK_SHIFT&&key!=WXK_ALT&&key!=WXK_CONTROL){

		flag=0;
		if(event.AltDown()){flag|=1;}
		if(event.ControlDown()){flag|=2;}
		if(event.ShiftDown()){flag|=4;}

		if(flag==0 && (hkname.StartsWith("G") || hkname.StartsWith("E") || scr ) && (key>30 && key<127 ))//|| key>313 && key<318
		{
			wxMessageBox(_("Skr�ty globalne i edytora musz� zawiera� modyfikatory (np. Shift, Ctrl, Alt)."));return;
		}
		hkey=key;

		EndModal(0);
	}
}


Hotkeys Hkeys;
