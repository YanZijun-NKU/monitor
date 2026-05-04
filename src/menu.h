#pragma once
#include <wx/wx.h>
#include "main.h"

#define ID_FULLSCN 1001
#define ID_CLRCANVAS 1002
#define ID_HOTKEY 1004

wxMenuBar *CreateMenubar(wxFrame *parent);
static wxFrame *g_mainframe=nullptr;