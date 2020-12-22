//=============================================================================
//
// X-BOWHUNTER の処理 [storm.cpp] (ボスのストーム)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "bullet.h"
#include "explosion.h"
#include "boss.h"
#include "storm.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureStorm = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffStorm = NULL;
STORM g_aStorm[MAX_STORM];//ストームの情報

						  //=============================================================================
						  //ストーム初期化処理
						  //=============================================================================
void InitStorm(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStorm;

	//ストームの情報の初期化
	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		g_aStorm[nCntStorm].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aStorm[nCntStorm].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aStorm[nCntStorm].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "bullet000.png", &g_pTextureStorm);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_STORM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffStorm, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffStorm->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//ストームの数分
	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
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
	g_pVtxBuffStorm->Unlock();
}

//=============================================================================
// ストーム終了処理
//=============================================================================
void UninitStorm(void)
{
	//テクスチャの開放
	if (g_pTextureStorm != NULL)
	{
		g_pTextureStorm->Release();
		g_pTextureStorm = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffStorm != NULL)
	{
		g_pVtxBuffStorm->Release();
		g_pVtxBuffStorm = NULL;
	}
}

//=============================================================================
// ストーム更新処理
//=============================================================================
void UpdateStorm(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStorm;
	VERTEX_2D *pVtx;
	HUNTER*pHunter;

	//頂点バッファをロック
	g_pVtxBuffStorm->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		if (g_aStorm[nCntStorm].bUse == true)
		{
			//自機を取得
			pHunter = GetHunter();

			if (pHunter->bDisp == true)
			{
				//自機の当たり判定
				if (g_aStorm[nCntStorm].pos.x <= pHunter->pos.x + (HUNTER_SIZE / 2) &&
					g_aStorm[nCntStorm].pos.x >= pHunter->pos.x - (HUNTER_SIZE / 2) &&
					g_aStorm[nCntStorm].pos.y <= pHunter->pos.y + (HUNTER_SIZE / 2) &&
					g_aStorm[nCntStorm].pos.y >= pHunter->pos.y - (HUNTER_SIZE / 2))
				{
					//自機にダメージ
					HitHunter(10);
					SetExplosion(g_aStorm[nCntStorm].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 25);
					//当たった時点で弾のHPを0にする
					g_aStorm[nCntStorm].nLife = 0;
				}
			}

		}

		//ストームの移動
		g_aStorm[nCntStorm].pos += g_aStorm[nCntStorm].move;

		//ボスの弾が当たった地面のX軸からの、左右の移動範囲
		if (g_aStorm[nCntStorm].pos.x < g_aStorm[nCntStorm].nHitPosX - 13 || g_aStorm[nCntStorm].pos.x > g_aStorm[nCntStorm].nHitPosX + 13)
		{
			g_aStorm[nCntStorm].move.x *= -1;
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x - (g_aStorm[nCntStorm].nSize / 2), g_aStorm[nCntStorm].pos.y - (g_aStorm[nCntStorm].nSize / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x + (g_aStorm[nCntStorm].nSize / 2), g_aStorm[nCntStorm].pos.y - (g_aStorm[nCntStorm].nSize / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x - (g_aStorm[nCntStorm].nSize / 2), g_aStorm[nCntStorm].pos.y + (g_aStorm[nCntStorm].nSize / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x + (g_aStorm[nCntStorm].nSize / 2), g_aStorm[nCntStorm].pos.y + (g_aStorm[nCntStorm].nSize / 2), 0.0f);

		//左に弾が出ていった時
		if (g_aStorm[nCntStorm].pos.x < g_aStorm[nCntStorm].nSize - (g_aStorm[nCntStorm].nSize / 2))
		{
			g_aStorm[nCntStorm].bUse = false;
		}

		//右に弾が出ていった時
		if (g_aStorm[nCntStorm].pos.x > SCREEN_WIDTH - g_aStorm[nCntStorm].nSize + (g_aStorm[nCntStorm].nSize / 2))
		{
			g_aStorm[nCntStorm].bUse = false;
		}

		//上に弾が出ていった時
		if (g_aStorm[nCntStorm].pos.y < g_aStorm[nCntStorm].nSize - (g_aStorm[nCntStorm].nSize / 2))
		{
			g_aStorm[nCntStorm].bUse = false;
		}

		//弾の射程を決める
		g_aStorm[nCntStorm].nLife--;
		if (g_aStorm[nCntStorm].nLife <= 0)
		{
			g_aStorm[nCntStorm].bUse = false;	//使用しない状態にする
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffStorm->Unlock();

}

//=============================================================================
// ストーム描画処理
//=============================================================================
void DrawStorm(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntStorm;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffStorm, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureStorm);

	//ポリゴンの描画
	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		if (g_aStorm[nCntStorm].bUse == true)			//弾が使用中なら描画
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStorm * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// ストームの設定処理
//=============================================================================
void SetStorm(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, int nHitPosX, int nSize)
{
	//変数宣言
	int nCntStorm;
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffStorm->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntStorm = 0; nCntStorm < MAX_STORM; nCntStorm++)
	{
		if (g_aStorm[nCntStorm].bUse == false)
		{
			//位置を設定
			g_aStorm[nCntStorm].pos = pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x - (nSize / 2), g_aStorm[nCntStorm].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x + (nSize / 2), g_aStorm[nCntStorm].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x - (nSize / 2), g_aStorm[nCntStorm].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStorm[nCntStorm].pos.x + (nSize / 2), g_aStorm[nCntStorm].pos.y + (nSize / 2), 0.0f);

			g_aStorm[nCntStorm].move = move;
			g_aStorm[nCntStorm].nLife = nLife;
			g_aStorm[nCntStorm].col = col;
			g_aStorm[nCntStorm].nHitPosX = nHitPosX;
			g_aStorm[nCntStorm].nSize = nSize;
			g_aStorm[nCntStorm].bUse = true;

			pVtx[0].col = g_aStorm[nCntStorm].col;
			pVtx[1].col = g_aStorm[nCntStorm].col;
			pVtx[2].col = g_aStorm[nCntStorm].col;
			pVtx[3].col = g_aStorm[nCntStorm].col;
			break;
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffStorm->Unlock();
}

//=============================================================================
// ストームの取得
//=============================================================================
STORM*GetStorm(void)
{
	return &g_aStorm[0];
}