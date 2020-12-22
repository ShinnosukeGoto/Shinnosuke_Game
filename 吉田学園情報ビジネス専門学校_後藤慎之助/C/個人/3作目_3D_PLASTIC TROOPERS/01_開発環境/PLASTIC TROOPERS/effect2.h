//=============================================================================
//
// PLASTIC TROOPERSの処理 [effect.h] (パーティクルを使ったエフェクト)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _EFFECT2_H_
#define _EFFECT2_H_
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_EFFECT2 (2048)
#define EFFECT2_SIZE_HIT (10.0f)
#define EFFECT2_SIZE_EXPLOSION (15.0f)
#define EFFECT2_SIZE_HEAL (6.0f)
#define EFFECT2_INIT_COLOR_HIT D3DXCOLOR(1.0f, 0.0f, 1.0f, 0.7f)    //紫
#define EFFECT2_INIT_COLOR_EXPLOSION D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)  //黒
#define EFFECT2_INIT_COLOR_EXPLOSION2 D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f) //赤
#define EFFECT2_INIT_COLOR_RUN D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f)        //灰色
#define EFFECT2_INIT_COLOR_HEAL D3DXCOLOR(0.0f, 0.88f, 0.57f, 1.0f)       //緑
#define EFFECT2_INIT_COLOR_GEYSER D3DXCOLOR(0.05f, 0.81f, 0.99f, 1.0f)       //水色

//=============================================================================
//構造体宣言
//=============================================================================
typedef enum
{
    EFFECT2TYPE_HIT = 0, //ヒット
    EFFECT2TYPE_EXPLOSION,
    EFFECT2TYPE_RUN,
    EFFECT2TYPE_HEAL,
    EFFECT2TYPE_GEYSER, //間欠泉
    EFFECT2TYPE_STORM, //ストームソード白
    EFFECT2TYPE_STORM2, //ストームソード黄
    EFFECT2TYPE_FIRE, //炎
    EFFECT2TYPE_RARE, //キラキラ
    EFFECT2TYPE_FIRE2, //炎2（ボスの火柱）
    EFFECT2TYPE_MAX
}EFFECT2TYPE;

typedef struct
{
    D3DXVECTOR3 pos;	//エフェクト発生源
    D3DXVECTOR3 move;	//移動
    D3DXMATRIX mtxWorld;//行列計算用
    EFFECT2TYPE type;//種類
    D3DXCOLOR col;
    float fRadius;      //円状のエフェクトの半径
    bool bUse;			//使用判定
}EFFECT2;

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitEffect2(void);
void UninitEffect2(void);
void UpdateEffect2(void);
void SetEffect2(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, D3DXCOLOR col, EFFECT2TYPE type);
void DrawEffect2(void);

void CollisionEffectPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
void CollisionEffectEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size,int nCnt);
void CollisionEffectBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
#endif