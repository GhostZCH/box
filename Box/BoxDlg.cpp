
// BoxDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Box.h"
#include "BoxDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
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


// CBoxDlg 对话框


CBoxDlg::CBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBoxDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBoxDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_HALT, &CBoxDlg::OnBnClickedButtonHalt)
	ON_BN_CLICKED(IDC_BUTTON_START, &CBoxDlg::OnBnClickedButtonStart)
	ON_WM_TIMER() // 映射添加消息
END_MESSAGE_MAP()


// CBoxDlg 消息处理程序

BOOL CBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	game = new Game(game->NET_HEIGHT,game->NET_WIDTH);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBoxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		DrawNet();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//-----------------自定义事件-----------------------


// 继续与暂停按钮点击
void CBoxDlg::OnBnClickedButtonHalt()
{
	game->HaltOrContinue();

	if(game->GetState()==HALT)
		KillTimer(1);

	if(game->GetState()==GO)
		SetTimer(1,game->TIME_STEP,NULL);
}

// 继续与暂停按钮点击
void CBoxDlg::OnBnClickedButtonStart()
{
	game->Start();
	SetTimer(1,game->TIME_STEP,NULL);
}

BOOL CBoxDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message==WM_KEYDOWN)
		OnKeyDown((UINT)pMsg->wParam);
	return false;
}


// 实现这个方法  定时器
void CBoxDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(!game->Go())
	{
		KillTimer(1);
		TCHAR *msg = _T("Game Over!");
		MessageBox(msg);
	}
	Invalidate(true);// 重绘画面
}


void CBoxDlg::DrawNet()
{
	DrawSmallNet();
	DrawBigNet();
}

void CBoxDlg::DrawBigNet()
{
	CRect rect;
	CWnd *wnd = GetDlgItem(IDC_PIC_MAIN);
	CPaintDC dc(wnd);

	wnd->GetClientRect(&rect);

	if(game->GetBigNet())
	{
		for(int i=0;i<game->NET_HEIGHT;i++)
			for(int j=0;j<game->NET_WIDTH;j++)
				if(game->GetBigNet()[i*(game->NET_WIDTH)+j]==1)
				{
					dc.Rectangle(
						j*rect.Width()/game->NET_WIDTH,
						i*rect.Height()/game->NET_HEIGHT,
						(j+1)*rect.Width()/game->NET_WIDTH,
						(i+1)*rect.Height()/game->NET_HEIGHT);
				}
	}

	wnd->RedrawWindow();
}

void CBoxDlg::DrawSmallNet()
{
	CRect rect;
	CWnd *wnd = GetDlgItem(IDC_PIC_SMALL);
	CPaintDC dc(wnd);

	wnd->GetClientRect(&rect);

	if(game->GetSmallNet())
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				if(game->GetSmallNet()[i*4+j]==1)
				{
					dc.Rectangle(
						j*rect.Width()/4,
						i*rect.Height()/4,
						(j+1)*rect.Width()/4,
						(i+1)*rect.Height()/4);
				}
	}

	wnd->RedrawWindow();
}


void CBoxDlg::OnKeyDown(UINT nChar)
{
	game->Input(nChar);
	Invalidate(true);// 重绘画面
}

