//=============================================================================
//
// ナインストーリーズ [enemy.cpp]の処理 (ゲームに出てくる敵)
// Author :後藤慎之助
//
//=============================================================================
#include "enemy.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "item.h"
#include "input.h"
#include "player.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "bg.h"
#include "block.h"
#include "effect2.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_apTextureEnemy[ENEMYTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffEnemy = NULL;

int g_CntEnemy = 1;				//弾をランダムに打つときに使用
ENEMY g_aEnemy[MAX_ENEMY];		//敵の情報
GAMELEVEL g_nEnemyLevel;		//敵のレベル
bool g_bHitEnemy;				//敵に攻撃が当たったかどうか
bool g_bBgXE;					//ステージ1の横が動くかどうか
bool g_bBlockXE;				//ブロックに横から当たった
int g_nCntBat;					//コウモリを倒した数を数える
int g_nCntBorn;					//ガイコツを倒した数を数える
MODE g_ModeEnemy;				//ゲーム画面に対応した敵

//=============================================================================
//エネミー初期化処理
//=============================================================================
void InitEnemy(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy;

	//グローバル変数の初期化
	g_bHitEnemy = false;		//敵に攻撃が当たったかどうかを初期化
	g_bBgXE = false;
	g_bBlockXE = false;
	g_nCntBat = 0;
	g_nCntBorn = 0;
	g_ModeEnemy = MODE_GAME;

	//敵の情報の初期化
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].type = ENEMYTYPE_000;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nPosX = 0;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].bLeft = true;
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nLife = 0;
		g_aEnemy[nCntEnemy].nAttackTime = 0;
	}
	g_nEnemyLevel = GetTitle();				//難易度を取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy000.png", &g_apTextureEnemy[0]);	//ビートル
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy001.png", &g_apTextureEnemy[1]);	//カラス
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy002.png", &g_apTextureEnemy[2]);	//カラス亜種
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy003.png", &g_apTextureEnemy[3]);	//ガイコツ
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy004.png", &g_apTextureEnemy[4]);	//イノシシ
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy005.png", &g_apTextureEnemy[5]);	//コウモリ
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy006.png", &g_apTextureEnemy[6]);	//アイスドラゴン
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy007.png", &g_apTextureEnemy[7]);	//ピラニア
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/enemy008.png", &g_apTextureEnemy[8]);	//ダークガイコツ

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
		if (g_apTextureEnemy[nCntTex] != NULL)
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
	PLAYER *pPlayer;
	int nCntGhost = 0, nAnimGhost = 0;		//イノシシのアニメーションに使用

											//頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//プレイヤーを取得
			pPlayer = GetPlayer();
			//背景の横が動くかどうか取得
			g_bBgXE = GetBg();
			//ブロックに横から当たったかどうか取得
			g_bBlockXE = GetBlockX();
			//ゲーム画面を取得
			g_ModeEnemy = GetMode();

			//ビートル
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
			{
				//アニメーション
				g_aEnemy[nCntEnemy].nCounterAnim++;	//カウンタ加算
				if (g_aEnemy[nCntEnemy].nCounterAnim == 60)//速さ
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 2)//枚数
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//ビートルの移動
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;
				g_aEnemy[nCntEnemy].nPosX += 1;

				//プレイヤーの操作に対応した移動
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//ビートルの移動範囲
				if (g_aEnemy[nCntEnemy].nPosX > 60)
				{
					//向きを変える
					g_aEnemy[nCntEnemy].bLeft = !g_aEnemy[nCntEnemy].bLeft;	//bool型のトグル
					g_aEnemy[nCntEnemy].move.x *= -1;
					g_aEnemy[nCntEnemy].nPosX *= -1;	
				}

				//テクスチャが左向き
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
				}

				//テクスチャが右向き
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MOS / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MOS / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MOS / 2), 0.0f);

			}

			//カラス
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
			{
				//アニメーション
				g_aEnemy[nCntEnemy].nCounterAnim++;	//カウンタ加算
				if (g_aEnemy[nCntEnemy].nCounterAnim == 8)//速さ
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 3)//枚数
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//カラスの移動
				//難易度ノーマル
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.0;
				}
				//難易度ハード
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3.0;
				}
				g_aEnemy[nCntEnemy].nPosX += 1;

				//プレイヤーの操作に対応した移動
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//カラスの移動範囲
				if (g_aEnemy[nCntEnemy].nPosX > 150)
				{
					//向きを変える
					g_aEnemy[nCntEnemy].bLeft = !g_aEnemy[nCntEnemy].bLeft;	//bool型のトグル
					g_aEnemy[nCntEnemy].move.x *= -1;
					g_aEnemy[nCntEnemy].nPosX *= -1;
				}

				//テクスチャが左向き
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 1.0f);
				}

				//テクスチャが右向き
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 1.0f);
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_MET / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_MET / 2), 0.0f);
			}

			//カラス亜種
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
			{
				//アニメーション
				g_aEnemy[nCntEnemy].nCounterAnim++;	//カウンタ加算
				if (g_aEnemy[nCntEnemy].nCounterAnim == 7)//速さ
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 3)//枚数
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//カラス亜種の移動
				//難易度ノーマル
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.5;
				}
				//難易度ハード
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3.5;
				}

				//プレイヤーの操作に対応した移動
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//カラス亜種の移動範囲
				if (g_aEnemy[nCntEnemy].pos.x < 0)
				{
					//画面の端を出て行ったら未使用にする
					g_aEnemy[nCntEnemy].bUse = false;
				}

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.333f + 0.333f, 1.0f);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_METⅡ / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_METⅡ / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_METⅡ / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_METⅡ / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_METⅡ / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_METⅡ / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_METⅡ / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_METⅡ / 2), 0.0f);
			}

			//ガイコツ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//難易度ノーマル
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//クールタイムが終わってから攻撃可能
						if (g_aEnemy[nCntEnemy].nAttackTime == 0)
						{
							//弾の発射
							if ((rand() % (g_CntEnemy * 150)) == 0)
							{
								//プレイヤーの位置を取得
								pPlayer = GetPlayer();

								float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//目標(プレイヤー)の座標
								float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;	//敵の座標
								float fAngle;																	//角度

								fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//角度を決める

								//弾を発射（左向き）
								if (g_aEnemy[nCntEnemy].bLeft == true)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*9.0f, -cosf(fAngle)*9.0f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(255, 255, 255, 255), 50);
								}
								//弾を発射（右向き）
								if (g_aEnemy[nCntEnemy].bLeft == false)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*9.0f, -cosf(fAngle)*9.0f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(255, 255, 255, 255), 50);
								}
								//PlaySound(SOUND_LABEL_SE_SHOT2);	//ガイコツのショット音
								g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD*2;	//攻撃のクールタイム
							}
						}
					}
					//難易度ハード
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//クールタイムが終わってから攻撃可能
						if (g_aEnemy[nCntEnemy].nAttackTime == 0)
						{
							//弾の発射
							if ((rand() % (g_CntEnemy * 70)) == 0)
							{
								//プレイヤーの位置を取得
								pPlayer = GetPlayer();

								float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//目標(プレイヤー)の座標
								float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;	//敵の座標
								float fAngle;																	//角度

								fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//角度を決める
								//弾を発射（左向き）
								if (g_aEnemy[nCntEnemy].bLeft == true)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*10.0f, -cosf(fAngle)*10.0f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(255, 255, 255, 255), 50);
								}
								//弾を発射（右向き）
								if (g_aEnemy[nCntEnemy].bLeft == false)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-sinf(fAngle)*10.0f, -cosf(fAngle)*10.0f, 0), BULLETTYPE_OCT, 95, D3DXCOLOR(255, 255, 255, 255), 50);
								}
								//PlaySound(SOUND_LABEL_SE_SHOT2);	//ガイコツのショット音
								g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD*2;	//攻撃のクールタイム
							}
						}
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//ガイコツの移動
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;

				//攻撃用のテクスチャに変更
				if (g_aEnemy[nCntEnemy].nAttackTime > 0)
				{
					g_aEnemy[nCntEnemy].nAttackTime--;
					g_aEnemy[nCntEnemy].nPatternAnim = 1;
				}

				//攻撃していないときのテクスチャ
				if (g_aEnemy[nCntEnemy].nAttackTime == 0)
				{
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//プレイヤーの操作に対応した移動
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//ゲーム2で、リフトにいるとき
				if (g_ModeEnemy == MODE_GAME2)
				{
					//重力
					g_aEnemy[nCntEnemy].move.y += GRAVITY;

					if (g_aEnemy[nCntEnemy].pos.y > SCREEN_HEIGHT - ENEMY_SIZE_OCT + (ENEMY_SIZE_OCT / 2) - 80)
					{
						g_aEnemy[nCntEnemy].pos.y = SCREEN_HEIGHT - ENEMY_SIZE_OCT + (ENEMY_SIZE_OCT / 2) - 80;

						g_aEnemy[nCntEnemy].move.y = 0;	//地面にいるときは重力を与えない
					}
				}

				//左向き
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//右向き
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//テクスチャが左向き
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
				}

				//テクスチャが右向き
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
			}

			//イノシシ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
			{
				//アニメーション
				g_aEnemy[nCntEnemy].nCounterAnim++;	//カウンタ加算
				if (g_aEnemy[nCntEnemy].nCounterAnim == 15)//速さ
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 2)//枚数
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}

				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}

				//左向き
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//右向き
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//テクスチャが左向き
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f + 0.5f, 1.0f);
				}

				//テクスチャが右向き
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
				}

				//プレイヤーの操作に対応した移動
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//イノシシの移動(プレイヤーが通常時のイノシシから見て画面の三分の一の長さに入ったら動く)
				if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_NORMAL)
				{
					if (pPlayer->pos.x + (PLAYER_SIZE / 2) >= g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2) - (SCREEN_WIDTH / 3) &&
						pPlayer->pos.x + (PLAYER_SIZE / 2) < g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2) + (SCREEN_WIDTH / 3) ||
						pPlayer->pos.x - (PLAYER_SIZE / 2) <= g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2) + (SCREEN_WIDTH / 3) &&
						pPlayer->pos.x - (PLAYER_SIZE / 2) > g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2) - (SCREEN_WIDTH / 3))
					{
						//自機の位置を取得
						pPlayer = GetPlayer();

						float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//自機の座標
						float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;		//イノシシの座標
						float fAngle;											//角度

						fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));	//角度を決める
						
						g_aEnemy[nCntEnemy].move.x = -sin(fAngle);
						//難易度ノーマル
						if (g_nEnemyLevel == GAMELEVEL_NORMAL)
						{
							g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x * 4.2;
						}
						//難易度ハード
						if (g_nEnemyLevel == GAMELEVEL_HARD)
						{
							g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x * 4.8;
						}

						//テクスチャが左向き
						if (g_aEnemy[nCntEnemy].bLeft == true)
						{
							//テクスチャ座標の設定
							pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
						}

						//テクスチャが右向き
						else if (g_aEnemy[nCntEnemy].bLeft == false)
						{
							//テクスチャ座標の設定
							pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
							pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
							pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
							pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
						}
					}
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GHO / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GHO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GHO / 2), 0.0f);
			}

			//コウモリ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
			{
				//アニメーション
				g_aEnemy[nCntEnemy].nCounterAnim++;	//カウンタ加算
				if (g_aEnemy[nCntEnemy].nCounterAnim == 5)//速さ
				{
					g_aEnemy[nCntEnemy].nCounterAnim = 0;
					g_aEnemy[nCntEnemy].nPatternAnim++;
					if (g_aEnemy[nCntEnemy].nPatternAnim == 2)//枚数
					{
						g_aEnemy[nCntEnemy].nPatternAnim = 0;
					}
				}

				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}

				//左向き
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//右向き
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//テクスチャが左向き
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
				}

				//テクスチャが右向き
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				}

				//自機の位置を取得
				pPlayer = GetPlayer();

				float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//目標(自機)の座標
				float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;		//コウモリの座標
				float fAngle;											//角度

				fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));	//角度を決める

				g_aEnemy[nCntEnemy].move.x = -sin(fAngle);
				g_aEnemy[nCntEnemy].move.y = -cos(fAngle);

				//難易度ノーマル
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.0;
				}
				//難易度ハード
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 3.5;
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GSO / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_GSO / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_GSO / 2), 0.0f);
			}

			//アイスドラゴン
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_006)
			{

				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

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
					//クールタイムが終わってから攻撃可能
					if (g_aEnemy[nCntEnemy].nAttackTime == 0)
					{
						//つららの発射
						if ((rand() % (g_CntEnemy * 150)) == 0)
						{
							//プレイヤーの位置を取得
							pPlayer = GetPlayer();

							//頭上につららを召喚する
							SetEffect2(D3DXVECTOR3(pPlayer->pos.x, 180, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE * 2, EFFECTTYPE_002);

							g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD * 2;	//攻撃のクールタイム
						}
					}

					//トルネードの発射
					if ((rand() % (g_CntEnemy * 150)) == 0)
					{
						//プレイヤーの位置を取得
						pPlayer = GetPlayer();

						//弾を発射（左向き）
						if (g_aEnemy[nCntEnemy].bLeft == true)
						{
							float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//目標(プレイヤー)の座標
							float fEposx = g_aEnemy[nCntEnemy].pos.x - 60, fEposy = g_aEnemy[nCntEnemy].pos.y + 90;	//敵の座標
							float fAngle;																	//角度

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//角度を決める

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 60, g_aEnemy[nCntEnemy].pos.y + 90, 0.0f), D3DXVECTOR3(-sinf(fAngle)*10.0f, -cosf(fAngle)*10.0f, 0), BULLETTYPE_BOSS, 95, D3DXCOLOR(255, 255, 255, 255), 85);
						}
						//弾を発射（右向き）
						if (g_aEnemy[nCntEnemy].bLeft == false)
						{
							float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//目標(プレイヤー)の座標
							float fEposx = g_aEnemy[nCntEnemy].pos.x + 60, fEposy = g_aEnemy[nCntEnemy].pos.y + 90;	//敵の座標
							float fAngle;																	//角度

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//角度を決める

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 60, g_aEnemy[nCntEnemy].pos.y + 90, 0.0f), D3DXVECTOR3(-sinf(fAngle)*10.0f, -cosf(fAngle)*10.0f, 0), BULLETTYPE_BOSS, 95, D3DXCOLOR(255, 255, 255, 255), 85);
						}

					}

					//自機の位置を取得
					pPlayer = GetPlayer();

					float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//自機の座標
					float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;		//アイスドラゴンの座標
					float fAngle;											//角度

					fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));	//角度を決める

					g_aEnemy[nCntEnemy].move.x = -sin(fAngle);

					//クールタイムが終わってから移動可能
					if (g_aEnemy[nCntEnemy].nAttackTime == 0)
					{
						//HPが4分の1を下回ったら移動速度アップ
						if (g_aEnemy[nCntEnemy].nLife < 375)
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.8;
						}

						//アイスドラゴンの移動
						else
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.2;
						}
					}
				}

				//難易度ハード
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					//クールタイムが終わってから攻撃可能
					if (g_aEnemy[nCntEnemy].nAttackTime == 0)
					{
						//つららの発射
						if ((rand() % (g_CntEnemy * 70)) == 0)
						{
							//プレイヤーの位置を取得
							pPlayer = GetPlayer();

							//頭上につららを召喚する
							SetEffect2(D3DXVECTOR3(pPlayer->pos.x, 180, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SIZE * 2, EFFECTTYPE_002);

							g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD * 2;	//攻撃のクールタイム
						}
					}

					//トルネードの発射
					if ((rand() % (g_CntEnemy * 70)) == 0)
					{
						//プレイヤーの位置を取得
						pPlayer = GetPlayer();

						//弾を発射（左向き）
						if (g_aEnemy[nCntEnemy].bLeft == true)
						{
							float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//目標(プレイヤー)の座標
							float fEposx = g_aEnemy[nCntEnemy].pos.x - 60, fEposy = g_aEnemy[nCntEnemy].pos.y + 90;	//敵の座標
							float fAngle;																	//角度

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//角度を決める

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 60, g_aEnemy[nCntEnemy].pos.y+90, 0.0f), D3DXVECTOR3(-sinf(fAngle)*12.0f, -cosf(fAngle)*12.0f, 0), BULLETTYPE_BOSS, 95, D3DXCOLOR(255, 255, 255, 255), 85);
						}
						//弾を発射（右向き）
						if (g_aEnemy[nCntEnemy].bLeft == false)
						{
							float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//目標(プレイヤー)の座標
							float fEposx = g_aEnemy[nCntEnemy].pos.x + 60, fEposy = g_aEnemy[nCntEnemy].pos.y + 90;	//敵の座標
							float fAngle;																	//角度

							fAngle = atan2f((fEposx - fHposx), (fEposy - fHposy));	//角度を決める

							SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 60, g_aEnemy[nCntEnemy].pos.y+90, 0.0f), D3DXVECTOR3(-sinf(fAngle)*12.0f, -cosf(fAngle)*12.0f, 0), BULLETTYPE_BOSS, 95, D3DXCOLOR(255, 255, 255, 255), 85);
						}

					}

					//自機の位置を取得
					pPlayer = GetPlayer();

					float fPposx = pPlayer->pos.x, fPposy = pPlayer->pos.y;	//自機の座標
					float fEposx = g_aEnemy[nCntEnemy].pos.x, fEposy = g_aEnemy[nCntEnemy].pos.y;		//アイスドラゴンの座標
					float fAngle;											//角度

					fAngle = atan2f((fEposx - fPposx), (fEposy - fPposy));	//角度を決める

					g_aEnemy[nCntEnemy].move.x = -sin(fAngle);

					//クールタイムが終わってから移動可能
					if (g_aEnemy[nCntEnemy].nAttackTime == 0)
					{
						//HPが4分の1を下回ったら移動速度アップ
						if (g_aEnemy[nCntEnemy].nLife < 375)
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 2.0;
						}

						//アイスドラゴンの移動
						else
						{
							g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 0.5;
						}
					}
				}

				//左向き
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//右向き
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//攻撃用のテクスチャに変更
				if (g_aEnemy[nCntEnemy].nAttackTime > 0)
				{
					g_aEnemy[nCntEnemy].nAttackTime--;

					//テクスチャが左向き
					if (g_aEnemy[nCntEnemy].bLeft == true)
					{
						//テクスチャ座標の設定
						pVtx[0].tex = D3DXVECTOR2(0.75f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(0.75f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
					}

					//テクスチャが右向き
					else if (g_aEnemy[nCntEnemy].bLeft == false)
					{
						//テクスチャ座標の設定
						pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
					}
				}
				else
				{
					//アニメーション
					g_aEnemy[nCntEnemy].nCounterAnim++;	//カウンタ加算
					if (g_aEnemy[nCntEnemy].nCounterAnim == 15)//速さ
					{
						g_aEnemy[nCntEnemy].nCounterAnim = 0;
						g_aEnemy[nCntEnemy].nPatternAnim++;
						if (g_aEnemy[nCntEnemy].nPatternAnim == 2)//枚数
						{
							g_aEnemy[nCntEnemy].nPatternAnim = 0;
						}
					}

					//テクスチャが左向き
					if (g_aEnemy[nCntEnemy].bLeft == true)
					{
						//テクスチャ座標の設定
						pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f + 0.25f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f + 0.25f, 1.0f);
					}

					//テクスチャが右向き
					else if (g_aEnemy[nCntEnemy].bLeft == false)
					{
						//テクスチャ座標の設定
						pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f + 0.25f, 0.0f);
						pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f, 0.0f);
						pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f + 0.25f, 1.0f);
						pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.25f, 1.0f);
					}
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_BOSS_X / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_BOSS_Y / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_BOSS_X / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_BOSS_Y / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_BOSS_X / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_BOSS_Y / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_BOSS_X / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_BOSS_Y / 2), 0.0f);
			}

			//ピラニア
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_007)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					break;
				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//ピラニアの移動
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;

				//プレイヤーの操作に対応した移動
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				if (g_aEnemy[nCntEnemy].move.y == -1.0f)
				{
					//向きを変える
					g_aEnemy[nCntEnemy].bLeft = !g_aEnemy[nCntEnemy].bLeft;	//bool型のトグル
				}

				//重力
				g_aEnemy[nCntEnemy].move.y += 0.5;

				//水面から飛び出る
				if (g_aEnemy[nCntEnemy].move.y > 20.0f)
				{
					//向きを変える
					g_aEnemy[nCntEnemy].bLeft = !g_aEnemy[nCntEnemy].bLeft;	//bool型のトグル

					//ジャンプのように飛びでる
					g_aEnemy[nCntEnemy].move.y = 0;
					g_aEnemy[nCntEnemy].move.y -= 20.0f;
				}

				//落下中のテクスチャ
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//上昇中のテクスチャ
				if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					g_aEnemy[nCntEnemy].nPatternAnim = 1;
				}

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_PIR / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_PIR / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_PIR / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_PIR / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_PIR / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_PIR / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_PIR / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_PIR / 2), 0.0f);

			}

			//ダークガイコツ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_008)
			{
				switch (g_aEnemy[nCntEnemy].state)
				{
				case ENEMYSTATE_NORMAL:
					//難易度ノーマル
					if (g_nEnemyLevel == GAMELEVEL_NORMAL)
					{
						//クールタイムが終わってから攻撃可能
						if (g_aEnemy[nCntEnemy].nAttackTime == 0)
						{
							//弾の発射
							if ((rand() % (g_CntEnemy * 150)) == 0)
							{
								//弾を発射（左向き）
								if (g_aEnemy[nCntEnemy].bLeft == true)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-2, -2, 0), BULLETTYPE_GHO, 180, D3DXCOLOR(255, 255, 255, 255), 75);
								}
								//弾を発射（右向き）
								if (g_aEnemy[nCntEnemy].bLeft == false)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(2, -2, 0), BULLETTYPE_GHO, 180, D3DXCOLOR(255, 255, 255, 255), 75);
								}
								//PlaySound(SOUND_LABEL_SE_SHOT2);	//ガイコツのショット音
								g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD * 2;	//攻撃のクールタイム
							}
						}
					}
					//難易度ハード
					if (g_nEnemyLevel == GAMELEVEL_HARD)
					{
						//クールタイムが終わってから攻撃可能
						if (g_aEnemy[nCntEnemy].nAttackTime == 0)
						{
							//弾の発射
							if ((rand() % (g_CntEnemy * 70)) == 0)
							{
								//弾を発射（左向き）
								if (g_aEnemy[nCntEnemy].bLeft == true)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(-2, -2, 0), BULLETTYPE_GHO, 180, D3DXCOLOR(255, 255, 255, 255), 75);
								}
								//弾を発射（右向き）
								if (g_aEnemy[nCntEnemy].bLeft == false)
								{
									SetBullet(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 30, g_aEnemy[nCntEnemy].pos.y + 20, 0.0f), D3DXVECTOR3(2, -2, 0), BULLETTYPE_GHO, 180, D3DXCOLOR(255, 255, 255, 255), 75);
								}
								//PlaySound(SOUND_LABEL_SE_SHOT2);	//ガイコツのショット音
								g_aEnemy[nCntEnemy].nAttackTime = ATTACK_CD * 2;	//攻撃のクールタイム
							}
						}
					}
					break;

				case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					g_bHitEnemy = true;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;

						g_bHitEnemy = false;

						//通常描画に戻す処理
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
					break;
				}
				//ダークガイコツの移動
				g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move * 1;

				//攻撃用のテクスチャに変更
				if (g_aEnemy[nCntEnemy].nAttackTime > 0)
				{
					g_aEnemy[nCntEnemy].nAttackTime--;
					g_aEnemy[nCntEnemy].nPatternAnim = 1;
				}

				//攻撃していないときのテクスチャ
				if (g_aEnemy[nCntEnemy].nAttackTime == 0)
				{
					g_aEnemy[nCntEnemy].nPatternAnim = 0;
				}

				//プレイヤーの操作に対応した移動
				if (pPlayer->bDisp == true && g_bBgXE == true && g_bBlockXE == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aEnemy[nCntEnemy].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aEnemy[nCntEnemy].pos.x += VALUE_MOVE;
					}
				}

				//左向き
				if (pPlayer->pos.x < g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = true;
				}

				//右向き
				if (pPlayer->pos.x > g_aEnemy[nCntEnemy].pos.x)
				{
					g_aEnemy[nCntEnemy].bLeft = false;
				}

				//テクスチャが左向き
				if (g_aEnemy[nCntEnemy].bLeft == true)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
				}

				//テクスチャが右向き
				else if (g_aEnemy[nCntEnemy].bLeft == false)
				{
					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f + 0.5f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_aEnemy[nCntEnemy].nPatternAnim*0.5f, 1.0f);
				}

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y - (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + (ENEMY_SIZE_OCT / 2), g_aEnemy[nCntEnemy].pos.y + (ENEMY_SIZE_OCT / 2), 0.0f);
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

			//ビートル
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MOS;
			}

			//カラス
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_MET;
			}

			//カラス亜種
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_METⅡ;
			}

			//ガイコツ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
			{
				//難易度ノーマル
				if (g_nEnemyLevel == GAMELEVEL_NORMAL)
				{
					//敵の体力
					g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_OCT_NORMAL;
				}

				//難易度ハード
				if (g_nEnemyLevel == GAMELEVEL_HARD)
				{
					//敵の体力
					g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_OCT_HARD;
				}
			}

			//イノシシ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_GHO;
			}

			//コウモリ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_GSO;
			}

			//アイスドラゴン
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_006)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_BOSS;
			}

			//ピラニア
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_007)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_PIR;
			}

			//ダークガイコツ
			if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_008)
			{
				//敵の体力
				g_aEnemy[nCntEnemy].nLife = ENEMY_LIFE_DARK;
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
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	int nBat;			//コウモリが出現する位置

	PlaySound(SOUND_LABEL_SE_EXPLOSION);	//爆発音

	//頂点バッファをロック
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;

	if (g_aEnemy[nCntEnemy].nLife <= 0)//敵の体力が0以下になった時
	{
		//ビートルのスコア
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_000)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MOS);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(500);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(1000);
			}
		}

		//カラスのスコア
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_001)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_MET);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(750);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(1500);
			}
		}

		//カラス亜種のスコア
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_002)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_METⅡ);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(7500);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(10000);
			}
		}

		//ガイコツのスコア
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_003)
		{
			//ゲーム2では、倒したガイコツを数える
			if (g_ModeEnemy == MODE_GAME2)
			{
				g_nCntBorn++;

				//難易度ノーマル
				if (g_nEnemyLevel == GAMELEVEL_NORMAL && g_nCntBorn >= 3)
				{
					g_nCntBorn = 0;

					//塔の頂への足場を設置
					SetBlock(D3DXVECTOR3(300, 0, 0.0f), 200, 30, BLOCKTYPE_001, 30);	//左の足場
					SetBlock(D3DXVECTOR3(980, 0, 0.0f), 200, 30, BLOCKTYPE_001, 30);	//右の足場
					SetBlock(D3DXVECTOR3(550, -240, 0.0f), 80, 80, BLOCKTYPE_008, 30);	//W看板
					SetBlock(D3DXVECTOR3(640, -200, 0.0f), 300, 30, BLOCKTYPE_005, 30);	//塔の頂への足場

					//左右の足場にアイテムを設置
					SetItem(D3DXVECTOR3(250, -45, 0.0f), ITEMTYPE_002, 70);		//樽
					SetItem(D3DXVECTOR3(1030, -45, 0.0f), ITEMTYPE_002, 70);	//樽

					//塔の頂へのワープゾーンを設置
					SetBlock(D3DXVECTOR3(640, -250, 0.0f), 150, 150, BLOCKTYPE_007, 30);	//塔の頂へのワープゾーン
				}

				//難易度ハード
				if (g_nEnemyLevel == GAMELEVEL_HARD && g_nCntBorn >= 6)
				{
					g_nCntBorn = 0;

					//塔の頂への足場を設置
					SetBlock(D3DXVECTOR3(300, 0, 0.0f), 200, 30, BLOCKTYPE_001, 30);	//左の足場
					SetBlock(D3DXVECTOR3(980, 0, 0.0f), 200, 30, BLOCKTYPE_001, 30);	//右の足場
					SetBlock(D3DXVECTOR3(550, -240, 0.0f), 80, 80, BLOCKTYPE_008, 30);	//W看板
					SetBlock(D3DXVECTOR3(640, -200, 0.0f), 300, 30, BLOCKTYPE_005, 30);	//塔の頂への足場

					//左右の足場にアイテムを設置
					SetItem(D3DXVECTOR3(250, -45, 0.0f), ITEMTYPE_002, 70);		//樽
					SetItem(D3DXVECTOR3(1030, -45, 0.0f), ITEMTYPE_002, 70);	//樽

					//塔の頂へのワープゾーンを設置
					SetBlock(D3DXVECTOR3(640, -250, 0.0f), 150, 150, BLOCKTYPE_007, 30);	//塔の頂へのワープゾーン
				}
			}

			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_OCT);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(2500);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(5000);
			}
		}

		//イノシシのスコア
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_004)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_GHO);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(3000);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(4000);
			}
		}

		//コウモリのスコア
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_005)
		{
			//コウモリを倒した数を数える
			g_nCntBat++;

			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_GSO);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				//10体ずつガイコツを降らせる
				if (g_nCntBat == 10)//左
				{
					SetEnemy(D3DXVECTOR3(270, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
				}

				if (g_nCntBat == 20)//右
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 270, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
				}

				if (g_nCntBat == 30)//左
				{
					SetEnemy(D3DXVECTOR3(320, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
				}

				//コウモリを倒した数が30体以下なら、ランダムで次のコウモリを生成
				if (g_nCntBat < 30)
				{

					nBat = rand() % 15 + 1;

					//右端
					if (nBat == 1)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 400, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//右端
					if (nBat == 2)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 200, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//右端
					if (nBat == 3)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 600, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//右端
					if (nBat == 4)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//右端
					if (nBat == 5)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 500, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 6)
					{
						SetEnemy(D3DXVECTOR3(0, 200, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 7)
					{
						SetEnemy(D3DXVECTOR3(0, 400, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 8)
					{
						SetEnemy(D3DXVECTOR3(0, 600, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 9)
					{
						SetEnemy(D3DXVECTOR3(0, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 10)
					{
						SetEnemy(D3DXVECTOR3(0, 500, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 11)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 12)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 +100, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 13)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 +200, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 14)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 -100, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 15)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 -200, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}
				}

				ScoreEffect(250);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				//10体ずつガイコツを降らせる
				if (g_nCntBat == 10)//左
				{
					SetEnemy(D3DXVECTOR3(270, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
				}

				if (g_nCntBat == 20)//右
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 270, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
				}

				if (g_nCntBat == 30)//左
				{
					SetEnemy(D3DXVECTOR3(320, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
				}

				if (g_nCntBat == 40)//右
				{
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 320, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
				}

				if (g_nCntBat == 50)//左右
				{
					SetEnemy(D3DXVECTOR3(370, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
					SetEnemy(D3DXVECTOR3(SCREEN_WIDTH - 370, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_003);	//ガイコツ
				}

				//コウモリを倒した数が50体以下なら、ランダムで次のコウモリを生成
				if (g_nCntBat < 50)
				{

					nBat = rand() % 15 + 1;

					//右端
					if (nBat == 1)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 400, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//右端
					if (nBat == 2)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 200, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//右端
					if (nBat == 3)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 600, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//右端
					if (nBat == 4)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//右端
					if (nBat == 5)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH, 500, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 6)
					{
						SetEnemy(D3DXVECTOR3(0, 200, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 7)
					{
						SetEnemy(D3DXVECTOR3(0, 400, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 8)
					{
						SetEnemy(D3DXVECTOR3(0, 600, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 9)
					{
						SetEnemy(D3DXVECTOR3(0, 300, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//左端
					if (nBat == 10)
					{
						SetEnemy(D3DXVECTOR3(0, 500, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 11)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 12)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 100, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 13)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 + 200, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 14)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 100, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}

					//中央上
					if (nBat == 15)
					{
						SetEnemy(D3DXVECTOR3(SCREEN_WIDTH / 2 - 200, 0, 0.0f), D3DXVECTOR3(0.0, 0.0, 0.0f), ENEMYTYPE_005);	//コウモリ
					}
				}

				ScoreEffect(500);
			}
		}

		//アイスドラゴン
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_006)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_BOSS_Y);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(12500);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(21875);
			}

			PlaySound(SOUND_LABEL_SE_VOICE);	//ボス撃破音
			StopSound(SOUND_LABEL_BGM004);	//ボス戦のBGMを止める
		}

		//ピラニア
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_007)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_PIR);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(1000);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(2000);
			}
		}

		//ダークガイコツのスコア
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_008)
		{
			//爆発生成
			SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), ENEMY_SIZE_OCT);

			//難易度ノーマル
			if (g_nEnemyLevel == GAMELEVEL_NORMAL)
			{
				ScoreEffect(4000);
			}
			//難易度ハード
			if (g_nEnemyLevel == GAMELEVEL_HARD)
			{
				ScoreEffect(6000);
			}
		}

		g_aEnemy[nCntEnemy].bUse = false;//敵を未使用にする

		// 設定を戻す
		pVtx[(nCntEnemy * 4) + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEnemy * 4) + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEnemy * 4) + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[(nCntEnemy * 4) + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	else
	{
		//ダメージ状態へ
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;

		//ボスはダメージ状態が20fの時間
		if (g_aEnemy[nCntEnemy].type == ENEMYTYPE_006)
		{
			g_aEnemy[nCntEnemy].nCounterState = INVINCIBLE_FRAMEⅡ;
		}
		else
		{
			g_aEnemy[nCntEnemy].nCounterState = INVINCIBLE_FRAME;
		}

		if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
		{
			// 頂点カラーの設定
			pVtx[(nCntEnemy * 4) + 0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[(nCntEnemy * 4) + 1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[(nCntEnemy * 4) + 2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
			pVtx[(nCntEnemy * 4) + 3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		}
	}

	//頂点データをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// エネミーのダメージ設定2
//=============================================================================
bool HitEnemy2(void)
{
	return g_bHitEnemy;
}