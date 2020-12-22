//=============================================================================
//
// 最強昔話決定戦 [enemy.h]の処理 (ゲームに出てくる敵)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_ENEMY (16)			//敵の最大数

#define ENEMY_SIZE_CANNON (D3DXVECTOR3(50.0f,50.0f,50.0f))  //キャノンのサイズ
#define ENEMY_HEIGHT_CANNON (50.0f)                         //キャノンの高さ
#define ENEMY_LIFE_CANNON (20.0f)		                    //キャノンの体力
#define ENEMY_CANNON_SCORE 3000                             //キャノンのスコア

#define ENEMY_SIZE_UMBRELLA (D3DXVECTOR3(65.0f,50.0f,65.0f))//アンブレラのサイズ
#define ENEMY_LIFE_UMBRELLA (20.0f)		                    //アンブレラの体力
#define ENEMY_UMBRELLA_SCORE 4000                           //アンブレラのスコア
#define ENEMY_UMBRELLA_SPEED 2.0f                           //アンブレラのスピード

#define ENEMY_SIZE_GHOST (D3DXVECTOR3(50.0f,50.0f,50.0f))   //ゴーストのサイズ
#define ENEMY_LIFE_GHOST (24.0f)		                    //ゴーストの体力
#define ENEMY_LIFE_RED_GHOST (40.0f)		                //レッドゴーストの体力
#define ENEMY_GHOST_SCORE 2000                              //ゴーストのスコア
#define ENEMY_RED_GHOST_SCORE 5000                          //レッドゴーストのスコア
#define ENEMY_GHOST_SPEED 1.5f                              //ゴーストのスピード
#define ENEMY_RED_GHOST_SPEED 2.5f                          //レッドゴーストのスピード

#define ENEMY_SIZE_SNAKE (D3DXVECTOR3(50.0f,50.0f,50.0f))   //ツチノコのサイズ
#define ENEMY_LIFE_SNAKE (20.0f)		                    //ツチノコの体力
#define ENEMY_SNAKE_SCORE 10000                             //ツチノコのスコア

#define HIT_ENEMY_DAMAGE (50.0f)                            //敵に当たった時の、自機へのダメージ

#define ENEMY_FIELD_OF_VISION 800.0f                        //敵の視野

#define ENEMY_TURN_SPEED 0.025f                             //敵の振り向き速度

#define ENEMY_ATTACK_COOL_DOWN 90                           //攻撃のクールダウン

#define ENEMY_JUMP 15.0f                                    //敵のジャンプ量

#define ENEMY_DISABLED_BOMB_FRAME 30                        //爆弾を通り抜けできるフレーム数

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
    ENEMYTYPE_CANNON = 0,		//キャノン
    ENEMYTYPE_UMBRELLA,			//アンブレラ
    ENEMYTYPE_GHOST,			//ゴースト
    ENEMYTYPE_SNAKE,			//ツチノコ
    ENEMYTYPE_RED_GHOST,		//レッドゴースト
    ENEMYTYPE_MAX			    //種類の最大数
}ENEMYTYPE;//敵の種類

typedef struct
{
    D3DXVECTOR3 pos;	            //現在の位置
    D3DXVECTOR3 posOld;	            //1F前の位置
    D3DXVECTOR3 move;	            //移動
    D3DXVECTOR3 rot;	            //向き
    D3DXVECTOR3 rotDest;            //向き(目的値)
    D3DXVECTOR3 rotMin;             //向きを変えるときに、最短で振り向くため使用
    D3DXVECTOR3 size;	            //大きさ
    D3DXMATRIX mtxWorld;            //ワールドマトリクス
    ENEMYSTATE state;               //状態
    ENEMYTYPE type;		            //種類
    float fLife;		            //ライフ
    bool bUse;			            //使用しているかどうか
    int nAttackTime;	            //攻撃時間
    int nShadow;                    //影の番号
    int nCntInvincibleFrame;        //無敵時間
    bool bTransparence;             //透明かどうか
    float fDistance;                //自機との距離
    bool bFindPlayer;               //自機を発見したかどうか
    int nCntDisabledBomb;           //爆弾を無効化するフレーム
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
void HitEnemy(int nCntEnemy, float nDamage);
bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, bool bPlayer);

bool StopFireEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);          //爆風を止めることができる敵
#endif