#pragma once

class ServerSocket : public CAsyncSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();

	virtual void OnAccept(int nErrorCode);//클라이언트 접속 요청시 호출함수
	void CloseClientSocket(CSocket* Client);	//클라이언트 소켓 종료 후 리스트에서 삭제

	const CPtrList& GetClientList() const {	return ptrClientSocketList;	}

private:
	CPtrList ptrClientSocketList;	//클라이언트 소켓 데이터 저장리스트
};

