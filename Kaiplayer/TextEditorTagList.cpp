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

#include "TextEditorTagList.h"
#include "KaiScrollbar.h"
#include "config.h"

const int maxVisible = 15;

PopupTagList::PopupTagList(wxWindow *DialogParent)
: wxPopupWindow(DialogParent)
, sel(0)
, scPos(0)
, scroll(NULL)
, orgY(0)
, bmp(NULL)
, Parent(DialogParent)
{
	int fw = 0;
	SetFont(DialogParent->GetFont());
	GetTextExtent("#TWFfGH", &fw, &height);
	height += 6;
	Bind(wxEVT_MOTION, &PopupTagList::OnMouseEvent, this);
	Bind(wxEVT_LEFT_UP, &PopupTagList::OnMouseEvent, this);
	Bind(wxEVT_MOUSEWHEEL, &PopupTagList::OnMouseEvent, this);
	Bind(wxEVT_PAINT, &PopupTagList::OnPaint, this);
	Bind(wxEVT_SCROLL_THUMBTRACK, &PopupTagList::OnScroll, this);
	Bind(wxEVT_MOUSE_CAPTURE_LOST, &PopupTagList::OnLostCapture, this);
}

PopupTagList::~PopupTagList()
{
	wxDELETE(bmp);
}

void PopupTagList::Popup(const wxPoint &pos, const wxSize &controlSize, int selectedItem)
{
	SetSelection(selectedItem);
	wxPoint npos = pos;//Parent->ClientToScreen(pos);
	wxSize size;
	CalcPosAndSize(&npos, &size, controlSize);
	SetPosition(npos);
	SetSize(size);
	orgY = size.y;
	Show();
	
	if (scroll){
		scroll->SetSize(size.x - 18, 1, 17, size.y - 2);
	}
}

void PopupTagList::CalcPosAndSize(wxPoint *pos, wxSize *size, const wxSize &controlSize)
{
	int tx = 0, ty = 0;
	size_t isize = itemsList.size();
	wxString items;
	for (size_t i = 0; i < isize; i++){
		itemsList[i]->GetTagText(&items);
		GetTextExtent(items, &tx, &ty);
		if (tx > size->x){ size->x = tx; }
	}

	size->x += 18;
	if (isize > (size_t)maxVisible) { size->x += 20; isize = maxVisible; }
	if (size->x > 400){ size->x = 400; }
	if (size->x < controlSize.x){ size->x = controlSize.x; }
	size->y = height * isize + 2;
	wxPoint ScreenPos = Parent->ClientToScreen(*pos);
	wxRect workArea = GetMonitorRect(0, NULL, ScreenPos, true);
	int h = workArea.height + workArea.y;
	if ((ScreenPos.y + size->y) > h){
		pos->y -= (size->y + controlSize.y);
	}
}

void PopupTagList::OnMouseEvent(wxMouseEvent &evt)
{
	int x = evt.GetX();
	int y = evt.GetY();
	wxSize sz = GetClientSize();

	if (evt.GetWheelRotation() != 0) {
		int step = 3 * evt.GetWheelRotation() / evt.GetWheelDelta();
		scPos -= step;
		if (scPos<0){ scPos = 0; }
		else if (scPos >(int)itemsList.size() - maxVisible){ scPos = itemsList.size() - maxVisible; }
		Refresh(false);
		return;
	}

	int elem = y / height;
	elem += scPos;
	if (elem >= (int)itemsList.size() || elem < 0 || x < 0 || x > sz.x || y <0 || y > sz.y){ return; }
	if (elem != sel){
		if (elem >= scPos + maxVisible || elem < scPos){ return; }
		sel = elem;
		Refresh(false);
	}
	if (evt.LeftUp()){
		wxCommandEvent evt(wxEVT_COMMAND_CHOICE_SELECTED, GetId());
		this->ProcessEvent(evt);
	}

	//evt.Skip();
}

