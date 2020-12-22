//===============================================
//
// �J�[�h���̏��� (card_info.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "card_info.h"
#include "number.h"
#include "card.h"
#include "game.h"

//========================================
// �}�N����`
//========================================
#define JUSTIFIED_NUMBER 20.0f  // �������l�߂�

//========================================
// �ÓI�����o�ϐ��錾
//========================================
CNumber *CCardInfo::m_apNumber[MAX_CARD_INFO_NUMBER] = {};

//========================================
// �J�[�h���̃R���X�g���N�^
//========================================
CCardInfo::CCardInfo() :CScene(OBJTYPE::OBJTYPE_CARD_INFO)
{

}

//========================================
// �J�[�h���̃f�X�g���N�^
//========================================
CCardInfo::~CCardInfo()
{

}

//========================================
// �J�[�h���̐���
//========================================
CCardInfo* CCardInfo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CCardInfo *pCardInfo = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pCardInfo = new CCardInfo;

    // ������
    pCardInfo->Init(pos, size);

    return pCardInfo;
}

//========================================
// �J�[�h���̏���������
//========================================
HRESULT CCardInfo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �����̃J�[�h���
    // �傫���ʂ���A���ꂼ��Create
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_CARD_INFO_DIGITS - 1) / MAX_CARD_INFO_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + CARD_INFO_ATTACK_POS, D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_CARD_INFO_DIGITS + 1) / MAX_CARD_INFO_DIGITS / 2, 0.0f, 0.0f) + CARD_INFO_ATTACK_POS,
        D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_CARD_INFO_DIGITS - 1) / MAX_CARD_INFO_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + CARD_INFO_DEFENSE_POS, D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[3] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_CARD_INFO_DIGITS + 1) / MAX_CARD_INFO_DIGITS / 2, 0.0f, 0.0f) + CARD_INFO_DEFENSE_POS,
        D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);

    // �G�̃J�[�h���
    m_apNumber[4] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_CARD_INFO_DIGITS - 1) / MAX_CARD_INFO_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + CARD_INFO_ENEMY_POS, D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[5] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_CARD_INFO_DIGITS + 1) / MAX_CARD_INFO_DIGITS / 2, 0.0f, 0.0f) + CARD_INFO_ENEMY_POS,
        D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// �J�[�h���̏I������
//========================================
void CCardInfo::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_CARD_INFO_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            // �������̏I������
            m_apNumber[nCntNumber]->Uninit();

            // �������̃������̊J��
            ReleaseNumber(nCntNumber);
        }
    }

    // �I�u�W�F�N�g�̔j��
    Release();
}

//========================================
// �J�[�h���N���X�������Ă���A�����̊J������
//========================================
void CCardInfo::ReleaseNumber(int nCntNumber)
{
    // �������̊J��
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// �J�[�h���̍X�V����
//========================================
void CCardInfo::Update(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_CARD_INFO_NUMBER; nCntNumber++)
    {
        // �ϐ��錾
        bool bUse = true;   // �g�p��

        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            bUse = m_apNumber[nCntNumber]->Update();

            // ���g�p�ɂȂ�����
            if (bUse == false)
            {
                // �������̃������̊J��
                ReleaseNumber(nCntNumber);
            }
        }
    }

    // �Q�[���I�������Ȃ瓧����
    if (CGame::GetFinishBattle() == true)
    {
        for (int nCntNumber = 0; nCntNumber < MAX_CARD_INFO_NUMBER; nCntNumber++)
        {
            // ���g������Ȃ�
            if (m_apNumber[nCntNumber] != NULL)
            {
                // ������S�ē�����
                m_apNumber[nCntNumber]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
            }
        }
    }
}

//========================================
// �J�[�h���̕`�揈��
//========================================
void CCardInfo::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_CARD_INFO_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// �J�[�h���̐ݒ�
//========================================
void CCardInfo::SetCardInfo(int nAttack, int nDefence)
{
    // �ϐ��錾
    int nIndex;		        // �w���@(�J�[�h���\���p)
    int nRadix = 10;	    // ��@(�J�[�h���\���p)

    // �U��
    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_CARD_INFO_DIGITS; nIndex++)
    {
        int nCardInfo = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex);
        int nCardInfo2 = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = nAttack % nCardInfo / nCardInfo2;

        // �ԍ���ݒ�
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }

    // �h��
    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_CARD_INFO_DIGITS; nIndex++)
    {
        int nCardInfo = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex);
        int nCardInfo2 = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = nDefence % nCardInfo / nCardInfo2;

        // �ԍ���ݒ�
        m_apNumber[nIndex + MAX_CARD_INFO_DIGITS]->SetNumber(nAnswer);
    }
}

//========================================
// �G�̃J�[�h���̐ݒ�
//========================================
void CCardInfo::SetEnemyCardInfo(int nValue)
{
    // �ϐ��錾
    int nIndex;		        // �w���@(�J�[�h���\���p)
    int nRadix = 10;	    // ��@(�J�[�h���\���p)

    for (nIndex = 0; nIndex < MAX_CARD_INFO_DIGITS; nIndex++)
    {
        // -1�Ȃ瓧��
        if (nValue == -1)
        {
            m_apNumber[nIndex + MAX_CARD_INFO_DIGITS + MAX_CARD_INFO_DIGITS]->SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,0.0f));
        }
        else
        {
            int nCardInfo = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex);
            int nCardInfo2 = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex - 1);

            // 
            int nAnswer = nValue % nCardInfo / nCardInfo2;

            m_apNumber[nIndex + MAX_CARD_INFO_DIGITS + MAX_CARD_INFO_DIGITS]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

            // �ԍ���ݒ�
            m_apNumber[nIndex + MAX_CARD_INFO_DIGITS + MAX_CARD_INFO_DIGITS]->SetNumber(nAnswer);
        }
    }
}
