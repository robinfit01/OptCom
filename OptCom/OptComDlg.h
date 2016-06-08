
// OptComDlg.h : 头文件
//

#pragma once


// COptComDlg 对话框
class COptComDlg : public CDialogEx
{
// 构造
public:
	COptComDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTCOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	SerialCom scom;
	static CWinThread* pComListenThread;
	static bool s_bExit;

public:
	void UpdateCombox();
	void InitCBRCombox();
	void InitCodingModeCombox();
	void ResetInfoStatic();
	void UpdateProgress(ULONGLONG finish, ULONGLONG total);
	SerialCom* getSerialComPtr();
	static UINT ComListenThreadProc(LPVOID pParam);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnClickedCOMport();
	DECLARE_MESSAGE_MAP()
public:
	
	afx_msg void OnCbnSelchangeCbrcombo();

};
