
// KFixDlg.h : header file
//

#pragma once
#include "afxdialogex.h"
#include "FixSearch.h"
#include "ListBoxEx.h"

#define ALL_SESSION "*ALL SESSIONS"

// CKFixDlg dialog
class CKFixDlg : public CDialogEx
{
// Construction
public:
	CKFixDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KFIX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	bool DlgInit();

private:
	CFixSearch m_KFS;
	char MsgType_Desc[11][32] = { "ALL MSGTYPES", "D-New Order", "F-Cancel Req", "G-Replace Req", "8-Execution RT", "   New", "   Filled", "   Cancelled", "   Replaced", "   Rejected", "9-Cxl Rej" };
	char MsgType_Q[11][64] = { "", "D", "F", "G", "8", "8)&(sOrdStatus=`0", "8)&((sOrdStatus=`1)|(sOrdStatus=`2)", "8)&((sOrdStatus=`4)|(sOrdStatus=`3)", "8)&(sOrdStatus=`5", "8)&(sOrdStatus=`8", "9" };

	CComboBox cFixSession;
	CEdit	cOrdId;
	bool	bCR;
	CEdit	cSym;
	CComboBox	cMsgType;
	CListBoxEx	cFixMsg;
	CStatic	cCount;

	CString sOrdId, sSym, sDate;
public:
	afx_msg void OnBnClickedKsearch();
	afx_msg void OnBnClickedCheckCr();
	afx_msg void OnDtnDatetimechangeDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult);
};
