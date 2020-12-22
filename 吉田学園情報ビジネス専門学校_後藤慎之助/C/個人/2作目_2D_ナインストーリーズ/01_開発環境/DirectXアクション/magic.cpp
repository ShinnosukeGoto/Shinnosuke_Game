//=============================================================================
//
// ナインストーリーズ [magic.cpp]の処理（プレイヤーの魔法）
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "magic.h"
#include "explosion.h"
#include "enemy.h"
#include "input.h"
#include "effect.h"
#include "bg.h"
#include "block.h"
#include "effect2.h"
#include "sound.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureMagic[MAGICTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffMagic = NULL;

int g_nCounterAnimLi;			//雷のアニメーションカウンター
int g_nPatternAnimLi;			//雷のアニメーションパターン

MAGIC g_aMagic[MAX_MAGIC];	//魔法の情報
bool g_bBgXM;		//背景が動いているかどうか取得する
bool g_bBlockXM;	//ブロックに当たっているかどうか取得する

//=============================================================================
//魔法の初期化処理
//=============================================================================
void InitMagic(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntMagic;

	//魔法の情報の初期化
	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		g_aMagic[nCntMagic].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic[nCntMagic].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aMagic[nCntMagic].bUse = false;
	}

	g_nCounterAnimLi = 0;	//雷のアニメーション
	g_nPatternAnimLi = 0;	//雷のアニメーション
	g_bBgXM = false;
	g_bBlockXM = false;

						//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/magic000.png", &g_pTextureMagic[0]);	//炎
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/magic001.png", &g_pTextureMagic[1]);	//雷
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/magic002.png", &g_pTextureMagic[2]);	//吸収

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_MAGIC, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffMagic, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//弾の数分
	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
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
	g_pVtxBuffMagic->Unlock();
}

