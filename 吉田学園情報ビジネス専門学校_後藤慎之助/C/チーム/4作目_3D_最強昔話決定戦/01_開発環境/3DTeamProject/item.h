//=============================================================================
//
// �ŋ��̘b�����̏��� [item.h] (�A�C�e��)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_ITEM (64)	//�A�C�e���̍ő吔
#define ITEM_SIZE (D3DXVECTOR3(80.0f,80.0f,80.0f))

#define ITEM_SET_TIME 2         //�A�C�e���ݒu�܂ł̃t���[��
#define ITEM_VISIBLE_TIME 10    //���F�����グ�邽�߂̃t���[��
#define ITEM_INVINCIBLE_TIME 60 //���ւ̖��G����

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
    ITEMTYPE_000 = 0,	//��
    ITEMTYPE_001,		//�{��
    ITEMTYPE_002,		//�X�s�[�h
    ITEMTYPE_003,		//�L�b�N
    ITEMTYPE_004,		//�n�[�g
    ITEMTYPE_005,		//�^�C�}�[
    ITEMTYPE_MAX
}ITEMTYPE;//�A�C�e���̎��

typedef struct
{
    D3DXVECTOR3 pos;	//�ʒu
    D3DXVECTOR3 move;	//�ړ���
    D3DXVECTOR3 size;   //�傫��
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    ITEMTYPE type;		//���
    int nCntTime;       //�Z�b�g���̃J�E���g
    int nCntInvincible; //���G�t���[���̃J�E���g
    int nCntVisible;    //�o���㑦���Ɏ�����ꍇ�́A���F�����グ�邽��
    bool bSet;          //�Z�b�g�����ǂ���
    bool bUse;			//�g�p���Ă��邩�ǂ���
}ITEM;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEMTYPE type);
ITEM*GetItem(void);
bool HitItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size , bool bPlayer, ITEMTYPE type);
bool HitAllTypeItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

#endif