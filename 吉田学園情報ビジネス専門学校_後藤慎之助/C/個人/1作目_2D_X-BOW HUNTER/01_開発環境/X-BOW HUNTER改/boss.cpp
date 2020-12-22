//=============================================================================
//
// DirectXシューティング [boss.cpp] (ファイヤーバード)
// Author :後藤慎之助
//
//=============================================================================
#include "boss.h"
#include "explosion.h"
#include "bullet.h"
#include <time.h>
#include "game.h"
#include "score.h"
#include "item.h"
#include "sound.h"
#include "enemy.h"
#include "title.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureBoss[MAX_TYPE_BOSS] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBoss = NULL;

int g_nCounterAnimBoss;			//アニメーションカウンター
int g_nPatternAnimBoss;			//アニメーションパターン
int g_nCntBoss = 1;				//弾をランダムに打つときに使用

BOSS g_aBoss[MAX_BOSS];			//ボスの情報
GAMELEVEL g_nBossLevel;			//ボスのレベル
								//=============================================================================
								//ボス初期化処理
								//=============================================================================
void InitBoss(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBoss;

	//弾の情報の初期化
	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		g_aBoss[nCntBoss].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;
		g_aBoss[nCntBoss].nType = 0;
		g_aBoss[nCntBoss].bUse = false;
	}
	g_nBossLevel = GetTitle();				//難易度を取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "boss000.png", &g_apTextureBoss[0]);	//ファイヤーバード

																			//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BOSS, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBoss, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

	pVtx[0].pos = D3DXVECTOR3(350.0f, 50.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(430.0f, 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(350.0f, 130.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(430.0f, 130.0f, 0.0f);

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

	//頂点データをアンロックする
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// ボス終了処理
//=============================================================================
void UninitBoss(void)
{
	//変数宣言
	int nCntTex;

	//テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_BOSS; nCntTex++)
	{
		if (g_apTextureBoss != NULL)
		{
			g_apTextureBoss[nCntTex]->Release();
			g_apTextureBoss[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffBoss != NULL)
	{
		g_pVtxBuffBoss->Release();
		g_pVtxBuffBoss = NULL;
	}
}

//=============================================================================
// ボス更新処理
//=============================================================================
void UpdateBoss(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntBoss;	//ボスを数える

					//アニメーション
	g_nCounterAnimBoss++;	//カウンタ加算
	if (g_nCounterAnimBoss == 9)//速さ
	{
		g_nCounterAnimBoss = 0;
		g_nPatternAnimBoss++;
		if (g_nPatternAnimBoss == 3)//枚数
		{
			g_nPatternAnimBoss = 0;
		}
	}

	//頂点バッファをロック
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true)
		{
			switch (g_aBoss[nCntBoss].state)
			{
			case BOSSSTATE_NORMAL:
				//ピンチなら、ピンチ状態とループ
				if (g_aBoss[nCntBoss].nLife < BOSS_LIFE / 4)
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_PINCH;

					//通常描画に戻す処理
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

				}
				break;
			case BOSSSTATE_PINCH:
				//難易度ノーマル
				if (g_nBossLevel == GAMELEVEL_NORMAL)
				{
					//必殺 追従火炎ブレス
					if ((rand() % (g_nCntBoss * 50)) == 0)
					{
						PlaySound(SOUND_LABEL_SE_SHOT3);	//ショット音
						SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(-30, 0, 0), BULLETTYPE_BOSS_PINCH, 95, D3DXCOLOR(255, 255, 255, 255), 30);
					}
				}
				//難易度ハード
				if (g_nBossLevel == GAMELEVEL_HARD)
				{
					//必殺 追従火炎ブレス
					if ((rand() % (g_nCntBoss * 25)) == 0)
					{
						PlaySound(SOUND_LABEL_SE_SHOT3);	//ショット音
						SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(-30, 0, 0), BULLETTYPE_BOSS_PINCH, 95, D3DXCOLOR(255, 255, 255, 255), 30);
					}
				}

				g_aBoss[nCntBoss].nCounterState--;
				if (g_aBoss[nCntBoss].nCounterState <= 0)
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;

					//ピンチ描画にする処理
					pVtx[0].col = D3DCOLOR_RGBA(246, 120, 59, 255);
					pVtx[1].col = D3DCOLOR_RGBA(246, 120, 59, 255);
					pVtx[2].col = D3DCOLOR_RGBA(246, 120, 59, 255);
					pVtx[3].col = D3DCOLOR_RGBA(246, 120, 59, 255);
				}
				break;
			case BOSSSTATE_DAMAGE:
				g_aBoss[nCntBoss].nCounterState--;
				if (g_aBoss[nCntBoss].nCounterState <= 0)
				{
					g_aBoss[nCntBoss].state = BOSSSTATE_NORMAL;

					//通常描画に戻す処理
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				break;
			}

			//難易度ノーマル
			if (g_nBossLevel == GAMELEVEL_NORMAL)
			{
				//弾の発射
				if ((rand() % (g_nCntBoss * 100)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(-10, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//弾の発射
				if ((rand() % (g_nCntBoss * 100)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(0, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//弾の発射
				if ((rand() % (g_nCntBoss * 100)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(10, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//ボスの移動
				g_aBoss[nCntBoss].pos -= g_aBoss[nCntBoss].move * 3;
			}
			//難易度ハード
			if (g_nBossLevel == GAMELEVEL_HARD)
			{
				//弾の発射
				if ((rand() % (g_nCntBoss * 20)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(-10, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//弾の発射
				if ((rand() % (g_nCntBoss * 20)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(0, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//弾の発射
				if ((rand() % (g_nCntBoss * 20)) == 0)
				{
					SetBullet(D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - 100, g_aBoss[nCntBoss].pos.y + 25, 0.0f), D3DXVECTOR3(10, 20, 0), BULLETTYPE_BOSS, 50, D3DXCOLOR(255, 255, 255, 255), 10);
				}

				//ボスの移動
				g_aBoss[nCntBoss].pos -= g_aBoss[nCntBoss].move * 5;
			}

			//ボスの移動範囲（X軸)
			if (g_aBoss[nCntBoss].pos.x < 0 + (BOSS_SIZE / 2) + 300 || g_aBoss[nCntBoss].pos.x > SCREEN_WIDTH - (BOSS_SIZE / 2))
			{
				g_aBoss[nCntBoss].move.x *= -1;
			}

			//ボスの移動範囲（Y軸)
			if (g_aBoss[nCntBoss].pos.y < 0 + (BOSS_SIZE / 2) || g_aBoss[nCntBoss].pos.y > SCREEN_HEIGHT - (BOSS_SIZE / 2) - 200)
			{
				g_aBoss[nCntBoss].move.y *= -1;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y - (BOSS_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y - (BOSS_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y + (BOSS_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y + (BOSS_SIZE / 2), 0.0f);

			//テクスチャ座標を更新
			pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBoss * 0.33f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBoss * 0.33f + 0.33f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBoss * 0.33f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBoss * 0.33f + 0.33f, 1.0f);

		}
		pVtx += 4;
	}
	//頂点データをアンロックする
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// ボス描画処理
//=============================================================================
void DrawBoss(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBoss;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBoss, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_apTextureBoss[0]);

	//ポリゴンの描画
	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == true)//敵が使用中なら描画
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureBoss[g_aBoss[nCntBoss].nType]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
// ボスの設定処理
//=============================================================================
void SetBoss(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nType)
{
	//変数宣言
	int nCntBoss;
	VERTEX_2D *pVtx;

	StopSound(SOUND_LABEL_BGM001);	//流れているBGMを止める
	PlaySound(SOUND_LABEL_BGM004);	//ボス戦のBGMをかける

									//頂点バッファをロック
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++)
	{
		if (g_aBoss[nCntBoss].bUse == false)
		{
			//位置を設定
			g_aBoss[nCntBoss].pos = pos;
			g_aBoss[nCntBoss].move = move;

			//敵の体力
			g_aBoss[nCntBoss].nLife = BOSS_LIFE;	//ファイヤーバード

													//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y - (BOSS_SIZE / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y - (BOSS_SIZE / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x - (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y + (BOSS_SIZE / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBoss[nCntBoss].pos.x + (BOSS_SIZE / 2), g_aBoss[nCntBoss].pos.y + (BOSS_SIZE / 2), 0.0f);

			g_aBoss[nCntBoss].bUse = true;
			break;
		}
	}

	//頂点データをアンロックする
	g_pVtxBuffBoss->Unlock();
}

//=============================================================================
// ボスの取得
//=============================================================================
BOSS*GetBoss(void)
{
	return &g_aBoss[0];
}

//=============================================================================
// ボスのダメージ設定
//=============================================================================
void HitBoss(int nCntBoss, int nDamage)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	PlaySound(SOUND_LABEL_SE_EXPLOSION);	//爆発音

											//頂点バッファをロック
	g_pVtxBuffBoss->Lock(0, 0, (void**)&pVtx, 0);

	g_aBoss[nCntBoss].nLife -= nDamage;
	if (g_aBoss[nCntBoss].nLife <= 0)
	{
		//爆発生成
		SetExplosion(g_aBoss[nCntBoss].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), BOSS_SIZE);

		g_aBoss[nCntBoss].bUse = false;

		//難易度ノーマル
		if (g_nBossLevel == GAMELEVEL_NORMAL)
		{
			ScoreEffect(11100);
		}
		//難易度ハード
		if (g_nBossLevel == GAMELEVEL_HARD)
		{
			ScoreEffect(15100);
		}

		StopSound(SOUND_LABEL_BGM004);	//ボス戦のBGMを止める

		PlaySound(SOUND_LABEL_SE_VOICE);	//ボス撃破音

		SetItem(g_aBoss[nCntBoss].pos, ITEMTYPE_001);
	}

	else if (g_aBoss[nCntBoss].nLife < BOSS_LIFE / 4)
	{
		g_aBoss[nCntBoss].state = BOSSSTATE_PINCH;
		g_aBoss[nCntBoss].nCounterState = 5;
		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);

		ScoreEffect(100);
	}

	else
	{
		//ダメージ状態に遷移
		g_aBoss[nCntBoss].state = BOSSSTATE_DAMAGE;
		g_aBoss[nCntBoss].nCounterState = 5;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);

		ScoreEffect(100);
	}
	//頂点データをアンロックする
	g_pVtxBuffBoss->Unlock();
}