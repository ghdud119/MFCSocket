#pragma once

class ClientSocket : public CSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();

	CAsyncSocket* listenSocket;

	void SetListenSocket(CAsyncSocket* Socket);	//클라이언트 연결 형성시 소켓 주소를 ListenServer에 저장
	void OnClose(int ErrorCode);	//클라이언트 소켓 Close시 호출함수
	void OnReceive(int ErrorCode);	//클라이언트가 message 전송시 호출함순.
};

