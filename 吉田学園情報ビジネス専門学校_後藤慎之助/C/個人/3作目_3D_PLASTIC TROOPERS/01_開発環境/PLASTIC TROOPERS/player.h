//=============================================================================
//
// 3D�Q�[������̏��� [player.h]
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_PARTS (12)					// �p�[�c�̐�
#define REACH_BOSS_Z (2020.0f)          //�{�X�̌��ɂ��ǂ蒅�������������Z���W�i2020.0f)

//=============================================================================
//�\���̒�`
//=============================================================================

// �A�j���[�V�����L�[�Z�b�g���
typedef struct
{
    D3DXVECTOR3 pos[MAX_PARTS];
    D3DXVECTOR3 rot[MAX_PARTS];

    int nFrame;     // 1���[�V�����̃L�[�t���[����
}KEY_INFO;

// �A�j���[�V�������
typedef struct
{
    bool nLoop;		// ���[�v���邩
    int nNum_Key;			// ���[�V�����̃L�[��
    KEY_INFO KeyInfo[MOTION_KEYSET_MAX];		// �A�j���[�V�����Ɋ܂ރL�[�Z�b�g���
}ANIMATION;

//���[�V�����̏��
typedef enum
{
    MOTION_IDLE = 0,		// �ҋ@���[�V����
    MOTION_WALK,			// �������[�V����
    MOTION_BULLET,			// �e�U�����[�V����
    MOTION_JUMP,			// �W�����v���[�V����
    MOTION_GUARD,			// �K�[�h���[�V����
    MOTION_SWORD1,			// ���U�����[�V����1
    MOTION_SWORD2,			// ���U�����[�V����1
    MOTION_SKYDASH,         //�󒆃_�b�V�����[�V����
    MOTION_MAX
}MOTIONSTATE;

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
}MODEL;

// ���f���p�[�c�̍\����
typedef struct
{
    char *pFileName;
    int nData;
    int nParents;			// ���f���i���o�[
    D3DXVECTOR3 pos;			// �ʒu
    D3DXVECTOR3 posOrigin;		// �n�܂�̈ʒu
    D3DXVECTOR3 rot;			// �p�x

}ModelParts;

//���@�̏��
typedef enum
{
    PLAYERSTATE_NORMAL = 0,	//�ʏ�
    PLAYERSTATE_DAMAGE,		//�_���[�W
    PLAYERSTATE_MAX			//��Ԃ̍ő吔
}PLAYERSTATE;

//�v���C���[�̏��
typedef struct
{
    D3DXVECTOR3 pos;	            //���݂̈ʒu
    D3DXVECTOR3 posOld;	            //1F�O�̈ʒu
    D3DXVECTOR3 rot;	            //����
    D3DXMATRIX mtxWorld;			// �s��v�Z�p
    D3DXVECTOR3 move;	            //�ړ�
    PLAYERSTATE state;	            //���
    MOTIONSTATE MotionState;		//���[�V�����̏��
    MODEL modelParts[MAX_PARTS];	//�v���C���[���\���֎q�p�[�c�Q
    float fLife;		            //���C�t
    float fEnergy;		            //�G�i�W�[
    float fOldEnergy;               //1F�O�̃G�i�W�[
    float fChanceMeter;             //�`�����X�Q�[�W
    int nChance;                    //�`�����X�l
    int nCntSkyDash;                //�󒆃_�b�V���̎���
    int nCounterState;	            //��ԃJ�E���^
    int nAttackTime;	            //�U������
    int nCntEXAttackCD;             //�K�E�Z�̃N�[���^�C��
    bool bJump;			            //�W�����v
    bool bSecondJump;	            //��i�ڂ̃W�����v
    bool bSkyDash;                  //�󒆃_�b�V��
    bool bDisp;			            //�\���ؑ�
    bool bLand;                     //�u���b�N�ɒ��n���Ă���
    bool bGuard;                    //�K�[�h���Ă���
    bool bChanceAttack;             //�K�E�U����
    bool bBullet;                   //�e������
                        //WSELECT Wtype;		//����I��
                        //MSELECT Mtype;		//���@�I��
}PLAYER;

//=====================================================
// �v���g�^�C�v�錾
//=====================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER*GetPlayer(void);
void HitPlayer(int nCntRigid, float fDamage);

void UpdatePlayerMotion(void);
void LoadAnimation(void);
void LoadPlayer(void);
#endif