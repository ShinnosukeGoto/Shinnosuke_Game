//===================================================================
//
// �J�[�h���� (card.cpp)
// Author : �㓡�T�V��
//
//===================================================================

//===================================================================
// �C���N���[�h�t�@�C��
//===================================================================
#include "card.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "card_info.h"
#include "card_name.h"
#include "sound.h"

//===================================================================
// �}�N����`
//===================================================================
#define INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)                       // ��A�N�e�B�u�F

///==================================================================
// �ÓI�����o�ϐ��錾
//===================================================================
LPDIRECT3DTEXTURE9 CCard::m_apTexture[CCard::TYPE_MAX] = {};
int CCard::m_nNumAll = 0;
bool CCard::m_bDispDefenceCard = false;
bool CCard::m_bDispAttackCard = false;

//===================================================================
// �J�[�h�̃R���X�g���N�^
//===================================================================
CCard::CCard() :CScene2D(OBJTYPE::OBJTYPE_CARD)
{
    m_posDest = DEFAULT_VECTOR;
    m_nType = 0;
    m_bUse = true;  // �g�p��
    m_nMoveTime = 0;

    m_nAttackValue = 0;
    m_nDefenceValue = 0;
}

//===================================================================
// �J�[�h�̃f�X�g���N�^
//===================================================================
CCard::~CCard()
{

}

//===================================================================
// �J�[�h�̏���������
//===================================================================
HRESULT CCard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_apTexture[m_nType]);

    return S_OK;
}

//===================================================================
// �J�[�h�̏I������
//===================================================================
void CCard::Uninit(void)
{
    // ��D�̍ő吔���X�V
    m_nNumAll--;

    // �I������
    CScene2D::Uninit();
}

//===================================================================
// �J�[�h�̍X�V����
//===================================================================
void CCard::Update(void)
{
    // �ϐ��錾
    D3DXVECTOR3 pos = GetPosition();    // �ʒu���擾
    D3DXVECTOR3 size = GetSize();       // �T�C�Y���擾

    // �U���^�[���Ȃ�
    if (CGame::GetAttackTurn() == true)
    {
        // �U���͂̂���J�[�h���A�N�e�B�u��
        if (m_nAttackValue > 0)
        {
            SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        }
        // �Ȃ��J�[�h�́A��A�N�e�B�u��
        else
        {
            SetColor(INACTIVE_COLOR);
        }
    }

    // �h��^�[���Ȃ�
    if (CGame::GetDefenceTurn() == true)
    {
        // �h��͂̂���J�[�h���A�N�e�B�u��
        if (m_nDefenceValue > 0)
        {
            SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        }
        // �Ȃ��J�[�h�́A��A�N�e�B�u��
        else
        {
            SetColor(INACTIVE_COLOR);
        }
    }

    // �U���^�[���ł��A�h��^�[���ł��Ȃ��Ȃ�A�ǂ̃J�[�h���A�N�e�B�u��
    if (CGame::GetAttackTurn() == false && CGame::GetDefenceTurn() == false)
    {
        SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // �ړI�̈ʒu�Ɉړ�
    pos = Move(pos);

    // �J�[�\���Ƃ̓����蔻��
    size = CollisionCursor(pos, size);

    // �ʒu�𔽉f
    SetPosition(pos);

    // �傫���𔽉f
    SetSize(size);

    // ���_���W��ύX
    SetVertex();

    // �g�p�t���O��false�A�܂��̓Q�[���I�������Ȃ����
    if (m_bUse == false || CGame::GetFinishBattle() == true)
    {
        Uninit();
    }
}

//===================================================================
// �J�[�h�̕`�揈��
//===================================================================
void CCard::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//===================================================================
// �J�[�h�̉摜���[�h����
//===================================================================
HRESULT CCard::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_01_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_000]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_02_Hood.png", &m_apTexture[CCard::TYPE_ARMOR_001]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_03_Mail.png", &m_apTexture[CCard::TYPE_ARMOR_002]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_05_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_003]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_06_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_004]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_07_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_005]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_08_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_006]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_09_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_007]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_04_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_FINAL]);

    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_01_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_000]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_02_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_001]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_03_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_002]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_05_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_003]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_06_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_004]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_07_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_005]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_08_club.png", &m_apTexture[CCard::TYPE_WEAPON_006]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_09_axe.png", &m_apTexture[CCard::TYPE_WEAPON_007]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_10_whip.png", &m_apTexture[CCard::TYPE_WEAPON_008]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_11_boomerang.png", &m_apTexture[CCard::TYPE_WEAPON_009]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_12_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_010]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_04_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_FINAL]);

    return S_OK;
}

