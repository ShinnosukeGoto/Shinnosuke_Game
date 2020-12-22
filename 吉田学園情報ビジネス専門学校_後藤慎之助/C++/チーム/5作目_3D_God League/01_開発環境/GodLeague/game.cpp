//=============================================================================
//
// �Q�[������ [game.cpp]
// Author : �㓡�T�V��
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "cursor.h"
#include "card.h"
#include "life.h"
#include "character.h"
#include "card_info.h"
#include "card_name.h"
#include "sky.h"
#include "ui3d.h"
#include "animation.h"
#include "camera.h"
#include "fade.h"
#include "manual.h"
#include "button.h"
#include "field.h"

#include "input.h"

//========================================
// �}�N����`
//========================================

// �X�^�[�g�ŃQ�[����Ԃ̊Ǘ�
#define START_ATTACK 210    // ��U�̍U���J�n

// �A�j���[�V�����ŃQ�[����Ԃ̊Ǘ�
#define ANIM_SET_CAMERA 2   // �J�����Z�b�g
#define ANIM_START 60       // �A�j���[�V�����X�^�[�g
#define ANIM_DAMAGE 150     // �_���[�W�v�Z�A���o
#define ANIM_END 240        // �A�j���[�V�����I��

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CLife *CGame::m_pLife = NULL;
CCardInfo *CGame::m_pCardInfo = NULL;
CCharacter *CGame::m_apCharacter[MAX_CHARACTER] = {};
bool CGame::m_bAttackTurn = false;
bool CGame::m_bDefenceTurn = false;

int CGame::m_nEnemyUseCard = 0;
int CGame::m_nPlayerUseCard = 0;

bool CGame::m_bFinishBattle = false;

bool CGame::m_bUpdateEnemyUseCardInfo = false;
bool CGame::m_bUpdateAttackTurn = false;
bool CGame::m_bUpdateDefenceTurn = false;

CGame::STATE CGame::m_state = STATE_NORMAL;

//=============================================================================
// �Q�[���ŒʐM�ɂ��������̂̏�����
//=============================================================================
void CGame::ResetGame(void)
{
    m_bFinishBattle = false;

    m_bUpdateAttackTurn = false;
    m_bUpdateDefenceTurn = false;

    m_state = STATE_START;
}

//=============================================================================
// �Q�[���̃R���X�g���N�^
//=============================================================================
CGame::CGame()
{
    m_nCntGameTime = 0;
    m_nCntFinishGame = 0;

    m_pLife = NULL;
    m_pCardInfo = NULL;
    memset(m_apCharacter, 0, sizeof(m_apCharacter));

    // ���݂��A�U���^�[���ł��h��^�[���ł��Ȃ�
    m_bAttackTurn = false;
    m_bDefenceTurn = false;

    // �ÓI�����o�ϐ���������
    m_nEnemyUseCard = 0;
    m_nPlayerUseCard = 0;
    m_bFinishBattle = false;
    m_bUpdateEnemyUseCardInfo = false;
    m_bUpdateAttackTurn = false;
    m_bUpdateDefenceTurn = false;
    m_state = STATE_START;
}

