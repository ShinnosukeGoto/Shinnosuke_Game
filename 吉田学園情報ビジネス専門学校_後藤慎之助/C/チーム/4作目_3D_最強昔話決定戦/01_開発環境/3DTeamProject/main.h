//=============================================================================
//
// 最強昔話決定戦 [main.h]の処理 (メイン)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include <windows.h>
#include <stdio.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xaudio2.h>
#include <time.h>

//=============================================================================
// ライブラリのリンク
//=============================================================================
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"winmm.lib")

//=============================================================================
// マクロ定義
//=============================================================================
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define NUM_VERTEX		(4)
#define NUM_POLYGON	(2)
#define CAMERA_VIEW_RANGE (10000.0f)        //カメラの視野
#define AVOID_INFINITE_LOOP 300             //無限ループ回避

//頂点フォーマット2D		(頂点座標|頂点カラー|テクスチャ座標)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//頂点フォーマット3D		(頂点座標|ノーマル|頂点カラー|テクスチャ座標)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//ゲーム状態の管理フレーム
#define COUNT_WAIT_START_GAME (120)	        //ゲーム開始へ行くまでの時間
#define COUNT_WAIT_FINISH_GAME (180)	    //リザルト画面へ行くまでの時間

//プレイヤーの基本設定関連
#define MAX_PARTS (10)				//パーツの数
#define MOTION_KEYSET_MAX (32)		//キーセット最大数
#define MAX_MATERIAL (64)           //モデルのマテリアル最大数
#define PLAYER_WIDTH (50.0f)        //プレイヤーの横
#define PLAYER_HEIGHT (50.0f)       //プレイヤーの縦
#define PLAYER_LIFE (5.0f)          //プレイヤーの体力
#define PLAYER_STORY_LIFE (250.0f)  //ストーリーの体力
#define PLAYER_FIRST_FIRE (2)       //プレイヤーの初期火力
#define PLAYER_FIRST_BOMB (1)       //プレイヤーの初期ボム個数
#define PLAYER_FIRST_SPEEDRANK (1)  //プレイヤーの初期スピードランク
#define PLAYER_FIRST_KICK (0)       //プレイヤーの初期キック力
#define GRAVITY 0.4f                //重力
#define VALUE_KNOCKBACK (25.0f)     //ノックバックの強さ
#define PLAYER_INVINCIBLE_FRAME 90  //無敵時間

#define ENEMY_INVINCIBLE_FRAME 60   //敵の無敵時間

//プレイヤーの移動関連
#define VALUE_MOVE (5.0f)		    //プレイヤーの初期移動量(5.0f)
#define VALUE_UPMOVE (1.0f)         //スピードランクに応じてあがる移動量
#define COLLISION_AVOID 49.0f       //衝突を調整

//一マスの大きさ
#define GRID_SIZE (100.0f)

//マスの数
//バトルロイヤルの場合
#define MESH_BLOCK_X 15                                                                 //メッシュのブロック数(x方向)
#define MESH_BLOCK_Z 13                                                                 //メッシュのブロック数(z方向)
#define MAX_MESH_BLOCK (MESH_BLOCK_Z * MESH_BLOCK_X)                                    //メッシュブロックの合計
#define MESH_VERTEX (MESH_BLOCK_X + 1) * (MESH_BLOCK_Z + 1)                             //頂点の数
#define MESH_INDEX (((MESH_BLOCK_X + 1) * 2) * MESH_BLOCK_Z + ((MESH_BLOCK_Z - 1) * 2)) //インデックス数
#define MESH_POLYGON (MESH_BLOCK_X * 2 * MESH_BLOCK_Z) + (MESH_BLOCK_Z - 1) * 4         //ポリゴンの数

//ストーリーの場合
#define STORY_MESH_BLOCK_X 60    //メッシュのブロック数(x方向)
#define STORY_MESH_BLOCK_Z 13    //メッシュのブロック数(z方向)
#define MAX_STORY_MESH_BLOCK (STORY_MESH_BLOCK_X * STORY_MESH_BLOCK_Z)
#define STORY_MESH_VERTEX (STORY_MESH_BLOCK_X + 1) * (STORY_MESH_BLOCK_Z + 1)//頂点の数
#define STORY_MESH_INDEX (((STORY_MESH_BLOCK_X + 1) * 2) * STORY_MESH_BLOCK_Z + ((STORY_MESH_BLOCK_Z - 1) * 2))  //インデックス数
#define STORY_MESH_POLYGON (STORY_MESH_BLOCK_X * 2 * STORY_MESH_BLOCK_Z) + (STORY_MESH_BLOCK_Z - 1) * 4       //ポリゴンの数

//ボム関連
#define TIME_BOMB 180       //ボムの爆発までの時間
#define TIME_FIRE 30        //炎の持続時間
#define BOMB_DAMAGE 25.0f   //爆弾のダメージ
#define BOMB_FLINCH 50      //ボムの怯み値

//壊せるブロック関連
#define MAX_BROKEN_BLOCK 92 //壊せるブロックの最大数
#define MAX_FIRE_BLOCK 6    //炎出現ブロックの最大数
#define MAX_BOMB_BLOCK 10   //ボム出現ブロックの最大数
#define MAX_SPEED_BLOCK 4   //スピード出現ブロックの最大数
#define MAX_KICK_BLOCK 4    //キック出現ブロックの最大数

//=============================================================================
//構造体定義
//=============================================================================
typedef struct
{
    D3DXVECTOR3 pos;	//頂点座標
    float rhw;			//平面にするため
    D3DCOLOR col;		//頂点カラー
    D3DXVECTOR2 tex;	//テクスチャ用
}VERTEX_2D;	//2D情報

typedef struct
{
    D3DXVECTOR3 pos;	//頂点座標
    D3DXVECTOR3 nor;	//法線ベクトル
    D3DCOLOR col;		//頂点カラー
    D3DXVECTOR2 tex;	//テクスチャ用
}VERTEX_3D;	//3D情報

typedef enum
{
    MODE_TITLE,		//タイトル
    MODE_MENU,	    //メニュー
    MODE_SETTING,   //設定
    MODE_MANUAL,	//遊び方
    MODE_GAME,	    //ゲーム
    MODE_NEXT_GAME,	//次のゲーム(ラウンド)
    MODE_RESULT,	//リザルト
    MODE_RANKING,	//ランキング
    MODE_EXIT,		//EXIT
    MODE_MAX		//モードの最大数
}MODE;//モードの種類

typedef enum
{
    SELECT_MOMO = 0,	//桃太郎
    SELECT_URA,		    //浦島太郎
    SELECT_KIN,		    //金太郎
    SELECT_ISUN,		//一寸法師
    SELECT_BOSS,        //ボス
    SELECT_MAX			//キャラの最大数
}SELECT;	//キャラの選択

typedef enum
{
    DIRECT_RIGHT,	//右
    DIRECT_LEFT,	//左
    DIRECT_UP,		//上
    DIRECT_DOWN,	//下
    DIRECT_MAX
}DIRECT;//方向の種類

//=============================================================================
// プロトタイプ宣言
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void);

void SetMode(MODE mode);
MODE GetMode(void);
#endif
