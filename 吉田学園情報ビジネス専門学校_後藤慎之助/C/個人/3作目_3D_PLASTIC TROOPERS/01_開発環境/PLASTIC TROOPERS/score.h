//=============================================================================
//
// ナインストーリーズの処理 [score.h] (スコア)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_
#define _CRT_SECURE_NO_WARNINGS_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define SCORE_SIZE_X (200.0f/10.0f)	//スコアの横のサイズ
#define SCORE_SIZE_Y (60.0f)		//スコアの縦のサイズ

#define MAX_SCORE (8)//スコアの桁の数

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitScore(void);			    //初期化処理
void UpdateScore(void);			    //更新処理
void UninitScore(void);			    //終了処理
void DrawScore(void);			    //描画処理
void ScoreCalculation(int score);	//スコアの計算
int GetScore(void);

#endif