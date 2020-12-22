//=============================================================================
//
// ナインストーリーズ [bg.cpp]の処理 (バックグラウンド)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "block.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonBg[3] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonBg = NULL;
D3DXVECTOR3 g_moveBg;			//背景の移動量（ステージ全体の長さを決める）
int g_nCounterAnimBgLeft;		//左のアニメーションカウンター
int g_nCounterAnimBgRight;		//右のアニメーションカウンター
int g_nPatternAnimBg;			//アニメーションパターン
int g_nShakeMoveBg;				//背景が揺れるときの数値(沈む）
int g_nShakeMoveBg2;			//背景が揺れるときの数値(戻る)
bool g_bMoveBg;					//背景が動いている
bool g_bShakeBg;				//背景が揺れる
bool g_bBlockXB;				//ブロックに横から当たった時
bool g_bBlockYB;				//ブロックが停止位置にたどり着いたとき
MODE g_ModeBg;					//ゲーム画面に対応した背景

//=============================================================================
//バックグラウンド初期化処理
//=============================================================================
void InitBg(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg000.jpg", &g_pTexturePolygonBg[0]);	//雪の平原
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg001.png", &g_pTexturePolygonBg[1]);	//塔内部の背景
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bg009.png", &g_pTexturePolygonBg[2]);	//塔の頂

	//グローバル変数の初期化
	g_moveBg = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nCounterAnimBgLeft = 0;
	g_nCounterAnimBgRight = 0;
	g_nPatternAnimBg = 0;
	g_nShakeMoveBg = 0;
	g_nShakeMoveBg2 = 0;
	g_bMoveBg = false;	//最初は、背景は動かない
	g_bShakeBg = false;	//最初は、背景は揺れない
	g_bBlockXB = false;
	g_bBlockYB = false;
	g_ModeBg = MODE_GAME;

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonBg, NULL);
	VERTEX_2D *pVtxBg;	//バックグラウンドの頂点情報へのポインタ

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPolygonBg->Lock(0, 0, (void**)&pVtxBg, 0);	//この書式は変えないこと

	//頂点座標の設定（右回りで設定する）
	pVtxBg[0].pos = D3DXVECTOR3(0.0f, -7.0f, 0.0f);
	pVtxBg[1].pos = D3DXVECTOR3(SCREEN_WIDTH, -7.0f, 0.0f);
	pVtxBg[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHTⅡ, 0.0f);
	pVtxBg[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHTⅡ, 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtxBg[0].rhw = 1.0f;
	pVtxBg[1].rhw = 1.0f;
	pVtxBg[2].rhw = 1.0f;
	pVtxBg[3].rhw = 1.0f;

	//頂点カラーを設定（0～255の値で設定する）
	pVtxBg[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtxBg[0].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f, 0.0f);
	pVtxBg[1].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f + 0.997f, 0.0f);
	pVtxBg[2].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f, 1.0f);
	pVtxBg[3].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f + 0.997f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffPolygonBg->Unlock();
}