//===================================================================
// �J�[�h�̉摜�j������
//===================================================================
void CCard::Unload(void)
{
    // �e�N�X�`���̔j��
    for (int nCntTexture = 0; nCntTexture < CCard::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//===================================================================
// �J�[�h�̍쐬
//===================================================================
CCard *CCard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int type)
{
    CCard *pCard = NULL;

    // ���������m��
    pCard = new CCard;

    // �^�C�v�̂݁A�e�N�X�`�����蓖�Ă̂��߂Ɍ��т��Ă���
    pCard->m_nType = type;

    // ������
    pCard->Init(pos, size);

    // �����o�ϐ��̏�����
    pCard->m_posDest = pos;

    // �U���́A�h��͂��擾
    STATUS status;
    status = List(type);
    pCard->m_nAttackValue = status.nAttack;
    pCard->m_nDefenceValue = status.nDefence;

    return pCard;
}

//===================================================================
// �J�[�\���Ƃ̓����蔻��
//===================================================================
D3DXVECTOR3 CCard::CollisionCursor(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // �ϐ��錾
    D3DXVECTOR3 returnSize = size;  // �Ԃ��T�C�Y

    // �J�[�\���Ƃ̓����蔻��
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // �V�[�����擾
        CScene *pScene = GetScene(nCntScene);

        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // �^�C�v���擾
            OBJTYPE objType = pScene->GetObjType();

            // �J�[�\���Ȃ�A
            if (objType == CScene::OBJTYPE_CURSOR)
            {
                // �V�[��2D�ɃL���X�g
                CScene2D *pScene2D = (CScene2D*)pScene;

                // �J�[�h�̓����蔻��̃T�C�Y��ݒ�
                D3DXVECTOR3 cardCollisionSize = CARD_COLLISION_SIZE;

                // �����蔻����v�Z
                if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                    &cardCollisionSize, &pScene2D->GetSize()) == true)
                {// ���������Ȃ�

                    // �J�[�h�����X�V
                    SetCardInfo();

                    // �g�債���T�C�Y��Ԃ�
                    returnSize = CARD_EXTEND_SIZE;

                    // �J�[�h�̑I�����\
                    SelectCard();

                    // �J�[�\���͈�Ȃ̂�for�𔲂���
                    break;
                }
                else
                {// �������ĂȂ��Ȃ�

                    // ���ʂ̃T�C�Y��Ԃ�
                    returnSize = CARD_SIZE;

                    // �J�[�\���͈�Ȃ̂�for�𔲂���
                    break;
                }
            }
        }
    }

    return returnSize;
}

//===================================================================
// �J�[�h���̐ݒ�
//===================================================================
void CCard::SetCardInfo(void)
{
    // �J�[�h�̍U����h��̐��l��ݒ�
    CCardInfo *pCardInfo = CGame::GetCardInfo();
    pCardInfo->SetCardInfo(m_nAttackValue, m_nDefenceValue);

    // �J�[�h����ݒ�
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // �V�[�����擾
        CScene *pScene = GetScene(nCntScene);

        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // �^�C�v���擾
            OBJTYPE objType = pScene->GetObjType();

            // �J�[�h���Ȃ�
            if (objType == CScene::OBJTYPE_CARD_NAME)
            {
                // �J�[�h���ɃL���X�g
                CCardName *pCardName = (CCardName*)pScene;

                // ���g�̃J�[�h�^�C�v�Ȃ�
                if (pCardName->GetUseType() == CCardName::USETYPE_PLAYER)
                {
                    // ���g�ƈႤ�J�[�h�Ȃ�
                    if (pCardName->GetType() != m_nType)
                    {
                        // �Ώۂ𖢎g�p��
                        pCardName->SetDontUse();

                        // ���g�̖��O�𐶐�
                        CCardName::Create(CARD_NAME_POS, CARD_NAME_SIZE, m_nType, CCardName::USETYPE_PLAYER);
                    }
                }
            }
        }
    }
}

