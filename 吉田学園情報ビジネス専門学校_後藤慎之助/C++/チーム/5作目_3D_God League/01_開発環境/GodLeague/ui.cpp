//===============================================
//
// UI���� (ui.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "title.h"

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CUI::m_apTexture[CUI::TYPE_MAX] = {};

//========================================
// UI�̃R���X�g���N�^
//========================================
CUI::CUI() :CScene2D(OBJTYPE::OBJTYPE_UI)
{
    m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_col = DEFAULT_COLOR;
    m_type = CUI::TYPE_NONE;
    m_fAngle = 0.0f;
    m_bUse = true;
    m_bBlinking = false;
    m_nCntTime = 0;
    m_bFadeOut = false;
}

//========================================
// UI�̃f�X�g���N�^
//========================================
CUI::~CUI()
{

}

//========================================
// UI�̏���������
//========================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// UI�̏I������
//========================================
void CUI::Uninit(void)
{
    // �I������
    CScene2D::Uninit();
}

//========================================
// UI�̍X�V����
//========================================
void CUI::Update(void)
{
    // �ϐ��錾
    D3DXVECTOR3 pos = GetPosition();                   // �ʒu���擾
    D3DXVECTOR3 size = GetSize();                      // �T�C�Y���擾
    int nPatternAnim = 0;                              // �p�^�[���A�j���̃J�E���^�𗘗p����Ƃ�

    //�L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    switch (m_type)
    {
    case TYPE_TITLE_LOGO:

        // ���ꗎ����
        pos.y += TITLE_LOGO_FLOW_DOWN_SPEED;

        // ���̒l�Ŏ~�߂Ă���
        if (pos.y >= TITLE_LOGO_TARGET_POS_Y)
        {
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        // �G���^�[�A�܂��̓X�^�[�g�{�^������������
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // �^�C�g�����S�������ɉ���
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        break;

    case TYPE_PRESS_START:

        // �^�C�g���̎��Ԃ��J�E���g
        m_nCntTime++;

        // �^�C�g���̍ő厞��
        if (m_nCntTime > TITLE_MAX_TIME)
        {
            m_nCntTime = TITLE_MAX_TIME;
        }

        // �F��ς���
        if (m_bFadeOut == true)
        {
            m_col.r -= PRESS_START_FADE_RATE;
            m_col.g -= PRESS_START_FADE_RATE;
            m_col.b -= PRESS_START_FADE_RATE;
        }
        else
        {
            m_col.r += PRESS_START_FADE_RATE;
            m_col.g += PRESS_START_FADE_RATE;
            m_col.b += PRESS_START_FADE_RATE;
        }
        if (m_col.r <= PRESS_START_FADE_OUT_MIN)
        {
            m_bFadeOut = false;
        }
        else if (m_col.r >= 1.0f)
        {
            m_bFadeOut = true;
        }

        // �G���^�[�A�܂��̓X�^�[�g�{�^������������
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // ��莞�Ԍo�߂œ_�ŉ\��
            if (m_nCntTime >= TITLE_NEXT_SCENE)
            {
                // �_�ł̃t���O��true��
                m_bBlinking = true;
            }
            // ����ȊO�́A�J�E���^��J�ډ\�܂ň�C�ɐi�߂�
            else
            {
                m_nCntTime = TITLE_NEXT_SCENE;
            }
        }

        // �_�ł���Ȃ�
        if (m_bBlinking == true)
        {
            // �}�b�`���O�ҋ@���A�ڂ��`�J�`�J����̂ł�߂�
            //// �J�E���^�[��i�߂āA�p�^�[����؂�ւ���
            //nPatternAnim = CountAnimation(2, 2);

            //// �F��2�p�^�[���Ő؂�ւ���
            //if (nPatternAnim == 1)
            //{
            //    m_col.r = PRESS_START_FADE_OUT_MIN;
            //    m_col.g = PRESS_START_FADE_OUT_MIN;
            //    m_col.b = PRESS_START_FADE_OUT_MIN;
            //}
            //else
            //{
            //    m_col.r = 1.0f;
            //    m_col.g = 1.0f;
            //    m_col.b = 1.0f;
            //}
        }

        break;

    case TYPE_GAME_UNDER:

        // �퓬�I�����A����
        GameFinishFade();

        break;

    case TYPE_CARD_INFO_BG:

        // �퓬�I�����A����
        GameFinishFade();

        break;

    case TYPE_GAME_START:

        // �g��
        size = Extend(size);

        // �퓬�I�����A����
        GameFinishFade();

        break;

    case TYPE_ATTACK_START:

        // �g��
        size = Extend(size);

        // �퓬�I�����A����
        GameFinishFade();

        break;

    case TYPE_DEFENCE_START:

        // �g��
        size = Extend(size);

        // �퓬�I�����A����
        GameFinishFade();

        break;

    case TYPE_YOU_LOSE:

        // �g��
        size = Extend(size);

        break;

    case TYPE_YOU_WIN:

        // �g��
        size = Extend(size);

        break;

    case TYPE_DISCONNECT:

        // �g��
        size = Extend(size);

        break;

    case TYPE_TITLE_UI04:

        // �ҋ@��
        Wait();

        // ��
        m_fAngle += UI_NOW_LOADING_ROT_SPEED;

        break;

    case TYPE_WAIT:

        // �ҋ@��
        Wait();

        break;

    case TYPE_ENEMY_USE_ATTACK:

        // �퓬�I�����A����
        GameFinishFade();

        break;

    case TYPE_ENEMY_USE_DEFENCE:

        // �퓬�I�����A����
        GameFinishFade();

        break;

    case TYPE_MANUAL:

        // �퓬�I�����A����
        GameFinishFade();

        break;
    }

    // �F�𔽉f
    SetColor(m_col);

    // �ʒu�𔽉f
    SetPosition(pos);

    // �T�C�Y�𔽉f
    SetSize(size);

    // ���_���W��ݒ�
    if (m_type == TYPE_TITLE_UI04)
    {
        // ���[�h���̉�]
        SetRotVertex(m_fAngle);
    }
    else
    {
        SetVertex();
    }

    // �g�p�t���O��false�Ȃ����
    if (m_bUse == false)
    {
        Uninit();
    }
}

