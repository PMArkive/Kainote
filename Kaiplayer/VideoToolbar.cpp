#include "VideoToolbar.h"
#include "Config.h"


static int startDrawPos = 5;
std::vector< itemdata *> VideoToolbar::icons;

VideoToolbar::VideoToolbar (wxWindow *parent, const wxPoint &pos)
	:wxWindow(parent, -1, pos, wxSize(-1, 22))
	,Toggled(0)
	,clipToggled(toolsSize+1)
	,sel(-1)
	,clicked(false)
	,showClipTools(false)
	,blockScroll(false)
	,bmp(NULL)
{
	if(icons.size()==0){
		//pami�taj, dodaj�c tutaj elementy, zmie� ich warto�� w pliku h!!
		icons.push_back(new itemdata(PTR_BITMAP_PNG("cross"),_("Wska�nik pozycji")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("position"),_("Przesuwanie tekstu")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("move"),_("Ruch")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("scale"),_("Skalowanie")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("frz"),_("Obr�t wok� osi Z")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("frxy"),_("Obr�t wok� osi X / Y")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("cliprect"),_("Wycinki prostok�tne")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("clip"),_("Wycinki wektorowe")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("drawing"),_("Rysunki wektorowe")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("MOVEAll"),_("Przesuwanie wielu tag�w")));
		//tutaj mamy ikony dla clipa
		icons.push_back(new itemdata(PTR_BITMAP_PNG("VectorDrag"),_("Przesu� punkty")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("VectorLine"),_("Dodaj lini�")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("VectorBezier"),_("Dodaj krzyw� Beziera")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("VECTORBSPLINE"),_("Dodaj krzyw� B-sklejan�")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("VectorMove"),_("Dodaj nowy oddzielny punkt")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("VectorDelete"),_("Usu� element")));
		//ikony move all
		icons.push_back(new itemdata(PTR_BITMAP_PNG("MOVEPOS"),_("Przenie� punkty pozycjonowania")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("MOVEMOVESTART"),_("Przenie� startowe punkty ruchu")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("MOVE"),_("Przenie� ko�cowe punkty ruchu")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("MOVECLIPS"),_("Przenie� wycinki")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("MOVEDRAWINGS"),_("Przenie� rysunki")));
		icons.push_back(new itemdata(PTR_BITMAP_PNG("MOVEORGS"),_("Przenie� punkty org")));

	}
	Connect(wxEVT_PAINT, (wxObjectEventFunction)&VideoToolbar::OnPaint);
	Connect(wxEVT_LEFT_DOWN, (wxObjectEventFunction)&VideoToolbar::OnMouseEvent);
	Connect(wxEVT_LEFT_UP, (wxObjectEventFunction)&VideoToolbar::OnMouseEvent);
	Connect(wxEVT_MOTION, (wxObjectEventFunction)&VideoToolbar::OnMouseEvent);
	Connect(wxEVT_LEAVE_WINDOW, (wxObjectEventFunction)&VideoToolbar::OnMouseEvent);
	Connect(wxEVT_MOUSEWHEEL, (wxObjectEventFunction)&VideoToolbar::OnMouseEvent);
	MoveToggled[i]=true;
	for (int i = 1; i < moveToolsSize; i++){
		MoveToggled[i]=false;
	}
}

int VideoToolbar::GetToggled()
{
	return Toggled;
}

