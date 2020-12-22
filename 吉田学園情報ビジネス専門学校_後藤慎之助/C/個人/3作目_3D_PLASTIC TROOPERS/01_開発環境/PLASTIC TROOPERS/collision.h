//=============================================================================
//
// PLASTIC TROOPERSの処理 [collision.h] (当たり判定の共通化)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _COLLISION_H_
#define _COLLISION_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
bool BoxCollision(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos1Old, D3DXVECTOR3 size1, D3DXVECTOR3 * pPos2, D3DXVECTOR3 size2);
bool RectangleCollision(D3DXVECTOR3* pPos1, D3DXVECTOR3* pPos1Old, D3DXVECTOR3 size1, D3DXVECTOR3 * pPos2, D3DXVECTOR3 size2);
bool SphereCollision(D3DXVECTOR3 pos1, float size1, D3DXVECTOR3 pos2, float size2);
#endif  //_COLLISION_H_
