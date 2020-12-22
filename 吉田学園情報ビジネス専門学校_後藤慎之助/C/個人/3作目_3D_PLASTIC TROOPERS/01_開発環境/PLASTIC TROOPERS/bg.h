//=============================================================================
//
// PLASTIC TROOPERS�̏��� [bg.h] (�F���̔w�i)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//=============================================================================
//�}�N����`
//=============================================================================
#define BG_SIZE (7000.0f)
#define BG_ROT_SPEED 0.00005f
//#define MESHFIELD_X_BLOCK (2)
//#define MESHFIELD_Z_BLOCK (8)
//#define MESHFIELD_VERTEX_NUM ((MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Z_BLOCK + 1 )) //���_��
//#define MESHFIELD_INDEX_NUM (((MESHFIELD_X_BLOCK + 1) * 2) * MESHFIELD_Z_BLOCK + ((MESHFIELD_Z_BLOCK - 1) * 2))//�C���f�b�N�X��14
//#define MESHFIELD_PRIMITIVE_NUM (((MESHFIELD_X_BLOCK * MESHFIELD_Z_BLOCK) * 2) + ((MESHFIELD_Z_BLOCK - 1) * 4))//�|���S����12

//=============================================================================
//�\���̐錾
//=============================================================================
typedef struct
{
    //���b�V���t�B�[���h�̈ʒu
    D3DXVECTOR3 pos;
    //���b�V���t�B�[���h�̌���
    D3DXVECTOR3 rot;
    int nBgBlockX = 8;
    int nBgBlockZ = 8;
    int nBgVertexNUM;
    int nBgIndexNUM;
    int nBgPrimitiveNUM;
}BG;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitBg(void);
void UninitBg(void);
void UpdateBg(void);
void DrawBg(void);

#endif
