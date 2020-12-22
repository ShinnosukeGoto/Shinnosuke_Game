//=============================================================================
//
// X-Bow Hunter [bullet.h] (敵の弾)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_BULLET (128)	//弾の最大数

//=============================================================================
//構造体定義
//=============================================================================
//弾の状態
typedef enum
{
	BULLETTYPE_ENEMY = 0,	//敵の弾
	BULLETTYPE_ENEMYⅡ,		//メタルバード亜種の弾
	BULLETTYPE_OCT,			//オクトパスの弾
	BULLETTYPE_GHO,			//ゴーストの弾
	BULLETTYPE_BOSS,		//ボスの弾
	BULLETTYPE_BOSS_PINCH,	//ピンチのボスの弾
	BULLETTYPE_MAX			//種類の最大数
}BULLETTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	BULLETTYPE type;	//種類
	int nLife;			//寿命
	D3DXCOLOR col;		//色
	int nSize;			//大きさ
	bool bUse;			//使用しているかどうか
}BULLET;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, int nLife, D3DXCOLOR col, int nSize);
BULLET*GetBullet(void);
#endif