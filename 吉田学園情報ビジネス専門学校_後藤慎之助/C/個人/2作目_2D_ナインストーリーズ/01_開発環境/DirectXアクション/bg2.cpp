//=============================================================================
//
// ナインストーリーズ [bg2.cpp]の処理 (雪と鎖）
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "bg2.h"
#include "input.h"
#include "player.h"
#include "bg.h"
#include "block.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonBg2[2] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonBg2 = NULL;
int g_nCounterAnimBg2Left;		//左のアニメーションカウンター
int g_nCounterAnimBg2Right;		//右のアニメーションカウンター
int g_nCounterAnimBg2;			//アニメーションカウンター
int g_nPatternAnimBg2X;			//アニメーションパターンX
int g_nPatternAnimBg2Y;			//アニメーションパターンY
int g_nCntTime;		//時間を数える
int g_nCntLight;	//蛍の点滅
bool g_bMoveBg2;				//背景が動いている
bool g_bBlockXB2;				//ブロックに横から当たった時
bool g_bBlockYB2;				//ブロックが停止位置にたどり着いたとき
MODE g_ModeBg2;					//ゲーム画面に対応した背景

//=============================================================================
//バックグラウンド初期化処理
//=============================================================================
void InitBg2(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//グローバル変数の初期化
	g_nCounterAnimBg2Left = 0;
	g_nCounterAnimBg2Right = 0;
	g_nCounterAnimBg2 = 0;
	g_nPatternAnimBg2X = 0;
	g_nPatternAnimBg2Y = 0;
	g_nCntTime = 0;
	g_nCntLight = 0;
	g_bMoveBg2 = false;
	g_bBlockXB2 = false;
	g_bBlockYB2 = false;
	g_ModeBg2 = MODE_GAME;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg002.png", &g_pTexturePolygonBg2[0]);	//雪
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg003.png", &g_pTexturePolygonBg2[1]);	//鎖

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg2, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPolygonBg2->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

														//頂点座標の設定（右回りで設定する）
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 205);
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f, g_nPatternAnimBg2Y *0.001f);
	pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f + 0.997f, g_nPatternAnimBg2Y *0.001f);
	pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f, g_nPatternAnimBg2Y *0.001f - 0.999f);
	pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f + 0.997f, g_nPatternAnimBg2Y *0.001f - 0.999f);

	//テクスチャ座標を更新
	pVtx[4].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2Y *0.01f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2Y *0.01f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2Y *0.01f - 0.99f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2Y *0.01f - 0.99f);

	//頂点データをアンロックする
	g_pVtxBuffPolygonBg2->Unlock();
}

//=============================================================================
// バックグラウンド終了処理
//=============================================================================
void UninitBg2(void)
{
	//テクスチャの開放
	for (int nCntBg = 0; nCntBg < 2; nCntBg++)	//使っている画像分
	{
		if (g_pTexturePolygonBg2[nCntBg] != NULL)
		{
			g_pTexturePolygonBg2[nCntBg]->Release();
			g_pTexturePolygonBg2[nCntBg] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffPolygonBg2 != NULL)
	{
		g_pVtxBuffPolygonBg2->Release();
		g_pVtxBuffPolygonBg2 = NULL;
	}
}

//=============================================================================
// バックグラウンド更新処理
//=============================================================================
void UpdateBg2(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	PLAYER *pPlayer;

	//ゲームのステージを取得
	g_ModeBg2 = GetMode();

	//頂点バッファをロック
	g_pVtxBuffPolygonBg2->Lock(0, 0, (void**)&pVtx, 0);

	//雪
	if (g_ModeBg2 == MODE_GAME)
	{
		//下に流れるアニメーション
		g_nCounterAnimBg2++;	//カウンタ加算
		if (g_nCounterAnimBg2 == 1)//速さ
		{
			g_nCounterAnimBg2 = 0;
			g_nPatternAnimBg2Y++;
			if (g_nPatternAnimBg2Y == 1000)//枚数
			{
				g_nPatternAnimBg2Y = 0;
			}
		}

		//プレイヤーの状況を取得
		pPlayer = GetPlayer();

		//背景の状況を取得
		g_bMoveBg2 = GetBg();

		//ブロックに横から当たっているかどうか取得
		g_bBlockXB2 = GetBlockX();

		//プレイヤーの操作に対応した移動
		if (pPlayer->bDisp == true && g_bMoveBg2 == true && g_bBlockXB2 == false)
		{
			if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
			{
				g_nCounterAnimBg2Right++;					//カウンタ加算
				if (g_nCounterAnimBg2Right == 2)			//速さ
				{
					g_nCounterAnimBg2Right = 0;
					g_nPatternAnimBg2X++;
					if (g_nPatternAnimBg2X == 1000)	//枚数
					{
						g_nPatternAnimBg2X = 0;
					}
				}
			}

			if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
			{
				g_nCounterAnimBg2Left++;					//カウンタ加算
				if (g_nCounterAnimBg2Left == 2)			//速さ
				{
					g_nCounterAnimBg2Left = 0;
					g_nPatternAnimBg2X--;
					if (g_nPatternAnimBg2X == -1000)	//枚数
					{
						g_nPatternAnimBg2X = 0;
					}
				}
			}
		}

		//テクスチャ座標を更新
		pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f, g_nPatternAnimBg2Y *0.001f);
		pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f + 0.997f, g_nPatternAnimBg2Y *0.001f);
		pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f, g_nPatternAnimBg2Y *0.001f - 0.999f);
		pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBg2X *0.003f + 0.997f, g_nPatternAnimBg2Y *0.001f - 0.999f);

		//頂点カラーを設定（0～255の値で設定する）
		if (g_nCntLight == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		}
	}

	//鎖
	if (g_ModeBg2 == MODE_GAME2)
	{
		//ブロックが停止位置にたどり着いたかどうかを取得
		g_bBlockYB2 = GetBlockY();

		//下に流れるアニメーション
		if (g_bBlockYB2 == false)
		{
			g_nCounterAnimBg2++;	//カウンタ加算
			if (g_nCounterAnimBg2 == 4)//速さ
			{
				g_nCounterAnimBg2 = 0;
				g_nPatternAnimBg2Y++;
				if (g_nPatternAnimBg2Y == 100)//枚数
				{
					g_nPatternAnimBg2Y = 0;
				}
			}
		}

		//テクスチャ座標を更新
		pVtx[4].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2Y *0.01f);
		pVtx[5].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2Y *0.01f);
		pVtx[6].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2Y *0.01f - 0.99f);
		pVtx[7].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2Y *0.01f - 0.99f);
	}

	//頂点データをアンロックする
	g_pVtxBuffPolygonBg2->Unlock();
}

//=============================================================================
// バックグラウンド描画処理
//=============================================================================
void DrawBg2(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg2, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ゲーム1 雪
	if (g_ModeBg2 == MODE_GAME)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonBg2[0]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4*0, NUM_POLYGON);
	}

	//ゲーム2 鎖
	if (g_ModeBg2 == MODE_GAME2)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonBg2[1]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4*1, NUM_POLYGON);
	}
}