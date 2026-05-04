#include "menu.h"

void OnCLRCanvas(wxCommandEvent &e)
{
    if (g_mainframe)
    {
        MainFrame *frame = dynamic_cast<MainFrame *>(g_mainframe);
        frame->GetMap3D()->ClearCanvas();
    }
}

void OnAbout(wxCommandEvent &e)
{
    wxMessageBox("Author:NKU_Yzj\nStudentID:2514108\nVersion:1.2.test\nContactMe:1181889301@qq.com", "monitor.game.testEDITION");
}

void OnFullSCN(wxCommandEvent &e)
{
    if (g_mainframe)
    {
        g_mainframe->ShowFullScreen(!g_mainframe->IsFullScreen());
    }
}

void OnHotKey(wxCommandEvent &e)
{
    wxMessageBox("E:Open/Close the door\nQ:Open/Close the windows\n \
    WASD:Move\n Space:Jump\n \
    Mouse:{cursor mode = adjust perspective\n \
    Pencil mode = draw line\n Eraser mode = erase your line} \n",
                 "Instruction"); // 玩法说明
}

wxMenuBar *CreateMenubar(wxFrame *parent)
{
    wxMenuBar *menubar = new wxMenuBar();
    g_mainframe = parent;
    // 菜单里面项目的子目录储存在wxMenu中
    wxMenu *wxStart = new wxMenu();
    wxStart->Append(ID_CLRCANVAS, "清空屏幕");

    wxMenu *wxView = new wxMenu();
    wxView->Append(ID_FULLSCN, "全屏");

    wxMenu *wxHelp = new wxMenu();
    wxHelp->Append(wxID_ABOUT, "关于");
    wxHelp->Append(ID_HOTKEY, "热键查询");

    parent->Bind(wxEVT_MENU, OnCLRCanvas, ID_CLRCANVAS);
    parent->Bind(wxEVT_MENU, OnFullSCN, ID_FULLSCN);
    parent->Bind(wxEVT_MENU, OnAbout, wxID_ABOUT);
    parent->Bind(wxEVT_MENU, OnHotKey, ID_HOTKEY);

    menubar->Append(wxStart, "开始");
    menubar->Append(wxView, "显示");
    menubar->Append(wxHelp, "帮助");

    return menubar;
}