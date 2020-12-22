//=============================================================================
//
// X-Bow Hunter [arrow.cpp]の処理（プレイヤーの矢）
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "arrow.h"
#include "explosion.h"
#include "enemy.h"
#include "boss.h"
#include "storm.h"
#include "input.h"
#include "item.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureArrow[ARROWTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffArrow = NULL;
ARROW g_aArrow[MAX_ARROW];	//矢の情報
int g_nUseArrow;			//矢を使った回数

							//=============================================================================
							//矢の初期化処理
							//=============================================================================
void InitArrow(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntArrow;

	//弾の情報の初期化
	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		g_aArrow[nCntArrow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aArrow[nCntArrow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aArrow[nCntArrow].bUse = false;
	}

	g_nUseArrow = 0;	//使った回数の初期化

						//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "arrow.png", &g_pTextureArrow[0]);
	D3DXCreateTextureFromFile(pDevice, "arrow200.png", &g_pTextureArrow[1]);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ARROW, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffArrow, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//弾の数分
	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
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

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データをアンロックする
	g_pVtxBuffArrow->Unlock();
}

//=============================================================================
// 矢の終了処理
//=============================================================================
void UninitArrow(void)
{
	//変数宣言
	int nCntArrow;

	for (nCntArrow = 0; nCntArrow < ARROWTYPE_MAX; nCntArrow++)
	{
		//テクスチャの開放
		if (g_pTextureArrow != NULL)
		{
			g_pTextureArrow[nCntArrow]->Release();
			g_pTextureArrow[nCntArrow] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffArrow != NULL)
	{
		g_pVtxBuffArrow->Release();
		g_pVtxBuffArrow = NULL;
	}
}

//=============================================================================
// 矢の更新処理
//=============================================================================
void UpdateArrow(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntArrow;
	VERTEX_2D *pVtx;
	int nDirect = 0;	//頂点付近で向きを変えるために使用
	ENEMY*pEnemy;
	int nCntEnemy;
	BOSS*pBoss;
	int nCntBoss;
	STORM*pStorm;

	//頂点バッファをロック
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == true)
		{
			//敵を取得
			pEnemy = GetEnemy();
			//ボスを取得
			pBoss = GetBoss();
			//ストームを取得
			pStorm = GetStorm();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true) //敵が使用されている
				{
					//モスキートの当たり判定
					if (pEnemy->type == ENEMYTYPE_000)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
						{
							//敵にダメージ
							HitEnemy(nCntEnemy++, 10);

							//当たった時点で弾のHPを0にする
							g_aArrow[nCntArrow].nLife = 0;
						}
					}

					//メタルバードの当たり判定
					if (pEnemy->type == ENEMYTYPE_001)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
						{
							//敵にダメージ
							HitEnemy(nCntEnemy++, 10);

							//当たった時点で弾のHPを0にする
							g_aArrow[nCntArrow].nLife = 0;
						}
					}

					//メタルバード亜種の当たり判定
					if (pEnemy->type == ENEMYTYPE_002)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_METⅡ / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_METⅡ / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_METⅡ / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_METⅡ / 2))
						{
							//敵にダメージ
							HitEnemy(nCntEnemy++, 10);

							//当たった時点で弾のHPを0にする
							g_aArrow[nCntArrow].nLife = 0;
						}
					}

					//オクトパスの当たり判定
					if (pEnemy->type == ENEMYTYPE_003)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
						{
							//敵にダメージ
							HitEnemy(nCntEnemy++, 10);

							//当たった時点で弾のHPを0にする
							g_aArrow[nCntArrow].nLife = 0;
						}
					}

					//ゴーストの当たり判定
					if (pEnemy->type == ENEMYTYPE_004)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) - 10 &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) + 10 &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) - 10 &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2) + 10)
						{
							if (GetKeyboardPress(DIK_A))	//恥ずかしがり状態でないときに当たり判定が出現
							{
								//敵にダメージ
								HitEnemy(nCntEnemy++, 10);

								//当たった時点で弾のHPを0にする
								g_aArrow[nCntArrow].nLife = 0;
							}
						}
					}

					//グソクムシの当たり判定
					if (pEnemy->type == ENEMYTYPE_005)
					{
						if (g_aArrow[nCntArrow].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
							g_aArrow[nCntArrow].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
							g_aArrow[nCntArrow].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
							g_aArrow[nCntArrow].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2) - 10)
						{
							//敵にダメージ
							HitEnemy(nCntEnemy++, 10);

							//当たった時点で弾のHPを0にする
							g_aArrow[nCntArrow].nLife = 0;
						}
					}
				}
			}


			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				if (pBoss->bUse == true) //ボスが使用されている
				{
					//敵の当たり判定
					if (g_aArrow[nCntArrow].pos.x <= pBoss->pos.x + (BOSS_SIZE / 2) - 150 &&
						g_aArrow[nCntArrow].pos.x >= pBoss->pos.x - (BOSS_SIZE / 2) + 80 &&
						g_aArrow[nCntArrow].pos.y <= pBoss->pos.y + (BOSS_SIZE / 2) - 150 &&
						g_aArrow[nCntArrow].pos.y >= pBoss->pos.y - (BOSS_SIZE / 2) + 100)
					{
						//敵にダメージ
						HitBoss(nCntBoss++, 10);

						//当たった時点で弾のHPを0にする
						g_aArrow[nCntArrow].nLife = 0;
					}

					//矢がストームに当たった時の処理
					if (pStorm->bUse == true)
					{
						//弾とストームの当たり判定
						if (g_aArrow[nCntArrow].pos.x <= pStorm->pos.x + (pStorm->nSize / 2) + 85 &&
							g_aArrow[nCntArrow].pos.x >= pStorm->pos.x - (pStorm->nSize / 2) - 85 &&
							g_aArrow[nCntArrow].pos.y <= pStorm->pos.y + (pStorm->nSize / 2) + 85 &&
							g_aArrow[nCntArrow].pos.y >= pStorm->pos.y - (pStorm->nSize / 2) - 85)
						{
							//当たった時点で弾のHPを0にする
							g_aArrow[nCntArrow].nLife = 0;
						}
					}
				}
			}

			//矢の移動
			g_aArrow[nCntArrow].pos += g_aArrow[nCntArrow].move;

			//頂点の座標設定
			pVtx[0].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x - (g_aArrow[nCntArrow].nSize / 2), g_aArrow[nCntArrow].pos.y - (g_aArrow[nCntArrow].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x + (g_aArrow[nCntArrow].nSize / 2), g_aArrow[nCntArrow].pos.y - (g_aArrow[nCntArrow].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x - (g_aArrow[nCntArrow].nSize / 2), g_aArrow[nCntArrow].pos.y + (g_aArrow[nCntArrow].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x + (g_aArrow[nCntArrow].nSize / 2), g_aArrow[nCntArrow].pos.y + (g_aArrow[nCntArrow].nSize / 2), 0.0f);

			//曲射時の矢に対する重力
			if (g_aArrow[nCntArrow].type == ARROWTYPE_BEND)
			{
				g_aArrow[nCntArrow].move.y += BOW_MOVE;
				g_aArrow[nCntArrow].pos.y += g_aArrow[nCntArrow].move.y;	//位置更新
				g_aArrow[nCntArrow].move.y += (0.0 - g_aArrow[nCntArrow].move.x)*RATE_MOVE;
			}

			//矢の体力を減らす
			g_aArrow[nCntArrow].nLife--;

			//曲射時に矢の体力を参照して、頂点付近で向きを変える
			if (g_aArrow[nCntArrow].type == ARROWTYPE_BEND)
			{
				if (g_aArrow[nCntArrow].nLife < 33)	//矢の体力66の半分
				{
					//テクスチャ座標を更新(右下向き)
					pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
				}
				else
				{
					//テクスチャ座標を更新(右上向き)
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
			}

			//剛射時はテクスチャの向きを固定する
			if (g_aArrow[nCntArrow].type == ARROWTYPE_STRAIGHT)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//矢の体力が0
			if (g_aArrow[nCntArrow].nLife <= 0)
			{
				//爆発生成
				SetExplosion(g_aArrow[nCntArrow].pos, D3DXCOLOR(255, 255, 255, 255), g_aArrow[nCntArrow].nSize);
				g_aArrow[nCntArrow].bUse = false;
			}
		}

		//画面を出たら未使用に
		if (g_aArrow[nCntArrow].pos.x < 0 - (g_aArrow[nCntArrow].nSize / 2) || g_aArrow[nCntArrow].pos.x > SCREEN_WIDTH + (g_aArrow[nCntArrow].nSize / 2))
		{
			g_aArrow[MAX_ARROW].bUse = false;
		}
		if (g_aArrow[nCntArrow].pos.y < 0 - (g_aArrow[nCntArrow].nSize / 2) || g_aArrow[nCntArrow].pos.y > SCREEN_WIDTH + (g_aArrow[nCntArrow].nSize / 2))
		{
			g_aArrow[MAX_ARROW].bUse = false;
		}

		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffArrow->Unlock();

}

//=============================================================================
// 矢の描画処理
//=============================================================================
void DrawArrow(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntArrow;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffArrow, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//矢の描画
	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureArrow[g_aArrow[nCntArrow].type]);

			//矢が使用中なら描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntArrow * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// 矢の設定処理
//=============================================================================
void SetArrow(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, ARROWTYPE type, int nSize)
{
	//変数宣言
	int nCntArrow;
	VERTEX_2D *pVtx;
	ITEM*pItem = GetItem();

	//アイテムが落ちていないときに撃った矢の数を加算
	if (pItem->bUse == false)
	{
		g_nUseArrow++;
	}

	//頂点バッファをロック
	g_pVtxBuffArrow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == false)
		{
			//位置を設定
			g_aArrow[nCntArrow].pos = pos;
			g_aArrow[nCntArrow].nSize = nSize;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x - (nSize / 2), g_aArrow[nCntArrow].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x + (nSize / 2), g_aArrow[nCntArrow].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x - (nSize / 2), g_aArrow[nCntArrow].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aArrow[nCntArrow].pos.x + (nSize / 2), g_aArrow[nCntArrow].pos.y + (nSize / 2), 0.0f);

			//頂点カラーを設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aArrow[nCntArrow].move = move;
			g_aArrow[nCntArrow].nLife = nLife;
			g_aArrow[nCntArrow].col = col;
			g_aArrow[nCntArrow].type = type;
			g_aArrow[nCntArrow].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffArrow->Unlock();
}
//=============================================================================
//矢の取得
//=============================================================================
ARROW*GetArrow(void)
{
	return &g_aArrow[0];
}

//=============================================================================
//矢の使用回数の取得
//=============================================================================
int GetUseArrow(void)
{
	return g_nUseArrow;
}