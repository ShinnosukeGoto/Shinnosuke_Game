//=============================================================================
//
// X-Bow Hunter [enemy.cpp]の処理 (ザコ敵)
// Author :後藤慎之助
//
//=============================================================================
#include "enemy.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "boss.h"
#include "item.h"
#include "input.h"
#include "hunter.h"
#include "sound.h"
#include "title.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureEnemy[ENEMYTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEnemy = NULL;

int g_nCounterAnimEn;			//モスキートのアニメーションカウンター
int g_nPatternAnimEn;			//モスキートのアニメーションパターン

int g_nCounterAnimEn1;			//メタルバードのアニメーションカウンター
int g_nPatternAnimEn1;			//メタルバードのアニメーションパターン

int g_nCounterAnimEn2;			//グソクムシのアニメーションカウンター
int g_nPatternAnimEn2;			//グソクムシのアニメーションパターン

int g_nChangeAnimGso;			//グソクムシのテクスチャを反転させるときに使用

int g_CntEnemy = 1;				//弾をランダムに打つときに使用

int g_nNumEnemyStage1;			//ステージ1の敵の総数
int g_nNumEnemyStage2;			//ステージ2の敵の総数
int g_nNumEnemyStage3;			//ステージ3の敵の総数
int g_nNumEnemyStage4;			//ステージ3の敵の総数

ENEMY g_aEnemy[MAX_ENEMY];		//敵の情報
GAMELEVEL g_nEnemyLevel;		//敵のレベル

								//=============================================================================
								//エネミー初期化処理
								//=============================================================================
void InitEnemy(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy;
	g_nNumEnemyStage1 = ENEMY_STAGE1;		//ステージ1の初期化 (右の数字は敵の数)
	g_nNumEnemyStage2 = ENEMY_STAGE2;		//ステージ2の初期化 (右の数字は敵の数)
	g_nNumEnemyStage3 = ENEMY_STAGE3;		//ステージ3の初期化 (右の数字は敵の数)
	g_nNumEnemyStage4 = ENEMY_STAGE4;		//ステージ4の初期化 (右の数字は敵の数)
	g_nChangeAnimGso = 0;		//グソクムシの向きを初期化

								//敵の情報の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nLife = 0;
	}
	g_nEnemyLevel = GetTitle();				//難易度を取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "enemy000.png", &g_apTextureEnemy[0]);	//モスキート
	D3DXCreateTextureFromFile(pDevice, "enemy001.png", &g_apTextureEnemy[1]);	//メタルバード
	D3DXCreateTextureFromFile(pDevice, "enemy200.png", &g_apTextureEnemy[2]);	//メタルバード亜種
	D3DXCreateTextureFromFile(pDevice, "enemy300.png", &g_apTextureEnemy[3]);	//オクトパス
	D3DXCreateTextureFromFile(pDevice, "enemy003.png", &g_apTextureEnemy[4]);	//ゴースト
	D3DXCreateTextureFromFile(pDevice, "enemy005.png", &g_apTextureEnemy[5]);	//グソクムシ

																				//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ENEMY, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffEnemy, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//頂点座標の設定（右回りで設定する）
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

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

		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// エネミー終了処理
