//=============================================================================
//
// 最強昔話決定戦の処理 [game.h] (ゲーム画面)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// 構造体定義
//=============================================================================
//ゲームの状態
typedef enum
{
    GAMESTATE_NONE,         //なし(試合が終わった後、ここの状態にする)
    GAMESTATE_START,        //開始
    GAMESTATE_NORMAL,       //通常
    GAMESTATE_FINSIH_ROUND, //ラウンド終了
    GAMESTATE_FINSIH_GAME,  //ゲーム終了
    GAMESTATE_PAUSE,        //ポーズ
    GAMESTATE_MAX           //最大数
}GAMESTATE;

//ゲームの管理情報
typedef struct
{
    int nCounter;           //参加人数
    int nCntPointPlayer1;   //プレイヤー1の得点
    int nCntPointPlayer2;   //プレイヤー2の得点
    int nCntPointPlayer3;   //プレイヤー3の得点
    int nCntPointPlayer4;   //プレイヤー4の得点
    D3DXVECTOR3 startPos1;  //開始位置1
    D3DXVECTOR3 startPos2;  //開始位置2
    D3DXVECTOR3 startPos3;  //開始位置3
    D3DXVECTOR3 startPos4;  //開始位置4
    SELECT winner;          //勝者
    GAMESTATE state;        //ゲーム状態
    int nMemoryTimeLimit;         //制限時間を記憶
    bool bMemoryRandomStartPos;   //ランダムな開始位置かどうかを記憶
    bool bMemorySuddenDeath;      //サドンデスがあるかどうかを記憶
}GAME;

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitGame(void);               //最初のゲームの初期化
void InitNextGame(void);           //次のラウンドへ行くときの初期化
void UpdateGame(void);
void UninitGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state);//ゲームの状態
GAME*GetGame(void);
void ResetGame(void);              //共通の初期化内容
#endif