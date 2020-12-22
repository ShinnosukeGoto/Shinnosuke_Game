//=============================================================================
//
// �ŋ��̘b����� [petal.h]�̏��� (�Ԃт�j
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _PETAL_H_
#define _PETAL_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_PETAL (512)	//�Ԃт�̍ő吔

#define SET_PETAL_FRAME 18    //�Ԃт�𔭐�������t���[��

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
    PETALTYPE_000 = 0,		//���̉Ԃт�
    PETALTYPE_MAX			//��ނ̍ő吔
}PETALTYPE;	//�Ԃт�̎��

typedef struct
{
    D3DXVECTOR3 pos;	//�ʒu
    D3DXVECTOR3 move;   //�ړ���
    D3DXCOLOR col;		//�F
    float fSize;		//�傫��
    float fAngle;       //�摜�̊p�x
    D3DXVECTOR3 vertex1;//�e���_
    D3DXVECTOR3 vertex2;//�e���_
    D3DXVECTOR3 vertex3;//�e���_
    D3DXVECTOR3 vertex4;//�e���_
    float fRotSpeed;    //��]���x
    PETALTYPE type;		//���
    bool bUse;			//�g�p���Ă��邩�ǂ���
}PETAL;	//�Ԃт�̏��

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitPetal(void);
void UninitPetal(void);
void UpdatePetal(void);
void DrawPetal(void);
void SetPetal(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSize, float fAngle, float fRotSpeed, PETALTYPE type);

#endif