#include"pch.h"
#include "ServerSocket.h"
#include "ClientSocket.h"
#include "SocketServerDlg.h"


ServerSocket::ServerSocket()
{
}

ServerSocket::~ServerSocket()
{
}

void ServerSocket::OnAccept(int nErrorCode)
{
	ClientSocket* client = new ClientSocket();

	if (Accept(*client))	//클라이언트 접속 요청시 연결 형성함수
	{
		client->SetListenSocket(this);	//클라이언트소켓에게 연결된 소켓정보 전송
		ptrClientSocketList.AddTail(client);	//리스트에 연결된 소켓 데이터 저장
		CSocketServerDlg* Main = (CSocketServerDlg*)AfxGetMainWnd();
		Main->UpdateClientList();;
	}
	else
	{
		delete client;
		AfxMessageBox(_T("ERROR : Client Acception Failed"));
	}

	
	CAsyncSocket::OnAccept(nErrorCode);
}

void ServerSocket::CloseClientSocket(CSocket* Client)
{
	//연결된 클라이언트의 데이터를 종류 후 데이터 삭제
	POSITION pos = ptrClientSocketList.Find(Client);

	if (pos != NULL)
	{
		if (Client != NULL)
		{
			Client->ShutDown();
			Client->Close();
		}
		ptrClientSocketList.RemoveAt(pos);
		delete Client;

		CSocketServerDlg* Main = (CSocketServerDlg*)AfxGetMainWnd();

		Main->UpdateClientList();;
	}
}
