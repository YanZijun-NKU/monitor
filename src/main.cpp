#include "main.h"
#include "menu.h"
#include "statuebar.h"
#include "toolbar.h"
// 实质上是wximplement->myapp->oninit->mainframe->content

class MyApp : public wxApp
{
    bool OnInit() override;
    // override is used to make sure that you key in the right funcname
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();
    MainFrame *frame = new MainFrame();
    frame->Show(true); // show是使窗口显示
    return true;
}

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "monitor.game", wxDefaultPosition, wxSize(1280, 720))
// args-list : fatherframe addr,sys_id,name,position,size
{
    map = new Map3D(this);
    mp2d = new Map2D(this);
    Mystatusbar *statusBar = new Mystatusbar(this);
    Mytoolbar *ToolBar = new Mytoolbar(this);
    // 自适应大小，且让地图填满
    // https://zetcode.com/gui/wxwidgets/layoutmanagement/
    // 豆包提供中文和样例支持
    wxMenuBar *bar = CreateMenubar(this);
    SetMenuBar(bar);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *viceSizer = new wxBoxSizer(wxHORIZONTAL);

    viceSizer->Add(ToolBar, 0, wxEXPAND); // 宽度由系统自动决定
    viceSizer->Add(map, 1, wxEXPAND);

    mainSizer->Add(viceSizer, 1, wxEXPAND);
    mainSizer->Add(statusBar, 0, wxEXPAND);

    SetSizer(mainSizer);
    Centre();
    if (mp2d)
        mp2d->Show(true); // 单独展示
    map->SetMap2D(mp2d);
    map->SetStatusBar(statusBar);
    map->Refresh();
    this->Bind(wxEVT_SHOW, [this](wxShowEvent &)
               {if(map) map->SetFocus(); });
}
