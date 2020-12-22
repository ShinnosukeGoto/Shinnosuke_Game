//=============================================================================
//
// PLASTIC TROOPERS�̏��� [effect.h] (�G�t�F�N�g)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_EFFECT (32)	//�G�t�F�N�g�̍ő吔
#define TARGET_SIZE D3DXVECTOR3(60.0f,60.0f,0.0f)
#define ENERGYOUT_SIZE D3DXVECTOR3(100.0f,20.0f,0.0f)
#define UNLOCK_SIZE D3DXVECTOR3(200.0f,50.0f,0.0f)

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
    EFFECTTYPE_000 = 0,	//���b�N�I��
    EFFECTTYPE_001,		//�G�i�W�[�؂�
    EFFECTTYPE_002,		//���b�N����
    EFFECTTYPE_003,		//
    EFFECTTYPE_004,		//
    EFFECTTYPE_005,		//
    EFFECTTYPE_MAX
}EFFECTTYPE;//�A�C�e���̎��

typedef struct
{
    D3DXVECTOR3 pos;	//�ʒu
    D3DXVECTOR3 move;	//�ړ���
    D3DXVECTOR3 size;   //�傫��
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    EFFECTTYPE type;	//���
    D3DXCOLOR col;
    int nCounterAnim;	//�J�E���^�[
    int nPatternAnim;	//�p�^�[��No
    bool bUse;			//�g�p���Ă��邩�ǂ���
}EFFECT;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, EFFECTTYPE type);
EFFECT*GetEffect(void);

#endif