
// OpenCVMiscDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "OpenCVMisc.h"
#include "OpenCVMiscDlg.h"
#include "afxdialogex.h"
#include "CAutoTicker.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// OpenCV documentation: https://docs.opencv.org/4.x/index.html
// 应用案例参考文章：https://blog.csdn.net/qq_28245087/article/details/131229053
//
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

	// Open the console window for logging
	if (::GetConsoleWindow() == NULL)
	{
		if (::AllocConsole())
		{
			FILE* stream;
			freopen_s(&stream, "CONOUT$", "w", stdout);
		}
	}

#ifdef DEBUG
	mSourceFile = _T("D:\\Dev\\GitHub\\OpenCVMisc\\assets\\puppy.png");
	gSrcImg = imread((LPCTSTR)mSourceFile, IMREAD_COLOR);
	DumpImageInfo();
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
	ON_BN_CLICKED(IDC_BUTTON_PROCESS_PIXELS, &COpenCVMiscDlg::OnBnClickedButtonProcessPixels)
	ON_BN_CLICKED(IDC_BUTTON_BLEND_2_IMAGES, &COpenCVMiscDlg::OnBnClickedButtonBlend2Images)
	ON_BN_CLICKED(IDC_BUTTON_bilateralFilter, &COpenCVMiscDlg::OnBnClickedButtonbilateralfilter)
	ON_BN_CLICKED(IDC_BUTTON_RESIZE, &COpenCVMiscDlg::OnBnClickedButtonResize)
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
		DumpImageInfo();
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

void COpenCVMiscDlg::DumpImageInfo()
{
	if (mSourceFile.IsEmpty()) return;

	std::cout << "Source file: " << mSourceFile << std::endl;
	std::cout << "[Format] width: " << gSrcImg.cols << " height: " << gSrcImg.rows << " channels: " << gSrcImg.channels() << " type: " << gSrcImg.type() << std::endl;
	std::cout << "Mean: " << cv::mean(gSrcImg) << std::endl;
}

void COpenCVMiscDlg::ShowOriginalImage()
{
	if (mSourceFile.IsEmpty()) return;

	namedWindow("Original Image", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Original Image", gSrcImg); // Show our image inside it.
}

int gamma_cor = 100;
char pGammaWinName[] = "Brightness-3-Gamma correction";
void on_gamma_correction_trackbar(int, void*)
{
	double gamma_value = gamma_cor / 100.0;

	Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; ++i) // 生成查询表
		p[i] = saturate_cast<uchar>(pow(i / 255.0, gamma_value) * 255.0);

	Mat new_image;
	LUT(gSrcImg, lookUpTable, new_image);
	imshow(pGammaWinName, new_image);
}

// [遍历每个像素] 调整画面 亮度/对比度，以及非线性优化：gamma校正
// 处理方法：newPixel(i, j) = alpha * originalPixel(i, j) + beta
// https://docs.opencv.org/4.10.0/d3/dc1/tutorial_basic_linear_transform.html
void COpenCVMiscDlg::OnBnClickedButtonProcessPixels()
{
	Mat new_image = Mat::zeros(gSrcImg.size(), gSrcImg.type());

	double alpha = 1.3; /*< Simple contrast control, [1.0-3.0] */
	int beta = 40;       /*< Simple brightness control, [0-100] */

	for (int y = 0; y < gSrcImg.rows; y++) {
		for (int x = 0; x < gSrcImg.cols; x++) {
			for (int c = 0; c < gSrcImg.channels(); c++) {
				new_image.at<Vec3b>(y, x)[c] =
					saturate_cast<uchar>(alpha * gSrcImg.at<Vec3b>(y, x)[c] + beta);
			}
		}
	}

	imshow("Brightness-1", new_image); // 可以发现“天空”过曝


#ifdef DEBUG
	// 等价调用
	// convertTo is more optimized and works a lot faster
	Mat new_image2;
	gSrcImg.convertTo(new_image2, -1, alpha, beta);
	imshow("Brightness-2", new_image2);
#endif // DEBUG


	// 非线性优化：gamma correction，解决“天空”过曝问题
	// opencv\samples\cpp\tutorial_code\ImgProc\changing_contrast_brightness_image
	namedWindow(pGammaWinName);
	createTrackbar("Gamma correction", pGammaWinName, &gamma_cor, 200, on_gamma_correction_trackbar);
}

