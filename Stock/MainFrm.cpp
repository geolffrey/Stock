
// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// ����һ����ͼ��ռ�ÿ�ܵĹ�����
	//if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	//{
	//	TRACE0("δ�ܴ�����ͼ����\n");
	//	return -1;
	//}
	// ����һ����ͼ��ռ�ÿ�ܵĹ�����

	//������ʹ��CFormView
	CCreateContext ccx;
	ccx.m_pNewViewClass = RUNTIME_CLASS(CHomeBox);
	m_pMainView = DYNAMIC_DOWNCAST(CHomeBox, this->CreateView(&ccx));
	if (!m_pMainView)
	{
		TRACE0("Creation of view failed\n");
	}
	//��һ�ַ�ʽ��
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

	RecalcLayout(); //����ʵ���ϲ���Ҫ���²���
	// Show the view and do an initial update
	//m_pMainView->ShowWindow(SW_SHOW);
	//m_pMainView->OnInitialUpdate();
	// Set this view active
	SetActiveView(m_pMainView);



	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("δ�ܴ���������\n");
	//	return -1;      // δ�ܴ���
	//}

	//��һ�� ����imagelist
	m_ImageList.Create(32, 32, ILC_COLOR32 | ILC_MASK, 1, 1); //ILC_COLOR32��ʾ32λͼ��ILC_MASK��ʾ�ޱ���ɫ
															  //m_imagelist.SetBkColor(RGB(255, 255, 255));//���ñ�����ɫ���ƺ�������

															  //5����ťbmp��id���������
	UINT images[3]{ IDB_HOMEPAGE, IDB_FIND ,IDB_IMPORT};
	CBitmap bm;
	for (int i = 0; i<3; i++)
	{
		bm.LoadBitmap(images[i]); //����һ��ͼƬ
		m_ImageList.Add(&bm, (CBitmap*)NULL); //���뵽ͼƬ�б�
		bm.Detach(); //�����ͼƬ��Ϊ�´�������׼��

					 //����ֱ��ʹ��ͼƬ��id����Ϊ��������ť��id
		images[i] += 0; //��ʱ�û��Զ�����Ϣ
	}
	//�ڶ������������������󣬲�ʹ��LoadToolBar����ϲ��Ĺ�����ͼƬ
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ImageList); //m_ImageList��Ϊ�������ؼ���ͼ���б��ɹ���ʹ�ã�����������ť��δ����
	m_wndToolBar.EnableToolTips();

	//��������Ϊ���������Ӱ�ť
	m_wndToolBar.SetButtons(images, 3);
	m_wndToolBar.SetButtonText(0, L"��ҳ");
	m_wndToolBar.SetButtonText(1, L"����");
	m_wndToolBar.SetButtonText(2, L"����");


	//���Ĳ������ð�ť��С��ͼƬ��С
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(50, 70); //��С���50�����70�����������˹������ľ�������
														  //70��ʾÿ����ť�Ŀ�ȣ�60Ϊ�߶�
	m_wndToolBar.SetSizes(CSize(70, 60), CSize(28, 28)); //��ť�Ĵ�С�趨����������ʾ�����������

														 //���岽������֮��ִ�У�
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	//��������ʽ������
	//1.���ð�ťID_ORDER��ID_SORT����������ͷ
	//m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
	//DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(IDB_BITMAP_FIND)); //�����õ���ͼƬid
	//dwStyle |= TBSTYLE_DROPDOWN;
	//m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(IDB_BITMAP_FIND), dwStyle);

	//2.ʹĳ����ť���...�����Զ���ͼƬ��ҵģ�����Ҫ������ɫ��ť
	//Frame�ṹ�£�û����Ϣӳ�䣬Ҳ����
	//CToolBarCtrl *pToolbarCtrl = (CToolBarCtrl*)&m_wndToolBar.GetToolBarCtrl();
	//pToolbarCtrl->EnableButton(IDB_BITMAP_FIND, false);
	//����Ҳ��
	//pToolbarCtrl->SetState(IDB_BITMAP_IMPORT, TBSTATE_MARKED);

	//�����������Ϊ���µ�״̬
	//pToolbarCtrl->SetState(IDB_BITMAP_IMPORT, TBSTATE_PRESSED);
	//����������
	//pToolbarCtrl->SetState(IDB_BITMAP_IMPORT, TBSTATE_HIDDEN);

	//����״̬��Ϊ���ã����PRESSED��ʵ�ֵ�һ����ʾĿǰ�����
	//pToolbarCtrl->SetState(IDB_BITMAP_IMPORT, TBSTATE_ENABLED);

	

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}

	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame ���

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


// CMainFrame ��Ϣ�������

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ������ǰ�Ƶ���ͼ����
	//m_wndView.SetFocus();
	m_pMainView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ����ͼ��һ�γ��Ը�����
	if (m_pMainView && ::IsWindow(m_pMainView->GetSafeHwnd()))
		if (m_pMainView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return TRUE;

	// ����ִ��Ĭ�ϴ���
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CFrameWnd::OnGetMinMaxInfo(lpMMI);

	//�趨�������С��СΪ800X600��������ִ�и���ķ���
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;
}

afx_msg void CMainFrame::OnPageHome()
{
	TRACE0("����ˣ���ҳ\n");
}


afx_msg void CMainFrame::OnPageFind()
{
	TRACE0("����ˣ�����\n");
}


afx_msg void CMainFrame::OnPageImport()
{
	TRACE0("����ˣ�����\n");
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
		
		//��ȥ�������ĸ߶�
		::GetWindowRect(m_wndToolBar, &rectTB);
		Rect.top += rectTB.bottom - rectTB.top;

		//��ȥ״̬���ĸ߶�
		::GetWindowRect(m_wndStatusBar, &rectTB);
		Rect.bottom -= rectTB.bottom - rectTB.top;

		GetActiveView()->SetWindowPos(NULL, Rect.left + 50, Rect.top + 50, Rect.Width() - 100, Rect.Height() - 100, SWP_SHOWWINDOW);
	}

	// TODO: �ڴ˴������Ϣ����������
}


//ÿ����Ҫ������������������Ӱ
BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	CRect Rect;
	GetClientRect(&Rect);
	pDC->FillSolidRect(&Rect, ::GetSysColor(COLOR_APPWORKSPACE));

	return TRUE;

}
