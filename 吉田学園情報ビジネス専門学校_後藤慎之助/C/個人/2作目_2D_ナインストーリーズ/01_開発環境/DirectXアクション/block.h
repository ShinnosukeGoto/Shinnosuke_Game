//=============================================================================
//
//	�i�C���X�g�[���[�Y�̏��� [block.h](�u���b�N)
// Author:�㓡�T�V��
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
#define MAX_BLOCK (64)//�u���b�N�̍ő吔

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
	BLOCKTYPE_000 = 0,	//�ʂ蔲���s�\�ȃu���b�N
	BLOCKTYPE_001,		//�ʂ蔲���\�Ȕ�
	BLOCKTYPE_002,		//W�₶�邵�Ŕ�
	BLOCKTYPE_003,		//��
	BLOCKTYPE_004,		//������
	BLOCKTYPE_005,		//���̒��ւ̑���
	BLOCKTYPE_006,		//���
	BLOCKTYPE_007,		//���̒��ւ̃��[�v�]�[��
	BLOCKTYPE_008,		//W�Ŕ�
	BLOCKTYPE_009,		//�󂹂�u���b�N
	BLOCKTYPE_010,		//�^���[
	BLOCKTYPE_011,		//��
	BLOCKTYPE_012,		//�c�ɒ�����
	BLOCKTYPE_013,		//S�₶�邵�Ŕ�
	BLOCKTYPE_014,		//�A�X���`�b�N�̒�
	BLOCKTYPE_015,		//�g�Q�u���b�N
	BLOCKTYPE_MAX		//�u���b�N�̎�ނ̐�
}BLOCKTYPE;//�u���b�N�̎��

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	BLOCKTYPE type;		//�u���b�N�̎��
	int nSizeX;			//���̑傫��
	int nSizeY;			//�c�̑傫��
	int nLife;			//�̗�
	bool bUse;			//�g�p���Ă��邩�ǂ���
}BLOCK;//�u���b�N�̏��

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
BLOCK *GetBlock(void);
void SetBlock(D3DXVECTOR3 pos, int nSizeX,int nSizeY,BLOCKTYPE type,int nLife);
bool CollisionBlock(D3DXVECTOR3 *pPos,		// �v���C���[�̌��݂̈ʒu
	D3DXVECTOR3 *pPosOld,					// �v���C���[�̑O�̈ʒu
	int nSize);
bool GetBlockX(void);
bool GetBlockY(void);

#endif