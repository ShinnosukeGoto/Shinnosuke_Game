//=============================================================================
//
// �ŋ��̘b�����̏��� [collision.h] (�Փ˔���A�����蔻��̋��ʉ�)
// Author:KUDO AYANO
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//*****************************************************************************
//�v���g�^�C�v�錾
//*****************************************************************************
bool BoxCollision(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos1Old, D3DXVECTOR3 size1, D3DXVECTOR3 * pPos2, D3DXVECTOR3 size2);
bool RectangleCollision(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos1Old, D3DXVECTOR3 size1, D3DXVECTOR3 * pPos2, D3DXVECTOR3 size2);
bool SphereCollision(D3DXVECTOR3 pos1, float size1, D3DXVECTOR3 pos2, float size2);
#endif  //_COLLISION_H_
