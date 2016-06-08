
// OptComDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
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


// COptComDlg �Ի���



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
END_MESSAGE_MAP()


// COptComDlg ��Ϣ�������

BOOL COptComDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SerialCom scom;

	UpdateCombox();

	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);

	((CEdit*)GetDlgItem(IDC_FILEDIR))->SetWindowTextW(L"\\\\Mac\\Home\\Desktop\\test.txt");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COptComDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
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
		
		if (!pCOptComDlg->scom.InitCom()) {
			pCOptComDlg->MessageBox(L"COM port open error!");
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
				return 0;
			}
			catch (CFileException *e)
			{
				CString errStr;
				errStr.Format(L"File error! Error code:%d", e->m_cause);
				pCOptComDlg->MessageBox(errStr);
				pCOptComDlg->scom.ClosePort();
				return 1;
			}
		}
		
	}
	else
	{
		if (!pCOptComDlg->scom.InitCom()) {
			pCOptComDlg->MessageBox(L"COM port open error!");
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
					FileLength -= COMBufferLength;
					if (FileLength <= 0)break;
				}
				fs.close();
				// targetFile.Close();
				pCOptComDlg->scom.ClosePort();
				return 0;
			}
			catch (CFileException *e)
			{
				CString errStr;
				errStr.Format(L"File error! Error code:%d", e->m_cause);
				pCOptComDlg->MessageBox(errStr);
				pCOptComDlg->scom.ClosePort();
				return 1;
			}
		}
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COptComDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void COptComDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

UINT COptComDlg::OnNcHitTest(CPoint point)
{
	int ret = CDialog::OnNcHitTest(point);

	//if����ǰ������������ֹ�ı��С�ģ����һ����������ֹ�ƶ���
	if(HTTOP == ret || HTBOTTOM == ret || HTLEFT == ret || HTRIGHT == ret)
		return HTCLIENT;
	return ret;
}


void COptComDlg::OnBnClickedSend()
{
	s_bExit = 1;
	this->pComListenThread = AfxBeginThread(ComListenThreadProc,this, THREAD_PRIORITY_NORMAL);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	scom.SetComPort(((CComboBox*)GetDlgItem(IDC_PORTCOMBO))->GetCurSel());
}

void COptComDlg::OnCbnClickedCOMport()
{
	UpdateCombox();
}
