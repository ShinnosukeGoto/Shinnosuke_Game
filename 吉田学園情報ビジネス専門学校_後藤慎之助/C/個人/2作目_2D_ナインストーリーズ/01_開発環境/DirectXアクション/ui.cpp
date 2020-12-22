//=============================================================================
//
// ナインストーリーズ [ui.cpp]の処理 (ゲーム画面のUI)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "ui.h"
#include "title.h"
#include "select.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonUI[11] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonUI = NULL;

GAMELEVEL g_nUILevel;	//レベルの情報
WSELECT g_nUIWtype;		//武器選択
MSELECT g_nUIMtype;		//魔法選択

//=============================================================================
//UI初期化処理
//=============================================================================
void InitUI(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nUILevel = GetTitle();		//難易度を取得
	g_nUIWtype = GetWSelect();		//武器選択を取得
	g_nUIMtype = GetMSelect();		//魔法選択を取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/score000.png", &g_pTexturePolygonUI[0]);	//スコアの背景
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/time000.png", &g_pTexturePolygonUI[1]);		//タイムの背景
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/hpmp000.png", &g_pTexturePolygonUI[2]);		//HP,MPバーの背景
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI000.png", &g_pTexturePolygonUI[3]);		//剣
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI001.png", &g_pTexturePolygonUI[4]);		//槍
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI002.png", &g_pTexturePolygonUI[5]);		//弓
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI003.png", &g_pTexturePolygonUI[6]);		//炎
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI004.png", &g_pTexturePolygonUI[7]);		//雷
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/UI005.png", &g_pTexturePolygonUI[8]);		//ドレイン
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/NORMAL000.png", &g_pTexturePolygonUI[9]);	//NORMAL
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/HARD000.png", &g_pTexturePolygonUI[10]);	//HARD

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 6, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonUI, NULL);	//スコア、タイム、バー、武器、魔法、難易度
	VERTEX_2D *pVtxUI;	//バックグラウンドの頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtxUI, 0);	//この書式は変えないこと

	//スコアの座標設定
	pVtxUI[0].pos = D3DXVECTOR3(900.0f, 20.0f, 0.0f);
	pVtxUI[1].pos = D3DXVECTOR3(1260.0f, 20.0f, 0.0f);
	pVtxUI[2].pos = D3DXVECTOR3(900.0f, 80.0f, 0.0f);
	pVtxUI[3].pos = D3DXVECTOR3(1260.0f, 80.0f, 0.0f);

	//タイムの座標設定
	pVtxUI[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - 75, 0.0f, 0.0f);
	pVtxUI[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + 75, 0.0f, 0.0f);
	pVtxUI[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - 75, 110.0f, 0.0f);
	pVtxUI[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + 75, 110.0f, 0.0f);

	//HPMPの座標設定
	pVtxUI[8].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
	pVtxUI[9].pos = D3DXVECTOR3(70.0f, 20.0f, 0.0f);
	pVtxUI[10].pos = D3DXVECTOR3(20.0f, 90.0f, 0.0f);
	pVtxUI[11].pos = D3DXVECTOR3(70.0f, 90.0f, 0.0f);

	//武器選択の座標設定
	pVtxUI[12].pos = D3DXVECTOR3(70.0f, 90.0f, 0.0f);
	pVtxUI[13].pos = D3DXVECTOR3(160.0f, 90.0f, 0.0f);
	pVtxUI[14].pos = D3DXVECTOR3(70.0f, 130.0f, 0.0f);
	pVtxUI[15].pos = D3DXVECTOR3(160.0f, 130.0f, 0.0f);

	//魔法選択の座標設定
	pVtxUI[16].pos = D3DXVECTOR3(165.0f, 90.0f, 0.0f);
	pVtxUI[17].pos = D3DXVECTOR3(365.0f, 90.0f, 0.0f);
	pVtxUI[18].pos = D3DXVECTOR3(165.0f, 130.0f, 0.0f);
	pVtxUI[19].pos = D3DXVECTOR3(365.0f, 130.0f, 0.0f);

	//難易度選択の座標設定
	pVtxUI[20].pos = D3DXVECTOR3(180.0f, 0.0f, 0.0f);
	pVtxUI[21].pos = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
	pVtxUI[22].pos = D3DXVECTOR3(180.0f, 30.0f, 0.0f);
	pVtxUI[23].pos = D3DXVECTOR3(280.0f, 30.0f, 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtxUI[0].rhw = 1.0f;
	pVtxUI[1].rhw = 1.0f;
	pVtxUI[2].rhw = 1.0f;
	pVtxUI[3].rhw = 1.0f;
	pVtxUI[4].rhw = 1.0f;
	pVtxUI[5].rhw = 1.0f;
	pVtxUI[6].rhw = 1.0f;
	pVtxUI[7].rhw = 1.0f;
	pVtxUI[8].rhw = 1.0f;
	pVtxUI[9].rhw = 1.0f;
	pVtxUI[10].rhw = 1.0f;
	pVtxUI[11].rhw = 1.0f;
	pVtxUI[12].rhw = 1.0f;
	pVtxUI[13].rhw = 1.0f;
	pVtxUI[14].rhw = 1.0f;
	pVtxUI[15].rhw = 1.0f;
	pVtxUI[16].rhw = 1.0f;
	pVtxUI[17].rhw = 1.0f;
	pVtxUI[18].rhw = 1.0f;
	pVtxUI[19].rhw = 1.0f;
	pVtxUI[20].rhw = 1.0f;
	pVtxUI[21].rhw = 1.0f;
	pVtxUI[22].rhw = 1.0f;
	pVtxUI[23].rhw = 1.0f;

	//頂点カラーを設定（0～255の値で設定する）
	pVtxUI[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[20].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[21].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[22].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxUI[23].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtxUI[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[11].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[15].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[19].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtxUI[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtxUI[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtxUI[22].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtxUI[23].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffPolygonUI->Unlock();
}

//=============================================================================
// UI終了処理
//=============================================================================
void UninitUI(void)
{
	//テクスチャの開放
	for (int nCntUI = 0; nCntUI < 11; nCntUI++)
	{
		if (g_pTexturePolygonUI[nCntUI] != NULL)
		{
			g_pTexturePolygonUI[nCntUI]->Release();
			g_pTexturePolygonUI[nCntUI] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffPolygonUI != NULL)
	{
		g_pVtxBuffPolygonUI->Release();
		g_pVtxBuffPolygonUI = NULL;
	}
}

//=============================================================================
// UI更新処理
//=============================================================================
void UpdateUI(void)
{
	
}

//=============================================================================
// UI描画処理
//=============================================================================
void DrawUI(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonUI, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//スコア、タイム、バー
	for (int nCntUI = 0; nCntUI < 3; nCntUI++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[nCntUI]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUI, NUM_POLYGON);
	}

	//武器
	if (g_nUIWtype == WSELECT_SWORD)	//剣
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[3]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	}

	if (g_nUIWtype == WSELECT_LANCE)	//槍
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[4]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	}

	if (g_nUIWtype == WSELECT_BOW)		//弓
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[5]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, NUM_POLYGON);
	}

	//魔法
	if (g_nUIMtype == MSELECT_FIRE)			//炎
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[6]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	}

	if (g_nUIMtype == MSELECT_LIGHTNING)	//雷
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[7]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	}

	if (g_nUIMtype == MSELECT_DRAIN)		//吸収
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[8]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, NUM_POLYGON);
	}

	//難易度
	if (g_nUILevel == GAMELEVEL_NORMAL)		//NORMAL
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[9]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);
	}

	if (g_nUILevel == GAMELEVEL_HARD)	//HARD
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonUI[10]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, NUM_POLYGON);
	}
}
