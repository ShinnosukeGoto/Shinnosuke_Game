//=====================================================
//
// PLASTIC TROOPERS�̏��� [model.h](�����蔻��̂Ȃ����f��)
// Author:�㓡�T�V��
//
//=====================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#define _CRT_SECURE_NO_WARNINGS

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "main.h"

//=====================================================
// �}�N����`
//=====================================================
#define MAX_MODEL (32)
#define MODEL_SIZE_X (200.0f)		// X���W�̑傫��
#define MODEL_SIZE_Y (400.0f)		// Y���W�̑傫��
#define MODEL_SIZE_Z (200.0f)		// Z���W�̑傫��

//=====================================================
// �\���̒�`
//=====================================================

typedef enum
{
    MODELTYPE_000 = 0,		//��
    MODELTYPE_001,          //�����傫����
    MODELTYPE_002,          //�Ƃ�
    MODELTYPE_003,          //��
    MODELTYPE_004,          //�s���~�b�h
    MODELTYPE_MAX			//��ނ̍ő吔
}MODELTYPE;//�u���b�N�̎��

typedef struct
{
    D3DXVECTOR3 pos;				// ���݂̈ʒu
    D3DXVECTOR3 rot;				// ���݂̊p�x
    D3DXVECTOR3 size;				// �傫��
    MODELTYPE type;                 // �^�C�v
    D3DXMATRIX mtxWorld;			// �s��v�Z�p
    LPDIRECT3DTEXTURE9 textureModel[MAX_MATERIAL];
    bool bUse;						// �g���Ă��邩
}ORNAMENT;

//=====================================================
// �v���g�^�C�v�錾
//=====================================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 size, MODELTYPE type);

#endif