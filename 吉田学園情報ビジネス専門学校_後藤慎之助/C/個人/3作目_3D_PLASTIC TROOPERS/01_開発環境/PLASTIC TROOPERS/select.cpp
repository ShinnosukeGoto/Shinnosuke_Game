//=============================================================================
//
// ナインストーリーズの処理 [select.cpp] (武器と魔法の選択)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "input.h"
//#include "game.h"
#include "select.h"
#include "fade.h"
//#include "sound.h"
#include "manual.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureSelect[SELECT_UI] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;//頂点バッファへのポインタ

CSELECT g_nCSelect;	//キャラ選択
WSELECT g_nWSelect;	//武器選択
SSELECT g_nSSelect;	//盾選択

					//=============================================================================
					// セレクト初期化処理
					//=============================================================================
void InitSelect(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//選択の初期化
	g_nCSelect = CSELECT_ART;	//最初のカーソルはアーサー
	g_nWSelect = WSELECT_SWORD;	//最初のカーソルは剣
	g_nSSelect = SSELECT_NO;	//最初のカーソルは盾なし

								//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select999.png", &g_pTextureSelect[0]);	//背景
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select000.png", &g_pTextureSelect[1]);	//ゲーム
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select001.png", &g_pTextureSelect[2]);	//遊び方
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select002.png", &g_pTextureSelect[3]);	//ランキング
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select003.png", &g_pTextureSelect[4]);	//タイトルに戻る

																						//頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX*SELECT_UI,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&g_pVtxBuffSelect,//変数名が変わると変更が必要
		NULL);
	VERTEX_2D*pVtx;//頂点情報のポインタ

				   //頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

												   //頂点座標の設定（右回りで設定する）
												   //セレクトの背景
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//ゲーム
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	//遊び方
	pVtx[8].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
	//ランキング
	pVtx[12].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0.0f);
	//タイトルに戻る
	pVtx[16].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;
	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;
	pVtx[16].rhw = 1.0f;
	pVtx[17].rhw = 1.0f;
	pVtx[18].rhw = 1.0f;
	pVtx[19].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//テクスチャの座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データアンロック
	g_pVtxBuffSelect->Unlock();
}
//=============================================================================
// セレクト更新処理
//=============================================================================
void UpdateSelect(void)
{
	//VERTEX_2D *pVtx;	//頂点情報へのポインタ
	//int nFade = GetFade();	//フェードの情報を獲得

	//						//頂点バッファをロック
	//g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	////モード選択
	//switch (g_nMSelect)
	//{
	//case MSELECT_GAME:
	//	//ゲームが明るい
	//	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	//→キーが押された（右移動,遊び方選択）
	//	if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_MANUAL;
	//	}

	//	//↓キーが押された（下移動,ランキング選択）
	//	if (GetKeyboardTrigger(DIK_DOWN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_RANKING;
	//	}

	//	//Enterキーを押した時
	//	if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音でプレスエンター音の代用
	//		SetFade(MODE_GAME);//ゲーム画面に移動
	//	}

	//	break;

	//case MSELECT_MANUAL:
	//	//遊び方が明るい
	//	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	//←キーが押された（左移動,ゲーム選択）
	//	if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_GAME;
	//	}

	//	//↓キーが押された（下移動,タイトルに戻る選択）
	//	if (GetKeyboardTrigger(DIK_DOWN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_RETURN;
	//	}

	//	//Enterキーを押した時
	//	if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音でプレスエンター音の代用
	//		SetFade(MODE_MANUAL);//遊び方画面に移動
	//	}

	//	break;

	//case MSELECT_RANKING:
	//	//ランキングが明るい
	//	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	//→キーが押された（右移動,タイトルに戻る選択）
	//	if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_RETURN;
	//	}

	//	//↑キーが押された（上移動,ゲーム選択）
	//	if (GetKeyboardTrigger(DIK_UP) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_GAME;
	//	}

	//	//Enterキーを押した時
	//	if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音でプレスエンター音の代用
	//		//SetFade(MODE_RANKING);//ランキング画面に移動
	//	}

	//	break;

	//case MSELECT_RETURN:
	//	//タイトルに戻るが明るい
	//	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	//	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	//	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//	//←キーが押された（左移動,ランキング選択）
	//	if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_RANKING;
	//	}

	//	//↑キーが押された（上移動,遊び方選択）
	//	if (GetKeyboardTrigger(DIK_UP) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_JUMP);
	//		g_nMSelect = MSELECT_MANUAL;
	//	}

	//	//Enterキーを押した時
	//	if (GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	//	{
	//		//PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音でプレスエンター音の代用
	//		SetFade(MODE_TITLE);//タイトル画面に移動
	//	}

	//	break;
	//}

	////頂点データをアンロックする
	//g_pVtxBuffSelect->Unlock();

}

//=============================================================================
// セレクト終了処理
//=============================================================================
void UninitSelect(void)
{
	//テクスチャの開放
	for (int nCntSelect = 0; nCntSelect < SELECT_UI; nCntSelect++)
	{
		if (g_pTextureSelect[nCntSelect] != NULL)
		{
			g_pTextureSelect[nCntSelect]->Release();
			g_pTextureSelect[nCntSelect] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}
}
//=============================================================================
// セレクト描画処理
//=============================================================================
void DrawSelect(void)
{
	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	for (int nCntSelect = 0; nCntSelect < SELECT_UI; nCntSelect++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSelect[nCntSelect]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSelect, NUM_POLYGON);
	}
}

//=============================================================================
// キャラ選択の取得
//=============================================================================
CSELECT GetCSelect(void)
{
	return g_nCSelect;
}

//=============================================================================
// 武器選択の取得
//=============================================================================
WSELECT GetWSelect(void)
{
	return g_nWSelect;
}

//=============================================================================
// 盾選択の取得
//=============================================================================
SSELECT GetSSelect(void)
{
	return g_nSSelect;
}