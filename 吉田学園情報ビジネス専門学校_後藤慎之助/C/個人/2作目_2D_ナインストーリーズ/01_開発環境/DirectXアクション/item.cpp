//=============================================================================
//
// ナインストーリーズの処理 [item.cpp] (アイテム)
// Author :後藤慎之助
//
//=============================================================================
#include "player.h"
#include "item.h"
#include "game.h"
#include "score.h"
#include "sound.h"
#include "time.h"
#include "input.h"
#include "attack.h"
#include "bg.h"
#include "block.h"
#include "magic.h"
#include "explosion.h"
#include "effect2.h"
#include "fade.h"
#include "game3.h"
#include "enemy.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureItem[ITEMTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffItem = NULL;

int g_nTimeScore;				//残り時間に対応したスコア
int g_nCntClick;				//矢を打った回数をカウント

ITEM g_aItem[MAX_ITEM];			//アイテムの情報
bool g_bBgXI;					//ステージ1の横が動くかどうか
bool g_bBlockXI;				//ブロックに横から当たった
MODE g_ModeItem;				//ゲーム画面に対応

//=============================================================================
//アイテム初期化処理
//=============================================================================
void InitItem(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntItem;

	//アイテムの情報の初期化
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_aItem[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aItem[nCntItem].type = ITEMTYPE_000;
		g_aItem[nCntItem].nSize = 0;
		g_aItem[nCntItem].bUse = false;
	}

	//グローバル変数の初期化
	g_nTimeScore = 0;
	g_nCntClick = 0;
	g_bBgXI = false;
	g_bBlockXI = false;
	g_ModeItem = MODE_GAME;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item000.png", &g_apTextureItem[0]);		//壺
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item001.png", &g_apTextureItem[1]);		//ダイヤモンド
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item002.png", &g_apTextureItem[2]);		//樽
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item003.png", &g_apTextureItem[3]);		//ハート
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item004.png", &g_apTextureItem[4]);		//つららの姫
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/item005.png", &g_apTextureItem[5]);		//姫

																				//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ITEM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffItem, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//頂点座標の設定（右回りで設定する）
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// アイテム終了処理
//=============================================================================
void UninitItem(void)
{
	//テクスチャの開放
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		if (g_apTextureItem[nCntItem] != NULL)
		{
			g_apTextureItem[nCntItem]->Release();
			g_apTextureItem[nCntItem] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=============================================================================
// アイテム更新処理
//=============================================================================
void UpdateItem(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	PLAYER*pPlayer;
	ATTACK*pAttack;
	MAGIC*pMagic;
	//ゲームのステージを取得
	g_ModeItem = GetMode();
	ENEMY*pEnemy = GetEnemy();
	int nFade = GetFade();	//フェードの情報を獲得

	//頂点バッファをロック
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			//自機を取得
			pPlayer = GetPlayer();
			//攻撃を取得
			pAttack = GetAttack();
			//魔法を取得
			pMagic = GetMagic();
			//背景の横が動くかどうか取得
			g_bBgXI = GetBg();
			//ブロックに横から当たったかどうか取得
			g_bBlockXI = GetBlockX();

			if (pPlayer->bDisp == true)
			{
				//壺
				if (g_aItem[nCntItem].type == ITEMTYPE_000)
				{
					//攻撃がアイテムに当たった時の当たり判定
					if (pAttack->bUse == true)
					{
						if (g_aItem[nCntItem].pos.x <= pAttack->pos.x + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.x >= pAttack->pos.x - (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y <= pAttack->pos.y + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y >= pAttack->pos.y - (g_aItem[nCntItem].nSize / 2))
						{
							//爆発生成
							SetExplosion(g_aItem[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aItem[nCntItem].nSize);

							//未使用に
							g_aItem[nCntItem].bUse = false;

							SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 5.0f, 0.0f), ITEMTYPE_001, 60);	//壺が壊れたらダイヤモンドが出てくる
						}
					}

					//魔法がアイテムに当たった時の当たり判定
					if (pMagic->bUse == true)
					{
						if (g_aItem[nCntItem].pos.x <= pMagic->pos.x + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.x >= pMagic->pos.x - (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y <= pMagic->pos.y + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y >= pMagic->pos.y - (g_aItem[nCntItem].nSize / 2))
						{
							//爆発生成
							SetExplosion(g_aItem[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aItem[nCntItem].nSize);

							//未使用に
							g_aItem[nCntItem].bUse = false;

							SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 5.0f, 0.0f), ITEMTYPE_001, 60);	//壺が壊れたらダイヤモンドが出てくる
						}
					}
				}

				//樽
				if (g_aItem[nCntItem].type == ITEMTYPE_002)
				{
					//攻撃がアイテムに当たった時の当たり判定
					if (pAttack->bUse == true)
					{
						if (g_aItem[nCntItem].pos.x <= pAttack->pos.x + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.x >= pAttack->pos.x - (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y <= pAttack->pos.y + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y >= pAttack->pos.y - (g_aItem[nCntItem].nSize / 2))
						{
							//爆発生成
							SetExplosion(g_aItem[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aItem[nCntItem].nSize);

							//未使用に
							g_aItem[nCntItem].bUse = false;

							SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, 0.0f), ITEMTYPE_003, 50);	//樽が壊れたらハートが出てくる
						}
					}

					//魔法がアイテムに当たった時の当たり判定
					if (pMagic->bUse == true)
					{
						if (g_aItem[nCntItem].pos.x <= pMagic->pos.x + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.x >= pMagic->pos.x - (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y <= pMagic->pos.y + (g_aItem[nCntItem].nSize / 2) &&
							g_aItem[nCntItem].pos.y >= pMagic->pos.y - (g_aItem[nCntItem].nSize / 2))
						{
							//爆発生成
							SetExplosion(g_aItem[nCntItem].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), g_aItem[nCntItem].nSize);

							//未使用に
							g_aItem[nCntItem].bUse = false;

							SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y + 10.0f, 0.0f), ITEMTYPE_003, 50);	//壺が壊れたらハートが出てくる
						}
					}
				}

				//つららの姫
				if (g_aItem[nCntItem].type == ITEMTYPE_004)
				{
					//敵を倒したら、つららから姫が出てくる
					if (pEnemy->bUse == false)
					{
						//未使用に
						g_aItem[nCntItem].bUse = false;
						SetItem(D3DXVECTOR3(g_aItem[nCntItem].pos.x, g_aItem[nCntItem].pos.y, 0.0f), ITEMTYPE_005, PLAYER_SIZE);
					}
				}

				//姫
				if (g_aItem[nCntItem].type == ITEMTYPE_005)
				{
					//下にゆっくり移動
					g_aItem[nCntItem].pos.y += 1.0;

					//リフトが止まるから、アイテムも止まる
					if (g_aItem[nCntItem].pos.y > SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80)
					{
						g_aItem[nCntItem].pos.y = SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80;
					}
				}

				//自機がアイテムに当たった時の当たり判定
				if (g_aItem[nCntItem].pos.x <= pPlayer->pos.x + (g_aItem[nCntItem].nSize / 2) &&
					g_aItem[nCntItem].pos.x >= pPlayer->pos.x - (g_aItem[nCntItem].nSize / 2) &&
					g_aItem[nCntItem].pos.y <= pPlayer->pos.y + (g_aItem[nCntItem].nSize / 2) &&
					g_aItem[nCntItem].pos.y >= pPlayer->pos.y - (g_aItem[nCntItem].nSize / 2))
				{
					//ダイヤモンド
					if (g_aItem[nCntItem].type == ITEMTYPE_001)
					{
						//未使用に
						g_aItem[nCntItem].bUse = false;

						PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音

						ScoreEffect(4600);

					}

					//ハート
					if (g_aItem[nCntItem].type == ITEMTYPE_003)
					{
						//未使用に
						g_aItem[nCntItem].bUse = false;

						//回復エフェクト
						SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE,EFFECTTYPE_000);

						//回復
						pPlayer->nLife += 100;
						pPlayer->nMp += 50;

						PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音

						ScoreEffect(500);

					}

					//姫
					if (g_aItem[nCntItem].type == ITEMTYPE_005 && nFade == FADE_NONE)
					{
						//残り時間を取得
						g_nTimeScore = GetTime();

						//矢の使用回数を取得
						g_nCntClick = GetUseAttack();

						PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音

						//スコアに、自機の残り体力と、残り時間と、残りストックに応じたものを加え、攻撃回数分引く
						ScoreEffect(pPlayer->nLife * 30 + g_nTimeScore * 100 + pPlayer->nStock * 10000 + g_nCntClick*(-1));

						//ゲームクリアへ
						SetGameState3(GAMESTATE3_GAMECLEAR);
					}
				}
			}

			//プレイヤーの操作に対応した移動（ゲーム1）
			if (g_ModeItem == MODE_GAME && pPlayer->bDisp == true && g_bBgXI == true && g_bBlockXI == false)
			{
				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
				{
					g_aItem[nCntItem].pos.x -= VALUE_MOVE;
				}

				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
				{
					g_aItem[nCntItem].pos.x += VALUE_MOVE;
				}
			}

			//ゲーム2で、足場においてあるアイテム
			if (g_ModeItem == MODE_GAME2)
			{
				//リフトに乗っているから、アイテムは下に移動
				g_aItem[nCntItem].pos.y += 1.0;

				//リフトが止まるから、アイテムも止まる
				if (g_aItem[nCntItem].pos.y > 435)
				{
					g_aItem[nCntItem].pos.y = 435;
				}
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y - (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y - (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y + (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y + (g_aItem[nCntItem].nSize / 2), 0.0f);

		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// アイテム描画処理
//=============================================================================
void DrawItem(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntItem;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)//アイテムが使用中なら描画
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureItem[g_aItem[nCntItem].type]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4* nCntItem, NUM_POLYGON);
		}
	}
}

//=============================================================================
// アイテムの設定処理
//=============================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE type, int nSize)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntItem;

	//頂点バッファをロック
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == false)
		{
			g_aItem[nCntItem].pos = pos;
			g_aItem[nCntItem].bUse = true;
			g_aItem[nCntItem].type = type;
			g_aItem[nCntItem].nSize = nSize;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y - (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y - (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y + (g_aItem[nCntItem].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (g_aItem[nCntItem].nSize / 2), g_aItem[nCntItem].pos.y + (g_aItem[nCntItem].nSize / 2), 0.0f);

			break;
		}
		pVtx += 4;
	}
	//頂点データをアンロックする
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
//アイテムの取得
//=============================================================================
ITEM*GetItem(void)
{
	return &g_aItem[0];
}