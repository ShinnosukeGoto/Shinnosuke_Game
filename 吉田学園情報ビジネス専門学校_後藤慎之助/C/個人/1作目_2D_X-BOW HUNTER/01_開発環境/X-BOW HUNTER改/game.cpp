//=============================================================================
//
// X-Bow Hunterの処理 [game.cpp] (ゲーム画面)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "game.h"
#include "bg.h"			//背景の関連の処理
#include "bg2.h"		//背景の関連の処理
#include "hunter.h"		//ポリゴン描画系の処理
#include "arrow.h"		//弾の発射処理
#include "bullet.h"		//弾の発射処理
#include "explosion.h"	//爆発の処理
#include "enemy.h"		//敵の処理
#include "boss.h"		//ボスの処理
#include "effect.h"
#include "score.h"		//スコアの表示
#include "item.h"
#include "storm.h"
#include "wing.h"
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "life.h"
#include "main.h"
#include "title.h"
#include "result.h"
#include "time.h"
#include "bar.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
GAMESTATE g_gamestate;
HUCKERSTATE g_huckerstate;	//Pボタン使用の有無
int g_nCunterGamestate = 0;	//リザルトまで行く時間

int g_nCntStateGame;		//ENTER連打回避用
							//=============================================================================
							//ゲーム初期化処理 (一度に読み込むと重くなるので2つにわけている)
							//=============================================================================
void InitGame1(void)
{
	InitItem();			//アイテムの初期化
	InitWing();			//ウィングの初期化
	InitArrow();		//矢の初期化
	InitExplosion();	//爆発の初期化
}

void InitGame2(void)
{
	InitBg();			//背景初期化
	InitBg2();			//背景初期化
	InitScore();		//スコアの初期化
	InitTime();			//タイムの初期化
	InitEnemy();		//敵の初期化
	InitBoss();			//ボスの初期化
	InitBullet();		//弾の初期化
	InitStorm();		//ストームの初期化
	InitEffect();		//エフェクトの初期化
	InitHunter();		//プレイヤーの初期化

	SetEnemy(D3DXVECTOR3(1040, 240, 0.0f), D3DXVECTOR3(-0.5, 0.5, 0.0f), ENEMYTYPE_000);
	SetEnemy(D3DXVECTOR3(1140, 340, 0.0f), D3DXVECTOR3(-1.0, 0.5, 0.0f), ENEMYTYPE_000);

	SetHunter(D3DXVECTOR3(100, 590, 0.0f));

	InitLife();			//ライフ表示の初期化(SetHunterで、nStockを調整してから初期化）
	InitBar();			//ライフ表示の初期化(SetHunterで、nLifeを調整してから初期化）

	PlaySound(SOUND_LABEL_BGM001);

	//グローバル変数の初期化
	g_gamestate = GAMESTATE_NORMAL;
	g_huckerstate = AREYOUHUCKER？_NO;
	g_nCunterGamestate = 0;
	g_nCntStateGame = 0;
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(void)
{
	switch (g_gamestate)
	{
	case GAMESTATE_NORMAL:
		UpdateTime();		//タイムの更新

							//Pキーが押されたら、リザルト画面を変化させるためにハッカーと判断
		if (GetKeyboardPress(DIK_P))
		{
			g_huckerstate = AREYOUHUCKER？_YES;
		}

		//Tキーが押されたら、タイトル画面へリセット
		if (g_nCntStateGame == 0)
		{
			if (GetKeyboardPress(DIK_T))
			{
				SetMode(MODE_TITLE);
				InitTitle();
				StopSound(SOUND_LABEL_BGM001);	//ゲームのBGMをとめる
				StopSound(SOUND_LABEL_BGM004);	//ボス戦のBGMを止める
				g_nCntStateGame = 6000;	//Tキー連打回避
			}
		}

		break;
	case GAMESTATE_END:
		g_nCunterGamestate++;
		if (g_nCunterGamestate >= COUNT_WAIT_FINISH_GAME)
		{
			g_gamestate = GAMESTATE_NONE;
			InitResult();		//リザルト画面の初期化
								//画面モードの設定
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM001);//曲の停止(特定の物)
			StopSound(SOUND_LABEL_BGM004);	//ボス戦のBGMを止める
		}
		break;
	}

	UpdateBg();			//背景更新
	UpdateBg2();		//背景更新
	UpdateScore();		//スコアの更新
	UpdateItem();		//アイテムの更新
	UpdateEnemy();		//敵の更新
	UpdateBoss();		//ボスの更新
	UpdateWing();		//ウィングの更新
	UpdateHunter();		//プレイヤーの更新
	UpdateLife();		//ライフ表示の更新
	UpdateBar();		//HPバーの更新
	UpdateArrow();		//矢の更新
	UpdateBullet();		//弾の更新
	UpdateStorm();		//ストームの更新
	UpdateEffect();		//エフェクトの更新
	UpdateExplosion();	//爆発の更新

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(void)
{
	UninitBg();			//背景
	UninitBg2();		//背景
	UninitScore();		//スコア
	UninitTime();		//タイム
	UninitItem();		//アイテム
	UninitEnemy();		//敵
	UninitBoss();		//ボス
	UninitWing();		//ウィング
	UninitHunter();		//プレイヤー
	UninitLife();		//ライフ表示
	UninitBar();		//HP
	UninitArrow();		//矢
	UninitBullet();		//弾
	UninitStorm();		//ストーム
	UninitEffect();		//エフェクト
	UninitExplosion();	//爆発
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(void)
{
	DrawBg();		//背景の描画
	DrawBg2();		//背景の描画
	DrawScore();	//スコアの描画
	DrawTime();		//タイムの描画
	DrawItem();		//アイテムの描画
	DrawEnemy();	//敵の描画
	DrawBoss();		//ボスの描画
	DrawWing();		//ウィングの描画
	DrawHunter();	//プレイヤーの描画
	DrawLife();		//ライフ表示の描画
	DrawBar();		//HPバーの描画
	DrawArrow();	//矢の描画
	DrawBullet();	//弾の描画
	DrawStorm();	//ストームの描画
	DrawEffect();	//エフェクトの描画
	DrawExplosion();//爆発の描画
}

//=============================================================================
// ゲーム状態の設定
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_gamestate = state;
	g_nCunterGamestate = 0;
}

//=============================================================================
// ゲームの状態の取得
//=============================================================================
HUCKERSTATE GetGameState(void)
{
	return g_huckerstate;
}