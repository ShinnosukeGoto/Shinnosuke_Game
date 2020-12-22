//=============================================================================
//
// X-Bow Hunter [hunter.cpp]の処理 (ハンターが自機)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "hunter.h"
#include "input.h"
#include "arrow.h"
#include "explosion.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include "game.h"
#include "wing.h"
#include "sound.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define VALUE_MOVE (5.5f)		//羽がないハンターの移動量
#define VALUE_JUMP (22.5f)		//ジャンプの強さ
#define VALUE_MOVE_WING (0.95f)	//羽が生えたハンターの移動量
#define ARROW_CD (12)			//次の矢を打てるまでの時間
#define BLUE_ARROW_CD (10)		//次の矢を打てるまでの時間
#define GRAVITY (1.0f)			//重力

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygon = NULL;
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygon = NULL;
D3DXVECTOR3 g_movePolygon;//ポリゴンの移動量
int g_nCounterAnimHunt;		//アニメーションカウンター
int g_nPatternAnimHunt;		//アニメーションパターン
HUNTER g_Hunter;			//自機の情報
int g_nArrowCD;				//矢のクールタイム

							//=============================================================================
							//ハンター初期化処理
							//=============================================================================
void InitHunter(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nCounterAnimHunt = 0;
	g_nPatternAnimHunt = 0;

	//ハンターの初期化
	g_Hunter.state = HUNTERSTATE_NORMAL;
	g_Hunter.pos = D3DXVECTOR3(100.0f, 590.0f, 0.0f);
	g_Hunter.nLife = HUNTER_LIFE;
	g_Hunter.nCounterState = 0;
	g_Hunter.bDisp = false;
	g_Hunter.nStock = 2;	//残機の総数の初期化
	g_movePolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nArrowCD = ARROW_CD;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "hunter003.png", &g_pTexturePolygon);

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygon, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

														//頂点座標の設定（右回りで設定する）
	pVtx[0].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// ハンター終了処理
//=============================================================================
void UninitHunter(void)
{
	//テクスチャの開放
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}

	//頂点バッファの開放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=============================================================================
