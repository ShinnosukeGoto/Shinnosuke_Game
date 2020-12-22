//------------------------------------------------------------
//TCP通信処理のクラス化
//Author : 伊藤 大輝
//------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS //scanfの警告抑制用
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "main.h"
#include <stdio.h>
#include "tcp_client.h"

//------------------------------------------------------------
// TCP通信処理クラスのインスタンス生成
// pHostName : IPアドレス
// nPortNum	 : ポート番号
//------------------------------------------------------------
CTcpClient * CTcpClient::Create(const char*pHostName, int nPortNum)
{
	//インスタンス生成
	CTcpClient * pTcpClient = new CTcpClient;
	if (pTcpClient->Init(pHostName, nPortNum) == false)
	{
		delete pTcpClient;
		pTcpClient = NULL;
	}
	return pTcpClient;
}

//------------------------------------------------------------
//TCP通信処理クラスのコンストラクタ
//------------------------------------------------------------
CTcpClient::CTcpClient()
{
	m_sock = -1;
}

//------------------------------------------------------------
//TCP通信処理クラスのデストラクタ
//------------------------------------------------------------
CTcpClient::~CTcpClient()
{

}

//------------------------------------------------------------
//TCP通信処理クラスの初期化処理
//pHostName : 接続先のIPアドレス
//nPortNum  : 接続先のポート番号
//------------------------------------------------------------
bool CTcpClient::Init(const char *pHostName, int nPortNum)
{
	//ソケット生成
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		return 1;
	}

	//接続情報の設定
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;				//IPv4 192.168.11.1
	addr.sin_port = htons(nPortNum);	//ポート番号 1024～65535 80番 HTTP 
	addr.sin_addr.S_un.S_addr = inet_addr(pHostName);

	//接続
	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		//接続切断
		return false;
	}

	return true;

}

//------------------------------------------------------------
//TCP通信処理クラスの送信処理
//pSendData		: 送信したデータ
//nSendDataSize : 送信したデータサイズ
//------------------------------------------------------------
int CTcpClient::Send(char *pSendData, int nSendDataSize)
{
	if (m_sock < 0)
	{
		//ソケットが開いていない場合の送信処理用
		return 0;
	}
	int nSendSize = send(m_sock, pSendData, nSendDataSize, 0);
	return nSendSize;
}

//------------------------------------------------------------
//TCP通信処理クラスの受信処理
//pRecvBuf	   : 受信するデータ
//pRecvBufSize : 受信するデータサイズ
//------------------------------------------------------------
int CTcpClient::Recv(char *pRecvBuf, int nRecvBufSize)
{
	if (m_sock < 0)
	{
		//ソケットが開いていない場合の受信処理用
		return 0;
	}

	int nRecvSize = recv(m_sock, pRecvBuf, nRecvBufSize, 0);

	if (nRecvSize <= 0)
	{
		Close();
	}
	return nRecvSize;
}

//------------------------------------------------------------
//TCP通信処理クラスの切断処理
//------------------------------------------------------------
void CTcpClient::Close()
{
	if (m_sock < 0)
	{
		//使われていないソケットをクローズしないために
		return;
	}
	closesocket(m_sock);
	m_sock = -1;
}

//------------------------------------------------------------
//TCP通信処理クラスの開放処理
//------------------------------------------------------------
void CTcpClient::Release()
{
	Close();
	delete this;
}