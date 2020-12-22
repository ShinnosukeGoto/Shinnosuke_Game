//=============================================================================
//
// ナインストーリーズ [hunter.cpp]の処理 (自機)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "explosion.h"
#include "game.h"
#include "sound.h"
#include "attack.h"
#include "bg.h"
#include "magic.h"
#include "enemy.h"
#include "select.h"
#include "block.h"
#include "effect2.h"
#include "game2.h"
#include "game3.h"

//=============================================================================
//マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygon[ATTACKTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygon = NULL;
int g_nCounterAnimHunt;		//アニメーションカウンター
int g_nPatternAnimHunt;		//アニメーションパターン
int g_nAttackPatternAnim;	//攻撃のアニメーションパターン
PLAYER g_Player;			//自機の情報
int g_nSnowCD;				//雪のクールタイム
int g_nDirect;				//入力された方向
int g_nChange;				//向きを変える
bool g_bBgX;				//ステージ1の横が動くかどうか
bool g_bHitPlayer;			//プレイヤーが攻撃を受けたかどうか
bool g_bEndPlayer;			//プレイヤーが背景へ飛んで行ったかどうか
MODE g_ModePlayer;			//ゲームのモードを取得

//=============================================================================
//プレイヤー初期化処理
//=============================================================================
void InitPlayer(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nCounterAnimHunt = 0;
	g_nPatternAnimHunt = 0;
	g_nAttackPatternAnim = 0;

	//プレイヤーの構造体の初期化
	g_Player.state = PLAYERSTATE_NORMAL;
	g_Player.posOld = D3DXVECTOR3(SCREEN_WIDTH / 4, 590, 0.0f);
	g_Player.pos = D3DXVECTOR3(SCREEN_WIDTH / 4, 590, 0.0f);
	g_Player.nLife = PLAYER_LIFE;
	g_Player.nMp = PLAYER_MP;
	g_Player.nCounterState = 0;
	g_Player.bLeft = false;
	g_Player.bDisp = false;
	g_Player.nStock = 0;	//残機の総数の初期化
	g_Player.Wtype = GetWSelect();
	g_Player.Mtype = GetMSelect();
	g_Player.nAttackTime = 0;
	//グローバル変数の初期化
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nSnowCD = SNOW_CD;
	g_Player.bSecondJump = false;
	g_nDirect = 0;
	g_nChange = 0;
	g_Player.bJump = false;
	g_bBgX = false;
	g_bHitPlayer = false;
	g_bEndPlayer = false;
	g_ModePlayer = MODE_GAME;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/player000.png", &g_pTexturePolygon[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/player001.png", &g_pTexturePolygon[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/player003.png", &g_pTexturePolygon[2]);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

	//頂点座標の設定（右回りで設定する）
	pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーを設定（0～255の値で設定する）
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.0f);
	pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.0f);
	pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.5f);
	pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.5f);

	//頂点データをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// プレイヤー終了処理
