//=============================================================================
//
// X-Bow Hunter [hunter.h]�̏��� (�n���^�[�����@)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _HUNTER_H_
#define _HUNTER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define HUNTER_SIZE (100)	//���@�̃T�C�Y
#define HUNTER_LIFE (250.0f)	//���@�̃��C�t
#define COUNT_WAIT_NEXT_HUNTER (90)	//���̎��@���ݒu�����܂ł̎���

//=============================================================================
//�\���̒�`
//=============================================================================
//���@�̏��
typedef enum
{
	HUNTERSTATE_NORMAL = 0,	//�ʏ�
	HUNTERSTATE_DAMAGE,		//�_���[�W
	HUNTERSTATE_PINCH,		//HP4����1�ȉ��Ńs���`
	HUNTERSTATE_DEATH,		//���S���
	HUNTERSTATE_APPEAR,		//�o���܂ł̏��
	HUNTERSTATE_MAX			//��Ԃ̍ő吔
}HUNTERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	HUNTERSTATE state;	//���
	int nLife;			//���C�t
	int nStock;			//�c�@
	int nCounterState;	//��ԃJ�E���^
	bool bDisp;			//�\���ؑ�
}HUNTER;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitHunter(void);
void UninitHunter(void);
void UpdateHunter(void);
void DrawHunter(void);

HUNTER*GetHunter(void);
void HitHunter(int nDamage);
void SetHunter(D3DXVECTOR3 pos);
#endif