//=============================================================================
void UninitEnemy(void)
{
	//変数宣言
	int nCntTex;

	//テクスチャの開放
	for (nCntTex = 0; nCntTex < ENEMYTYPE_MAX; nCntTex++)
	{
		if (g_apTextureEnemy != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
// エネミー更新処理
//=============================================================================
void UpdateEnemy(void)
{
	//変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nCntGhost = 0, nAnimGhost = 0;		//ゴーストのアニメーションに使用

	//頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{

			//モスキート
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
			{
				//アニメーション
				g_nCounterAnimEn++;	//カウンタ加算
				if (g_nCounterAnimEn == 2)//速さ
				{
					g_nCounterAnimEn = 0;
					g_nPatternAnimEn++;
					if (g_nPatternAnimEn == 2)//枚数
					{
						g_nPatternAnimEn = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//難易度ノーマル
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 1046)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 1046)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-4, 6, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 1046)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}
					}
					//難易度ハード
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 46)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 46)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-4, 6, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 46)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 203, 100, 155), 15);
						}
					}

					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//モスキートの移動
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MOS / 2), 0.0f);

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn * 0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn * 0.5f + 0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn * 0.5f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn * 0.5f + 0.5f, 1.0f);

				//モスキートの移動範囲
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_MOS / 2) + 640 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - (ENEMY_SIZE_MOS / 2) - 100)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
				}

				if (g_aEnemy[nCntEnemy].pos.y < 0 + (ENEMY_SIZE_MOS / 2) + 200 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - (ENEMY_SIZE_MOS / 2) - 300)
				{
					g_aEnemy[nCntEnemy].move.y *= -1;
				}
			}

			//メタルバード
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
			{
				//アニメーション
				g_nCounterAnimEn1++;	//カウンタ加算
				if (g_nCounterAnimEn1 == 8)//速さ
				{
					g_nCounterAnimEn1 = 0;
					g_nPatternAnimEn1++;
					if (g_nPatternAnimEn1 == 3)//枚数
					{
						g_nPatternAnimEn1 = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//難易度ノーマル
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 530)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 530)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(4, 6, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 530)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}
					}
					//難易度ハード
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 30)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 30)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(4, 6, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 30)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_ENEMY, 150, D3DXCOLOR(0, 150, 200, 155), 25);
						}
					}

					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//難易度ノーマル
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					//メタルバードの移動
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2;
				}
				//難易度ハード
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					//メタルバードの移動
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3;
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET / 2), 0.0f);

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f + 0.333f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f + 0.333f, 1.0f);


				//メタルバードの移動範囲
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_MET / 2) + 440 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - (ENEMY_SIZE_MET / 2) - 50)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
				}

				if (g_aEnemy[nCntEnemy].pos.y < 0 + (ENEMY_SIZE_MET / 2) + 150 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - (ENEMY_SIZE_MET / 2) - 110)
				{
					g_aEnemy[nCntEnemy].move.y *= -1;
				}
			}

			//メタルバード亜種
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
			{
				//アニメーション
				g_nCounterAnimEn1++;	//カウンタ加算
				if (g_nCounterAnimEn1 == 8)//速さ
				{
					g_nCounterAnimEn1 = 0;
					g_nPatternAnimEn1++;
					if (g_nPatternAnimEn1 == 3)//枚数
					{
						g_nPatternAnimEn1 = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//難易度ノーマル
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(6, 10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(3, 10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-6, 10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(6, -10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, -10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 140)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-6, -10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}
					}
					//難易度ハード
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(6, 10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(3, 10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-6, 10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(6, -10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-3, -10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(200, 50, 200, 155), 30);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 40)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-6, -10, 0), BULLETTYPE_ENEMYⅡ, 150, D3DXCOLOR(0, 100, 200, 155), 30);
						}
					}

					if (g_aEnemy[nCntEnemy].nLife < ENEMY_LIFE_METⅡ / 4)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_PINCH;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

					}

					break;

				case ENEMYSTATE_PINCH:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//ピンチ描画にする処理
						pVtx[0].col = D3DCOLOR_RGBA(250, 199, 33, 255);
						pVtx[1].col = D3DCOLOR_RGBA(250, 199, 33, 255);
						pVtx[2].col = D3DCOLOR_RGBA(250, 199, 33, 255);
						pVtx[3].col = D3DCOLOR_RGBA(250, 199, 33, 255);
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//難易度ノーマル
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					//メタルバード亜種の移動
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.5;
				}
				//難易度ハード
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					//メタルバード亜種の移動
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3.5;
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_METⅡ / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_METⅡ / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_METⅡ / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_METⅡ / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_METⅡ / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_METⅡ / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_METⅡ / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_METⅡ / 2), 0.0f);

				//テクスチャ座標を更新
				pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f + 0.333f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn1 * 0.333f + 0.333f, 1.0f);

				//メタルバード亜種の移動範囲
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_METⅡ / 2) + 240 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - (ENEMY_SIZE_METⅡ / 2) - 450)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
				}

				if (g_aEnemy[nCntEnemy].pos.y < 0 + (ENEMY_SIZE_METⅡ / 2) + 100 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - (ENEMY_SIZE_METⅡ / 2) - 110)
				{
					g_aEnemy[nCntEnemy].move.y *= -1;
				}

			}

			//オクトパス
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//難易度ノーマル
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 300)) == 0)
						{
							//ハンターの位置を取得
							HUNTER*pHunter;
							pHunter = GetHunter();

							float fHposx = pHunter->pos.x, fHposy = pHunter->pos.y;							//目標(ハンター)の座標
							float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;	//敵の座標
							float fAngle;																	//角度

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//角度を決める
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*14.5f, -cosf(fAngle)*14.5f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(0, 200, 100, 255), 50);
							PlaySound(SOUND_LABEL_SE_SHOT2);	//オクトパスのショット音
						}
					}
					//難易度ハード
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 70)) == 0)
						{
							//ハンターの位置を取得
							HUNTER*pHunter;
							pHunter = GetHunter();

							float fHposx = pHunter->pos.x, fHposy = pHunter->pos.y;							//目標(ハンター)の座標
							float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;	//敵の座標
							float fAngle;																	//角度

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//角度を決める
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*14.5f, -cosf(fAngle)*14.5f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(0, 200, 100, 255), 50);
							PlaySound(SOUND_LABEL_SE_SHOT2);	//オクトパスのショット音
						}
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//ゴースト
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//難易度ノーマル
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, -2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 155), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, -4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 3, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, -2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, -4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 450)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 450)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 3, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 550)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 5, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}
					}
					//難易度ハード
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//弾の発射
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, -2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 155), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, -4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 3, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(-5, 5, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, -2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 250)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, -4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 2, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 3, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 4, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}

						//弾の発射
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 15, g_aEnemy[nCntEnemy].pos.y, 0.0f), D3DXVECTOR3(5, 5, 0), BULLETTYPE_GHO, 300, D3DXCOLOR(0, 0, 200, 255), 40);
						}
					}

					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//ゴーストの移動
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.5;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GHO / 2), 0.0f);

				//Aキーが押されていない間は、恥ずかしがり状態
				pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 105);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 105);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 105);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 105);

				//Aキーが押された（恥ずかしがり状態解除）
				if (GetKeyboardPress(DIK_A))
				{
					nCntGhost = 1;
				}

				//恥ずかしがり状態を解除する
				if (nCntGhost > 0)
				{
					nCntGhost = 0;
					nAnimGhost = 1;
				}

				//テクスチャを恥ずかしがり状態から解除
				if (nAnimGhost == 1)
				{
					nAnimGhost = 0;
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}

				//ゴーストの移動範囲
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_GHO / 2) + 440 || g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH - (ENEMY_SIZE_GHO / 2) - 350)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
				}

				if (g_aEnemy[nCntEnemy].pos.y < 0 + (ENEMY_SIZE_GHO / 2) + 200 || g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - (ENEMY_SIZE_GHO / 2) - 210)
				{
					g_aEnemy[nCntEnemy].move.y *= -1;
				}
			}

			//グソクムシ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
			{
				//アニメーション
				g_nCounterAnimEn2++;	//カウンタ加算
				if (g_nCounterAnimEn2 == 8)//速さ
				{
					g_nCounterAnimEn2 = 0;
					g_nPatternAnimEn2++;
					if (g_nPatternAnimEn2 == 2)//枚数
					{
						g_nPatternAnimEn2 = 0;
					}
				}

				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//グソクムシの移動
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 5;

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GSO / 2), 0.0f);

				//グソクムシの移動範囲
				if (g_aEnemy[nCntEnemy].pos.x < 0 + (ENEMY_SIZE_GSO / 2) + 640)
				{
					g_aEnemy[nCntEnemy].move.x *= -1;
					g_nChangeAnimGso = 1;
				}

				//テクスチャが左向き
				if (g_nChangeAnimGso == 0)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f + 0.5f, 1.0f);
				}

				//テクスチャが右向き
				if (g_nChangeAnimGso == 1)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimEn2*0.5f, 1.0f);
				}

				//画面を出たら未使用に
				if (g_aEnemy[nCntEnemy].pos.x > SCREEN_WIDTH + (ENEMY_SIZE_GSO / 2))
				{
					g_aEnemy[nCntEnemy].bUse = false;
					g_nChangeAnimGso = 0;	//次に出てくるグソクムシを左向きにする
				}
			}
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffEnemy->Unlock();

}

