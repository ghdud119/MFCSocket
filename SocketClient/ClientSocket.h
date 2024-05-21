#pragma once
#include <afxsock.h>
class ClientSocket : public CSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();

	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
};