//===================================================================
// �g���J�[�h�̑I��
//===================================================================
void CCard::SelectCard(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // ����L�[����������
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B))
    {
        //***********************************************************************************
        // �U���^�[���Ȃ�
        //***********************************************************************************
        if (CGame::GetAttackTurn() == true)
        {
            // �U���J�[�h�̂�
            if (m_nType >= TYPE_WEAPON_000 && m_nType <= TYPE_WEAPON_FINAL)
            {
                // �J�[�h�𑊎�ɑ���
                CManager::SendCard(m_nType);

                // �U���^�[�����I����
                CGame::SetAttackTurn(false);

                // ����
                m_bUse = false;

                // �J�[�h���艹
                CSound *pSound = CManager::GetSound();
                pSound->Play(CSound::LABEL_SE_008);

                //===============================================================
                // �����_���Ȓl�ŁA�J�[�h�𓾂�
                DrawCard(1);
                //===============================================================
            }
        }
        
        //***********************************************************************************
        // �h��^�[���Ȃ�
        //***********************************************************************************
        if (CGame::GetDefenceTurn() == true)
        {
            // �h��J�[�h�̂�
            if (m_nType >= TYPE_ARMOR_000 && m_nType <= TYPE_ARMOR_FINAL)
            {
                // �J�[�h�𑊎�ɑ���
                CManager::SendCard(m_nType);

                // �h��^�[�����I����
                CGame::SetDefenceTurn(false);

                // ����
                m_bUse = false;

                // �J�[�h���艹
                CSound *pSound = CManager::GetSound();
                pSound->Play(CSound::LABEL_SE_008);

                //===============================================================
                // �����_���Ȓl�ŁA�J�[�h�𓾂�
                DrawCard(1);
                //===============================================================
            }
        }
    }
}

//===================================================================
// �J�[�h�̕��ёւ�
//===================================================================
void CCard::SortCards(void)
{
    // �ϐ��錾
    CCard *apCard[MAX_OBJECT];   // �J�[�h�̈ʒu��ς��邽�߁A�z��ŏ����L��
    int nIndex = 0;              // �擾�ƃ\�[�g�̃C���f�b�N�X
    CCard *pCard;                // �\�[�g���ɁA�Ⴂ�ԍ��̒l���L���p

    // �z��̏�����
    memset(apCard, 0, sizeof(apCard));

    // �J�[�h�̎擾
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // �V�[�����擾
        CScene *pScene = GetScene(nCntScene);

        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // �^�C�v���擾
            OBJTYPE objType = pScene->GetObjType();

            // �J�[�h�Ȃ�A
            if (objType == CScene::OBJTYPE_CARD)
            {
                // �J�[�h�^�ɃL���X�g
                CCard *pCard = (CCard*)pScene;

                // �z��ɋL��
                apCard[nIndex] = pCard;

                // �C���f�b�N�X����i�߂�
                nIndex++;                  
            }
        }
    }

    // �J�[�h�̏����\�[�g(�I��@)���g���āA�^�C�v���ɕ��ёւ���i�����j
    for (int nNum01 = 0; nNum01 < nIndex - 1; nNum01++)
    {
        for (int nNum02 = nNum01 + 1; nNum02 < nIndex; nNum02++)
        {
            // �ǂ�������g������Ȃ�
            if (apCard[nNum01] != NULL && apCard[nNum02] != NULL)
            {
                // �Ⴂ�ԍ��̃^�C�v�̂ق����傫���Ȃ�
                if (apCard[nNum01]->GetType() > apCard[nNum02]->GetType())
                {
                    // �Ⴂ�ԍ����L��
                    pCard = apCard[nNum01];

                    // �Ⴂ�ԍ��Ɏ��̔ԍ�����
                    apCard[nNum01] = apCard[nNum02];

                    // ���̔ԍ��ɋL���������̂���
                    apCard[nNum02] = pCard;
                }
            }
        }
    }

    // �ϐ��錾
    int nIndexPos = 0;           // �ʒu�p�̃C���f�b�N�X

    // �J�[�h��z�u���Ă���
    for (int nCnt = 0; nCnt < nIndex; nCnt++)
    {
        // �V�[��2D�ɃL���X�g
        CScene2D *pScene2D = (CScene2D*)apCard[nCnt];

        // �ʒu���擾
        D3DXVECTOR3 pos = pScene2D->GetPosition();
        
        // �ʒu���Ԋu�������Čv�Z
        D3DXVECTOR3 spacePos = D3DXVECTOR3(CARD_FIRST_POS_X + CARD_SPACE_X * nIndexPos, CARD_POS_Y, 0.0f);

        // �ړI�̈ʒu��ݒ�
        apCard[nCnt]->SetPosDest(spacePos);

        // �ʒu�𔽉f
        pScene2D->SetPosition(spacePos);

        // ���_���W��ύX
        pScene2D->SetVertex();

        // �C���f�b�N�X����i�߂�
        nIndexPos++;
    }
}

