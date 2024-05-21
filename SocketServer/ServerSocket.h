#pragma once

class ServerSocket : public CAsyncSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();

	virtual void OnAccept(int nErrorCode);//Ŭ���̾�Ʈ ���� ��û�� ȣ���Լ�
	void CloseClientSocket(CSocket* Client);	//Ŭ���̾�Ʈ ���� ���� �� ����Ʈ���� ����

	const CPtrList& GetClientList() const {	return ptrClientSocketList;	}

private:
	CPtrList ptrClientSocketList;	//Ŭ���̾�Ʈ ���� ������ ���帮��Ʈ
};

