//=============================================================================
//
// X-Bow Hunter [bg.cpp]の処理 (流れるバックグラウンドと流れないUI)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "item.h"
#include "boss.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonBg[4] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonBg[2] = {};
int g_nCounterAnimBg;			//アニメーションカウンター
int g_nPatternAnimBg;			//アニメーションパターン

								//=============================================================================
								//バックグラウンド初期化処理
								//=============================================================================
void InitBg(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "bg000.png", &g_pTexturePolygonBg[0]);
	D3DXCreateTextureFromFile(pDevice, "score000.png", &g_pTexturePolygonBg[1]);
	D3DXCreateTextureFromFile(pDevice, "time000.png", &g_pTexturePolygonBg[2]);
	D3DXCreateTextureFromFile(pDevice, "stock000.png", &g_pTexturePolygonBg[3]);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg[0], NULL);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 3, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg[1], NULL);
	VERTEX_2D *pVtxBg;	//バックグラウンドの頂点情報へのポインタ
	VERTEX_2D *pVtxUI;	//UIの頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPolygonBg[0]->Lock(0, 0, (void**)&pVtxBg, 0);	//この書式は変えないこと
	g_pVtxBuffPolygonBg[1]->Lock(0, 0, (void**)&pVtxUI, 0);	//この書式は変えないこと

															//頂点座標の設定（右回りで設定する）
	pVtxBg[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtxBg[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtxBg[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtxBg[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//スコアの座標設定
	pVtxUI[0].pos = D3DXVECTOR3(800.0f, 0.0f, 0.0f);
	pVtxUI[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtxUI[2].pos = D3DXVECTOR3(800.0f, 100.0f, 0.0f);
	pVtxUI[3].pos = D3DXVECTOR3(1280.0f, 100.0f, 0.0f);

	//タイムの座標設定
	pVtxUI[4].pos = D3DXVECTOR3(530.0f, 0.0f, 0.0f);
	pVtxUI[5].pos = D3DXVECTOR3(680.0f, 0.0f, 0.0f);
	pVtxUI[6].pos = D3DXVECTOR3(530.0f, 110.0f, 0.0f);
	pVtxUI[7].pos = D3DXVECTOR3(680.0f, 110.0f, 0.0f);

	//ストックの座標設定
	pVtxUI[8].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtxUI[9].pos = D3DXVECTOR3(350.0f, 0.0f, 0.0f);
	pVtxUI[10].pos = D3DXVECTOR3(0.0f, 110.0f, 0.0f);
	pVtxUI[11].pos = D3DXVECTOR3(350.0f, 110.0f, 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtxBg[0].rhw = 1.0f;
	pVtxBg[1].rhw = 1.0f;
	pVtxBg[2].rhw = 1.0f;
	pVtxBg[3].rhw = 1.0f;
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

	//頂点カラーを設定（0～255の値で設定する）
	pVtxBg[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
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

	//テクスチャ座標の設定
	pVtxBg[0].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f, 0.0f);
	pVtxBg[1].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f - 0.999f, 0.0f);
	pVtxBg[2].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f, 1.0f);
	pVtxBg[3].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f - 0.999f, 1.0f);
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

	//頂点データをアンロックする
	for (int nCntBg = 0; nCntBg < 2; nCntBg++)
	{
		g_pVtxBuffPolygonBg[nCntBg]->Unlock();
	}
}

//=============================================================================
// バックグラウンド終了処理
//=============================================================================
void UninitBg(void)
{

	//テクスチャの開放
	for (int nCntUI = 0; nCntUI < 4; nCntUI++)
	{
		if (g_pTexturePolygonBg != NULL)
		{
			g_pTexturePolygonBg[nCntUI]->Release();
			g_pTexturePolygonBg[nCntUI] = NULL;
		}
	}

	//頂点バッファの開放
	for (int nCntBg = 0; nCntBg < 2; nCntBg++)
	{
		if (g_pVtxBuffPolygonBg != NULL)
		{
			g_pVtxBuffPolygonBg[nCntBg]->Release();
			g_pVtxBuffPolygonBg[nCntBg] = NULL;
		}
	}
}

//=============================================================================
// バックグラウンド更新処理
//=============================================================================
void UpdateBg(void)
{
	VERTEX_2D *pVtxBg;	//頂点情報へのポインタ
	ITEM *pItem;		//アイテムの情報を取得するため
	BOSS *pBoss;		//ボスの情報を取得するため

	g_pVtxBuffPolygonBg[0]->Lock(0, 0, (void**)&pVtxBg, 0);	//この書式は変えないこと

	pItem = GetItem();	//アイテムの情報を取得
	pBoss = GetBoss();	//ボスの情報を取得

						//左へ流れるアニメーション
	if (pItem->bUse == false)				//アイテムがあるときは、背景を止めて、アイテム取得を促す
	{
		g_nCounterAnimBg++;					//カウンタ加算
		if (g_nCounterAnimBg == 2)			//速さ
		{
			g_nCounterAnimBg = 0;
			g_nPatternAnimBg++;
			if (g_nPatternAnimBg == 1000)	//枚数
			{
				g_nPatternAnimBg = 0;
			}
		}
	}

	//テクスチャ座標を更新
	pVtxBg[0].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f, 0.0f);
	pVtxBg[1].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f - 0.999f, 0.0f);
	pVtxBg[2].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f, 1.0f);
	pVtxBg[3].tex = D3DXVECTOR2(g_nPatternAnimBg *0.999f - 0.999f, 1.0f);

	//ボス戦以外の色
	if (pBoss->bUse == false)
	{
		//頂点カラーを設定（0～255の値で設定する）
		pVtxBg[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtxBg[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtxBg[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtxBg[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//ボス戦の時の背景色(危険な感じを出す)
	if (pBoss->bUse == true)
	{
		//頂点カラーを設定（0～255の値で設定する）
		pVtxBg[0].col = D3DCOLOR_RGBA(249, 173, 0, 255);
		pVtxBg[1].col = D3DCOLOR_RGBA(249, 173, 0, 255);
		pVtxBg[2].col = D3DCOLOR_RGBA(249, 173, 0, 255);
		pVtxBg[3].col = D3DCOLOR_RGBA(249, 173, 0, 255);
	}

	//頂点データをアンロックする
	g_pVtxBuffPolygonBg[0]->Unlock();
}

//=============================================================================
// バックグラウンド描画処理
//=============================================================================
void DrawBg(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg[0], 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePolygonBg[0]);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);


	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg[1], 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntBgUI = 0; nCntBgUI < 3; nCntBgUI++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonBg[nCntBgUI + 1]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBgUI, NUM_POLYGON);
	}
}