//===============================================
//
// ���C�t�̏��� (life.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "life.h"
#include "number.h"
#include "card.h"
#include "game.h"
#include "card_name.h"
#include "hit3d.h"
#include "sound.h"
#include "manager.h"

//========================================
// �}�N����`
//========================================
#define JUSTIFIED_NUMBER 30.0f  // �������l�߂�

//========================================
// �ÓI�����o�ϐ��錾
//========================================
CNumber *CLife::m_apNumber[MAX_LIFE_NUMBER] = {};

//========================================
// ���C�t�̃R���X�g���N�^
//========================================
CLife::CLife() :CScene(OBJTYPE::OBJTYPE_LIFE)
{
    m_nPlayerLife = FIRST_LIFE;
    m_nEnemyLife = FIRST_LIFE;
}

//========================================
// ���C�t�̃f�X�g���N�^
//========================================
CLife::~CLife()
{

}

//========================================
// ���C�t�̐���
//========================================
CLife* CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CLife *pLife = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pLife = new CLife;

    // ������
    pLife->Init(pos, size);

    return pLife;
}

//========================================
// ���C�t�̏���������
//========================================
HRESULT CLife::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ����̃��C�t
    // �傫���ʂ���A���ꂼ��Create
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_LIFE_DIGITS - 1) / MAX_LIFE_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + LIFE_PLAYER_POS, D3DXVECTOR3(size.x / MAX_LIFE_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_LIFE_DIGITS + 1) / MAX_LIFE_DIGITS / 2, 0.0f, 0.0f) + LIFE_PLAYER_POS,
        D3DXVECTOR3(size.x / MAX_LIFE_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_LIFE_DIGITS - 1) / MAX_LIFE_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + LIFE_ENEMY_POS, D3DXVECTOR3(size.x / MAX_LIFE_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[3] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_LIFE_DIGITS + 1) / MAX_LIFE_DIGITS / 2, 0.0f, 0.0f) + LIFE_ENEMY_POS,
        D3DXVECTOR3(size.x / MAX_LIFE_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// ���C�t�̏I������
//========================================
void CLife::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_LIFE_NUMBER; nCntNumber++)
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
// ���C�t�N���X�������Ă���A�����̊J������
//========================================
void CLife::ReleaseNumber(int nCntNumber)
{
    // �������̊J��
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// ���C�t�̍X�V����
//========================================
void CLife::Update(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_LIFE_NUMBER; nCntNumber++)
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
}

//========================================
// ���C�t�̕`�揈��
//========================================
void CLife::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_LIFE_NUMBER; nCntNumber++)
    {
        // ���g������Ȃ�
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// �v���C���[�̃_���[�W�v�Z����
//========================================
void CLife::PlayerDamageCalculation(void)
{
    // �ϐ��錾
    int nDamage = 0;        // �^����_���[�W
    int nAttackValue = 0;   // �U����
    int nDefenceValue = 0;  // �h���

    // �G�̍U���͂��擾
    nAttackValue = GetAttackValue(TYPE_ENEMY);

    // �v���C���[�̖h��͂��擾
    nDefenceValue = GetDefenceValue(TYPE_PLAYER);

    // �_���[�W�v�Z
    nDamage = nAttackValue - nDefenceValue;

    // �_���[�W��0�����Ȃ�
    if (nDamage < 0)
    {
        // 0�ɌŒ�
        nDamage = 0;
    }

    // ���C�t����_���[�W������
    m_nPlayerLife -= nDamage;

    // �G���g�p�����J�[�h���F��łȂ��Ȃ�
    if (CGame::GetEnemyUseCard() != PRAY_ID)
    {
        // �U�����Đ�
        PlayAttackSound(nDamage);

        // �_���[�W���o
        CHit3D::Create(PLAYER_DAMAGE_POS, nDamage);
    }

    // ���C�t��0�ȉ��Ȃ�A0�ŌŒ�
    if (m_nPlayerLife <= 0)
    {
        m_nPlayerLife = 0;
    }

    // �ϐ��錾
    int nIndex;		        // �w���@(���C�t�\���p)
    int nRadix = 10;	    // ��@(���C�t�\���p)

    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_LIFE_DIGITS; nIndex++)
    {
        int nLife = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex);
        int nLife2 = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = m_nPlayerLife % nLife / nLife2;

        // �ԍ���ݒ�
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}

