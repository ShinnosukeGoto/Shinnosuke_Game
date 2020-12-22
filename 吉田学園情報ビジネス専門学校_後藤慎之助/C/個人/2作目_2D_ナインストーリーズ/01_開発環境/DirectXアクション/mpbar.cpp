//=============================================================================
//
// ナインストーリーズの処理 [mpbar.cpp] (MPバー)
// Author :後藤慎之助
//
//=============================================================================
#include "mpbar.h"
#include "player.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureMpbar[2] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffMpbar = NULL;

int g_nCounterMpbar;
int g_nPatternMpbar;

//=============================================================================
//MPバー初期化処理
//=============================================================================
void InitMpbar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER*pPlayer = GetPlayer();	//プレイヤーのMPを取得

									//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTextureMpbar[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTextureMpbar[1]);

	//グローバル変数の初期化
	g_nCounterMpbar = 0;
	g_nPatternMpbar = 0;

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMpbar, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMpbar->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

	//MPが減った場合、画像の右側から描画をやめる
	pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nMp / PLAYER_MP)), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nMp / PLAYER_MP)), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);

	//MPバーの背景
	pVtx[4].pos = D3DXVECTOR3(70.0f - MPBAR_EXTEND, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635.0f - MPBAR_EXTEND, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(70.0f + 320.0f + MPBAR_EXTEND, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635.0f - MPBAR_EXTEND, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(70.0f - MPBAR_EXTEND, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635.0f + MPBAR_EXTEND, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(70.0f + 320.0f + MPBAR_EXTEND, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635.0f + MPBAR_EXTEND, 0.0f);

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
	pVtx[0].col = D3DCOLOR_RGBA(8, 123, 202, 255);
	pVtx[1].col = D3DCOLOR_RGBA(8, 123, 202, 255);
	pVtx[2].col = D3DCOLOR_RGBA(8, 123, 202, 255);
	pVtx[3].col = D3DCOLOR_RGBA(8, 123, 202, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pPlayer->nMp) * (1 / PLAYER_MP), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pPlayer->nMp) * (1 / PLAYER_MP), 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffMpbar->Unlock();
}

//=============================================================================
// MPバー終了処理
//=============================================================================
void UninitMpbar(void)
{
	//テクスチャの開放
	for (int nCntMpbar = 0; nCntMpbar < 2; nCntMpbar++)
	{
		if (g_pTextureMpbar[nCntMpbar] != NULL)
		{
			g_pTextureMpbar[nCntMpbar]->Release();
			g_pTextureMpbar[nCntMpbar] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffMpbar != NULL)
	{
		g_pVtxBuffMpbar->Release();
		g_pVtxBuffMpbar = NULL;
	}
}

//=============================================================================
// MPバー更新処理
//=============================================================================
void UpdateMpbar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	PLAYER*pPlayer = GetPlayer();	//プレイヤーのMPを取得

									//頂点バッファをロック
	g_pVtxBuffMpbar->Lock(0, 0, (void**)&pVtx, 0);

	//MPが101以上にならないようにする
	if (pPlayer->nMp > PLAYER_MP)
	{
		pPlayer->nMp = PLAYER_MP;
	}

	//時間の概念を獲得
	if (pPlayer->bDisp == true)
	{
		g_nCounterMpbar++;	//カウンタ加算
		if (g_nCounterMpbar == 29)	//0.5秒で1回復
		{
			g_nCounterMpbar = 0;
			g_nPatternMpbar++;
			if (g_nPatternMpbar == 1)
			{
				g_nPatternMpbar = 0;

				//MPが最大値でないなら、1MP回復
				if (pPlayer->nMp < PLAYER_MP)
				{
					pPlayer->nMp += 1;
				}
			}
		}
	}

	//MPが減った場合、画像の右側から描画をやめる
	if (pPlayer->bDisp == true)
	{
		pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nMp / PLAYER_MP)), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nMp / PLAYER_MP)), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	}

	//HPがなくなった場合、画像を表示させない
	if (pPlayer->bDisp == false)
	{
		pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(70 + 320 * (0), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) - (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(70 + 320 * (0), (SCREEN_HEIGHT - MPBAR_SIZE_Y / 2) + (MPBAR_SIZE_Y / 2) - 635, 0.0f);
	}

	//炎選択時
	if (pPlayer->Mtype == MSELECT_FIRE)
	{
		if (pPlayer->nMp < FIRE_MP)
		{
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[1].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[2].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[3].col = D3DCOLOR_RGBA(8, 123, 202, 255);
		}
	}

	//雷選択時
	if (pPlayer->Mtype == MSELECT_LIGHTNING)
	{
		if (pPlayer->nMp < LIGHTNING_MP)
		{
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[1].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[2].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[3].col = D3DCOLOR_RGBA(8, 123, 202, 255);
		}
	}

	//吸収選択時
	if (pPlayer->Mtype == MSELECT_DRAIN)
	{
		if (pPlayer->nMp < DRAIN_MP)
		{
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[1].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[2].col = D3DCOLOR_RGBA(8, 123, 202, 255);
			pVtx[3].col = D3DCOLOR_RGBA(8, 123, 202, 255);
		}
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pPlayer->nMp / PLAYER_MP), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pPlayer->nMp / PLAYER_MP), 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffMpbar->Unlock();

}

//=============================================================================
// MPバー描画処理
//=============================================================================
void DrawMpbar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER*pPlayer = GetPlayer();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMpbar, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntMpbar = 1; nCntMpbar >-1; nCntMpbar--)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMpbar[nCntMpbar]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMpbar, NUM_POLYGON);
	}

}