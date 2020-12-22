//=============================================================================
//
// PLASTIC TROOPERSの処理 [item.h] (アイテム)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_ITEM (64)	//アイテムの最大数
#define ITEM_SIZE (D3DXVECTOR3(50.0f,50.0f,50.0f))

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
    ITEMTYPE_000 = 0,	//ハート
    ITEMTYPE_001,		//ダイヤ
    ITEMTYPE_002,		//
    ITEMTYPE_003,		//
    ITEMTYPE_004,		//
    ITEMTYPE_005,		//
    ITEMTYPE_MAX
}ITEMTYPE;//アイテムの種類

typedef struct
{
    D3DXVECTOR3 pos;	//位置
    D3DXVECTOR3 move;	//移動量
    D3DXVECTOR3 size;   //大きさ
    D3DXMATRIX mtxWorld;//行列計算用
    ITEMTYPE type;		//種類
    bool bUse;			//使用しているかどうか
}ITEM;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move,ITEMTYPE type);
ITEM*GetItem(void);
void CollisionItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

#endif