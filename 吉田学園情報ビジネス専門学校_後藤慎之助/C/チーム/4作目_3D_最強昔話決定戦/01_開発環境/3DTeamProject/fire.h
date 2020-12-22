//=============================================================================
//
//	�ŋ��̘b�����̏��� [fire.h](���e���甭���鉊)
//	Author:�㓡�T�V��
//
//=============================================================================
#ifndef _FIRE_H_
#define _FIRE_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_FIRE (256)

//=============================================================================
//�\���̒�`
//=============================================================================

typedef struct
{
    DWORD MatFire = 0;//�}�e���A�����̐�
    D3DXVECTOR3 pos;
    D3DXVECTOR3 rot;
    D3DXVECTOR3 size;
    SELECT type;     // �^�C�v
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    LPD3DXMESH pMeshFire;//�}�e���A�����̃|�C���^
    DWORD nNumMatFire;	//�}�e���A�����̐�
    bool bUse;
    int nCntTime;       //���̔����t���[��
}FIRE;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitFire(void);
void UninitFire(void);
void UpdateFire(void);
void DrawFire(void);
FIRE* GetFire(void);
bool HitFire(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
void SetFire(D3DXVECTOR3 pos, SELECT type);
bool HitMomoFire(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);    //�{�X�͓����Y�̔��e�݂̂��炤
#endif