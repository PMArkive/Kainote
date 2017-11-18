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


#pragma once

#include <wx/wx.h>
#include <vector>
#include <wx/evtloop.h>
#include <wx/popupwin.h>
#include <wx/msw/popupwin.h>
#include <map>
#include "Hotkeys.h"
class MenuEvent;
class Menu;
class KaiToolbar;
class KaiScrollbar;

wxDECLARE_EVENT(EVT_MENU_OPENED, MenuEvent);

class  MenuEvent : public wxEvent
{
public:
    MenuEvent(wxEventType type = wxEVT_NULL, int winid = 0, Menu* menu = NULL)
        : wxEvent(winid, type)
        { m_menuId = winid; m_menu = menu; }
    MenuEvent(const MenuEvent& event)
        : wxEvent(event)
    { m_menuId = event.m_menuId; m_menu = event.m_menu; }

    // only for wxEVT_MENU_HIGHLIGHT
    int GetMenuId() const { return m_menuId; }

    // only for wxEVT_MENU_OPEN/CLOSE
    bool IsPopup() const { return m_menuId == wxID_ANY; }

    // only for wxEVT_MENU_OPEN/CLOSE
    Menu* GetMenu() const { return m_menu; }

    virtual wxEvent *Clone() const { return new MenuEvent(*this); }

private:
    int     m_menuId;
    Menu* m_menu;

    //DECLARE_DYNAMIC_CLASS_NO_ASSIGN(MenuEvent)
};

class MenuItem
{
	//friend class MenuBar;
public:
	MenuItem(int _id, const wxString& _label, const wxString& _help="", bool _enable = true, wxBitmap *_icon = NULL, Menu *Submenu = 0, byte _type = 0);
	~MenuItem();
	bool Enable(bool enable);
	wxBitmap GetBitmap();
	int GetType(){
		return type;
	}
	wxString GetLabel(){
		return label;
	}
	wxString GetLabelText(){
		wxString desc = label;
		desc.Replace("&","");
		if(desc.find("\t")!= -1){desc= desc.BeforeFirst('\t');}
		return desc;
	}
	wxString GetHelp(){
		return help;
	}
	void Check(bool _check=true){
		check = _check;
	}
	bool IsEnabled(){
		return enabled;
	}
	Menu *GetSubMenu(){
		return submenu;
	}
	int GetId(){
		return id;
	}
	bool IsChecked(){
		return check;
	}

	void SetAccel(wxAcceleratorEntry *entry);
	wxBitmap *icon;
	wxString label;
	wxString help;
	int id;
	byte type;
	bool enabled;
	Menu* submenu;
	bool check;
	wxAcceleratorEntry *accel;
};

class MenuDialog : public wxPopupWindow{
	friend class Menu;
	friend class MenuBar;
public:
	MenuDialog(Menu *parent, wxWindow *DialogParent, const wxPoint &pos, const wxSize &size, bool sendEvent = true);
	~MenuDialog();

private:
	void OnMouseEvent(wxMouseEvent &evt);
	void OnPaint(wxPaintEvent &event);
	void OnScroll(wxScrollEvent& event);
	bool AcceptsFocus() const {return false;};
	bool AcceptsFocusRecursively() const {return false;};
	bool AcceptsFocusFromKeyboard() const {return false;};
	void OnShowSubmenu(wxTimerEvent &evt);
	void OnHideSubmenu(wxTimerEvent &evt);
	int ShowPartialModal();
	void EndPartialModal(int ReturnId);
	bool SendEvent(MenuItem *item, int accel);
	//void OnLostCapture(wxMouseCaptureLostEvent &evt){if(HasCapture()){ReleaseMouse();}};
	//bool CheckMouse();
	//void OnIdle(wxIdleEvent &evt);
	virtual void DoGetPosition(int *x, int *y) const;
	void HideMenus(int id = -3);
	int submenuShown;
	int submenuToHide;
	int sel;
	int scPos;
protected:
	wxBitmap *bmp;
	Menu *parent;
	KaiScrollbar *scroll;
	bool subMenuIsShown;
	static MenuDialog* ParentMenu;
	static MenuDialog* lastActiveMenu;
	virtual WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
	bool isPartialModal;
	int accel;

	wxTimer showSubmenuTimer;
	wxTimer hideSubmenuTimer;
	wxGUIEventLoop *loop;
	DECLARE_EVENT_TABLE()
};

class Mnemonics{
public:
	Mnemonics(){}
	virtual ~Mnemonics(){}
	void findMnemonics(const wxString &label, int position);
	std::map<char, int> mnemonics;
};