// ハンター更新処理
//=============================================================================
void UpdateHunter(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
	ENEMY*pEnemy;
	int nCntEnemy;
	BOSS*pBoss;
	int nCntBoss;
	WING*pWing;

	int nDirect = 0;		//入力された方向を数える
	int nChange = 0;		//向きを変える
	int nAttack = 0;		//攻撃ボタンを数える
	int nOblique = 0;		//斜めを向く

							//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤーの状態
	switch (g_Hunter.state)
	{
	case HUNTERSTATE_NORMAL:	//通常の状態
								//ピンチ状態とループ
		if (g_Hunter.nLife < HUNTER_LIFE / 4)
		{
			g_Hunter.state = HUNTERSTATE_PINCH;

			//通常描画に戻す処理
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case HUNTERSTATE_PINCH:	//ピンチ状態
		g_Hunter.nCounterState--;
		if (g_Hunter.nCounterState <= 0)
		{
			g_Hunter.state = HUNTERSTATE_NORMAL;

			//ピンチ描画にする処理
			pVtx[0].col = D3DCOLOR_RGBA(246, 120, 59, 255);
			pVtx[1].col = D3DCOLOR_RGBA(246, 120, 59, 255);
			pVtx[2].col = D3DCOLOR_RGBA(246, 120, 59, 255);
			pVtx[3].col = D3DCOLOR_RGBA(246, 120, 59, 255);
		}
		break;

	case HUNTERSTATE_DAMAGE:	//ダメージ状態
		g_Hunter.nCounterState--;
		if (g_Hunter.nCounterState <= 0)
		{
			g_Hunter.state = HUNTERSTATE_NORMAL;

			//通常描画に戻す処理
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		break;

	case HUNTERSTATE_APPEAR:	//出現までの状態
		g_Hunter.nCounterState--;
		g_Hunter.bDisp = false;	//出現まで、表示させない
		if (g_Hunter.nCounterState <= 0)
		{
			PlaySound(SOUND_LABEL_SE_ITEM);	//復活音

			g_Hunter.state = HUNTERSTATE_NORMAL;

			//通常描画に戻す処理
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			g_Hunter.bDisp = true;	//表示させる
		}
		break;
	}

	//ハンターが表示されているなら更新
	if (g_Hunter.bDisp == true)
	{
		//アニメーション
		g_nCounterAnimHunt++;	//カウンタ加算
		if (g_nCounterAnimHunt == 10)	//fps
		{
			g_nCounterAnimHunt = 0;
			g_nPatternAnimHunt++;
			if (g_nPatternAnimHunt == 2)//枚数
			{
				g_nPatternAnimHunt = 0;
			}
		}

		//敵を取得
		pEnemy = GetEnemy();
		pBoss = GetBoss();
		//ウィングの使用状況を取得
		pWing = GetWing();

		//羽を使用していないときの処理
		if (pWing->bUse == false)
		{
			//Aキーが押された（左移動）
			if (GetKeyboardPress(DIK_A))
			{
				g_Hunter.pos.x -= VALUE_MOVE;
				nDirect--;
			}

			//Dキーが押された（右移動）
			if (GetKeyboardPress(DIK_D))
			{
				g_Hunter.pos.x += VALUE_MOVE;
				nDirect++;
			}

			//AとDキーで、左向きに固定
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{
				nDirect--;
			}

			if (GetKeyboardPress(DIK_SPACE))
			{
				nAttack++;
				if (g_nArrowCD % ARROW_CD == 0)
				{
					SetArrow(D3DXVECTOR3(g_Hunter.pos.x + 20.5f, g_Hunter.pos.y + 5.5, 0.0f), D3DXVECTOR3(7.9, -7.9, 0), 66, D3DXCOLOR(255, 255, 255, 255), ARROWTYPE_BEND, 50);
					PlaySound(SOUND_LABEL_SE_SHOT);	//ショット音
				}
			}
			g_nArrowCD++;

			//羽を使用していないときにかかるプレイヤーへの重力
			g_movePolygon.y += GRAVITY;	//重力
			g_Hunter.pos.y += g_movePolygon.y;	//位置更新
			g_movePolygon.y += (0.0 - g_movePolygon.x)*RATE_MOVE;
		}

		//羽を使用しているときの処理
		if (pWing->bUse == true)
		{
			//Aキーが押された(左移動)
			if (GetKeyboardPress(DIK_A))
			{
				nDirect++;
				if (GetKeyboardPress(DIK_W))//AとWが同時に押された
				{
					g_movePolygon.x -= sinf(D3DX_PI / 4) * VALUE_MOVE_WING;
					g_movePolygon.y -= cosf(D3DX_PI / 4) * VALUE_MOVE_WING;
				}
				else if (GetKeyboardPress(DIK_S))//AとSが同時に押された
				{
					g_movePolygon.x -= (D3DX_PI / 4) * VALUE_MOVE_WING;
					g_movePolygon.y += (D3DX_PI / 4) * VALUE_MOVE_WING;
				}
				else				//左だけ押した
				{
					g_movePolygon.x -= VALUE_MOVE_WING;
				}

			}

			//Dキーが押された(右移動)
			else if (GetKeyboardPress(DIK_D))
			{
				nDirect++;
				if (GetKeyboardPress(DIK_W))//DとWが同時に押された
				{
					g_movePolygon.x += sinf(D3DX_PI / 4) * VALUE_MOVE_WING;
					g_movePolygon.y -= cosf(D3DX_PI / 4) * VALUE_MOVE_WING;
				}
				else if (GetKeyboardPress(DIK_S))//DとSが同時に押された
				{
					g_movePolygon.x += sinf(D3DX_PI / 4) * VALUE_MOVE_WING;
					g_movePolygon.y += cosf(D3DX_PI / 4) * VALUE_MOVE_WING;
				}
				else				//右だけ押した
				{
					g_movePolygon.x += VALUE_MOVE_WING;
				}
			}

			//Wキーが押された(上移動)
			else if (GetKeyboardPress(DIK_W))
			{
				g_movePolygon.y -= VALUE_MOVE_WING;
				nDirect++;
			}

			//Sキーが押された(下移動)
			else if (GetKeyboardPress(DIK_S))
			{
				g_movePolygon.y += VALUE_MOVE_WING;
				nDirect++;
			}

			//スペースキーが押された(羽があるときは剛射)
			if (GetKeyboardPress(DIK_SPACE))
			{
				if (g_nArrowCD % BLUE_ARROW_CD == 0)
				{
					SetArrow(D3DXVECTOR3(g_Hunter.pos.x + 20.5f, g_Hunter.pos.y + 5.5, 0.0f), D3DXVECTOR3(14, 0, 0), 33, D3DXCOLOR(255, 255, 255, 255), ARROWTYPE_STRAIGHT, 55);
					PlaySound(SOUND_LABEL_SE_SHOT);	//ショット音
				}
			}
			g_nArrowCD++;

			//位置更新
			g_Hunter.pos.x += g_movePolygon.x;
			g_Hunter.pos.y += g_movePolygon.y;

			//慣性
			g_movePolygon.x += (0.0 - g_movePolygon.x)*RATE_MOVE;
			g_movePolygon.y += (0.0 - g_movePolygon.y)*RATE_MOVE;
		}

		//左の壁
		if (g_Hunter.pos.x < HUNTER_SIZE - (HUNTER_SIZE / 2))
		{
			g_Hunter.pos.x = HUNTER_SIZE - (HUNTER_SIZE / 2);
		}

		//右の壁
		if (g_Hunter.pos.x > SCREEN_WIDTH - HUNTER_SIZE + (HUNTER_SIZE / 2))
		{
			g_Hunter.pos.x = SCREEN_WIDTH - HUNTER_SIZE + (HUNTER_SIZE / 2);
		}

		//上の壁
		if (g_Hunter.pos.y < HUNTER_SIZE - (HUNTER_SIZE / 2) + 146)
		{
			g_Hunter.pos.y = HUNTER_SIZE - (HUNTER_SIZE / 2) + 146;
		}

		//地面にいるとき
		if (g_Hunter.pos.y > SCREEN_HEIGHT - HUNTER_SIZE + (HUNTER_SIZE / 2) - 80)
		{
			g_Hunter.pos.y = SCREEN_HEIGHT - HUNTER_SIZE + (HUNTER_SIZE / 2) - 80;

			//かつ、羽を使用していないときにジャンプができる
			if (pWing->bUse == false)
			{
				g_movePolygon.y = 0;	//地面にいるときは重力を与えない

				//ジャンプ処理
				if (GetKeyboardTrigger(DIK_W))
				{
					g_movePolygon.y -= VALUE_JUMP;	//ジャンプ
					PlaySound(SOUND_LABEL_SE_JUMP);	//ジャンプ音
				}
			}
		}

		//左向きに変える
		if (nDirect < 0)
		{
			nDirect = 0;
			nChange = 1;
		}

		//右向きに変える
		if (nDirect > 0)
		{
			nDirect = 0;
			nChange = 0;
		}

		//右斜め前を向く
		if (nAttack > 0)
		{
			nAttack = 0;
			nOblique = 1;
		}

		for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{
				//自機がモスキートに当たった時の当たり判定
				if (pEnemy->type == ENEMYTYPE_000)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MOS / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MOS / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MOS / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MOS / 2))
					{
						//自機にダメージ
						HitHunter(1000);
					}
				}

				//自機がメタルバードに当たった時の当たり判定
				if (pEnemy->type == ENEMYTYPE_001)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_MET / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_MET / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_MET / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_MET / 2))
					{
						//自機にダメージ
						HitHunter(1000);
					}
				}

				//自機がメタルバード亜種に当たった時の当たり判定
				if (pEnemy->type == ENEMYTYPE_002)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_METⅡ / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_METⅡ / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_METⅡ / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_METⅡ / 2))
					{
						//自機にダメージ
						HitHunter(1000);
					}
				}

				//自機がオクトパスに当たった時の当たり判定
				if (pEnemy->type == ENEMYTYPE_003)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_OCT / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_OCT / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_OCT / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_OCT / 2))
					{
						//自機にダメージ
						HitHunter(1000);
					}
				}

				//自機がゴーストに当たった時の当たり判定
				if (pEnemy->type == ENEMYTYPE_004)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GHO / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GHO / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GHO / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GHO / 2))
					{
						//自機にダメージ
						HitHunter(1000);
					}
				}

				//自機がグソクムシに当たった時の当たり判定
				if (pEnemy->type == ENEMYTYPE_004)
				{
					if (g_Hunter.pos.x <= pEnemy->pos.x + (ENEMY_SIZE_GSO / 2) &&
						g_Hunter.pos.x >= pEnemy->pos.x - (ENEMY_SIZE_GSO / 2) &&
						g_Hunter.pos.y <= pEnemy->pos.y + (ENEMY_SIZE_GSO / 2) &&
						g_Hunter.pos.y >= pEnemy->pos.y - (ENEMY_SIZE_GSO / 2))
					{
						//自機にダメージ
						HitHunter(10);
					}
				}
			}
		}

		for (nCntBoss = 0; nCntBoss < MAX_BOSS; nCntBoss++, pBoss++)
		{
			if (pBoss->bUse == true)
			{
				//自機がボスに当たった時の当たり判定
				if (g_Hunter.pos.x <= pBoss->pos.x + (BOSS_SIZE / 2) - 50 &&
					g_Hunter.pos.x >= pBoss->pos.x - (BOSS_SIZE / 2) + 50 &&
					g_Hunter.pos.y <= pBoss->pos.y + (BOSS_SIZE / 2) - 50 &&
					g_Hunter.pos.y >= pBoss->pos.y - (BOSS_SIZE / 2) + 50)
				{
					//自機にダメージ
					HitHunter(1000);
				}
			}
		}

		//テクスチャ座標を更新
		pVtx[0 + nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.33f, 0.0f);
		pVtx[1 - nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.33f + 0.33f, 0.0f);
		pVtx[2 + nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.33f, 1.0f);
		pVtx[3 - nChange].tex = D3DXVECTOR2(g_nPatternAnimHunt * 0.33f + 0.33f, 1.0f);

		//テクスチャの向きを右斜め前に更新
		if (nOblique == 1)
		{
			nOblique = 0;
			pVtx[0].tex = D3DXVECTOR2(0.67f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.67f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);
	}

	//頂点データをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// ハンター描画処理
