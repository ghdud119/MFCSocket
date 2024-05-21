
// SocketServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "SocketServer.h"
#include "SocketServerDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"
#include <WinSock2.h>
#include <WS2tcpip.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CSocketServerDlg 대화 상자



CSocketServerDlg::CSocketServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSocketServerDlg::TextOut(LPCTSTR Text)
{
	CString newText = Text;
	newText += _T("\n");
	MessageList.AddString(newText);
}

void CSocketServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, MessageList);
}

BEGIN_MESSAGE_MAP(CSocketServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Fout, &CSocketServerDlg::OnBnClickedButtonFileSave)
	ON_EN_CHANGE(IDC_EDIT_SERVERINFO, &CSocketServerDlg::OnEnChangeEditServerinfo)
	ON_EN_CHANGE(IDC_EDIT_ClientList, &CSocketServerDlg::OnEnChangeEditClientlist)
END_MESSAGE_MAP()


// CSocketServerDlg 메시지 처리기

void CSocketServerDlg::OnDestroy()
{
	CDialog::OnDestroy();

	POSITION position = serverSocket.GetClientList().GetHeadPosition();
	ClientSocket* client = NULL;

	// 생성되어있는 클라이언트 소켓이 없을때까지 체크하여 소켓닫기
	while (position != NULL) {
		client = (ClientSocket*)serverSocket.GetClientList().GetNext(position);
		if (client != NULL) {
			client->ShutDown(); // 연결된 상대방 소켓에 연결이 종료됨을 알린다. 
			client->Close(); // 소켓을 닫는다

			delete client;
		}
	}
	serverSocket.ShutDown();
}

BOOL CSocketServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	SetFilePath();

	if (serverSocket.Create(25000, SOCK_STREAM))
	{
		if (!serverSocket.Listen())
		{
			AfxMessageBox(_T("ERROR : Server Listen Failed"));
		}
		else
		{
			// 서버 소켓의 IP 정보와 포트 번호를 얻는다.
			sockaddr_in serverAddr;
			int serverAddrLen = sizeof(serverAddr);
			serverSocket.GetSockName((SOCKADDR*)&serverAddr, &serverAddrLen);

			// IP 주소와 포트 번호를 문자열로 변환하여 합친다.
			CString serverInfo;
			char ipAddress[INET_ADDRSTRLEN]; // IPv4 주소 최대 길이

			inet_ntop(AF_INET, &(serverAddr.sin_addr), ipAddress, INET_ADDRSTRLEN);
			UINT portNumber = ntohs(serverAddr.sin_port); // 포트 번호를 호스트 바이트 순서로 변환

			serverInfo.Format(_T("IP: %s, Port: %u"), CString(ipAddress), portNumber);

			// 에딧창에 출력한다.
			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SERVERINFO);
			if (pEdit != nullptr)
				pEdit->SetWindowText(serverInfo);
		}
	}
	else
	{
		AfxMessageBox(_T("ERROR : Server Creation Failed"));
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CSocketServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CSocketServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CSocketServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CSocketServerDlg::OnEnChangeEditServerinfo()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSocketServerDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSocketServerDlg::OnEnChangeEditClientlist()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSocketServerDlg::OnBnClickedButtonFileSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 파일 대화 상자 생성
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, _T("All Files (*.*)|*.*||"), this);

	// 파일 대화 상자를 표시하고 사용자가 파일을 선택하도록 함
	if (fileDlg.DoModal() == IDOK)
	{
		// 사용자가 선택한 파일의 경로를 얻음
		filePath = fileDlg.GetPathName();

		CFile file;
		if (file.Open(filePath, CFile::modeCreate | CFile::modeWrite))
		{
			// CListBox의 모든 항목을 얻음
			int itemCount = MessageList.GetCount();
			for (int i = 0; i < itemCount; ++i)
			{
				CString itemText;
				MessageList.GetText(i, itemText); // ListBox에서 항목 텍스트 가져오기
				itemText += _T("\r\n"); // 개행 문자 추가

				// 파일에 항목 텍스트 쓰기
				file.Write(itemText, itemText.GetLength() * sizeof(TCHAR));
			}

			file.Close();
		}
		else
		{
			AfxMessageBox(_T("Failed to open file for writing."));
		}
	}
}

void CSocketServerDlg::UpdateClientList()
{
	// 에디트 창을 가져옴
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_ClientList);

	// 클라이언트 소켓 리스트를 가져옴
	const CPtrList& clientList = serverSocket.GetClientList();

	CString strOutput;
	POSITION pos = clientList.GetHeadPosition();

	while (pos != nullptr) {
		CSocket* pSocket = (CSocket*)clientList.GetNext(pos);

		if (pSocket != nullptr) {
			// 소켓 주소 구조체를 가져옴
			sockaddr_in clientAddr;
			int addrLen = sizeof(clientAddr);
			pSocket->GetPeerName((SOCKADDR*)&clientAddr, &addrLen);

			// IP 주소를 문자열로 변환
			CString strIP;
			char ipAddress[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &(clientAddr.sin_addr), ipAddress, INET_ADDRSTRLEN);
			strIP.Format(_T("%s"), CString(ipAddress));

			// IP 주소를 출력 문자열에 추가
			strOutput += strIP + _T("\r\n");
		}
	}

	// 에디트 창에 문자열을 설정
	if (pEdit != nullptr) {
		pEdit->SetWindowText(strOutput);
	}
}

void CSocketServerDlg::SetFilePath()
{
	// EXE 파일의 경로를 얻음
	TCHAR exePath[MAX_PATH];
	GetModuleFileName(NULL, exePath, MAX_PATH);
	CString strExePath(exePath);
	int pos = strExePath.ReverseFind(_T('\\'));
	filePath = strExePath.Left(pos + 1) + _T("log");

	// log 디렉터리가 존재하지 않으면 생성
	if (!PathIsDirectory(filePath))
	{
		CreateDirectory(filePath, NULL);
	}
}

void CSocketServerDlg::SaveData(CString Text)
{
	CFile file;
	CString Time;
	Time.Format(_T("%04d-%02d-%02d"), CTime::GetCurrentTime().GetYear(), CTime::GetCurrentTime().GetMonth(), CTime::GetCurrentTime().GetDay());
	CString TodayfilePath = filePath + _T("\\") + Time + _T("_log.txt");

	if (file.Open(TodayfilePath, CFile::modeCreate | CFile::modeWrite | CFile::modeNoTruncate))
	{
		// 파일 포인터를 파일 끝으로 이동
		file.SeekToEnd();

		// 문자열을 파일에 쓰기
		file.Write(Text, Text.GetLength() * sizeof(TCHAR));

		// 파일 닫기
		file.Close();
	}
	else
	{
		AfxMessageBox(_T("ERROR: Unable to open log file for writing."));
	}
}
