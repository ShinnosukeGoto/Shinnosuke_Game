//=====================================================
//
// PLASTIC TROOPERSの処理 [block.h](ビルなどのブロック)
// Author:後藤慎之助
//
//=====================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#define _CRT_SECURE_NO_WARNINGS

//=====================================================
// インクルードファイル
//=====================================================
#include "main.h"

//=====================================================
// マクロ定義
//=====================================================
#define MAX_BLOCK (32)
#define BLOCK_SIZE_X (200.0f)		// X座標の大きさ
#define BLOCK_SIZE_Y (400.0f)		// Y座標の大きさ
#define BLOCK_SIZE_Z (200.0f)		// Z座標の大きさ

//=====================================================
// 構造体定義
//=====================================================

typedef enum
{
    BLOCKTYPE_000 = 0,		//ビル	
    BLOCKTYPE_MAX			//種類の最大数
}BLOCKTYPE;//ブロックの種類

typedef struct
{
    D3DXVECTOR3 pos;				// 現在の位置
    D3DXVECTOR3 posOld;				// 現在の位置
    D3DXVECTOR3 rot;				// 現在の角度
    D3DXVECTOR3 size;				// 大きさ
    BLOCKTYPE type;                 // タイプ
    D3DXMATRIX mtxWorld;			// 行列計算用
    LPDIRECT3DTEXTURE9 textureModel[MAX_MATERIAL];
    bool bUse;						// 使われているか
}BLOCK;

//=====================================================
// プロトタイプ宣言
//=====================================================
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 size, BLOCKTYPE type);
bool LandBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool EnemyHitBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool HitBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
BLOCK*GetBlock(void);

#endif