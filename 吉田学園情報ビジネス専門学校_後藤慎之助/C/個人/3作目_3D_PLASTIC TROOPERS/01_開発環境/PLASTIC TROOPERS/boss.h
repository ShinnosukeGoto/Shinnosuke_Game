//=============================================================================
//
// 3D�Q�[������̏��� [boss.h]
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_BOSS_PARTS (14)					// �p�[�c�̐�

#define BOSS_SIZE (D3DXVECTOR3(150.0f,250.0f,120.0f))     //���C�o�[���̃T�C�Y
#define BOSS_LIFE (1000.0f)		                         //���C�o�[���̗̑�
#define BOSS_ANGER_LIFE (BOSS_LIFE / 2)                  //���C�o�[���̓{��ڍs�̗�
#define BOSS_EXANGER_LIFE (BOSS_LIFE / 4)                //���C�o�[�����V�ڍs�̗�
#define BOSS_HEIGHT (125.0f)                             //���C�o�[���̍���

#define BOSS_FIRST_POS_Z (3300.0f)  //�{�X�̏����ʒu

#define HIT_BOSS_DAMAGE (4.5f)  //�{�X�ɓ����������́A���@�ւ̃_���[�W

#define BOSS_CONSIDER_SHORT (450.0f)    //�ߋ����Ƃ݂Ȃ�
#define BOSS_CONSIDER_MEDIUM (650.0f)   //�������Ƃ݂Ȃ�

//=====================
//�{�X�̊e�s���̎���
//=====================
//�����擾�ҋ@
#define NORMAL_STAY_TIME (120) //�����擾�ҋ@����
#define ANGER_STAY_TIME (95)  //�{�莞�̑ҋ@����
#define EXANGER_STAY_TIME (70) //���V���̑ҋ@����
#define ANGER_ROT_SPEED (0.1f)//�{�莞�̐U������X�s�[�h
#define EXANGER_ROT_SPEED (0.15f)//���V���̐U������X�s�[�h
//�^�b�N��
#define ATTACK_TIME_TACKLE (90) //�^�b�N���̍U���t���[��
#define CHANCE_TIME_TACKLE (55) //�^�b�N���̌��t���[��
#define NORMAL_TACKLE_SPEED (10.0f)  //�ʏ펞�̃^�b�N�����x
#define ANGER_TACKLE_SPEED (9.0f)   //�{�莞�̒Ǐ]�^�b�N�����x
#define EXANGER_TACKLE_SPEED (11.0f)  //���V���̒Ǐ]�^�b�N�����x
//�X�s��
#define ATTACK_TIME_SPIN (70)   //�X�s���̍U���t���[��
#define CHANCE_TIME_SPIN (85)   //�X�s���̌��t���[��
#define DAMAGE_SPIN (20.0f)     //�X�s���̃_���[�W
#define RANGE_SPIN_XZ (400.0f)  //�X�s���̉��U���͈�
#define RANGE_SPIN_Y (120.0f)  //�X�s���̏c�U���͈�
#define CHARGE_SPIN (20)//�X�s���̃`���[�W����
#define KNOCKBACK_SPIN (80.0f)  //�X�s���̃m�b�N�o�b�N��
#define UPPER_SPIN (15.0f)      //�X�s����Y���㏸��
//�o���b�g
#define ATTACK_TIME_BULLET (52) //�o���b�g�̍U���t���[��
#define CHANCE_TIME_BULLET (25) //�o���b�g�̌��t���[��
#define BULLET_MOMENT (20)      //�ˌ�����u�Ԃ̃t���[��
#define BULLET_SMASH (20.0f)    //�o���b�g�̈ړ���Y���������āAXZ�����x��艺���āA����������
//����/�X�^��
#define BOSS_REACH_RIGID (100)   //�{�X�̋���/�X�^���l
#define FLINCH_TIME (90)    //���ݎ���
#define STUN_TIME (300) //�X�^������
#define FLINCH_MOVE (4.0f)  //���ݎ��̈ړ���

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
    MOTION_ATTACK_BULLET_BOSS,	// �e�U�����[�V����
    MOTION_STUN_BOSS,			// �X�^�����[�V����
    MOTION_FLINCH_BOSS,			// ���݃��[�V����
    MOTION_ATTACK_SPIN_BOSS,    // ��]�����ۍU��
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
    BOSSSTATE_ATTACK_SPIN,  //��]�����ۍU����
    BOSSSTATE_ATTACK_BULLET,//�e�U����
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
}BOSS;

//=====================================================
// �v���g�^�C�v�錾
//=====================================================
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
BOSS*GetBoss(void);
void HitBoss(int nCntRigid, float fDamage);
bool CollisionBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

void UpdateBossMotion(void);
void LoadBossAnimation(void);
void LoadBoss(void);
#endif