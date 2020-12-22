//=============================================================================
//
// ナインストーリーズ [effect2.cpp]の処理 (回復や二段ジャンプなどのエフェクト）
// Author :後藤慎之助
//
//=============================================================================
#include "effect2.h"
#include "block.h"
#include "player.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureEffect2[EFFECTTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEffect2 = NULL;
EFFECT2 g_aEffect2[MAX_EFFECT2];						//エフェクト2の情報

//=============================================================================
//エフェクト2初期化処理
//=============================================================================
void InitEffect2(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//エフェクト2情報の初期化
	for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		g_aEffect2[nCntEffect2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect2[nCntEffect2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect2[nCntEffect2].nCounterAnim = 0;
		g_aEffect2[nCntEffect2].nPatternAnim = 0;
		g_aEffect2[nCntEffect2].nSize = 0;
		g_aEffect2[nCntEffect2].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect000.png", &g_pTextureEffect2[0]);	//回復
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect001.png", &g_pTextureEffect2[1]);	//二段ジャンプ
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect002.png", &g_pTextureEffect2[2]);	//召喚
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect003.png", &g_pTextureEffect2[3]);	//着地の煙
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect004.png", &g_pTextureEffect2[4]);	//自機が背景へ吸い込まれる

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_EFFECT2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEffect2, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

	//エフェクト2のpVtxを生成
	for (int nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		//頂点座標の設定（右回りで設定する）
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定（値は1.0で設定する）
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーを設定（0～255の値で設定する）
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.125f + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.125f + 0.125f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データをアンロックする
	g_pVtxBuffEffect2->Unlock();
}

//=============================================================================
// エフェクト2終了処理
//=============================================================================
void UninitEffect2(void)
{
	//テクスチャの開放
	for (int nCntEffect2 = 0; nCntEffect2 < EFFECTTYPE_MAX; nCntEffect2++)
	{
		if (g_pTextureEffect2[nCntEffect2] != NULL)
		{
			g_pTextureEffect2[nCntEffect2] -> Release();
			g_pTextureEffect2[nCntEffect2] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffEffect2 != NULL)
	{
		g_pVtxBuffEffect2->Release();
		g_pVtxBuffEffect2 = NULL;
	}
}

//=============================================================================
// エフェクト2更新処理
//=============================================================================
void UpdateEffect2(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect2;
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		if (g_aEffect2[nCntEffect2].bUse == true)
		{
			//回復
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_000)
			{
				//エフェクト2のカウンターを進めて、パターンを切り替える
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 2)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 1.0f);

				//パターンが最後まで終わった
				if (g_aEffect2[nCntEffect2].nPatternAnim > 5)
				{
					g_aEffect2[nCntEffect2].bUse = false;
				}
			}

			//二段ジャンプ
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_001)
			{
				//エフェクト2のカウンターを進めて、パターンを切り替える
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 2)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 1.0f);

				//パターンが最後まで終わった
				if (g_aEffect2[nCntEffect2].nPatternAnim > 5)
				{
					g_aEffect2[nCntEffect2].bUse = false;
				}
			}

			//召喚
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_002)
			{
				//エフェクト2のカウンターを進めて、パターンを切り替える
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 3)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.1f + 0.1f, 1.0f);

				//途中でつららを召喚
				if (g_aEffect2[nCntEffect2].nPatternAnim == 60)
				{
					SetBlock(D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x, g_aEffect2[nCntEffect2].pos.y + 90,0.0f), 85, 170, BLOCKTYPE_006, 160);	//つらら(HP80)
				}

				//パターンが最後まで終わった
				if (g_aEffect2[nCntEffect2].nPatternAnim > 60)
				{
					g_aEffect2[nCntEffect2].col.a -= 0.02;

					//透明度が0.1を下回ったら未使用に
					if (g_aEffect2[nCntEffect2].col.a <= 0.1)
					{
						g_aEffect2[nCntEffect2].bUse = false;
					}
				}

				//頂点カラーの更新
				pVtx[0].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, g_aEffect2[nCntEffect2].col.a);
				pVtx[1].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, g_aEffect2[nCntEffect2].col.a);
				pVtx[2].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, g_aEffect2[nCntEffect2].col.a);
				pVtx[3].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, g_aEffect2[nCntEffect2].col.a);
			}

			//着地の煙
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_003)
			{
				//エフェクト2のカウンターを進めて、パターンを切り替える
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 2)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.2f + 0.2f, 1.0f);

				//パターンが最後まで終わった
				if (g_aEffect2[nCntEffect2].nPatternAnim > 4)
				{
					g_aEffect2[nCntEffect2].bUse = false;
				}
			}

			//自機が背景へ吸い込まれる
			if (g_aEffect2[nCntEffect2].type == EFFECTTYPE_004)
			{
				//エフェクト2のカウンターを進めて、パターンを切り替える
				g_aEffect2[nCntEffect2].nCounterAnim++;
				if (g_aEffect2[nCntEffect2].nCounterAnim == 7)
				{
					g_aEffect2[nCntEffect2].nCounterAnim = 0;
					g_aEffect2[nCntEffect2].nPatternAnim++;
				}

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.0625f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.0625f + 0.0625f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.0625f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEffect2[nCntEffect2].nPatternAnim*0.0625f + 0.0625f, 1.0f);

				//頂点座標
				pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, 0, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, 0, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2, 0.0f);

				//頂点カラーの更新
				pVtx[0].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, 1.0f);
				pVtx[1].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, 1.0f);
				pVtx[2].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, 1.0f);
				pVtx[3].col = D3DXCOLOR(g_aEffect2[nCntEffect2].col.r, g_aEffect2[nCntEffect2].col.g, g_aEffect2[nCntEffect2].col.b, 1.0f);

				//パターンが最後まで終わった
				if (g_aEffect2[nCntEffect2].nPatternAnim > 15)
				{
					g_aEffect2[nCntEffect2].bUse = false;
					HitPlayer(1000);
				}
			}
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffEffect2->Unlock();
}

