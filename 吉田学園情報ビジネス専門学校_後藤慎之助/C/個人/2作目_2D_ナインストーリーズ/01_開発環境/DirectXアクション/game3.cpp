//=============================================================================
//
// ナインストーリーズの処理 [game3.cpp] (ゲーム画面2(塔の頂)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "bg.h"			//背景の関連の処理
#include "bg2.h"
#include "player.h"		//ポリゴン描画系の処理
#include "explosion.h"	//爆発の処理
#include "score.h"		//スコアの表示
#include "fade.h"
#include "input.h"
#include "sound.h"
#include "mpbar.h"
#include "main.h"
#include "title.h"
#include "result.h"
#include "time.h"
#include "bar.h"
#include "effect.h"
#include "attack.h"
#include "magic.h"
#include "enemy.h"
#include "block.h"
#include "floor.h"
#include "bullet.h"
#include "item.h"
#include "effect2.h"
#include "game3.h"
#include "ui.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
GAMESTATE3 g_gamestate3;
int g_nCunterGamestate3 = 0;	//次の画面へ行くまでの時間

int g_nCntStateGame3;		//T連打回避用
GAMELEVEL g_nGameLevel3;	//ゲームの難易度

//=============================================================================
//ゲーム3初期化処理 
//=============================================================================
void InitGame3(void)
{
	InitBlock();		//ブロックの初期化
	InitFloor();		//床の初期化
	//InitEffect2();		//エフェクト2の初期化

	InitItem();			//アイテムの初期化
	//InitBullet();		//敵の弾の初期化
	//InitAttack();		//攻撃の初期化
	//InitExplosion();	//爆発の初期化

	//InitBg();			//背景初期化
	//InitBg2();			//背景初期化
						//InitScore();		//スコアの初期化
						//InitTime();			//タイムの初期化
	InitEnemy();		//敵の初期化
	//InitMagic();		//魔法の初期化
	//InitEffect();		//エフェクトの初期化
						//InitPlayer();		//プレイヤーの初期化

	//ゲームの難易度を取得
	g_nGameLevel3 = GetTitle();

	//ボスの設置
	SetEnemy(D3DXVECTOR3(1050, 425, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_006);		//アイスドラゴン

	//凍った姫の設置
	SetItem(D3DXVECTOR3(1100, 200, 0.0f), ITEMTYPE_004, PLAYER_SIZE*2);

	//ボス戦のBGM
	PlaySound(SOUND_LABEL_BGM004);	//ボス戦の曲

	//グローバル変数の初期化
	g_gamestate3 = GAMESTATE3_NORMAL;
	g_nCunterGamestate3 = 0;
	g_nCntStateGame3 = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame3(void)
{
	switch (g_gamestate3)
	{
	case GAMESTATE3_NORMAL:
		UpdateTime();		//タイムの更新

		//Tキーが押されたら、タイトル画面へリセット
		if (g_nCntStateGame3 == 0)
		{
			if (GetKeyboardPress(DIK_T))
			{
				SetMode(MODE_TITLE);
				InitTitle();
				StopSound(SOUND_LABEL_BGM004);	//ボス戦のBGMを止める
				g_nCntStateGame3 = 6000;	//Tキー連打回避
			}
		}
		break;

		//ゲームクリア
	case GAMESTATE3_GAMECLEAR:
		g_nCunterGamestate3++;
		if (g_nCunterGamestate3 >= COUNT_WAIT_NEXTSTAGE_GAME)
		{
			g_gamestate3 = GAMESTATE3_NONE;
			InitResult();		//リザルト画面の初期化
			//画面モードの設定
			SetFade(MODE_RESULT);		//クリア画面へ
			//StopSound(SOUND_LABEL_BGM004);	//ボス戦のBGMを止める(エネミーの方で止めているのでコメントアウト)
		}
		break;

		//ゲームオーバー
	case GAMESTATE3_GAMEOVER:
		g_nCunterGamestate3++;
		if (g_nCunterGamestate3 >= COUNT_WAIT_FINISH_GAME)
		{
			g_gamestate3 = GAMESTATE3_NONE;
			InitResult();		//リザルト画面の初期化
			//画面モードの設定
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM004);	//ボス戦のBGMを止める
		}
		break;
	}

	UpdateBg();			//背景更新
	//UpdateBg2();		//背景更新
	UpdateFloor();		//床更新
	UpdateBlock();		//ブロックの更新
	UpdateItem();		//アイテムの更新
	UpdateEnemy();		//敵の更新
	UpdatePlayer();		//プレイヤーの更新
	UpdateMpbar();		//ライフ表示の更新
	UpdateBar();		//HPバーの更新
	UpdateBullet();		//敵の弾の更新
	UpdateAttack();		//攻撃の更新
	UpdateMagic();		//魔法の更新
	UpdateEffect();		//エフェクトの更新
	UpdateEffect2();	//エフェクト2の更新
	UpdateExplosion();	//爆発の更新
	UpdateUI();			//UIの更新
	UpdateScore();		//スコアの更新
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGame3(void)
{
	UninitBg();			//背景
	//UninitBg2();		//背景
	UninitFloor();		//床
	UninitBlock();		//ブロック
	UninitItem();		//アイテム
	UninitEnemy();		//敵
	UninitPlayer();		//プレイヤー
	UninitMpbar();		//ライフ表示
	UninitBar();		//HP
	UninitBullet();		//敵の弾
	UninitAttack();		//攻撃
	UninitMagic();		//魔法
	UninitEffect();		//エフェクト
	UninitEffect2();	//エフェクト2
	UninitExplosion();	//爆発
	UninitUI();			//UI
	UninitScore();		//スコア
	UninitTime();		//タイム
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawGame3(void)
{
	DrawBg();		//背景の描画
	//DrawBg2();		//背景の描画
	DrawFloor();	//床の描画
	DrawBlock();	//ブロックの描画
	DrawItem();		//アイテムの描画
	DrawEnemy();	//敵の描画
	DrawPlayer();	//プレイヤーの描画
	DrawMpbar();	//ライフ表示の描画
	DrawBar();		//HPバーの描画
	DrawBullet();	//敵の弾の描画
	DrawAttack();	//攻撃の描画
	DrawMagic();	//魔法の描画
	DrawEffect();	//エフェクトの描画
	DrawEffect2();	//エフェクト2の描画
	DrawExplosion();//爆発の描画
	DrawUI();		//UIの描画
	DrawScore();	//スコアの描画
	DrawTime();		//タイムの描画
}

//=============================================================================
// ゲーム状態の設定
//=============================================================================
void SetGameState3(GAMESTATE3 state)
{
	g_gamestate3 = state;
	g_nCunterGamestate3 = 0;
}
