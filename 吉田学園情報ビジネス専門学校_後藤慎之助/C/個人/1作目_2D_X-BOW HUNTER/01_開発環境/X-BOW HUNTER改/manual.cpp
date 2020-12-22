//=============================================================================
//
// X-Bow Hunterの処理 [manual.cpp] (遊び方)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "input.h"
#include "game.h"
#include "main.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureManual = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManual = NULL;//頂点バッファへのポインタ

						//=============================================================================
						// マニュアル初期化処理
						//=============================================================================
void InitManual(void)
{
	//変数宣言
	VERTEX_2D*pVtx;//頂点情報のポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "manual000.png", &g_pTextureManual);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&g_pVtxBuffManual,//変数名が変わると変更が必要
		NULL);

								//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

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

	//頂点データアンロック
	g_pVtxBuffManual->Unlock();
}
//=============================================================================
// マニュアル更新処理
//=============================================================================
void UpdateManual(void)
{
	int nFade = GetFade();	//フェードの情報を獲得

	//Enterキーを押した時
	if (GetKeyboardPress(DIK_RETURN)&&nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音でプレスエンター音の代用
		InitGame2();		//ゲームの初期化2
		SetFade(MODE_GAME);//ゲーム画面に移動
		StopSound(SOUND_LABEL_BGM000);//曲の停止(特定の物)
	}
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitManual(void)
{
	//テクスチャの開放
	if (g_pTextureManual != NULL)
	{
		g_pTextureManual->Release();
		g_pTextureManual = NULL;
	}
	//頂点バッファの開放
	if (g_pVtxBuffManual != NULL)
	{
		g_pVtxBuffManual->Release();
		g_pVtxBuffManual = NULL;
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawManual(void)
{
	//頂点フォーマットの設定
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffManual, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

								   //テクスチャの設定
	pDevice->SetTexture(0, g_pTextureManual);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
}