//=============================================================================
// 魔法の終了処理
//=============================================================================
void UninitMagic(void)
{
	//変数宣言
	int nCntMagic;

	//テクスチャの開放
	for (nCntMagic = 0; nCntMagic < MAGICTYPE_MAX; nCntMagic++)
	{
		if (g_pTextureMagic[nCntMagic] != NULL)
		{
			g_pTextureMagic[nCntMagic]->Release();
			g_pTextureMagic[nCntMagic] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffMagic != NULL)
	{
		g_pVtxBuffMagic->Release();
		g_pVtxBuffMagic = NULL;
	}
}

//=============================================================================
// 魔法の更新処理
//=============================================================================
void UpdateMagic(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntMagic;
	VERTEX_2D *pVtx;
	ENEMY*pEnemy;
	int nCntEnemy;
	PLAYER*pPlayer;
	BLOCK*pBlock;

	//頂点バッファをロック
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nCntMagic].bUse == true)
		{

			//敵を取得
			pEnemy = GetEnemy();
			//自機を取得
			pPlayer = GetPlayer();
			//背景を取得
			g_bBgXM = GetBg();
			//ブロックに当たっているかどうかを取得
			g_bBlockXM = GetBlockX();
			//ブロックを取得
			pBlock = GetBlock();

			//敵への当たり判定
			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				if (pEnemy->bUse == true) //敵が使用されている
				{
					//ビートルの当たり判定
					if (pEnemy->type == ENEMYTYPE_000)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);

									//当たった時点で炎のHPを0にする
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//雷,吸収の当たり判定は全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 150);
								}
							}

							//吸収
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 50);

									//回復エフェクト
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//自機が回復
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
								}
							}
						}
					}

					//カラスの当たり判定
					if (pEnemy->type == ENEMYTYPE_001)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);

									//当たった時点で炎のHPを0にする
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//雷,吸収の当たり判定は全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 150);
								}
							}

							//吸収
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 50);

									//回復エフェクト
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//自機が回復
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
								}
							}
						}
					}

					//カラス亜種の当たり判定
					if (pEnemy->type == ENEMYTYPE_002)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_METⅡ / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_METⅡ / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_METⅡ / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_METⅡ / 2))
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);

									//当たった時点で炎のHPを0にする
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//雷,吸収の当たり判定は全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_METⅡ / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_METⅡ / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_METⅡ / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_METⅡ / 2))
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 150);
								}
							}

							//吸収
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 50);

									//回復エフェクト
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//自機が回復
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
								}
							}
						}
					}

					//ガイコツとダークガイコツの当たり判定
					if (pEnemy->type == ENEMYTYPE_003 || pEnemy->type == ENEMYTYPE_008)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);

									//当たった時点で炎のHPを0にする
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

			
						}

						//雷、吸収の当たり判定は全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 150);
								}
							}

							//吸収
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 50);

									//回復エフェクト
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//自機が回復
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
								}
							}
						}
					}

					//イノシシの当たり判定
					if (pEnemy->type == ENEMYTYPE_004)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);

									//当たった時点で炎のHPを0にする
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//雷,吸収の当たり判定は全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 150);
								}
							}

							//吸収
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 50);

									//回復エフェクト
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//自機が回復
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
								}
							}
						}
					}

					//コウモリの当たり判定
					if (pEnemy->type == ENEMYTYPE_005)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);

									//当たった時点で炎のHPを0にする
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//雷、吸収の当たり判定は全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 150);
								}
							}

							//吸収
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 50);

									//回復エフェクト
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//自機が回復
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
								}
							}
						}
					}

					//アイスドラゴンの当たり判定
					if (pEnemy->type == ENEMYTYPE_006)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_BOSS_X / 2) - 60 &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_BOSS_X / 2) + 60 &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_BOSS_Y / 2) + 80)
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 90);

									//当たった時点で炎のHPを0にする
									g_aMagic[nCntMagic].nLife = 0;
								}
							}

						}

						//雷、吸収の当たり判定は全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_BOSS_X / 2) - 60 &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_BOSS_X / 2) + 60 &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_BOSS_Y / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_BOSS_Y / 2) + 100)
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 65);
								}
							}

							//吸収
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 50);

									//回復エフェクト
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//自機が回復
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
								}
							}
						}
					}

					//ピラニアの当たり判定
					if (pEnemy->type == ENEMYTYPE_007)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pEnemy->pos.x + (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.x >= pEnemy->pos.x - (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.y <= pEnemy->pos.y + (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.y >= pEnemy->pos.y - (ENEMY_SIZE_PIR / 2))
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 80);

									//当たった時点で炎のHPを0にする
									g_aMagic[nCntMagic].nLife = 0;
								}
							}
						}

						//雷、吸収の当たり判定は全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pEnemy->pos.x + (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pEnemy->pos.x - (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pEnemy->pos.y + (ENEMY_SIZE_PIR / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pEnemy->pos.y - (ENEMY_SIZE_PIR / 2))
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 150);
								}
							}

							//吸収
							if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
							{
								if (pEnemy->state == ENEMYSTATE_NORMAL)	//多段ヒットなくすため、ノーマル時のみ
								{
									//敵にダメージ
									HitEnemy(nCntEnemy, 50);

									//回復エフェクト
									SetEffect2(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE, EFFECTTYPE_000);

									//自機が回復
									pPlayer->nLife += 25;

									PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音
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
					//つらら、壊せるブロックへの当たり判定
					if (pBlock->type == BLOCKTYPE_006 || pBlock->type == BLOCKTYPE_009)
					{
						//炎の当たり判定は魔法の中心座標
						if (g_aMagic[nCntMagic].pos.x <= pBlock->pos.x + (pBlock->nSizeX / 2) &&
							g_aMagic[nCntMagic].pos.x >= pBlock->pos.x - (pBlock->nSizeX / 2) &&
							g_aMagic[nCntMagic].pos.y <= pBlock->pos.y + (pBlock->nSizeY / 2) &&
							g_aMagic[nCntMagic].pos.y >= pBlock->pos.y - (pBlock->nSizeY / 2))
						{
							//炎
							if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
							{
								//ブロックにダメージ
								pBlock->nLife = 0;
							}
						}

						//雷の当たり判定は雷全体
						if (g_aMagic[nCntMagic].pos.x - g_aMagic[nCntMagic].nSize + 20 <= pBlock->pos.x + (pBlock->nSizeX / 2) &&
							g_aMagic[nCntMagic].pos.x + g_aMagic[nCntMagic].nSize - 20 >= pBlock->pos.x - (pBlock->nSizeX / 2) &&
							g_aMagic[nCntMagic].pos.y - g_aMagic[nCntMagic].nSize <= pBlock->pos.y + (pBlock->nSizeY / 2) &&
							g_aMagic[nCntMagic].pos.y + g_aMagic[nCntMagic].nSize >= pBlock->pos.y - (pBlock->nSizeY / 2))
						{
							//雷
							if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
							{
								//ブロックにダメージ
								pBlock->nLife = 0;
							}
						}
					}
				}
			}

			//プレイヤーの操作に対応した移動
			if (pPlayer->bDisp == true && g_bBgXM == true && g_bBlockXM == false)
			{
				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
				{
					g_aMagic[nCntMagic].pos.x -= VALUE_MOVE;
				}

				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
				{
					g_aMagic[nCntMagic].pos.x += VALUE_MOVE;
				}
			}

			//炎の魔法の移動
			if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
			{
				//頂点の座標設定
				pVtx[0].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);

				//重力
				g_aMagic[nCntMagic].move.y += GRAVITY;

				//位置更新
				g_aMagic[nCntMagic].pos.x += g_aMagic[nCntMagic].move.x;
				g_aMagic[nCntMagic].pos.y += g_aMagic[nCntMagic].move.y;
			}

			//雷の魔法の移動
			else if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
			{
				//アニメーション
				g_nCounterAnimLi++;	//カウンタ加算
				if (g_nCounterAnimLi == 3)//速さ
				{
					g_nCounterAnimLi = 0;
					g_nPatternAnimLi++;
					if (g_nPatternAnimLi == 2)//枚数
					{
						g_nPatternAnimLi = 0;
					}
				}

				//テクスチャ座標の設定
				if (g_nPatternAnimLi == 0)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
				else if (g_nPatternAnimLi == 1)
				{
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				g_aMagic[nCntMagic].pos.y += g_aMagic[nCntMagic].move.y;

				//頂点の座標設定
				pVtx[0].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), 0 - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), 0 - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);
			}

			//吸収の魔法の移動
			else if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
			{
				//頂点の座標設定
				pVtx[0].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y - (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].pos.y + (g_aMagic[nCntMagic].nSize / 2), 0.0f);

				//自機の位置を取得
				pPlayer = GetPlayer();

				float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//自機の座標
				float fMposx = g_aMagic[nCntMagic].pos.x, fMposy = g_aMagic[nCntMagic].pos.y;		//吸収魔法の座標
				float fAngle;											//角度

				fAngle = atan2f((fMposx - fPposx), (fMposy - fPposy));	//角度を決める

				g_aMagic[nCntMagic].move.x = -sinf(fAngle)*7.4f;		//X座標更新
				g_aMagic[nCntMagic].move.y = -cosf(fAngle)*7.4f;		//Y座標更新

				//位置更新
				g_aMagic[nCntMagic].pos.x += g_aMagic[nCntMagic].move.x;
				g_aMagic[nCntMagic].pos.y += g_aMagic[nCntMagic].move.y;
			}

			//魔法の体力を減らす
			g_aMagic[nCntMagic].nLife--;

			//魔法の体力が0
			if (g_aMagic[nCntMagic].nLife <= 0)
			{
				g_aMagic[nCntMagic].bUse = false;
			}

			//炎のときのエフェクト
			if (g_aMagic[nCntMagic].type == MAGICTYPE_FIRE)
			{
				SetEffect(g_aMagic[nCntMagic].pos, D3DXCOLOR(g_aMagic[nCntMagic].col.r, g_aMagic[nCntMagic].col.g, g_aMagic[nCntMagic].col.b, 1.0f)
					, (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].nLife / 4, DIRECT_RIGHT);
			}

			//雷のときのエフェクト
			else if (g_aMagic[nCntMagic].type == MAGICTYPE_LIGHTNING)
			{
				SetEffect(g_aMagic[nCntMagic].pos, D3DXCOLOR(g_aMagic[nCntMagic].col.r, g_aMagic[nCntMagic].col.g, g_aMagic[nCntMagic].col.b, 1.0f)
					, (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].nLife / 4, DIRECT_RIGHT);

				if (g_aMagic[nCntMagic].pos.y > SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80)
				{
					g_aMagic[nCntMagic].pos.y = SCREEN_HEIGHT - PLAYER_SIZE + (PLAYER_SIZE / 2) - 80;
					//爆発生成
					SetExplosion(g_aMagic[nCntMagic].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 125);
				}
			}

			//吸収のときのエフェクト
			else if (g_aMagic[nCntMagic].type == MAGICTYPE_DRAIN)
			{
				SetEffect(g_aMagic[nCntMagic].pos, D3DXCOLOR(g_aMagic[nCntMagic].col.r, g_aMagic[nCntMagic].col.g, g_aMagic[nCntMagic].col.b, 1.0f)
					, (g_aMagic[nCntMagic].nSize / 2), g_aMagic[nCntMagic].nLife / 4, DIRECT_RIGHT);
			}
		}

		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffMagic->Unlock();

}

