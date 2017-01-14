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

#include "Visuals.h"
#include "TabPanel.h"
#include <wx/regex.h>


enum {
	TAGPOS=1,
	TAGMOVES,
	TAGMOVEE=4,
	TAGCLIP=8,
	TAGP=16,
	TAGORG=32
};

MoveAll::MoveAll()
	: Visuals()
	, numElem(-1)
	, selectedTags(1)
{
}

void MoveAll::DrawVisual(int time)
{
	for(size_t i = 0; i <elems.size(); i++){
		if(!(selectedTags & elems[i].type)){continue;}
		if( elems[i].type == TAGPOS || elems[i].type == TAGMOVES){
			DrawRect(elems[i].elem);
		}else if( elems[i].type == TAGMOVEE ){
			DrawCircle(elems[i].elem);
		}else{
			D3DXCOLOR col= (elems[i].type == TAGCLIP)? 0xFF0000FF : (elems[i].type == TAGP)? 0xFFFF00FF : 0xFF8800FF;
			DrawCross(elems[i].elem, col);
		}
	}
	
}

void MoveAll::OnMouseEvent(wxMouseEvent &evt)
{
	if(blockevents){return;}
	bool click = evt.LeftDown()||evt.RightDown()||evt.MiddleDown();
	bool holding = (evt.LeftIsDown()||evt.RightIsDown()||evt.MiddleIsDown());

	int x, y;
	if(tab->Video->isFullscreen){wxGetMousePosition(&x,&y);}
	else{evt.GetPosition(&x,&y);}

	if(evt.ButtonUp()){
		if(tab->Video->HasCapture()){tab->Video->ReleaseMouse();}
		if(numElem>=0){ChangeInLines(true);}
		if(!hasArrow){tab->Video->SetCursor(wxCURSOR_ARROW);hasArrow=true;}
		numElem=-1;
	}
	
	if(click){

		for(size_t i = 0; i <elems.size(); i++){
			if(!(selectedTags & elems[i].type)){continue;}
			if(abs(elems[i].elem.x - x) < 8 && abs(elems[i].elem.y - y) < 8){
				numElem=i;
				beforeMove = lastmove = elems[i].elem;
				diffs.x=elems[i].elem.x-x;
				diffs.y=elems[i].elem.y-y;
				tab->Video->CaptureMouse();
			}
		}

	}else if(holding && numElem >= 0 ){
		lastmove = elems[numElem].elem;
		elems[numElem].elem.x = x + diffs.x;
		elems[numElem].elem.y = y + diffs.y;

		D3DXVECTOR2 moving = elems[numElem].elem - lastmove;
		for(size_t j = 0; j < elems.size(); j++){
			if(j == numElem || !(selectedTags & elems[j].type)){continue;}
			elems[j].elem.x += moving.x;
			elems[j].elem.y += moving.y;
		}
		ChangeInLines(false);
	}

}

void MoveAll::SetCurVisual()
{
	D3DXVECTOR2 linepos = GetPosnScale(NULL, NULL, tbl);
	if(tbl[6]>3){linepos=CalcMovePos();}
	from = to = D3DXVECTOR2(linepos.x/wspw,linepos.y/wsph);
	elems.clear();

	wxString res;
	if(tab->Edit->FindVal("org\\(([^\\)]+)", &res, "", 0, true)){
		wxString rest;
		double orx,ory;
		
		moveElems elem;
		if(res.BeforeFirst(',',&rest).ToDouble(&orx)){elem.elem.x=orx/wspw;}
		if(rest.ToDouble(&ory)){elem.elem.y=ory/wsph;}
		elem.type=TAGORG;
		elems.push_back(elem);
	}
	if(tab->Edit->FindVal("(i?clip[^\\)]+)", &res, "", 0, true)){
		wxRegEx re("m ([0-9.-]+) ([0-9.-]+)", wxRE_ADVANCED);
		moveElems elem;
		if(re.Matches(res)){
			elem.elem = D3DXVECTOR2(wxAtoi(re.GetMatch(res,1))/wspw, wxAtoi(re.GetMatch(res,2))/wsph);
		}else{
			wxString txt = tab->Edit->TextEdit->GetValue();
			int repl = txt.Replace(",", ",");
			wxRegEx re("\\(([0-9.-]+)[, ]*([0-9.-]+)[, ]*([0-9.-]+)", wxRE_ADVANCED);
			if(repl==3){
				elem.elem = D3DXVECTOR2(wxAtoi(re.GetMatch(res,1))/wspw, wxAtoi(re.GetMatch(res,2))/wsph);
			}else if(repl>3){
				elem.elem = D3DXVECTOR2(wxAtoi(re.GetMatch(res,2))/wspw, wxAtoi(re.GetMatch(res,3))/wsph);
			}
		}
		elem.type=TAGCLIP;
		elems.push_back(elem);
	}
	if(tab->Edit->FindVal("p([0-9]+)", &res, "", 0, true)){
		res=tab->Edit->TextEdit->GetValue();
		wxRegEx re("} ?m ([.0-9-]+) ([.0-9-]+)", wxRE_ADVANCED);
		if(re.Matches(res)){
			moveElems elem;

			elem.elem= D3DXVECTOR2(wxAtoi(re.GetMatch(res,1))/wspw, wxAtoi(re.GetMatch(res,2))/wsph);
			elem.type=TAGP;
			elems.push_back(elem);
		}

	}
	if(tbl[6]==2){
		moveElems elem;
		elem.elem= D3DXVECTOR2(tbl[0] / wspw, tbl[1] / wsph);
		elem.type=TAGPOS;
		elems.push_back(elem);
	}
	if(tbl[6]>=4){
		moveElems elem;
		elem.elem= D3DXVECTOR2(tbl[0] / wspw, tbl[1] / wsph);
		elem.type=TAGMOVES;
		elems.push_back(elem);
		elem.type=TAGMOVEE;
		elem.elem= D3DXVECTOR2(tbl[2] / wspw, tbl[3] / wsph);
		elems.push_back(elem);
	}

}

