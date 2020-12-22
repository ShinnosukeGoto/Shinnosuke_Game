//=============================================================================
//
// ナインストーリーズの処理 [bar.cpp] (HPバー)
// Author :後藤慎之助
//
//=============================================================================
#include "bar.h"
#include "player.h"
#include "enemy.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBar[4] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBar = NULL;

//=============================================================================
//HPバー初期化処理
//=============================================================================
void InitBar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	PLAYER*pPlayer = GetPlayer();	//プレイヤーのHPを取得

									//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTextureBar[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTextureBar[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTextureBar[2]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTextureBar[3]);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBar, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//HPが減った場合、画像の右側から描画をやめる
	pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 665, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nLife / PLAYER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 665, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 665, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nLife / PLAYER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 665, 0.0f);

	//HPバーの背景
	pVtx[4].pos = D3DXVECTOR3(70.0f - BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 665.0f - BAR_EXTEND, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(70.0f + 320.0f + BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 665.0f - BAR_EXTEND, 0.0f);
	pVtx[6].pos = D3DXVECTOR3(70.0f - BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 665.0f + BAR_EXTEND, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(70.0f + 320.0f + BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 665.0f + BAR_EXTEND, 0.0f);

	//敵のHPが減った場合、画像の右側から描画をやめる
	pVtx[8].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//敵のHPバーの背景
	pVtx[12].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwの設定（値は1.0で設定する）
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

	//頂点カラーを設定（0～255の値で設定する）
	pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[8].col = D3DCOLOR_RGBA(219, 73, 61, 255);
	pVtx[9].col = D3DCOLOR_RGBA(219, 73, 61, 255);
	pVtx[10].col =D3DCOLOR_RGBA(219, 73, 61, 255);
	pVtx[11].col =D3DCOLOR_RGBA(219, 73, 61, 255);

	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pPlayer->nLife) * (1 / PLAYER_LIFE), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pPlayer->nLife) * (1 / PLAYER_LIFE), 1.0f);

	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[10].tex =D3DXVECTOR2(0.0f, 0.0f);
	pVtx[11].tex =D3DXVECTOR2(0.0f, 0.0f);

	pVtx[12].tex =D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex =D3DXVECTOR2(0.0f, 0.0f);
	pVtx[14].tex =D3DXVECTOR2(0.0f, 0.0f);
	pVtx[15].tex =D3DXVECTOR2(0.0f, 0.0f);

	//頂点データをアンロックする
	g_pVtxBuffBar->Unlock();
}

