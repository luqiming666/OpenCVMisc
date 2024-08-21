
// OpenCVMiscDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "OpenCVMisc.h"
#include "OpenCVMiscDlg.h"
#include "afxdialogex.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Mat gSrcImg; // The original source image


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// COpenCVMiscDlg 对话框
COpenCVMiscDlg::COpenCVMiscDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCVMISC_DIALOG, pParent)
	, mSourceFile(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

#ifdef DEBUG
	mSourceFile = _T("D:\\opencv_copy.png");
	gSrcImg = imread((LPCTSTR)mSourceFile, IMREAD_COLOR); // Read the file
#endif // DEBUG
}

void COpenCVMiscDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SOURCE_FILE, mSourceFile);
}

BEGIN_MESSAGE_MAP(COpenCVMiscDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &COpenCVMiscDlg::OnBnClickedButtonBrowse)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_ALL_WIN, &COpenCVMiscDlg::OnBnClickedButtonCloseAllWin)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_ORIGINAL, &COpenCVMiscDlg::OnBnClickedButtonShowOriginal)
	ON_BN_CLICKED(IDC_BUTTON_GRAYSCALE, &COpenCVMiscDlg::OnBnClickedButtonGrayscale)
	ON_BN_CLICKED(IDC_BUTTON_BLUR, &COpenCVMiscDlg::OnBnClickedButtonBlur)
END_MESSAGE_MAP()


// COpenCVMiscDlg 消息处理程序

BOOL COpenCVMiscDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COpenCVMiscDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void COpenCVMiscDlg::OnPaint()
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
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COpenCVMiscDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COpenCVMiscDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void COpenCVMiscDlg::OnBnClickedButtonBrowse()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Image Files (*.jpg;*.jpeg;*.png;*.gif;*.bmp;*.webp)|*.jpg;*.jpeg;*.png;*.gif;*.bmp;*.webp|All Files (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		mSourceFile = fileDlg.GetPathName(); 
		UpdateData(FALSE);

		gSrcImg = imread((LPCTSTR)mSourceFile, IMREAD_COLOR); // Read the file
		ShowOriginalImage();
	}
}

void COpenCVMiscDlg::OnBnClickedButtonShowOriginal()
{
	ShowOriginalImage();
}

void COpenCVMiscDlg::OnBnClickedButtonCloseAllWin()
{
	cv::destroyAllWindows();
}

void COpenCVMiscDlg::ShowOriginalImage()
{
	if (mSourceFile.IsEmpty()) return;

	namedWindow("Original Image", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Original Image", gSrcImg); // Show our image inside it.
}

// 图像处理函数 #include <opencv2/imgproc/imgproc.hpp>
void COpenCVMiscDlg::OnBnClickedButtonGrayscale()
{
	Mat grey;
	cvtColor(gSrcImg, grey, COLOR_BGR2GRAY);
	imshow("Grayscale", grey);
}

// 高斯模糊
//	参数Size：内核的宽度和高度，须为奇数，或为0（则由sigma参数计算而得）
//	参数sigmaX：X方向的sigma，sigma越大越模糊（对更多的领域像素产生影响）
//	参数sigmaY：如果为0，则取值同sigmaX
void COpenCVMiscDlg::OnBnClickedButtonBlur()
{
	Mat newImage;
	GaussianBlur(gSrcImg, newImage, Size(0, 0), 5);
	imshow("Blur", newImage); 
}