//=============================================================================
void DrawHunter(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ハンターが表示されているなら描画
	if (g_Hunter.bDisp == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygon);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
}

//=============================================================================
// ハンターの設定処理
//=============================================================================
void SetHunter(D3DXVECTOR3 pos)
{
	//変数宣言
	VERTEX_2D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	//ハンターが表示されていないかつ、ストックがある状態のときに設定可能
	if (g_Hunter.bDisp == false && g_Hunter.nStock >-1)
	{
		g_Hunter.bDisp = true;
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y - (HUNTER_SIZE / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Hunter.pos.x - (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Hunter.pos.x + (HUNTER_SIZE / 2), g_Hunter.pos.y + (HUNTER_SIZE / 2), 0.0f);

		//位置を設定
		g_Hunter.pos = pos;
		g_movePolygon = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//ハンターの体力
		g_Hunter.nLife = HUNTER_LIFE;
	}

	//羽が生えるときのカラーを通常の色に戻す
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点データをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// ハンターの取得
//=============================================================================
HUNTER*GetHunter(void)
{
	return &g_Hunter;
}

//=============================================================================
// ハンターのダメージ設定
//=============================================================================
void HitHunter(int nDamage)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	PlaySound(SOUND_LABEL_SE_EXPLOSION);	//爆発音

											//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	g_Hunter.nLife -= nDamage;
	if (g_Hunter.nLife <= 0)
	{
		//爆発生成
		SetExplosion(g_Hunter.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), HUNTER_SIZE);

		//ハンターを未使用に
		g_Hunter.bDisp = false;

		//頂点カラーを設定（0～255の値で設定する）
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//次の自機を設置
		if (g_Hunter.nStock > -1)
		{
			SetHunter(D3DXVECTOR3(5, 590, 0.0f));
			g_Hunter.state = HUNTERSTATE_APPEAR;				//出現状態にする
			g_Hunter.nCounterState = COUNT_WAIT_NEXT_HUNTER;	//出現までの時間を数える
		}

		//設置後に、自機の総数を減らす
		g_Hunter.nStock--;
	}

	else if (g_Hunter.nLife > 0 && g_Hunter.nLife < HUNTER_LIFE / 4)
	{
		g_Hunter.state = HUNTERSTATE_PINCH;
		g_Hunter.nCounterState = 5;
		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	else
	{
		//ダメージ状態に遷移
		g_Hunter.state = HUNTERSTATE_DAMAGE;
		g_Hunter.nCounterState = 5;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
		pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
	}

	//頂点データをアンロックする
	g_pVtxBuffPolygon->Unlock();

	//ゲームオーバー
	if (g_Hunter.nStock < 0)
	{
		g_Hunter.bDisp = false;
		SetGameState(GAMESTATE_END);
	}
}