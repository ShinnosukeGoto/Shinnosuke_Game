//=============================================================================
//
// ナインストーリーズ [attack.cpp]の処理（プレイヤーの攻撃）
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "attack.h"
#include "explosion.h"
#include "enemy.h"
#include "input.h"
#include "effect.h"
#include "block.h"


//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureAttack[ATTACKTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffAttack = NULL;
ATTACK g_aAttack[MAX_ATTACK];	//攻撃の情報
int g_nUseAttack;			//攻撃を使った回数

//=============================================================================
//攻撃の初期化処理
//=============================================================================
void InitAttack(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntAttack;

	//弾の情報の初期化
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		g_aAttack[nCntAttack].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aAttack[nCntAttack].bUse = false;
	}

	g_nUseAttack = 0;	//使った回数の初期化

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack004.png", &g_pTextureAttack[0]);	//剣
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack002.png", &g_pTextureAttack[1]);	//槍
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack002.png", &g_pTextureAttack[2]);	//槍(下攻撃用)
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack200.png", &g_pTextureAttack[3]);	//弓
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/attack200.png", &g_pTextureAttack[4]);	//弓(下攻撃用)

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_ATTACK, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffAttack, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//弾の数分
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
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

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データをアンロックする
	g_pVtxBuffAttack->Unlock();
}

