#pragma once

class ClientSocket : public CSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();

	CAsyncSocket* listenSocket;

	void SetListenSocket(CAsyncSocket* Socket);	//Ŭ���̾�Ʈ ���� ������ ���� �ּҸ� ListenServer�� ����
	void OnClose(int ErrorCode);	//Ŭ���̾�Ʈ ���� Close�� ȣ���Լ�
	void OnReceive(int ErrorCode);	//Ŭ���̾�Ʈ�� message ���۽� ȣ���Լ�.
};

