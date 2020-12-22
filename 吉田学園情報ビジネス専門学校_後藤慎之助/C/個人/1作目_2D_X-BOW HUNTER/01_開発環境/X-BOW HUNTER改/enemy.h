//=============================================================================
//
// X-Bow Hunter [enemy.h]�̏��� (�U�R�G)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_ENEMY (128)			//�G�̍ő吔

#define ENEMY_SIZE_MOS (80)		//���X�L�[�g�̃T�C�Y
#define ENEMY_LIFE_MOS (100)	//���X�L�[�g�̗̑�

#define ENEMY_SIZE_MET (100)	//���^���o�[�h�̃T�C�Y
#define ENEMY_LIFE_MET (160)	//���^���o�[�h�̗̑�

#define ENEMY_SIZE_MET�U (150)	//���^���o�[�h����̃T�C�Y
#define ENEMY_LIFE_MET�U (280)	//���^���o�[�h����̗̑�

#define ENEMY_SIZE_OCT (100)	//�I�N�g�p�X�̃T�C�Y
#define ENEMY_LIFE_OCT (120)	//�I�N�g�p�X�̗̑�

#define ENEMY_SIZE_GHO (130)	//�S�[�X�g�̃T�C�Y
#define ENEMY_LIFE_GHO (200)	//�S�[�X�g�̗̑�

#define ENEMY_SIZE_GSO (100)	//�O�\�N���V�̃T�C�Y
#define ENEMY_LIFE_GSO (20)		//�O�\�N���V�̗̑�

#define ENEMY_STAGE1 (2)		//�X�e�[�W1�̓G�̑���
#define ENEMY_STAGE2 (2)		//�X�e�[�W2�̓G�̑���
#define ENEMY_STAGE3 (1)		//�X�e�[�W3�̓G�̑���
#define ENEMY_STAGE4 (1)		//�X�e�[�W4�̓G�̑���

//=============================================================================
//�\���̒�`
//=============================================================================
//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//�ʏ�
	ENEMYSTATE_PINCH,		//HP4����1�ȉ��Ńs���`
	ENEMYSTATE_DAMAGE,		//�_���[�W
	ENEMYSTATE__MAX			//��Ԃ̍ő吔
}ENEMYSTATE;

typedef enum
{
	ENEMYTYPE_000 = 0,		//���X�L�[�g
	ENEMYTYPE_001,			//���^���o�[�h
	ENEMYTYPE_002,			//���^���o�[�h����
	ENEMYTYPE_003,			//�I�N�g�p�X
	ENEMYTYPE_004,			//�S�[�X�g
	ENEMYTYPE_005,			//�O�\�N���V
	ENEMYTYPE_MAX			//��ނ̍ő吔
}ENEMYTYPE;//�G�̎��

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ�
	ENEMYSTATE state;	//���
	int nLife;			//���C�t
	int nCounterState;	//��ԃJ�E���^
	ENEMYTYPE type;		//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
}ENEMY;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type);
ENEMY*GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);

#endif