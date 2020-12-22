//=============================================================================
//
// ナインストーリーズの処理 [game2.cpp] (ゲーム画面2(塔内部))
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "game2.h"
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
GAMESTATE2 g_gamestate2;
int g_nCunterGamestate2 = 0;	//次の画面へ行くまでの時間

int g_nCntStateGame2;		//T連打回避用
GAMELEVEL g_nGameLevel2;	//ゲームの難易度

//=============================================================================
//ゲーム2初期化処理 
//=============================================================================
void InitGame2(void)
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
	g_nGameLevel2 = GetTitle();

	//敵の設置
	SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
	SetEnemy(D3DXVECTOR3(0, 250, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ

	//SetBlock(D3DXVECTOR3(500, 370, 0.0f), 120, 120, BLOCKTYPE_000,30);		//足場(デバック用)
	//SetBlock(D3DXVECTOR3(500, 570, 0.0f), 120, 120, BLOCKTYPE_007, 30);		//塔の頂へのワープゾーン(デバック用)
	
	//グローバル変数の初期化
	g_gamestate2 = GAMESTATE2_NORMAL;
	g_nCunterGamestate2 = 0;
	g_nCntStateGame2 = 0;
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame2(void)
{
	switch (g_gamestate2)
	{
	case GAMESTATE2_NORMAL:
		UpdateTime();		//タイムの更新

		//Tキーが押されたら、タイトル画面へリセット
		if (g_nCntStateGame2 == 0)
		{
			if (GetKeyboardPress(DIK_T))
			{
				SetMode(MODE_TITLE);
				InitTitle();
				StopSound(SOUND_LABEL_BGM001);	//ステージ1,2のBGMをとめる
				g_nCntStateGame2 = 6000;	//Tキー連打回避
			}
		}
		break;

	//次のステージへ
	case GAMESTATE2_NEXTSTAGE:
		g_nCunterGamestate2++;
		if (g_nCunterGamestate2 >= COUNT_WAIT_NEXTSTAGE_GAME)
		{
			g_gamestate2 = GAMESTATE2_NONE;
			InitGame3();		//ゲーム3の初期化
			//画面モードの設定
			SetFade(MODE_GAME3);		//ステージ3へ
			StopSound(SOUND_LABEL_BGM001);	//ステージ1,2のBGMをとめる
		}
		break;

	//ゲームオーバー
	case GAMESTATE2_GAMEOVER:
		g_nCunterGamestate2++;
		if (g_nCunterGamestate2 >= COUNT_WAIT_FINISH_GAME)
		{
			g_gamestate2 = GAMESTATE2_NONE;
			InitResult();		//リザルト画面の初期化
			//画面モードの設定
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM001);	//ステージ1,2のBGMをとめる
		}
		break;
	}

	UpdateBg();			//背景更新
	UpdateBg2();		//背景更新
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
void UninitGame2(void)
{
	UninitBg();			//背景
	UninitBg2();		//背景
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
void DrawGame2(void)
{
	DrawBg();		//背景の描画
	DrawBg2();		//背景の描画
	DrawFloor();	//床の描画
	DrawBlock();	//ブロックの描画
	DrawItem();		//アイテムの描画
	DrawEnemy();	//敵の描画
	DrawPlayer();	//プレイヤーの描画
	DrawMpbar();		//ライフ表示の描画
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
void SetGameState2(GAMESTATE2 state)
{
	g_gamestate2 = state;
	g_nCunterGamestate2 = 0;
}
