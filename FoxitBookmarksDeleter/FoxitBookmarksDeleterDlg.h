
// FoxitBookmarksDeleterDlg.h : header file
//

#pragma once


// CFoxitBookmarksDeleterDlg dialog
class CFoxitBookmarksDeleterDlg : public CDialogEx
{
// Construction
public:
	CFoxitBookmarksDeleterDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CFoxitBookmarksDeleterDlg();
// Dialog Data
	enum { IDD = IDD_FOXITBOOKMARKSDELETER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	void SetFinderToolImage(BOOL isFilled);
	void SearchWindow();
	void MoveCursorPositionToBullsEye();
	void DisplayInfoOnFoundWnd(CWnd* foundWnd);
	void RefreshWindow();
	void HighlightFoundWindow();

private:
	CBitmap m_bitmapFinderToolFilled;
	CBitmap m_bitmapFinderToolEmpty;
	HCURSOR m_cursorSearchWindow;
	HCURSOR m_cursorPrevious;
	BOOL m_startSearchWindow;
	CWnd* m_foundWnd;
	CString m_classNameFoundWnd;
	HPEN m_rectPen;
	
	const int BULLSEYE_CENTER_X_OFFSET;
	const int BULLSEYE_CENTER_Y_OFFSET;
	
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedFinder();
	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
