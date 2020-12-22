//=============================================================================
//
// �ŋ��̘b�����̏��� [effect.h] (�p�[�e�B�N�����g�����G�t�F�N�g)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
//�C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define MAX_EFFECT (8192)   //�G�t�F�N�g�̍ő吔

//�G�t�F�N�g�̑傫��
#define EFFECT_SIZE_FIRE 12.0f
#define EFFECT_SIZE_EXPLODE_PLAYER 15.0f
#define EFFECT_SIZE_EXPLODE_ITEM 13.0f
#define EFFECT_SIZE_GET_ITEM 10.0f
#define EFFECT_SIZE_EVIL 20.0f
#define EFFECT_SIZE_STUN 150.0f

//�G�t�F�N�g�̌�
#define COUNT_EFFECT_FIRE 30
#define COUNT_EFFECT_EXPLODE_PLAYER 100
#define COUNT_EFFECT_EXPLODE_ITEM 50
#define COUNT_EFFECT_ITEM 150
#define COUNT_EFFECT_HIT 75

//�G�t�F�N�g�̃t�F�[�h�A�E�g���x
#define FADE_SPEED_EFFECT_FIRE 0.01f
#define FADE_SPEED_EFFECT_EXPLOSION 0.005f
#define FADE_SPEED_EFFECT_ITEM 0.04f
#define FADE_SPEED_EFFECT_EVIL 0.02f
#define FADE_SPEED_EFFECT_BULLET_TRAIL 1.0f
#define FADE_SPEED_EFFECT_STUN 0.1f

//=============================================================================
//�\���̐錾
//=============================================================================
typedef enum
{
    EFFECTTYPE_FIRE = 0,        //��
    EFFECTTYPE_EXPLOSION,       //����
    EFFECTTYPE_GET_ITEM,        //�A�C�e���擾
    EFFECTTYPE_EVIL,            //�E�ӂ̔g��
    EFFECTTYPE_BULLET_TRAIL,    //�o���b�g�̋O��
    EFFECTTYPE_BOSS_STUN,       //�{�X�̋C��
    EFFECTTYPE_PLAYER_STUN,     //�v���C���[�̋C��
    EFFECTTYPE_MAX
}EFFECTTYPE;

typedef struct
{
    D3DXVECTOR3 pos;	//�G�t�F�N�g������
    D3DXVECTOR3 move;	//�ړ�
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    EFFECTTYPE type;    //���
    D3DXCOLOR col;      //�F
    float fRadius;      //�G�t�F�N�g�̔��a
    int nLife;          //�G�t�F�N�g�̎c�莞��
    bool bUse;			//�g�p����
}EFFECT;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, EFFECTTYPE type);
void DrawEffect(void);

#endif