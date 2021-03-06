//=============================================================================
//
// X-Bow Hunterの処理 [result.h] (ゲームクリアかゲームオーバー)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitResult(void);
void UpdateResult(void);
void UninitResult(void);
void DrawResult(void);
#endif