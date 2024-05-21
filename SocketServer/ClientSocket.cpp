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
	CString recivedMessage = _T(""), IPAddress = _T("");	//�����͸� ������ ��Ʈ��, IP�ּ�
	UINT PortNumber = 0;
	TCHAR reciveBuffer[2048];	//�����͸� ���۹��� �ӽù���
	::ZeroMemory(reciveBuffer, sizeof(reciveBuffer));	//�ӽù��� ����

	GetPeerName(IPAddress, PortNumber);	//Ŭ���̾�Ʈ�� IP/Port ����

	if (Receive(reciveBuffer, sizeof(reciveBuffer)))	//���۵� ������ �����
	{
		CSocketServerDlg* Main = (CSocketServerDlg*)AfxGetMainWnd();

		CString Time = _T("");
		Time.Format(_T("[%02d:%02d:%02d] : "), CTime::GetCurrentTime().GetHour(), CTime::GetCurrentTime().GetMinute(), CTime::GetCurrentTime().GetSecond());	//�ð� �߰�

		recivedMessage.Format(reciveBuffer);	//������ �����͸� ��Ʈ������ �̵�
		Main->TextOut(Time + recivedMessage);
		Main->SaveData(Time + recivedMessage);

	}
}
