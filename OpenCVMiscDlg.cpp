
// OpenCVMiscDlg.cpp: 实现文件
//

#include "pch.h"
#include "OpenCVMisc.h"
#include "OpenCVMiscDlg.h"
#include "afxdialogex.h"
#include "CAutoTicker.h"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/features2d.hpp>

using namespace cv;

// Only works for Release configuration
//#define _ENABLE_TESSERACT_

#ifdef _ENABLE_TESSERACT_
#include "baseapi.h" 
#include "allheaders.h"

#pragma comment(lib, "leptonica-1.84.1.lib")
#pragma comment(lib, "tesseract54.lib")
#endif // _ENABLE_TESSERACT_

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// OpenCV documentation: https://docs.opencv.org/4.x/index.html
// 基础概念，e.g. Mat - https://docs.opencv.org/4.10.0/d6/d6d/tutorial_mat_the_basic_image_container.html
// 应用案例参考文章：https://blog.csdn.net/qq_28245087/article/details/131229053
//
Mat gSrcImg; // The original source image


int GetBytesPerPixel(Mat& image)
{
	int depth = image.depth();
	int channels = image.channels();

	int bytesPerPixel;
	switch (depth) {
	case CV_8U:
		bytesPerPixel = channels;
		break;
	case CV_8S:
		bytesPerPixel = channels;
		break;
	case CV_16U:
		bytesPerPixel = channels * 2;
		break;
	case CV_16S:
		bytesPerPixel = channels * 2;
		break;
	case CV_32S:
		bytesPerPixel = channels * 4;
		break;
	case CV_32F:
		bytesPerPixel = channels * 4;
		break;
	case CV_64F:
		bytesPerPixel = channels * 8;
		break;
	default:
		bytesPerPixel = 0;
		break;
	}
	return bytesPerPixel;
}

// CV_[The number of bits per item][Signed or Unsigned][Type Prefix]C[The channel number]
const char* GetTypeName(Mat& image)
{
	int type = image.type();
	// 一些常见的type值
	switch (type) {
	case CV_8UC1:
		return "CV_8UC1"; // 8 位无符号整数，1 通道，即灰度图像
	case CV_8UC2:
		return "CV_8UC2"; // 8 位无符号整数，2 通道
	case CV_8UC3:
		return "CV_8UC3"; // 8 位无符号整数，3 通道，通常表示 BGR 彩色图像
	case CV_8UC4:
		return "CV_8UC4"; // 8 位无符号整数，4 通道
	case CV_16UC1:
		return "CV_16UC1"; // 16 位无符号整数，1 通道
	case CV_16UC3:
		return "CV_16UC3"; // 16 位无符号整数，3 通道
	case CV_16SC1:
		return "CV_16SC1"; // 16 位有符号整数，1 通道
	case CV_16SC3:
		return "CV_16SC3"; // 16 位有符号整数，3 通道
	case CV_32FC1:
		return "CV_32FC1"; // 32 位浮点数，1 通道
	case CV_32FC3:
		return "CV_32FC3"; // 32 位浮点数，3 通道
	case CV_64FC1:
		return "CV_64FC1"; // 64 位浮点数，1 通道
	case CV_64FC3:
		return "CV_64FC3"; // 64 位浮点数，3 通道
	default:
		return "default";
	}
}

