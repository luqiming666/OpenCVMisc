
// OpenCVMiscDlg.h: 头文件
//

#pragma once

#include "BtnST.h" // https://www.codeproject.com/Articles/57/CButtonST-v3-9-MFC-Flat-buttons

// COpenCVMiscDlg 对话框
class COpenCVMiscDlg : public CDialogEx
{
// 构造
public:
	COpenCVMiscDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVMISC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CString mSourceFile;
	CButtonST mBtnBasic;

private:
	void ShowOriginalImage();
	void _FindObjectInImage();
	void _FindImageMatches();
	void _DetectIDCard_WithGoodDilation();
	void _DetectIDCard_WithBadDilation();

public:	
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedButtonCloseAllWin();
	afx_msg void OnBnClickedButtonShowOriginal();
	afx_msg void OnBnClickedButtonProcessPixels();
	afx_msg void OnBnClickedButtonGrayscale();
	afx_msg void OnBnClickedButtonBlur();
	afx_msg void OnBnClickedButtonBlend2Images();
	afx_msg void OnBnClickedButtonbilateralfilter();
	afx_msg void OnBnClickedButtonResize();
	afx_msg void OnBnClickedButtonDetectEdge();
	afx_msg void OnBnClickedButtonFlip();
	afx_msg void OnBnClickedButtonRotate();
	afx_msg void OnBnClickedButtonFindObject();
	afx_msg void OnBnClickedButtonScanIdCard();
	afx_msg void OnBnClickedButtonMorphology();
	afx_msg void OnBnClickedButtonDetectFace();
	afx_msg void OnBnClickedButtonBasic();
	afx_msg void OnBnClickedButtonDetectCorners();
	afx_msg void OnBnClickedButtonFindObjectSift();
	afx_msg void OnBnClickedButtonDetectInVideo();
	afx_msg void OnBnClickedButtonDetectAndTrack();
	afx_msg void OnBnClickedButtonWarp();
	afx_msg void OnBnClickedButtonSplitMerge();
	afx_msg void OnBnClickedButtonBorder();
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnBnClickedButtonGooglenet();
};
