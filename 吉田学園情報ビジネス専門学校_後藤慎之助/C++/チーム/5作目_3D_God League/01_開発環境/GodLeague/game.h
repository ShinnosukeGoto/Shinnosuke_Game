//=============================================================================
//
// �Q�[������ [game.h]
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"
#include "mode.h"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CLife;
class CCardInfo;
class CCharacter;

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//�Q�[����Ԃ̊Ǘ��t���[��
#define COUNT_WAIT_FINISH 300	    // �Q�[���I�����ɁA�^�C�g����ʂ֍s���܂ł̎���
#define FIRST_CARD 7                // �ŏ��̎�D
#define MAX_CARD 9                  // �J�[�h�̍ő吔

#define PLAYER 0
#define ENEMY 1

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CGame : public CMode
{
public:

    // �Q�[���̏��
    typedef enum
    {
        STATE_START = 0,        // �J�n
        STATE_NORMAL,           // �ʏ�
        STATE_PLAYER_ATTACK,	// �v���C���[�̍U�����o
        STATE_ENEMY_ATTACK,	    // �G�l�~�[�̍U�����o
        STATE_MAX			    // ��Ԃ̍ő吔
    }STATE;

    static void ResetGame(void);                                            // �Q�[�������Z�b�g
    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Judgment(void);                                                    // ���s����
    static CLife *GetLife(void) { return m_pLife; }
    static CCardInfo *GetCardInfo(void) { return m_pCardInfo; }
    static bool GetAttackTurn(void) { return m_bAttackTurn; }
    static void SetAttackTurn(bool bAttack);
    static bool GetDefenceTurn(void) { return m_bDefenceTurn; }
    static void SetDefenceTurn(bool bDefence);
    static void AttackPlayerToEnemy(void);
    static void AttackEnemyToPlayer(void);
    void SetEnemyUseCardInfo(void);                                         // �G���g�p�����J�[�h�������т���
    void SetYourTurn(void);

    static int GetEnemyUseCard(void) { return m_nEnemyUseCard; }            // �G���g�p�����J�[�h���擾
    static int GetPlayerUseCard(void) { return m_nPlayerUseCard; }          // �v���C���[���g�p�����J�[�h���擾
    static void SetEnemyUseCard(int nCard);                                 // �G���g�p�����J�[�h��ݒ�
    static void SetPlayerUseCard(int nCard);                                // �v���C���[���g�p�����J�[�h��ݒ�

    static bool GetFinishBattle(void) { return m_bFinishBattle; }           // �킢���I��������ǂ������擾

    void ManageState(void);                                                 // �Q�[����Ԃ̊Ǘ�
    static STATE GetState(void) { return m_state; }                         // �Q�[���̏�Ԃ��擾
    static void SetState(STATE state) { m_state = state; }                  // �Q�[���̏�Ԃ�ݒ�

    void DisconnectWin(void);                                               // �ؒf����

private:
    int m_nCntGameTime;                              // �Q�[�����Ԃ̃J�E���^
    int m_nCntFinishGame;                            // �Q�[�����I���܂ł̃J�E���^

    static CLife *m_pLife;                           // ���C�t�̃|�C���^
    static CCardInfo *m_pCardInfo;                   // �J�[�h���̃|�C���^
    static CCharacter *m_apCharacter[MAX_CHARACTER]; // �L�����N�^�[�̃|�C���^    
    static bool m_bAttackTurn;                       // �U���^�[�����ǂ���
    static bool m_bDefenceTurn;                      // �h��^�[�����ǂ���

    static int m_nEnemyUseCard;                      // �G���g�p�����J�[�h���
    static int m_nPlayerUseCard;                     // �v���C���[���g�p�����J�[�h

    static bool m_bFinishBattle;                     // �킢���I��������ǂ���

    static bool m_bUpdateEnemyUseCardInfo;           // �G���g�p�����J�[�h�����X�V���邩�ǂ���
    static bool m_bUpdateAttackTurn;                 // �A�^�b�N�^�[���\�����邩�ǂ���
    static bool m_bUpdateDefenceTurn;                // �f�B�t�F���X�^�[���\�����邩�ǂ���

    static STATE m_state;                            // ���
};

#endif