//=============================================================================
//
//	ナインストーリーズの処理 [block.h](ブロック)
// Author:後藤慎之助
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
#define MAX_BLOCK (64)//ブロックの最大数

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
	BLOCKTYPE_000 = 0,	//通り抜け不可能なブロック
	BLOCKTYPE_001,		//通り抜け可能な板
	BLOCKTYPE_002,		//Wやじるし看板
	BLOCKTYPE_003,		//家
	BLOCKTYPE_004,		//塔入口
	BLOCKTYPE_005,		//塔の頂への足場
	BLOCKTYPE_006,		//つらら
	BLOCKTYPE_007,		//塔の頂へのワープゾーン
	BLOCKTYPE_008,		//W看板
	BLOCKTYPE_009,		//壊せるブロック
	BLOCKTYPE_010,		//タワー
	BLOCKTYPE_011,		//水
	BLOCKTYPE_012,		//縦に長い家
	BLOCKTYPE_013,		//Sやじるし看板
	BLOCKTYPE_014,		//アスレチックの柱
	BLOCKTYPE_015,		//トゲブロック
	BLOCKTYPE_MAX		//ブロックの種類の数
}BLOCKTYPE;//ブロックの種類

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	BLOCKTYPE type;		//ブロックの種類
	int nSizeX;			//横の大きさ
	int nSizeY;			//縦の大きさ
	int nLife;			//体力
	bool bUse;			//使用しているかどうか
}BLOCK;//ブロックの情報

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
BLOCK *GetBlock(void);
void SetBlock(D3DXVECTOR3 pos, int nSizeX,int nSizeY,BLOCKTYPE type,int nLife);
bool CollisionBlock(D3DXVECTOR3 *pPos,		// プレイヤーの現在の位置
	D3DXVECTOR3 *pPosOld,					// プレイヤーの前の位置
	int nSize);
bool GetBlockX(void);
bool GetBlockY(void);

#endif