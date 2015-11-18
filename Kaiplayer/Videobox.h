#ifndef VIDEOBOX
#define VIDEOBOX

#pragma once

#include <wx/wx.h>
#include "VideoSlider.h"
#include "BitmapButton.h"
#include "VideoRenderer.h"
#include "VideoFullscreen.h"

class kainoteFrame;

class VideoCtrl : public VideoRend
{
public:

    VideoCtrl(wxWindow *parent, kainoteFrame *kfparent, const wxSize &size=wxDefaultSize);
    virtual ~VideoCtrl();
    bool Play();
	void PlayLine(int start, int end);
    bool Pause(bool burstbl=true);
    bool Stop();

    bool Load(const wxString& fileName, wxString *subsName, bool fulls=false);
    PlaybackState GetState();

    bool Seek(int whre,bool starttime=true,bool disp=true);
    int Tell();
	bool CalcSize(int *width, int *height,int wwidth=0,int wheight=0,bool setstatus=false,bool calcH=false);

	void NextFile(bool next=true);
	void SetFullskreen();
	void SetAspectRatio(float AR);
	VideoSlider* vslider;
	wxPanel* panel;
	bool isfullskreen;
	bool seekfiles;
	bool eater;
	bool fullarrow;
	bool blockpaint;
	wxMutex vbmutex;
	wxMutex nextmutex;
	wxTimer vtime;
	wxTextCtrl* mstimes;
	VolSlider* volslider;
	void OpenEditor(bool esc=true);
	void OnEndFile(wxCommandEvent& event);
	void OnPrew();
    void OnNext();
	void OnAccelerator(wxCommandEvent& event);
	void OnVButton(wxCommandEvent& event);
	void OnVolume(wxScrollEvent& event);
	void OnSMinus();
	void OnSPlus();
	void ChangeStream();
	void displaytime();
	void NextChap();
	void PrevChap();
	void ConnectAcc(int id);
	float fps,wspx,wspy;
	long ax,ay;
	float AR;
	Fullscreen *TD;
	bool isarrow;
	wxString oldpath;
private:
    BitmapButton* bprev;
	BitmapButton* bpause;
	BitmapButton* bstop;
	BitmapButton* bnext;
	BitmapButton* bpline;
	
    

    kainoteFrame *Kai;
	int actfile;
	int id;
	int prevchap;
	wxArrayString pliki;
	bool ismenu;

	void OnSize(wxSizeEvent& event);
	void OnMouseEvent(wxMouseEvent& event);
	void OnKeyPress(wxKeyEvent& event);
	void OnPlaytime(wxTimerEvent& event);
	void OnIdle(wxTimerEvent& event);
	void ContextMenu(const wxPoint &pos);
	void OnHidePB();
	void OnDeleteVideo();
	void OnOpVideo();
	void OnOpSubs();
	void OnPaint(wxPaintEvent& event);
	void OnCopyCoords(const wxPoint &pos);
	void InternalOnPopupMenu(wxCommandEvent& event){id = event.GetId();};
	void InternalOnPopupMenuUpdate(wxUpdateUIEvent& WXUNUSED(event)){};
	void ChangeButtonBMP(bool play=false);
	wxTimer idletime;
	DECLARE_EVENT_TABLE()
};

enum
	{
	idvtime=2000,
	ID_BPREV,
	ID_BPAUSE,
	ID_BSTOP,
	ID_BNEXT,
	ID_BPLINE,
	ID_SLIDER,
	ID_VOL,
	MENU_STOP,
	MENU_FULLS,
	MENU_OPVIDEO,
	MENU_OPEDITOR,
	MENU_OPSUBS,
	MENU_HIDEPB,
	MENU_DELVIDEO,
	MENU_PLAYP,
	MENU_P5SEC,	
	MENU_M5SEC,
	MENU_MMIN,
	MENU_PMIN,
	MENU_SPLUS,
	MENU_SMINUS,
	MENU_PREV,
	MENU_NEXT,
	MENU_PREVCHAP,
	MENU_NEXTCHAP,
	MENU_AR,
	MENU_CPYCOORDS,
	MENU_SAVEPNG,
	MENU_SAVECPBD,
	MENU_SAVESPNG,
	MENU_SAVESCPBD,
	ID_MRECSUBS,
	ID_MRECVIDEO,
	ID_IDLE,
	MENU_STREAMS=3333,
	MENU_CHAPTERS=12000
	};

#endif