//=============================================================================
// �Q�[���̃f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGame::Init(void)
{
    // ��ʉ��̔w�i�𐶐�
    CUI::Create(GAME_UNDER_POS, GAME_UNDER_SIZE, DEFAULT_COLOR, CUI::TYPE_GAME_UNDER);

    // UI�𐶐�
    CUI::Create(UI_HP_PLAYER_POS, UI_HP_PLAYER_SIZE, DEFAULT_COLOR, CUI::TYPE_HP);
    CUI::Create(UI_HP_ENEMY_POS, UI_HP_ENEMY_SIZE, DEFAULT_COLOR, CUI::TYPE_HP);
    CUI::Create(UI_YOU_POS, UI_YOU_SIZE, DEFAULT_COLOR, CUI::TYPE_YOU);
    CUI::Create(UI_ENEMY_POS, UI_ENEMY_SIZE, DEFAULT_COLOR, CUI::TYPE_ENEMY);
    CUI::Create(UI_CARD_INFO_POS, UI_CARD_INFO_SIZE, DEFAULT_COLOR, CUI::TYPE_CARD_INFO_BG);
    CUI::Create(UI_MANUAL_POS, UI_MANUAL_SIZE, DEFAULT_COLOR, CUI::TYPE_MANUAL);
    CUI::Create(UI_WAIT_POS, UI_WAIT_SIZE, DEFAULT_COLOR, CUI::TYPE_WAIT);
    CUI::Create(UI_NOW_LOADING_IN_GAME_POS, UI_NOW_LOADING_SIZE, DEFAULT_COLOR, CUI::TYPE_TITLE_UI04);

    // �o�g���X�^�[�g�\��
    CUI::Create(UI_YOUR_TURN_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_GAME_START);

    // �V�ѕ�����
    CManual::Create(MANUAL_POS, MANUAL_SIZE);

    // ���C�t�𐶐�
    m_pLife = CLife::Create(DEFAULT_VECTOR, LIFE_SIZE);
    m_pLife->AddPlayerLife(0);  // �v���C���[�̃��C�t�\���̏�����
    m_pLife->AddEnemyLife(0);   // �G�̃��C�t�\���̏�����

    // �J�[�\���𐶐�
    CCursor::Create(CURSOR_POS, CURSOR_SIZE);

    // �J�[�h�������_���ɐ���
    CCard::DrawCard(FIRST_CARD);

    // �J�[�h���𐶐�
    m_pCardInfo = CCardInfo::Create(DEFAULT_VECTOR, CARD_INFO_NUMBER_SIZE);
    m_pCardInfo->SetCardInfo(0, 0);                                     // �����J�[�h��I�����Ă��Ȃ��̂ŁA�U�����h���0
    m_pCardInfo->SetEnemyCardInfo(CARD_INFO_ENEMY_TRANSPARENT);         // -1�Ő����𓧖���

    // �J�[�h�̒��ɁA�����h����邩�ǂ����`�F�b�N
    CCard::CheckDispAttackCard();
    CCard::CheckDispDefenceCard();

    // �J�[�h���𐶐�
    CCardName::Create(CARD_NAME_POS, CARD_NAME_SIZE, CCard::TYPE_NONE, CCardName::USETYPE_PLAYER); // �����J�[�h��I�����Ă��Ȃ��̂ŁA�u�Ȃ��v

    // �{�^���̐���
    CButton::Create(BUTTON_FORGIVE_POS, BUTTON_SIZE, DEFAULT_COLOR, CButton::TYPE_FORGIVE);
    CButton::Create(BUTTON_PRAY_POS, BUTTON_SIZE, DEFAULT_COLOR, CButton::TYPE_PRAY);

    // �L�����N�^�[�̐���
    m_apCharacter[0] = CCharacter::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(90), 0.0f));
    m_apCharacter[1] = CCharacter::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(270), 0.0f));

    // �J�����̃��b�N�I���ꏊ
    CCamera::SetCameraLookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

    // �r���{�[�h�J�[�h�̐���
	CUi3D::Create(D3DXVECTOR3(100.0f, 60.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0);
	CUi3D::Create(D3DXVECTOR3(-100.0f, 60.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1);

    // �X�J�C�{�b�N�X�̐���
    CSky::Create();

	// �t�B�[���h�̐���
	CField::Create();

    // �J�[�h����u�ŕ��בւ��Ă���
    CCard::SortCards();

    // BGM�A�������Đ�
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_BGM_GAME);
    pSound->Play(CSound::LABEL_SE_005);

    return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGame::Uninit(void)
{
    // BGM�A�������~
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::LABEL_BGM_GAME);
    pSound->Stop(CSound::LABEL_SE_005);

    // �}�l�[�W���[�̐ÓI�����o�ϐ���S�ď�����
    CManager::ResetAll();
}

//=============================================================================
// �X�V����
//=============================================================================
void CGame::Update(void)
{
    // �J�[�h�̓��I�ȕ��ёւ�
    CCard::SortMoveCards();

    // ���s����
    Judgment();

    // �G���g�p�����J�[�h�����X�V
    SetEnemyUseCardInfo();

    // �^�[���\�����X�V
    SetYourTurn();

    // �Q�[����Ԃ̊Ǘ�
    ManageState();
}

