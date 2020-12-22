//------------------------------
//TCP通信処理のクラス化
//Author:伊藤大輝
//------------------------------
#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_

#include "main.h"

//------------------------------
//マクロ定義
//------------------------------
#define IP_ADDRESS "127.0.0.1"
//#define IP_ADDRESS "20.37.110.55"
#define SERVER_PORT_NUM 12345 //接続先サーバーの接続先ポート

//------------------------------
//TCP送受信クラス
//------------------------------
class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();
	static CTcpClient * Create(const char*pHostName, int nPortNum);
	bool Init(const char*pHostName, int nPortNum);
	int Send(char * pSendData, int nSendDatasize);
	int Recv(char * pRecvBuf, int nRecvBufSize);
	void Close();
	void Release();

private:
	SOCKET m_sock;
};
#endif
