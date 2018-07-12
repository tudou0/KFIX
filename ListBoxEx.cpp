// ListBoxEx.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ListBoxEx.h"


// CListBoxEx

IMPLEMENT_DYNAMIC(CListBoxEx, CListBox)

CListBoxEx::CListBoxEx()
{

}

CListBoxEx::~CListBoxEx()
{
}


BEGIN_MESSAGE_MAP(CListBoxEx, CListBox)
//	ON_MESSAGE(WM_COPY, OnCopy)
	ON_WM_CONTEXTMENU()
	ON_WM_CHAR()
	ON_COMMAND(ID_COPY_SELECTALL, &CListBoxEx::OnCopySelectall)
	ON_COMMAND(ID_COPY_COPY, &CListBoxEx::OnCopyCopy)
END_MESSAGE_MAP()

void CListBoxEx::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{ /* nChar */
	case SHORTCUT_SELECT_ALL:
		DoSelectAll();
		return;
	case SHORTCUT_COPY:
		DoCopy();
		return;
	} /* nChar */
	CListBox::OnChar(nChar, nRepCnt, nFlags);
}

void CListBoxEx::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_LISTBOX_POPUP);
	CMenu * popup = menu.GetSubMenu(0);

	int itemcount = CListBox::GetCount();
	int selcount = CListBox::GetSelCount();
	popup->EnableMenuItem(ID_COPY_SELECTALL,
		MF_BYCOMMAND | ((itemcount > 0 && selcount != itemcount) ? MF_ENABLED : MF_GRAYED));

	popup->EnableMenuItem(ID_COPY_COPY,
		MF_BYCOMMAND | (selcount > 0 ? MF_ENABLED : MF_GRAYED));

	popup->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

LRESULT CListBoxEx::OnCopy(WPARAM, LPARAM)
{
	DoCopy();
	return 0;
} // CListBoxEx::OnCopy



void CListBoxEx::OnCopySelectall()
{
	// TODO: Add your command handler code here
	DoSelectAll();
}


void CListBoxEx::OnCopyCopy()
{
	// TODO: Add your command handler code here
	DoCopy();
}

void CListBoxEx::DoSelectAll()
{
	CListBox::SelItemRange(TRUE, 0, CListBox::GetCount());
} // CListBoxEx::DoSelectAll

void CListBoxEx::DoCopy()
{
	CArray<int, int> sels;
	int n = CListBox::GetSelCount();
	if (n <= 0)
		return; // nothing to copy

	sels.SetSize(n);
	CListBox::GetSelItems(n, sels.GetData());

	CString s;
	//*****************************************************************************
	// This segment of code only works if the listbox is non-owner-draw,          *
	// or is owner-draw with LBS_HASSTRINGS                                       *
	// So first check to make sure this is true                                   *
	//*****************************************************************************
	ASSERT((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_OWNERDRAWVARIABLE)) == 0 || //*
		(GetStyle() & LBS_HASSTRINGS) == LBS_HASSTRINGS);                   //*
																			//*
																			// Extract the text                                                         //*
	for (int i = 0; i < n; i++)                                                  //*
	{ /* copy items */                                                       //*
		CString t;                                                              //*
		CListBox::GetText(sels[i], t);                                          //*
		t += _T("\r\n");                                                        //*
		s += t;                                                                 //*
	} /* copy items */                                                       //*
																			 //*****************************************************************************

	HGLOBAL g = ::GlobalAlloc(GMEM_MOVEABLE, (s.GetLength() + 1) * sizeof(TCHAR));
	if (g == NULL)
	{ /* alloc failed */
		ASSERT(FALSE);  // failed to allocate memory
		return;
	} /* alloc failed */

	LPTSTR p = (LPTSTR)::GlobalLock(g);
	if (p == NULL)
	{ /* lock failed */
		ASSERT(FALSE);
		return;
	} /* lock failed */

	memcpy_s(p, s.GetLength() + 1, (LPCTSTR)s, s.GetLength());

	::GlobalUnlock(g);

	if (!OpenClipboard())
	{ /* clipboard open failed */
		ASSERT(FALSE);
		GlobalFree(g);
		return;
	} /* clipboard open failed */

	if (!EmptyClipboard())
	{ /* empty failed */
		ASSERT(FALSE);
		GlobalFree(g);
		return;
	} /* empty failed */

#ifdef _UNICODE
#define CF_ CF_UNICODETEXT 
#else
#define CF_ CF_TEXT
#endif
	if (::SetClipboardData(CF_, g) == NULL)
	{ /* SetClipboardData failed */
		ASSERT(FALSE); //
		::CloseClipboard();
		::GlobalFree(g);
		return;
	} /* SetClipboardData failed */
	::CloseClipboard();
} // CListBoxEx::DoCopy


