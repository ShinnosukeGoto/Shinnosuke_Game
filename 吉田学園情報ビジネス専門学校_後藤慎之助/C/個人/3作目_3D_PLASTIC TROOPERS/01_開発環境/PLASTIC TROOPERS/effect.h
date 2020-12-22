//=============================================================================
//
// PLASTIC TROOPERSの処理 [effect.h] (エフェクト)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_EFFECT (32)	//エフェクトの最大数
#define TARGET_SIZE D3DXVECTOR3(60.0f,60.0f,0.0f)
#define ENERGYOUT_SIZE D3DXVECTOR3(100.0f,20.0f,0.0f)
#define UNLOCK_SIZE D3DXVECTOR3(200.0f,50.0f,0.0f)

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
    EFFECTTYPE_000 = 0,	//ロックオン
    EFFECTTYPE_001,		//エナジー切れ
    EFFECTTYPE_002,		//ロック解除
    EFFECTTYPE_003,		//
    EFFECTTYPE_004,		//
    EFFECTTYPE_005,		//
    EFFECTTYPE_MAX
}EFFECTTYPE;//アイテムの種類

typedef struct
{
    D3DXVECTOR3 pos;	//位置
    D3DXVECTOR3 move;	//移動量
    D3DXVECTOR3 size;   //大きさ
    D3DXMATRIX mtxWorld;//行列計算用
    EFFECTTYPE type;	//種類
    D3DXCOLOR col;
    int nCounterAnim;	//カウンター
    int nPatternAnim;	//パターンNo
    bool bUse;			//使用しているかどうか
}EFFECT;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, EFFECTTYPE type);
EFFECT*GetEffect(void);

#endif