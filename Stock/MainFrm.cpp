
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Stock.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_GETMINMAXINFO()

	ON_COMMAND(IDB_HOMEPAGE, OnPageHome)
	ON_COMMAND(IDB_IMPORT, OnPageImport)
	ON_COMMAND(IDB_FIND, OnPageFind)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建一个视图以占用框架的工作区
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("未能创建视图窗口\n");
	//	return -1;
	//}
	// 创建一个视图以占用框架的工作区

	//在这里使用CFormView
	CCreateContext ccx;
	ccx.m_pNewViewClass = RUNTIME_CLASS(CHomeBox);
	m_pMainView = DYNAMIC_DOWNCAST(CHomeBox, this->CreateView(&ccx));
	if (!m_pMainView)
	{
		TRACE0("Creation of view failed\n");
	}
	//另一种方式：
	// Creating the child view step-by-step
	// First order the frame window to create a new view for us
	//CWnd* pNewView = this->CreateView(&ccx);

	//// Cast down from the returned pointer
	//m_pMainView = DYNAMIC_DOWNCAST(CHomeBox, pNewView);

	//// Succesfull ?
	//if (!m_pMainView)
	//{
	//	TRACE0("Creation of view failed\n");
	//}

	RecalcLayout(); //这里实际上不需要重新布局
	// Show the view and do an initial update
	//m_pMainView->ShowWindow(SW_SHOW);
	//m_pMainView->OnInitialUpdate();
	// Set this view active
	SetActiveView(m_pMainView);



	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("未能创建工具栏\n");
	//	return -1;      // 未能创建
	//}

	//第一步 创建imagelist
	m_ImageList.Create(32, 32, ILC_COLOR32 | ILC_MASK, 1, 1); //ILC_COLOR32表示32位图象，ILC_MASK表示无背景色
															  //m_imagelist.SetBkColor(RGB(255, 255, 255));//设置背景颜色，似乎无作用

															  //5个按钮bmp的id，组成数组
	UINT images[3]{ IDB_HOMEPAGE, IDB_FIND ,IDB_IMPORT};
	CBitmap bm;
	for (int i = 0; i<3; i++)
	{
		bm.LoadBitmap(images[i]); //载入一个图片
		m_ImageList.Add(&bm, (CBitmap*)NULL); //加入到图片列表
		bm.Detach(); //分离该图片，为下次载入做准备

					 //这里直接使用图片的id，作为工具栏按钮的id
		images[i] += 0; //此时用户自定义消息
	}
	//第二步，创建工具栏对象，不使用LoadToolBar载入合并的工具栏图片
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ImageList); //m_ImageList作为工具栏控件的图象列表，可供其使用，但工具栏按钮尚未创建
	m_wndToolBar.EnableToolTips();

	//第三步：为工具栏增加按钮
	m_wndToolBar.SetButtons(images, 3);
	m_wndToolBar.SetButtonText(0, L"主页");
	m_wndToolBar.SetButtonText(1, L"搜索");
	m_wndToolBar.SetButtonText(2, L"导入");


	//第四步：设置按钮大小、图片大小
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(50, 70); //最小宽度50，最大70，这样处理了工具栏的距离问题
														  //70表示每个按钮的宽度，60为高度
	m_wndToolBar.SetSizes(CSize(70, 60), CSize(28, 28)); //按钮的大小设定，这样才显示了下面的文字

														 //第五步：调整之后，执行：
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	//工具栏样式调整：
	//1.设置按钮ID_ORDER和ID_SORT具有下拉箭头
	//m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	//DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(IDB_BITMAP_FIND)); //这里用的是图片id
	//dwStyle |= TBSTYLE_DROPDOWN;
	//m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(IDB_BITMAP_FIND), dwStyle);

	//2.使某个按钮变灰...这是自动将图片变灰的，不需要制作灰色按钮
	//Frame结构下，没有消息映射，也会变灰
	//CToolBarCtrl *pToolbarCtrl = (CToolBarCtrl*)&m_wndToolBar.GetToolBarCtrl();
	//pToolbarCtrl->EnableButton(IDB_BITMAP_FIND, false);
	//这样也行
	//pToolbarCtrl->SetState(IDB_BITMAP_IMPORT, TBSTATE_MARKED);

	//这个可以设置为按下的状态
	//pToolbarCtrl->SetState(IDB_BITMAP_IMPORT, TBSTATE_PRESSED);
	//这样会隐藏
	//pToolbarCtrl->SetState(IDB_BITMAP_IMPORT, TBSTATE_HIDDEN);

	//这样状态改为可用，配合PRESSED，实现单一的提示目前哪项功能
	//pToolbarCtrl->SetState(IDB_BITMAP_IMPORT, TBSTATE_ENABLED);

	

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 将焦点前移到视图窗口
	//m_wndView.SetFocus();
	m_pMainView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// 让视图第一次尝试该命令
	if (m_pMainView && ::IsWindow(m_pMainView->GetSafeHwnd()))
		if (m_pMainView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

	// 否则，执行默认处理
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CFrameWnd::OnGetMinMaxInfo(lpMMI);

	//设定窗体的最小大小为800X600，必须先执行父类的方法
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;
}

afx_msg void CMainFrame::OnPageHome()
{
	TRACE0("点击了：主页\n");
}


afx_msg void CMainFrame::OnPageFind()
{
	TRACE0("点击了：搜索\n");
}


afx_msg void CMainFrame::OnPageImport()
{
	TRACE0("点击了：导入\n");
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	//CFrameWnd::OnSize(nType, cx, cy);

	m_nIdleFlags &= ~idleLayout;

	CWnd::OnSize(nType, cx, cy);

	m_bInRecalcLayout = TRUE;
	RepositionBars(0, 0xffff, 0, reposExtra, &m_rectBorder);
	m_bInRecalcLayout = FALSE;

	if (GetActiveView() != NULL)
	{
		CRect Rect;
		GetClientRect(Rect);
		RECT rectTB = { 0 };
		
		//减去工具栏的高度
		::GetWindowRect(m_wndToolBar, &rectTB);
		Rect.top += rectTB.bottom - rectTB.top;

		//减去状态栏的高度
		::GetWindowRect(m_wndStatusBar, &rectTB);
		Rect.bottom -= rectTB.bottom - rectTB.top;

		GetActiveView()->SetWindowPos(NULL, Rect.left + 50, Rect.top + 50, Rect.Width() - 100, Rect.Height() - 100, SWP_SHOWWINDOW);
	}

	// TODO: 在此处添加消息处理程序代码
}


//每次需要清除背景，否则会有虚影
BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	CRect Rect;
	GetClientRect(&Rect);
	pDC->FillSolidRect(&Rect, ::GetSysColor(COLOR_APPWORKSPACE));

	return TRUE;

}