//========================================
// UI�̕`�揈��
//========================================
void CUI::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//========================================
// UI�̉摜���[�h����
//========================================
HRESULT CUI::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //�e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ver.png", &m_apTexture[CUI::TYPE_VERSION]);	             // �o�[�W����
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui014.png", &m_apTexture[CUI::TYPE_TITLE_LOGO]);	         // �^�C�g�����S
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui015.png", &m_apTexture[CUI::TYPE_PRESS_START]);	         // �X�^�[�g�{�^���������Ă�
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/game000.png", &m_apTexture[CUI::TYPE_GAME_UNDER]);	         // �Q�[���̉��̃o�[
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui000.png", &m_apTexture[CUI::TYPE_HP]);	                 // HP
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui001.png", &m_apTexture[CUI::TYPE_YOU]);	                 // ���Ȃ�
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui002.png", &m_apTexture[CUI::TYPE_ENEMY]);	             // ������
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui003.png", &m_apTexture[CUI::TYPE_FIRST]);	             // ��U
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui004.png", &m_apTexture[CUI::TYPE_SECOND]);	             // ��U
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui016.png", &m_apTexture[CUI::TYPE_GAME_START]);	         // �Q�[���J�n
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui005.png", &m_apTexture[CUI::TYPE_ATTACK_START]);	         // �U���J�n
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui006.png", &m_apTexture[CUI::TYPE_DEFENCE_START]);         // �h��J�n
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui007.png", &m_apTexture[CUI::TYPE_CARD_INFO_BG]);	         // �J�[�h���̔w�i
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui008.png", &m_apTexture[CUI::TYPE_YOU_WIN]);               // ����
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui009.png", &m_apTexture[CUI::TYPE_YOU_LOSE]);	             // ����
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui010.png", &m_apTexture[CUI::TYPE_ENEMY_USE_ATTACK]);      // �G�̍U��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui011.png", &m_apTexture[CUI::TYPE_ENEMY_USE_DEFENCE]);	 // �G�̖h��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui012.png", &m_apTexture[CUI::TYPE_MANUAL]);	             // �V�ѕ�
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui013.png", &m_apTexture[CUI::TYPE_WAIT]);	                 // �����ҋ@��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui017.png", &m_apTexture[CUI::TYPE_DISCONNECT]);	         // �ؒf

    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI000.png", &m_apTexture[CUI::TYPE_TITLE_UI00]);// �ΐ푊���T���Ă��܂��E�E�E
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI001.png", &m_apTexture[CUI::TYPE_TITLE_UI01]);// �ΐ푊�肪������܂����I
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI002.png", &m_apTexture[CUI::TYPE_TITLE_UI02]);// �ΐ푊�肪������܂���ł���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI003.png", &m_apTexture[CUI::TYPE_TITLE_UI03]);// �����o��
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI004.png", &m_apTexture[CUI::TYPE_TITLE_UI04]);// ���[�h���ɉ�]������

    return S_OK;
}

