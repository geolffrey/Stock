// HomeBox.cpp : 实现文件
//

#include "stdafx.h"
#include "Stock.h"
#include "HomeBox.h"


// CHomeBox

IMPLEMENT_DYNCREATE(CHomeBox, CFormView)

CHomeBox::CHomeBox()
	: CFormView(IDD_DIALOG_HOME)
{

}

CHomeBox::~CHomeBox()
{
}

void CHomeBox::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHomeBox, CFormView)
END_MESSAGE_MAP()


// CHomeBox 诊断

#ifdef _DEBUG
void CHomeBox::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHomeBox::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHomeBox 消息处理程序