//=============================================================================
// ���s����
//=============================================================================
void CGame::Judgment(void)
{
    // �ϐ��錾
    int nPlayerLife = 0;    // ������HP
    int nEnemyLife = 0;     // �����HP

    // �e���C�t���擾
    nPlayerLife = m_pLife->GetPlayerLife();
    nEnemyLife = m_pLife->GetEnemyLife();

    // ��������
    if (nPlayerLife <= 0)
    {
        // ���s�����ĂȂ��Ȃ�
        if (m_bFinishBattle == false)
        {
            // �S���O����
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_007);

            // ����
            CUI::Create(UI_JUDGMENT_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_YOU_LOSE);

            // �����������[�V����
            m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_LOSE);

            // ���菟�����[�V����
            m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_ENEMY_WIN);

            // ���s������
            m_bFinishBattle = true;

            // �^�C�g���ɍs���܂ł̎��Ԃ�ݒ�
            m_nCntFinishGame = COUNT_WAIT_FINISH;
        }
    }

    // ��������
    if (nEnemyLife <= 0)
    {
        // ���s�����ĂȂ��Ȃ�
        if (m_bFinishBattle == false)
        {
            // �S���O����
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_007);

            // ����
            CUI::Create(UI_JUDGMENT_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_YOU_WIN);

            // �����������[�V����
            m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_PLAYER_WIN);

            // ���蕉�����[�V����
            m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_LOSE);

            // ���s������
            m_bFinishBattle = true;

            // �^�C�g���ɍs���܂ł̎��Ԃ�ݒ�
            m_nCntFinishGame = COUNT_WAIT_FINISH;
        }
    }

    // ���U���g�ɍs���܂ł̃J�E���^
    if (m_nCntFinishGame > 0)
    {
        m_nCntFinishGame--;

        // 0�ɂȂ�������
        if (m_nCntFinishGame == 0)
        {
            // �t�F�[�h���Ă��Ȃ��Ȃ�
            if (CFade::GetFade() == CFade::FADE_NONE)
            {
                // �^�C�g���Ɉڍs
                CFade::SetFade(CManager::MODE_TITLE);
            }
        }
    }
}

//=============================================================================
// �v���C���[�̍U������
//=============================================================================
void CGame::AttackPlayerToEnemy(void)
{
    // �G�̃_���[�W�v�Z
    m_pLife->EnemyDamageCalculation();
}

//=============================================================================
// �G�l�~�[�̍U������
//=============================================================================
void CGame::AttackEnemyToPlayer(void)
{
    // �v���C���[�̃_���[�W�v�Z
    m_pLife->PlayerDamageCalculation();
}

//=============================================================================
// �G�l�~�[�̃J�[�h���ݒ菈��
//=============================================================================
void CGame::SetEnemyUseCardInfo(void)
{
    // �����X�V�̃t���O��true�Ȃ�
    if (m_bUpdateEnemyUseCardInfo == true)
    {
        // ������G�̃J�[�h��������
        for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
        {
            // �V�[�����擾
            CScene *pScene = CScene::GetScene(nCntScene);

            // ���g������Ȃ�
            if (pScene != NULL)
            {
                // �^�C�v���擾
                CScene::OBJTYPE objType = pScene->GetObjType();

                // �J�[�h���Ȃ�
                if (objType == CScene::OBJTYPE_CARD_NAME)
                {
                    // �J�[�h���ɃL���X�g
                    CCardName *pCardName = (CCardName*)pScene;

                    // �G�̃J�[�h�Ȃ�
                    if (pCardName->GetUseType() == CCardName::USETYPE_ENEMY)
                    {
                        // �Ώۂ𖢎g�p��
                        pCardName->SetDontUse();
                    }
                }
                // UI�Ȃ�
                else if (objType == CScene::OBJTYPE_UI)
                {
                    // UI�ɃL���X�g
                    CUI *pUI = (CUI*)pScene;

                    // �G�̎g�p�J�[�hUI�Ȃ�
                    if (pUI->GetType() == CUI::TYPE_ENEMY_USE_ATTACK || pUI->GetType() == CUI::TYPE_ENEMY_USE_DEFENCE)
                    {
                        // �Ώۂ𖢎g�p��
                        pUI->SetDontUse();
                    }
                }
            }
        }

        // ���肪�ؒf���Ă��Ȃ��Ȃ�
        if (CManager::GetEnemyDisconnect() == false)
        {
            // �G�̎g�p�����J�[�h�����擾
            CCard::STATUS status;
            status = CCard::List(m_nEnemyUseCard);

            // �U���͂�����Ȃ�A�܂��͋F��Ȃ�
            if (status.nAttack > 0 || m_nEnemyUseCard == PRAY_ID)
            {
                // �U���p��UI
                CUI::Create(UI_ENEMY_USE_POS, UI_ENEMY_USE_SIZE, DEFAULT_COLOR, CUI::TYPE_ENEMY_USE_ATTACK);

                // �U���͐ݒ�
                m_pCardInfo->SetEnemyCardInfo(status.nAttack);
            }

            // �h��͂�����Ȃ�A�܂��͋����Ȃ�
            if (status.nDefence > 0 || m_nEnemyUseCard == FORGIVE_ID)
            {
                // �h��p��UI
                CUI::Create(UI_ENEMY_USE_POS, UI_ENEMY_USE_SIZE, DEFAULT_COLOR, CUI::TYPE_ENEMY_USE_DEFENCE);

                // �h��͐ݒ�
                m_pCardInfo->SetEnemyCardInfo(status.nDefence);
            }

            // �J�[�h���𐶐�
            CCardName::Create(CARD_NAME_ENEMY_POS, CARD_NAME_SIZE, m_nEnemyUseCard, CCardName::USETYPE_ENEMY);
        }
        // ���肪�ؒf�����Ȃ�A����̃J�[�h���̐������\����
        else
        {
            m_pCardInfo->SetEnemyCardInfo(CARD_INFO_ENEMY_TRANSPARENT);         // -1�Ő����𓧖���
        }

        // �t���O�����Z�b�g
        m_bUpdateEnemyUseCardInfo = false;
    }
}

