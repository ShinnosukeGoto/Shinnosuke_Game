//=============================================================================
//
// X-Bow Hunter [arrow.h]�̏����i�v���C���[�̖�j
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_ARROW (64)		//��̍ő吔
#define BOW_MOVE (1.18f)	//��̈ړ���

//=============================================================================
//�\���̒�`
//=============================================================================
//��̏��
typedef enum
{
	ARROWTYPE_BEND = 0,		//�Ȏ�
	ARROWTYPE_STRAIGHT,		//����
	ARROWTYPE_MAX			//��ނ̍ő吔
}ARROWTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	int nLife;			//����
	D3DXCOLOR col;		//�F
	ARROWTYPE type;		//���
	int nSize;			//�傫��
	bool bUse;			//�g�p���Ă��邩�ǂ���
}ARROW;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitArrow(void);
void UninitArrow(void);
void UpdateArrow(void);
void DrawArrow(void);
void SetArrow(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, ARROWTYPE type, int nSize);
ARROW*GetArrow(void);
int GetUseArrow(void);	//����g������
#endif