void PopupTagList::OnPaint(wxPaintEvent &event)
{
	int w = 0;
	int h = 0;
	GetClientSize(&w, &h);
	if (w == 0 || h == 0){ return; }
	int itemsize = itemsList.size();
	if (scPos >= itemsize - maxVisible){ scPos = itemsize - maxVisible; }
	if (scPos < 0){ scPos = 0; }
	int maxsize = itemsize;
	int ow = w;
	if (itemsize > maxVisible){
		maxsize = maxVisible;
		if (!scroll){
			scroll = new KaiScrollbar(this, -1, wxPoint(w - 18, 1), wxSize(17, h - 2), wxVERTICAL);
			scroll->SetScrollRate(3);
		}
		scroll->SetScrollbar(scPos, maxVisible, itemsize, maxVisible - 1);
		w -= 18;
	}

	wxMemoryDC tdc;
	if (bmp && (bmp->GetWidth() < ow || bmp->GetHeight() < h)) {
		delete bmp;
		bmp = NULL;
	}
	if (!bmp){ bmp = new wxBitmap(ow, h); }
	tdc.SelectObject(*bmp);
	const wxColour & text = Options.GetColour(WindowText);

	tdc.SetFont(GetFont());
	tdc.SetBrush(wxBrush(Options.GetColour(MenuBackground)));
	tdc.SetPen(wxPen(Options.GetColour(WindowBorder)));
	tdc.DrawRectangle(0, 0, ow, h);
	//tdc.SetTextForeground(Options.GetColour("Menu Bar Border Selection"));
	for (int i = 0; i < maxsize; i++)
	{
		int scrollPos = i + scPos;

		if (scrollPos == sel){
			tdc.SetPen(wxPen(Options.GetColour(MenuBorderSelection)));
			tdc.SetBrush(wxBrush(Options.GetColour(MenuBackgroundSelection)));
			tdc.DrawRectangle(2, (height*i) + 2, w - 4, height - 2);
		}
		wxString desc;
		itemsList[scrollPos]->GetTagText(&desc);
		if (desc.Len() > 1000)
			desc = desc.Mid(0, 1000);

		tdc.SetTextForeground(text);
		tdc.DrawText(desc, 4, (height*i) + 3);
	}

	wxPaintDC dc(this);
	dc.Blit(0, 0, ow, h, &tdc, 0, 0);
}

void PopupTagList::SetSelection(int pos){
	sel = pos;
	scPos = pos;
	if (sel < scPos && sel != -1){ scPos = sel; }
	else if (sel >= scPos + maxVisible && (sel - maxVisible + 1) >= 0){ scPos = sel - maxVisible + 1; }
	Refresh(false);
};

void PopupTagList::OnScroll(wxScrollEvent& event)
{
	int newPos = event.GetPosition();
	if (scPos != newPos) {
		scPos = newPos;
		Refresh(false);
	}
}


