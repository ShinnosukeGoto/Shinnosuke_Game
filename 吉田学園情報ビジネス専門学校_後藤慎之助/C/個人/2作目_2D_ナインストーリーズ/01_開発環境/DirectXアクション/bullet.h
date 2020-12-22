//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [bullet.h] (�G�̒e)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_BULLET (64)	//�e�̍ő吔

//=============================================================================
//�\���̒�`
//=============================================================================
//�e�̏��
typedef enum
{
	BULLETTYPE_OCT=0,		//�K�C�R�c�̒e
	BULLETTYPE_GHO,			//�_�[�N�K�C�R�c�̒e
	BULLETTYPE_BOSS,		//�{�X�̒e
	BULLETTYPE_MAX			//��ނ̍ő吔
}BULLETTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	BULLETTYPE type;	//���
	int nLife;			//����
	D3DXCOLOR col;		//�F
	int nSize;			//�傫��
	int nCounterAnim;	//�A�j���[�V�����J�E���^�[
	int nPatternAnim;	//�A�j���[�V�����p�^�[��
	bool bUse;			//�g�p���Ă��邩�ǂ���
}BULLET;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, int nLife, D3DXCOLOR col, int nSize);
BULLET*GetBullet(void);
#endif