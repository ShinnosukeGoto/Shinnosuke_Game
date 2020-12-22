//=============================================================================
//
// X-Bow Hunterの処理 [result2.cpp] (ランキング)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "result2.h"
#include "input.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "score.h"
#include "game.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult2[7] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult2 = NULL;//頂点バッファへのポインタ

int g_nHuckerState2;	//Pキーが押された状況
int g_nResultScore;		//最終スコア

						//=============================================================================
						// リザルト2初期化処理
						//=============================================================================
void InitResult2(void)
{
	//変数宣言
	VERTEX_2D*pVtx;//頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Pを押した状況の初期化
	g_nHuckerState2 = AREYOUHUCKER？_NO;

	//リザルト用のスコアを初期化
	g_nResultScore = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "gameend002.png", &g_pTextureResult2[0]);	//かけだしハンター
	D3DXCreateTextureFromFile(pDevice, "number000.png", &g_pTextureResult2[1]);		//ナンバー
	D3DXCreateTextureFromFile(pDevice, "gameend003.png", &g_pTextureResult2[2]);	//一人前ハンター
	D3DXCreateTextureFromFile(pDevice, "gameend004.png", &g_pTextureResult2[3]);	//ベテランハンター
	D3DXCreateTextureFromFile(pDevice, "gameend001.png", &g_pTextureResult2[4]);	//ジェノサイドハンター
	D3DXCreateTextureFromFile(pDevice, "gameend000.png", &g_pTextureResult2[5]);	//ハッカーエンディング
	D3DXCreateTextureFromFile(pDevice, "gameend005.png", &g_pTextureResult2[6]);	//三流ハンター

																					//頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX * 9,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&g_pVtxBuffResult2,//変数名が変わると変更が必要
		NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffResult2->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

													//頂点座標の設定（右回りで設定する）
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
	{
		//頂点座標の設定（右回りで設定する）
		pVtx[4].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RESULT2_SIZE_X + RESULT2_SIZE_X / 2) - (RESULT2_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RESULT2_SIZE_Y / 2) - (RESULT2_SIZE_Y / 2) - 400, 0.0f);
		pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RESULT2_SIZE_X + RESULT2_SIZE_X / 2) + (RESULT2_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RESULT2_SIZE_Y / 2) - (RESULT2_SIZE_Y / 2) - 400, 0.0f);
		pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RESULT2_SIZE_X + RESULT2_SIZE_X / 2) - (RESULT2_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RESULT2_SIZE_Y / 2) + (RESULT2_SIZE_Y / 2) - 400, 0.0f);
		pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RESULT2_SIZE_X + RESULT2_SIZE_X / 2) + (RESULT2_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RESULT2_SIZE_Y / 2) + (RESULT2_SIZE_Y / 2) - 400, 0.0f);

		//rhwの設定
		pVtx[4].rhw = 1.0f;
		pVtx[5].rhw = 1.0f;
		pVtx[6].rhw = 1.0f;
		pVtx[7].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[4].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[5].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[6].col = D3DCOLOR_RGBA(0, 0, 0, 255);
		pVtx[7].col = D3DCOLOR_RGBA(0, 0, 0, 255);

		//テクスチャの座標
		pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[5].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[7].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データアンロック
	g_pVtxBuffResult2->Unlock();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult2(void)
{
	//変数宣言
	VERTEX_2D*pVtx;//頂点情報のポインタ
	float fIndex;		//指数　(スコア表示用)
	float fRadix = 10.0f;	//基数　(スコア表示用)
	g_nResultScore = GetScore();	//スコアをリザルトに反映
	int nFade = GetFade();	//フェードの情報を獲得


									//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffResult2->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

	for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
	{
		int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
		int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
		int nAnswer = g_nResultScore % nResultScore / nResultScore2;

		//テクスチャの座標
		pVtx[32].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
		pVtx[33].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
		pVtx[34].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
		pVtx[35].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

		pVtx -= 4;//頂点データのポインタを4つ分進める

	}
	//頂点データアンロック
	g_pVtxBuffResult2->Unlock();

	//Enterキーを押した時
	if (GetKeyboardPress(DIK_RETURN) && nFade == FADE_NONE)
	{
		StopSound(SOUND_LABEL_BGM002);
		StopSound(SOUND_LABEL_BGM003);
		PlaySound(SOUND_LABEL_SE_ENTER);	//プレスエンター音
		InitTitle();		//タイトルを初期化
		SetFade(MODE_TITLE);//タイトル画面に移動
		PlaySound(SOUND_LABEL_BGM000);	//タイトルのBGM
	}
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitResult2(void)
{
	//テクスチャの開放
	if (g_pTextureResult2 != NULL)
	{
		for (int nCntResultScore = 0; nCntResultScore < 7; nCntResultScore++)
		{
			g_pTextureResult2[nCntResultScore]->Release();
			g_pTextureResult2[nCntResultScore] = NULL;
		}

	}
	//頂点バッファの開放
	if (g_pVtxBuffResult2 != NULL)
	{
		g_pVtxBuffResult2->Release();
		g_pVtxBuffResult2 = NULL;
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawResult2(void)
{
	g_nResultScore = GetScore();	//スコアをリザルトに反映

									//Pボタンの使用状況を取得
	g_nHuckerState2 = GetGameState();

	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffResult2, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //リザルト2の背景
	if (g_nHuckerState2 == AREYOUHUCKER？_NO)	//デバックコマンド使用なし
	{
		//スコア0～19999pt
		if (g_nResultScore >= 0 && g_nResultScore < 20000)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult2[0]);
			//リザルト背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		//スコア20000～49999pt
		if (g_nResultScore >= 20000 && g_nResultScore < 50000)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult2[6]);
			//リザルト背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		//スコア50000～79999pt
		if (g_nResultScore >= 50000 && g_nResultScore < 80000)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult2[2]);
			//リザルト背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		//スコア80000～99999pt
		if (g_nResultScore >= 80000 && g_nResultScore < 100000)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult2[3]);
			//リザルト背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}

		//スコア100000pt～
		if (g_nResultScore >= 100000)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResult2[4]);
			//リザルト背景の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
		}
	}

	if (g_nHuckerState2 == AREYOUHUCKER？_YES)	//デバックコマンド使用あり
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult2[5]);
		//リザルト背景の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
	}

	//スコアの数字の描画
	for (int nResultScore = 1; nResultScore < MAX_SCORE + 1; nResultScore++)	//pVtx0～3を読み込まないようにするため、1からforを始める
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult2[1]);
		//数字の描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nResultScore, NUM_POLYGON);
	}

}