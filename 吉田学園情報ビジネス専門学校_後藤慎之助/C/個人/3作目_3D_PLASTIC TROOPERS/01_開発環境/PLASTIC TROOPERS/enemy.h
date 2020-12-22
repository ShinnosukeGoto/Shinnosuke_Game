//=============================================================================
//
// PLASTIC TROOPERS [enemy.h]の処理 (ゲームに出てくる敵)
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
#define	MAX_ENEMY (32)			//敵の最大数

#define ENEMY_SIZE_CANNON (D3DXVECTOR3(50.0f,50.0f,50.0f))  //キャノンのサイズ
#define ENEMY_HEIGHT_CANNON (50.0f)                         //キャノンの高さ
#define ENEMY_LIFE_CANNON (40.0f)		                    //キャノンの体力

#define ENEMY_SIZE_SPIDER (D3DXVECTOR3(80.0f,50.0f,80.0f))  //スパイダーのサイズ
#define ENEMY_LIFE_SPIDER (30.0f)		                    //スパイダーの体力
#define ENEMY_LIFE_GOLDENSPIDER (30.0f)		                //ゴールデンスパイダーの体力

#define ENEMY_SIZE_BUTTERFLY (D3DXVECTOR3(30.0f,40.0f,30.0f))//バタフライのサイズ
#define ENEMY_HEIGHT_BUTTERFLY (100.0f)                         //バタフライの高さ
#define ENEMY_LIFE_BUTTERFLY (50.0f)		                    //バタフライの体力

#define ENEMY_SIZE_JUMPBOX (D3DXVECTOR3(50.0f,50.0f,50.0f))  //ジャンプボックスのサイズ
#define ENEMY_LIFE_JUMPBOX (20.0f)		                     //ジャンプボックスの体力

#define ENEMY_SIZE_FISH (D3DXVECTOR3(50.0f,50.0f,80.0f))     //フィッシュのサイズ
#define ENEMY_LIFE_FISH (60.0f)		                     //フィッシュの体力

#define HIT_ENEMY_DAMAGE (3.5f)     //敵に当たった時の、自機へのダメージ

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
    ENEMYSTATE_NORMAL = 0,	//通常
    ENEMYSTATE_DAMAGE,		//ダメージ
    ENEMYSTATE__MAX			//状態の最大数
}ENEMYSTATE;//敵の状態

typedef enum
{
    ENEMYTYPE_000 = 0,		//キャノン
    ENEMYTYPE_001,			//スパイダー
    ENEMYTYPE_002,			//バタフライ
    ENEMYTYPE_003,			//ゴールデンスパイダー
    ENEMYTYPE_004,			//ジャンプボックス
    ENEMYTYPE_005,			//フィッシュ
    ENEMYTYPE_MAX			//種類の最大数
}ENEMYTYPE;//敵の種類

typedef struct
{
    D3DXVECTOR3 pos;	//現在の位置
    D3DXVECTOR3 posOld;	//1F前の位置
    D3DXVECTOR3 posMemory;	//配置された時の位置
    D3DXVECTOR3 move;	//移動
    D3DXVECTOR3 rot;	//向き
    D3DXVECTOR3 rotDest;//向き(目的値)
    D3DXVECTOR3 rotMin; //向きを変えるときに、最短で振り向くため使用
    D3DXVECTOR3 size;	// 大きさ
    D3DXMATRIX mtxWorld;//ワールドマトリクス
    ENEMYSTATE state;	//状態
    float fLife;		//ライフ
    int nCounterState;	//状態カウンタ
    ENEMYTYPE type;		//種類
    int nPosX;			//移動できる横範囲
    int nPosZ;			//移動できる奥範囲
    int nCounterAnim;	//アニメーションカウンター
    int nPatternAnim;	//アニメーションパターン
    bool bUse;			//使用しているかどうか
    int nAttackTime;	//攻撃時間
    float fDistance;    //自機との距離
    int nRank;          //距離の順位
    bool bFollow;       //追従させるかどうか
}ENEMY;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos,D3DXVECTOR3 move, ENEMYTYPE type);
ENEMY*GetEnemy(void);
void HitEnemy(int nCntEnemy,int nCntRigid, float nDamage);
bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
void UpperEnemy(float fUp, int nCnt);
#endif