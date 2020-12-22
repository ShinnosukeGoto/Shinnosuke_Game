//=============================================================================
//
//	最強昔話決定戦の処理 [block.h](ブロック)
//	Author:後藤慎之助
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_BLOCK (512)

//=============================================================================
//構造体定義
//=============================================================================

typedef enum
{
    BLOCKTYPE_000 = 1,		//壊せないブロック
    BLOCKTYPE_001 = 2,		//壊せるブロック
    BLOCKTYPE_002 = 3,      //炎
    BLOCKTYPE_003 = 4,      //ボム
    BLOCKTYPE_004 = 5,      //スピード
    BLOCKTYPE_005 = 6,      //キック
    BLOCKTYPE_006 = 7,      //プレッシャーブロック
    BLOCKTYPE_007 = 8,      //柵ブロック
    BLOCKTYPE_MAX			//種類の最大数
}BLOCKTYPE;//ブロックの種類

typedef struct
{
    DWORD MatBlock = 0;//マテリアル情報の数
    D3DXVECTOR3 pos;
    D3DXVECTOR3 posOld;
    D3DXVECTOR3 move;
    D3DXVECTOR3 rot;
    D3DXVECTOR3 size;
    D3DXCOLOR col;
    BLOCKTYPE type;     // タイプ
    D3DXMATRIX mtxWorld;//行列計算用
    LPD3DXMESH pMeshBlock;//マテリアル情報のポインタ
    DWORD nNumMatBlock;	//マテリアル情報の数
    bool bUse;
}BLOCK;

//*****************************************************************************
//プロトタイプ宣言
//*****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
BLOCK* GetBlock(void);
bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool HitNormalBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size); //壊れないブロックの当たり判定
bool HitBrokenBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size); //壊れるブロックの当たり判定
bool HitAllTypeBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);                //種類の関係ない当たり判定
void SetBlock(D3DXVECTOR3 pos, BLOCKTYPE type);
bool CrushingBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBlock);

#endif