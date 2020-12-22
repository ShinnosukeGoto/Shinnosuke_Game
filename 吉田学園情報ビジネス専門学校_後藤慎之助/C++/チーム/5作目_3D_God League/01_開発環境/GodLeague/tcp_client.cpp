//------------------------------------------------------------
//TCP�ʐM�����̃N���X��
//Author : �ɓ� ��P
//------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS //scanf�̌x���}���p
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "main.h"
#include <stdio.h>
#include "tcp_client.h"

//------------------------------------------------------------
// TCP�ʐM�����N���X�̃C���X�^���X����
// pHostName : IP�A�h���X
// nPortNum	 : �|�[�g�ԍ�
//------------------------------------------------------------
CTcpClient * CTcpClient::Create(const char*pHostName, int nPortNum)
{
	//�C���X�^���X����
	CTcpClient * pTcpClient = new CTcpClient;
	if (pTcpClient->Init(pHostName, nPortNum) == false)
	{
		delete pTcpClient;
		pTcpClient = NULL;
	}
	return pTcpClient;
}

//------------------------------------------------------------
//TCP�ʐM�����N���X�̃R���X�g���N�^
//------------------------------------------------------------
CTcpClient::CTcpClient()
{
	m_sock = -1;
}

//------------------------------------------------------------
//TCP�ʐM�����N���X�̃f�X�g���N�^
//------------------------------------------------------------
CTcpClient::~CTcpClient()
{

}

//------------------------------------------------------------
//TCP�ʐM�����N���X�̏���������
//pHostName : �ڑ����IP�A�h���X
//nPortNum  : �ڑ���̃|�[�g�ԍ�
//------------------------------------------------------------
bool CTcpClient::Init(const char *pHostName, int nPortNum)
{
	//�\�P�b�g����
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET)
	{
		return 1;
	}

	//�ڑ����̐ݒ�
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;				//IPv4 192.168.11.1
	addr.sin_port = htons(nPortNum);	//�|�[�g�ԍ� 1024�`65535 80�� HTTP 
	addr.sin_addr.S_un.S_addr = inet_addr(pHostName);

	//�ڑ�
	if (connect(m_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
	{
		//�ڑ��ؒf
		return false;
	}

	return true;

}

//------------------------------------------------------------
//TCP�ʐM�����N���X�̑��M����
//pSendData		: ���M�����f�[�^
//nSendDataSize : ���M�����f�[�^�T�C�Y
//------------------------------------------------------------
int CTcpClient::Send(char *pSendData, int nSendDataSize)
{
	if (m_sock < 0)
	{
		//�\�P�b�g���J���Ă��Ȃ��ꍇ�̑��M�����p
		return 0;
	}
	int nSendSize = send(m_sock, pSendData, nSendDataSize, 0);
	return nSendSize;
}

//------------------------------------------------------------
//TCP�ʐM�����N���X�̎�M����
//pRecvBuf	   : ��M����f�[�^
//pRecvBufSize : ��M����f�[�^�T�C�Y
//------------------------------------------------------------
int CTcpClient::Recv(char *pRecvBuf, int nRecvBufSize)
{
	if (m_sock < 0)
	{
		//�\�P�b�g���J���Ă��Ȃ��ꍇ�̎�M�����p
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
//TCP�ʐM�����N���X�̐ؒf����
//------------------------------------------------------------
void CTcpClient::Close()
{
	if (m_sock < 0)
	{
		//�g���Ă��Ȃ��\�P�b�g���N���[�Y���Ȃ����߂�
		return;
	}
	closesocket(m_sock);
	m_sock = -1;
}

//------------------------------------------------------------
//TCP�ʐM�����N���X�̊J������
//------------------------------------------------------------
void CTcpClient::Release()
{
	Close();
	delete this;
}