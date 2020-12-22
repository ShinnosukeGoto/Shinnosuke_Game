//=============================================================================
//
// 最強昔話決定戦の処理 [title.h] (タイトル)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define PRESS_ENTER_X (460)//画像のサイズ（縦）
#define PRESS_ENTER_Y (80)//画像のサイズ（横）

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitTitle(void);
void UpdateTitle(void);
void UninitTitle(void);
void DrawTitle(void);
#endif