
// FoxitBookmarksDeleterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FoxitBookmarksDeleter.h"
#include "FoxitBookmarksDeleterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RETURN_IF_FALSE(flag) \
	do \
	{ \
		if (!(flag)) return; \
	} while (false);

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFoxitBookmarksDeleterDlg dialog




CFoxitBookmarksDeleterDlg::CFoxitBookmarksDeleterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFoxitBookmarksDeleterDlg::IDD, pParent)
	, m_startSearchWindow(FALSE)
	, BULLSEYE_CENTER_X_OFFSET(15)
	, BULLSEYE_CENTER_Y_OFFSET(18)
	, m_cursorPrevious(NULL)
	, m_foundWnd(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	VERIFY(m_bitmapFinderToolFilled.LoadBitmap(MAKEINTRESOURCE(IDB_FINDERF)));
	VERIFY(m_bitmapFinderToolEmpty.LoadBitmap(MAKEINTRESOURCE(IDB_FINDERE)));
	VERIFY(m_cursorSearchWindow = AfxGetApp()->LoadCursor(MAKEINTRESOURCE(IDC_SEARCHW)));
	VERIFY(m_rectPen = ::CreatePen(PS_SOLID, 3, RGB(255, 0, 0)));
}

CFoxitBookmarksDeleterDlg::~CFoxitBookmarksDeleterDlg()
{
	m_bitmapFinderToolFilled.DeleteObject();
	m_bitmapFinderToolEmpty.DeleteObject();
	::DeleteObject(m_rectPen);
}

void CFoxitBookmarksDeleterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFoxitBookmarksDeleterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_FINDER, &CFoxitBookmarksDeleterDlg::OnStnClickedFinder)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CFoxitBookmarksDeleterDlg::OnBnClickedBtnDelete)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CFoxitBookmarksDeleterDlg message handlers

BOOL CFoxitBookmarksDeleterDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFoxitBookmarksDeleterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFoxitBookmarksDeleterDlg::OnPaint()
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
HCURSOR CFoxitBookmarksDeleterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFoxitBookmarksDeleterDlg::SetFinderToolImage(BOOL isFilled)
{
	HBITMAP bitmapToSet(static_cast<HBITMAP>(m_bitmapFinderToolFilled));
	if (!isFilled)
	{
		bitmapToSet = static_cast<HBITMAP>(m_bitmapFinderToolEmpty);
	}

	(static_cast<CStatic*>(GetDlgItem(IDC_FINDER)))->SetBitmap(bitmapToSet);
}

void CFoxitBookmarksDeleterDlg::SearchWindow()
{
	m_startSearchWindow = TRUE;
	SetFinderToolImage(FALSE);

	MoveCursorPositionToBullsEye();

	if (m_cursorSearchWindow)
	{
		m_cursorPrevious = SetCursor(m_cursorSearchWindow);
	}
	else
	{
		m_cursorPrevious = NULL;
	}

	SetCapture();
}

void CFoxitBookmarksDeleterDlg::MoveCursorPositionToBullsEye()
{
	CRect rect;
	GetDlgItem(IDC_FINDER)->GetWindowRect(rect);
	CPoint screenPoint(rect.left+BULLSEYE_CENTER_X_OFFSET,
		rect.top+BULLSEYE_CENTER_Y_OFFSET);
	SetCursorPos(screenPoint.x, screenPoint.y);
}

void CFoxitBookmarksDeleterDlg::DisplayInfoOnFoundWnd(CWnd* foundWnd)
{
	m_foundWnd = foundWnd;
	HWND hwnd = foundWnd->GetSafeHwnd();
	TCHAR className[128];
	GetClassName(hwnd, className, 128);

	CString tmp;
	tmp.Format(_T("%x"), (DWORD)hwnd);
	SetDlgItemText(IDC_EDIT_HWND, tmp);

	SetDlgItemText(IDC_EDIT_CLASS, className);
	m_classNameFoundWnd = className;

	CRect rect;
	::GetWindowRect(hwnd, rect);
	tmp.Format(_T("(%d,%d)-(%d,%d) %d*%d"), rect.left, rect.top, rect.right, rect.bottom, rect.Width(), rect.Height());
	SetDlgItemText(IDC_EDIT_RECT, tmp);
}

void CFoxitBookmarksDeleterDlg::RefreshWindow()
{
	RETURN_IF_FALSE(m_foundWnd && m_foundWnd->IsWindowEnabled());

	m_foundWnd->InvalidateRect(NULL, TRUE);
	m_foundWnd->UpdateWindow();
	m_foundWnd->RedrawWindow(NULL, NULL, RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
}

void CFoxitBookmarksDeleterDlg::HighlightFoundWindow()
{
	RETURN_IF_FALSE(m_foundWnd && m_foundWnd->IsWindowEnabled());

	CRect rect;
	m_foundWnd->GetWindowRect(&rect);
	m_foundWnd->GetWindowDC();
	CDC* dc = m_foundWnd->GetWindowDC();
	if (dc)
	{
		HGDIOBJ oldPen = dc->SelectObject(m_rectPen);
		HGDIOBJ oldBrush = dc->SelectObject(GetStockObject(HOLLOW_BRUSH));

		dc->Rectangle(0, 0, rect.Width(), rect.Height());

		dc->SelectObject(oldPen);
		dc->SelectObject(oldBrush);

		m_foundWnd->ReleaseDC(dc);
	}
}

void CFoxitBookmarksDeleterDlg::OnStnClickedFinder()
{
	SearchWindow();
}


void CFoxitBookmarksDeleterDlg::OnBnClickedBtnDelete()
{
	RETURN_IF_FALSE(m_foundWnd && m_foundWnd->IsWindowEnabled());
	if (m_classNameFoundWnd==_T("SysTreeView32"))
	{
		m_foundWnd->SendMessage(TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT);
	}
}

void CFoxitBookmarksDeleterDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_startSearchWindow)
	{
		CPoint screenPoint;
		CWnd* foundWnd(NULL);
		GetCursorPos(&screenPoint);

		foundWnd = WindowFromPoint(screenPoint);
		if (foundWnd && (foundWnd->GetSafeHwnd()!=m_foundWnd->GetSafeHwnd()))
		{
			RefreshWindow();
			DisplayInfoOnFoundWnd(foundWnd);
			HighlightFoundWindow();
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CFoxitBookmarksDeleterDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_cursorPrevious)
	{
		SetCursor(m_cursorPrevious);
	}
	SetFinderToolImage(TRUE);
	ReleaseCapture();
	m_startSearchWindow = FALSE;

	CDialogEx::OnLButtonUp(nFlags, point);
}

