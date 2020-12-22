//=============================================================================
//
// X-Bow Hunterの処理 [item.cpp] (アイテム)
// Author :後藤慎之助
//
//=============================================================================
#include "hunter.h"
#include "item.h"
#include "game.h"
#include "score.h"
#include "boss.h"
#include "wing.h"
#include "sound.h"
#include "time.h"
#include "input.h"
#include "arrow.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureItem[ITEMTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffItem = NULL;

int g_nCounterAnimItem;			//アニメーションカウンター
int g_nPatternAnimItem;			//アニメーションパターン
int g_nTimeScore;				//残り時間に対応したスコア
int g_nCntClick;				//矢を打った回数をカウント

ITEM g_aItem[MAX_ITEM];			//アイテムの情報
								//=============================================================================
								//マクロ定義
								//=============================================================================
#define ITEM_MOVE (1.18f)	//アイテムの移動速度

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
		g_aItem[nCntItem].nType = 0;
		g_aItem[nCntItem].bUse = false;
	}
	g_nCounterAnimItem = 0;
	g_nPatternAnimItem = 0;
	g_nTimeScore = 0;
	g_nCntClick = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "item400.png", &g_apTextureItem[0]);		//青い羽根
	D3DXCreateTextureFromFile(pDevice, "item200.png", &g_apTextureItem[1]);		//黄金の羽

																				//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ITEM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffItem, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

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
		if (g_apTextureItem != NULL)
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
	HUNTER*pHunter;

	//頂点バッファをロック
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_aItem[nCntItem].bUse == true)
		{
			g_nCounterAnimItem++;	//カウンタ加算
			if (g_nCounterAnimItem == 15)//速さ
			{
				g_nCounterAnimItem = 0;
				g_nPatternAnimItem++;
				if (g_nPatternAnimItem == 2)//枚数
				{
					g_nPatternAnimItem = 0;
				}
			}

			//ハンターを取得
			pHunter = GetHunter();

			if (pHunter->bDisp == true)
			{
				//自機がアイテムに当たった時の当たり判定
				if (g_aItem[nCntItem].pos.x <= pHunter->pos.x + (ITEM_SIZE / 2) &&
					g_aItem[nCntItem].pos.x >= pHunter->pos.x - (ITEM_SIZE / 2) &&
					g_aItem[nCntItem].pos.y <= pHunter->pos.y + (ITEM_SIZE / 2) &&
					g_aItem[nCntItem].pos.y >= pHunter->pos.y - (ITEM_SIZE / 2))
				{
					//青い羽根
					if (g_aItem[nCntItem].nType == ITEMTYPE_000)
					{
						//未使用に
						g_aItem[nCntItem].bUse = false;

						PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音

						ScoreEffect(5000);

						//ピンチ状態かつストック0（羽が生えない状態)で、ボス戦に入るときに、羽が生えるために、ライフを増やす
						if (pHunter->nLife > 0 && pHunter->nLife < HUNTER_LIFE / 4 && pHunter->nStock == 0)
						{
							pHunter->nLife = 100;
						}
						SetWing();	//羽をはやす

									//アイテムをとることでボスが出現
						SetBoss(D3DXVECTOR3(940, 240, 0.0f), D3DXVECTOR3(-1, -1, 0), 0);
					}

					//黄金の羽
					if (g_aItem[nCntItem].nType == ITEMTYPE_001)
					{
						//未使用に
						g_aItem[nCntItem].bUse = false;

						//残り時間を取得
						g_nTimeScore = GetTime();

						//矢の使用回数を取得
						g_nCntClick = GetUseArrow();

						PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音

														//スコアに、自機の残り体力と、残り時間と、残りストックに応じたものを加え、攻撃回数分引く
						ScoreEffect(pHunter->nLife * 40 + g_nTimeScore * 100 + pHunter->nStock * 10000 + g_nCntClick*(-1));

						//ボス戦で羽が取れているときに、羽を生やしてゲームクリアの見栄えをよくする
						if (pHunter->nLife > 0 && pHunter->nLife < HUNTER_LIFE / 4 && pHunter->nStock == 0)
						{
							pHunter->nLife = 100;	//羽が生えるために、ライフを増やす
							SetWing();
						}

						//ゲームクリアへ
						SetGameState(GAMESTATE_END);
					}
				}
			}
			//アイテムに対する重力
			g_aItem[nCntItem].move.y += ITEM_MOVE;
			g_aItem[nCntItem].pos.y += g_aItem[nCntItem].move.y;	//位置更新
			g_aItem[nCntItem].move.y += (0.0 - g_aItem[nCntItem].move.x)*RATE_MOVE;

			//地面にあるとき
			if (g_aItem[nCntItem].pos.y > SCREEN_HEIGHT - ITEM_SIZE + (ITEM_SIZE / 2) - 80)
			{
				g_aItem[nCntItem].pos.y = SCREEN_HEIGHT - ITEM_SIZE + (ITEM_SIZE / 2) - 80;
				g_aItem[nCntItem].move.y = 0;	//地面にあるときは重力を与えない
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y + (ITEM_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y + (ITEM_SIZE / 2), 0.0f);

			//テクスチャ座標を更新
			pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimItem * 0.5f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimItem * 0.5f + 0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimItem * 0.5f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimItem * 0.5f + 0.5f, 1.0f);
		}
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
			pDevice->SetTexture(0, g_apTextureItem[g_aItem[nCntItem].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
// アイテムの設定処理
//=============================================================================
void SetItem(D3DXVECTOR3 pos, ITEMTYPE nType)
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
			g_aItem[nCntItem].nType = nType;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y - (ITEM_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x - (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y + (ITEM_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aItem[nCntItem].pos.x + (ITEM_SIZE / 2), g_aItem[nCntItem].pos.y + (ITEM_SIZE / 2), 0.0f);

			break;
		}
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