//=============================================================================
//
// フェード関係の処理 [fade.cpp]
// Author:後藤慎之助
//
//=============================================================================
#include "fade.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;//頂点バッファへのポインタ

FADE g_Fade;			//フェード状態
MODE g_modeNext;		//次の画面
D3DXCOLOR g_colorFade;	//フェード色

//=============================================================================
// 初期化処理
//=============================================================================
void InitFade(MODE modeNext)
{
	g_Fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D *pVtx;//頂点情報のポインタ

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/fade000.png", &g_pTextureFade);	//スコアの背景

					//オブジェクトの頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&g_pVtxBuffFade,//変数名が変わると変更が必要
		NULL);
	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定（右回りで設定する）
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データアンロック
	g_pVtxBuffFade->Unlock();

}
//=============================================================================
//終了処理
//=============================================================================
void UninitFade(void)
{
	//テクスチャの開放
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
	//頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	VERTEX_2D *pVtx;//頂点情報のポインタ

	if (g_Fade != FADE_NONE)
	{//フェード処理中の時
		if (g_Fade == FADE_IN)
		{//フェードイン
			g_colorFade.a -= FADE_RATE;
			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_Fade = FADE_NONE;

				//SetMode(g_modeNext);
			}
		}

		if (g_Fade == FADE_OUT)
		{//フェード
			g_colorFade.a += FADE_RATE;
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_Fade = FADE_IN;

				SetMode(g_modeNext);//モードの設定
			}
		}
	}
	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
	//頂点情報(カラーのみ更新)
	pVtx[0].col = g_colorFade;
	pVtx[1].col = g_colorFade;
	pVtx[2].col = g_colorFade;
	pVtx[3].col = g_colorFade;
	//頂点データアンロック
	g_pVtxBuffFade->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawFade(void)
{
	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

}
//=============================================================================
//フェードの設定
//=============================================================================
void SetFade(MODE modeNext)
{
	g_Fade = FADE_OUT;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
}
//=============================================================================
//フェード状態の取得
//=============================================================================
FADE GetFade(void)
{
	return g_Fade;
}