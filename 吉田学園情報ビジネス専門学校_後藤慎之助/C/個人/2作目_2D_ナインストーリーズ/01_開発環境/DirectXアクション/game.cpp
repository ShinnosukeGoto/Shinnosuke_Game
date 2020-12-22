//=============================================================================
//
// ナインストーリーズの処理 [game.cpp] (ゲーム画面1(雪のステージ))
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "game.h"
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
#include "game2.h"
#include "ui.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
GAMESTATE g_gamestate;
int g_nCunterGamestate = 0;	//次の画面へ行くまでの時間

int g_nCntStateGame;		//T連打回避用
GAMELEVEL g_nGameLevel;		//ゲームの難易度

//=============================================================================
//ゲーム初期化処理 (一度に読み込むと重くなるので3つにわけている)
//=============================================================================
void InitGameFirst(void)
{
	InitBlock();		//ブロックの初期化
	InitFloor();		//床の初期化
	InitEffect2();		//エフェクト2の初期化
}

void InitGameSecond(void)
{
	InitItem();			//アイテムの初期化
	InitBullet();		//敵の弾の初期化
	InitAttack();		//攻撃の初期化
	InitExplosion();	//爆発の初期化
	InitUI();			//UIの初期化
}

void InitGameThird(void)
{
	InitBg();			//背景初期化
	InitBg2();			//背景初期化
	InitScore();		//スコアの初期化
	InitTime();			//タイムの初期化
	InitEnemy();		//敵の初期化
	InitMagic();		//魔法の初期化
	InitEffect();		//エフェクトの初期化
	InitPlayer();		//プレイヤーの初期化

	//ゲームの難易度を取得
	g_nGameLevel = GetTitle();

	//敵の設置
	SetEnemy(D3DXVECTOR3(1040, 600, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//ビートル
	SetEnemy(D3DXVECTOR3(1550, 550, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//ビートル
	SetEnemy(D3DXVECTOR3(2215, 350, 0.0f), D3DXVECTOR3(-1.0, 0.0, 0.0f), ENEMYTYPE_001);	//カラス
	SetEnemy(D3DXVECTOR3(5015, 250, 0.0f), D3DXVECTOR3(-2.5, 0.0, 0.0f), ENEMYTYPE_002);	//カラス亜種
	SetEnemy(D3DXVECTOR3(15000, 200, 0.0f), D3DXVECTOR3(-2.5, 0.0, 0.0f), ENEMYTYPE_002);	//カラス亜種
	SetEnemy(D3DXVECTOR3(3015, 590, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);		//ガイコツ
	SetEnemy(D3DXVECTOR3(3750, 400, 0.0f), D3DXVECTOR3(-0.7, 0.0, 0.0f), ENEMYTYPE_000);	//ビートル
	SetEnemy(D3DXVECTOR3(4350, 600, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//ビートル
	SetEnemy(D3DXVECTOR3(4550, 350, 0.0f), D3DXVECTOR3(-1.0, 0.0, 0.0f), ENEMYTYPE_001);	//カラス
	SetEnemy(D3DXVECTOR3(5270, 100, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_007);		//ピラニア
	SetEnemy(D3DXVECTOR3(5550, 100, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_007);		//ピラニア
	SetEnemy(D3DXVECTOR3(5830, 100, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_007);		//ピラニア
	SetEnemy(D3DXVECTOR3(6550, 600, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//ビートル
	SetEnemy(D3DXVECTOR3(8050, 250, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_008);		//ダークガイコツ
	SetEnemy(D3DXVECTOR3(9150, 400, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//ビートル
	SetEnemy(D3DXVECTOR3(9400, 200, 0.0f), D3DXVECTOR3(-0.5, 0.0, 0.0f), ENEMYTYPE_000);	//ビートル
	SetEnemy(D3DXVECTOR3(10500, 570, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_004);	//イノシシ

	//ゲームレベルハードの場合、敵が増える
	if (g_nGameLevel == GAMELEVEL_HARD)
	{
		SetEnemy(D3DXVECTOR3(3750, 590, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);		//ガイコツ
		SetEnemy(D3DXVECTOR3(4450, 175, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);		//ガイコツ
	}

	//ブロックの設置
	//SetBlock(D3DXVECTOR3(500, 570, 0.0f), 120, 120, BLOCKTYPE_004, 30);		//塔入口(デバック用)
	SetBlock(D3DXVECTOR3(450, 580, 0.0f), 80, 80, BLOCKTYPE_002,30);		//やじるし看板
	SetBlock(D3DXVECTOR3(1500, 600, 0.0f), 300, 50, BLOCKTYPE_000, 30);		//通れない
	SetBlock(D3DXVECTOR3(2040, 590, 0.0f), 150, 70,BLOCKTYPE_000, 30);		//通れない
	SetBlock(D3DXVECTOR3(2215, 525, 0.0f), 200, 200, BLOCKTYPE_000, 30);	//通れない
	SetBlock(D3DXVECTOR3(2800, 525, 0.0f), 250, 200, BLOCKTYPE_003, 30);	//家
	SetBlock(D3DXVECTOR3(2800, 425, 0.0f), 250, 10, BLOCKTYPE_001, 30);		//家の屋根
	SetBlock(D3DXVECTOR3(3700, 525, 0.0f), 300, 200, BLOCKTYPE_003, 30);	//家
	SetBlock(D3DXVECTOR3(3700, 425, 0.0f), 300, 10, BLOCKTYPE_001, 30);		//家の屋根
	SetBlock(D3DXVECTOR3(4350, 425, 0.0f), 300, 400, BLOCKTYPE_012, 30);	//縦に長い家
	SetBlock(D3DXVECTOR3(4350, 225, 0.0f), 300, 10, BLOCKTYPE_001, 30);		//家の屋根
	SetBlock(D3DXVECTOR3(4550, 525, 0.0f), 300, 200, BLOCKTYPE_003, 30);	//家
	SetBlock(D3DXVECTOR3(4550, 425, 0.0f), 300, 10, BLOCKTYPE_001, 30);		//家の屋根
	SetBlock(D3DXVECTOR3(3600, 590, 0.0f), 70, 70, BLOCKTYPE_000, 30);		//通れない
	SetBlock(D3DXVECTOR3(5550, 500, 0.0f), 900, 250, BLOCKTYPE_011, 30);	//水
	SetBlock(D3DXVECTOR3(5550, 320, 0.0f), 1000, 10, BLOCKTYPE_001, 30);	//橋
	SetBlock(D3DXVECTOR3(5030, 575, 0.0f), 200, 100, BLOCKTYPE_000, 30);	//通れない
	SetBlock(D3DXVECTOR3(5100, 425, 0.0f), 100, 200, BLOCKTYPE_000, 30);	//通れない(端)
	SetBlock(D3DXVECTOR3(6000, 425, 0.0f), 100, 200, BLOCKTYPE_000, 30);	//通れない(端)
	SetBlock(D3DXVECTOR3(6070, 575, 0.0f), 200, 100, BLOCKTYPE_000, 30);	//通れない
	SetBlock(D3DXVECTOR3(7100, 545, 0.0f), 350, 180, BLOCKTYPE_014, 30);	//アスレチックの柱
	SetBlock(D3DXVECTOR3(7100, 455, 0.0f), 350, 20, BLOCKTYPE_001, 30);		//アスレチック床
	SetBlock(D3DXVECTOR3(7000, 415, 0.0f), 80, 80, BLOCKTYPE_013, 30);		//S看板
	SetBlock(D3DXVECTOR3(7600, 465, 0.0f), 150, 335, BLOCKTYPE_014, 30);	//アスレチックの柱
	SetBlock(D3DXVECTOR3(7600, 305, 0.0f), 150, 20, BLOCKTYPE_001, 30);		//アスレチック床
	SetBlock(D3DXVECTOR3(7790, 535, 0.0f), 150, 185, BLOCKTYPE_014, 30);	//アスレチックの柱
	SetBlock(D3DXVECTOR3(7790, 445, 0.0f), 150, 20, BLOCKTYPE_001, 30);		//アスレチック床
	SetBlock(D3DXVECTOR3(7350, 590, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//トゲブロック
	SetBlock(D3DXVECTOR3(7430, 590, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//トゲブロック
	SetBlock(D3DXVECTOR3(7750, 590, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//トゲブロック
	SetBlock(D3DXVECTOR3(7830, 590, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//トゲブロック
	SetBlock(D3DXVECTOR3(8050, 465, 0.0f), 200, 335, BLOCKTYPE_014, 30);	//アスレチックの柱
	SetBlock(D3DXVECTOR3(8050, 305, 0.0f), 200, 20, BLOCKTYPE_001, 30);		//アスレチック床
	SetBlock(D3DXVECTOR3(8480, 415, 0.0f), 125, 435, BLOCKTYPE_014, 30);	//アスレチックの柱
	SetBlock(D3DXVECTOR3(8480, 208, 0.0f), 125, 30, BLOCKTYPE_001, 30);		//アスレチック床
	SetBlock(D3DXVECTOR3(8300, 593, 0.0f), 530, 90, BLOCKTYPE_014, 30);		//アスレチックの柱
	SetBlock(D3DXVECTOR3(8300, 538, 0.0f), 530, 20, BLOCKTYPE_001, 30);		//アスレチック床
	SetBlock(D3DXVECTOR3(8480, 490, 0.0f), 80, 80, BLOCKTYPE_015, 30);		//トゲブロック
	SetBlock(D3DXVECTOR3(8900, 580, 0.0f), 100, 100, BLOCKTYPE_009, 30);	//壊せるブロック
	SetBlock(D3DXVECTOR3(9100, 475, 0.0f), 400, 100, BLOCKTYPE_000, 30);	//通れない
	SetBlock(D3DXVECTOR3(9400, 425, 0.0f), 200, 400, BLOCKTYPE_000, 30);	//越えれない&通れない
	SetBlock(D3DXVECTOR3(11000, 300, 0.0f), 500, 650, BLOCKTYPE_010, 30);	//タワー
	SetBlock(D3DXVECTOR3(11000, 570, 0.0f), 110, 110, BLOCKTYPE_004, 30);	//塔入口
	SetBlock(D3DXVECTOR3(10885, 570, 0.0f), 120, 120, BLOCKTYPE_008, 30);	//W看板

	//アイテムの設置
	SetItem(D3DXVECTOR3(200, 585, 0.0f), ITEMTYPE_000, 70);		//壺
	SetItem(D3DXVECTOR3(3250, 585, 0.0f), ITEMTYPE_000, 70);	//壺
	SetItem(D3DXVECTOR3(3800, 385, 0.0f), ITEMTYPE_000, 70);	//壺
	SetItem(D3DXVECTOR3(4250, 185, 0.0f), ITEMTYPE_000, 70);	//壺
	SetItem(D3DXVECTOR3(4500, 590, 0.0f), ITEMTYPE_002, 70);	//樽
	SetItem(D3DXVECTOR3(7600, 585, 0.0f), ITEMTYPE_000, 70);	//壺
	SetItem(D3DXVECTOR3(8300, 590, 0.0f), ITEMTYPE_000, 70);	//壺
	SetItem(D3DXVECTOR3(8480, 160, 0.0f), ITEMTYPE_000, 70);	//壺
	SetItem(D3DXVECTOR3(9180, 585, 0.0f), ITEMTYPE_000, 70);	//壺
	SetItem(D3DXVECTOR3(9250, 585, 0.0f), ITEMTYPE_000, 70);	//壺
	SetItem(D3DXVECTOR3(10800, 590, 0.0f), ITEMTYPE_002, 70);	//樽
	SetItem(D3DXVECTOR3(11200, 590, 0.0f), ITEMTYPE_002, 70);	//樽

	//自機の設置
	SetPlayer(D3DXVECTOR3(SCREEN_WIDTH/4, 590, 0.0f));

	InitMpbar();			//ライフ表示の初期化(SetPlayerで、nStockを調整してから初期化）
	InitBar();			//ライフ表示の初期化(SetPlayerで、nMpbarを調整してから初期化）

	//ステージ1,2のBGM
	PlaySound(SOUND_LABEL_BGM001);

	//グローバル変数の初期化
	g_gamestate = GAMESTATE_NORMAL;
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

		//Tキーが押されたら、タイトル画面へリセット
		if (g_nCntStateGame == 0)
		{
			if (GetKeyboardPress(DIK_T))
			{
				SetMode(MODE_TITLE);
				InitTitle();
				StopSound(SOUND_LABEL_BGM001);	//ゲームのBGMをとめる
				g_nCntStateGame = 6000;	//Tキー連打回避
			}
		}

		break;

	//次のステージへ
	case GAMESTATE_NEXTSTAGE:
		g_nCunterGamestate++;
		if (g_nCunterGamestate >= COUNT_WAIT_NEXTSTAGE_GAME)
		{
			g_gamestate = GAMESTATE_NONE;
			InitGame2();		//ゲーム2の初期化
			//画面モードの設定
			SetFade(MODE_GAME2);		//ステージ2へ
		}
		break;

	//ゲームオーバー
	case GAMESTATE_GAMEOVER:
		g_nCunterGamestate++;
		if (g_nCunterGamestate >= COUNT_WAIT_FINISH_GAME)
		{
			g_gamestate = GAMESTATE_NONE;
			InitResult();		//リザルト画面の初期化
			//画面モードの設定
			SetFade(MODE_RESULT);
			StopSound(SOUND_LABEL_BGM001);//曲の停止(特定の物)
		}
		break;
	}

	UpdateBg();			//背景更新
	UpdateFloor();		//床更新
	UpdateBg2();		//背景更新
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
void UninitGame(void)
{
	UninitBg();			//背景
	UninitFloor();		//床
	UninitBg2();		//背景
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
void DrawGame(void)
{
	DrawBg();		//背景の描画
	DrawFloor();	//床の描画
	DrawBg2();		//背景の描画
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
void SetGameState(GAMESTATE state)
{
	g_gamestate = state;
	g_nCunterGamestate = 0;
}