//=============================================================================
// エネミー描画処理
//=============================================================================
void DrawEnemy(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	//ポリゴンの描画
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)//敵が使用中なら描画
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].type]);

			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEnemy, NUM_POLYGON);
		}
	}
}

//=============================================================================
// エネミーの設定処理
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type)
{
	//変数宣言
	int nCntEnemy;
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			//位置を設定
			g_aEnemy[nCntEnemy].pos = pos;

			g_aEnemy[nCntEnemy].move = move;

			g_aEnemy[nCntEnemy].type = type;

			//モスキート
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MOS;
			}

			//メタルバード
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MET;
			}

			//メタルバード亜種
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_METⅡ;
			}

			//オクトパス
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_OCT;
			}

			//ゴースト
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_GHO;
			}

			//グソクムシ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_GSO;
				PlaySound(SOUND_LABEL_SE_STAR);
			}

			//頂点カラーを設定（0～255の値で設定する）
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffEnemy->Unlock();

}

//=============================================================================
// エネミーの取得
//=============================================================================
ENEMY*GetEnemy(void)
{
	return &g_aEnemy[0];
}

//=============================================================================
// エネミーのダメージ設定
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	PlaySound(SOUND_LABEL_SE_EXPLOSION);	//爆発音

											//頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	//モスキート
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
	{
		if (g_aEnemy[nCntEnemy].nLife == 0)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MOS);

			g_aEnemy[nCntEnemy].bUse = false;

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(510);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(1010);
			}

			g_nNumEnemyStage1--;
		}
		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//ダメージ状態に遷移
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;

				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
				{
					//頂点カラーの設定
					pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
					pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
					pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
					pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				}
				pVtx += 4;
			}
			ScoreEffect(10);
		}
	}

	//メタルバード
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
	{
		if (g_aEnemy[nCntEnemy].nLife == 0)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MET);

			g_aEnemy[nCntEnemy].bUse = false;

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(760);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(1510);
			}

			g_nNumEnemyStage2--;

		}

		else if (g_aEnemy[nCntEnemy].nLife == (ENEMY_LIFE_MET / 2))
		{
			//ダメージ状態に遷移
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			//体力半分でオクトパスが出現
			SetEnemy(D3DXVECTOR3(1000, 590, 0.0f), D3DXVECTOR3(0, 0, 0.0f), ENEMYTYPE_003);

			ScoreEffect(10);
		}

		else
		{
			//ダメージ状態に遷移
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			ScoreEffect(10);
		}
	}

	//メタルバード亜種
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
	{
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_METⅡ);

			g_aEnemy[nCntEnemy].bUse = false;

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(1510);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(3010);
			}

			g_nNumEnemyStage4--;
		}

		else if (g_aEnemy[nCntEnemy].nLife == (ENEMY_LIFE_METⅡ / 2))
		{
			//ダメージ状態に遷移
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			//体力半分でグソクムシが出現
			SetEnemy(D3DXVECTOR3(1200, 590, 0.0f), D3DXVECTOR3(-1, 0, 0.0f), ENEMYTYPE_005);

			ScoreEffect(10);
		}

		else if (g_aEnemy[nCntEnemy].nLife < (ENEMY_LIFE_METⅡ / 4))
		{
			//ピンチ状態に遷移
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_PINCH;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			ScoreEffect(10);
		}

		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//ダメージ状態に遷移
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			}

			ScoreEffect(10);
		}
	}

	//オクトパス
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
	{
		if (g_aEnemy[nCntEnemy].nLife == 0)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_OCT);

			g_aEnemy[nCntEnemy].bUse = false;

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(1510);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(3010);
			}

			g_nNumEnemyStage2--;
		}

		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//ダメージ状態に遷移
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;

				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx += 4;
			}
			ScoreEffect(10);
		}
	}

	//ゴースト
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
	{
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_GHO);

			g_aEnemy[nCntEnemy].bUse = false;

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(2510);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(5010);
			}

			g_nNumEnemyStage3--;
		}

		else if (g_aEnemy[nCntEnemy].nLife == (ENEMY_LIFE_GHO / 2))
		{
			//ダメージ状態に遷移
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

			//体力半分でグソクムシが出現
			SetEnemy(D3DXVECTOR3(1200, 590, 0.0f), D3DXVECTOR3(-1, 0, 0.0f), ENEMYTYPE_005);

			ScoreEffect(10);
		}

		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//ダメージ状態に遷移
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			}

			ScoreEffect(10);
		}
	}

	//グソクムシ
	if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
	{
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_GHO);

			g_aEnemy[nCntEnemy].bUse = false;

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(5010);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(10010);
			}

			g_nChangeAnimGso = 0;	//次に出てくるグソクムシを左向きにする
		}

		else
		{
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
				//ダメージ状態に遷移
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
				g_aEnemy[nCntEnemy].nCounterState = 5;
				//頂点カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
				pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			}

			ScoreEffect(10);
		}
	}

	//頂点データをアンロックする
	g_pVtxBuffEnemy->Unlock();

	//ステージ1から2へ
	if (g_nNumEnemyStage1 == 0)	//0以下にすると、同時の矢で倒したときにバグるため0と同値にする
	{
		SetEnemy(D3DXVECTOR3(1180, 240, 0.0f), D3DXVECTOR3(-1.0, 1.0, 0.0f), ENEMYTYPE_001);
		g_nNumEnemyStage1 = 10;	//使い終わった数字は、バグを回避するため10にする
	}

	//ステージ2から3へ
	if (g_nNumEnemyStage2 == 0)	//0以下にすると、同時の矢で倒したときにバグるため0と同値にする
	{
		SetEnemy(D3DXVECTOR3(840, 300, 0.0f), D3DXVECTOR3(-0.5, 0.5, 0.0f), ENEMYTYPE_004);
		g_nNumEnemyStage2 = 10;	//使い終わった数字は、バグを回避するため10にする
	}

	//ステージ3から4へ
	if (g_nNumEnemyStage3 == 0)	//0以下にすると、同時の矢で倒したときにバグるため0と同値にする
	{
		SetEnemy(D3DXVECTOR3(540, 200, 0.0f), D3DXVECTOR3(1.0, 1.0, 0.0f), ENEMYTYPE_002);
		g_nNumEnemyStage3 = 10;	//使い終わった数字は、バグを回避するため10にする
	}

	//ステージ4からアイテム(羽)へ
	if (g_nNumEnemyStage4 == 0)	//0以下にすると、同時の矢で倒したときにバグるため0と同値にする
	{
		SetItem(g_aEnemy[nCntEnemy].pos, ITEMTYPE_000);
		g_nNumEnemyStage4 = 10;	//使い終わった数字は、バグを回避するため10にする
	}

}