//=============================================================================
//
// �ŋ��̘b�����̏��� [boss.h](�X�g�[���[���[�h�̃{�X)
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
//�{�X�̊�{���
#define MAX_BOSS_PARTS (10)					// �p�[�c�̐�
#define BOSS_SIZE (D3DXVECTOR3(GRID_SIZE * 3,GRID_SIZE * 2,GRID_SIZE * 3))    //�{�X�̃T�C�Y
#define BOSS_LIFE (1200.0f)		                         //�{�X�̗̑�
#define BOSS_ANGER_LIFE (BOSS_LIFE / 2)                  //�{�X�̓{��ڍs�̗�
#define BOSS_EXANGER_LIFE (BOSS_LIFE / 4)                //�{�X���V�ڍs�̗�
#define BOSS_HEIGHT (50.0f)                             //�{�X�̍���

//�{�X�̏����ʒu���
#define BOSS_FIRST_POS_X 5500.0f                         //�{�X�̏���X���W�ʒu
#define BOSS_FIRST_POS D3DXVECTOR3(BOSS_FIRST_POS_X,0.0f,600.0f)  //�{�X�̏����ʒu

//�����ɂ���āA�s����ς���
#define BOSS_CONSIDER_SHORT (550.0f)    //�ߋ����Ƃ݂Ȃ�
#define BOSS_CONSIDER_MEDIUM (750.0f)   //�������Ƃ݂Ȃ�

//=====================
//�{�X�̊e�s���̎���
//=====================
//�����擾�ҋ@
#define NORMAL_STAY_TIME (120)      //�����擾�ҋ@����
#define ANGER_STAY_TIME (95)        //�{�莞�̑ҋ@����
#define EXANGER_STAY_TIME (70)      //���V���̑ҋ@����
#define NORMAL_ROT_SPEED (0.05f)    //�ʏ펞�̐U������X�s�[�h
#define ANGER_ROT_SPEED (0.075f)    //�{�莞�̐U������X�s�[�h
#define EXANGER_ROT_SPEED (0.1f)    //���V���̐U������X�s�[�h

//�^�b�N��
#define ATTACK_TIME_TACKLE (90)     //�^�b�N���̍U���t���[��
#define CHANCE_TIME_TACKLE (55)     //�^�b�N���̌��t���[��
#define NORMAL_TACKLE_SPEED (7.0f)  //�ʏ펞�̃^�b�N�����x
#define ANGER_TACKLE_SPEED (8.0f)   //�{�莞�̃^�b�N�����x
#define EXANGER_TACKLE_SPEED (9.0f) //���V���̃^�b�N�����x

//�W�����v
#define ATTACK_TIME_JUMP (150)          //�W�����v�̍U���t���[��
#define CHANCE_TIME_JUMP (15)           //�W�����v�̌��t���[��
#define VALUE_BOSS_JUMP (35.0f)         //�W�����v�̋���
#define RANGE_STUN_XZ (500.0f)          //�W�����v�̉��U���͈�
#define ANGER_RANGE_STUN_XZ (550.0f)    //�{�莞�̃W�����v�̉��U���͈�
#define RANGE_STUN_Y (120.0f)           //�W�����v�̏c�U���͈�

//�u���X
#define ATTACK_TIME_BREATH (52) //�u���X�̍U���t���[��
#define CHANCE_TIME_BREATH (25) //�u���X�̌��t���[��
#define BREATH_MOMENT (20)      //�ˌ�����u�Ԃ̃t���[��

//����/�X�^��
#define BOSS_REACH_FLINCH (180)   //�{�X�̋���/�X�^���l
#define FLINCH_TIME (90)          //���ݎ���
#define STUN_TIME (300)           //�X�^������
#define FLINCH_MOVE (3.5f)        //���ݎ��̈ړ���

//=============================================================================
//�\���̒�`
//=============================================================================

// �A�j���[�V�����L�[�Z�b�g���
typedef struct
{
    D3DXVECTOR3 pos[MAX_BOSS_PARTS];
    D3DXVECTOR3 rot[MAX_BOSS_PARTS];
    int nFrame;     // 1���[�V�����̃L�[�t���[����
}KEY_INFO_BOSS;

// �A�j���[�V�������
typedef struct
{
    bool nLoop;		// ���[�v���邩
    int nNum_Key;			// ���[�V�����̃L�[��
    KEY_INFO_BOSS KeyInfo[MOTION_KEYSET_MAX];		// �A�j���[�V�����Ɋ܂ރL�[�Z�b�g���
}ANIMATION_BOSS;

