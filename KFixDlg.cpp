
// KFixDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KFix.h"
#include "KFixDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKFixDlg dialog



CKFixDlg::CKFixDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KFIX_DIALOG, pParent), sOrdId(""), sSym("")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKFixDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FIXSESSION, cFixSession);
	DDX_Control(pDX, IDC_ORDID, cOrdId);
	DDX_Control(pDX, IDC_SYMBOL, cSym);
	DDX_Control(pDX, IDC_MSGTYPE, cMsgType);
	DDX_Control(pDX, IDC_FIXMSGLIST, cFixMsg);
	DDX_Control(pDX, IDC_COUNT, cCount);

	DDX_Text(pDX, IDC_ORDID, sOrdId);
	DDX_Text(pDX, IDC_SYMBOL, sSym);
}

BEGIN_MESSAGE_MAP(CKFixDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_KSEARCH, &CKFixDlg::OnBnClickedKsearch)
	ON_BN_CLICKED(IDC_CHECK_CR, &CKFixDlg::OnBnClickedCheckCr)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER, &CKFixDlg::OnDtnDatetimechangeDatetimepicker)
END_MESSAGE_MAP()


// CKFixDlg message handlers
bool CKFixDlg::DlgInit()
{
	int i,row;
	try
	{
		m_KFS.Init("s07apdv206", 12345, ":");
		row = m_KFS.Query("select distinct sSenderCompID from fixmsgs");
		for (i = 0; i < row; i++)
		{
			K_RET ret;
			if (m_KFS.Fetch(0, i, ret))
				cFixSession.AddString((LPCTSTR)ret._str.s);
		}
		m_KFS.CloseQuery();

		cFixSession.AddString((LPCTSTR)ALL_SESSION);
	}
	catch (const char *e)
	{
		MessageBox(e, "KX error");
		m_KFS.CloseConn();

		return false;
	}
	cFixSession.SetCurSel(1);

	SYSTEMTIME t;
	GetLocalTime(&t);
	sDate.Format("%4d.%02d.%02dT00:00:00.000", t.wYear, t.wMonth, t.wDay);

	for (i = 0; i < sizeof(MsgType_Desc)/32;i++)
		cMsgType.AddString((LPCTSTR)MsgType_Desc[i]);
	cMsgType.SetCurSel(0);

	bCR = false;
	((CButton*)GetDlgItem(IDC_CHECK_CR))->SetCheck(bCR);
	cFixMsg.SetHorizontalExtent(5000);
	cCount.SetWindowText("");

	SetDefID(IDC_KSEARCH);

	return true;
}

BOOL CKFixDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	DlgInit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKFixDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKFixDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CKFixDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CKFixDlg::OnBnClickedKsearch()
{
	// TODO: Add your control notification handler code here
	CString sFixSession, sQ, sCond;
	
	sQ = "select cFixMsg from fixmsgs where (tMsgDate="+sDate+")";

	sCond = "";

	int iCur;
	// session
	iCur=cFixSession.GetCurSel();
	cFixSession.GetLBText(iCur, sFixSession);
	if (sFixSession != ALL_SESSION)
	{
		sCond.Format("&((sSenderCompID=`$(\"%s\"))|(sTargetCompID=`$(\"%s\")))", sFixSession, sFixSession);
		sQ.Append(sCond);
	}

	int iLen;
	// Order id
	iLen = cOrdId.LineLength(0);
	if (iLen > 0)
	{
		cOrdId.GetLine(0, sOrdId.GetBuffer(iLen), iLen);
		sOrdId.ReleaseBuffer(iLen);
		if (sOrdId != "")
		{
			if (sFixSession == ALL_SESSION)
			{	// order id need to have session
				MessageBox("Please pick 'FIX Session'", "K Search Error");
				return;
			}

			char chCROrdIdStr[32768];
			int CR_Cnt = 0;
			try
			{
				sprintf_s(chCROrdIdStr, 64, "\"%s\"", sOrdId.GetBuffer());
				if (bCR)
				{	// include CR chain
					m_KFS.GetCROrd(0, sFixSession.GetBuffer(), sOrdId.GetBuffer(), chCROrdIdStr + strlen(chCROrdIdStr), CR_Cnt);
					m_KFS.GetCROrd(1, sFixSession.GetBuffer(), sOrdId.GetBuffer(), chCROrdIdStr + strlen(chCROrdIdStr), CR_Cnt);
				}
			}
			catch (const char *e)
			{
				m_KFS.CloseQuery();
				MessageBox(e, "KX error");
				return;
			}

			if(CR_Cnt==0)
				sCond.Format("&((cClOrdID like %s)|(sOrigClOrdID=`$(%s)))", chCROrdIdStr, chCROrdIdStr);
			else
				sCond.Format("&((cClOrdID in (%s))|(sOrigClOrdID in`$(%s)))", chCROrdIdStr, chCROrdIdStr);
			sQ.Append(sCond);
		}
	}
	else
	{
		if (bCR)
		{
			MessageBox("Need OrdId to check on 'CxlRpl'", "K Search Error");
			return;
		}
	}

	// Symbol
	iLen = cSym.LineLength(0);
	if (iLen > 0)
	{
		cSym.GetLine(0, sSym.GetBuffer(iLen), iLen);
		sSym.ReleaseBuffer(iLen);
		if (sSym != "")
		{
			sCond.Format("&(sSymbol=`%s)", sSym);
			sQ.Append(sCond);
		}
	}
	else
	{
		if (sFixSession == ALL_SESSION)
		{	// global browwing need to have symbol/avoid too big data set
			MessageBox("Please pick 'FIX Session'", "K Search Error");
			return;
		}
	}

	// Msg type
	iCur = cMsgType.GetCurSel();
	if (iCur != 0)
	{
		sCond.Format("&(sMsgType=`%s)", MsgType_Q[iCur]);
		sQ.Append(sCond);
	}

	cFixMsg.ResetContent();
	try
	{
		int i, j;
		j = m_KFS.Query(sQ.GetBuffer());

		CString sCnt;
		sCnt.Format("%ld", j);
		cCount.SetWindowTextA(sCnt.GetBuffer());
		for (i = 0; i < j ; i++)
		{
			K_RET ret;
			if (m_KFS.Fetch(0, i, ret))
			{
				*(ret._str.s + ret._str.len-1) = 0;
				cFixMsg.AddString((LPCTSTR)ret._str.s);
			}
		}

		m_KFS.CloseQuery();
		
		cFixMsg.SetCurSel(0);
	}
	catch (const char *e)
	{
		m_KFS.CloseQuery();
		MessageBox(e, "KX error");
	}
}

void CKFixDlg::OnBnClickedCheckCr()
{
	bCR= BST_CHECKED==(((CButton*)GetDlgItem(IDC_CHECK_CR))->GetCheck());
}


void CKFixDlg::OnDtnDatetimechangeDatetimepicker(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	sDate.Format("%4d.%02d.%02dT00:00:00.000", pDTChange->st.wYear, pDTChange->st.wMonth, pDTChange->st.wDay);
	*pResult = 0;
}
