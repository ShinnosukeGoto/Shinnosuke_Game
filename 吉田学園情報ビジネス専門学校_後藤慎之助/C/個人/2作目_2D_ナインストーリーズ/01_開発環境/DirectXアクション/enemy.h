//=============================================================================
//
// �i�C���X�g�[���[�Y [enemy.h]�̏��� (�Q�[���ɏo�Ă���G)
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

#define ENEMY_SIZE_MOS (60)			//�r�[�g���̃T�C�Y
#define ENEMY_LIFE_MOS (40)			//�r�[�g���̗̑�

#define ENEMY_SIZE_MET (100)		//�J���X�̃T�C�Y
#define ENEMY_LIFE_MET (80)			//�J���X�̗̑�

#define ENEMY_SIZE_MET�U (100)		//�J���X����̃T�C�Y
#define ENEMY_LIFE_MET�U (40)		//�J���X����̗̑�

#define ENEMY_SIZE_OCT (100)		//�K�C�R�c�̃T�C�Y
#define ENEMY_LIFE_OCT_NORMAL (80)	//�K�C�R�c�̗̑�(�m�[�}��)
#define ENEMY_LIFE_OCT_HARD (94)	//�K�C�R�c�̗̑�(�n�[�h)
#define ENEMY_LIFE_DARK (160)		//�_�[�N�K�C�R�c�̗̑�

#define ENEMY_SIZE_GHO (120)		//�C�m�V�V�̃T�C�Y
#define ENEMY_LIFE_GHO (150)		//�C�m�V�V�̗̑�

#define ENEMY_SIZE_GSO (85)			//�R�E�����̃T�C�Y
#define ENEMY_LIFE_GSO (30)			//�R�E�����̗̑�

#define ENEMY_SIZE_BOSS_X (300)		//�A�C�X�h���S���̏c�T�C�Y
#define ENEMY_SIZE_BOSS_Y (450)		//�A�C�X�h���S���̉��T�C�Y
#define ENEMY_LIFE_BOSS (1500.0f)	//�A�C�X�h���S���̗̑�

#define ENEMY_SIZE_PIR (75)			//�s���j�A�̃T�C�Y
#define ENEMY_LIFE_PIR (70)			//�s���j�A�̗̑�

//=============================================================================
//�\���̒�`
//=============================================================================
//�G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//�ʏ�
	ENEMYSTATE_DAMAGE,		//�_���[�W
	ENEMYSTATE__MAX			//��Ԃ̍ő吔
}ENEMYSTATE;//�G�̎��

typedef enum
{
	ENEMYTYPE_000 = 0,		//�r�[�g��
	ENEMYTYPE_001,			//�J���X
	ENEMYTYPE_002,			//�J���X����
	ENEMYTYPE_003,			//�K�C�R�c
	ENEMYTYPE_004,			//�C�m�V�V
	ENEMYTYPE_005,			//�R�E����
	ENEMYTYPE_006,			//�A�C�X�h���S��
	ENEMYTYPE_007,			//�s���j�A
	ENEMYTYPE_008,			//�_�[�N�K�C�R�c
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
	int nPosX;			//�ړ��ł��鉡�͈�
	int nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int nPatternAnim;	//�A�j���[�V�����p�^�[��
	bool bLeft;			//������
	bool bUse;			//�g�p���Ă��邩�ǂ���
	int nAttackTime;	//�U������
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
bool HitEnemy2(void);
#endif