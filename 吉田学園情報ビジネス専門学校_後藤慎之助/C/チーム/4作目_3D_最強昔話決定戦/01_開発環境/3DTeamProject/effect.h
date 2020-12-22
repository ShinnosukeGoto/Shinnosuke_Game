//=============================================================================
//
// 最強昔話決定戦の処理 [effect.h] (パーティクルを使ったエフェクト)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_EFFECT (8192)   //エフェクトの最大数

//エフェクトの大きさ
#define EFFECT_SIZE_FIRE 12.0f
#define EFFECT_SIZE_EXPLODE_PLAYER 15.0f
#define EFFECT_SIZE_EXPLODE_ITEM 13.0f
#define EFFECT_SIZE_GET_ITEM 10.0f
#define EFFECT_SIZE_EVIL 20.0f
#define EFFECT_SIZE_STUN 150.0f

//エフェクトの個数
#define COUNT_EFFECT_FIRE 30
#define COUNT_EFFECT_EXPLODE_PLAYER 100
#define COUNT_EFFECT_EXPLODE_ITEM 50
#define COUNT_EFFECT_ITEM 150
#define COUNT_EFFECT_HIT 75

//エフェクトのフェードアウト速度
#define FADE_SPEED_EFFECT_FIRE 0.01f
#define FADE_SPEED_EFFECT_EXPLOSION 0.005f
#define FADE_SPEED_EFFECT_ITEM 0.04f
#define FADE_SPEED_EFFECT_EVIL 0.02f
#define FADE_SPEED_EFFECT_BULLET_TRAIL 1.0f
#define FADE_SPEED_EFFECT_STUN 0.1f

//=============================================================================
//構造体宣言
//=============================================================================
typedef enum
{
    EFFECTTYPE_FIRE = 0,        //炎
    EFFECTTYPE_EXPLOSION,       //爆発
    EFFECTTYPE_GET_ITEM,        //アイテム取得
    EFFECTTYPE_EVIL,            //殺意の波動
    EFFECTTYPE_BULLET_TRAIL,    //バレットの軌跡
    EFFECTTYPE_BOSS_STUN,       //ボスの気絶
    EFFECTTYPE_PLAYER_STUN,     //プレイヤーの気絶
    EFFECTTYPE_MAX
}EFFECTTYPE;

typedef struct
{
    D3DXVECTOR3 pos;	//エフェクト発生源
    D3DXVECTOR3 move;	//移動
    D3DXMATRIX mtxWorld;//行列計算用
    EFFECTTYPE type;    //種類
    D3DXCOLOR col;      //色
    float fRadius;      //エフェクトの半径
    int nLife;          //エフェクトの残り時間
    bool bUse;			//使用判定
}EFFECT;

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fRadius, EFFECTTYPE type);
void DrawEffect(void);

#endif