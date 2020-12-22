//=============================================================================
//
// X-BOW HUNTERの処理 [wing.cpp] (ボス戦時の翼)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "wing.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureWing = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffWing = NULL;
int g_nCounterAnimWing;			//アニメーションカウンター
int g_nPatternAnimWing;			//アニメーションパターン

WING g_Wing;	//ウィングの情報

				//=============================================================================
				//ウィング初期化処理
				//=============================================================================
void InitWing(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nCounterAnimWing = 0;
	g_nPatternAnimWing = 0;

	//ウィング情報の初期化
	g_Wing.bUse = false;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "wing000.png", &g_pTextureWing);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffWing, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffWing->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//頂点座標の設定（右回りで設定する）
	pVtx[0].pos = D3DXVECTOR3(0.0f - (WING_SIZE / 2), 0.0f - (WING_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f + (WING_SIZE / 2), 0.0f - (WING_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f - (WING_SIZE / 2), 0.0f + (WING_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f + (WING_SIZE / 2), 0.0f + (WING_SIZE / 2), 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーを設定（0～255の値で設定する）
	pVtx[0].col = D3DCOLOR_RGBA(0, 166, 248, 200);
	pVtx[1].col = D3DCOLOR_RGBA(0, 166, 248, 200);
	pVtx[2].col = D3DCOLOR_RGBA(0, 166, 248, 200);
	pVtx[3].col = D3DCOLOR_RGBA(0, 166, 248, 200);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f + 0.33f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f + 0.33f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffWing->Unlock();
}

//=============================================================================
// ウィング終了処理
//=============================================================================
void UninitWing(void)
{
	//テクスチャの開放
	if (g_pTextureWing != NULL)
	{
		g_pTextureWing->Release();
		g_pTextureWing = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffWing != NULL)
	{
		g_pVtxBuffWing->Release();
		g_pVtxBuffWing = NULL;
	}
}

//=============================================================================
// ウィング更新処理
//=============================================================================
void UpdateWing(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	HUNTER *pHunter;

	pHunter = GetHunter();

	//羽が使われているかつハンターが生存しているなら更新
	if (g_Wing.bUse == true && pHunter->bDisp == true)
	{
		g_nCounterAnimWing++;	//カウンタ加算
		if (g_nCounterAnimWing == 7)
		{
			g_nCounterAnimWing = 0;
			g_nPatternAnimWing++;
			if (g_nPatternAnimWing == 3)
			{
				g_nPatternAnimWing = 0;
			}
		}

		//頂点バッファをロック
		g_pVtxBuffWing->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標を更新
		pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f + 0.34f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimWing * 0.33f + 0.34f, 1.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pHunter->pos.x - (WING_SIZE / 2) - 20, pHunter->pos.y - (WING_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pHunter->pos.x + (WING_SIZE / 2) - 20, pHunter->pos.y - (WING_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pHunter->pos.x - (WING_SIZE / 2) - 20, pHunter->pos.y + (WING_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pHunter->pos.x + (WING_SIZE / 2) - 20, pHunter->pos.y + (WING_SIZE / 2), 0.0f);

		//自機がピンチ状態かつストック0で羽が取れる
		if (pHunter->nLife > 0 && pHunter->nLife < HUNTER_LIFE / 4 && pHunter->nStock == 0)
		{
			g_Wing.bUse = false;
		}

		//頂点データをアンロックする
		g_pVtxBuffWing->Unlock();
	}
}

//=============================================================================
// ウィング描画処理
//=============================================================================
void DrawWing(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER *pHunter;

	pHunter = GetHunter();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffWing, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Wing.bUse == true && pHunter->bDisp == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWing);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}


//=============================================================================
// ウィングの設定
//=============================================================================
void SetWing(void)
{
	if (g_Wing.bUse == false)
	{
		g_Wing.bUse = true;
	}
}

//=============================================================================
// ウィングの取得
//=============================================================================
WING*GetWing(void)
{
	return &g_Wing;
}