//=============================================================================
//
// PLASTIC TROOPERSの処理 [bg.h] (宇宙の背景)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

//=============================================================================
//マクロ定義
//=============================================================================
#define BG_SIZE (7000.0f)
#define BG_ROT_SPEED 0.00005f
//#define MESHFIELD_X_BLOCK (2)
//#define MESHFIELD_Z_BLOCK (8)
//#define MESHFIELD_VERTEX_NUM ((MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Z_BLOCK + 1 )) //頂点数
//#define MESHFIELD_INDEX_NUM (((MESHFIELD_X_BLOCK + 1) * 2) * MESHFIELD_Z_BLOCK + ((MESHFIELD_Z_BLOCK - 1) * 2))//インデックス数14
//#define MESHFIELD_PRIMITIVE_NUM (((MESHFIELD_X_BLOCK * MESHFIELD_Z_BLOCK) * 2) + ((MESHFIELD_Z_BLOCK - 1) * 4))//ポリゴン数12

//=============================================================================
//構造体宣言
//=============================================================================
typedef struct
{
    //メッシュフィールドの位置
    D3DXVECTOR3 pos;
    //メッシュフィールドの向き
    D3DXVECTOR3 rot;
    int nBgBlockX = 8;
    int nBgBlockZ = 8;
    int nBgVertexNUM;
    int nBgIndexNUM;
    int nBgPrimitiveNUM;
}BG;

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitBg(void);
void UninitBg(void);
void UpdateBg(void);
void DrawBg(void);

#endif
