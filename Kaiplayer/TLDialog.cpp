#include "TLDialog.h"

TLDialog::TLDialog(wxWindow *parent, Grid *subsgrid)
	: wxDialog(parent,-1,_("Opcje dopasowywania t�umaczenia"), wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE)
{
	Sbsgrid=subsgrid;

	wxStaticBoxSizer *sizer=new wxStaticBoxSizer(wxVERTICAL,this,_("Przesuwanie tekstu t�umaczenia"));
	wxGridSizer *sizer1 = new wxGridSizer(2,2,2);
	//wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
	//wxBoxSizer *sizer3 = new wxBoxSizer(wxHORIZONTAL);
	Up=new wxButton(this,29995,_("Usu� lini�"));
	Up->SetToolTip(_("Usuwa zaznaczon� linijk�.\nT�umaczenie idzie do g�ry."));
	Down=new wxButton(this,29997,_("Dodaj lini�"));
	Down->SetToolTip(_("Dodaje pust� linijk� przed zaznaczon�.\nT�umaczenie idzie w d�."));
	UpJoin=new wxButton(this,29998,_("Z��cz linie"));
	UpJoin->SetToolTip(_("Z��cza nast�pn� linijk� z zaznaczon�.\nT�umaczenie idzie do g�ry."));
	DownJoin=new wxButton(this,29996,_("Z��cz linie"));
	DownJoin->SetToolTip(_("Z��cza nast�pn� linijk� z zaznaczon�.\nOrygina� idzie w g�r�."));
	DownDel=new wxButton(this,29994,_("Usu� lini�"));
	DownDel->SetToolTip(_("Usuwa zaznaczon� linijk�.\nOrygina� idzie do g�ry."));
	UpExt=new wxButton(this,29993,_("Dodaj lini�"));
	UpExt->SetToolTip(_("Dodaje pust� linijk� przed zaznaczon�.\nOrygina� idzie w d�.\nDodanej linii nale�y ustawi� czasy."));
	
	sizer1->Add(new wxStaticText(this,-1,_("Tekst orygina�u")),0,wxALL|wxEXPAND,5);
	sizer1->Add(new wxStaticText(this,-1,_("Tekst t�umaczenia")),0,wxALL|wxEXPAND,5);
	sizer1->Add(DownJoin,0,wxALL|wxEXPAND,5);
	sizer1->Add(Up,0,wxALL|wxEXPAND,5);
	sizer1->Add(DownDel,0,wxALL|wxEXPAND,5);
	sizer1->Add(Down,0,wxALL|wxEXPAND,5);
	sizer1->Add(UpExt,0,wxALL|wxEXPAND,5);
	sizer1->Add(UpJoin,0,wxALL|wxEXPAND,5);
	
	sizer->Add(sizer1,0,wxEXPAND,0);
	sizer->Add(new wxStaticText(this,-1,"Obja�nienie:\norygina� - tekst napis�w, do kt�rych przek�adamy tekst innych\nt�umaczenie - tekst, kt�ry przek�adamy do timingu wczytanych napis�w"),0,wxEXPAND,0);
	//sizer->Add(sizer3,0,wxEXPAND,0);
	SetSizerAndFit(sizer);
	CenterOnParent();

	Connect(29997,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TLDialog::OnDown);
	Connect(29998,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TLDialog::OnUpJoin);
	Connect(29995,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TLDialog::OnUp);
	Connect(29996,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TLDialog::OnDownJoin);
	Connect(29993,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TLDialog::OnUpExt);
	Connect(29994,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&TLDialog::OnDownDel);
}

TLDialog::~TLDialog()
{

}

void TLDialog::OnDown(wxCommandEvent& event)
{
	Sbsgrid->MoveTextTL(3);
}

void TLDialog::OnUp(wxCommandEvent& event)
{
	Sbsgrid->MoveTextTL(0);
}

void TLDialog::OnUpJoin(wxCommandEvent& event)
{
	Sbsgrid->MoveTextTL(1);
}

void TLDialog::OnDownJoin(wxCommandEvent& event)
{
	Sbsgrid->MoveTextTL(4);
}

void TLDialog::OnUpExt(wxCommandEvent& event)
{
	Sbsgrid->MoveTextTL(2);
}

void TLDialog::OnDownDel(wxCommandEvent& event)
{
	Sbsgrid->MoveTextTL(5);
}