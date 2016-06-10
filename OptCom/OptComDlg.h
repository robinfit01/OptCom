
// OptComDlg.h : ͷ�ļ�
//

#pragma once


// COptComDlg �Ի���
class COptComDlg : public CDialogEx
{
// ����
public:
	COptComDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTCOM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

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

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
