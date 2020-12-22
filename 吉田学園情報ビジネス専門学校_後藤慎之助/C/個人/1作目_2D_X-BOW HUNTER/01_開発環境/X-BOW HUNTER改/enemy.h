//=============================================================================
//
// X-Bow Hunter [enemy.h]の処理 (ザコ敵)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_ENEMY (128)			//敵の最大数

#define ENEMY_SIZE_MOS (80)		//モスキートのサイズ
#define ENEMY_LIFE_MOS (100)	//モスキートの体力

#define ENEMY_SIZE_MET (100)	//メタルバードのサイズ
#define ENEMY_LIFE_MET (160)	//メタルバードの体力

#define ENEMY_SIZE_METⅡ (150)	//メタルバード亜種のサイズ
#define ENEMY_LIFE_METⅡ (280)	//メタルバード亜種の体力

#define ENEMY_SIZE_OCT (100)	//オクトパスのサイズ
#define ENEMY_LIFE_OCT (120)	//オクトパスの体力

#define ENEMY_SIZE_GHO (130)	//ゴーストのサイズ
#define ENEMY_LIFE_GHO (200)	//ゴーストの体力

#define ENEMY_SIZE_GSO (100)	//グソクムシのサイズ
#define ENEMY_LIFE_GSO (20)		//グソクムシの体力

#define ENEMY_STAGE1 (2)		//ステージ1の敵の総数
#define ENEMY_STAGE2 (2)		//ステージ2の敵の総数
#define ENEMY_STAGE3 (1)		//ステージ3の敵の総数
#define ENEMY_STAGE4 (1)		//ステージ4の敵の総数

//=============================================================================
//構造体定義
//=============================================================================
//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//通常
	ENEMYSTATE_PINCH,		//HP4分の1以下でピンチ
	ENEMYSTATE_DAMAGE,		//ダメージ
	ENEMYSTATE__MAX			//状態の最大数
}ENEMYSTATE;

typedef enum
{
	ENEMYTYPE_000 = 0,		//モスキート
	ENEMYTYPE_001,			//メタルバード
	ENEMYTYPE_002,			//メタルバード亜種
	ENEMYTYPE_003,			//オクトパス
	ENEMYTYPE_004,			//ゴースト
	ENEMYTYPE_005,			//グソクムシ
	ENEMYTYPE_MAX			//種類の最大数
}ENEMYTYPE;//敵の種類

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動
	ENEMYSTATE state;	//状態
	int nLife;			//ライフ
	int nCounterState;	//状態カウンタ
	ENEMYTYPE type;		//種類
	bool bUse;			//使用しているかどうか
}ENEMY;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type);
ENEMY*GetEnemy(void);
void HitEnemy(int nCntEnemy, int nDamage);

#endif