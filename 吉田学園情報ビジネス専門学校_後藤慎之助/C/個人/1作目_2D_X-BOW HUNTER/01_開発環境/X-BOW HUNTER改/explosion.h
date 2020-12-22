//=============================================================================
//
// X-Bow Hunter [explosion.h]の処理 (爆発）
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;	//色
	int nCounterAnim;	//カウンター
	int nPatternAnim;	//パターンNo
	bool bUse;			//使用しているかどうか
}EXPLOSION;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
bool SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nSize);

#endif