class Menu : public Mnemonics
{
	friend class MenuDialog;
	friend class MenuBar;
	public:
	Menu(char window = 0);
	//Menu(const wxString& title);
	virtual ~Menu(){
		Clear();
	};
	MenuItem *AppendTool(KaiToolbar *ktb, int id, wxString text, wxString help, wxBitmap *bitmap, bool enable=true, Menu *SubMenu=0);
	MenuItem *Append(int _id,const wxString& _label, const wxString& _help="", bool _enable = true, wxBitmap *_icon = NULL, Menu* Submenu = NULL, byte _type = 0);
	MenuItem *Append(int _id,const wxString& _label, Menu* Submenu, const wxString& _help="", byte _type = 0, bool _enable = true, wxBitmap *_icon = NULL);
	MenuItem *Append(MenuItem *item);
	MenuItem *Prepend(int _id, const wxString& _label, const wxString& _help="", bool _enable = true, wxBitmap *_icon = NULL, Menu* Submenu = NULL, byte _type = 0);
	MenuItem *Prepend(MenuItem *item);
	MenuItem *Insert(int position, int _id, const wxString& _label, const wxString& _help="", bool _enable = true, wxBitmap *_icon = NULL, Menu* Submenu = NULL, byte _type = 0);
	MenuItem *Insert(int position, MenuItem *item);
	MenuItem *SetAccMenu(int id, const wxString &txt, const wxString &help="", bool enable=true, int kind=0);
	MenuItem *SetAccMenu(MenuItem *menuitem, const wxString &name);
	void Delete(int position);
	void Clear(){
		for(auto cur = items.begin(); cur!= items.end(); cur++){
			delete (*cur);
		}
		items.clear();
	}
	bool Destroy(MenuItem *item);
	int GetMenuItemCount();
	MenuItem *FindItem(int id);
	static MenuItem *FindItemGlobally(int id);
	MenuItem *FindItem(const wxString& label);
	MenuItem *FindItemByPosition(int pos);
	void Check(int id, bool check);
	void AppendSeparator();
	int GetPopupMenuSelection(const wxPoint &pos, wxWindow *parent, int *accels = 0, bool clientPos=true, bool center = false);
	void PopupMenu(const wxPoint &pos, wxWindow *parent, bool clientPos=true, bool center = false);
	void SetMaxVisible(int maxVisible);
	void SetShowIcons(bool showIcons);
	void SetMinWidth(int width);
	void SetTitle(const wxString &_title){title = _title;};
	wxString GetTitle() const {return title;};
	void HideMenu(){dialog->HideMenus();}
	void SelectOnStart(int numitem);
private:
	void CalcPosAndSize(wxWindow *parent, wxPoint *pos, wxSize *size, bool clientPos);
	void DestroyDialog();
	void GetAccelerators(std::vector <wxAcceleratorEntry> *entries);
	std::vector< MenuItem* > items;
	wxString title;
	Menu *parentMenu;

protected:
	char wnd;
	MenuDialog *dialog;
};


class MenuBar : public wxWindow, Mnemonics
{
	friend class Menu;
	friend class MenuDialog;
public:
	MenuBar(wxWindow *parent);
	virtual ~MenuBar();
	void Append(Menu *menu, const wxString &title);
	void Prepend(Menu *menu, const wxString &title);
	void Insert(int position, Menu *menu, const wxString &title);
	MenuItem *FindItem(int id); 
	void Enable(int id, bool enable=true);
	void AppendAccelerators(std::vector <wxAcceleratorEntry> *entries);
private:
	void OnMouseEvent(wxMouseEvent &evt);
	void OnCharHook(wxKeyEvent &evt);
	void OnPaint(wxPaintEvent &event);
	//void OnLostCapture(wxMouseCaptureLostEvent &evt){if(HasCapture()){ReleaseMouse();}};
	//bool CheckMouse();
	int CalcMousePos(wxPoint *pos);
	void HideMnemonics();
	
	std::vector< Menu* > Menus;
	wxBitmap *bmp;
	int sel;
	bool clicked;
	bool altDown;
	wxTimer showMenuTimer;
	int shownMenu;
	int oldelem;
	Menu *md;
	static MenuBar *Menubar;
	HHOOK HookKey;
	static LRESULT CALLBACK OnKey( int code, WPARAM wParam, LPARAM lParam );
	HHOOK HookMouse;
	static LRESULT CALLBACK OnMouseClick( int code, WPARAM wParam, LPARAM lParam );
	DECLARE_EVENT_TABLE()
};

enum{
	ITEM_NORMAL=0,
	ITEM_CHECK,
	ITEM_RADIO,
	ITEM_SEPARATOR,
	ID_CHECK_EVENT=29595
};