void DumpImageInfo(Mat& image, const char* tag = NULL)
{
	if (tag) {
		std::cout << "Dumping image info for " << tag << "..." << std::endl;
	}
	if (image.empty()) {
		std::cout << "The image is empty!" << std::endl;
		return;
	}

	std::cout << "Dimensions: " << image.dims;
	if (image.dims >= 2) {
		std::cout << " Size: ";
		for (int i = 0; i < image.dims; i++) {
			std::cout << image.size[i];
			if (i != image.dims - 1) std::cout << " x ";
		}
		std::cout << std::endl;
	}	

	std::cout << "Width: " << image.cols << " height: " << image.rows << " channels: " << image.channels() << std::endl;
	std::cout << "Type: " << image.type() << " which means " << GetTypeName(image) << std::endl;
	std::cout << "Depth: " << image.depth() << " and then we can calculate bytes-per-pixel: " << GetBytesPerPixel(image) << std::endl;
	std::cout << "Mean: " << cv::mean(image) << std::endl;
}


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
	DumpImageInfo(gSrcImg, mSourceFile);
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
	ON_BN_CLICKED(IDC_BUTTON_DETECT_EDGE, &COpenCVMiscDlg::OnBnClickedButtonDetectEdge)
	ON_BN_CLICKED(IDC_BUTTON_FLIP, &COpenCVMiscDlg::OnBnClickedButtonFlip)
	ON_BN_CLICKED(IDC_BUTTON_ROTATE, &COpenCVMiscDlg::OnBnClickedButtonRotate)
	ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT, &COpenCVMiscDlg::OnBnClickedButtonFindObject)
	ON_BN_CLICKED(IDC_BUTTON_SCAN_ID_CARD, &COpenCVMiscDlg::OnBnClickedButtonScanIdCard)
	ON_BN_CLICKED(IDC_BUTTON_MORPHOLOGY, &COpenCVMiscDlg::OnBnClickedButtonMorphology)
	ON_BN_CLICKED(IDC_BUTTON_DETECT_FACE, &COpenCVMiscDlg::OnBnClickedButtonDetectFace)
	ON_BN_CLICKED(IDC_BUTTON_BASIC, &COpenCVMiscDlg::OnBnClickedButtonBasic)
	ON_BN_CLICKED(IDC_BUTTON_DETECT_CORNERS, &COpenCVMiscDlg::OnBnClickedButtonDetectCorners)
	ON_BN_CLICKED(IDC_BUTTON_FIND_OBJECT_SIFT, &COpenCVMiscDlg::OnBnClickedButtonFindObjectSift)
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

	mBtnBasic.SubclassDlgItem(IDC_BUTTON_BASIC, this); // 定制按钮
	mBtnBasic.SetIcon(IDI_ICON_LAB);
	mBtnBasic.SizeToContent();

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
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Image Files (*.jpg;*.jpeg;*.png;*.tif;*.bmp;*.webp)|*.jpg;*.jpeg;*.png;*.tif;*.bmp;*.webp|All Files (*.*)|*.*||"), NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		mSourceFile = fileDlg.GetPathName(); 
		UpdateData(FALSE);

		gSrcImg = imread((LPCTSTR)mSourceFile, IMREAD_COLOR); // Read the file
		if (gSrcImg.empty()) {
			::AfxMessageBox("Failed to open the source file.");
			return;
		}

		ShowOriginalImage();
		DumpImageInfo(gSrcImg, mSourceFile);
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

Mat& ScanImageAndReduceC(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() == CV_8U);

	int channels = I.channels();

	int nRows = I.rows;
	int nCols = I.cols * channels;

	// Mat中存储的数据 行与行之间在内存上未必是连续的。如果是连续的（可以通过Mat::isContinuous来判断），可以提升处理性能
	if (I.isContinuous())
	{
		nCols *= nRows;
		nRows = 1;
	}

	int i, j;
	uchar* p;
	for (i = 0; i < nRows; ++i)
	{
		p = I.ptr<uchar>(i);
		for (j = 0; j < nCols; ++j)
		{
			p[j] = table[p[j]];
		}
	}
	return I;
}

Mat& ScanImageAndReduceIterator(Mat& I, const uchar* const table)
{
	// accept only char type matrices
	CV_Assert(I.depth() == CV_8U);

	const int channels = I.channels();
	switch (channels)
	{
	case 1:
	{
		MatIterator_<uchar> it, end;
		for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
			*it = table[*it];
		break;
	}
	case 3:
	{
		MatIterator_<Vec3b> it, end;
		for (it = I.begin<Vec3b>(), end = I.end<Vec3b>(); it != end; ++it)
		{
			(*it)[0] = table[(*it)[0]];
			(*it)[1] = table[(*it)[1]];
			(*it)[2] = table[(*it)[2]];
		}
	}
	}

	return I;
}

