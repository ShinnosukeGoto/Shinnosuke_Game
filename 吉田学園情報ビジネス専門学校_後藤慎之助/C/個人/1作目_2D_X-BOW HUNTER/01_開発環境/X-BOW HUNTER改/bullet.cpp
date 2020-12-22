//=============================================================================
//
// DirectXシューティング [bullet.cpp] (敵の弾)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "boss.h"
#include "effect.h"
#include "storm.h"
#include "input.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBullet = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET];

//=============================================================================
//バレット初期化処理
//=============================================================================
void InitBullet(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBullet;

	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "bullet000.png", &g_pTextureBullet);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//弾の数分
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//rhwの設定（値は1.0で設定する）
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーを設定（0～255の値で設定する）
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// バレット終了処理
//=============================================================================
void UninitBullet(void)
{
	//テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// バレット更新処理
//=============================================================================
void UpdateBullet(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBullet;
	VERTEX_2D *pVtx;
	ENEMY*pEnemy;
	int nCntEnemy;
	BOSS*pBoss;
	int nCntBoss;
	HUNTER*pHunter;

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//Pキーが押された（デバックコマンド）
			if (GetKeyboardPress(DIK_P))
			{
				g_aBullet[nCntBullet].bUse = false;	//使用しない状態にする
				break;
			}

			//敵を取得
			pEnemy = GetEnemy();

			//ボスを取得
			pBoss = GetBoss();

			//自機を取得
			pHunter = GetHunter();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true) //敵が使用されている
				{
					if (pHunter->bDisp == true)
					{
						//自機の当たり判定
						if (g_aBullet[nCntBullet].pos.x <= pHunter->pos.x + (HUNTER_SIZE / 2) - 10 &&
							g_aBullet[nCntBullet].pos.x >= pHunter->pos.x - (HUNTER_SIZE / 2) + 10 &&
							g_aBullet[nCntBullet].pos.y <= pHunter->pos.y + (HUNTER_SIZE / 2) - 10 &&
							g_aBullet[nCntBullet].pos.y >= pHunter->pos.y - (HUNTER_SIZE / 2) + 10)
						{
							if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)//敵の弾
							{
								//自機にダメージ
								HitHunter(10);
							}

							if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMYⅡ || g_aBullet[nCntBullet].type == BULLETTYPE_GHO)//メタルバード亜種とゴーストの弾
							{
								//自機にダメージ
								HitHunter(30);
							}

							if (g_aBullet[nCntBullet].type == BULLETTYPE_OCT)//オクトパスの弾
							{
								//自機にダメージ
								HitHunter(50);
							}

							//当たった時点で弾のHPを0にする
							g_aBullet[nCntBullet].nLife = 0;
						}
					}
				}
			}

			for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
			{
				if (pBoss->bUse == true) //ボスが使用されている
				{
					if (pHunter->bDisp == true)
					{
						//自機の当たり判定
						if (g_aBullet[nCntBullet].pos.x <= pHunter->pos.x + (HUNTER_SIZE / 2) - 10 &&
							g_aBullet[nCntBullet].pos.x >= pHunter->pos.x - (HUNTER_SIZE / 2) + 10 &&
							g_aBullet[nCntBullet].pos.y <= pHunter->pos.y + (HUNTER_SIZE / 2) - 10 &&
							g_aBullet[nCntBullet].pos.y >= pHunter->pos.y - (HUNTER_SIZE / 2) + 10)
						{
							//自機にダメージ
							HitHunter(10);

							//当たった時点で弾のHPを0にする
							g_aBullet[nCntBullet].nLife = 0;

							//自機に直接ボスの弾が当たった時も、見栄えをよくするために自機のやや上(連続ヒットしない位置)にストームを発生
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(-10, -10, 0), 35, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 35);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(9.5, -9.5, 0), 38, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 30);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(-9.0, -9.0, 0), 41, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 25);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(8.5, -8.5, 0), 44, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 20);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(-8.0, -8.0, 0), 47, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 15);
							SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 30, 0.0f), D3DXVECTOR3(7.5, -7.5, 0), 50, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 10);
						}
					}
				}
			}

			if (g_aBullet[nCntBullet].type == BULLETTYPE_BOSS_PINCH)
			{
				//ボスとハンターの位置を取得
				pHunter = GetHunter();
				pBoss = GetBoss();

				float fHposx = pHunter->pos.x, fHposy = pHunter->pos.y;	//目標(ハンター)の座標
				float fBposx = pBoss->pos.x, fBposy = pBoss->pos.y;		//敵の座標
				float fAngle;											//角度

				fAngle = atan2f((fBposx - fHposx), (fBposy - fHposy));	//角度を決める

				g_aBullet[nCntBullet].move.x = -sinf(fAngle)*17.5f;		//X座標更新
				g_aBullet[nCntBullet].move.y = -cosf(fAngle)*17.5f;		//Y座標更新
			}

			//弾の移動速度(moveが等倍だと、全体的に速く、難易度が高かったため、0.5をかけている)
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move*0.5;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y + (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y + (g_aBullet[nCntBullet].nSize / 2), 0.0f);

			//左に弾が出ていった時
			if (g_aBullet[nCntBullet].pos.x < g_aBullet[nCntBullet].nSize - (g_aBullet[nCntBullet].nSize / 2))
			{
				g_aBullet[nCntBullet].bUse = false;
			}

			//右に弾が出ていった時
			if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH - g_aBullet[nCntBullet].nSize + (g_aBullet[nCntBullet].nSize / 2))
			{
				g_aBullet[nCntBullet].bUse = false;
			}

			//上に弾が出ていった時
			if (g_aBullet[nCntBullet].pos.y < g_aBullet[nCntBullet].nSize - (g_aBullet[nCntBullet].nSize / 2))
			{
				g_aBullet[nCntBullet].bUse = false;
			}

			//地面にに弾が当たった時
			if (g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT - g_aBullet[nCntBullet].nSize + (g_aBullet[nCntBullet].nSize / 2) - 80)
			{
				//ボスとオクトパス以外の場合（オクトパスは地面を這う弾なので、未使用にしない）
				if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY || g_aBullet[nCntBullet].type == BULLETTYPE_ENEMYⅡ ||
					g_aBullet[nCntBullet].type == BULLETTYPE_GHO || g_aBullet[nCntBullet].type == BULLETTYPE_BOSS_PINCH)
				{
					g_aBullet[nCntBullet].nLife = 0;
				}

				//ボスの場合、ストームを発生させる
				if (g_aBullet[nCntBullet].type == BULLETTYPE_BOSS)
				{
					g_aBullet[nCntBullet].nLife = 0;
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(-10, -10, 0), 35, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 35);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(9.5, -9.5, 0), 38, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 30);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(-9.0, -9.0, 0), 41, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 25);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(8.5, -8.5, 0), 44, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 20);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(-8.0, -8.0, 0), 47, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 15);
					SetStorm(D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y - 35, 0.0f), D3DXVECTOR3(7.5, -7.5, 0), 50, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].pos.x, 10);
				}
			}

			//弾の射程を決める
			g_aBullet[nCntBullet].nLife--;
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				//爆発生成
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(255, 255, 255, 255), g_aBullet[nCntBullet].nSize);

				g_aBullet[nCntBullet].bUse = false;	//使用しない状態にする
			}

			//エフェクトをつける(オクトパス以外)
			if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY || g_aBullet[nCntBullet].type == BULLETTYPE_ENEMYⅡ
				|| g_aBullet[nCntBullet].type == BULLETTYPE_GHO || g_aBullet[nCntBullet].type == BULLETTYPE_BOSS ||
				g_aBullet[nCntBullet].type == BULLETTYPE_BOSS_PINCH)
			{
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(g_aBullet[nCntBullet].col.r, g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, 1.0f)
					, (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].nLife);
			}
		}

		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffBullet->Unlock();

}

//=============================================================================
// バレット描画処理
//=============================================================================
void DrawBullet(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBullet;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)			//弾が使用中なら描画
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, NUM_POLYGON);
		}
	}

}

//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type, int nLife, D3DXCOLOR col, int nSize)
{
	//変数宣言
	int nCntBullet;
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//位置を設定
			g_aBullet[nCntBullet].pos = pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (nSize / 2), g_aBullet[nCntBullet].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (nSize / 2), g_aBullet[nCntBullet].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (nSize / 2), g_aBullet[nCntBullet].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (nSize / 2), g_aBullet[nCntBullet].pos.y + (nSize / 2), 0.0f);

			//頂点カラーを設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].nSize = nSize;
			g_aBullet[nCntBullet].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//弾の取得
//=============================================================================
BULLET*GetBullet(void)
{
	return &g_aBullet[0];
}