//=============================================================================
// 攻撃の終了処理
//=============================================================================
void UninitAttack(void)
{
	//変数宣言
	int nCntAttack;

	//テクスチャの開放
	for (nCntAttack = 0; nCntAttack < ATTACKTYPE_MAX; nCntAttack++)
	{
		if (g_pTextureAttack[nCntAttack] != NULL)
		{
			g_pTextureAttack[nCntAttack]->Release();
			g_pTextureAttack[nCntAttack] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffAttack != NULL)
	{
		g_pVtxBuffAttack->Release();
		g_pVtxBuffAttack = NULL;
	}
}

//=============================================================================
// 攻撃の更新処理
//=============================================================================
void UpdateAttack(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntAttack;
	VERTEX_2D *pVtx;
	ENEMY*pEnemy;
	int nCntEnemy;
	BLOCK*pBlock;

	//頂点バッファをロック
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//敵を取得
			pEnemy = GetEnemy();
			
			//ブロックを取得
			pBlock = GetBlock();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true) //敵が使用されている
				{
					if (pEnemy->state == ENEMYSTATE_NORMAL)
					{
						//ビートルの当たり判定
						if (pEnemy->type == ENEMYTYPE_000)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
							{
								//剣
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 60);
								}

								//槍
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 40);
								}

								//急降下攻撃
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);
								}

								//弓
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 30);
									//当たった時点で弾のHPを0にする
									g_aAttack[nCntAttack].fLife = 0;
								}

							}
						}

						//カラスの当たり判定
						if (pEnemy->type == ENEMYTYPE_001)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
							{
								//剣
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 60);
								}

								//槍
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 40);
								}

								//急降下攻撃
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);
								}

								//弓
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 30);
									//当たった時点で弾のHPを0にする
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//カラス亜種の当たり判定
						if (pEnemy->type == ENEMYTYPE_002)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_METⅡ / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_METⅡ / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_METⅡ / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_METⅡ / 2))
							{
								//剣
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 60);
								}

								//槍
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 40);
								}

								//急降下攻撃
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);
								}

								//弓
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 30);
									//当たった時点で弾のHPを0にする
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//ガイコツとダークガイコツの当たり判定
						if (pEnemy->type == ENEMYTYPE_003 || pEnemy->type == ENEMYTYPE_008)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
							{
								//剣
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 60);

								}

								//槍
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 40);
								}

								//急降下攻撃
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);
								}

								//弓
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 30);
									//当たった時点で弾のHPを0にする
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//イノシシの当たり判定
						if (pEnemy->type == ENEMYTYPE_004)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
							{
								//剣
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 60);

								}

								//槍
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 40);
								}

								//急降下攻撃
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);
								}

								//弓
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 30);
									//当たった時点で弾のHPを0にする
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//コウモリの当たり判定
						if (pEnemy->type == ENEMYTYPE_005)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
							{
								//剣
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 60);

								}

								//槍
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 40);
								}

								//急降下攻撃
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);
								}

								//弓
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//敵にダメージ(コウモリは弓矢貫通)
									HitEnemy(nCntEnemy, 30);
								}
							}
						}

						//アイスドラゴンの当たり判定
						if (pEnemy->type == ENEMYTYPE_006)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_BOSS_X / 2) - 60 &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_BOSS_X / 2) + 60 &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_BOSS_Y / 2) + 80)
							{
								//剣
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 40);

								}

								//槍
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 30);
								}

								//急降下攻撃
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 60);
								}

								//弓
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 25);
									//当たった時点で弾のHPを0にする
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}

						//ピラニアの当たり判定
						if (pEnemy->type == ENEMYTYPE_007)
						{
							if (g_aAttack[nCntAttack].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_PIR / 2) &&
								g_aAttack[nCntAttack].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_PIR / 2) &&
								g_aAttack[nCntAttack].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_PIR / 2) &&
								g_aAttack[nCntAttack].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_PIR / 2))
							{
								//剣
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 60);

								}

								//槍
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 40);
								}

								//急降下攻撃
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);
								}

								//弓
								if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 30);
									//当たった時点で弾のHPを0にする
									g_aAttack[nCntAttack].fLife = 0;
								}
							}
						}
					}
				}
			}

			//ブロックへの当たり判定
			for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
			{
				if (pBlock->bUse == true) //ブロックが使用されている
				{
					//壊せるブロックへの当たり判定
					if (pBlock->type == BLOCKTYPE_009)
					{
						//当たり判定は攻撃の中心座標
						if (g_aAttack[nCntAttack].pos.x <= pBlock->pos.x + (pBlock->nSizeX / 2) &&
							g_aAttack[nCntAttack].pos.x >= pBlock->pos.x - (pBlock->nSizeX / 2) &&
							g_aAttack[nCntAttack].pos.y <= pBlock->pos.y + (pBlock->nSizeY / 2) &&
							g_aAttack[nCntAttack].pos.y >= pBlock->pos.y - (pBlock->nSizeY / 2))
						{
							//ブロックにダメージ
							pBlock->nLife = 0;
						}
					}
				}
			}

			//攻撃の体力を減らす
			g_aAttack[nCntAttack].fLife--;

			//攻撃の体力が0
			if (g_aAttack[nCntAttack].fLife <= 0)
			{
				////爆発生成
				//SetExplosion(g_aAttack[nCntAttack].pos, D3DXCOLOR(255, 255, 255, 255), g_aAttack[nCntAttack].nSize);
				g_aAttack[nCntAttack].bUse = false;
			}

			//頂点の座標設定
			pVtx[0].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x - (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].pos.y - (g_aAttack[nCntAttack].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x + (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].pos.y - (g_aAttack[nCntAttack].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x - (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].pos.y + (g_aAttack[nCntAttack].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x + (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].pos.y + (g_aAttack[nCntAttack].nSize / 2), 0.0f);

			//剣の移動量
			if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD)
			{
				if (g_aAttack[nCntAttack].fLife < 11)
				{
					//攻撃の移動量は少なめ
					g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move*0.6;
				}
				else
				{
					//攻撃の移動量
					g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move;
				}
			}
			//槍の移動量
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
			{
				if (g_aAttack[nCntAttack].fLife < 11)
				{
					//攻撃の移動量
					g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move;
				}
				else
				{
					//攻撃の移動量は少なめ
					g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move*0.7;
				}
			}
			//弓の移動量
			else if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW || g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
			{
				//攻撃の移動量
				g_aAttack[nCntAttack].pos += g_aAttack[nCntAttack].move;
			}

			//上向き
			if (g_aAttack[nCntAttack].nDirect == DIRECT_UP)
			{
				//剣・槍のときのエフェクト
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
				{
					//テクスチャ座標の設定
					pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);

					SetEffect(g_aAttack[nCntAttack].pos, D3DXCOLOR(g_aAttack[nCntAttack].col.r, g_aAttack[nCntAttack].col.g, g_aAttack[nCntAttack].col.b, 1.0f)
						, (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].fLife / 2, DIRECT_UP);
				}

				//弓のときのエフェクト
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW)
				{
					//テクスチャ座標の設定
					pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f, 1.0f);
				}
			}

			//下向き
			if (g_aAttack[nCntAttack].nDirect == DIRECT_DOWN)
			{
				//剣・槍のときのエフェクト
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

					SetEffect(g_aAttack[nCntAttack].pos, D3DXCOLOR(g_aAttack[nCntAttack].col.r, g_aAttack[nCntAttack].col.g, g_aAttack[nCntAttack].col.b, 1.0f)
						, (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].fLife / 2, DIRECT_DOWN);
				}

				//弓のときのエフェクト(右向き)
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				//弓のときのエフェクト(左向き)
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW2)
				{
					//テクスチャ座標の設定
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
			}

			//右向き
			if (g_aAttack[nCntAttack].nDirect == DIRECT_RIGHT)
			{
				//剣・槍のときのエフェクト
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

					SetEffect(g_aAttack[nCntAttack].pos, D3DXCOLOR(g_aAttack[nCntAttack].col.r, g_aAttack[nCntAttack].col.g, g_aAttack[nCntAttack].col.b, 1.0f)
						, (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].fLife / 2, DIRECT_RIGHT);
				}

				//弓のときのエフェクト
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
				}
			}

			//左向き
			if (g_aAttack[nCntAttack].nDirect == DIRECT_LEFT)
			{
				//剣・槍のときのエフェクト
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_SWORD || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE || g_aAttack[nCntAttack].type == ATTACKTYPE_LANCE2)
				{
					//テクスチャ座標の設定
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);

					SetEffect(g_aAttack[nCntAttack].pos, D3DXCOLOR(g_aAttack[nCntAttack].col.r, g_aAttack[nCntAttack].col.g, g_aAttack[nCntAttack].col.b, 1.0f)
						, (g_aAttack[nCntAttack].nSize / 2), g_aAttack[nCntAttack].fLife / 2, DIRECT_LEFT);
				}

				//弓のときのエフェクト
				if (g_aAttack[nCntAttack].type == ATTACKTYPE_BOW)
				{
					//テクスチャ座標の設定
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
				}
			}

		}

		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffAttack->Unlock();

}