//=============================================================================
// エフェクト2描画処理
//=============================================================================
void DrawEffect2(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect2;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEffect2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		if (g_aEffect2[nCntEffect2].bUse == true)	//エフェクト2が使用中なら描画
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect2[g_aEffect2[nCntEffect2].type]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect2 * 4, 2);
		}
	}
}

//=============================================================================
// エフェクト2設定処理
//=============================================================================
void SetEffect2(D3DXVECTOR3 pos, D3DXCOLOR col, int nSize, EFFECTTYPE type)
{
	//変数宣言
	int nCntEffect2;
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect2->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect2 = 0; nCntEffect2 < MAX_EFFECT2; nCntEffect2++)
	{
		if (g_aEffect2[nCntEffect2].bUse == false)
		{
			g_aEffect2[nCntEffect2].pos = pos;
			g_aEffect2[nCntEffect2].nCounterAnim = 0;
			g_aEffect2[nCntEffect2].nPatternAnim = 0;
			g_aEffect2[nCntEffect2].nSize = nSize;
			g_aEffect2[nCntEffect2].type = type;

			//頂点情報の更新
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x - (g_aEffect2[nCntEffect2].nSize / 2), g_aEffect2[nCntEffect2].pos.y - (g_aEffect2[nCntEffect2].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x + (g_aEffect2[nCntEffect2].nSize / 2), g_aEffect2[nCntEffect2].pos.y - (g_aEffect2[nCntEffect2].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x - (g_aEffect2[nCntEffect2].nSize / 2), g_aEffect2[nCntEffect2].pos.y + (g_aEffect2[nCntEffect2].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect2[nCntEffect2].pos.x + (g_aEffect2[nCntEffect2].nSize / 2), g_aEffect2[nCntEffect2].pos.y + (g_aEffect2[nCntEffect2].nSize / 2), 0.0f);

			g_aEffect2[nCntEffect2].col = col;

			g_aEffect2[nCntEffect2].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffEffect2->Unlock();
}