//=============================================================================
// バックグラウンド終了処理
//=============================================================================
void UninitBg(void)
{
	//テクスチャの開放
	for (int nCntBg = 0; nCntBg < 3; nCntBg++)
	{
		if (g_pTexturePolygonBg[nCntBg] != NULL)
		{
			g_pTexturePolygonBg[nCntBg]->Release();
			g_pTexturePolygonBg[nCntBg] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffPolygonBg != NULL)
	{
		g_pVtxBuffPolygonBg ->Release();
		g_pVtxBuffPolygonBg = NULL;
	}
}

//=============================================================================
// バックグラウンド更新処理
//=============================================================================
void UpdateBg(void)
{
	VERTEX_2D *pVtxBg;	//頂点情報へのポインタ
	PLAYER*pPlayer = GetPlayer();		//プレイヤーの状況を取得

	//プレイヤーが攻撃を受けたかどうか取得
	g_bShakeBg = HitPlayer2();

	g_bBlockXB = GetBlockX();

	//ゲームのステージを取得
	g_ModeBg = GetMode();

	//プレイヤーが攻撃を受けていないなら、敵が攻撃を受けているかどうかを取得
	if (g_bShakeBg == false)
	{
		g_bShakeBg = HitEnemy2();
	}

	g_pVtxBuffPolygonBg->Lock(0, 0, (void**)&pVtxBg, 0);	//この書式は変えないこと

	//ゲーム1(雪の平原)
	if (g_ModeBg == MODE_GAME)
	{
		//プレイヤーが右に進むにつれて、右にあるものが出てくる
		if (pPlayer->bDisp == true)
		{

			//AとD同時押しは無効
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{

			}

			//右に進む
			else if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D) && g_bBlockXB == false)
			{
				if (pPlayer->pos.x == SCREEN_WIDTH / 2)	//プレイヤーが中央にいるときだけ背景を流す
				{
					g_moveBg.x++;
					g_nCounterAnimBgRight++;					//カウンタ加算
					if (g_nCounterAnimBgRight == 2)			//速さ
					{
						g_nCounterAnimBgRight = 0;
						g_nPatternAnimBg++;
						if (g_nPatternAnimBg == STAGE1_X)	//枚数
						{
							g_nPatternAnimBg = 0;
						}
					}
				}

				//一番右以外は、背景を流す
				if (g_moveBg.x > STAGE1_X)
				{
					g_moveBg.x = STAGE1_X;
				}
			}

			//左に進む
			else if (pPlayer->nAttackTime == 0 && g_bBlockXB == false && GetKeyboardPress(DIK_A))
			{
				if (pPlayer->pos.x == SCREEN_WIDTH / 2)	//プレイヤーが中央にいるときだけ背景を流す
				{
					g_moveBg.x--;
					g_nCounterAnimBgLeft++;					//カウンタ加算
					if (g_nCounterAnimBgLeft == 2)			//速さ
					{
						g_nCounterAnimBgLeft = 0;
						g_nPatternAnimBg--;
						if (g_nPatternAnimBg == -STAGE1_X)	//枚数
						{
							g_nPatternAnimBg = 0;
						}
					}
				}

				//一番左以外は、背景を流す
				if (g_moveBg.x < 0)
				{
					g_moveBg.x = 0;
				}
			}
		}

		//画面の端では背景は動いていない
		if (g_moveBg.x == 0)
		{
			g_bMoveBg = false;
		}
		else if (g_moveBg.x == STAGE1_X)
		{
			g_bMoveBg = false;
		}
		else
		{
			g_bMoveBg = true;
		}

		//テクスチャ座標を更新
		pVtxBg[0].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f, 0.0f);
		pVtxBg[1].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f + 0.997f, 0.0f);
		pVtxBg[2].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f, 1.0f);
		pVtxBg[3].tex = D3DXVECTOR2(g_nPatternAnimBg *0.003f + 0.997f, 1.0f);
	}

	//ゲーム2(塔の内部)
	if (g_ModeBg == MODE_GAME2)
	{
		//ブロックが停止位置にたどり着いたかどうかを取得
		g_bBlockYB = GetBlockY();

		//下に流れるアニメーション
		if (g_bBlockYB == false)
		{
			g_nCounterAnimBgRight++;					//カウンタ加算
			if (g_nCounterAnimBgRight == 2)			//速さ
			{
				g_nCounterAnimBgRight = 0;
				g_nPatternAnimBg++;
				if (g_nPatternAnimBg == 1000)	//枚数
				{
					g_nPatternAnimBg = 0;
				}
			}
		}

		g_bMoveBg = false;	//背景は動いていない

		//テクスチャ座標を更新
		pVtxBg[0].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg *0.002f);
		pVtxBg[1].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg *0.002f);
		pVtxBg[2].tex = D3DXVECTOR2(0.0f, g_nPatternAnimBg *0.002f - 0.998f);
		pVtxBg[3].tex = D3DXVECTOR2(1.0f, g_nPatternAnimBg *0.002f - 0.998f);
	}

	//ゲーム3(塔の頂)
	if (g_ModeBg == MODE_GAME3)
	{
		g_bMoveBg = false;	//背景は動いていない
		pVtxBg[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtxBg[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtxBg[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtxBg[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//背景を揺らす
	if (g_bShakeBg == true)
	{
		g_nShakeMoveBg++;	//カウンタ加算
		if (g_nShakeMoveBg >= SHAKE_BG)		//沈む深さ
		{
			g_nShakeMoveBg = SHAKE_BG;
			g_nShakeMoveBg2--;
			if (g_nShakeMoveBg2 <= -SHAKE_BG)	//戻る深さ
			{
				g_nShakeMoveBg = 0;
				g_nShakeMoveBg2 = 0;
				g_bShakeBg = false;
			}
		}
	}
	else
	{
		g_nShakeMoveBg = 0;
		g_nShakeMoveBg2 = 0;
	}

	//頂点カラーを設定（0～255の値で設定する）
	pVtxBg[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxBg[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点座標の設定（右回りで設定する）
	pVtxBg[0].pos = D3DXVECTOR3(0.0f, -7.0f + g_nShakeMoveBg + g_nShakeMoveBg2, 0.0f);
	pVtxBg[1].pos = D3DXVECTOR3(SCREEN_WIDTH, -7.0f + g_nShakeMoveBg + g_nShakeMoveBg2, 0.0f);
	pVtxBg[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHTⅡ + g_nShakeMoveBg + g_nShakeMoveBg2, 0.0f);
	pVtxBg[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHTⅡ + g_nShakeMoveBg + g_nShakeMoveBg2, 0.0f);

	//頂点データをアンロックする
	g_pVtxBuffPolygonBg->Unlock();
}

//=============================================================================
// バックグラウンド描画処理
//=============================================================================
void DrawBg(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonBg, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ゲーム1の背景
	if (g_ModeBg == MODE_GAME)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonBg[0]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
	}

	//ゲーム2の背景
	if (g_ModeBg == MODE_GAME2)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonBg[1]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
	}

	//ゲーム3の背景
	if (g_ModeBg == MODE_GAME3)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonBg[2]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
	}

}

//==============================================================================
// バッググラウンドの取得
//==============================================================================
bool GetBg(void)
{
	return g_bMoveBg;
}