//========================================
// UI�̉摜�j������
//========================================
void CUI::Unload(void)
{
    // �e�N�X�`���̔j��
    for (int nCntTexture = 0; nCntTexture < CUI::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// UI�̍쐬
//================================
CUI *CUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CUI::TYPE type)
{
    CUI *pUI = NULL;

    // ���������m��
    pUI = new CUI;

    // �^�C�v�̂݁A�e�N�X�`�������蓖�Ă̂��߂Ɍ��т��Ă���
    pUI->m_type = type;

    // ������
    pUI->Init(pos, size);

    // �����o�ϐ������т���
    pUI->m_col = col;

    return pUI;
}

//================================
// �Q�[���I�����A����UI
//================================
void CUI::GameFinishFade(void)
{
    if (CGame::GetFinishBattle() == true)
    {
        m_col.a = 0.0f;
    }
}

//================================
// �����ҋ@��
//================================
void CUI::Wait(void)
{
    // �Q�[�����Ȃ�
    if (CManager::GetMode() == CManager::MODE_GAME)
    {
        // �U���^�[���ł��A�h��^�[���ł��Ȃ��Ȃ�
        if (CGame::GetAttackTurn() == false && CGame::GetDefenceTurn() == false)
        {
            m_col.a = 1.0f;
        }
        else
        {
            m_col.a = 0.0f;
        }

        // �Q�[���̃X�e�[�g���ʏ�ȊO�Ȃ�
        if (CGame::GetState() != CGame::STATE_NORMAL)
        {
            m_col.a = 0.0f;
        }

        // �퓬�I�����A����
        GameFinishFade();
    }
}

//================================
// �g�傷��UI
//================================
D3DXVECTOR3 CUI::Extend(D3DXVECTOR3 size)
{
    // �ϐ��錾
    D3DXVECTOR3 returnSize = size;  // �Ԃ��T�C�Y
    int nCnt = 0;                   // �J�E���^

    // �ؒf�ȊO�Ȃ�
    if (m_type != TYPE_DISCONNECT)
    {
        // ���T�C�Y�g��
        returnSize.x += UI_DISCONNECT_EXTEND_RATE_X;

        // �c�T�C�Y�g��
        returnSize.y += UI_DISCONNECT_EXTEND_RATE_Y;

        // ���T�C�Y�̐���
        if (returnSize.x >= UI_DISCONNECT_MAX_SIZE_X)
        {
            returnSize.x = UI_DISCONNECT_MAX_SIZE_X;
        }

        // �c�T�C�Y�̐���
        if (returnSize.y >= UI_DISCONNECT_MAX_SIZE_Y)
        {
            returnSize.y = UI_DISCONNECT_MAX_SIZE_Y;
        }
    }
    else
    {
        // ���T�C�Y�g��
        returnSize.x += UI_YOUR_TURN_EXTEND_RATE_X;

        // �c�T�C�Y�g��
        returnSize.y += UI_YOUR_TURN_EXTEND_RATE_Y;

        // ���T�C�Y�̐���
        if (returnSize.x >= UI_YOUR_TURN_MAX_SIZE_X)
        {
            returnSize.x = UI_YOUR_TURN_MAX_SIZE_X;
        }

        // �c�T�C�Y�̐���
        if (returnSize.y >= UI_YOUR_TURN_MAX_SIZE_Y)
        {
            returnSize.y = UI_YOUR_TURN_MAX_SIZE_Y;
        }
    }

    // �^�[���\���́A�e���̃^�[�����I����������
    switch (m_type)
    {
    case TYPE_GAME_START:

        // �A�j���[�V�����̃J�E���^�𗘗p���āAUI�̃t�F�[�h�A�E�g�ɂȂ���
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // �t�F�[�h�A�E�g���n�߂�J�E���g�𒴂�����A���l�������Ă���
        if (nCnt >= UI_YOUR_TURN_FADE_OUT_COUNT)
        {
            m_col.a -= UI_YOUR_TURN_FADE_OUT_RATE;
        }

        break;

    case TYPE_ATTACK_START:

        // �U���^�[���łȂ��Ȃ�A�����ɂ��Ă���
        if (CGame::GetAttackTurn() == false)
        {
            m_col.a -= UI_YOUR_TURN_FADE_OUT_RATE;
        }

        // �A�j���[�V�����p�[�g�̐ؒf�Ή�
        DisconnectInAnim();

        break;

    case TYPE_DEFENCE_START:

        // �h��^�[���o�Ȃ��Ȃ�A���񂾂񓧖��ɂ��Ă���
        if (CGame::GetDefenceTurn() == false)
        {
            m_col.a -= UI_YOUR_TURN_FADE_OUT_RATE;
        }

        // �A�j���[�V�����p�[�g�̐ؒf�Ή�
        DisconnectInAnim();

        break;
    }

    // �����x��0�����������A����
    if (m_col.a <= 0.0f)
    {
        m_bUse = false;
    }

    return returnSize;
}

//================================
// �A�j���[�V�����p�[�g�ł́A�ؒf�Ή�
//================================
void CUI::DisconnectInAnim(void)
{
    // �A�j���[�V�����p�[�g�̐ؒf�Ή�
    if (CManager::GetEnemyDisconnect() == true && CGame::GetState() == CGame::STATE_PLAYER_ATTACK ||
        CManager::GetEnemyDisconnect() == true && CGame::GetState() == CGame::STATE_ENEMY_ATTACK)
    {
        m_col.a = 0.0f;
    }
}
