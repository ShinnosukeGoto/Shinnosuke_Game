//=============================================================================
//
// �ŋ��̘b�����̏���[player.h]�i���@1�j
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
//�v���C���[�̏����ʒu
#define PLAYER_FIRST_POS_X (100.0f)
#define PLAYER_FIRST_POS_Z (1100.0f)

#define PLAYER_STUN_TIME 180 //�v���C���[�̃X�^������

//=============================================================================
//�\���̒�`
//=============================================================================

// �A�j���[�V�����L�[�Z�b�g���
typedef struct
{
    D3DXVECTOR3 pos[MAX_PARTS];
    D3DXVECTOR3 rot[MAX_PARTS];
    int nFrame;     // 1���[�V�����̃L�[�t���[����
}KEY_INFO_PLAYER;

// �A�j���[�V�������
typedef struct
{
    bool nLoop;		// ���[�v���邩
    int nNum_Key;			// ���[�V�����̃L�[��
    KEY_INFO_PLAYER KeyInfo[MOTION_KEYSET_MAX];		// �A�j���[�V�����Ɋ܂ރL�[�Z�b�g���
}ANIMATION_PLAYER;

//���[�V�����̏��
typedef enum
{
    MOTIONSTATE_PLAYER_IDLE = 0,		// �ҋ@���[�V����
    MOTIONSTATE_PLAYER_WALK,			// �������[�V����
    MOTIONSTATE_PLAYER_REGRET,		    // �������胂�[�V����
    MOTIONSTATE_PLAYER_PLEASURE,		// ��у��[�V����
    MOTIONSTATE_PLAYER_,			    // 
    MOTIONSTATE_PLAYER_MAX
}MOTIONSTATE_PLAYER;

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
}MODEL_PLAYER;

// ���f���p�[�c�̍\����
typedef struct
{
    char *pFileName;
    int nData;
    int nParents;			// ���f���i���o�[
    D3DXVECTOR3 pos;			// �ʒu
    D3DXVECTOR3 posOrigin;		// �n�܂�̈ʒu
    D3DXVECTOR3 rot;			// �p�x
}ModelParts_Player;

//�v���C���[�̏��
typedef struct
{
    D3DXVECTOR3 pos;	            //���݂̈ʒu
    D3DXVECTOR3 posOld;	            //1F�O�̈ʒu
    D3DXVECTOR3 rot;	            //����
    D3DXVECTOR3 posGrid;            //�O���b�h�̒��S�̒l�𓾂�
    D3DXMATRIX mtxWorld;			// �s��v�Z�p
    D3DXVECTOR3 move;	            //�ړ�
    MOTIONSTATE_PLAYER MotionState;		//���[�V�����̏��
    MODEL_PLAYER modelParts[MAX_PARTS];	//�v���C���[���\���֎q�p�[�c�Q
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
    bool bStun;                     //�X�^�����Ă��邩�ǂ���
}PLAYER;

//=====================================================
// �v���g�^�C�v�錾
//=====================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER*GetPlayer(void);
void HitPlayer(float fDamage);
bool CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

void UpdatePlayerMotion(void);
void LoadPlayerAnimation(void);
void LoadPlayer(void);
void InitDispPlayer(void);
#endif