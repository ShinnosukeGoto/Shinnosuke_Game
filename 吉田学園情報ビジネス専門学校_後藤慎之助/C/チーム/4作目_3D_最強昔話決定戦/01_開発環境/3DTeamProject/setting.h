//=============================================================================
//
// 最強昔話決定戦の処理 [setting.h] (設定選択)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _SETTING_H_
#define _SETTING_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
//構造体定義
//=============================================================================

typedef enum
{
    SETTINGTYPE_PLAYER,	        //プレイヤー
    SETTINGTYPE_ROUND,	        //ラウンド/難易度選択
    SETTINGTYPE_TIME,           //制限時間
    SETTINGTYPE_START_POS,		//開始位置
    SETTINGTYPE_SUDDEN_DEATH,   //サドンデス
    SETTINGTYPE_STAGE,          //ステージ
    SETTINGTYPE_OK,             //OK
    SETTINGTYPE_MAX		        //設定の種類の最大数
}SETTINGTYPE;//設定の種類

typedef enum
{
    SETTINGSTAGE_NORMAL,	        //ノーマル
    SETTINGSTAGE_PLAIN,             //平原
    SETTINGSTAGE_RUIN,		        //城跡
    SETTINGSTAGE_CROSS,		        //クロス
    SETTINGSTAGE_RANDOM,		    //ランダム
    SETTINGSTAGE_MAX		        //設定の種類の最大数
}SETTINGSTAGE;//ステージ選択の種類

typedef enum
{
    SETTINGLEVEL_NORMAL,	        //ふつう
    SETTINGLEVEL_EXPERT,	        //エキスパート
    SETTINGLEVEL_MAX		        //設定の種類の最大数
}SETTINGLEVEL;    //ストーリーの難易度選択

//設定の情報
typedef struct
{
    int nMaxPlayer;         //プレイヤーの最大数
    int nMaxRound;          //先取ラウンドの最大数
    int nTimeLimit;         //制限時間
    bool bRandomStartPos;   //ランダムな開始位置かどうか
    bool bSuddenDeath;      //サドンデスがあるかどうか
    int nStage;             //ステージ選択
    int nLevel;             //難易度
    SETTINGTYPE type;       //何を設定中か
}SETTING;

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitSetting(void);
void UpdateSetting(void);
void UninitSetting(void);
void DrawSetting(void);
SETTING *GetSetting(void);	//設定選択の取得

void InitFirstSetting(void);    //設定画面に入らなくても、初期化するもの

//=============================================================================
//マクロ定義
//=============================================================================
#define SETTING_UI (32)	//設定の表示物

//最初の設定
#define FIRST_SETTING_MAX_PLAYER 4                //プレイヤーの最大数
#define FIRST_SETTING_MAX_ROUND 3                 //先取ラウンドの最大数
#define FIRST_SETTING_TIME_LIMIT 3                //制限時間
#define FIRST_SETTING_RANDOM_START_POS false      //ランダムな開始位置かどうか
#define FIRST_SETTING_SUDDEN_DEATH true           //サドンデスがあるかどうか
#define FIRST_SETTING_STAGE SETTINGSTAGE_NORMAL   //ステージ選択
#define FIRST_SETTING_LEVEL SETTINGLEVEL_NORMAL   //難易度選択

//設定の項目名の大きさ
#define SUBJECT_SIZE_X 180.0f
#define SUBJECT_SIZE_Y 90.0f

//設定の項目名の位置
#define SUBJECT_PLAYER_POS D3DXVECTOR3(250.0f,135.0f,0.0f)
#define SUBJECT_ROUND_POS D3DXVECTOR3(250.0f,225.0f,0.0f)
#define SUBJECT_TIME_LIMIT_POS D3DXVECTOR3(250.0f,315.0f,0.0f)
#define SUBJECT_RANDOM_START_POS D3DXVECTOR3(250.0f,405.0f,0.0f)
#define SUBJECT_SUDDEN_DEATH_POS D3DXVECTOR3(250.0f,495.0f,0.0f)
#define SUBJECT_STAGE_POS D3DXVECTOR3(250.0f,585.0f,0.0f)

//選択肢の大きさ
#define CHOICE_SIZE_X 80.0f
#define CHOICE_SIZE_Y 40.0f

//選択肢の位置
#define CHOICE_PLAYER_POS D3DXVECTOR3(1000.0f,135.0f,0.0f)
#define CHOICE_ROUND_POS D3DXVECTOR3(1000.0f,225.0f,0.0f)
#define CHOICE_TIME_LIMIT_POS D3DXVECTOR3(1000.0f,315.0f,0.0f)
#define CHOICE_RANDOM_START_POS D3DXVECTOR3(1000.0f,405.0f,0.0f)
#define CHOICE_SUDDEN_DEATH_POS D3DXVECTOR3(1000.0f,495.0f,0.0f)
#define CHOICE_STAGE_POS D3DXVECTOR3(1000.0f,585.0f,0.0f)

//OKの大きさと位置
#define OK_SIZE_X 75.0f
#define OK_SIZE_Y 50.0f
#define OK_POS D3DXVECTOR3(SCREEN_WIDTH / 2, 675.0f, 0.0f)

//カーソル関連
#define COUNT_MOVE_CURSOR 12                                    //カーソルを動かせるまでのフレーム
#define CURSOR_SIZE 40.0f                                       //カーソルの大きさ
#define LEFT_CURSOR_POS D3DXVECTOR3(-120.0f,0.0f,0.0f)          //左カーソルの位置
#define RIGHT_CURSOR_POS D3DXVECTOR3(120.0f,0.0f,0.0f)          //右カーソルの位置

#endif