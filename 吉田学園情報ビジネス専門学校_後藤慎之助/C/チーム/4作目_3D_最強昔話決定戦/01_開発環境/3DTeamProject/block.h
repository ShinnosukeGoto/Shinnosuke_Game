//=============================================================================
//
//	�ŋ��̘b�����̏��� [block.h](�u���b�N)
//	Author:�㓡�T�V��
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_BLOCK (512)

//=============================================================================
//�\���̒�`
//=============================================================================

typedef enum
{
    BLOCKTYPE_000 = 1,		//�󂹂Ȃ��u���b�N
    BLOCKTYPE_001 = 2,		//�󂹂�u���b�N
    BLOCKTYPE_002 = 3,      //��
    BLOCKTYPE_003 = 4,      //�{��
    BLOCKTYPE_004 = 5,      //�X�s�[�h
    BLOCKTYPE_005 = 6,      //�L�b�N
    BLOCKTYPE_006 = 7,      //�v���b�V���[�u���b�N
    BLOCKTYPE_007 = 8,      //��u���b�N
    BLOCKTYPE_MAX			//��ނ̍ő吔
}BLOCKTYPE;//�u���b�N�̎��

typedef struct
{
    DWORD MatBlock = 0;//�}�e���A�����̐�
    D3DXVECTOR3 pos;
    D3DXVECTOR3 posOld;
    D3DXVECTOR3 move;
    D3DXVECTOR3 rot;
    D3DXVECTOR3 size;
    D3DXCOLOR col;
    BLOCKTYPE type;     // �^�C�v
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    LPD3DXMESH pMeshBlock;//�}�e���A�����̃|�C���^
    DWORD nNumMatBlock;	//�}�e���A�����̐�
    bool bUse;
}BLOCK;

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
BLOCK* GetBlock(void);
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool HitNormalBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size); //���Ȃ��u���b�N�̓����蔻��
bool HitBrokenBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size); //����u���b�N�̓����蔻��
bool HitAllTypeBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);                //��ނ̊֌W�Ȃ������蔻��
void SetBlock(D3DXVECTOR3 pos, BLOCKTYPE type);
bool CrushingBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBlock);

#endif