//=============================================================================
// �^�[���\����ݒ�
//=============================================================================
void CGame::SetYourTurn(void)
{
    // �U���^�[���\���̃t���O��true�Ȃ�
    if (m_bUpdateAttackTurn == true)
    {
        // 2��ʂ�Ȃ��悤false��
        m_bUpdateAttackTurn = false;

        // �U���^�[���\��
        CUI::Create(UI_YOUR_TURN_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_ATTACK_START);
    }

    // �h��^�[���\���̃t���O��true�Ȃ�
    if (m_bUpdateDefenceTurn == true)
    {
        // 2��ʂ�Ȃ��悤false��
        m_bUpdateDefenceTurn = false;

        // �h��^�[���\��
        CUI::Create(UI_YOUR_TURN_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_DEFENCE_START);
    }
}

//=============================================================================
// �G���g�p�����J�[�h��ݒ�
//=============================================================================
void CGame::SetEnemyUseCard(int nCard)
{
    // �G���g�p�����J�[�h�����т���
    m_nEnemyUseCard = nCard;

    // �G���g�p�����J�[�h���̍X�V�̃t���O��true��
    m_bUpdateEnemyUseCardInfo = true;
}

//=============================================================================
// �v���C���[���g�p�����J�[�h��ݒ�
//=============================================================================
void CGame::SetPlayerUseCard(int nCard)
{
    m_nPlayerUseCard = nCard;
}

//=============================================================================
// �U���^�[�����ǂ�����ݒ�
//=============================================================================
void CGame::SetAttackTurn(bool bAttack)
{
    // �^�[�������т���
    m_bAttackTurn = bAttack;

    // true�Ȃ�AUI�𐶐��̃t���O��true��
    if (bAttack == true)
    {
        m_bUpdateAttackTurn = true;
    }
}

//=============================================================================
// �h��^�[�����ǂ�����ݒ�
//=============================================================================
void CGame::SetDefenceTurn(bool bDefence)
{
    // �^�[�������т���
    m_bDefenceTurn = bDefence;

    // true�Ȃ�AUI�𐶐��̃t���O��true��
    if (bDefence == true)
    {
        m_bUpdateDefenceTurn = true;
    }
}

