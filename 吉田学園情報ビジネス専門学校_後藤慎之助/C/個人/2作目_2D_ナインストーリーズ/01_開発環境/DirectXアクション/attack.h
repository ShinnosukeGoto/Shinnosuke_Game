//=============================================================================
//
// �i�C���X�g�[���[�Y [attack.h]�̏����i�v���C���[�̕���U���j
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _ATTACK_H_
#define _ATTACK_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_ATTACK (32)		//�U���̍ő吔

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
	ATTACKTYPE_SWORD = 0,		//��
	ATTACKTYPE_LANCE,			//��
	ATTACKTYPE_LANCE2,			//��
	ATTACKTYPE_BOW,				//�|
	ATTACKTYPE_BOW2,			//�|�̉��U���p
	ATTACKTYPE_MAX				//��ނ̍ő吔
}ATTACKTYPE;	//����I��

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	float fLife;		//�U���S�̂̃t���[����
	D3DXCOLOR col;		//�F
	ATTACKTYPE type;	//���
	int nSize;			//�傫��
	DIRECT nDirect;		//����
	bool bUse;			//�g�p���Ă��邩�ǂ���
}ATTACK;	//����U���̏��

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitAttack(void);
void UninitAttack(void);
void UpdateAttack(void);
void DrawAttack(void);
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLife, D3DXCOLOR col, ATTACKTYPE type, int nSize, DIRECT nDirect);
ATTACK*GetAttack(void);
int GetUseAttack(void);	//�U�����g������
#endif