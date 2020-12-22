//===============================================
//
// �����̊Ǘ����� (manager.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "manager.h"
#include "scene2d.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "mode.h"
#include "title.h"
#include "game.h"
#include "ui.h"
#include <typeinfo.h>
#include "cursor.h"
#include "card.h"
#include "fade.h"
#include "number.h"
#include "light.h"
#include "camera.h"
#include "scene3d.h"
#include "character.h"
#include "tcp_client.h"
#include "card_name.h"
#include "sky.h"
#include "manual.h"
#include "button.h"
#include "field.h"
#include "ui3d.h"
#include "bg.h"
#include "hit3d.h"

//========================================
// �}�N����`
//========================================

//========================================
// �ÓI�����o�ϐ��錾
//========================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CSound *CManager::m_pSound = NULL;
CMode *CManager::m_pMode = NULL;
CFade *CManager::m_pFade = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
bool CManager::m_bStartMatching = false;
bool CManager::m_bMatching = false;
bool CManager::m_bAttackTurn = false;
bool CManager::m_bDefenceTurn = false;
bool CManager::m_bDefenceCardUse = false;
bool CManager::m_bRecvCard = false;
bool CManager::m_bDisconnect = false;
bool CManager::m_bGameEnd = false;
int CManager::m_nAttackOrder = 0;
int CManager::m_nEnemyUseCard = 0;
int CManager::m_nPlayerUseCard = 0;

//========================================
// �S�Ă̒ʐM�ɂ��������̂̏�����
//========================================
void CManager::ResetAll(void)
{
    m_bStartMatching = false;
    m_bMatching = false;
    m_bAttackTurn = false;
    m_bDefenceTurn = false;
    m_bDefenceCardUse = false;
    m_bRecvCard = false;
	m_bDisconnect = false;
	m_bGameEnd = false;
    m_nAttackOrder = 0;
    m_nEnemyUseCard = 0;
    m_nPlayerUseCard = 0;

	m_pCamera->ResetCamera();
}

//========================================
// �����̊Ǘ��̃f�t�H���g�R���X�g���N�^
//========================================
CManager::CManager()
{

}

//========================================
// �����̊Ǘ��̃f�X�g���N�^
//========================================
CManager::~CManager()
{

}

