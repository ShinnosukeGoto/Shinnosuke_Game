//=============================================================================
//
// ナインストーリーズ [bg.h]の処理  (バックグラウンド)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _bg_H_
#define _bg_H_

//=============================================================================
// マクロ定義
//=============================================================================
#define SHAKE_BG (6)	//背景を揺らす深さ

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBg(void);
void UninitBg(void);
void UpdateBg(void);
void DrawBg(void);
bool GetBg(void);
#endif