//=============================================================================
// 魔法の描画処理
//=============================================================================
void DrawMagic(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntMagic;

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMagic, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//魔法の描画
	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nCntMagic].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMagic[g_aMagic[nCntMagic].type]);

			//魔法が使用中なら描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMagic * 4, NUM_POLYGON);
		}
	}
}

//=============================================================================
// 魔法の設定処理
//=============================================================================
void SetMagic(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, D3DXCOLOR col, MAGICTYPE type, int nSize)
{
	//変数宣言
	int nCntMagic;
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMagic->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMagic = 0; nCntMagic < MAX_MAGIC; nCntMagic++)
	{
		if (g_aMagic[nCntMagic].bUse == false)
		{
			//位置を設定
			g_aMagic[nCntMagic].pos = pos;
			g_aMagic[nCntMagic].nSize = nSize;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (nSize / 2), g_aMagic[nCntMagic].pos.y - (nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (nSize / 2), g_aMagic[nCntMagic].pos.y - (nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x - (nSize / 2), g_aMagic[nCntMagic].pos.y + (nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aMagic[nCntMagic].pos.x + (nSize / 2), g_aMagic[nCntMagic].pos.y + (nSize / 2), 0.0f);

			//頂点カラーを設定
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aMagic[nCntMagic].move = move;
			g_aMagic[nCntMagic].nLife = nLife;
			g_aMagic[nCntMagic].col = col;
			g_aMagic[nCntMagic].type = type;
			//g_aMagic[nCntMagic].nDirect = nDirect;
			g_aMagic[nCntMagic].bUse = true;

			break;
		}
		pVtx += 4;
	}

	//頂点データをアンロックする
	g_pVtxBuffMagic->Unlock();
}
//=============================================================================
//魔法の取得
//=============================================================================
MAGIC*GetMagic(void)
{
	return &g_aMagic[0];
}