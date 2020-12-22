//=============================================================================
//
// 影の描画処理 [shadow.h]
// Author:KUDO AYANO
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
//マクロ定義
//*****************************************************************************
#define SHADOW_POS_Y 0.5f        //モデルの影の位置

#define SHADOW_SIZE_RATE 300     //影の大きさ変更の割合
#define SHADOW_ALPHA_RATE 0.01f  //影の透明度変更の割合

#define SHADOW_ALPHA_MAX 0.7f    //影の透明度の最大値
#define SHADOW_ALPHA_MIN 0.1f    //影の透明度の最小値

//*****************************************************************************
//構造体定義
//*****************************************************************************
typedef struct
{
    D3DXVECTOR3 pos;        //位置
    D3DXVECTOR3 rot;        //回転情報
    float fDiameter;        //直径
    float fDiameterMemory;  //直径を記憶
    float fHeight;          //高さ
    float fHeightOld;       //1F前の高さ
    D3DXCOLOR col;          //色
    D3DXMATRIX mtxWorld;    //行列計算用
    bool use;               //使用判定
}SHADOW;

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void InitShadow(void);
void UpdateShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fDiameter ,float fHeight , D3DXCOLOR col);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fHeight);
SHADOW *GetShadow();
void DrawShadow(void);
void UninitShadow(void);
#endif      //_SHADOW_H_