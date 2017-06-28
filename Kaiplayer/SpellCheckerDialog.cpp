//  Copyright (c) 2016, Marcin Drob

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


#include "SpellCheckerDialog.h"
#include "KaiStaticText.h"
#include "SpellChecker.h"
#include "KainoteMain.h"
#include "KaiMessageBox.h"
#include <regex>

SpellCheckerDialog::SpellCheckerDialog(kainoteFrame *parent)
	:KaiDialog((wxWindow*)parent,-1, _("Sprawdzanie pisowni"))
	,Kai(parent)
	,lastLine(0)
	,lastMisspell(0)
	,lastActiveLine(-1)
{
	DialogSizer *main = new DialogSizer(wxVERTICAL);
	wxBoxSizer *misspellSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *replaceSizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *buttonSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *listSizer = new wxBoxSizer(wxHORIZONTAL);
	wxString misspellWord = FindNextMisspell();
	misSpell = new KaiTextCtrl(this,-1,misspellWord);
	replaceWord = new KaiTextCtrl(this,-1);
	misspellSizer->Add(new KaiStaticText(this,-1, _("B��dne s�owo:")), 3, wxEXPAND|wxALL, 2);
	misspellSizer->Add(misSpell, 3, wxEXPAND|wxALL, 2);
	replaceSizer->Add(new KaiStaticText(this,-1, _("Zmie� na:")), 3, wxEXPAND|wxALL, 2);
	replaceSizer->Add(replaceWord, 3, wxEXPAND|wxALL, 2);
	suggestionsList = new KaiListCtrl(this, ID_SUGGESTIONS_LIST, SpellChecker::Get()->Suggestions(misspellWord));
	ignoreComments = new KaiCheckBox(this, -1, _("Ignoruj komentarze"));
	ignoreUpper = new KaiCheckBox(this, -1, _("Ignoruj s�owa z duzej litery"));
	replace = new MappedButton(this, ID_REPLACE, _("Zamie�"));
	replaceAll = new MappedButton(this, ID_REPLACE_ALL, _("Zamie� wszystko"));
	ignore = new MappedButton(this, ID_IGNORE, _("Ignoruj"));
	ignoreAll = new MappedButton(this, ID_IGNORE_ALL, _("Ignoruj wszystko"));
	addWord = new MappedButton(this, ID_ADD_WORD, _("Dodaj do s�ownika"));
	removeWord = new MappedButton(this, ID_REMOVE_WORD, _("Usu� ze s�ownika"));
	removeWord->Enable(false);
	close = new MappedButton(this, ID_CLOSE_DIALOG, _("Zamknij"));
	buttonSizer->Add(ignoreComments, 0, wxEXPAND|wxALL, 2);
	buttonSizer->Add(ignoreUpper, 0, wxEXPAND|wxALL, 2);
	buttonSizer->Add(replace, 0, wxEXPAND|wxALL, 2);
	buttonSizer->Add(replaceAll, 0, wxEXPAND|wxALL, 2);
	buttonSizer->Add(ignore, 0, wxEXPAND|wxALL, 2);
	buttonSizer->Add(ignoreAll, 0, wxEXPAND|wxALL, 2);
	buttonSizer->Add(addWord, 0, wxEXPAND|wxALL, 2);
	buttonSizer->Add(removeWord, 0, wxEXPAND|wxALL, 2);
	buttonSizer->Add(close, 0, wxEXPAND|wxALL, 2);
	listSizer->Add(suggestionsList, 2, wxEXPAND|wxALL, 2);
	listSizer->Add(buttonSizer, 1, wxEXPAND|wxALL, 2);
	main->Add(misspellSizer, 0, wxEXPAND);
	main->Add(replaceSizer, 0, wxEXPAND);
	main->Add(listSizer, 0, wxEXPAND);
	SetSizerAndFit(main);
	CenterOnParent();
	Show();

	Bind(LIST_ITEM_DOUBLECLICKED, &SpellCheckerDialog::OnSelectSuggestion, this, ID_SUGGESTIONS_LIST);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SpellCheckerDialog::Replace, this, ID_REPLACE);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SpellCheckerDialog::ReplaceAll, this, ID_REPLACE_ALL);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SpellCheckerDialog::Ignore, this, ID_IGNORE);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SpellCheckerDialog::IgnoreAll, this, ID_IGNORE_ALL);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SpellCheckerDialog::AddWord, this, ID_ADD_WORD);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SpellCheckerDialog::RemoveWord, this, ID_REMOVE_WORD);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED, [=](wxCommandEvent &evt){
		Destroy();
	}, ID_REMOVE_WORD);
	SetEscapeId(ID_REMOVE_WORD);
}

wxString SpellCheckerDialog::FindNextMisspell()
{
	//jako� zidentyfikowa�, �e u�yszkodnik niczego nie zmieni�
	bool noComments = ignoreComments->GetValue();
	errors.clear();
	TabPanel *tab = Kai->GetTab();
	if(lastActiveLine != tab->Edit->ebrow){
		lastLine = lastActiveLine = tab->Edit->ebrow;
		lastMisspell = 0;
	}
	for(int i = lastLine; i < tab->Grid1->GetCount(); i++){
		Dialogue *Dial = tab->Grid1->GetDial(i);
		if(Dial->IsComment && noComments){continue;}
		wxString &Text = (tab->Grid1->transl)? Dial->TextTl : Dial->Text;
		//w checktext kopiuje tekst wi�c nie musz� robi� tego dwa razy.
		tab->Grid1->CheckText(Text, errors);
		if(i != lastLine){lastMisspell=0;}
		if(errors.size()>1 && lastMisspell < errors.size()){
			wxString misspellWord = Text.SubString(errors[lastMisspell], errors[lastMisspell+1]);
			lastMisspell += 2;
			if(ignored.Index(misspellWord) == -1){
				return misspellWord;
			}
		}
	}
	return "";
}
	
