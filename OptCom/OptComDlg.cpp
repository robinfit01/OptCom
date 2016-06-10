
// OptComDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OptCom.h"
#include "OptComDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Definition of the static variables
CWinThread* COptComDlg::pComListenThread = NULL;
bool COptComDlg::s_bExit = FALSE;

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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

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


// COptComDlg 对话框



COptComDlg::COptComDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPTCOM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COptComDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void COptComDlg::UpdateCombox()
{
	((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->ResetContent();
	
	if (scom.UpdateComList())
	{
		((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->SetCueBanner(L"Select");
		int strLenght = 0;
		CString str;
		for (int i = 0; i < scom.GetComListSize(); i++) {
			str.Format(L"COM%d  --  %s",scom.GetComNoOfList(i),scom.GetComName(i));
			((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->AddString(str);
			strLenght = str.GetLength()>strLenght ? str.GetLength() : strLenght;
		}
		((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->SetDroppedWidth(strLenght * 6 + 3);
	}
	else
	{
		((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->AddString(L"No COM port available!");
		((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->SetCurSel(0);
		((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->SetDroppedWidth(CString("No COM port available!").GetLength() * 6 + 3);
	}
}

void COptComDlg::InitCBRCombox()
{
	CComboBox* CurCombox = (CComboBox*)GetDlgItem(IDC_CBRCOMBO);
	CString ComBoxStr;
	int strLenght = 0;
	int CBRList[29] = { 300, 600, 1200, 1800, 2400, 4000, 4800, 7200, 9600, 14400, 16000, 19200, 28800, 38400,
					51200, 56000, 57600, 64000, 76800, 115200, 128000, 153600, 230400, 250000, 256000,
					460800, 500000, 576000, 921600 };
	

	for (int i = 0; i < 29; i++)
	{
		ComBoxStr.Format(L"%d", CBRList[i]);
		CurCombox->AddString(ComBoxStr);
		CurCombox->SetItemData(i, CBRList[i]);
		strLenght = ComBoxStr.GetLength()>strLenght ? ComBoxStr.GetLength() : strLenght;
	}
	CurCombox->SetDroppedWidth(strLenght * 6 + 3);
	CurCombox->SetMinVisibleItems(10);
	CurCombox->SetCurSel(8);
}

void COptComDlg::InitCodingModeCombox()
{
	CComboBox* CurCombox = (CComboBox*)GetDlgItem(IDC_CODECOMBO);
	
	CurCombox->AddString(L"None");
	CurCombox->AddString(L"Compressed only");
	CurCombox->AddString(L"FEC only");
	CurCombox->AddString(L"Both");
	CurCombox->SetCurSel(0);

	CurCombox->SetDroppedWidth(CString(L"Compressed only").GetLength() * 6 + 12);
#ifdef DEBUG
	afxDump << CString(L"Compressed only").GetLength();
#endif // DEBUG
}

void COptComDlg::ResetInfoStatic()
{
	GetDlgItem(IDC_INFOSTATIC)->SetWindowTextW(L"");
	((CButton*)GetDlgItem(IDC_SENDBUTTON))->SetWindowTextW(L"Start");
}

void COptComDlg::UpdateProgress(ULONGLONG finish, ULONGLONG total)
{
	CProgressCtrl * CurProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	CString str;
	int unit = 0;

	CurProgressCtrl->SetPos(finish*100/total);
	str.Format(L"%0.1f%%\n", finish / (double)total * 100);
	
	while (finish>1024)
	{
		unit++;
		finish = finish / 1024;
	}
	switch (unit)
	{
	case 0:
		str.Format(L"%s%dB/", str,finish);
		break;
	case 1:
		str.Format(L"%s%dKB/", str,finish);
		break;
	case 2:
		str.Format(L"%s%dMB/", str,finish);
		break;
	case 3:
		str.Format(L"%s%dGB/", str,finish);
		break;
	default:
		break;
	}

	unit = 0;
	while (total>1024)
	{
		unit++;
		total = total / 1024;
	}
	switch (unit)
	{
	case 0:
		str.Format(L"%s%dB", str,total);
		break;
	case 1:
		str.Format(L"%s%dKB", str,total);
		break;
	case 2:
		str.Format(L"%s%dMB", str,total);
		break;
	case 3:
		str.Format(L"%s%dGB", str,total);
		break;
	default:
		break;
	}

	GetDlgItem(IDC_INFOSTATIC)->SetWindowTextW(str);
}

BEGIN_MESSAGE_MAP(COptComDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO1, &COptComDlg::OnBnClickedRadio1)
	//ON_WM_NCHITTEST()
	ON_CBN_SELCHANGE(IDC_COMBO1, &COptComDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_SENDBUTTON,&COptComDlg::OnBnClickedSend)
	ON_BN_CLICKED(IDC_BROWSEBUTTON, &COptComDlg::OnBnClickedBrowse)
	ON_CBN_DROPDOWN(IDC_PORTCOMBO,&COptComDlg::OnCbnClickedCOMport)
	ON_CBN_SELCHANGE(IDC_CBRCOMBO, &COptComDlg::OnCbnSelchangeCbrcombo)
END_MESSAGE_MAP()


// COptComDlg 消息处理程序

BOOL COptComDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SerialCom scom;

	UpdateCombox();
	InitCBRCombox();
	InitCodingModeCombox();

	GetDlgItem(IDC_INFOSTATIC)->SetWindowTextW(L"");
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

	((CEdit*)GetDlgItem(IDC_FILEDIR))->SetWindowTextW(L"\\\\Mac\\Home\\Desktop\\test.txt");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void COptComDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void COptComDlg::OnPaint()
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

SerialCom* COptComDlg::getSerialComPtr(){
	return &scom;
}

UINT COptComDlg::ComListenThreadProc(LPVOID pParam)
{
	CString FileName;
	CString buff;
	CStdioFile targetFile;
	std::fstream fs;
	unsigned char* pBuffer;

	COptComDlg* pCOptComDlg = reinterpret_cast<COptComDlg*>(pParam);

	((CEdit*)pCOptComDlg->GetDlgItem(IDC_FILEDIR))->GetWindowTextW(FileName);

	if (((CButton*)pCOptComDlg->GetDlgItem(IDC_MODERADIO1))->GetCheck() == BST_CHECKED)
	{
		pCOptComDlg->GetDlgItem(IDC_INFOSTATIC)->SetWindowTextW(L"Starting\n...");
		if (!pCOptComDlg->scom.InitCom()) {
			pCOptComDlg->MessageBox(L"COM port open error!");
			pCOptComDlg->ResetInfoStatic();
			return 1;
		}
		else
		{
			try
			{
				// targetFile.Open(FileName, CFile::modeRead);
				fs.open(FileName,std::fstream::in | std::fstream::binary );
				//ULONGLONG FileLength = targetFile.SeekToEnd();
				if(fs){
					fs.seekg(0, std::ios::end);
					ULONGLONG FileLength = fs.tellg();
					fs.seekg(0, std::ios::beg);
	#ifdef DEBUG
					afxDump << FileLength;
	#endif // DEBUG
					// targetFile.SeekToBegin();
					pCOptComDlg->scom.WriteData((unsigned char*)(&FileLength), sizeof(ULONGLONG));
					pCOptComDlg->UpdateProgress(fs.tellg(), FileLength);
					UINT BufferLength = 0;
					while (s_bExit)
					{
						// if (targetFile.GetPosition() == FileLength) break;

						if ((ULONGLONG)fs.tellg() == FileLength) break;
						// BufferLength = FileLength - targetFile.GetPosition() >= SENDBUFFLENGHT ? SENDBUFFLENGHT : FileLength - targetFile.GetPosition();
						BufferLength = FileLength - fs.tellg() >= SENDBUFFLENGHT ? SENDBUFFLENGHT : FileLength - fs.tellg();
						// targetFile.Read(buff.GetBuffer(SENDBUFFLENGHT), SENDBUFFLENGHT);
						pBuffer = (unsigned char*)malloc(BufferLength);
						fs.read((char *)pBuffer,BufferLength);
						// buff.ReleaseBuffer();
						// pCOptComDlg->scom.WriteData((unsigned char*)(buff.GetBuffer()),BufferLength);
						pCOptComDlg->scom.WriteData(pBuffer,BufferLength);
						free(pBuffer);
						pCOptComDlg->UpdateProgress(fs.tellg(), FileLength);
	#ifdef DEBUG
						afxDump << BufferLength * 1000*8 / pCOptComDlg->scom.GetBaud();
	#endif // DEBUG
						Sleep(BufferLength * 1000 * NORMSLEEPFACTOR/ pCOptComDlg->scom.GetBaud());
					}
					Sleep(BufferLength * 1000 * LASTSLEEPFACTOR / pCOptComDlg->scom.GetBaud());
					// targetFile.Close();
					fs.close();	
				}
				else
				{
					pCOptComDlg->MessageBox(L"File open error!");
				}
				pCOptComDlg->scom.ClosePort();
				((CButton*)pCOptComDlg->GetDlgItem(IDC_SENDBUTTON))->SetWindowTextW(L"Start");
				return 0;
			}
			catch (CFileException *e)
			{
				CString errStr;
				errStr.Format(L"File error! Error code:%d", e->m_cause);
				pCOptComDlg->MessageBox(errStr);
				pCOptComDlg->scom.ClosePort();
				pCOptComDlg->ResetInfoStatic();
				return 1;
			}
		}
		
	}
	else
	{
		pCOptComDlg->GetDlgItem(IDC_INFOSTATIC)->SetWindowTextW(L"Listening\n...");
		if (!pCOptComDlg->scom.InitCom()) {
			pCOptComDlg->MessageBox(L"COM port open error!");
			pCOptComDlg->ResetInfoStatic();
			return 1;
		}
		else
		{
			try
			{

				ULONGLONG FileLength = 0;
				unsigned char c;
				while (pCOptComDlg->scom.GetBytesInCOM() < sizeof(ULONGLONG))
				{
					if (!s_bExit) {
						pCOptComDlg->scom.ClosePort();
						pCOptComDlg->ResetInfoStatic();
						return 0;
					}
				}
				
				for (int i = 0; i < sizeof(ULONGLONG); i++) 
				{
					ULONGLONG temp;
					pCOptComDlg->scom.ReadChar(c);
					temp = c;
					for (int j = i; j > 0; j--)
					{
						temp = temp*256;
					}
					FileLength += temp;
				}
				
				fs.open(FileName,std::fstream::out | std::fstream::binary);
				// targetFile.Open(FileName, CFile::modeCreate | CFile::modeWrite);
				UINT COMBufferLength = 0;
				// LPWSTR pBuffer;
				// targetFile.SeekToBegin();
				pCOptComDlg->UpdateProgress(fs.tellg(), FileLength);
				while (s_bExit)
				{
					COMBufferLength = pCOptComDlg->scom.GetBytesInCOM();
					if(!COMBufferLength) 
					{
						Sleep(WAIT_TIME);
						continue;
					}

					pBuffer = (unsigned char*)malloc(COMBufferLength);
					// buff.Empty();
					// pBuffer = buff.GetBuffer(COMBufferLength);
					for (UINT i = 0; i<COMBufferLength; i++)
					{
						pCOptComDlg->scom.ReadChar(c);
						*(pBuffer+i) = c;
					}
					
					fs.write((char*)pBuffer,COMBufferLength);
					free(pBuffer);
					// targetFile.Write(pBuffer, COMBufferLength);
					// targetFile.Flush();
					
					pCOptComDlg->UpdateProgress(fs.tellg(), FileLength);
					if ((UINT)fs.tellg()>= FileLength)break;
				}
				fs.close();
				// targetFile.Close();
				pCOptComDlg->scom.ClosePort();
				((CButton*)pCOptComDlg->GetDlgItem(IDC_SENDBUTTON))->SetWindowTextW(L"Start");
				return 0;
			}
			catch (CFileException *e)
			{
				CString errStr;
				errStr.Format(L"File error! Error code:%d", e->m_cause);
				pCOptComDlg->MessageBox(errStr);
				pCOptComDlg->scom.ClosePort();
				pCOptComDlg->ResetInfoStatic();
				return 1;
			}
		}
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR COptComDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COptComDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
}

UINT COptComDlg::OnNcHitTest(CPoint point)
{
	int ret = CDialog::OnNcHitTest(point);

	//if语句的前两行是用来禁止改变大小的，最后一行是用来禁止移动的
	if(HTTOP == ret || HTBOTTOM == ret || HTLEFT == ret || HTRIGHT == ret)
		return HTCLIENT;
	return ret;
}


void COptComDlg::OnBnClickedSend()
{
	s_bExit = 1;
	CButton * CurButton = (CButton*)GetDlgItem(IDC_SENDBUTTON);
	if (CurButton->GetWindowTextLengthW() == 5)
	{
		CurButton->SetWindowTextW(L"Stop");
		this->pComListenThread = AfxBeginThread(ComListenThreadProc,this, THREAD_PRIORITY_NORMAL);
	}
	else
	{
		s_bExit = 0;
	}

}

void COptComDlg::OnBnClickedBrowse()
{
	BOOL flag;
	if (((CButton*)GetDlgItem(IDC_MODERADIO1))->GetCheck() == BST_CHECKED)
		flag = TRUE;
	else
		flag = FALSE;

	CFileDialog	fileDlg(flag, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"All files(*.*)|*.*");
	if (fileDlg.DoModal() == IDOK)
	{
		((CEdit*)GetDlgItem(IDC_FILEDIR))->SetWindowTextW(fileDlg.GetPathName());
	}
}

void COptComDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	scom.SetComPort(((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->GetCurSel());
}

void COptComDlg::OnCbnClickedCOMport()
{
	UpdateCombox();
}


void COptComDlg::OnCbnSelchangeCbrcombo()
{
	// TODO: 在此添加控件通知处理程序代码
	scom.SetBaud(((CComboBox*)GetDlgItem(IDC_CBRCOMBO))->GetItemData(((CComboBox*)GetDlgItem(IDC_CBRCOMBO))->GetCurSel()));

}



