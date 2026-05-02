#include "menu.h"

void OnExit(wxCommandEvent &e)
{
    wxExit();
}

void OnAbout(wxCommandEvent &e)
{
    wxMessageBox("Author:NKU_Yzj\nStudentID:2514108\nVersion:0.2.test\nContactMe:1181889301@qq.com", "monitor.game.testEDITION");
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
    wxMessageBox("", "Instruction"); // 玩法说明
}

wxMenuBar *CreateMenubar(wxFrame *parent)
{
    wxMenuBar *menubar = new wxMenuBar();
    g_mainframe = parent;
    // 菜单里面项目的子目录储存在wxMenu中
    wxMenu *wxStart = new wxMenu();
    wxStart->Append(wxID_EXIT, "退出");
    // wxStart -> AppendSeparator();
    // wxStart -> Append(wxID_OPEN,"打开新场景");
    // wxStart -> Append(wxID_SAVE,"存档");

    wxMenu *wxView = new wxMenu();
    wxView->Append(ID_FULLSCN, "全屏");

    wxMenu *wxHelp = new wxMenu();
    wxHelp->Append(wxID_ABOUT, "关于");
    wxHelp->Append(ID_HOTKEY, "热键查询");

    menubar->Append(wxStart, "开始");
    menubar->Append(wxView, "显示");
    menubar->Append(wxHelp, "帮助");

    parent->Bind(wxEVT_MENU, OnExit, wxID_EXIT);
    parent->Bind(wxEVT_MENU, OnFullSCN, ID_FULLSCN);

    parent->Bind(wxEVT_MENU, OnAbout, wxID_ABOUT);
    parent->Bind(wxEVT_MENU, OnHotKey, ID_HOTKEY);

    return menubar;
}