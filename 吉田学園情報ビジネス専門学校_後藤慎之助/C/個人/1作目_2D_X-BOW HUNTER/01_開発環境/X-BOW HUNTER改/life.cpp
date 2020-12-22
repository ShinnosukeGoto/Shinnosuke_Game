//=============================================================================
//
// X-BOW HUNTERの処理 [life.cpp] (ライフ)
// Author :後藤慎之助
//
//=============================================================================
#include "life.h"
#include "hunter.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureLife = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffLife = NULL;

//=============================================================================
//ライフ初期化処理
//=============================================================================
void InitLife(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER*pHunter = GetHunter();	//ハンターのストックを取得

									//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "life001.png", &g_pTextureLife);


	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffLife, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//ストックが減った場合、画像の右側から描画をやめる
	pVtx[0].pos = D3DXVECTOR3(55, (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) - (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(55 + 320 * (0.25*(pHunter->nStock + 1)), (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) - (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(55, (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) + (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(55 + 320 * (0.25*(pHunter->nStock + 1)), (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) + (LIFE_SIZE_Y / 2) - 625, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2((pHunter->nStock + 1) * 0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pHunter->nStock + 1) * 0.25f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// ライフ終了処理
//=============================================================================
void UninitLife(void)
{
	//テクスチャの開放
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
// ライフ更新処理
//=============================================================================
void UpdateLife(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	HUNTER*pHunter = GetHunter();	//ハンターのストックを取得

									//頂点バッファをロック
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	//ストックが減った場合、画像の右側から描画をやめる
	pVtx[0].pos = D3DXVECTOR3(55, (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) - (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(55 + 320 * (0.25*(pHunter->nStock + 1)), (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) - (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(55, (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) + (LIFE_SIZE_Y / 2) - 625, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(55 + 320 * (0.25*(pHunter->nStock + 1)), (SCREEN_HEIGHT - LIFE_SIZE_Y / 2) + (LIFE_SIZE_Y / 2) - 625, 0.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pHunter->nStock + 1) * 0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pHunter->nStock + 1) * 0.25f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
// ライフ描画処理
//=============================================================================
void DrawLife(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HUNTER*pHunter = GetHunter();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}