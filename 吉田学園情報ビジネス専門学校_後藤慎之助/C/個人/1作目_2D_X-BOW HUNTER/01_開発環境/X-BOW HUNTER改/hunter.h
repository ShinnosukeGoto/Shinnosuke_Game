//=============================================================================
//
// X-Bow Hunter [hunter.h]の処理 (ハンターが自機)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _HUNTER_H_
#define _HUNTER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define HUNTER_SIZE (100)	//自機のサイズ
#define HUNTER_LIFE (250.0f)	//自機のライフ
#define COUNT_WAIT_NEXT_HUNTER (90)	//次の自機が設置されるまでの時間

//=============================================================================
//構造体定義
//=============================================================================
//自機の状態
typedef enum
{
	HUNTERSTATE_NORMAL = 0,	//通常
	HUNTERSTATE_DAMAGE,		//ダメージ
	HUNTERSTATE_PINCH,		//HP4分の1以下でピンチ
	HUNTERSTATE_DEATH,		//死亡状態
	HUNTERSTATE_APPEAR,		//出現までの状態
	HUNTERSTATE_MAX			//状態の最大数
}HUNTERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	HUNTERSTATE state;	//状態
	int nLife;			//ライフ
	int nStock;			//残機
	int nCounterState;	//状態カウンタ
	bool bDisp;			//表示切替
}HUNTER;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitHunter(void);
void UninitHunter(void);
void UpdateHunter(void);
void DrawHunter(void);

HUNTER*GetHunter(void);
void HitHunter(int nDamage);
void SetHunter(D3DXVECTOR3 pos);
#endif