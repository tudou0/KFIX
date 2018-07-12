#pragma once

#define SHORTCUT_SELECT_ALL 0x0001
#define SHORTCUT_COPY       0x0003


// CListBoxEx

class CListBoxEx : public CListBox
{
	DECLARE_DYNAMIC(CListBoxEx)

public:
	CListBoxEx();
	virtual ~CListBoxEx();

protected:
	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint point);
	afx_msg LRESULT OnCopy(WPARAM, LPARAM);
public:
	void DoSelectAll();
	void DoCopy();
	afx_msg void OnCopySelectall();
	afx_msg void OnCopyCopy();
};


