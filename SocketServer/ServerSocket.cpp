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

	if (Accept(*client))	//Ŭ���̾�Ʈ ���� ��û�� ���� �����Լ�
	{
		client->SetListenSocket(this);	//Ŭ���̾�Ʈ���Ͽ��� ����� �������� ����
		ptrClientSocketList.AddTail(client);	//����Ʈ�� ����� ���� ������ ����
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
	//����� Ŭ���̾�Ʈ�� �����͸� ���� �� ������ ����
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