//===================================================================
// �J�[�h�̕��ёւ�(�ړ����銴��)
//===================================================================
void CCard::SortMoveCards(void)
{
    // �ϐ��錾
    CCard *apCard[MAX_OBJECT];   // �J�[�h�̈ʒu��ς��邽�߁A�z��ŏ����L��
    int nIndex = 0;              // �擾�ƃ\�[�g�̃C���f�b�N�X
    CCard *pCard;                // �\�[�g���ɁA�Ⴂ�ԍ��̒l���L���p

    // �z��̏�����
    memset(apCard, 0, sizeof(apCard));

    // �J�[�h�̎擾
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // �V�[�����擾
        CScene *pScene = GetScene(nCntScene);

        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // �^�C�v���擾
            OBJTYPE objType = pScene->GetObjType();

            // �J�[�h�Ȃ�A
            if (objType == CScene::OBJTYPE_CARD)
            {
                // �J�[�h�^�ɃL���X�g
                CCard *pCard = (CCard*)pScene;

                // �z��ɋL��
                apCard[nIndex] = pCard;

                // �C���f�b�N�X����i�߂�
                nIndex++;
            }
        }
    }

    // �J�[�h�̏����\�[�g(�I��@)���g���āA�^�C�v���ɕ��ёւ���i�����j
    for (int nNum01 = 0; nNum01 < nIndex - 1; nNum01++)
    {
        for (int nNum02 = nNum01 + 1; nNum02 < nIndex; nNum02++)
        {
            // �ǂ�������g������Ȃ�
            if (apCard[nNum01] != NULL && apCard[nNum02] != NULL)
            {
                // �Ⴂ�ԍ��̃^�C�v�̂ق����傫���Ȃ�
                if (apCard[nNum01]->GetType() > apCard[nNum02]->GetType())
                {
                    // �Ⴂ�ԍ����L��
                    pCard = apCard[nNum01];

                    // �Ⴂ�ԍ��Ɏ��̔ԍ�����
                    apCard[nNum01] = apCard[nNum02];

                    // ���̔ԍ��ɋL���������̂���
                    apCard[nNum02] = pCard;
                }
            }
        }
    }

    // �ϐ��錾
    int nIndexPos = 0;           // �ʒu�p�̃C���f�b�N�X

    // �J�[�h��z�u���Ă���
    for (int nCnt = 0; nCnt < nIndex; nCnt++)
    {
        // �ʒu���擾
        D3DXVECTOR3 pos = apCard[nCnt]->GetPosition();

        // �ʒu���Ԋu�������Čv�Z
        D3DXVECTOR3 spacePos = D3DXVECTOR3(CARD_FIRST_POS_X + CARD_SPACE_X * nIndexPos, CARD_POS_Y, 0.0f);

        // �ړI�̈ʒu��ݒ�
        apCard[nCnt]->SetPosDest(spacePos);

        // �C���f�b�N�X����i�߂�
        nIndexPos++;
    }
}

