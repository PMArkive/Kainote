//  Copyright (c) 2018, Marcin Drob

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


#include "MisspellReplacer.h"
#include "OpennWrite.h"
#include "KaiStaticBoxSizer.h"
#include "Tabs.h"


MisspellReplacer::MisspellReplacer(wxWindow *parent)
	:KaiDialog(parent, -1, _("Korekcja drobnych b��d�w"))
{
	DialogSizer *MainSizer = new DialogSizer(wxHORIZONTAL);
	wxBoxSizer *ListSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *PhrasesSizer = new wxBoxSizer(wxHORIZONTAL);
	PutWordBoundary = new KaiCheckBox(this, ID_PUT_WORD_BOUNDARY, _("Wstawiaj automatycznie granice s��w \b"));
	ShowBuiltInRules = new KaiCheckBox(this, ID_SHOW_BUILT_IN_RULES, _("Poka� wbudowane zasady"));
	PhraseToFind = new KaiTextCtrl(this, ID_PHRASE_TO_FIND);
	PhraseToReplace = new KaiTextCtrl(this, ID_PHRASE_TO_REPLACE);
	PhrasesSizer->Add(PhraseToFind, 1, wxALL, 2);
	PhrasesSizer->Add(PhraseToReplace, 1, wxALL, 2);
	RulesList = new KaiListCtrl(this, ID_RULES_LIST,wxDefaultPosition, wxSize(320,-1));
	RulesList->InsertColumn(0, L"", TYPE_TEXT, 20);
	RulesList->InsertColumn(1, _("Opis / szukana fraza"), TYPE_COLOR, 200);
	RulesList->InsertColumn(2, _("Zmieniania fraza"), TYPE_COLOR, 90);
	FillRulesList();
	ListSizer->Add(PhrasesSizer, 0);
	ListSizer->Add(RulesList, 0, wxALL | wxEXPAND, 2);
	ListSizer->Add(PutWordBoundary, 0, wxALL, 2);
	ListSizer->Add(ShowBuiltInRules, 0, wxALL, 2);

	KaiStaticBoxSizer *WhichLinesSizer = new KaiStaticBoxSizer(wxVERTICAL, this, _("Kt�re linijki"));
	wxString choices[] = { _("Wszystkie linijki"), _("Zaznaczone linijki"), _("Od zaznaczonej linijki"), _("Wed�ug wybranych styl�w") };
	WhichLines = new KaiChoice(this, ID_WHICH_LINES_LIST, wxDefaultPosition, wxDefaultSize, 4, choices);
	wxBoxSizer *styleChooseSizer = new wxBoxSizer(wxHORIZONTAL);
	MappedButton *ChooseStylesButton = new MappedButton(this, ID_STYLES_CHOOSE, L"+");
	ChoosenStyles = new KaiTextCtrl(this, -1);
	styleChooseSizer->Add(ChooseStylesButton, 0, wxRIGHT, 2);
	styleChooseSizer->Add(ChoosenStyles, 0);

	WhichLinesSizer->Add(WhichLines, 0);
	WhichLinesSizer->Add(styleChooseSizer, 0);

	wxBoxSizer *ButtonsSizer = new wxBoxSizer(wxVERTICAL);
	//doda� brakuj�ce przyciski i poprawi� opisy.
	MappedButton *AddRuleToList = new MappedButton(this, ID_FIND_RULE, _("Dodaj zasad�"));
	MappedButton *RemoveRuleFromList = new MappedButton(this, ID_FIND_RULE, _("Usu� zasad�"));
	MappedButton *FindRule = new MappedButton(this, ID_FIND_RULE, _("Znajd� b��d"));
	MappedButton *FindRulesOnTab = new MappedButton(this, ID_FIND_ALL_RULES, _("Znajd� b��dy\nw bie��cej zak�adce"));
	MappedButton *FindRulesOnAllTabs = new MappedButton(this, ID_FIND_ALL_RULES_ON_ALL_TABS, _("Znajd� b��dy\nwe wszystkich zak�adkach"));
	MappedButton *ReplaceRule = new MappedButton(this, ID_REPLACE_RULE, _("Zmie� b��d"));
	MappedButton *ReplaceRules = new MappedButton(this, ID_REPLACE_ALL_RULES, _("Zamie� wszystkie b��dy\nw bie��cej zak�adce"));
	MappedButton *ReplaceRulesOnAllTabs = new MappedButton(this, ID_REPLACE_ALL_RULES_ON_ALL_TABS, _("Zamie� wszystkie b��dy\nwe wszystkich zak�adkach"));
	ButtonsSizer->Add(WhichLinesSizer, 0, wxALL, 2);
	ButtonsSizer->Add(AddRuleToList, 0, wxALL, 2);
	ButtonsSizer->Add(RemoveRuleFromList, 0, wxALL, 2);
	ButtonsSizer->Add(FindRule, 0, wxALL, 2);
	ButtonsSizer->Add(FindRulesOnTab, 0, wxALL, 2);
	ButtonsSizer->Add(FindRulesOnAllTabs, 0, wxALL, 2);
	ButtonsSizer->Add(ReplaceRule, 0, wxALL, 2);
	ButtonsSizer->Add(ReplaceRules, 0, wxALL, 2);
	ButtonsSizer->Add(ReplaceRulesOnAllTabs, 0, wxALL, 2);

	MainSizer->Add(ListSizer, 0, wxALL, 2);
	MainSizer->Add(ButtonsSizer, 0, wxALL, 2);

	SetSizerAndFit(MainSizer);
	CenterOnParent();
}

void MisspellReplacer::ReplaceChecked()
{

}

void MisspellReplacer::ShowResult(TabPanel *tab, int keyLine)
{

}

void MisspellReplacer::FillRulesList()
{
	wxString rulesText;
	OpenWrite ow;
	ow.FileOpen(Options.pathfull + L"\\rules.txt", &rulesText);
	if (rulesText.empty())
		return;

	wxStringTokenizer tokenizer(rulesText, "\n", wxTOKEN_STRTOK);
	wxString headertoken = tokenizer.GetNextToken();
	if (headertoken.StartsWith(L"#Kainote rules file"))
		headertoken = tokenizer.GetNextToken();
	wxStringTokenizer tokenizerOnOff(headertoken, "|", wxTOKEN_STRTOK);

	while(tokenizer.HasMoreTokens())
	{
		wxString token = tokenizer.GetNextToken();
		wxString replaceRule;
		wxString findrule = token.BeforeFirst('\f', &replaceRule);
		wxString OnOff = (tokenizerOnOff.HasMoreTokens())? tokenizerOnOff.GetNextToken() : "0";
		rules.push_back(std::make_pair(findrule, replaceRule));

		int row = RulesList->AppendItem(new ItemCheckBox(OnOff == "1", L""));
		RulesList->SetItem(row, 1, new ItemText(findrule));
		RulesList->SetItem(row, 2, new ItemText(replaceRule));
	}
}

void MisspellReplacer::SeekOnce()
{

}

void MisspellReplacer::SeekOnTab(TabPanel *tab, bool seekOnce)
{

}

void MisspellReplacer::SeekOnActualTab()
{

}

void MisspellReplacer::SeekOnAllTabs()
{

}

void MisspellReplacer::ReplaceOnce()
{

}

void MisspellReplacer::ReplaceOnTab(TabPanel *tab, bool seekOnce)
{

}

void MisspellReplacer::ReplaceOnActualTab()
{

}

void MisspellReplacer::ReplaceOnAllTabs()
{

}
