//=============================================================================
//
// X-Bow Hunter [boss.h]�̏��� (�t�@�C���[�o�[�h)
// Author : �㓡�T�V��
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
#define	MAX_BOSS (4)
#define MAX_TYPE_BOSS (4)
#define BOSS_SIZE (380)	//�t�@�C���[�o�[�h�̃T�C�Y
#define BOSS_LIFE (400)	//�t�@�C���[�o�[�h�̃��C�t

//=============================================================================
//�\���̒�`
//=============================================================================
//�G�̏��
typedef enum
{
	BOSSSTATE_NORMAL = 0,	//�ʏ�
	BOSSSTATE_DAMAGE,		//�_���[�W
	BOSSSTATE_PINCH,		//HP4����1�ȉ��Ńs���`
	BOSSSTATE_MAX			//��Ԃ̍ő吔
}BOSSSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ�
	BOSSSTATE state;	//���
	int nLife;			//���C�t
	int nCounterState;	//��ԃJ�E���^
	int nType;			//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
}BOSS;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);
BOSS*GetBoss(void);
void HitBoss(int nCntBoss, int nDamage);

#endif