//=============================================================================
void UninitPlayer(void)
{
	//テクスチャの開放
	for (int nCntPlayer = 0; nCntPlayer < ATTACKTYPE_MAX; nCntPlayer++)
	{
		if (g_pTexturePolygon[nCntPlayer] != NULL)
		{
			g_pTexturePolygon[nCntPlayer]->Release();
			g_pTexturePolygon[nCntPlayer] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=============================================================================
// プレイヤー更新処理
//=============================================================================
void UpdatePlayer(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	ENEMY*pEnemy;
	int nCntEnemy;

	//ゲームのステージを取得
	g_ModePlayer = GetMode();

	int nAttack = 0;		//攻撃しているかどうか

	//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤーの状態
	switch (g_Player.state)
	{
	case PLAYERSTATE_NORMAL:	//通常の状態
		break;

	case PLAYERSTATE_DAMAGE:	//ダメージ状態
		g_Player.nCounterState--;
		
		g_bHitPlayer = true;

		if (g_Player.nCounterState <= 0)
		{
			g_Player.state = PLAYERSTATE_NORMAL;

			g_bHitPlayer = false;

			//通常描画に戻す処理
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case PLAYERSTATE_APPEAR:	//出現までの状態
		g_Player.nCounterState--;
		g_Player.bDisp = false;	//出現まで、表示させない
		if (g_Player.nCounterState <= 0)
		{
			PlaySound(SOUND_LABEL_SE_ITEM);	//復活音

			g_Player.state = PLAYERSTATE_NORMAL;

			//通常描画に戻す処理
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標を更新
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.0f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.0f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.5f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.5f);

			g_Player.bDisp = true;	//表示させる
		}
		break;
	}

	//プレイヤーが表示されているなら更新
	if (g_Player.bDisp == true)
	{
		//敵を取得
		pEnemy = GetEnemy();

		//背景の横が動くかどうか取得
		g_bBgX = GetBg();

		//前の位置を保存
		g_Player.posOld = g_Player.pos;

		//AとD同時押しは無効
		if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
		{

		}

		//Dキーが押された（右移動）
		else if (g_Player.nAttackTime == 0&&GetKeyboardPress(DIK_D))
		{
			g_nDirect++;

			//画面の端に行ったら動ける
			if (g_bBgX == false)
			{
				g_Player.pos.x += VALUE_MOVE;
			}
		}

		//Aキーが押された（左移動）
		else if (g_Player.nAttackTime == 0 && GetKeyboardPress(DIK_A))
		{
			g_nDirect--;
			
			//画面の端に行ったら動ける
			if (g_bBgX == false)
			{
				g_Player.pos.x -= VALUE_MOVE;
			}
		}

		//一段目のジャンプ処理
		if (GetKeyboardTrigger(DIK_SPACE)&& g_Player.bJump==false)
		{
			g_Player.move.y = 0;
			g_Player.move.y -= VALUE_JUMP;	//ジャンプ
			SetEffect2(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 20, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 110, EFFECTTYPE_003);
			PlaySound(SOUND_LABEL_SE_JUMP);	//ジャンプ音
			g_Player.bJump = true;	//一段目のジャンプをしている
			g_Player.bSecondJump = true;	//二段ジャンプを可能にする
		}

		//二段ジャンプ
		else if (g_Player.bSecondJump == true)
		{
			//ジャンプ処理
			if (GetKeyboardTrigger(DIK_SPACE))
			{
				g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
				g_Player.move.y -= VALUE_JUMP2;	//二段目のジャンプ
				SetEffect2(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_001);
				PlaySound(SOUND_LABEL_SE_JUMP);	//ジャンプ音
				g_Player.bSecondJump = false;	//二段ジャンプを終わらせる
			}
		}

		//スラッシュキーで攻撃
		if (g_Player.state == PLAYERSTATE_NORMAL && g_Player.nAttackTime == 0)	//通常時かつ攻撃のクールタイムがなくなったとき
		{
			//上攻撃
			if (GetKeyboardPress(DIK_W) && GetKeyboardTrigger(DIK_SLASH))
			{
				g_nAttackPatternAnim = 2;
				nAttack++;

				//剣
				if (g_Player.Wtype == WSELECT_SWORD)
				{
					if (g_Player.bLeft == false)	//右向き
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 25.5f, g_Player.pos.y - 15.5, 0.0f), D3DXVECTOR3(0, -10, 0), 19, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 56, DIRECT_UP);
						PlaySound(SOUND_LABEL_SE_SWORD);	//剣の音
					}
					if (g_Player.bLeft == true)	//左向き
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 25.5f, g_Player.pos.y - 15.5, 0.0f), D3DXVECTOR3(0, -10, 0), 19, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 56, DIRECT_UP);
						PlaySound(SOUND_LABEL_SE_SWORD);	//剣の音
					}
				}

				//槍
				if (g_Player.Wtype == WSELECT_LANCE)
				{
					if (g_Player.bLeft == false)	//右向き
					{
						//地上では一度上昇する
						if (g_Player.bJump == false)
						{
							g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
							g_Player.move.y -= VALUE_LANCEJUMP;	//ランスの上攻撃用のジャンプの高さ
							g_Player.bJump = true;	//ジャンプを消費
							g_Player.bSecondJump = true;	//二段ジャンプを可能にする
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 17.5f, g_Player.pos.y - 15.5, 0.0f), D3DXVECTOR3(0, -17.5, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 50, DIRECT_UP);
						}
						//空中
						else
						{
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y - 5.5, 0.0f), D3DXVECTOR3(0, -12, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 46, DIRECT_UP);
						}
						PlaySound(SOUND_LABEL_SE_SWORD);	//槍の音
					}
					if (g_Player.bLeft == true)	//左向き
					{
						//地上では一度上昇する
						if (g_Player.bJump == false)
						{
							g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
							g_Player.move.y -= VALUE_LANCEJUMP;//ランスの上攻撃用のジャンプの高さ
							g_Player.bJump = true;	//ジャンプを消費
							g_Player.bSecondJump = true;	//二段ジャンプを可能にする
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 17.5f, g_Player.pos.y - 15.5, 0.0f), D3DXVECTOR3(0, -17.5, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 50, DIRECT_UP);
						}
						//空中
						else
						{
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y - 5.5, 0.0f), D3DXVECTOR3(0, -10, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 46, DIRECT_UP);
						}
						PlaySound(SOUND_LABEL_SE_SWORD);	//槍の音
					}
				}

				//弓
				if (g_Player.Wtype == WSELECT_BOW)
				{
					if (g_Player.bLeft == false)	//右向き
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, -15.0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 58, DIRECT_UP);
						PlaySound(SOUND_LABEL_SE_SHOT);	//弓の音
					}
					if (g_Player.bLeft == true)	//左向き
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, -15.0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 58, DIRECT_UP);
						PlaySound(SOUND_LABEL_SE_SHOT);	//弓の音
					}
				}
			}

			//下攻撃
			else if (GetKeyboardPress(DIK_S) && GetKeyboardTrigger(DIK_SLASH))
			{
				//剣
				if (g_Player.Wtype == WSELECT_SWORD)
				{
					//空中のみ
					if (g_Player.bJump == true)
					{
						g_nAttackPatternAnim = 3;
						nAttack++;

						if (g_Player.bLeft == false)	//右向き
						{
							g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
							g_Player.move.y -= VALUE_SMALLJUMP;	//下攻撃の小ジャンプ
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 15.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, 10, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 60, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SWORD);	//剣の音
						}
						if (g_Player.bLeft == true)	//左向き
						{
							g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
							g_Player.move.y -= VALUE_SMALLJUMP;	//下攻撃の小ジャンプ
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 15.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, 10, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 60, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SWORD);	//剣の音
						}
					}
				}

				//槍
				if (g_Player.Wtype == WSELECT_LANCE)
				{
					//空中のみ
					if (g_Player.bJump == true)
					{
						g_nAttackPatternAnim = 3;
						nAttack++;

						if (g_Player.bLeft == false)	//右向き
						{
							g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
							g_Player.move.y += GRAVITY*3.5f;	//下攻撃の急降下
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 1.0f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, 20, 0), 23, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE2, 55, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SWORD);	//槍の音
						}
						if (g_Player.bLeft == true)	//左向き
						{
							g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
							g_Player.move.y += GRAVITY * 3.5f;	//下攻撃の急降下
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 1.0f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0, 20, 0), 23, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE2, 55, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SWORD);	//槍の音
						}
					}
				}

				//弓
				if (g_Player.Wtype == WSELECT_BOW)
				{
					//空中のみ
					if (g_Player.bJump == true)
					{
						g_nAttackPatternAnim = 3;
						nAttack++;

						if (g_Player.bLeft == false)	//右向き
						{
							g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
							g_Player.move.y -= VALUE_SMALLJUMP;	//下攻撃の小ジャンプ
							SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(11.0, 11.0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 58, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SHOT);	//弓の音
						}
						if (g_Player.bLeft == true)	//左向き
						{
							g_Player.move.y = 0;	//一度moveを0にして、二段目の高さを調節
							g_Player.move.y -= VALUE_SMALLJUMP;	//下攻撃の小ジャンプ
							SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-11.0, 11.0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW2, 58, DIRECT_DOWN);
							PlaySound(SOUND_LABEL_SE_SHOT);	//弓の音
						}
					}
				}
			}

			//左右攻撃
			else if (GetKeyboardTrigger(DIK_SLASH))
			{
				g_nAttackPatternAnim = 0;
				nAttack++;

				//剣
				if (g_Player.Wtype == WSELECT_SWORD)
				{
					if (g_Player.bLeft == false)	//右攻撃
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 25.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(10.0, 0, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 60, DIRECT_RIGHT);
						PlaySound(SOUND_LABEL_SE_SWORD);	//剣の音
					}
					if (g_Player.bLeft == true)	//左攻撃
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 25.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-10.0, 0, 0), 20, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_SWORD, 60, DIRECT_LEFT);
						PlaySound(SOUND_LABEL_SE_SWORD);	//剣の音
					}
				}

				//槍
				if (g_Player.Wtype == WSELECT_LANCE)
				{
					if (g_Player.bLeft == false)	//右攻撃
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 15.5f, 0.0f), D3DXVECTOR3(13.5, 0, 0), 21, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 60, DIRECT_RIGHT);
						PlaySound(SOUND_LABEL_SE_SWORD);	//槍の音
					}
					if (g_Player.bLeft == true)	//左攻撃
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 15.5f, 0.0f), D3DXVECTOR3(-13.5, 0, 0), 21, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_LANCE, 60, DIRECT_LEFT);
						PlaySound(SOUND_LABEL_SE_SWORD);	//槍の音
					}
				}

				//弓
				if (g_Player.Wtype == WSELECT_BOW)
				{
					if (g_Player.bLeft == false)	//右攻撃
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(15.0, 0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 60, DIRECT_RIGHT);
						PlaySound(SOUND_LABEL_SE_SHOT);	//弓の音
					}
					if (g_Player.bLeft == true)	//左攻撃
					{
						SetAttack(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-15.0, 0, 0), 24, D3DXCOLOR(255, 255, 255, 255), ATTACKTYPE_BOW, 60, DIRECT_LEFT);
						PlaySound(SOUND_LABEL_SE_SHOT);	//弓の音
					}
				}
			}

			//魔法攻撃
			else if (GetKeyboardTrigger(DIK_RETURN))
			{
				//炎
				if (g_Player.Mtype == MSELECT_FIRE)
				{
					if (g_Player.nMp >= FIRE_MP)	//MP消費25
					{
						//攻撃時のアニメーション
						g_nAttackPatternAnim = 4;
						nAttack++;

						g_Player.nMp -= FIRE_MP;
						if (g_Player.bLeft == false)	//右攻撃
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(12.0, -15.0, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(12.0, -16.5, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(12.0, -18.0, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
						}
						if (g_Player.bLeft == true)	//左攻撃
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-12.0, -15.0, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-12.0, -16.5, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 20.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(-12.0, -18.0, 0), 32, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_FIRE, 60);
						}
						PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
					}
				}

				//雷
				if (g_Player.Mtype == MSELECT_LIGHTNING)
				{
					if (g_Player.nMp >= LIGHTNING_MP)	//MP消費40
					{
						//攻撃時のアニメーション
						g_nAttackPatternAnim = 4;
						nAttack++;

						g_Player.nMp -= LIGHTNING_MP;
						if (g_Player.bLeft == false)	//右攻撃
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 285.5f, 0, 0.0f), D3DXVECTOR3(0.0, 19.5, 0), 37, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_LIGHTNING, 70);
						}
						if (g_Player.bLeft == true)	//左攻撃
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 285.5f, 0, 0.0f), D3DXVECTOR3(0.0, 19.5, 0), 37, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_LIGHTNING, 70);
						}
						PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
					}
				}

				//吸収
				if (g_Player.Mtype == MSELECT_DRAIN)
				{
					if (g_Player.nMp >= DRAIN_MP)	//MP消費
					{
						//攻撃時のアニメーション
						g_nAttackPatternAnim = 4;
						nAttack++;

						g_Player.nMp -= DRAIN_MP;
						if (g_Player.bLeft == false)	//右攻撃
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x + 225.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0.0, 0.0, 0), 37, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_DRAIN, 70);
						}
						if (g_Player.bLeft == true)	//左攻撃
						{
							SetMagic(D3DXVECTOR3(g_Player.pos.x - 225.5f, g_Player.pos.y + 5.5, 0.0f), D3DXVECTOR3(0.0, 0.0, 0), 37, D3DXCOLOR(255, 255, 255, 255), MAGICTYPE_DRAIN, 70);
						}
						PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
					}
				}
			}
		}

		//プレイヤーへの重力
		g_Player.move.y += GRAVITY;	//重力

		//位置更新
		if (g_ModePlayer == MODE_GAME3)	//ゲーム3でつららを使用
		{
			g_Player.pos.x += g_Player.move.x;	//つらら用
		}
		g_Player.pos.y += g_Player.move.y;

		//ゲーム1とゲーム3のときの左右の壁
		if (g_ModePlayer == MODE_GAME || g_ModePlayer == MODE_GAME3)
		{
			//左の壁
			if (g_Player.pos.x < PLAYER_SIZE - (PLAYER_SIZE / 2))
			{
				g_Player.pos.x = PLAYER_SIZE - (PLAYER_SIZE / 2);
			}

			//右の壁
			if (g_Player.pos.x > SCREEN_WIDTH - PLAYER_SIZE + (PLAYER_SIZE / 2))
			{
				g_Player.pos.x = SCREEN_WIDTH - PLAYER_SIZE + (PLAYER_SIZE / 2);
			}
		}

		//ゲーム2のときの左右の壁
		if (g_ModePlayer == MODE_GAME2)
		{
			//左の壁
			if (g_Player.pos.x < PLAYER_SIZE - (PLAYER_SIZE / 2) + 200)
			{
				g_Player.pos.x = PLAYER_SIZE - (PLAYER_SIZE / 2) + 200;
			}

			//右の壁
			if (g_Player.pos.x > SCREEN_WIDTH - PLAYER_SIZE + (PLAYER_SIZE / 2) -200)
			{
				g_Player.pos.x = SCREEN_WIDTH - PLAYER_SIZE + (PLAYER_SIZE / 2) -200;
			}
		}

		//地面にいるとき
		if (g_Player.pos.y > SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80)
		{
			//着地の煙
			if (g_Player.bJump == true)
			{
				SetEffect2(D3DXVECTOR3(g_Player.pos.x, g_Player.pos.y + 20, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 110, EFFECTTYPE_003);
			}

			g_Player.pos.y = SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80;

			g_Player.move.y = 0;	//地面にいるときは重力を与えない

								//ジャンプを復活
			g_Player.bJump = false;

			g_Player.bSecondJump = false;	//二段ジャンプを終わらせる

			//AとD同時押しは無効
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{
				g_nPatternAnimHunt = 0;
			}

			//AキーまたはDキーが押された（左右移動）
			else if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
			{
				//アニメーション
				g_nCounterAnimHunt++;	//カウンタ加算
				if (g_nCounterAnimHunt == 9)	//fps
				{
					g_nCounterAnimHunt = 0;
					g_nPatternAnimHunt++;
					if (g_nPatternAnimHunt == 4)//枚数
					{
						g_nPatternAnimHunt = 0;
					}
				}

				//ゲーム1(雪の平原)
				if (g_ModePlayer == MODE_GAME)
				{
					if (g_nSnowCD % SNOW_CD == 0 && g_Player.nAttackTime == 0)
					{
						PlaySound(SOUND_LABEL_SE_SNOW);	//雪の音
					}

					g_nSnowCD++;	//足音を一定間隔で鳴らすために加算

					if (g_nSnowCD >= SNOW_CD * 1000)//オーバーフロー防止
					{
						g_nSnowCD = 0;
					}
				}
			}

			//地面の上で立ち止まっているとき
			else
			{
				g_nPatternAnimHunt = 0;
			}

			//テクスチャ座標を更新
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.0f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.0f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.5f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.5f);
		}
		//空中にいるとき
		else
		{
			g_Player.bJump = true;		//ブロックから降りた時、2段ジャンプをしなくさせる

			//テクスチャ座標を更新
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(0.22f, 0.5f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(0.4f, 0.5f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(0.22f, 1.0f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(0.4f, 1.0f);
		}

		// ブロックの上にいるとき
		if (CollisionBlock(&g_Player.pos, &g_Player.posOld, PLAYER_SIZE) == true)
		{
			g_Player.move.y = 0;
			g_Player.bJump = false;		//ジャンプを復活
			g_Player.bSecondJump = true;		//二段ジャンプを復活

			//つららの上にいるとき
			if (g_ModePlayer == MODE_GAME3)
			{
				//左右キーを押すと滑る
				if (GetKeyboardPress(DIK_A))//左向き
				{
					g_Player.move.x -= 0.25;
				}

				//左右キーを押すと滑る
				if (GetKeyboardPress(DIK_D))//右向き
				{
					g_Player.move.x += 0.25;
				}

				//滑る上限
				if (g_Player.move.x > 2.5)
				{
					g_Player.move.x = 2.5;
				}

				//滑る下限
				if (g_Player.move.x < -2.5)
				{
					g_Player.move.x = -2.5;
				}
			}

			//AとD同時押しは無効
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{
				g_nPatternAnimHunt = 0;
			}

			//AキーまたはDキーが押された（左右移動）
			else if (GetKeyboardPress(DIK_A) || GetKeyboardPress(DIK_D))
			{
				//アニメーション
				g_nCounterAnimHunt++;	//カウンタ加算
				if (g_nCounterAnimHunt == 9)	//fps
				{
					g_nCounterAnimHunt = 0;
					g_nPatternAnimHunt++;
					if (g_nPatternAnimHunt == 4)//枚数
					{
						g_nPatternAnimHunt = 0;
					}
				}

				//if (g_nSnowCD % SNOW_CD == 0)
				//{
				//	PlaySound(SOUND_LABEL_SE_SNOW);	//雪の音
				//}
			}

			//地面の上で立ち止まっているとき
			else
			{
				g_nPatternAnimHunt = 0;
			}

			//テクスチャ座標を更新
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.0f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.0f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f, 0.5f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.2f + 0.2f, 0.5f);
		}
		else
		{
			//ゲーム3
			if (g_ModePlayer == MODE_GAME3)
			{
				//つららの上にいないときは、滑らない
				g_Player.move.x = 0;
			}
		}

		//左向きに変える
		if (g_nDirect < 0)
		{
			g_nDirect = 0;
			g_nChange = 1;
			g_Player.bLeft = true;
		}

		//右向きに変える
		if (g_nDirect > 0)
		{
			g_nDirect = 0;
			g_nChange = 0;
			g_Player.bLeft = false;
		}

		//攻撃のアニメーション
		if (nAttack > 0)
		{
			nAttack = 0;
			g_Player.nAttackTime = ATTACK_CD;	//次の攻撃ができるまでの時間
		}

		//自機が敵に当たった時の判定
		for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				//自機がノーマル状態なら
				if (g_Player.state == PLAYERSTATE_NORMAL)
				{

					//自機がビートルに当たった時の当たり判定
					if (pEnemy->type == ENEMYTYPE_000)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
						{
							//自機にダメージ
							HitPlayer(7);
						}
					}

					//自機がカラスに当たった時の当たり判定
					if (pEnemy->type == ENEMYTYPE_001)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
						{
							//自機にダメージ
							HitPlayer(15);
						}
					}

					//自機がカラス亜種に当たった時の当たり判定
					if (pEnemy->type == ENEMYTYPE_002)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_METⅡ / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_METⅡ / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_METⅡ / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_METⅡ / 2))
						{
							//自機にダメージ
							HitPlayer(20);
						}
					}

					//自機がガイコツに当たった時の当たり判定
					if (pEnemy->type == ENEMYTYPE_003 || pEnemy->type == ENEMYTYPE_008)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
						{
							//自機にダメージ
							HitPlayer(17);
						}
					}

					//自機がイノシシに当たった時の当たり判定
					if (pEnemy->type == ENEMYTYPE_004)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
						{
							//自機にダメージ
							HitPlayer(22);
						}
					}

					//自機がコウモリに当たった時の当たり判定
					if (pEnemy->type == ENEMYTYPE_005)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
						{
							//自機にダメージ
							HitPlayer(5);
						}
					}

					//自機がアイスドラゴンに当たった時の当たり判定
					if (pEnemy->type == ENEMYTYPE_006)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_BOSS_X / 2) - 70 &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_BOSS_X / 2) + 70 &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_BOSS_Y / 2) + 80)
						{
							//左向きなら左に飛ばす
							if (pEnemy->bLeft == true)
							{
								g_Player.pos.x -= 50.0f;
							}
							//右向きなら右に飛ばす
							if (pEnemy->bLeft == false)
							{
								g_Player.pos.x += 50.0f;
							}
							//自機にダメージ
							HitPlayer(25);
						}
					}

					//自機がピラニアに当たった時の当たり判定
					if (pEnemy->type == ENEMYTYPE_007)
					{
						if (g_Player.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_PIR / 2) &&
							g_Player.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_PIR / 2) &&
							g_Player.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_PIR / 2) &&
							g_Player.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_PIR / 2))
						{
							//自機にダメージ
							HitPlayer(21);
						}
					}
				}
			}
		}

		//テクスチャを攻撃用に更新
		if (g_Player.nAttackTime > 0)
		{
			g_Player.nAttackTime--;
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(g_nAttackPatternAnim * 0.2f, 0.5f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(g_nAttackPatternAnim * 0.2f + 0.2f, 0.5f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(g_nAttackPatternAnim * 0.2f, 1.0f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(g_nAttackPatternAnim * 0.2f + 0.2f, 1.0f);
		}

		//テクスチャをダメージ状態に更新
		if (g_Player.nCounterState > 0)
		{
			//テクスチャ座標を更新
			pVtx[0 + g_nChange].tex = D3DXVECTOR2(0.8f, 0.0f);
			pVtx[1 - g_nChange].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2 + g_nChange].tex = D3DXVECTOR2(0.8f, 0.5f);
			pVtx[3 - g_nChange].tex = D3DXVECTOR2(1.0f, 0.5f);
		}

		//塔の頂
		if (g_ModePlayer == MODE_GAME3)
		{
			//上の画面外にいったらゲームオーバー
			if (g_Player.pos.y < -325)
			{
				//何度もエフェクトがでないようにする
				if (g_bEndPlayer == false)
				{
					g_Player.bDisp = false;
					SetEffect2(D3DXVECTOR3(SCREEN_WIDTH/2, SCREEN_HEIGHT/4, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_HEIGHT / 2, EFFECTTYPE_004);
					g_bEndPlayer = true;
				}
			}
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);
	}

	//頂点データをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// プレイヤー描画処理
//=============================================================================
void DrawPlayer(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//プレイヤーが表示されているなら描画
	if (g_Player.bDisp == true)
	{
		if (g_Player.Wtype == WSELECT_SWORD)	//剣
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePolygon[0]);
		}

		if (g_Player.Wtype == WSELECT_LANCE)	//槍
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePolygon[1]);
		}

		if (g_Player.Wtype == WSELECT_BOW)		//弓
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTexturePolygon[2]);
		}

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// プレイヤーの設定処理
//=============================================================================
void SetPlayer(D3DXVECTOR3 pos)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤーが表示されていないかつ、ストックがある状態のときに設定可能
	if (g_Player.bDisp == false && g_Player.nStock >-1)
	{
		g_Player.bDisp = true;
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y - (PLAYER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Player.pos.x - (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Player.pos.x + (PLAYER_SIZE / 2), g_Player.pos.y + (PLAYER_SIZE / 2), 0.0f);

		//位置を設定
		g_Player.pos = pos;
		g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//プレイヤーの体力
		g_Player.nLife = PLAYER_LIFE;
		g_Player.nMp = PLAYER_MP;
	}

	//羽が生えるときのカラーを通常の色に戻す
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点データをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// プレイヤーの取得
//=============================================================================
PLAYER*GetPlayer(void)
{
	return &g_Player;
}

//=============================================================================
// プレイヤーのダメージ設定
//=============================================================================
void HitPlayer(int nDamage)
{
	//宣言
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//背景へ吸い込まれたときは、きらーん
	if (nDamage == 1000)
	{
		PlaySound(SOUND_LABEL_SE_STAR);	//きら-ん
	}
	else
	{
		PlaySound(SOUND_LABEL_SE_EXPLOSION);	//爆発音
	}

											//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	g_Player.nLife -= nDamage;
	if (g_Player.nLife <= 0)
	{
		//背景へ吸い込まれたときは、爆発をださない
		if (nDamage == 1000)
		{

		}
		else
		{
			//爆発生成
			SetExplosion(g_Player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE);
		}

		//プレイヤーを未使用に
		g_Player.bDisp = false;

		//頂点カラーを設定（0～255の値で設定する）
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//設置後に、自機の総数を減らす
		g_Player.nStock--;
	}

	else
	{
		//ダメージ状態に遷移
		g_Player.state = PLAYERSTATE_DAMAGE;
		g_Player.nCounterState = INVINCIBLE_FRAME;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	//頂点データをアンロックする
	g_pVtxBuffPolygon->Unlock();

	//ゲームオーバー
	if (g_Player.nStock < 0)
	{
		g_Player.bDisp = false;

		//ゲーム1(雪の平原)
		if (g_ModePlayer == MODE_GAME)
		{
			SetGameState(GAMESTATE_GAMEOVER);
		}

		//ゲーム2(塔内部)
		if (g_ModePlayer == MODE_GAME2)
		{
			SetGameState2(GAMESTATE2_GAMEOVER);
		}

		//ゲーム3(塔の頂)
		if (g_ModePlayer == MODE_GAME3)
		{
			SetGameState3(GAMESTATE3_GAMEOVER);
		}
	}
}

//=============================================================================
// プレイヤーのダメージ設定2
//=============================================================================
bool HitPlayer2(void)
{
	return g_bHitPlayer;
}
