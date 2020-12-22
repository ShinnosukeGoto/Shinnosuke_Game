//===============================================
//
// Button���� (button.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "button.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "card.h"
#include "scene.h"
#include "card_info.h"
#include "card_name.h"
#include "input.h"
#include "sound.h"

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CButton::m_apTexture[CButton::TYPE_MAX] = {};

//========================================
// Button�̃R���X�g���N�^
//========================================
CButton::CButton() :CScene2D(OBJTYPE::OBJTYPE_BUTTON)
{
    m_col = DEFAULT_COLOR;
    m_type = CButton::TYPE_MAX;

    m_nID = 0;
}

//========================================
// Button�̃f�X�g���N�^
//========================================
CButton::~CButton()
{

}

//========================================
// Button�̏���������
//========================================
HRESULT CButton::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// Button�̏I������
//========================================
void CButton::Uninit(void)
{
    // �I������
    CScene2D::Uninit();
}

//========================================
// Button�̍X�V����
//========================================
void CButton::Update(void)
{
    // �ϐ��錾
    D3DXVECTOR3 pos = GetPosition();                   // �ʒu���擾
    D3DXVECTOR3 size = GetSize();                      // �T�C�Y���擾

    switch (m_type)
    {
    case TYPE_FORGIVE:

        // �h��^�[���Ȃ�
        if (CGame::GetDefenceTurn() == true)
        {
            // ���ł��g����悤��
            m_col.a = 1.0f;
        }
        else
        {
            m_col.a = 0.0f;
        }

        break;

    case TYPE_PRAY:

        // �U���^�[���Ȃ�
        if (CGame::GetAttackTurn() == true)
        {
            // �U���J�[�h�����邩�ǂ����̃t���O���X�V
            CCard::CheckDispAttackCard();

            // �U���J�[�h���Ȃ��Ȃ�A�A�N�e�B�u��
            if (CCard::GetDispAttackCard() == false)
            {
                m_col.a = 1.0f;
            }
            else
            {
                m_col.a = 0.0f;
            }
        }
        else
        {
            m_col.a = 0.0f;
        }

        break;
    }

    // �Q�[���I�����ɁA�����ɂ���
    GameFinishFade();

    // �A�j���[�V�����p�[�g�ł́A�ؒf�Ή�
    DisconnectInAnim();

    // �A�N�e�B�u�Ȃ�A�I���\
    if (m_col.a == 1.0f)
    {
        size = CollisionCursor(pos, size);
    }

    // �F�𔽉f
    SetColor(m_col);

    // �ʒu�𔽉f
    SetPosition(pos);

    // �T�C�Y�𔽉f
    SetSize(size);

    // ���_���W��ݒ�
    SetVertex();
}

//========================================
// Button�̕`�揈��
//========================================
void CButton::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//========================================
// Button�̉摜���[�h����
//========================================
HRESULT CButton::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/button000.png", &m_apTexture[CButton::TYPE_FORGIVE]);	// ����
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/button001.png", &m_apTexture[CButton::TYPE_PRAY]);     // �F��

    return S_OK;
}

//========================================
// Button�̉摜�j������
//========================================
void CButton::Unload(void)
{
    // �e�N�X�`���̔j��
    for (int nCntTexture = 0; nCntTexture < CButton::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// Button�̍쐬
//================================
CButton *CButton::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CButton::TYPE type)
{
    CButton *pButton = NULL;

    // ���������m��
    pButton = new CButton;

    // �^�C�v�̂݁A�e�N�X�`�������蓖�Ă̂��߂Ɍ��т��Ă���
    pButton->m_type = type;

    // ������
    pButton->Init(pos, size);

    // �����o�ϐ������т���
    pButton->m_col = col;

    // ID�����т���
    switch (type)
    {
    case TYPE_FORGIVE:
        pButton->m_nID = FORGIVE_ID;
        break;

    case TYPE_PRAY:
        pButton->m_nID = PRAY_ID;
        break;
    }

    return pButton;
}

//================================
// �Q�[���I�����A����Button
//================================
void CButton::GameFinishFade(void)
{
    if (CGame::GetFinishBattle() == true)
    {
        m_col.a = 0.0f;
    }
}

//================================
// �{�^����I�������Ƃ�
//================================
D3DXVECTOR3 CButton::CollisionCursor(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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

                // �{�^���̓����蔻��̃T�C�Y��ݒ�
                D3DXVECTOR3 buttonCollisionSize = BUTTON_COLLISION_SIZE;

                // �����蔻����v�Z
                if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                    &buttonCollisionSize, &pScene2D->GetSize()) == true)
                {// ���������Ȃ�

                    // �J�[�h�����X�V
                    SetCardInfo();

                    // �g�債���T�C�Y��Ԃ�
                    returnSize = BUTTON_EXTEND_SIZE;

                    // �I���\
                    Select();

                    // �J�[�\���͈�Ȃ̂�for�𔲂���
                    break;
                }
                else
                {// �������ĂȂ��Ȃ�

                    // ���ʂ̃T�C�Y��Ԃ�
                    returnSize = BUTTON_SIZE;

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
void CButton::SetCardInfo(void)
{
    // �U���A�h��A�ǂ����0
    CCardInfo *pCardInfo = CGame::GetCardInfo();
    pCardInfo->SetCardInfo(0, 0);

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
                    if (pCardName->GetType() != m_nID)
                    {
                        // �Ώۂ𖢎g�p��
                        pCardName->SetDontUse();

                        // ���g�̖��O�𐶐�
                        CCardName::Create(CARD_NAME_POS, CARD_NAME_SIZE, m_nID, CCardName::USETYPE_PLAYER);
                    }
                }
            }
        }
    }
}

//===================================================================
// �I��
//===================================================================
void CButton::Select(void)
{
    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // �����擾
    CSound *pSound = CManager::GetSound();

    // ����L�[����������
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B))
    {
        switch (m_type)
        {
        case TYPE_FORGIVE:

            // �J�[�h�𑊎�ɑ���
            CManager::SendCard(FORGIVE_ID);

            // �h��^�[�����I����
            CGame::SetDefenceTurn(false);

            // �J�[�h���艹
            pSound->Play(CSound::LABEL_SE_008);

            break;

        case TYPE_PRAY:

            // �J�[�h�𑊎�ɑ���
            CManager::SendCard(PRAY_ID);

            // �U���^�[�����I����
            CGame::SetAttackTurn(false);

            // �J�[�h���艹
            pSound->Play(CSound::LABEL_SE_008);

            //===============================================================
            // �����_���Ȓl�ŁA�J�[�h�𓾂�
            CCard::DrawCard(1);
            //===============================================================

            break;
        }
    }
}

//================================
// �A�j���[�V�����p�[�g�ł́A�ؒf�Ή�
//================================
void CButton::DisconnectInAnim(void)
{
    // �A�j���[�V�����p�[�g�̐ؒf�Ή�
    if (CManager::GetEnemyDisconnect() == true && CGame::GetState() == CGame::STATE_PLAYER_ATTACK ||
        CManager::GetEnemyDisconnect() == true && CGame::GetState() == CGame::STATE_ENEMY_ATTACK)
    {
        m_col.a = 0.0f;
    }
}
