//=============================================================================
//
// �ŋ��̘b�����̏���[player4.h]�i���@4�j
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _PLAYER4_H_
#define _PLAYER4_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
//�v���C���[�̏����ʒu
#define PLAYER4_FIRST_POS_X (1300.0f)
#define PLAYER4_FIRST_POS_Z (100.0f)

//=============================================================================
//�\���̒�`
//=============================================================================

// �A�j���[�V�����L�[�Z�b�g���
typedef struct
{
    D3DXVECTOR3 pos[MAX_PARTS];
    D3DXVECTOR3 rot[MAX_PARTS];

    int nFrame;     // 1���[�V�����̃L�[�t���[����
}KEY_INFO_PLAYER4;

// �A�j���[�V�������
typedef struct
{
    bool nLoop;		// ���[�v���邩
    int nNum_Key;			// ���[�V�����̃L�[��
    KEY_INFO_PLAYER4 KeyInfo[MOTION_KEYSET_MAX];		// �A�j���[�V�����Ɋ܂ރL�[�Z�b�g���
}ANIMATION_PLAYER4;

//���[�V�����̏��
typedef enum
{
    MOTIONSTATE_PLAYER4_IDLE = 0,		// �ҋ@���[�V����
    MOTIONSTATE_PLAYER4_WALK,			// �������[�V����
    MOTIONSTATE_PLAYER4_REGRET,		    // �������胂�[�V����
    MOTIONSTATE_PLAYER4_PLEASURE,		// ��у��[�V����
    MOTIONSTATE_PLAYER4_,			    // 
    MOTIONSTATE_PLAYER4_MAX
}MOTIONSTATE_PLAYER4;

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
}MODEL_PLAYER4;

// ���f���p�[�c�̍\����
typedef struct
{
    char *pFileName;
    int nData;
    int nParents;			// ���f���i���o�[
    D3DXVECTOR3 pos;			// �ʒu
    D3DXVECTOR3 posOrigin;		// �n�܂�̈ʒu
    D3DXVECTOR3 rot;			// �p�x
}ModelParts_Player4;

//�v���C���[�̏��
typedef struct
{
    D3DXVECTOR3 pos;	            //���݂̈ʒu
    D3DXVECTOR3 posOld;	            //1F�O�̈ʒu
    D3DXVECTOR3 rot;	            //����
    D3DXVECTOR3 posGrid;            //�O���b�h�̒��S�̒l�𓾂�
    D3DXMATRIX mtxWorld;			// �s��v�Z�p
    D3DXVECTOR3 move;	            //�ړ�
    MOTIONSTATE_PLAYER4 MotionState;		//���[�V�����̏��
    MODEL_PLAYER4 modelParts[MAX_PARTS];	//�v���C���[���\���֎q�p�[�c�Q
    float fLife;		            //���C�t
    float fSpeed;                   //����
    int nSpeedRank;                 //���������N
    int nFire;                      //�Η�
    int nBomb;                      //�{���̐ݒu�\��
    int nMaxBomb;                   //�{���ݒu�̍ő吔
    int nCanKick;                   //1�ȏ�ŃL�b�N�\
    bool bDisp;			            //�\���ؑ�
    int nShadow;                    //�v���C���[�̉e�̔ԍ�
    int nCntInvincibleFrame;        //���G����
    bool bTransparence;             //�������ǂ���
}PLAYER4;

//=====================================================
// �v���g�^�C�v�錾
//=====================================================
void InitPlayer4(void);
void UninitPlayer4(void);
void UpdatePlayer4(void);
void DrawPlayer4(void);
PLAYER4*GetPlayer4(void);
void HitPlayer4(float fDamage);
bool CollisionPlayer4(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

void UpdatePlayer4Motion(void);
void LoadPlayer4Animation(void);
void LoadPlayer4(void);
void InitDispPlayer4(void);
#endif