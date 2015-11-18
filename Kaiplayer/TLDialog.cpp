#include "TLDialog.h"

TLDialog::TLDialog(wxWindow *parent, Grid *subsgrid)
	: wxDialog(parent,-1,"Opcje przesuwania tekstu t�umaczenia", wxDefaultPosition, wxDefaultSize,wxDEFAULT_DIALOG_STYLE)
{
	Sbsgrid=subsgrid;

	wxStaticBoxSizer *sizer=new wxStaticBoxSizer(wxVERTICAL,this,"Przesuwanie tekstu t�umaczenia");
	wxGridSizer *sizer1 = new wxGridSizer(2,2,2);
	//wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
	//wxBoxSizer *sizer3 = new wxBoxSizer(wxHORIZONTAL);
	Up=new wxButton(this,29995,"W g�r� usu� t�umaczenie");
	Up->SetToolTip("Przenosi t�umaczenie w g�r�,\nusuwaj�c przy tym tekst pierwszej zaznaczonej");
	Down=new wxButton(this,29997,"W d� zostaw puste pole");
	Down->SetToolTip("Przenosi t�umaczenie w d�,\npozostawiaj�c puste miejsce");
	UpJoin=new wxButton(this,29998,"W g�r� z��cz t�umaczenie");
	UpJoin->SetToolTip("Przenosi t�umaczenie w g�r�,\nz��cza t�umaczenie zamiast go usuwa�");
	DownJoin=new wxButton(this,29996,"W g�r� z��cz orygina�");
	DownJoin->SetToolTip("Przenosi orygina� w g�r�,\nz��czaj�c pierwsz� zaznaczon� linijk� z nast�pn�");
	DownDel=new wxButton(this,29994,"W g�r� usu� orygina�");
	DownDel->SetToolTip("Przenosi orygina� w g�r�,\nusuwaj�c puste pole");
	UpExt=new wxButton(this,29993,"W d�");
	UpExt->SetToolTip("Przenosi orygina� w d�,\npozostawiaj�c puste pole,\ndodan� linijk� nale�y stimingowa�");
	sizer1->Add(new wxStaticText(this,-1,"Tekst t�umaczenia"),0,wxALL|wxEXPAND,5);
	sizer1->Add(new wxStaticText(this,-1,"Tekst orygina�u"),0,wxALL|wxEXPAND,5);
	sizer1->Add(Up,0,wxALL|wxEXPAND,5);
	sizer1->Add(DownJoin,0,wxALL|wxEXPAND,5);
	sizer1->Add(Down,0,wxALL|wxEXPAND,5);
	sizer1->Add(DownDel,0,wxALL|wxEXPAND,5);
	sizer1->Add(UpJoin,0,wxALL|wxEXPAND,5);
	sizer1->Add(UpExt,0,wxALL|wxEXPAND,5);
	
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