//=============================================================================
//
// ナインストーリーズ [player.h]の処理 (自機)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define PLAYER_SIZE (100.0f)			//自機のサイズ
#define PLAYER_LIFE (250.0f)		//自機のライフ
#define PLAYER_MP (100.0f)			//自機のMP
#define COUNT_WAIT_NEXT_PLAYER (90)	//次の自機が設置されるまでの時間

//=============================================================================
//構造体定義
//=============================================================================
//自機の状態
typedef enum
{
	PLAYERSTATE_NORMAL = 0,	//通常
	PLAYERSTATE_DAMAGE,		//ダメージ
	PLAYERSTATE_APPEAR,		//出現までの状態
	PLAYERSTATE_MAX			//状態の最大数
}PLAYERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//現在の位置
	D3DXVECTOR3 posOld;	//前の位置
	D3DXVECTOR3 move;	//移動
	PLAYERSTATE state;	//状態
	int nLife;			//ライフ
	int nMp;			//MP
	int nStock;			//残機
	int nCounterState;	//状態カウンタ
	bool bJump;			//ジャンプ
	bool bSecondJump;	//二段目のジャンプ
	bool bLeft;			//左を向いている
	bool bDisp;			//表示切替
	WSELECT Wtype;		//武器選択
	MSELECT Mtype;		//魔法選択
	int nAttackTime;	//攻撃時間
}PLAYER;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER*GetPlayer(void);
void HitPlayer(int nDamage);
void SetPlayer(D3DXVECTOR3 pos);
bool HitPlayer2(void);
#endif