//=============================================================================
//
// PLASTIC TROOPERS�̏��� [effect.h] (�p�[�e�B�N�����g�����G�t�F�N�g)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _EFFECT2_H_
#define _EFFECT2_H_
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_EFFECT2 (2048)
#define EFFECT2_SIZE_HIT (10.0f)
#define EFFECT2_SIZE_EXPLOSION (15.0f)
#define EFFECT2_SIZE_HEAL (6.0f)
#define EFFECT2_INIT_COLOR_HIT D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f)    //��
#define EFFECT2_INIT_COLOR_EXPLOSION D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)  //��
#define EFFECT2_INIT_COLOR_EXPLOSION2 D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) //��
#define EFFECT2_INIT_COLOR_RUN D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)        //�D�F
#define EFFECT2_INIT_COLOR_HEAL D3DXCOLOR(0.0f, 0.88f, 0.57f, 1.0f)       //��
#define EFFECT2_INIT_COLOR_GEYSER D3DXCOLOR(0.05f, 0.81f, 0.99f, 1.0f)       //���F

//=============================================================================
//�\���̐錾
//=============================================================================
typedef enum
{
    EFFECT2TYPE_HIT = 0, //�q�b�g
    EFFECT2TYPE_EXPLOSION,
    EFFECT2TYPE_RUN,
    EFFECT2TYPE_HEAL,
    EFFECT2TYPE_GEYSER, //�Ԍ���
    EFFECT2TYPE_STORM, //�X�g�[���\�[�h��
    EFFECT2TYPE_STORM2, //�X�g�[���\�[�h��
    EFFECT2TYPE_FIRE, //��
    EFFECT2TYPE_RARE, //�L���L��
    EFFECT2TYPE_FIRE2, //��2�i�{�X�̉Β��j
    EFFECT2TYPE_MAX
}EFFECT2TYPE;

typedef struct
{
    D3DXVECTOR3 pos;	//�G�t�F�N�g������
    D3DXVECTOR3 move;	//�ړ�
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    EFFECT2TYPE type;//���
    D3DXCOLOR col;
    float fRadius;      //�~��̃G�t�F�N�g�̔��a
    bool bUse;			//�g�p����
}EFFECT2;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitEffect2(void);
void UninitEffect2(void);
void UpdateEffect2(void);
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, D3DXCOLOR col, EFFECT2TYPE type);
void DrawEffect2(void);

void CollisionEffectPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
void CollisionEffectEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size,int nCnt);
void CollisionEffectBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
#endif