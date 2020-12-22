//=============================================================================
//
// X-BOW HUNTERの処理 [bar.cpp] (HPバー)
// Author :後藤慎之助
//
//=============================================================================
#include "bar.h"
#include "hunter.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBar[2] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBar = NULL;

//=============================================================================
//HPバー初期化処理
//=============================================================================
void InitBar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER*pHunter = GetHunter();	//ハンターのHPを取得

									//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "bar000.png", &g_pTextureBar[0]);
	D3DXCreateTextureFromFile(pDevice, "bar003.png", &g_pTextureBar[1]);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBar, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//HPが減った場合、画像の右側から描画をやめる
	pVtx[0].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(20 + 320 * ((pHunter->nLife / HUNTER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(20 + 320 * ((pHunter->nLife / HUNTER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);

	//HPバーの背景
	pVtx[4].pos = D3DXVECTOR3(20.0f - BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585.0f - BAR_EXTEND, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(20.0f + 320.0f + BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585.0f - BAR_EXTEND, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(20.0f - BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585.0f + BAR_EXTEND, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(20.0f + 320.0f + BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585.0f + BAR_EXTEND, 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;

	//頂点カラーを設定（0～255の値で設定する）
	pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pHunter->nLife) * (1 / HUNTER_LIFE), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pHunter->nLife) * (1 / HUNTER_LIFE), 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffBar->Unlock();
}

//=============================================================================
// HPバー終了処理
//=============================================================================
void UninitBar(void)
{
	//テクスチャの開放
	if (g_pTextureBar != NULL)
	{
		for (int nCntBar = 0; nCntBar < 2; nCntBar++)
		{
			g_pTextureBar[nCntBar]->Release();
			g_pTextureBar[nCntBar] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBar != NULL)
	{
		g_pVtxBuffBar->Release();
		g_pVtxBuffBar = NULL;
	}
}

//=============================================================================
// HPバー更新処理
//=============================================================================
void UpdateBar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	HUNTER*pHunter = GetHunter();	//ハンターのHPを取得

									//頂点バッファをロック
	g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);

	//HPが減った場合、画像の右側から描画をやめる
	if (pHunter->bDisp == true)
	{
		pVtx[0].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20 + 320 * ((pHunter->nLife / HUNTER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20 + 320 * ((pHunter->nLife / HUNTER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
	}

	//HPがなくなった場合、画像を表示させない
	if (pHunter->bDisp == false)
	{
		pVtx[0].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20 + 320 * (0), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(20, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20 + 320 * (0), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 585, 0.0f);
	}


	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pHunter->nLife / HUNTER_LIFE), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pHunter->nLife / HUNTER_LIFE), 1.0f);

	//HPに応じてカラーを変える（満タンから半分まで緑）
	if (pHunter->nLife <= HUNTER_LIFE&&pHunter->nLife > HUNTER_LIFE / 2)
	{
		pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	}

	//HPに応じてカラーを変える（半分から4分の1まで黄色）
	if (pHunter->nLife <= HUNTER_LIFE / 2 && pHunter->nLife > HUNTER_LIFE / 4)
	{
		pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);
	}

	//HPに応じてカラーを変える（4分の1からは赤色）
	if (pHunter->nLife <= HUNTER_LIFE / 4)
	{
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	//頂点データをアンロックする
	g_pVtxBuffBar->Unlock();

}

//=============================================================================
// HPバー描画処理
//=============================================================================
void DrawBar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER*pHunter = GetHunter();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBar, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBar = 1; nCntBar >-1; nCntBar--)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBar[nCntBar]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBar, NUM_POLYGON);
	}

}