void SpellCheckerDialog::SetNextMisspell()
{
	wxString misspellWord = FindNextMisspell();
	misSpell->SetValue(misspellWord,true);
	if(misspellWord.IsEmpty()){
		//mamy brak b��d�w trzeba poinformowa� u�yszkodnika i zablokowa� jakiekolwiek akcje.
		suggestionsList->SetTextArray(wxArrayString());
		KaiMessageBox(_("Nie znaleziono wi�cej b��d�w pisowni"), _("Uwaga"), wxOK, this);
		return;
	}else{
		suggestionsList->SetTextArray(SpellChecker::Get()->Suggestions(misspellWord));
	}
	if(lastActiveLine != lastLine){
		TabPanel *tab = Kai->GetTab();
		tab->Edit->SetLine(lastLine);
		tab->Edit->TextEdit->SetSelection(errors[lastMisspell], errors[lastMisspell+1]-1);

	}
}
	
void SpellCheckerDialog::Replace(wxCommandEvent &evt)
{
	wxString replaceTxt = replaceWord->GetValue();
	if(replaceTxt.IsEmpty() || errors.size()<2){return;}
	TabPanel *tab = Kai->GetTab();
	Dialogue *Dial = tab->Grid1->CopyDial(lastLine);
	wxString &Text = (tab->Grid1->transl)? Dial->TextTl : Dial->Text;
	Text.replace(errors[lastMisspell], errors[lastMisspell+1]-1, replaceTxt);
	SetNextMisspell();
}
	
void SpellCheckerDialog::ReplaceAll(wxCommandEvent &evt)
{
	wxString replaceTxt = replaceWord->GetValue();
	wxString misspellTxt = misSpell->GetValue();
	if(replaceTxt.IsEmpty() || misspellTxt.IsEmpty()){return;}
	TabPanel *tab = Kai->GetTab();
	bool noComments = ignoreComments->GetValue();
	std::wregex r("\\b" + misspellTxt.ToStdWstring() + "\\b"); // the pattern \b matches a word boundary
	std::wsmatch m;
	std::wstring text;
	int lenMismatch = 0;
	int textPos = 0;
	
	for(int i = 0; i < tab->Grid1->GetCount(); i++){
		Dialogue *Dial = tab->Grid1->GetDial(i);
		if(Dial->IsComment && noComments){continue;}
		wxString &Text = (tab->Grid1->transl)? Dial->TextTl : Dial->Text;
		text = Text.ToStdWstring();
		while(std::regex_search(text, m, r)) {
			int pos = m.position(0) + textPos;
			int len = m.length(0);
			//zr�b co� z tymi danymi
			wxString misspellToReplace = Text.Mid(pos - lenMismatch, len);
			Text.replace(pos - lenMismatch, len, GetRightCase(replaceTxt, misspellToReplace));
			lenMismatch += (len - replaceTxt.Len());
			text = m.suffix().str();
			text.insert(0, L" ");
			textPos = pos + len;
		}
	}
	SetNextMisspell();
}
	
void SpellCheckerDialog::Ignore(wxCommandEvent &evt)
{
	SetNextMisspell();
}
	
void SpellCheckerDialog::IgnoreAll(wxCommandEvent &evt)
{
	ignored.Add(misSpell->GetValue());
	SetNextMisspell();
}
	
void SpellCheckerDialog::AddWord(wxCommandEvent &evt)
{
	SpellChecker::Get()->AddWord(misSpell->GetValue());
	SetNextMisspell();
}
	
void SpellCheckerDialog::RemoveWord(wxCommandEvent &evt)
{
	// na razie shithappens, bo nie wiem jeszcze co z tym fantem zrobi�

}

void SpellCheckerDialog::OnSelectSuggestion(wxCommandEvent &evt)
{
	int sel = evt.GetInt();
	Item *item = suggestionsList->GetItem(sel, 0);
	if(item){
		replaceWord->SetValue(item->name, true);
	}
}

wxString SpellCheckerDialog::GetRightCase(const wxString &replaceWord, const wxString &misspellWord)
{
	wxString firstCharacterR = replaceWord.Mid(0, 1);
	wxString firstCharacterM = misspellWord.Mid(0, 1);
	wxString firstCharacterRL = firstCharacterR.Lower();
	wxString firstCharacterML = firstCharacterM.Lower();
	bool replaceIsLower = (firstCharacterR == firstCharacterRL);
	bool misspellIsLower = (firstCharacterM == firstCharacterML);

	if(replaceIsLower != misspellIsLower){
		if(misspellIsLower){
			return firstCharacterRL + replaceWord.Mid(1);
		}else{
			return firstCharacterR.Upper() + replaceWord.Mid(1);
		}
	}
	return replaceWord;
}