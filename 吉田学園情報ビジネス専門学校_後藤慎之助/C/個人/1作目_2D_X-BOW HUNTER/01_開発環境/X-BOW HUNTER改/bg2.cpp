//=============================================================================
//
// X-Bow Hunter [bg2.cpp]の処理 (漂う蛍）
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "bg2.h"
#include "boss.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonBg2 = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonBg2 = NULL;
int g_nCounterAnimBg2;			//アニメーションカウンター
int g_nPatternAnimBg2;			//アニメーションパターン
int g_nCntTime;		//時間を数える
int g_nCntLight;	//蛍の点滅

					//=============================================================================
					//バックグラウンド初期化処理
					//=============================================================================
void InitBg2(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "bg001.png", &g_pTexturePolygonBg2);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg2, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPolygonBg2->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

														//頂点座標の設定（右回りで設定する）
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーを設定（0～255の値で設定する）
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 150);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 150);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 150);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 150);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2 *0.98f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2 *0.98f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2 *0.98f - 0.98f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2 *0.98f - 0.98f);

	//頂点データをアンロックする
	g_pVtxBuffPolygonBg2->Unlock();
}

//=============================================================================
// バックグラウンド終了処理
//=============================================================================
void UninitBg2(void)
{
	//テクスチャの開放
	if (g_pTexturePolygonBg2 != NULL)
	{
		g_pTexturePolygonBg2->Release();
		g_pTexturePolygonBg2 = NULL;
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
	BOSS *pBoss;		//ボスを取得するため

						//上に流れるアニメーション
	g_nCounterAnimBg2++;	//カウンタ加算
	if (g_nCounterAnimBg2 == 4)//速さ
	{
		g_nCounterAnimBg2 = 0;
		g_nPatternAnimBg2++;
		if (g_nPatternAnimBg2 == 1000)//枚数
		{
			g_nPatternAnimBg2 = 0;
		}
	}

	//点滅するアニメーション
	g_nCntTime++;
	if (g_nCntTime == 12)//速さ
	{
		g_nCntTime = 0;
		g_nCntLight++;
		if (g_nCntLight == 9)//透明度
		{
			g_nCntLight = 0;
		}
	}

	//頂点バッファをロック
	g_pVtxBuffPolygonBg2->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標を更新
	pVtx[0].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2 *0.98f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2 *0.98f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg2 *0.98f - 0.98f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg2 *0.98f - 0.98f);

	//ボスの情報を取得
	pBoss = GetBoss();

	//ボス戦以外の色
	if (pBoss->bUse == false)
	{

		////頂点カラーを設定（0～255の値で設定する）
		if (g_nCntLight == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		}

		if (g_nCntLight == 1)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		}

		if (g_nCntLight == 2)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		}

		if (g_nCntLight == 3)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		}

		if (g_nCntLight == 4)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 5);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 5);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 5);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 5);
		}

		if (g_nCntLight == 5)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		}

		if (g_nCntLight == 6)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);
		}

		if (g_nCntLight == 7)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 155);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		}

		if (g_nCntLight == 8)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 205);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 205);
		}
	}

	//ボス戦は赤い蛍
	if (pBoss->bUse == true)
	{
		////頂点カラーを設定（0～255の値で設定する）
		if (g_nCntLight == 0)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 205);
		}

		if (g_nCntLight == 1)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 155);
		}

		if (g_nCntLight == 2)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 105);
		}

		if (g_nCntLight == 3)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 55);
		}

		if (g_nCntLight == 4)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 5);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 5);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 5);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 5);
		}

		if (g_nCntLight == 5)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 55);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 55);
		}

		if (g_nCntLight == 6)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 105);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 105);
		}

		if (g_nCntLight == 7)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 155);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 155);
		}

		if (g_nCntLight == 8)
		{
			pVtx[0].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[1].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[2].col = D3DCOLOR_RGBA(251, 85, 3, 205);
			pVtx[3].col = D3DCOLOR_RGBA(251, 85, 3, 205);
		}
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

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygonBg2);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}