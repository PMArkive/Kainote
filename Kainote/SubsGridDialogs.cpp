﻿//  Copyright (c) 2018 - 2020, Marcin Drob

//  Kainote is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.

//  Kainote is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

//  You should have received a copy of the GNU General Public License
//  along with Kainote.  If not, see <http://www.gnu.org/licenses/>.

#include "SubsGridDialogs.h"
#include "KaiStaticText.h"
#include "MappedButton.h"
#include "KaiMessageBox.h"
#include "KaiStaticBoxSizer.h"

FPSDialog::FPSDialog(wxWindow *parent)
	:KaiDialog(parent, -1, _("Wybierz nowy FPS"))
{
	DialogSizer* siz = new DialogSizer(wxHORIZONTAL);
	wxFlexGridSizer *sizer = new wxFlexGridSizer(2, 2, 2);
	wxArrayString fpsy;
	wxTextValidator valid(wxFILTER_INCLUDE_CHAR_LIST);
	wxArrayString includes;
	includes.Add(L"0");
	includes.Add(L"1");
	includes.Add(L"2");
	includes.Add(L"3");
	includes.Add(L"4");
	includes.Add(L"5");
	includes.Add(L"6");
	includes.Add(L"7");
	includes.Add(L"8");
	includes.Add(L"9");
	includes.Add(L".");
	valid.SetIncludes(includes);

	fpsy.Add(L"23.976"); fpsy.Add(L"24"); fpsy.Add(L"25"); fpsy.Add(L"29.97"); fpsy.Add(L"30"); fpsy.Add(L"60");
	oldfps = new KaiChoice(this, -1, L"", wxDefaultPosition, wxDefaultSize, fpsy, 0, valid);
	oldfps->SetSelection(0);
	newfps = new KaiChoice(this, -1, L"", wxDefaultPosition, wxSize(80, -1), fpsy, 0, valid);
	newfps->SetSelection(2);
	sizer->Add(new KaiStaticText(this, -1, _("FPS napisów")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 4);
	sizer->Add(oldfps, 0, wxEXPAND | wxALL, 4);
	sizer->Add(new KaiStaticText(this, -1, _("Nowy FPS napisów")), 0, wxALIGN_CENTER_VERTICAL | wxALL, 4);
	sizer->Add(newfps, 0, wxEXPAND | wxALL, 4);
	MappedButton *ok = new MappedButton(this, 15555, _("Zmień FPS"));
	Connect(15555, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&FPSDialog::OkClick);
	MappedButton *cancel = new MappedButton(this, wxID_CANCEL, _("Anuluj"));
	sizer->Add(ok, 0, wxEXPAND | wxALL, 4);
	sizer->Add(cancel, 0, wxEXPAND | wxALL, 4);
	siz->Add(sizer, 0, wxEXPAND);
	SetSizerAndFit(siz);
	CenterOnParent();
	SetEnterId(15555);
}

void FPSDialog::OkClick(wxCommandEvent &evt)
{

	if (oldfps->GetValue().ToDouble(&ofps) && newfps->GetValue().ToDouble(&nfps)){
		EndModal(1);
	}
	else{ KaiMessageBox(_("Niewłaściwy fps")); }
}

TreeDialog::TreeDialog(wxWindow *parent, const wxString & currentName)
	:KaiDialog(parent, -1, _("Opis drzewka"))
{
	DialogSizer *mainSizer = new DialogSizer(wxVERTICAL);
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	KaiStaticBoxSizer *descriptionSizer = new KaiStaticBoxSizer(wxHORIZONTAL, this, _("Opis drzewka"));
	treeDescription = new KaiTextCtrl(this, -1, currentName, wxDefaultPosition, wxSize(400, -1));
	treeDescription->SetSelection(0, currentName.Len());
	treeDescription->SetMaxLength(500);
	descriptionSizer->Add(treeDescription, 0, wxEXPAND);
	mainSizer->Add(descriptionSizer, 0, wxEXPAND | wxALL, 2);
	MappedButton *ok = new MappedButton(this, 15555, currentName.empty() ? _("Ustaw nazwę drzewka") : _("Zmień nazwę drzewka"));
	Connect(15555, wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&TreeDialog::OkClick);
	MappedButton *cancel = new MappedButton(this, wxID_CANCEL, _("Anuluj"));
	buttonSizer->Add(ok, 0, wxALL, 4);
	buttonSizer->Add(cancel, 0, wxALL, 4);
	mainSizer->Add(buttonSizer, 0, wxCENTER);
	SetSizerAndFit(mainSizer);
	CenterOnScreen();
	SetEnterId(15555);
}

wxString TreeDialog::GetDescription()
{
	return treeDescription->GetValue();
}

void TreeDialog::OkClick(wxCommandEvent &evt)
{
	wxString newName = treeDescription->GetValue();
	if (!newName.empty())
		EndModal(wxID_OK);
}
