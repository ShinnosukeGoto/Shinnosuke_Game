//=====================================================
//
// PLASTIC TROOPERSの処理 [model.h](当たり判定のないモデル)
// Author:後藤慎之助
//
//=====================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#define _CRT_SECURE_NO_WARNINGS

//=====================================================
// インクルードファイル
//=====================================================
#include "main.h"

//=====================================================
// マクロ定義
//=====================================================
#define MAX_MODEL (32)
#define MODEL_SIZE_X (200.0f)		// X座標の大きさ
#define MODEL_SIZE_Y (400.0f)		// Y座標の大きさ
#define MODEL_SIZE_Z (200.0f)		// Z座標の大きさ

//=====================================================
// 構造体定義
//=====================================================

typedef enum
{
    MODELTYPE_000 = 0,		//草
    MODELTYPE_001,          //少し大きい草
    MODELTYPE_002,          //とげ
    MODELTYPE_003,          //月
    MODELTYPE_004,          //ピラミッド
    MODELTYPE_MAX			//種類の最大数
}MODELTYPE;//ブロックの種類

typedef struct
{
    D3DXVECTOR3 pos;				// 現在の位置
    D3DXVECTOR3 rot;				// 現在の角度
    D3DXVECTOR3 size;				// 大きさ
    MODELTYPE type;                 // タイプ
    D3DXMATRIX mtxWorld;			// 行列計算用
    LPDIRECT3DTEXTURE9 textureModel[MAX_MATERIAL];
    bool bUse;						// 使われているか
}ORNAMENT;

//=====================================================
// プロトタイプ宣言
//=====================================================
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 size, MODELTYPE type);

#endif