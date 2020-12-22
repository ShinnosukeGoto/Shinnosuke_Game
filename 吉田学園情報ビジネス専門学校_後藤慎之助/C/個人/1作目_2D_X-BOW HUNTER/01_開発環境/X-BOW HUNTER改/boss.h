//=============================================================================
//
// X-Bow Hunter [boss.h]の処理 (ファイヤーバード)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_BOSS (4)
#define MAX_TYPE_BOSS (4)
#define BOSS_SIZE (380)	//ファイヤーバードのサイズ
#define BOSS_LIFE (400)	//ファイヤーバードのライフ

//=============================================================================
//構造体定義
//=============================================================================
//敵の状態
typedef enum
{
	BOSSSTATE_NORMAL = 0,	//通常
	BOSSSTATE_DAMAGE,		//ダメージ
	BOSSSTATE_PINCH,		//HP4分の1以下でピンチ
	BOSSSTATE_MAX			//状態の最大数
}BOSSSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動
	BOSSSTATE state;	//状態
	int nLife;			//ライフ
	int nCounterState;	//状態カウンタ
	int nType;			//種類
	bool bUse;			//使用しているかどうか
}BOSS;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);

void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType);
BOSS*GetBoss(void);
void HitBoss(int nCntBoss, int nDamage);

#endif