wxString MoveAll::GetVisual()
{
	return "";
}

void MoveAll::ChangeInLines(bool all)
{
	//D3DXVECTOR2 moving;
	D3DXVECTOR2 moving = elems[numElem].elem - beforeMove;
	int _time = tab->Video->Tell();
	wxString *dtxt;
	if(!all){
		if(!dummytext){
			bool visible=false; 
			dummytext = tab->Grid1->GetVisible(&visible,0,true);
		}
		dtxt=new wxString(*dummytext);
	}

	wxString tmp;
	//bool isOriginal=(tab->Grid1->transl && tab->Edit->TextEdit->GetValue()=="");
	//MTextEditor *Editor=(isOriginal)? tab->Edit->TextEditTl : tab->Edit->TextEdit;
	//wxString origText=Editor->GetValue();
	
	wxArrayInt sels= tab->Grid1->GetSels();
	for(size_t i = 0; i< sels.size(); i++){
		wxString txt;
		Dialogue *Dial = tab->Grid1->GetDial(sels[i]);

		if(!all && !(_time >= Dial->Start.mstime && _time <= Dial->End.mstime)){continue;}
		bool istexttl=(tab->Grid1->transl && Dial->TextTl!="");
		txt = (istexttl)? Dial->TextTl : Dial->Text;

		for(int k = 0; k < 6; k++){
			byte type = selectedTags & (1 << k);
			if(!type){continue;}
			bool vector= type==TAGCLIP||type==TAGP;
			wxString delimiter= (vector)? " " : ",";
			wxString tagpattern = (type==TAGPOS)? "pos\\(([^\\)]+)" : (type==TAGORG)? "org\\(([^\\)]+)" : (type==TAGCLIP)? "i?clip\\(([^\\)]+)" : (type==TAGP)? "p[0-9-]+[^}]*} ?m ([^{]+)" : "move\\(([^\\)]+)"; 
			wxRegEx re(tagpattern, wxRE_ADVANCED);
			size_t startMatch=0, lenMatch=0;
			if(re.Matches(txt)){
				wxString visual;
				//wxString tag=re.GetMatch(txt, 1); tag te� nigdzie nie jest potrzebny, bo wycinamy tylko jego warto��.
				tmp= re.GetMatch(txt, 1);
				//re.GetMatch(&startMatch, &lenMatch, 2); niepotrzebny drugi raz u�ycie tego samego
				wxStringTokenizer tkn(tmp, delimiter,wxTOKEN_STRTOK);
				int count=0;
				while(tkn.HasMoreTokens()){
					wxString token=tkn.GetNextToken().Trim().Trim(false);
					double val;
					if(token.ToDouble(&val)){
						if(count % 2 == 0){val += (moving.x * wspw);}else{val += (moving.y * wsph);}
						if(type==TAGMOVES && count > 1 ){visual+=token+delimiter; continue;}
						else if(type==TAGMOVEE && count != 2 && count != 3){visual+=token+delimiter; count++; continue;}
						if(vector){visual<<getfloat(val,(type==TAGCLIP)? "6.0f" : "6.2f")<<delimiter;}
						else{visual += getfloat(val) + delimiter;}
						count++;
					}else{
						visual+=token+delimiter;
						if(!vector){count++;}
					}
				}
				if(re.GetMatch(&startMatch, &lenMatch, 1)){
					visual.RemoveLast();
					if(lenMatch){txt.erase(txt.begin()+startMatch, txt.begin()+startMatch+lenMatch);}
					txt.insert(startMatch,visual);

				}
			}
		
		}
		if(all){
			tab->Grid1->CopyDial(sels[i])->Text=txt;
		}else{
			Dialogue Cpy=Dialogue(*Dial);
			if(istexttl) {
				Cpy.TextTl = txt;
				(*dtxt)<<Cpy.GetRaw(true);
				(*dtxt)<<Cpy.GetRaw(false,tab->Grid1->GetSInfo("TLMode Style"));
			}else{
				Cpy.Text = txt;
				(*dtxt)<<Cpy.GetRaw();
			}

			
		}
	}
	if(all){
		tab->Video->VisEdit=true;
		if(tab->Edit->splittedTags){tab->Edit->TextEditTl->modified=true;}
		tab->Grid1->SetModified(true);
		tab->Grid1->Refresh();
	}else{
		if(!tab->Video->OpenSubs(dtxt)){wxLogStatus(_("Nie mo�na otworzy� napis�w"));}
		tab->Video->VisEdit=true;
		tab->Video->Render();

	}

}

void MoveAll::ChangeTool(int _tool)
{
	selectedTags = _tool;
	tab->Video->Render(false);
}