void VideoToolbar::OnMouseEvent(wxMouseEvent &evt)
{
	if (evt.GetWheelRotation() != 0) {
		if(blockScroll || !showClipTools){evt.Skip(); return;}
		int step = evt.GetWheelRotation() / evt.GetWheelDelta();
		clipToggled-=step;
		if(clipToggled < toolsSize){clipToggled=toolsSize+clipToolsSize-1;}
		else if(clipToggled >= toolsSize+clipToolsSize){clipToggled=toolsSize;}
		Refresh(false);
		return;
	}
	int x=evt.GetX();
	int w=0;
	int h=0;
	GetClientSize (&w, &h);
	bool noelem=false;
	int elem = (x - startDrawPos) / h;//startDrawPos +
	if(elem<0){noelem=true;}
	else if(elem>=toolsSize){
		int toolbarlen = h * clipToolsSize;
		int posx = (x - (w - toolbarlen));
		if(posx < 0 || posx > toolbarlen || !showClipTools || !showMoveTools){
			noelem=true;
		}
		else{ elem = ( posx / h) + toolsSize;}
		if(showMoveTools){elem+=clipToolsSize;}
	}
	if(evt.Leaving() || noelem){sel = -1; Refresh(false); if(HasToolTips()){UnsetToolTip();} return;}
	
	if(elem != sel){
		sel=elem;
		SetToolTip(icons[elem]->help);
		Refresh(false);
	}
	if(evt.LeftDown()){
		if(elem>=moveToolsStart){MoveToggled[elem-moveToolsStart] = !MoveToggled[elem-moveToolsStart];}
		if(elem>=toolsSize){clipToggled=elem;}
		else{Toggled=elem;}
		clicked=true;
		Refresh(false);
	}
	if(evt.LeftUp()){
		clicked=false;
		Refresh(false);
		wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, (elem<toolsSize)? ID_VIDEO_TOOLBAR_EVENT : (elem< moveToolsStart)? ID_VECTOR_TOOLBAR_EVENT : ID_MOVE_TOOLBAR_EVENT);
		evt.SetInt((elem>=toolsSize)? clipToggled - toolsSize : Toggled);
		ProcessEvent(evt);
	}

}
	
void VideoToolbar::OnPaint(wxPaintEvent &evt)
{
	int w=0;
	int h=0;
	GetClientSize (&w, &h);
	if(w==0||h==0){return;}
	wxMemoryDC tdc;
	if (bmp && (bmp->GetWidth() < w || bmp->GetHeight() < h)) {
		delete bmp;
		bmp = NULL;
	}
	if(!bmp){bmp=new wxBitmap(w,h);}
	tdc.SelectObject(*bmp);
	tdc.SetBrush(wxBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUBAR)));
	tdc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_MENUBAR)));
	tdc.DrawRectangle(0,0,w,h);
	//wxLogStatus("Paint");
	int posX = startDrawPos;
	int i = 0;
	while(i < toolsSize + clipToolsSize + moveToolsSize){
		if(i == toolsSize){
			if(showMoveTools){i=moveToolsStart; posX = w - (h * moveToolsSize);}
			else if(!showClipTools){break;}
			else{
				posX = w - (h * clipToolsSize);
			}
		}
		if(icons[i]->icon->IsOk()){
			if(i==sel){
				tdc.SetBrush(wxBrush(wxSystemSettings::GetColour((i==Toggled || i==clipToggled)? wxSYS_COLOUR_MENUHILIGHT : wxSYS_COLOUR_MENUBAR)));
				tdc.SetPen(wxPen(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT)));
				tdc.DrawRoundedRectangle(posX, 1, h-2, h-2, 2.0);
			}else if(i==Toggled || i==clipToggled || (i >= moveToolsStart && MoveToggled[i-moveToolsStart])){
				tdc.SetBrush(wxBrush(wxSystemSettings::GetColour((clicked && i==sel)? wxSYS_COLOUR_BTNFACE : wxSYS_COLOUR_HIGHLIGHT)));
				tdc.SetPen(wxPen(wxSystemSettings::GetColour((clicked && i==sel)? wxSYS_COLOUR_BTNSHADOW : wxSYS_COLOUR_HIGHLIGHT)));
				tdc.DrawRoundedRectangle(posX, 1, h-2, h-2, 2.0);
			}
			
			tdc.DrawBitmap(*(icons[i]->icon),posX+2,3);
			posX+=h;
		}
		i++;
	}

	wxPaintDC dc(this);
	dc.Blit(0,0,w,h,&tdc,0,0);
}
	
void VideoToolbar::OnSize(wxSizeEvent &evt)
{
	Refresh(false);
}