//========================================
// �����̊Ǘ��̏���������
//========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �����_���[�̐���(��񂵂��������Ȃ����̂́ACreate��p�ӂ��Ă��Ȃ�)
	m_pRenderer = new CRenderer;

	// �����_���[����������
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�̐���(new��Init�͘A�������ď���)
	m_pInputKeyboard = new CInputKeyboard;

	// �L�[�{�[�h�̏���������
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �R���g���[���̐���
	m_pInputJoypad = new CInputJoypad;

	// �R���g���[���̏���������
	if (FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �T�E���h�̐���
	m_pSound = new CSound;
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	// ���C�g�̐���
	m_pLight = new CLight;
	if (FAILED(m_pLight->Init()))
	{
		return E_FAIL;
	}

	// �J�����̐���
	m_pCamera = new CCamera;
	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	// �t�F�[�h�̐���
	m_pFade = CFade::Create(MODE_TITLE);    // �ŏ��̓^�C�g��

	// �e�N�X�`���̃��[�h
	CUI::Load();
	CCursor::Load();
	CCard::Load();
	CNumber::Load();
	CCharacter::Load();
    CCardName::Load();
    CSky::Load();
    CManual::Load();
    CButton::Load();
    CBg::Load();
	CField::Load();
	CUi3D::Load();
    CHit3D::Load();

	return S_OK;
}

//========================================
// �����̊Ǘ��̏I������
//========================================
void CManager::Uninit(void)
{
	// �S�ẴI�u�W�F�N�g��j��
	CScene::ReleaseAll();

	// �e�N�X�`���̃A�����[�h
	CUI::Unload();
	CCursor::Unload();
	CCard::Unload();
	CNumber::Unload();
	CCharacter::Unload();
    CCardName::Unload();
    CSky::Unload();
    CManual::Unload();
    CButton::Unload();
    CBg::Unload();
	CField::Unload();
	CUi3D::Unload();
    CHit3D::Unload();

	// ���[�h�j��
	if (m_pMode != NULL)
	{
		// ���[�h�̏I������
		m_pMode->Uninit();

		// ���[�h�̃������̊J��
		delete m_pMode;
		m_pMode = NULL;
	}

	// �t�F�[�h�j��
	if (m_pFade != NULL)
	{
		// �t�F�[�h�̏I������
		m_pFade->Uninit();

		// �t�F�[�h�̃������̊J��
		delete m_pFade;
		m_pFade = NULL;
	}

	// �T�E���h�j��
	if (m_pSound != NULL)
	{
		// �T�E���h�I������
		m_pSound->Uninit();

		// �T�E���h�̃������̊J��
		delete m_pSound;
		m_pSound = NULL;
	}

	// �R���g���[���j��
	if (m_pInputJoypad != NULL)
	{
		// �R���g���[���I������
		m_pInputJoypad->Uninit();

		// �R���g���[���̃������̊J��
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// �L�[�{�[�h�j���iInit�Ő��������t���Ŕj������j
	if (m_pInputKeyboard != NULL)
	{
		// �L�[�{�[�h�I������
		m_pInputKeyboard->Uninit();

		// �L�[�{�[�h�̃������̊J��
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// �����_���[�j��
	if (m_pRenderer != NULL)
	{
		// �����_���[�I������
		m_pRenderer->Uninit();

		// �����_���[�̃������̊J��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//========================================
// �����̊Ǘ��̍X�V����
//========================================
void CManager::Update(void)
{
	// �L�[�{�[�h�X�V����(�ŏ��ɍs��)
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// �R���g���[���X�V����
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}

	// �����_���[�X�V����
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// ���[�h�̍X�V����
	if (m_pMode != NULL)
	{
		m_pMode->Update();
	}

	// �t�F�[�h�̍X�V����
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

    // �J�����̍X�V����
    if (m_pCamera != NULL)
    {
        // �Q�[�����Ȃ�
        if (typeid(*m_pMode) == typeid(CGame))
        {
            m_pCamera->Update();
        }
    }
}

//========================================
// �����̊Ǘ��̕`�揈��
//========================================
void CManager::Draw(void)
{
	// �`�揈��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//========================================
// ���[�h�̎擾
//========================================
CManager::MODE CManager::GetMode(void)
{
	// �Ԃ�l�Ɏg�����[�h
	MODE mode = MODE_NONE;

	// ���̃��[�h�������Ă��邩���āA�^�����Ƃɏꍇ����
	if (typeid(*m_pMode) == typeid(CTitle))
	{
		mode = MODE_TITLE;
	}
	else if (typeid(*m_pMode) == typeid(CGame))
	{
		mode = MODE_GAME;
	}

	return mode;
}

//========================================
// ���[�h�̐ݒ�
// mode	:	�ݒ肷�郂�[�h
//========================================
void CManager::SetMode(MODE mode)
{
	// ���[�h�̒��g������Ȃ�
	if (m_pMode != NULL)
	{
		// �I������
		m_pMode->Uninit();

		// �������̔j��
		delete m_pMode;
		m_pMode = NULL;
	}

	// �V�[����̃I�u�W�F�N�g��S�ďI������
	CScene::ReleaseAll();

	// �󂯎�������[�h�ɉ�ʑJ��
	switch (mode)
	{
	case MODE_TITLE:
		m_pMode = new CTitle;
		break;

	case MODE_GAME:
		m_pMode = new CGame;
		break;
	}

	// �m�ۂ������[�h�̏�����
	m_pMode->Init();
}

//========================================
// �}�b�`���O�̃X�^�[�g
// bMatching	:	�}�b�`���O���X�^�[�g����t���O
//========================================
void CManager::SetStartMatching(bool bMatching)
{
	m_bStartMatching = bMatching;

	if (m_bStartMatching)
	{
		//�X���b�h����
		std::thread Connection(ConnectionThread);
		// �X���b�h��ؒf
		Connection.detach();
	}
}

//========================================
// �ڑ��X���b�h�̏���
//========================================
void CManager::ConnectionThread(void)
{
	WSADATA wsaData;
	char aDestIPAddr[128] = IP_ADDRESS;
	char aRecvBuff[256];
	char aSendBuff[256];
	int nPlayerUseCard;
	int nEnemyUseCard;
	int nTypeCard;

	// ��M�p�ϐ���0�N���A
	memset(aRecvBuff, 0, sizeof(aRecvBuff));
	memset(aSendBuff, 0, sizeof(aSendBuff));

	// winsock����������
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	//CTcpClient�𐶐����āA�T�[�o�[�ɐڑ�
	CTcpClient *pTcpClient = NULL;

	//�C���X�^���X����
	pTcpClient = CTcpClient::Create(aDestIPAddr, SERVER_PORT_NUM);

	// �ڑ��ł������M
	pTcpClient->Recv(aRecvBuff, sizeof(aRecvBuff));

	// ��M������U����U���̏����󂯎��
	char aRecvAttack;

	// ��M������U��U�̏����R�s�[
	memcpy(&aRecvAttack, &aRecvBuff[0], sizeof(aRecvAttack));

	//// �G���f�B�A���ϊ�
	//m_nAttackOrder = ntohl(m_nAttackOrder);

	// ��U��U�̏����擾
	m_nAttackOrder = (int)aRecvAttack;

	// �}�b�`���O�ł������ǂ���
	if (pTcpClient != NULL)
	{
		// �ł���
		m_bMatching = true;
	}
	else
	{
		// �ł��ĂȂ�
		m_bMatching = false;
	}

	// ��U�Ȃ�h��^�[����true�ɂ���
	if (m_nAttackOrder == 2)
	{
		m_bDefenceTurn = true;
	}

	// �o�g�����邽��while��
	while (1)
	{

		// �Q�[�����I���������ǂ����擾
		m_bGameEnd = CGame::GetFinishBattle();

		if (!m_bGameEnd)
		{
			// �U���^�[��
			if (m_bAttackTurn)
			{
				// ������
				memset(aSendBuff, 0, sizeof(aSendBuff));


				// �������g�p�����U���J�[�h��ϐ��Ɏ󂯎��
				nPlayerUseCard = m_nPlayerUseCard;


				// �G���f�B�A���ϊ�
				nPlayerUseCard = htonl(nPlayerUseCard);


				// �������g�p�����J�[�h����aSendBuff�ɃR�s�[
				memcpy(&aSendBuff[0], &nPlayerUseCard, sizeof(aSendBuff));


				// send(�������g�p�����J�[�h�����T�[�o�[�ɑ���)
				pTcpClient->Send(aSendBuff, sizeof(int));


				//���g��������
				memset(aRecvBuff, 0, sizeof(aRecvBuff));


				//recv(����̖h��J�[�h��҂�)
				if (pTcpClient->Recv(aRecvBuff, sizeof(aRecvBuff)) <= 0)
				{// �ؒf���ꂽ�ꍇ

					m_bDisconnect = true;

				}


				// �G���g�����J�[�h�ԍ����擾
				nTypeCard = *(int*)&aRecvBuff[0];


				// �G���f�B�A���ϊ�
				nTypeCard = ntohl(nTypeCard);

				// ���肪�g�p�����J�[�h���𑗂�
				// nTypeCard: ���肪�g�p�����h��J�[�h�̏��
				// TURN: �����̍��̃^�[��������
				RecvCard(nTypeCard, TURN_ATTACK);

				//=======================================
				// �v���C���[�̍U�����o
                CGame::SetState(CGame::STATE_PLAYER_ATTACK);
				//=======================================
			}
			// �h��^�[��
			else if (m_bDefenceTurn)
			{

				// ���g��������
				memset(aRecvBuff, 0, sizeof(aRecvBuff));

				// ����̃J�[�h���󂯎��Ă��Ȃ�
				if (!m_bRecvCard)
				{
					// recv(����̍U���J�[�h��҂�)
					if (pTcpClient->Recv(aRecvBuff, sizeof(aRecvBuff)) <= 0)
					{// �ؒf���ꂽ�ꍇ
						m_bDisconnect = true;
					}

					// �G���g�����J�[�h�����擾
					nEnemyUseCard = *(int*)&aRecvBuff[0];


					// �G���f�B�A���ϊ�
					nEnemyUseCard = ntohl(nEnemyUseCard);


					// ���肪�g�p�����J�[�h���擾
					RecvCard(nEnemyUseCard, TURN_DEFENCE);


					// ���肩��J�[�h���󂯎�ꂽ�̂�true�ɂ���
					m_bRecvCard = true;

					//====================================================
					// �h��J�n
					CGame::SetDefenceTurn(true);
					//====================================================
				}

				// �h��J�[�h���g�p�����Ȃ�
				if (m_bDefenceCardUse)
				{

					// �������g�p�����J�[�h��ϐ��ɓ����
					nPlayerUseCard = m_nPlayerUseCard;


					// �G���f�B�A���ϊ�
					nPlayerUseCard = htonl(nPlayerUseCard);


					// �g���J�[�h�����R�s�[
					memcpy(&aSendBuff[0], &nPlayerUseCard, sizeof(aSendBuff));


					//send(�������g�p�����h��J�[�h�𑗂�)
					pTcpClient->Send(aSendBuff, sizeof(int));

					// �h��^�[�����I����
					CGame::SetDefenceTurn(false);

					m_bDefenceCardUse = false;
					m_bRecvCard = false;

					// �h��^�[�����I��
					m_bDefenceTurn = false;

					//=======================================
					// �G�l�~�[�̍U�����o
                    CGame::SetState(CGame::STATE_ENEMY_ATTACK);
					//=======================================
				}

			}

			// �ؒf���ꂽ�ꍇ
			if (m_bDisconnect)
			{
				// While���𔲂���
				break;
			}
		}
		// �Q�[�����I�������ꍇ
		else
		{
			// While���𔲂���
			break;
		}
	}

	// �T�[�o�[�Ƃ̐ڑ���؂�
	pTcpClient->Release();
}

//========================================
// �J�[�h���󂯎�鏈��
//========================================
void CManager::RecvCard(int nTypeCard, TURN Turn)
{
	
	switch (Turn)
	{
		// �U���^�[���̏ꍇ
	case TURN_ATTACK:

		// �U���^�[���I��
		m_bAttackTurn = false;

		// �U���^�[�����I�������̂Ŗh��^�[��
		m_bDefenceTurn = true;

		// �擾�����J�[�h��ۑ�����
		m_nEnemyUseCard = nTypeCard;

        // �G���g�p�����J�[�h���Q�[���Ɍ��т���
        CGame::SetEnemyUseCard(nTypeCard);

		break;

		// �h��^�[���̏ꍇ
	case TURN_DEFENCE:

		// �G�̎g�p�����J�[�h���擾
		m_nEnemyUseCard = nTypeCard;

        // �G���g�p�����J�[�h���Q�[���Ɍ��т���
        CGame::SetEnemyUseCard(nTypeCard);

		break;
	}

}

void CManager::SendCard(int nType)
{
	//***************************************************************
	// ��ԉ��̕���J�[�h����ԍŌ�̕���J�[�h�A�܂��͋F��
	//***************************************************************
	// �U���J�[�h�Ȃ�
	if (nType >= CCard::TYPE_WEAPON_000 && nType <= CCard::TYPE_WEAPON_FINAL || nType == PRAY_ID)
	{
		// �g�p�����J�[�h���擾
		m_nPlayerUseCard = nType;

        // �������g�p�����J�[�h���Q�[���Ɍ��т���
        CGame::SetPlayerUseCard(nType);

		// �U������
		m_bAttackTurn = true;

	}

	//***************************************************************
	// ��ԉ��̖h��J�[�h����ԍŌ�̖h��J�[�h�A�܂��͋���
	//***************************************************************
	// �h��J�[�h�Ȃ�
	else if (nType >= CCard::TYPE_ARMOR_000 && nType <= CCard::TYPE_ARMOR_FINAL || nType == FORGIVE_ID)
	{

		// �g�p�����J�[�h���擾
		m_nPlayerUseCard = nType;

        // �������g�p�����J�[�h���Q�[���Ɍ��т���
        CGame::SetPlayerUseCard(nType);

		// �h��J�[�h���g�p
		m_bDefenceCardUse = true;
	}

}