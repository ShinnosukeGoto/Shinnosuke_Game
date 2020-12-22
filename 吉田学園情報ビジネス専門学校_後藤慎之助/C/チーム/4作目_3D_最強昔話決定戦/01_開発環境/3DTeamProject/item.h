//=============================================================================
//
// 最強昔話決定戦の処理 [item.h] (アイテム)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_ITEM (64)	//アイテムの最大数
#define ITEM_SIZE (D3DXVECTOR3(80.0f,80.0f,80.0f))

#define ITEM_SET_TIME 2         //アイテム設置までのフレーム
#define ITEM_VISIBLE_TIME 10    //視認性を上げるためのフレーム
#define ITEM_INVINCIBLE_TIME 60 //炎への無敵時間

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
    ITEMTYPE_000 = 0,	//炎
    ITEMTYPE_001,		//ボム
    ITEMTYPE_002,		//スピード
    ITEMTYPE_003,		//キック
    ITEMTYPE_004,		//ハート
    ITEMTYPE_005,		//タイマー
    ITEMTYPE_MAX
}ITEMTYPE;//アイテムの種類

typedef struct
{
    D3DXVECTOR3 pos;	//位置
    D3DXVECTOR3 move;	//移動量
    D3DXVECTOR3 size;   //大きさ
    D3DXMATRIX mtxWorld;//行列計算用
    ITEMTYPE type;		//種類
    int nCntTime;       //セット中のカウント
    int nCntInvincible; //無敵フレームのカウント
    int nCntVisible;    //出現後即座に取った場合の、視認性を上げるため
    bool bSet;          //セット中かどうか
    bool bUse;			//使用しているかどうか
}ITEM;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(D3DXVECTOR3 pos, D3DXVECTOR3 move, ITEMTYPE type);
ITEM*GetItem(void);
bool HitItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size , bool bPlayer, ITEMTYPE type);
bool HitAllTypeItem(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

#endif