void COpenCVMiscDlg::OnBnClickedButtonBasic()
{
	Mat matrix1;
	matrix1.create(4, 4, CV_8UC(2)); // 4行，4列，8bit-2通道
	std::cout << "M = " << std::endl << matrix1 << std::endl;

	cv::randu(matrix1, Scalar::all(0), Scalar::all(255)); // 随机数
	std::cout << "M filled with random numbers >>> " << std::endl << matrix1 << std::endl;
	matrix1.row(0).setTo(Scalar(0));
	std::cout << "The first row set to 0 >>> " << std::endl << matrix1 << std::endl;
	matrix1.col(matrix1.cols-1).setTo(Scalar(0));
	std::cout << "The last column set to 0 >>> " << std::endl << matrix1 << std::endl;
	std::cout << std::endl;

	// 2x2x2 的三维矩阵
	int sz[] = { 2, 2, 2 };
	Mat matrix2(3, sz, CV_8UC(1), Scalar::all(0));
	DumpImageInfo(matrix2, "Basic Matrix");

	// MATLAB style initializer: cv::Mat::zeros , cv::Mat::ones , cv::Mat::eye 
	Mat E = Mat::eye(4, 4, CV_64F);
	std::cout << "E = " << std::endl << E << std::endl;
	Mat Z = Mat::zeros(3, 3, CV_8UC1);
	std::cout << "Z = " << std::endl << Z << std::endl;
	Mat O = Mat::ones(2, 2, CV_32F);
	std::cout << "O = " << std::endl << O << std::endl;

	Mat C = (Mat_<double>(3, 3) << 0, -1, 0,
								   -1, 5, -1,
								   0, -1, 0);
	std::cout << "C = " << std::endl << C << std::endl;
	Mat RowClone = C.row(1).clone(); // 克隆C的第二行
	std::cout << "The 2nd row of C cloned >>> " << std::endl << RowClone << std::endl;

#if 0 // 测试自建图像
	Mat myImg;
	myImg.create(300, 300, CV_8UC(3)); // 自建 300x300 BGR图像
	myImg = Scalar(0); // 黑图
	imshow("Creat an image - Black", myImg);
	cv::randu(myImg, Scalar::all(0), Scalar::all(255)); // 随机数
	imshow("Creat an image - Random", myImg);
	Rect roi(10, 10, 100, 100);
	Mat smallImg = myImg(roi);
	imshow("Creat an image - ROI (Region Of Interest)", smallImg);
#endif 


	/////////////////////////////////////////////////////////////////////////////////
	// 演练：go through an image pixel by pixel
	// 通过"查表"替换图像中的每个像素值
	uchar table[256];
	for (int i = 0; i < 256; ++i)
		table[i] = (uchar)(30 * (i / 30)); // 减少颜色数量

	Mat srcImage = imread(".\\assets\\cat.png");
	Mat testImage = srcImage.clone();
	imshow("TestImage - Original", testImage);

#if 0 // 练习：随机访问像素点(x, y)，注意C++中的坐标点与OpenCV的行/列的对应详细
	int channelCount = testImage.channels();
	int x = 1;
	int y = 2;
	if (channelCount == 1) {
		Scalar intensity1 = testImage.at<uchar>(y, x);
		// 等价于：
		Scalar intensity2 = testImage.at<uchar>(Point(x, y));
	}
	else if (channelCount == 3) { // BRG
		Vec3b intensity = testImage.at<Vec3b>(y, x);
		uchar blue = intensity.val[0];
		uchar green = intensity.val[1];
		uchar red = intensity.val[2];
	}
#endif // 1 // 练习：随机访问像素点


	// how do we measure time? 
	double t = (double)cv::getTickCount();
	// do something ...
	ScanImageAndReduceC(testImage, table);
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	std::cout << "Times passed in seconds: " << t << std::endl;
	imshow("TestImage - Color reduction", testImage);

	Mat testI2 = srcImage.clone();
	Mat result;
	// 使用OpenCV内置的查表函数，效率更高！
	Mat lookUpTable(1, 256, CV_8U);
	uchar* p = lookUpTable.ptr();
	for (int i = 0; i < 256; ++i)
		p[i] = table[i];
	t = (double)cv::getTickCount();
	cv::LUT(testI2, lookUpTable, result);
	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
	std::cout << "Times passed in seconds (LUT): " << t << std::endl;
	imshow("TestImage - Color reduction - LUT", result);
	
}

