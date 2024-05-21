#include "pch.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "SocketServerDlg.h"

ClientSocket::ClientSocket()
{
	this->listenSocket = NULL;
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::SetListenSocket(CAsyncSocket* Socket)
{
	this->listenSocket = Socket;
}

void ClientSocket::OnClose(int ErrorCode)
{
	CSocket::OnClose(ErrorCode);

	ServerSocket* serverSocket = (ServerSocket*)(this->listenSocket);
	serverSocket->CloseClientSocket(this);
}

void ClientSocket::OnReceive(int ErrorCode)
{
	CString recivedMessage = _T(""), IPAddress = _T("");	//데이터를 저장할 스트링, IP주소
	UINT PortNumber = 0;
	TCHAR reciveBuffer[2048];	//데이터를 전송받을 임시버퍼
	::ZeroMemory(reciveBuffer, sizeof(reciveBuffer));	//임시버퍼 정리

	GetPeerName(IPAddress, PortNumber);	//클라이언트의 IP/Port 추출

	if (Receive(reciveBuffer, sizeof(reciveBuffer)))	//전송된 데이터 존재시
	{
		CSocketServerDlg* Main = (CSocketServerDlg*)AfxGetMainWnd();

		CString Time = _T("");
		Time.Format(_T("[%02d:%02d:%02d] : "), CTime::GetCurrentTime().GetHour(), CTime::GetCurrentTime().GetMinute(), CTime::GetCurrentTime().GetSecond());	//시간 추가

		recivedMessage.Format(reciveBuffer);	//버퍼의 데이터를 스트링으로 이동
		Main->TextOut(Time + recivedMessage);
		Main->SaveData(Time + recivedMessage);

	}
}
