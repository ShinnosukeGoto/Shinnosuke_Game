//=====================================================
//
// PLASTIC TROOPERS�̏��� [block.h](�r���Ȃǂ̃u���b�N)
// Author:�㓡�T�V��
//
//=====================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#define _CRT_SECURE_NO_WARNINGS

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "main.h"

//=====================================================
// �}�N����`
//=====================================================
#define MAX_BLOCK (32)
#define BLOCK_SIZE_X (200.0f)		// X���W�̑傫��
#define BLOCK_SIZE_Y (400.0f)		// Y���W�̑傫��
#define BLOCK_SIZE_Z (200.0f)		// Z���W�̑傫��

//=====================================================
// �\���̒�`
//=====================================================

typedef enum
{
    BLOCKTYPE_000 = 0,		//�r��	
    BLOCKTYPE_MAX			//��ނ̍ő吔
}BLOCKTYPE;//�u���b�N�̎��

typedef struct
{
    D3DXVECTOR3 pos;				// ���݂̈ʒu
    D3DXVECTOR3 posOld;				// ���݂̈ʒu
    D3DXVECTOR3 rot;				// ���݂̊p�x
    D3DXVECTOR3 size;				// �傫��
    BLOCKTYPE type;                 // �^�C�v
    D3DXMATRIX mtxWorld;			// �s��v�Z�p
    LPDIRECT3DTEXTURE9 textureModel[MAX_MATERIAL];
    bool bUse;						// �g���Ă��邩
}BLOCK;

//=====================================================
// �v���g�^�C�v�錾
//=====================================================
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 size, BLOCKTYPE type);
bool LandBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool EnemyHitBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool HitBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
BLOCK*GetBlock(void);

#endif