//=============================================================================
// HPバー終了処理
//=============================================================================
void UninitBar(void)
{
	//テクスチャの開放
	for (int nCntBar = 0; nCntBar < 4; nCntBar++)
	{
		if (g_pTextureBar[nCntBar] != NULL)
		{
			g_pTextureBar[nCntBar]->Release();
			g_pTextureBar[nCntBar] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBar != NULL)
	{
		g_pVtxBuffBar->Release();
		g_pVtxBuffBar = NULL;
	}
}

//=============================================================================
// HPバー更新処理
//=============================================================================
void UpdateBar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	PLAYER*pPlayer = GetPlayer();	//プレイヤーのHPを取得
	ENEMY*pEnemy = GetEnemy();		//ボスのHPを取得

	//頂点バッファをロック
	g_pVtxBuffBar->Lock(0, 0, (void**)&pVtx, 0);

	//HPが251以上にならないようにする
	if (pPlayer->nLife > PLAYER_LIFE)
	{
		pPlayer->nLife = PLAYER_LIFE;
	}

	//HPが減った場合、画像の右側から描画をやめる
	if (pPlayer->bDisp == true)
	{
		pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 665, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nLife / PLAYER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 665, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 665, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(70 + 320 * ((pPlayer->nLife / PLAYER_LIFE)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 665, 0.0f);
	}

	//HPがなくなった場合、画像を表示させない
	if (pPlayer->bDisp == false)
	{
		pVtx[0].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 665, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(70 + 320 * (0), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - 665, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(70, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 665, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(70 + 320 * (0), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - 665, 0.0f);
	}

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((pPlayer->nLife / PLAYER_LIFE), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((pPlayer->nLife / PLAYER_LIFE), 1.0f);

	//HPに応じてカラーを変える（満タンから半分まで緑）
	if (pPlayer->nLife <= PLAYER_LIFE&&pPlayer->nLife > PLAYER_LIFE / 2)
	{
		pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
		pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);
	}

	//HPに応じてカラーを変える（半分から4分の1まで黄色）
	if (pPlayer->nLife <= PLAYER_LIFE / 2 && pPlayer->nLife > PLAYER_LIFE / 4)
	{
		pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);
	}

	//HPに応じてカラーを変える（4分の1からは赤色）
	if (pPlayer->nLife <= PLAYER_LIFE / 4)
	{
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	//ボスのHP表示
	if (pEnemy->bUse == true)
	{
		//アイスドラゴン
		if (pEnemy->type == ENEMYTYPE_006)
		{
			//HPが減った場合、画像の右側から描画をやめる
			pVtx[8].pos = D3DXVECTOR3(pEnemy->pos.x-105, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y/2)-250, 0.0f);
			pVtx[9].pos = D3DXVECTOR3(pEnemy->pos.x-105+ 210 * ((pEnemy->nLife / ENEMY_LIFE_BOSS)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2)-250, 0.0f);
			pVtx[10].pos = D3DXVECTOR3(pEnemy->pos.x-105, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2)-250, 0.0f);
			pVtx[11].pos = D3DXVECTOR3(pEnemy->pos.x-105 + 210 * ((pEnemy->nLife / ENEMY_LIFE_BOSS)), (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2)-250, 0.0f);

			//敵のHPバーの背景
			pVtx[12].pos = D3DXVECTOR3(pEnemy->pos.x-105 - BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2)-250 - BAR_EXTEND, 0.0f);
			pVtx[13].pos = D3DXVECTOR3(pEnemy->pos.x-105 + 210.0f + BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) - (BAR_SIZE_Y / 2) - pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2)-250 - BAR_EXTEND, 0.0f);
			pVtx[14].pos = D3DXVECTOR3(pEnemy->pos.x-105 - BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - pEnemy->pos.y+ (ENEMY_SIZE_BOSS_Y / 2)-250 + BAR_EXTEND, 0.0f);
			pVtx[15].pos = D3DXVECTOR3(pEnemy->pos.x-105 + 210.0f + BAR_EXTEND, (SCREEN_HEIGHT - BAR_SIZE_Y / 2) + (BAR_SIZE_Y / 2) - pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2)-250 + BAR_EXTEND, 0.0f);

			pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[9].tex = D3DXVECTOR2((pEnemy->nLife) * (1 / ENEMY_LIFE_BOSS), 0.0f);
			pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[11].tex = D3DXVECTOR2((pEnemy->nLife) * (1 / ENEMY_LIFE_BOSS), 1.0f);

			pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);

			//4分の1以下でピンチカラーにする
			if (pEnemy->nLife < 375)
			{
				//ピンチ描画にする処理
				pVtx[8].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[9].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[10].col = D3DCOLOR_RGBA(255, 0, 0, 255);
				pVtx[11].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			}
			else
			{
				pVtx[8].col = D3DCOLOR_RGBA(219, 73, 61, 255);
				pVtx[9].col = D3DCOLOR_RGBA(219, 73, 61, 255);
				pVtx[10].col = D3DCOLOR_RGBA(219, 73, 61, 255);
				pVtx[11].col = D3DCOLOR_RGBA(219, 73, 61, 255);
			}
		}
	}

	//HPがなくなった場合、画像を表示させない
	if (pEnemy->bUse == false)
	{
		//HPが減った場合、画像の右側から描画をやめる
		pVtx[8].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[9].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[10].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[11].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		pVtx[12].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[13].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[14].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[15].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//頂点データをアンロックする
	g_pVtxBuffBar->Unlock();

}

//=============================================================================
// HPバー描画処理
//=============================================================================
void DrawBar(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBar, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//自機のHPバー
	for (int nCntBar = 1; nCntBar >-1; nCntBar--)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBar[nCntBar]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBar, NUM_POLYGON);
	}

	//ボスのHPバー
	for (int nCntBar = 3; nCntBar >1; nCntBar--)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBar[nCntBar]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBar, NUM_POLYGON);
	}
}