//========================================
// �G�̃_���[�W�v�Z����
//========================================
void CLife::EnemyDamageCalculation(void)
{
    // �ϐ��錾
    int nDamage = 0;        // �^����_���[�W
    int nAttackValue = 0;   // �U����
    int nDefenceValue = 0;  // �h���

    // �v���C���[�̍U���͂��擾
    nAttackValue = GetAttackValue(TYPE_PLAYER);

    // �G�̖h��͂��擾
    nDefenceValue = GetDefenceValue(TYPE_ENEMY);

    // �_���[�W�v�Z
    nDamage = nAttackValue - nDefenceValue;

    // �_���[�W��0�����Ȃ�
    if (nDamage < 0)
    {
        // 0�ɌŒ�
        nDamage = 0;
    }

    // ���C�t����_���[�W������
    m_nEnemyLife -= nDamage;

    // �v���C���[���g�p�����J�[�h���F��łȂ��Ȃ�
    if (CGame::GetPlayerUseCard() != PRAY_ID)
    {
        // �U�����Đ�
        PlayAttackSound(nDamage);

        // �_���[�W���o
        CHit3D::Create(ENEMY_DAMAGE_POS, nDamage);
    }

    // ���C�t��0�ȉ��Ȃ�A0�ŌŒ�
    if (m_nEnemyLife <= 0)
    {
        m_nEnemyLife = 0;
    }

    // �ϐ��錾
    int nIndex;		        // �w���@(���C�t�\���p)
    int nRadix = 10;	    // ��@(���C�t�\���p)

    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_LIFE_DIGITS; nIndex++)
    {
        int nLife = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex);
        int nLife2 = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = m_nEnemyLife % nLife / nLife2;

        // �ԍ���ݒ�
        m_apNumber[nIndex + MAX_LIFE_DIGITS]->SetNumber(nAnswer);
    }
}

//==========================================
// �^�C�v����U���͂��擾
//==========================================
int CLife::GetAttackValue(const TYPE type)
{
    // �ϐ��錾
    int nAttack = 0;
    int nType = 0;

    switch (type)
    {
    case TYPE_PLAYER:
        // �v���C���[�̎g�����J�[�h���擾
        nType = CGame::GetPlayerUseCard();
        break;

    case TYPE_ENEMY:
        // �G�l�~�[�̎g�����J�[�h���擾
        nType = CGame::GetEnemyUseCard();
        break;
    }

    // �X�e�[�^�X���擾
    CCard::STATUS status;
    status = CCard::List(nType);

    // �U���͂����т���
    nAttack = status.nAttack;

    return nAttack;
}

//==========================================
// �^�C�v����h��͂��擾
//==========================================
int CLife::GetDefenceValue(const TYPE type)
{
    // �ϐ��錾
    int nDefence = 0;
    int nType = 0;

    switch (type)
    {
    case TYPE_PLAYER:
        // �v���C���[�̎g�����J�[�h���擾
        nType = CGame::GetPlayerUseCard();
        break;

    case TYPE_ENEMY:
        // �G�l�~�[�̎g�����J�[�h���擾
        nType = CGame::GetEnemyUseCard();
        break;
    }

    // �X�e�[�^�X���擾
    CCard::STATUS status;
    status = CCard::List(nType);

    // �h��͂����т���
    nDefence = status.nDefence;

    return nDefence;
}

//========================================
// �v���C���[�̉񕜏���
//========================================
void CLife::AddPlayerLife(const int nValue)
{
    // ���C�t���Z
    m_nPlayerLife += nValue;

    // �ϐ��錾
    int nIndex;		        // �w���@(���C�t�\���p)
    int nRadix = 10;	    // ��@(���C�t�\���p)

    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_LIFE_DIGITS; nIndex++)
    {
        int nLife = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex);
        int nLife2 = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = m_nPlayerLife % nLife / nLife2;

        // �ԍ���ݒ�
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}

//========================================
// �G�l�~�[�̉񕜏���
//========================================
void CLife::AddEnemyLife(const int nValue)
{
    // ���C�t���Z
    m_nEnemyLife += nValue;

    // �ϐ��錾
    int nIndex;		        // �w���@(���C�t�\���p)
    int nRadix = 10;	    // ��@(���C�t�\���p)

    // �e���ɂ��Čv�Z
    for (nIndex = 0; nIndex < MAX_LIFE_DIGITS; nIndex++)
    {
        int nLife = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex);
        int nLife2 = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = m_nPlayerLife % nLife / nLife2;

        // �ԍ���ݒ�
        m_apNumber[nIndex + MAX_LIFE_DIGITS]->SetNumber(nAnswer);
    }
}

//========================================
// �U�����Đ�����
//========================================
void CLife::PlayAttackSound(int nDamage)
{
    // ���擾
    CSound *pSound = CManager::GetSound();

    if (nDamage == 0)
    {
        // ���S�h��Đ�
        pSound->Play(CSound::LABEL_SE_004);
    }
    else if (nDamage > 0 && nDamage < BORDER_HIT_SOUND)
    {
        // �a�����Đ�
        pSound->Play(CSound::LABEL_SE_002);
    }
    else if (nDamage >= BORDER_HIT_SOUND)
    {
        // �a����Đ�
        pSound->Play(CSound::LABEL_SE_003);
    }
}