// 将两个相同尺寸的图像叠加在一起
// https://docs.opencv.org/4.10.0/d5/dc4/tutorial_adding_images.html
void COpenCVMiscDlg::OnBnClickedButtonBlend2Images()
{
	Mat src1, src2, dst;
	src1 = imread(".\\assets\\LinuxLogo.jpg");
	src2 = imread(".\\assets\\WindowsLogo.jpg");

	double alpha = 0.5; 
	double beta = 1 - alpha;
	addWeighted(src1, alpha, src2, beta, 0.0, dst); // dst = alpha * src1 + beta * src2 + gamma
	imshow("Linear Blend", dst);
}




// 图像处理函数 #include <opencv2/imgproc.hpp>
// https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html
void COpenCVMiscDlg::OnBnClickedButtonGrayscale()
{
	Mat grey;
	cvtColor(gSrcImg, grey, COLOR_BGR2GRAY);
	imshow("Grayscale", grey);
}

// 几种图像平滑算法 Image Blurring (Image Smoothing)
// https://docs.opencv.org/4.x/d4/d13/tutorial_py_filtering.html
void COpenCVMiscDlg::OnBnClickedButtonBlur()
{
	// 高斯模糊
	//	参数Size：内核的宽度和高度，须为奇数，或为0（则由sigma参数计算而得）
	//	参数sigmaX：X方向的sigma，sigma越大越模糊（对更多的领域像素产生影响）
	//	参数sigmaY：如果为0，则取值同sigmaX
	Mat newImage;
	{
		CAutoTicker autoTicker("GaussianBlur");
		GaussianBlur(gSrcImg, newImage, Size(0, 0), 5);
	}
	imshow("Blur-1-GaussianBlur", newImage); 


	// 参数ksize为光圈大小，须为奇数，数值越大越模糊（主观：水彩/像素颗粒感，不如高斯平滑）
	// 有去噪效果
	{
		CAutoTicker autoTicker("medianBlur");
		medianBlur(gSrcImg, newImage, 15);
	}
	imshow("Blur-2-medianBlur", newImage);


	// 效果接近于高斯模糊，但更快！Size为内核的宽和高
	{
		CAutoTicker autoTicker("stackBlur");
		stackBlur(gSrcImg, newImage, Size(19, 19));
	}
	imshow("Blur-3-stackBlur", newImage);


	// 测试boxFilter，并且与原图合并对比
	Mat blur_merged = Mat(gSrcImg.rows, gSrcImg.cols * 2, gSrcImg.type());
	{
		CAutoTicker autoTicker("boxFilter");
		boxFilter(gSrcImg, newImage, -1, Size(19, 19));
	}
	hconcat(gSrcImg, newImage, blur_merged);
	imshow("Blur-4 (original vs. boxFilter)", blur_merged);
}

void COpenCVMiscDlg::OnBnClickedButtonbilateralfilter()
{
	// 去噪效果，与高斯模糊相比，增加了“边界”的考虑。算法较慢！
	// 注：medianBlur也有去噪效果
	Mat src = imread(".\\assets\\noisypic.png");
	Mat merged = Mat(src.rows, src.cols * 2, src.type());
	Mat newImage;
	{
		CAutoTicker autoTicker("bilateralFilter");
		bilateralFilter(src, newImage, 9, 250, 250);
	}

	cv::putText(src, "Before", Point(0, 180), FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 1, LINE_AA);
	cv::putText(newImage, "After", Point(0, 180), FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 1, LINE_AA);
	cv::hconcat(src, newImage, merged);
	imshow("bilateralFilter", merged);
}

void COpenCVMiscDlg::OnBnClickedButtonResize()
{
	//float aspectRatio = gSrcImg.cols * 1.0 / gSrcImg.rows;
	float ratio = 0.5f;
	int newWidth = gSrcImg.cols * ratio;
	int newHeight = gSrcImg.rows * ratio;

	Mat newImage;
	cv::resize(gSrcImg, newImage, Size(newWidth, newHeight), INTER_AREA);
	imshow("Resize", newImage);
}
