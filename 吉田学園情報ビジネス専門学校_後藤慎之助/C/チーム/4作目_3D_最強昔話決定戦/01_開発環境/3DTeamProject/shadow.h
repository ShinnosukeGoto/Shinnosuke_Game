//=============================================================================
//
// �e�̕`�揈�� [shadow.h]
// Author:KUDO AYANO
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
//�}�N����`
//*****************************************************************************
#define SHADOW_POS_Y 0.5f        //���f���̉e�̈ʒu

#define SHADOW_SIZE_RATE 300     //�e�̑傫���ύX�̊���
#define SHADOW_ALPHA_RATE 0.01f  //�e�̓����x�ύX�̊���

#define SHADOW_ALPHA_MAX 0.7f    //�e�̓����x�̍ő�l
#define SHADOW_ALPHA_MIN 0.1f    //�e�̓����x�̍ŏ��l

//*****************************************************************************
//�\���̒�`
//*****************************************************************************
typedef struct
{
    D3DXVECTOR3 pos;        //�ʒu
    D3DXVECTOR3 rot;        //��]���
    float fDiameter;        //���a
    float fDiameterMemory;  //���a���L��
    float fHeight;          //����
    float fHeightOld;       //1F�O�̍���
    D3DXCOLOR col;          //�F
    D3DXMATRIX mtxWorld;    //�s��v�Z�p
    bool use;               //�g�p����
}SHADOW;

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
void InitShadow(void);
void UpdateShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fDiameter ,float fHeight , D3DXCOLOR col);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fHeight);
SHADOW *GetShadow();
void DrawShadow(void);
void UninitShadow(void);
#endif      //_SHADOW_H_