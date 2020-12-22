//=============================================================================
//
// PLASTIC TROOPERS [main.h]の処理 (メイン)
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
//頂点フォーマット2D		(頂点座標|頂点カラー|テクスチャ座標)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//頂点フォーマット3D		(頂点座標|ノーマル|頂点カラー|テクスチャ座標)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//プレイヤーの基本設定関連
#define PLAYER_WIDTH (50.0f)        //プレイヤーの横
#define PLAYER_HEIGHT (50.0f)       //プレイヤーの縦
#define PLAYER_LIFE (250.0f)        //プレイヤーの体力

//エナジー関連
#define PLAYER_ENERGY (100.0f)      //プレイヤーのエナジー
#define ENERGY_DASH (0.75f)          //ダッシュ時のエナジー
#define ENERGY_BULLET_ATTACK (0.28f)       //銃攻撃時の消費エナジー
#define ENERGY_SWORD_ATTACK (0.25f)       //剣攻撃時の消費エナジー
#define START_HEAL_ENERGY (40)      //エナジー回復が始まるまでのフレーム
#define START_HEAL_OUT_ENERGY (180) //エナジー切れから、エナジー回復が始まるまでのフレーム
#define VALUE_HEAL_ENERGY (4.0f)    //エナジー回復量

//必殺ゲージ関連
#define PLAYER_CHANCE_METER_MAX (100.0f)  //プレイヤーの必殺ゲージの最大
#define PLAYER_CHANCE_MAX (3)       //プレイヤーの必殺値の最大
#define HIT_BULLET_CHARGE (1.5f)     //銃ヒット時の必殺ゲージ増加量
#define HIT_SWORD_CHARGE (3.0f)     //剣ヒット時の必殺ゲージ増加量
#define TAKE_DAMAGE_CHARGE (6.0f)   //攻撃を受けた時の必殺ゲージ増加量
#define EXATTACK_CD (600)           //必殺技のクールタイム

//プレイヤーの移動関連
#define VALUE_MOVE (5.0f)		    //プレイヤーの移動量(5.0f)
#define VALUE_DASH (15.0f)          //プレイヤーのダッシュ
#define VALUE_GUARD (2.5f)          //ガード中の移動量
#define VALUE_AFTER_SKYDASH (7.5f)  //プレイヤーの空中の慣性
#define VALUE_JUMP (10.5f)		    //ジャンプの強さ
#define SKY_DUSH_TIME (20)          //空中ダッシュの時間

#define VALUE_EXATTACK_FRAME (180)  //ストームソードのフレーム数

#define CAMERA_VIEW_RANGE (12000.0f)        //カメラの視野

#define VALUE_KNOCKBACK (25.0f)     //ノックバックの強さ

#define GRAVITY (0.46f)			    //重力
#define MAX_MATERIAL 64             //モデルのマテリアル最大数

#define COUNT_WAIT_NEXTSTAGE_GAME (1)	//次のステージへ行くまでの時間
#define COUNT_WAIT_FINISH_GAME (60)	//リザルト画面へ行くまでの時間

#define MOTION_KEYSET_MAX (32)			// キーセット最大数

#define NORMAL_ROT_SPEED (0.05f)//通常時の振り向きスピード

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
	MODE_MANUAL,	//遊び方
	MODE_SELECT,	//選択
	MODE_CAMPAIGN,	//キャンペーン
	MODE_SURVIVAL,	//サバイバル
	MODE_VERSUS,	//バーサス
	MODE_PRACTICE,	//プラクティス
	MODE_RANKING,	//ランキング
	MODE_RESULT,	//リザルト
	MODE_EXIT,		//EXIT
	MODE_MAX		//モードの最大数
}MODE;//モードの種類

typedef enum
{
	CSELECT_ART = 0,	//アーサー
	CSELECT_0,		//
	CSELECT_1,		//
	CSELECT_2,		//
	CSELECT_MAX			//キャラの最大数
}CSELECT;	//キャラの選択

typedef enum
{
	WSELECT_SWORD = 0,	//ソード
	WSELECT_LANCE,		//ランス
	WSELECT_HAMMER,		//ハンマー
	WSELECT_DAGGER,		//ダガー
	WSELECT_HANDGUN,	//ハンドガン
	WSELECT_SNIPER,		//スナイパー
	WSELECT_MAX			//武器選択の最大数
}WSELECT;	//武器の選択（右手）

typedef enum
{
	SSELECT_NO = 0,		//盾なし
	SSELECT_YES,		//盾あり
	SSELECT_MAX			//盾選択の最大数
}SSELECT;	//盾の選択（左手）

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