//=============================================================================
// 攻撃の描画処理
//=============================================================================
void DrawAttack(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntAttack;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffAttack, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//攻撃の描画
	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureAttack[g_aAttack[nCntAttack].type]);

			//攻撃が使用中なら描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAttack * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// 攻撃の設定処理
//=============================================================================
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLife, D3DXCOLOR col, ATTACKTYPE type, int nSize, DIRECT nDirect)
{
	//変数宣言
	int nCntAttack;
	VERTEX_2D *pVtx;
	/*ITEM*pItem = GetItem();*/

	//撃った攻撃の数を加算
	g_nUseAttack++;

	//頂点バッファをロック
	g_pVtxBuffAttack->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntAttack = 0; nCntAttack < MAX_ATTACK; nCntAttack++)
	{
		if (g_aAttack[nCntAttack].bUse == false)
		{
			//位置を設定
			g_aAttack[nCntAttack].pos = pos;
			g_aAttack[nCntAttack].nSize = nSize;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x - (nSize / 2), g_aAttack[nCntAttack].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x + (nSize / 2), g_aAttack[nCntAttack].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x - (nSize / 2), g_aAttack[nCntAttack].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aAttack[nCntAttack].pos.x + (nSize / 2), g_aAttack[nCntAttack].pos.y + (nSize / 2), 0.0f);

			//頂点カラーを設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aAttack[nCntAttack].move = move;
			g_aAttack[nCntAttack].fLife = fLife;
			g_aAttack[nCntAttack].col = col;
			g_aAttack[nCntAttack].type = type;
			g_aAttack[nCntAttack].nDirect = nDirect;
			g_aAttack[nCntAttack].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffAttack->Unlock();
}
//=============================================================================
//攻撃の取得
//=============================================================================
ATTACK*GetAttack(void)
{
	return &g_aAttack[0];
}

//=============================================================================
//攻撃の使用回数の取得
//=============================================================================
int GetUseAttack(void)
{
	return g_nUseAttack;
}