//===================================================================
// �J�[�h�̈ړ�
//===================================================================
D3DXVECTOR3 CCard::Move(D3DXVECTOR3 pos)
{
    // �ϐ��錾
    D3DXVECTOR3 posMove = pos;  // �ړ��̈ʒu

    float fTargetPosX = 0.0f;                  // �ڕW��X���W
    float fTargetPosY = 0.0f;                  // �ڕW��Y���W
    float fMyPosX = 0.0f;                      // ���g��X���W
    float fMyPosY = 0.0f;                      // ���g��Y���W
    float fAngle = 0.0f;                       // ���߂�p�x

    // �ڕW�̍��W�Ǝ��g�̍��W
    fTargetPosX = pos.x, fTargetPosY = pos.y;
    fMyPosX = m_posDest.x, fMyPosY = m_posDest.y;

    // �p�x�����߂�
    fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

    // �ڕW�̈ʒu�Ƃ́A�����_�ł̋���
    float fDistance = sqrtf(
        powf((pos.x - m_posDest.x), 2) +
        powf((pos.y - m_posDest.y), 2));

    // �ړI�̈ʒu�Ƌ߂����ǂ���
    if (fDistance <= CARD_CONSIDER_CLOSE_DEST)
    {
        // �߂��Ȃ�ړI�̈ʒu�ɂ���
        posMove = m_posDest;

        // �ړ��̃J�E���^�����Z�b�g
        m_nMoveTime = 0;
    }
    else
    {
        // �J�[�h�̃X�s�[�h�������ƃ\�[�g���Ԃ��狁�߂�
        float fSpeed = fDistance / (CARD_SORT_TIME - (float)m_nMoveTime);

        // �����Ȃ�߂Â��Ă���
        posMove = D3DXVECTOR3(pos.x + sinf(fAngle)*fSpeed, pos.y + cosf(fAngle)*fSpeed, 0.0f);

        // �ړ��̌o�ߎ���
        m_nMoveTime++;
    }

    return posMove;
}

//===================================================================
// �J�[�h���X�g
//===================================================================
CCard::STATUS CCard::List(int nType)
{
    // �ϐ��錾
    STATUS status;          // �X�e�[�^�X
    status.nAttack = 0;     // �U����
    status.nDefence = 0;    // �h���

     //***********************************************************************************
    // �J�[�h���ƂɁA�Œ�̒l�����т���
    switch (nType)
    {
        //==========================================================
        // �h��
    case TYPE_ARMOR_000:
        status.nDefence = CARD_ARMOR_000;
        break;
    case TYPE_ARMOR_001:
        status.nDefence = CARD_ARMOR_001;
        break;
    case TYPE_ARMOR_002:
        status.nDefence = CARD_ARMOR_002;
        break;
    case TYPE_ARMOR_003:
        status.nDefence = CARD_ARMOR_003;
        break;
    case TYPE_ARMOR_004:
        status.nDefence = CARD_ARMOR_004;
        break;
    case TYPE_ARMOR_005:
        status.nDefence = CARD_ARMOR_005;
        break;
    case TYPE_ARMOR_006:
        status.nDefence = CARD_ARMOR_006;
        break;
    case TYPE_ARMOR_007:
        status.nDefence = CARD_ARMOR_007;
        break;
    case TYPE_ARMOR_FINAL:
        status.nDefence = CARD_ARMOR_FINAL;
        break;
        //==========================================================

        //==========================================================
        // ����
    case TYPE_WEAPON_000:
        status.nAttack = CARD_WEAPON_000;
        break;
    case TYPE_WEAPON_001:
        status.nAttack = CARD_WEAPON_001;
        break;
    case TYPE_WEAPON_002:
        status.nAttack = CARD_WEAPON_002;
        break;
    case TYPE_WEAPON_003:
        status.nAttack = CARD_WEAPON_003;
        break;
    case TYPE_WEAPON_004:
        status.nAttack = CARD_WEAPON_004;
        break;
    case TYPE_WEAPON_005:
        status.nAttack = CARD_WEAPON_005;
        break;
    case TYPE_WEAPON_006:
        status.nAttack = CARD_WEAPON_006;
        break;
    case TYPE_WEAPON_007:
        status.nAttack = CARD_WEAPON_007;
        break;
    case TYPE_WEAPON_008:
        status.nAttack = CARD_WEAPON_008;
        break;
    case TYPE_WEAPON_009:
        status.nAttack = CARD_WEAPON_009;
        break;
    case TYPE_WEAPON_010:
        status.nAttack = CARD_WEAPON_010;
        break;
    case TYPE_WEAPON_FINAL:
        status.nAttack = CARD_WEAPON_FINAL;
        break;
        //==========================================================
    }
    //***********************************************************************************

    return status;
}

