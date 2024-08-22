
// OpenCVMiscDlg.h: 头文件
//

#pragma once


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

private:
	void DumpImageInfo();
	void ShowOriginalImage();
	void _FindObjectInImage();

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
};