//=============================================================================
// �Q�[����Ԃ̊Ǘ�
//=============================================================================
void CGame::ManageState(void)
{
    // �����擾
    CSound *pSound = CManager::GetSound();

    switch (m_state)
    {
    case STATE_START:

        // �J�E���^�����Z
        m_nCntGameTime++;

        if (m_nCntGameTime == START_ATTACK)
        {
            // ��U�Ȃ�
            if (CManager::GetAttackOrder() == 1)
            {
                // �U���^�[����
                m_bAttackTurn = true;

                // �U���^�[����UI�𐶐�
                CUI::Create(UI_YOUR_TURN_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_ATTACK_START);
            }

            // �J�E���^���Z�b�g
            m_nCntGameTime = 0;

            // ��ԃ��Z�b�g
            m_state = STATE_NORMAL;
        }

        break;

    case STATE_NORMAL:

        // �ؒf����
        DisconnectWin();

        break;

    case STATE_PLAYER_ATTACK:

        // �J�E���^�����Z
        m_nCntGameTime++;

        switch (m_nCntGameTime)
        {
        case ANIM_SET_CAMERA:

            // �S���O���
            pSound->Play(CSound::LABEL_SE_006);

            // �o�g���p�ɐݒ�
            CCamera::SetCameraState(CCamera::CAMERA_BATTLE);

            break;

        case ANIM_START:

            // �F��Ȃ�
            if (m_nPlayerUseCard == PRAY_ID)
            {
                // �F�郂�[�V����
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_PRAY);
            }
            // ���ʂ̍U���Ȃ�
            else
            {
                // �U�����[�V����
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_ATTACK);
            }

            // �����Ȃ�
            if (m_nEnemyUseCard == FORGIVE_ID)
            {
                // �������[�V����
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_PRAY);
            }
            else
            {
                // �h�䃂�[�V����
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_DIFENCE);
            }

            break;

        case ANIM_DAMAGE:

            // �G�ւ̃_���[�W
            CGame::AttackPlayerToEnemy();

            break;

        case ANIM_END:

            // ���s�����ĂȂ��Ȃ�
            if (m_bFinishBattle == false)
            {
                // �F��Ȃ�
                if (m_nPlayerUseCard == PRAY_ID)
                {
                    // �ҋ@�ɕς���
                    m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_IDLE);
                }

                // �ҋ@�ɕς���
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_IDLE);

                // �J�������I�[�o�[���b�N��
                CCamera::SetCameraState(CCamera::CAMERA_OVER_LOOK);

                // �J�E���^���Z�b�g
                m_nCntGameTime = 0;

                // ��ԃ��Z�b�g
                m_state = STATE_NORMAL;
            }

            break;
        }

        break;

    case STATE_ENEMY_ATTACK:

        // �J�E���^�����Z
        m_nCntGameTime++;

        switch (m_nCntGameTime)
        {
        case ANIM_SET_CAMERA:

            // �S���O���
            pSound->Play(CSound::LABEL_SE_006);

            // �o�g���p�ɐݒ�
            CCamera::SetCameraState(CCamera::CAMERA_BATTLE);

            break;

        case ANIM_START:

            // �F��Ȃ�
            if (m_nEnemyUseCard == PRAY_ID)
            {
                // �F�郂�[�V����
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_PRAY);
            }
            // ���ʂ̍U���Ȃ�
            else
            {
                // �U�����[�V����
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_ATTACK);
            }

            // �����Ȃ�
            if (m_nPlayerUseCard == FORGIVE_ID)
            {
                // �������[�V����
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_PRAY);
            }
            else
            {
                // �h�䃂�[�V����
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_DIFENCE);
            }

            break;

        case ANIM_DAMAGE:

            // �v���C���[�ւ̃_���[�W
            CGame::AttackEnemyToPlayer();

            break;

        case ANIM_END:

            // ���s�����ĂȂ��Ȃ�
            if (m_bFinishBattle == false)
            {
                // �F��Ȃ�
                if (m_nEnemyUseCard == PRAY_ID)
                {
                    // �ҋ@�ɕς���
                    m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_IDLE);
                }

                // �ҋ@�ɕς���
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_IDLE);

                // �J�������I�[�o�[���b�N��
                CCamera::SetCameraState(CCamera::CAMERA_OVER_LOOK);

                // �J�E���^���Z�b�g
                m_nCntGameTime = 0;

                // ��ԃ��Z�b�g
                m_state = STATE_NORMAL;

                // �v���C���[�̍U���^�[���̊J�n
                CGame::SetAttackTurn(true);
            }

            break;
        }

        break;
    }
}

//=============================================================================
// �ؒf��������
//=============================================================================
void CGame::DisconnectWin(void)
{
    // �G���ؒf�����Ȃ�
    if (CManager::GetEnemyDisconnect() == true)
    {
        // ���s�����ĂȂ��Ȃ�
        if (m_bFinishBattle == false)
        {
            // �S���O����
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_007);

            // �J�������o�g����
            CCamera::SetCameraState(CCamera::CAMERA_BATTLE);

            // �ؒf����
            CUI::Create(UI_DISCONNECT_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_DISCONNECT);

            // ����
            CUI::Create(UI_JUDGMENT_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_YOU_WIN);

            // �����A�C�e���g�p���[�V����
            m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_USE);

            // ���蕉�����[�V����
            m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_LOSE);

            // ���s������
            m_bFinishBattle = true;

            // �^�C�g���ɍs���܂ł̎��Ԃ�ݒ�
            m_nCntFinishGame = COUNT_WAIT_FINISH;
        }
    }
}