void PopupTagList::InitList()
{
	itemsList.push_back(new TagListItem(L"1a", _("Przezroczysto�� koloru podstawowego"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"2a", _("Przezroczysto�� koloru pomocniczego"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"3a", _("Przezroczysto�� koloru obw�dki"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"4a", _("Przezroczysto�� koloru cienia"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"1c", _("Kolor podstawowy"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"2c", _("Kolor pomocniczy"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"3c", _("Kolor obw�dki"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"4c", _("Kolor cienia"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"1img", _("Maska PNG pierwszego koloru"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"2img", _("Maska PNG drugiego koloru"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"3img", _("Maska PNG trzeciego koloru"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"4img", _("Maska PNG czwartego koloru"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"1va", _("Gradient przezroczysto�ci koloru podstawowego"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"2va", _("Gradient przezroczysto�ci koloru pomocniczego"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"3va", _("Gradient przezroczysto�ci koloru obw�dki"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"4va", _("Gradient przezroczysto�ci koloru cienia"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"1vc", _("Gradient koloru podstawowego"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"2vc", _("Gradient koloru pomocniczego"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"3vc", _("Gradient koloru obw�dki"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"4vc", _("Gradient koloru cienia"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"a", _("Po�o�enie tekstu (uk�ad SSA)"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"alpha", _("Przezroczysto�� ca�ego tekstu"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"an", _("Po�o�enie tekstu"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"b", _("Pogrubienie tekstu"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"be", _("Rozmycie kraw�dzi"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"blur", _("Rozmycie"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"bord", _("Grubo�� obw�dki"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"clip", _("Wyciknki wektorowe / prostok�tne"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"distort", _("Deformacja czcionki"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"fad", _("Pojawianie / znikanie tekstu"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"fade", _("Pojawianie / znikanie tekstu (zaawansowana wersja)"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"fax", _("Pochylenie w osi X"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"fay", _("Pochylenie w osi Y"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"fe", _("Kodowanie znak�w tekstu"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"fn", _("Nazwa czcionki"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"frs", _("Zaokr�glenie tekstu"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"frx", _("Obr�t w osi X"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"fry", _("Obr�t w osi Y"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"frz", _("Obr�t w osi Z"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"fs", _("Wielko�� czcionki"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"fsc", _("Skala w osi X i Y"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"fscx", _("Skala w osi X"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"fscy", _("Skala w osi Y"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"fsp", _("Odst�py mi�dzy znakami"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"fsvp", _("Odst�p mi�dzy znakami w pionie"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"i", _("Pochylenie tekstu"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"iclip", _("Odwr�cone wycinki wektorowe / prostok�tne"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"jitter", _("Trz�sienie tekstu"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"k", _("Timing karaoke"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"K", _("Timing karaoke p�ynne przej�cie"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"ko", _("Timing karaoke obw�dka"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"kt", _("Timing karaoke??"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"move", _("Ruch tekstu"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"mover", _("Ruch po okr�gu"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"moves3", _("Ruch po krzywej (3 punkty)"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"moves4", _("Ruch po krzywej (4 punkty)"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"movevc", _("Ruch rysunku wektorowego"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"org", _("Kotwica dla obrot�w"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"p", _("Rysunek wektorowy i skala"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"pbo", _("Przesuni�cie punkt�w wektora w osi Y??"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"pos", _("Pozycja tekstu"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"q", _("Spos�b �amania linii"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"r", _("Reset tag�w"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"rnd", _("Losowo�� punkt�w czcionki"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"rnds", _("Losowo�� punkt�w czcionki??"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"rndx", _("Losowo�� punkt�w czcionki o� X"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"rndy", _("Losowo�� punkt�w czcionki o� Y"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"rndz", _("Losowo�� punkt�w czcionki o� Z"), TYPE_TAG_VSFILTER_MOD));
	itemsList.push_back(new TagListItem(L"s", _("Przekre�lenie tekstu"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"shad", _("Cie� tekstu"), TYPE_TAG_USED_IN_VISUAL));
	itemsList.push_back(new TagListItem(L"t", _("Animacja tekstu"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"u", _("Podkre�lenie tekstu"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"xbord", _("Obw�dka w osi X"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"ybord", _("Obw�dka w osi Y"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"xshad", _("Cie� w osi X"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"yshad", _("Cie� w osi Y"), TYPE_NORMAL));
	itemsList.push_back(new TagListItem(L"z", _("koordynata Z dla tag�w frx i fry"), TYPE_NORMAL));
}

void PopupTagList::OnKeyPress(wxKeyEvent &event)
{
	if (event.GetKeyCode() == WXK_RETURN){
		wxCommandEvent evt(wxEVT_COMMAND_CHOICE_SELECTED, GetId());
		this->ProcessEvent(evt);
	}
	else if (event.GetKeyCode() == WXK_ESCAPE){
		
	}
	else if (event.GetKeyCode() == WXK_UP || event.GetKeyCode() == WXK_DOWN){
		int step = (event.GetKeyCode() == WXK_DOWN) ? 1 : -1;
		sel += step;
		scPos += step;
		if (sel >= (int)itemsList.size()){
			sel = 0;
			scPos = 0;
		}
		else if (sel < 0){
			sel = itemsList.size() - 1;
			scPos = sel;
		}
		Refresh(false);
	}
}


