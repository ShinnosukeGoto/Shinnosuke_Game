//=============================================================================
//
// PLASTIC TROOPERS�̏��� [wall.h]�i���F�����̕ǁj
// Author:�㓡�T�V��
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �}�N����`
//=============================================================================
#define WALL_SIZE (1000.0f)
#define WALL_SIZE_Y (500.0f)
#define MAX_WALL (16)

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
    WALLTYPE_YELLOW = 0, //���F
    WALLTYPE_RED1,       //�ԐF
    WALLTYPE_RED2,       //�ԐF
    WALLTYPE_RED3,       //�ԐF
    WALLTYPE_MAX
}WALLTYPE;

typedef struct
{
    D3DXVECTOR3 pos;		//�ʒu
    D3DXVECTOR3 rot;		//��]���
    D3DXMATRIX mtxWorld;	//�s��v�Z�p
    WALLTYPE type;          //���
    bool bUse;				//�g�p����
}WALL;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitWall(void);
void UpdateWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 dir,WALLTYPE type);
void DrawWall(void);
void UninitWall(void);

WALL*GetWall(void);

#endif