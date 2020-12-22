//=============================================================================
//
// ナインストーリーズ [floor.h]の処理 (ゲーム画面の床のテクスチャ)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _floor_H_
#define _floor_H_

//=============================================================================
// マクロ定義
//=============================================================================
#define SHAKE_FLOOR (5)	//床を揺らす深さ

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitFloor(void);
void UninitFloor(void);
void UpdateFloor(void);
void DrawFloor(void);
#endif