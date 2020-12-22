//=============================================================================
//
// PLASTIC TROOPERS�̏��� [item.h] (�A�C�e��)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_ITEM (64)	//�A�C�e���̍ő吔
#define ITEM_SIZE (D3DXVECTOR3(50.0f,50.0f,50.0f))

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
    ITEMTYPE_000 = 0,	//�n�[�g
    ITEMTYPE_001,		//�_�C��
    ITEMTYPE_002,		//
    ITEMTYPE_003,		//
    ITEMTYPE_004,		//
    ITEMTYPE_005,		//
    ITEMTYPE_MAX
}ITEMTYPE;//�A�C�e���̎��

typedef struct
{
    D3DXVECTOR3 pos;	//�ʒu
    D3DXVECTOR3 move;	//�ړ���
    D3DXVECTOR3 size;   //�傫��
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    ITEMTYPE type;		//���
    bool bUse;			//�g�p���Ă��邩�ǂ���
}ITEM;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move,ITEMTYPE type);
ITEM*GetItem(void);
void CollisionItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

#endif