//���[�V�����̏��
typedef enum
{
    MOTION_IDLE_BOSS = 0,		// �ҋ@���[�V����
    MOTION_WALK_BOSS,			// �������[�V����
    MOTION_ROAR_BOSS,	        // ���K���[�V����
    MOTION_JUMP_BOSS,			// �W�����v���[�V����
    MOTION_BREATH_BOSS,			// �u���X���[�V����
    MOTION_FLINCH_BOSS,         // �Ђ�݃��[�V����
    MOTION_STUN_BOSS,           // �X�^�����[�V����
    MOTION_MAX_BOSS
}MOTIONSTATE_BOSS;

// ���f���̍\����
typedef struct
{
    LPD3DXMESH pMesh;				// ���b�V�����ւ̃|�C���^
    LPD3DXBUFFER pBuffMat;			// �}�e���A�����ւ̃|�C���^
    DWORD nNumMat;					// �}�e���A�����̐�
    D3DXVECTOR3 pos;				// �ʒu
    D3DXVECTOR3 rot;				// ��]
    D3DXVECTOR3 size;
    D3DXVECTOR3 AngleAddition;		// �p�x�̉��Z
    D3DXMATRIX mtxWorld;			// �s��v�Z�p
    int nldxModelParent;			// �e���f���̃C���f�b�N�X
    bool bUse;
}MODEL_BOSS;

// ���f���p�[�c�̍\����
typedef struct
{
    char *pFileName;
    int nData;
    int nParents;			// ���f���i���o�[
    D3DXVECTOR3 pos;			// �ʒu
    D3DXVECTOR3 posOrigin;		// �n�܂�̈ʒu
    D3DXVECTOR3 rot;			// �p�x
}ModelParts_BOSS;

//�{�X�̏��
typedef enum
{
    BOSSSTATE_STAY = 0,	    //�����擾�ҋ@
    BOSSSTATE_ATTACK_TACKLE,//�ːi�U����
    BOSSSTATE_ATTACK_JUMP,  //�W�����v�U����
    BOSSSTATE_ATTACK_BREATH,//�u���X�U����
    BOSSSTATE_CHANCE,	    //�����Œ�ҋ@
    BOSSSTATE_MAX			//��Ԃ̍ő吔
}BOSSSTATE;

//�{�X�̏��
typedef struct
{
    D3DXVECTOR3 pos;	            //���݂̈ʒu
    D3DXVECTOR3 posOld;	            //1F�O�̈ʒu
    D3DXVECTOR3 size;               //�{�X�̑傫��
    D3DXVECTOR3 rot;	            //����
    D3DXVECTOR3 rotDest;	        //����(�ړI�l)
    D3DXVECTOR3 rotMin;             //������ς���Ƃ��ɁA�ŒZ�ŐU��������ߎg�p
    D3DXMATRIX mtxWorld;			//�s��v�Z�p
    D3DXVECTOR3 move;	            //�ړ�
    BOSSSTATE state;	            //���
    MOTIONSTATE_BOSS MotionState;	//���[�V�����̏��
    MODEL_BOSS modelParts[MAX_BOSS_PARTS];	//�v���C���[���\���֎q�p�[�c�Q
    float fLife;		            //���C�t
    int nCounterRigid;              //���݂�X�^���܂ł̃J�E���g
    int nStayTime;                  //�����擾�ҋ@����
    int nAttackTime;	            //�U������
    int nChanceTime;                //�����Œ�ҋ@����
    int nCntFlinch;                 //���݉񐔂��J�E���g
    int nStunTime;                  //�X�^������
    bool bStun;                     //�_�E�����Ă��邩�ǂ���
    bool bUse;			            //�g�p���Ă��邩�ǂ���
    float fDistance;    //���@�Ƃ̋���
    int nShadow;                    //�e�̔ԍ�
    int nCntInvincibleFrame;        //���G����
    bool bTransparence;             //�������ǂ���
}BOSS;

//=====================================================
// �v���g�^�C�v�錾
//=====================================================
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
BOSS*GetBoss(void);
void HitBoss(int nCntFlinch, float fDamage);
bool CollisionBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, bool bPlayer);

void UpdateBossMotion(void);
void LoadBossAnimation(void);
void LoadBoss(void);
#endif