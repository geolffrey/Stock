#pragma once



// CHomeBox ������ͼ

class CHomeBox : public CFormView
{
	DECLARE_DYNCREATE(CHomeBox)

protected:
	CHomeBox();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CHomeBox();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HOME };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};