void COpenCVMiscDlg::ShowOriginalImage()
{
	if (gSrcImg.empty()) return;

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

// 调整画面 亮度/对比度，以及非线性优化：gamma校正
// 处理方法：newPixel(i, j) = alpha * originalPixel(i, j) + beta
// https://docs.opencv.org/4.10.0/d3/dc1/tutorial_basic_linear_transform.html
void COpenCVMiscDlg::OnBnClickedButtonProcessPixels()
{
	Mat new_image = Mat::zeros(gSrcImg.size(), gSrcImg.type());

	double alpha = 1.3; /*< Simple contrast control, [1.0-3.0] */
	int beta = 40;       /*< Simple brightness control, [0-100] */

	// 遍历每个像素
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
	cv::addWeighted(src1, alpha, src2, beta, 0.0, dst); // dst = alpha * src1 + beta * src2 + gamma
	imshow("Linear Blend", dst);
}



// 图像处理函数 #include <opencv2/imgproc.hpp>
// https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html
void COpenCVMiscDlg::OnBnClickedButtonGrayscale()
{
	if (gSrcImg.empty()) return;

	Mat grey;
	cv::cvtColor(gSrcImg, grey, COLOR_BGR2GRAY);
	imshow("Grayscale", grey);
	//std::cout << "M = " << std::endl << grey << std::endl;

	// 对灰度图像进行二值化处理（即为黑白图像）
	// 工作原理：
	//	1. 当像素值大于阈值时，将该像素值设置为maxval（通常为 255）。
	//	2. 当像素值小于等于阈值时，将该像素值设置为 0。
	Mat binaryImage;
	cv::threshold(grey, binaryImage, 127, 255, THRESH_BINARY);
	imshow("Black and White", binaryImage);

	cv::threshold(grey, binaryImage, 127, 255, THRESH_BINARY_INV); // 黑白翻转
	imshow("Black and White - inverse", binaryImage);
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


	// 测试filter2D，通过一个kernal / mask 让每个像素与周围的像素进行运算
	// 均值滤波核
	Mat kernel_mean = Mat::ones(5, 5, CV_32F) / 25.0;
	std::cout << "kernel_mean = " << std::endl << kernel_mean << std::endl;
	Mat filtered1;
	filter2D(gSrcImg, filtered1, -1, kernel_mean);
	imshow("filter2D-1 blur", filtered1);

	// 拉普拉斯边缘检测核
	Mat kernel_laplacian = (Mat_<float>(3, 3) << 0, -1, 0, 
												-1, 4, -1,
												0, -1, 0);
	Mat filtered2;
	filter2D(gSrcImg, filtered2, -1, kernel_laplacian);
	imshow("filter2D-2 edge", filtered2);
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

	// 图片上叠加文字
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

void COpenCVMiscDlg::OnBnClickedButtonFlip()
{
	// 翻转方法：
	//	0 - 围绕X轴
	//	1 - 围绕Y轴
	//	-1 - X轴Y轴同时翻转
	Mat flipped1, flipped2;

	cv::flip(gSrcImg, flipped1, 0);
	imshow("Flip around x-axis", flipped1);

	cv::flip(gSrcImg, flipped2, 1);
	imshow("Flip around y-axis", flipped2);
}

void COpenCVMiscDlg::OnBnClickedButtonRotate()
{
	if (gSrcImg.empty()) return;

	// 获取图像大小
	int width = gSrcImg.cols;
	int height = gSrcImg.rows;
	Mat mtrx = cv::getRotationMatrix2D(Point2f((float)width / 2, (float)height / 2), 45, 1);
	Mat	rotated;
	cv::warpAffine(gSrcImg, rotated, mtrx, Size(width, height));
	imshow("Rotation", rotated);
}

void COpenCVMiscDlg::OnBnClickedButtonMorphology()
{
	if (gSrcImg.empty()) return;

	Mat processed;
	Mat srcGray = imread((LPCTSTR)mSourceFile, IMREAD_GRAYSCALE); // 灰度图

	// 膨胀
	// 膨胀的意义：1. 连接断开的区域；2. 突出和加粗物体的边界；3. 消除小的空洞和噪声；4. 增强物体的特征
	// 操作对象通常是灰度图或二值图
	Mat kernel = cv::getStructuringElement(MORPH_RECT, Size(3, 3)); 
	cv::dilate(srcGray, processed, kernel);
	imshow("Dilation", processed);

	// 腐蚀操作
	// 作用：去除孤立噪声点，消除小物体，断开狭窄连接以分离物体，细化物体轮廓
	cv::erode(srcGray, processed, kernel);
	imshow("Erode", processed);

	// 开运算（由腐蚀操作后跟膨胀操作组成）
	// 作用：去除小的物体，平滑物体边界
	cv::morphologyEx(srcGray, processed, MORPH_OPEN, kernel);
	imshow("Morphology - open", processed);

	// 闭运算（由膨胀操作后跟腐蚀操作组成）
	// 作用：填充小孔和间隙，平滑物体边界
	cv::morphologyEx(srcGray, processed, MORPH_CLOSE, kernel);
	imshow("Morphology - close", processed);

	// 形态学梯度（先对原始图像进行膨胀，再用膨胀后的图像减去腐蚀后的图像）
	// 作用：突出物体的边缘，增强图像的对比度
	cv::morphologyEx(srcGray, processed, MORPH_GRADIENT, kernel);
	imshow("Morphology - Gradient", processed);

	// 顶帽操作（用原始图像减去其开运算的结果）
	// 作用：提取明亮区域的细节，校正不均匀照明
	cv::morphologyEx(srcGray, processed, MORPH_TOPHAT, kernel);
	imshow("Morphology - TopHat", processed);

	// 黑帽操作（用闭运算的结果减去原始图像）
	// 作用：提取暗区域的细节，增强暗物体的对比度
	cv::morphologyEx(srcGray, processed, MORPH_BLACKHAT, kernel);
	imshow("Morphology - BlackHat", processed);
}

// 边缘检测的几种算法
void COpenCVMiscDlg::OnBnClickedButtonDetectEdge()
{
	if (mSourceFile.IsEmpty()) return;

	Mat srcGray = imread((LPCTSTR)mSourceFile, IMREAD_GRAYSCALE); // 灰度图
	Mat absImage;

	// Canny边缘检测
	Mat cannyEdges;
	cv::Canny(srcGray, cannyEdges, 100, 200);
	imshow("Edges - Canny", cannyEdges);

	// Laplacian边缘检测
	Mat lapEdges;
	cv::Laplacian(srcGray, lapEdges, CV_64F, 3);
	cv::convertScaleAbs(lapEdges, absImage); // 转换为 8 位无符号整数以便显示
	imshow("Edges - Laplacian", absImage);

	// Sobel边缘检测
	Mat sobelx, sobely, sobelImage;
	cv::Sobel(srcGray, sobelx, CV_64F, 1, 0, 5);
	cv::Sobel(srcGray, sobely, CV_64F, 0, 1, 5);
	cv::addWeighted(sobelx, 0.5, sobely, 0.5, 0, sobelImage);
	imshow("Edges - Sobel", sobelImage);
}

void COpenCVMiscDlg::OnBnClickedButtonDetectCorners()
{
	if (mSourceFile.IsEmpty()) return;

	Mat srcImage = imread((LPCTSTR)mSourceFile);
	if (srcImage.empty()) return;

	Mat srcGray;
	cv::cvtColor(srcImage, srcGray, COLOR_BGR2GRAY);

	std::vector<Point2f> corners;
	cv::goodFeaturesToTrack(srcGray, corners, 100, 0.01, 10);
	// 绘制角点
	for (const Point2f& corner : corners) {
		cv::circle(srcImage, corner, 3, Scalar(0, 255, 0), -1);
	}

	imshow("Corners", srcImage);
}



// 综合应用
// 图像/物体匹配：源图像与模板图像须保持相同的分辨率
void COpenCVMiscDlg::OnBnClickedButtonFindObject()
{
	CAutoTicker ticker("FindObject");
	_FindObjectInImage();
}

void COpenCVMiscDlg::_FindObjectInImage()
{
	Mat srcImage, targetObj;
	srcImage = imread(".\\assets\\wukong.jpg");
	targetObj = imread(".\\assets\\bsl.png", IMREAD_GRAYSCALE); // 灰度图
	if (srcImage.empty() || targetObj.empty()) return;

#ifdef DEBUG
	std::cout << "Source type: " << srcImage.type() << " Template type: " << targetObj.type() << std::endl;
#endif // DEBUG

	// 使用模板匹配算法进行比对
	Mat result, srcGray;
	//int result_cols = srcImage.cols - targetObj.cols + 1;
	//int result_rows = srcImage.rows - targetObj.rows + 1;
	//result.create(result_rows, result_cols, CV_32FC1);
	cv::cvtColor(srcImage, srcGray, COLOR_BGR2GRAY); // 灰度化处理
	cv::matchTemplate(srcGray, targetObj, result, TM_CCOEFF_NORMED);

	// 获取比对结果中大于阈值的位置
	double threshValue = 0.9;
	Mat threshResult;
	cv::threshold(result, threshResult, threshValue, 1.0, THRESH_BINARY);

	Mat locations;
	cv::findNonZero(threshResult, locations);
#ifdef DEBUG
	std::cout << "Valid locations: " << locations.total() << std::endl;
#endif // DEBUG

	if (!locations.empty()) {
		for (int i = 0; i < locations.total(); i++) {
			Point p = locations.at<Point>(i);
			cv::rectangle(srcImage, p, Point(p.x + targetObj.cols, p.y + targetObj.rows), Scalar(0, 255, 0), 1);
		}
		imshow("Find Object", srcImage);
	}
	else {
		::AfxMessageBox("Object Not Found!");
	}
}

Rect combineRects(const std::vector<Rect>& rects) {
	int minX = INT_MAX, minY = INT_MAX;
	int maxX = 0, maxY = 0;

	for (const Rect& rect : rects) {
		minX = std::min(minX, rect.x);
		minY = std::min(minY, rect.y);
		maxX = std::max(maxX, rect.x + rect.width);
		maxY = std::max(maxY, rect.y + rect.height);
	}

	return Rect(minX, minY, maxX - minX, maxY - minY);
}

void COpenCVMiscDlg::_DetectIDCard_WithBadDilation()
{
	Mat srcImage = imread(".\\assets\\pigidcard.png");
	if (srcImage.empty()) return;

	// 将图像转换为灰度图
	Mat grayImg;
	cv::cvtColor(srcImage, grayImg, COLOR_BGR2GRAY);

	// 对灰度图像进行二值化处理（注：THRESH_OTSU会自动选择最优的阈值 而忽略函数参数指定的阈值）
	Mat binary;
	cv::threshold(grayImg, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	// （不充分）膨胀操作 —— 合并小区域，但不足以把身份证号码去连成一片！
	Mat kernel = cv::getStructuringElement(MORPH_RECT, Size(5, 5));
	Mat dilation;
	cv::dilate(binary, dilation, kernel);
	//imshow("ID Card - dilation", dilation);

	// 查找轮廓
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	cv::findContours(dilation, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// 遍历轮廓进行筛选
	const int idNumCount = 18;
	std::vector<Rect> roiRects;
	Rect firstRect;
	if (!contours.empty()) {
		firstRect = cv::boundingRect(contours[0]);
	}
	for (size_t i = 0; i < contours.size(); i++) {
		double area = cv::contourArea(contours[i]);
		Rect roi = cv::boundingRect(contours[i]);
#if 0		
		std::cout << "轮廓 " << i << " 的面积：" << area << " 点数：" << contours[i].size() << std::endl;
		for (size_t j = 0; j < contours[i].size(); j++) {
			Point p = contours[i][j];
			std::cout << "点坐标：(" << p.x << ", " << p.y << ")" << std::endl;
		}
#endif

#if 0
		// 队列中的轮廓逐个画出
		if (i < 20) {
			cv::rectangle(srcImage, roi, Scalar(0, 255, 0), 2);
		}
#endif 	

		double aspectRatio = (double)roi.width / roi.height;
		// 假设身份证号码区域的面积较大且长宽比在一定范围内
		// 根据实际情况调整这些阈值
		if (area > 300 && area < 2000 && aspectRatio < 0.8) {
			//cv::rectangle(srcImage, roi, Scalar(0, 255, 0), 2);
			// 将处于同一水平线的轮廓归纳在一起
			if (abs(roi.y - firstRect.y) < 30) {
				roiRects.push_back(roi);
				if (roiRects.size() >= idNumCount) break;
			}
		}
	}

	if (roiRects.size() >= idNumCount) {
		Rect combinedRect = combineRects(roiRects); // 将身份证号码区域合并
		cv::rectangle(srcImage, combinedRect, Scalar(0, 255, 0), 2);

		Mat cropped = binary(combinedRect);
		imshow("ID Card - number only", cropped);

		// 使用 Tesseract OCR
		// 参见_DetectIDCard_WithGoodDilation()的实现
	}

	imshow("ID Card", srcImage);
}

void COpenCVMiscDlg::_DetectIDCard_WithGoodDilation()
{
	Mat srcImage = imread(".\\assets\\pigidcard.png");
	if (srcImage.empty()) return;

	// 将图像转换为灰度图
	Mat grayImg;
	cv::cvtColor(srcImage, grayImg, COLOR_BGR2GRAY);
	//DumpImageInfo(grayImg, "gray image");

	// 对灰度图像进行二值化处理（注：THRESH_OTSU会自动选择最优的阈值 而忽略函数参数指定的阈值）
	Mat binary;
	cv::threshold(grayImg, binary, 0, 255, THRESH_BINARY_INV | THRESH_OTSU);

	// 对二值化图像进行膨胀操作（重要！！！）
	// 目标：通过调整Size参数，让身份证号码区域连接成一个整体，便于后续的裁剪
	Mat kernel = cv::getStructuringElement(MORPH_RECT, Size(26, 26));
	Mat dilation;
	cv::dilate(binary, dilation, kernel);
	//imshow("ID Card - dilation", dilation);

	// 在膨胀后的图像中查找轮廓
	std::vector<std::vector<Point>> contours;
	std::vector<Vec4i> hierarchy;
	cv::findContours(dilation, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	// 遍历轮廓进行筛选
	for (size_t i = 0; i < contours.size(); i++) {
		double area = cv::contourArea(contours[i]);
		Rect roi = cv::boundingRect(contours[i]);
#if 0		
		std::cout << "轮廓 " << i << " 的面积：" << area << " 点数：" << contours[i].size() << std::endl;
		for (size_t j = 0; j < contours[i].size(); j++) {
			Point p = contours[i][j];
			std::cout << "点坐标：(" << p.x << ", " << p.y << ")" << std::endl;
		}
#endif

#if 0
		// 队列中的轮廓逐个画出
		if (i < 2) {
			//cv::drawContours(srcImage, contours, i, Scalar(0, 255, 0));
			cv::rectangle(srcImage, roi, Scalar(0, 255, 0), 2);
		}
#endif 	

		double aspectRatio = (double)roi.width / roi.height;
		// 假设身份证号码区域的面积较大且长宽比在一定范围内
		// 根据实际情况调整这些阈值
		if (area > 40000 && aspectRatio > 10) {
			cv::rectangle(srcImage, roi, Scalar(0, 255, 0), 2);

			Mat cropped = binary(roi);
			imshow("ID Card - number only", cropped);

			// 使用 Tesseract OCR
#ifdef _ENABLE_TESSERACT_
			tesseract::TessBaseAPI tess;
			if (tess.Init("tessdata", "eng") == 0) {
				tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);

				// Tesseract只支持输入 24bit 图像？
				Mat ocrImg;
				cv::cvtColor(cropped, ocrImg, COLOR_GRAY2BGR);
				//imshow("ID Card - image for OCR", ocrImg);
				//DumpImageInfo(ocrImg, "OCR");
				int bytesPerPixel = GetBytesPerPixel(ocrImg);
				tess.SetImage((uchar*)ocrImg.data, ocrImg.cols, ocrImg.rows, bytesPerPixel, ocrImg.cols * bytesPerPixel);

				char* outText = tess.GetUTF8Text();
				std::cout << "ID numbers: " << outText << std::endl;
				delete[] outText;

				tess.End();
			}
			
#endif // _ENABLE_TESSERACT_

			break;
		}
	}

	imshow("ID Card", srcImage);
}

// 识别身份证号码
void COpenCVMiscDlg::OnBnClickedButtonScanIdCard()
{
	_DetectIDCard_WithGoodDilation();
}

void COpenCVMiscDlg::OnBnClickedButtonDetectFace()
{
	cv::CascadeClassifier faceClassifier;
	// 加载级联分类器
	// load haarcascade_eye.xml for detecting eyes
	// load haarcascade_frontalcatface.xml for detecting cat's face: cat.png
	if (!faceClassifier.load(".\\assets\\haarcascade_frontalface_alt.xml")) {
		std::cout << "Failed to load classifier." << std::endl;
		return;
	}

	Mat srcImage = imread(".\\assets\\ruok.png");
	if (srcImage.empty()) return;

	// 灰度化处理
	Mat gray;
	cv::cvtColor(srcImage, gray, COLOR_BGR2GRAY);
	// 直方图均衡化，增强对比度
	cv::equalizeHist(gray, gray);

	std::vector<Rect> faces;
	faceClassifier.detectMultiScale(gray, faces);
	for each (Rect face in faces) {
		cv::rectangle(srcImage, face, Scalar(0, 255, 0));
	}

	imshow("Face Detection", srcImage);
}

/*
* SIFT（Scale-Invariant Feature Transform，尺度不变特征变换）算法
* 即使物体在图像中的大小、方向、光照条件不同，SIFT 算法也能提取出稳定的特征，从而实现准确识别。
* 典型应用：图像识别与分类、图像拼接（e.g.全景图）、医学影像处理（CT 扫描、MRI图像中的病变检测）、文物保护与修复（碎片拼接）、机器人视觉
* 
* DMatch结构用于表示两个特征点之间的匹配关系，成员变量释义：
* - queryIdx：表示查询图像中特征点的索引。在特征匹配过程中，通常有两幅图像，一幅被称为查询图像（query image），另一幅被称为训练图像（train image）。这个索引指向查询图像中被匹配的特征点在特征点容器中的位置。
* - trainIdx：表示训练图像中特征点的索引。指向训练图像中与查询图像中的特征点相匹配的特征点在特征点容器中的位置。
* - imgIdx：在多幅图像匹配的情况下，表示匹配的特征点所在的图像索引。如果只对两幅图像进行匹配，这个变量通常不被使用。
* - distance：表示两个特征点描述子之间的距离。距离越小，说明两个特征点越相似，匹配度越高。通常使用某种距离度量方法，如欧氏距离或汉明距离，来计算描述子之间的距离。
*/
void COpenCVMiscDlg::_FindImageMatches()
{
	Mat img1, img2;
	img1 = imread(".\\assets\\wukong.jpg");
	img2 = imread(".\\assets\\bsl2.png"); // 与源图像的分辨率可以不同
	if (img1.empty() || img2.empty()) return;

	// 创建 SIFT 特征检测器
	cv::Ptr<SIFT> sift = SIFT::create();

	// 检测关键点和计算描述子
	std::vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	sift->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
	sift->detectAndCompute(img2, noArray(), keypoints2, descriptors2);

	// 创建特征匹配器
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);

	// 匹配特征点
	// {外层向量}的每一个元素对应查询图像中的一个特征点；
	// 对于每个查询特征点，{内层向量}存储了该点与训练图像中特征点的一组最近邻匹配结果，数量由参数k决定
	std::vector<std::vector<DMatch>> knnMatches;
	matcher->knnMatch(descriptors1, descriptors2, knnMatches, 2); // 内层向量中存储2个最接近的匹配结果
	std::cout << "The total number of matches: " << knnMatches.size() << std::endl;

	// 筛选好的匹配点
	// 距离越小，说明两个特征点越相似，匹配度越高。
	const float ratioThreshold = 0.50f;
	std::vector<DMatch> goodMatches;
	for (size_t i = 0; i < knnMatches.size(); i++) {
		if (knnMatches[i][0].distance < ratioThreshold * knnMatches[i][1].distance) {
			goodMatches.push_back(knnMatches[i][0]);
		}
	}
	std::cout << "The count of good matches: " << goodMatches.size() << std::endl;

	// 尝试绘制矩形区域
	std::vector<Point2f> points1, points2;
	for (const DMatch& match : goodMatches) {
		points1.push_back(keypoints1[match.queryIdx].pt);
		points2.push_back(keypoints2[match.trainIdx].pt);
	}
	if (!points1.empty() && !points2.empty() && goodMatches.size() > 50) { // 通过好点数量来判断是否真的匹配，须依据实际情况修改！
		// 获取最小包围矩形
		Rect rect1 = cv::boundingRect(points1);
		Rect rect2 = cv::boundingRect(points2);

		cv::rectangle(img1, rect1, Scalar(0, 255, 0), 2);
		cv::rectangle(img2, rect2, Scalar(0, 255, 0), 2);
		//imshow("Matches - Image1", img1);
		//imshow("Matches - Image2", img2);
		std::cout << "Found matches successfully." << std::endl;
	}
	else {
		std::cout << "It is NOT a good match!" << std::endl;
	}

	// 绘制匹配结果
	Mat imgMatches;
	cv::drawMatches(img1, keypoints1, img2, keypoints2, goodMatches, imgMatches, Scalar::all(-1),
		Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
	imshow("SIFT matches", imgMatches);
}

void COpenCVMiscDlg::OnBnClickedButtonFindObjectSift()
{
	CAutoTicker ticker("FindObject - SIFT");
	_FindImageMatches();
}
