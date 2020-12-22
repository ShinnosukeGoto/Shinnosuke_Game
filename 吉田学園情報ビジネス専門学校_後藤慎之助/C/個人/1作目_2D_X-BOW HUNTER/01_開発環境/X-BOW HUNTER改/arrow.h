//=============================================================================
//
// X-Bow Hunter [arrow.h]の処理（プレイヤーの矢）
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_ARROW (64)		//矢の最大数
#define BOW_MOVE (1.18f)	//矢の移動量

//=============================================================================
//構造体定義
//=============================================================================
//矢の状態
typedef enum
{
	ARROWTYPE_BEND = 0,		//曲射
	ARROWTYPE_STRAIGHT,		//剛射
	ARROWTYPE_MAX			//種類の最大数
}ARROWTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	int nLife;			//寿命
	D3DXCOLOR col;		//色
	ARROWTYPE type;		//種類
	int nSize;			//大きさ
	bool bUse;			//使用しているかどうか
}ARROW;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitArrow(void);
void UninitArrow(void);
void UpdateArrow(void);
void DrawArrow(void);
void SetArrow(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, ARROWTYPE type, int nSize);
ARROW*GetArrow(void);
int GetUseArrow(void);	//矢を使った回数
#endif