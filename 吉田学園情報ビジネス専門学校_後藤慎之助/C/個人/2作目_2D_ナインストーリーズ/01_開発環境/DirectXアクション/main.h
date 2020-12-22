//=============================================================================
//
// ナインストーリーズ [main.h]の処理 (メイン)
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
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xaudio2.h>

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
#define SCREEN_HEIGHTⅡ	(713)
#define NUM_VERTEX		(4)
#define NUM_POLYGON	(2)
#define RATE_MOVE (0.12)
//頂点フォーマット		(頂点座標|頂点カラー|テクスチャ座標)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define FIRE_MP (25)			//炎のMP消費
#define LIGHTNING_MP (35)		//雷のMP消費
#define DRAIN_MP (30)			//吸収のMP消費
#define VALUE_MOVE (5.0f)		//プレイヤーの移動量(5.0f)
#define VALUE_JUMP (20.0f)		//ジャンプの強さ
#define VALUE_JUMP2 (17.5f)		//2段目のジャンプの強さ
#define VALUE_LANCEJUMP (16.0f)	//槍の上攻撃のジャンプの強さ
#define VALUE_SMALLJUMP (11.5f)	//小ジャンプの強さ
#define SNOW_CD (15)			//次の雪の足音までの時間
#define ATTACK_CD (22)			//次の攻撃までの時間
#define INVINCIBLE_FRAME (15)	//無敵フレーム
#define INVINCIBLE_FRAMEⅡ (23)	//ボスの無敵フレーム
#define GRAVITY (1.1f)			//重力
#define STAGE1_X (2000)			//ステージ1の横幅
#define COUNT_WAIT_NEXTSTAGE_GAME (1)	//次のステージへ行くまでの時間
#define COUNT_WAIT_FINISH_GAME (30)	//リザルト画面へ行くまでの時間

//=============================================================================
//構造体定義
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

typedef enum
{
	MODE_TITLE,		//タイトル
	MODE_MANUAL,	//遊び方
	MODE_SELECT,	//選択画面
	MODE_GAME,		//ゲーム1(雪の平原)
	MODE_GAME2,		//ゲーム2(塔内部)
	MODE_GAME3,		//ゲーム3(塔の頂)
	MODE_RESULT,	//リザルト
	MODE_RESULT2,	//ランキング的なリザルト2
	MODE_MAX		//モードの最大数
}MODE;//モードの種類

typedef enum
{
	WSELECT_SWORD = 0,	//剣
	WSELECT_LANCE,		//槍
	WSELECT_BOW,		//弓
	WSELECT_MAX			//武器の最大数
}WSELECT;	//武器の選択

typedef enum
{
	MSELECT_FIRE = 0,	//炎
	MSELECT_LIGHTNING,	//雷
	MSELECT_DRAIN,		//吸収
	MSELECT_MAX			//魔法の最大数
}MSELECT;	//魔法の選択

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

