//=============================================================================
//
// 最強昔話決定戦 [petal.h]の処理 (花びら）
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _PETAL_H_
#define _PETAL_H_

//=============================================================================
//インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_PETAL (512)	//花びらの最大数

#define SET_PETAL_FRAME 18    //花びらを発生させるフレーム

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
    PETALTYPE_000 = 0,		//桜の花びら
    PETALTYPE_MAX			//種類の最大数
}PETALTYPE;	//花びらの種類

typedef struct
{
    D3DXVECTOR3 pos;	//位置
    D3DXVECTOR3 move;   //移動量
    D3DXCOLOR col;		//色
    float fSize;		//大きさ
    float fAngle;       //画像の角度
    D3DXVECTOR3 vertex1;//各頂点
    D3DXVECTOR3 vertex2;//各頂点
    D3DXVECTOR3 vertex3;//各頂点
    D3DXVECTOR3 vertex4;//各頂点
    float fRotSpeed;    //回転速度
    PETALTYPE type;		//種類
    bool bUse;			//使用しているかどうか
}PETAL;	//花びらの情報

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitPetal(void);
void UninitPetal(void);
void UpdatePetal(void);
void DrawPetal(void);
void SetPetal(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fSize, float fAngle, float fRotSpeed, PETALTYPE type);

#endif