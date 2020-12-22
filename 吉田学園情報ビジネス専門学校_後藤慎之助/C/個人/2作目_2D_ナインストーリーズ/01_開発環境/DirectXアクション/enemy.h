//=============================================================================
//
// ナインストーリーズ [enemy.h]の処理 (ゲームに出てくる敵)
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

#define ENEMY_SIZE_MOS (60)			//ビートルのサイズ
#define ENEMY_LIFE_MOS (40)			//ビートルの体力

#define ENEMY_SIZE_MET (100)		//カラスのサイズ
#define ENEMY_LIFE_MET (80)			//カラスの体力

#define ENEMY_SIZE_METⅡ (100)		//カラス亜種のサイズ
#define ENEMY_LIFE_METⅡ (40)		//カラス亜種の体力

#define ENEMY_SIZE_OCT (100)		//ガイコツのサイズ
#define ENEMY_LIFE_OCT_NORMAL (80)	//ガイコツの体力(ノーマル)
#define ENEMY_LIFE_OCT_HARD (94)	//ガイコツの体力(ハード)
#define ENEMY_LIFE_DARK (160)		//ダークガイコツの体力

#define ENEMY_SIZE_GHO (120)		//イノシシのサイズ
#define ENEMY_LIFE_GHO (150)		//イノシシの体力

#define ENEMY_SIZE_GSO (85)			//コウモリのサイズ
#define ENEMY_LIFE_GSO (30)			//コウモリの体力

#define ENEMY_SIZE_BOSS_X (300)		//アイスドラゴンの縦サイズ
#define ENEMY_SIZE_BOSS_Y (450)		//アイスドラゴンの横サイズ
#define ENEMY_LIFE_BOSS (1500.0f)	//アイスドラゴンの体力

#define ENEMY_SIZE_PIR (75)			//ピラニアのサイズ
#define ENEMY_LIFE_PIR (70)			//ピラニアの体力

//=============================================================================
//構造体定義
//=============================================================================
//敵の状態
typedef enum
{
	ENEMYSTATE_NORMAL = 0,	//通常
	ENEMYSTATE_DAMAGE,		//ダメージ
	ENEMYSTATE__MAX			//状態の最大数
}ENEMYSTATE;//敵の種類

typedef enum
{
	ENEMYTYPE_000 = 0,		//ビートル
	ENEMYTYPE_001,			//カラス
	ENEMYTYPE_002,			//カラス亜種
	ENEMYTYPE_003,			//ガイコツ
	ENEMYTYPE_004,			//イノシシ
	ENEMYTYPE_005,			//コウモリ
	ENEMYTYPE_006,			//アイスドラゴン
	ENEMYTYPE_007,			//ピラニア
	ENEMYTYPE_008,			//ダークガイコツ
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
	int nPosX;			//移動できる横範囲
	int nCounterAnim;	//アニメーションカウンター
	int nPatternAnim;	//アニメーションパターン
	bool bLeft;			//左向き
	bool bUse;			//使用しているかどうか
	int nAttackTime;	//攻撃時間
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
bool HitEnemy2(void);
#endif