//===================================================================
// �J�[�h������
//===================================================================
void CCard::DrawCard(int nSheets)
{
    // �l�܂Ȃ��悤�ɁA�J�[�h�������_���ɐ���
    for (int nCnt = 0; nCnt < nSheets; nCnt++)
    {
        // �J�[�h���ő吔-1�Ȃ�
        if (m_nNumAll >= MAX_CARD - 1)
        {
            // �U���J�[�h�����邩�ǂ����̃t���O���X�V
            CheckDispAttackCard();

            // �U���J�[�h���Ȃ��Ȃ�
            if (m_bDispAttackCard == false)
            {
                // �U���J�[�h���烉���_���Ɏ�ɓ����
                int nRandNum = GetRandNum(TYPE_WEAPON_FINAL, TYPE_WEAPON_000);
                CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNum);
            }
            // �U���J�[�h������Ȃ�ʏ�̈���
            else
            {
                // 1����100�𓾂�
                int nRandNum = GetRandNum(100, 1);

                // �h��o��
                if (nRandNum <= ARMOR_PERCENTAGE)
                {
                    // �h��J�[�h���烉���_���Ɏ�ɓ����
                    int nRandNumber = GetRandNum(TYPE_ARMOR_FINAL, TYPE_ARMOR_000);
                    CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNumber);
                }
                // ���킪�o��
                else
                {
                    // �U���J�[�h���烉���_���Ɏ�ɓ����
                    int nRandNumber = GetRandNum(TYPE_WEAPON_FINAL, TYPE_WEAPON_000);
                    CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNumber);
                }
            }
        }
        else
        {
            // 1����100�𓾂�
            int nRandNum = GetRandNum(100, 1);

            // �h��o��
            if (nRandNum <= ARMOR_PERCENTAGE)
            {
                // �h��J�[�h���烉���_���Ɏ�ɓ����
                int nRandNumber = GetRandNum(TYPE_ARMOR_FINAL, TYPE_ARMOR_000);
                CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNumber);
            }
            // ���킪�o��
            else
            {
                // �U���J�[�h���烉���_���Ɏ�ɓ����
                int nRandNumber = GetRandNum(TYPE_WEAPON_FINAL, TYPE_WEAPON_000);
                CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNumber);
            }
        }

        // ��D�̍ő吔���X�V
        m_nNumAll++;
    }
}

//===================================================================
// �h��J�[�h�����邩�ǂ����`�F�b�N
//===================================================================
void CCard::CheckDispDefenceCard(void)
{
    // �����o�ϐ��̃t���O����xfalse��
    m_bDispDefenceCard = false;

    // �J�[�h�̎擾
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // �V�[�����擾
        CScene *pScene = GetScene(nCntScene);

        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // �^�C�v���擾
            OBJTYPE objType = pScene->GetObjType();

            // �J�[�h�Ȃ�A
            if (objType == CScene::OBJTYPE_CARD)
            {
                // �J�[�h�^�ɃL���X�g
                CCard *pCard = (CCard*)pScene;

                // �h��J�[�h�Ȃ�
                if (pCard->GetType() >= CCard::TYPE_ARMOR_000 && pCard->GetType() <= CCard::TYPE_ARMOR_FINAL)
                {
                    // �t���O��true��
                    m_bDispDefenceCard = true;

                    break;
                }
            }
        }
    }
}

//===================================================================
// �U���J�[�h�����邩�ǂ����`�F�b�N
//===================================================================
void CCard::CheckDispAttackCard(void)
{
    // �����o�ϐ��̃t���O����xfalse��
    m_bDispAttackCard = false;

    // �J�[�h�̎擾
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // �V�[�����擾
        CScene *pScene = GetScene(nCntScene);

        // ���g������Ȃ�
        if (pScene != NULL)
        {
            // �^�C�v���擾
            OBJTYPE objType = pScene->GetObjType();

            // �J�[�h�Ȃ�A
            if (objType == CScene::OBJTYPE_CARD)
            {
                // �J�[�h�^�ɃL���X�g
                CCard *pCard = (CCard*)pScene;

                // �U���J�[�h�Ȃ�
                if (pCard->GetType() >= CCard::TYPE_WEAPON_000 && pCard->GetType() <= CCard::TYPE_WEAPON_FINAL)
                {
                    // �t���O��true��
                    m_bDispAttackCard = true;

                    break;
                }
            }
        }
    }
}
