//=============================================================================
//
// X-Bow Hunter [explosion.cpp]の処理 (爆発）
// Author :後藤慎之助
//
//=============================================================================
#include "explosion.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_EXPLOSION (128)	//爆発の最大数

//=============================================================================
//構造体定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureExplosion = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffExplosion = NULL;
EXPLOSION g_aExplosion[MAX_EXPLOSION];						//爆発の情報

															//=============================================================================
															//爆発初期化処理
															//=============================================================================
void InitExplosion(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntExplosion;

	//爆発情報の初期化
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "explosion000.png", &g_pTextureExplosion);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_EXPLOSION, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

														//矢の数分爆発を生成
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
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
		pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 爆発終了処理
//=============================================================================
void UninitExplosion(void)
{
	//テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 爆発更新処理
//=============================================================================
void UpdateExplosion(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntExplosion;
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			//爆発のカウンターを進めて、パターンを切り替える
			g_aExplosion[nCntExplosion].nCounterAnim++;
			if (g_aExplosion[nCntExplosion].nCounterAnim == 1)	//爆発の速さ4
			{
				g_aExplosion[nCntExplosion].nCounterAnim = 0;
				g_aExplosion[nCntExplosion].nPatternAnim++;
			}

			//テクスチャ座標を更新
			pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 0.0f);

			//パターンが最後まで終わった
			if (g_aExplosion[nCntExplosion].nPatternAnim > 8)
			{
				g_aExplosion[nCntExplosion].bUse = false;
			}
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 爆発描画処理
//=============================================================================
void DrawExplosion(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntExplosion;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ポリゴンの描画
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)	//爆発が使用中なら描画
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}
}

//=============================================================================
// 爆発設定処理
//=============================================================================
bool SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int nSize)
{
	//変数宣言
	int nCntExplosion;
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			g_aExplosion[nCntExplosion].col = col;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;

			//頂点情報の更新
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - (nSize / 2), g_aExplosion[nCntExplosion].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + (nSize / 2), g_aExplosion[nCntExplosion].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x - (nSize / 2), g_aExplosion[nCntExplosion].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].pos.x + (nSize / 2), g_aExplosion[nCntExplosion].pos.y + (nSize / 2), 0.0f);
			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim*0.125f + 0.125f, 0.0f);

			break;
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffExplosion->Unlock();
	return(true);
}