//=============================================================================
//
// ナインストーリーズの処理 [score.cpp] (スコア)
// Author:後藤慎之助
//
//=============================================================================
#include "score.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//頂点バッファへのポインタ

int g_nScore;//スコアの情報

			 //=============================================================================
			 // 初期化処理
			 //=============================================================================
void InitScore(void)
{
	//変数宣言
	VERTEX_2D*pVtx;//頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//スコアの初期化
	g_nScore = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTextureScore);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * MAX_SCORE,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&g_pVtxBuffScore,//変数名が変わると変更が必要
		NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

	for (int nScore = 0; nScore < MAX_SCORE; nScore++)
	{
		//頂点座標の設定（右回りで設定する）
		pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) - (SCORE_SIZE_X / 2) - 90, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) - (SCORE_SIZE_Y / 2) - 640, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) + (SCORE_SIZE_X / 2) - 90, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) - (SCORE_SIZE_Y / 2) - 640, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) - (SCORE_SIZE_X / 2) - 90, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) + (SCORE_SIZE_Y / 2) - 640, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) + (SCORE_SIZE_X / 2) - 90, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) + (SCORE_SIZE_Y / 2) - 640, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(33, 128, 221, 255);
		pVtx[1].col = D3DCOLOR_RGBA(33, 128, 221, 255);
		pVtx[2].col = D3DCOLOR_RGBA(33, 128, 221, 255);
		pVtx[3].col = D3DCOLOR_RGBA(33, 128, 221, 255);

		//テクスチャの座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データアンロック
	g_pVtxBuffScore->Unlock();

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	//テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{

	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //スコアの数字の描画
	for (int nScore = 0; nScore < MAX_SCORE; nScore++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureScore);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nScore, NUM_POLYGON);
	}

}
//=============================================================================
// スコアの加算
//=============================================================================
void ScoreEffect(int score)
{
	g_nScore += score;
	//変数宣言
	VERTEX_2D*pVtx;//頂点情報のポインタ
	float fIndex;		//指数　(スコア表示用)
	float fRadix = 10.0f;	//基数　(スコア表示用)

							//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

												  //スコアが0点を下回らないようにする
	if (g_nScore < 0)
	{
		g_nScore = 0;
	}

	for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
	{
		int nScore = (int)powf(fRadix, MAX_SCORE - fIndex);
		int nScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
		int nAnswer = g_nScore % nScore / nScore2;

		//テクスチャの座標
		pVtx[28].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
		pVtx[29].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
		pVtx[30].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
		pVtx[31].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

		pVtx -= 4;//頂点データのポインタを4つ分進める

	}
	//頂点データアンロック
	g_pVtxBuffScore->Unlock();
}
//=============================================================================
// スコアの取得
//=============================================================================
int GetScore(void)
{
	return g_nScore;
}