
#include "MappedButton.h"


//w tooltipach nie nale�y ustawia� () bo zostan� usuni�te
MappedButton::MappedButton(wxWindow *parent, int id, const wxString& label, const wxString& toolTip,
             const wxPoint& pos, const wxSize& size, int window, long style)
			 :wxButton(parent, id, label, pos, size, style)
			 ,Window(window)
{
	Bind(wxEVT_LEFT_UP, &MappedButton::OnLeftClick, this);
	SetToolTip(toolTip);
}

MappedButton::MappedButton(wxWindow *parent, int id, const wxBitmap& bitmap, const wxPoint& pos,
            const wxSize& size, int window, long style)
			:wxButton(parent, id, "", pos, size, style)
			,Window(window)
{
	Bind(wxEVT_LEFT_UP, &MappedButton::OnLeftClick, this);
	SetBitmap(bitmap);
}

MappedButton::~MappedButton()
{
}

void MappedButton::SetToolTip(const wxString &_toolTip, bool twoHotkeys)
{
	idAndType itype(GetId(), Window);
	wxString key = Hkeys.GetMenuH(itype);
	if(twoHotkeys){
		idAndType itype(GetId()-1000, Window);
		key += _(" lub ") + Hkeys.GetMenuH(itype);
	}

	wxString toolTip = (_toolTip=="")? GetToolTipText().BeforeFirst('(').Trim() : _toolTip;
	if(key!="")
	{
		toolTip = toolTip + " ("+key+")";
	}
	wxButton::SetToolTip(toolTip);
}
	
void MappedButton::OnLeftClick(wxMouseEvent &evt)
{
	if(evt.ShiftDown()){
		//upewnij si�, �e da si� zmieni� idy na nazwy, 
		//mo�e i troch� spowolni operacj� ale sko�czy si� ci�g�e wywalanie hotkeys�w
		//mo�e od razu funkcji onmaphotkey przekaza� item by zrobi�a co trzeba
		wxString buttonName = (GetLabelText()!="")? GetLabelText() : GetToolTipText().BeforeFirst('(').Trim();
		Hkeys.OnMapHkey( GetId(), buttonName, this, Window, false);
		SetToolTip();
		Hkeys.SetAccels(true);
		Hkeys.SaveHkeys();
		
		return;
	}
	evt.Skip();
}