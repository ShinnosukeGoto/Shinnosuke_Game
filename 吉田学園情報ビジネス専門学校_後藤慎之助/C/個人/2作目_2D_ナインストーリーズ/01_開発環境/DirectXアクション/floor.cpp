//=============================================================================
//
// ナインストーリーズ [floor.cpp]の処理 (ゲーム画面の床のテクスチャ)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "bg.h"
#include "player.h"
#include "input.h"
#include "enemy.h"
#include "block.h"
#include "floor.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonFloor[3] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonFloor = NULL;
D3DXVECTOR3 g_moveFloor;		//背景の移動量（ステージ全体の長さを決める）
int g_nCounterAnimFloorLeft;	//左のアニメーションカウンター
int g_nCounterAnimFloorRight;	//右のアニメーションカウンター
int g_nPatternAnimFloor;		//アニメーションパターン
int g_nShakeMoveFloor;			//背景が揺れるときの数値(沈む）
int g_nShakeMoveFloor2;			//背景が揺れるときの数値(戻る)
bool g_bMoveFloor;				//背景が動いている
bool g_bShakeFloor;				//背景が揺れる
bool g_bBlockXF;				//ブロックに横から当たった時
MODE g_ModeFl;					//ゲーム画面に対応した床

//=============================================================================
//床初期化処理
//=============================================================================
void InitFloor(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/floor000.png", &g_pTexturePolygonFloor[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/floor001.png", &g_pTexturePolygonFloor[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/floor002.png", &g_pTexturePolygonFloor[2]);

	//グローバル変数の初期化
	g_moveFloor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nCounterAnimFloorLeft = 0;
	g_nCounterAnimFloorRight = 0;
	g_nPatternAnimFloor = 0;
	g_nShakeMoveFloor = 0;
	g_nShakeMoveFloor2 = 0;
	g_bMoveFloor = false;	//最初は、背景は動かない
	g_bShakeFloor = false;	//最初は、背景は揺れない
	g_bBlockXF = false;
	g_ModeFl = MODE_GAME;

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonFloor, NULL);
	VERTEX_2D *pVtxFloor;	//床の頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffPolygonFloor->Lock(0, 0, (void**)&pVtxFloor, 0);	//この書式は変えないこと

	//頂点座標の設定（右回りで設定する）
	pVtxFloor[0].pos = D3DXVECTOR3(0.0f, 580.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	pVtxFloor[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 580.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	pVtxFloor[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	pVtxFloor[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);

	//rhwの設定（値は1.0で設定する）
	pVtxFloor[0].rhw = 1.0f;
	pVtxFloor[1].rhw = 1.0f;
	pVtxFloor[2].rhw = 1.0f;
	pVtxFloor[3].rhw = 1.0f;

	//頂点カラーを設定（0～255の値で設定する）
	pVtxFloor[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxFloor[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxFloor[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtxFloor[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標を更新
	pVtxFloor[0].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f, 0.0f);
	pVtxFloor[1].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f + 0.9925f, 0.0f);
	pVtxFloor[2].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f, 1.0f);
	pVtxFloor[3].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f + 0.9925f, 1.0f);

	//頂点データをアンロックする
	g_pVtxBuffPolygonFloor->Unlock();
}

//=============================================================================
// 床終了処理
//=============================================================================
void UninitFloor(void)
{

	//テクスチャの開放
	for (int nCntBg = 0; nCntBg < 3; nCntBg++)
	{
		if (g_pTexturePolygonFloor[nCntBg] != NULL)
		{
			g_pTexturePolygonFloor[nCntBg]->Release();
			g_pTexturePolygonFloor[nCntBg] = NULL;
		}
	}

	//頂点バッファの開放
	if (g_pVtxBuffPolygonFloor != NULL)
	{
		g_pVtxBuffPolygonFloor->Release();
		g_pVtxBuffPolygonFloor = NULL;
	}
}

//=============================================================================
// 床更新処理
//=============================================================================
void UpdateFloor(void)
{
	VERTEX_2D *pVtxFloor;	//頂点情報へのポインタ
	PLAYER*pPlayer = GetPlayer();		//プレイヤーの状況を取得
	g_ModeFl = GetMode();

	//プレイヤーが攻撃を受けたかどうか取得
	g_bShakeFloor = HitPlayer2();

	g_bBlockXF = GetBlockX();

	//プレイヤーが攻撃を受けていないなら、敵が攻撃を受けているかどうかを取得
	if (g_bShakeFloor == false)
	{
		g_bShakeFloor = HitEnemy2();
	}

	g_pVtxBuffPolygonFloor->Lock(0, 0, (void**)&pVtxFloor, 0);	//この書式は変えないこと

	//ゲーム1(雪の平原)
	if (g_ModeFl == MODE_GAME)
	{
		//プレイヤーが右に進むにつれて、右にあるものが出てくる
		if (pPlayer->bDisp == true)
		{

			//AとD同時押しは無効
			if (GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_D))
			{

			}

			//右に進む
			else if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D) && g_bBlockXF == false)
			{
				if (pPlayer->pos.x == SCREEN_WIDTH / 2)	//プレイヤーが中央にいるときだけ背景を流す
				{
					g_moveFloor.x++;
					g_nCounterAnimFloorRight++;					//カウンタ加算
					if (g_nCounterAnimFloorRight == 2)			//速さ
					{
						g_nCounterAnimFloorRight = 0;
						g_nPatternAnimFloor++;
						if (g_nPatternAnimFloor == STAGE1_X)	//枚数
						{
							g_nPatternAnimFloor = 0;
						}
					}
				}

				//一番右以外は、背景を流す
				if (g_moveFloor.x > STAGE1_X)
				{
					g_moveFloor.x = STAGE1_X;
				}
			}

			//左に進む
			else if (pPlayer->nAttackTime == 0 && g_bBlockXF == false && GetKeyboardPress(DIK_A))
			{
				if (pPlayer->pos.x == SCREEN_WIDTH / 2)	//プレイヤーが中央にいるときだけ背景を流す
				{
					g_moveFloor.x--;
					g_nCounterAnimFloorLeft++;					//カウンタ加算
					if (g_nCounterAnimFloorLeft == 2)			//速さ
					{
						g_nCounterAnimFloorLeft = 0;
						g_nPatternAnimFloor--;
						if (g_nPatternAnimFloor == -STAGE1_X)	//枚数
						{
							g_nPatternAnimFloor = 0;
						}
					}
				}

				//一番左以外は、背景を流す
				if (g_moveFloor.x < 0)
				{
					g_moveFloor.x = 0;
				}
			}

		}

		//画面の端では背景は動いていない
		if (g_moveFloor.x == 0)
		{
			g_bMoveFloor = false;
		}
		else if (g_moveFloor.x == STAGE1_X)
		{
			g_bMoveFloor = false;
		}
		else
		{
			g_bMoveFloor = true;
		}

		//テクスチャ座標を更新
		pVtxFloor[0].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f, 0.0f);
		pVtxFloor[1].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f + 0.9925f, 0.0f);
		pVtxFloor[2].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f, 1.0f);
		pVtxFloor[3].tex = D3DXVECTOR2(g_nPatternAnimFloor *0.0075f + 0.9925f, 1.0f);
	}


	//ゲーム2(塔内部)
	if (g_ModeFl == MODE_GAME2)
	{
		//テクスチャ座標を更新
		pVtxFloor[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtxFloor[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtxFloor[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtxFloor[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//ゲーム3(塔の頂)
	if (g_ModeFl == MODE_GAME3)
	{
		//テクスチャ座標を更新
		pVtxFloor[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtxFloor[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtxFloor[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtxFloor[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	//背景を揺らす
	if (g_bShakeFloor == true)
	{
		g_nShakeMoveFloor++;	//カウンタ加算
		if (g_nShakeMoveFloor >= SHAKE_FLOOR)		//沈む深さ
		{
			g_nShakeMoveFloor = SHAKE_FLOOR;
			g_nShakeMoveFloor2--;
			if (g_nShakeMoveFloor2 <= -SHAKE_FLOOR)	//戻る深さ
			{
				g_nShakeMoveFloor = 0;
				g_nShakeMoveFloor2 = 0;
				g_bShakeFloor = false;
			}
		}
	}
	else
	{
		g_nShakeMoveFloor = 0;
		g_nShakeMoveFloor2 = 0;
	}

	//ゲーム1、ゲーム2
	if (g_ModeFl == MODE_GAME|| g_ModeFl == MODE_GAME2)
	{

		//頂点座標の設定（右回りで設定する）
		pVtxFloor[0].pos = D3DXVECTOR3(0.0f, 580.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 580.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	}

	//ゲーム3
	if (g_ModeFl == MODE_GAME3)
	{

		//頂点座標の設定（右回りで設定する）
		pVtxFloor[0].pos = D3DXVECTOR3(0.0f, 500.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 500.0f + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
		pVtxFloor[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT + g_nShakeMoveFloor + g_nShakeMoveFloor2, 0.0f);
	}

	//頂点データをアンロックする
	g_pVtxBuffPolygonFloor->Unlock();
}

//=============================================================================
// 床描画処理
//=============================================================================
void DrawFloor(void)
{
	//宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPolygonFloor, 0, sizeof(VERTEX_2D));
	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ゲーム1
	if (g_ModeFl == MODE_GAME)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonFloor[0]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	//ゲーム2
	if (g_ModeFl == MODE_GAME2)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonFloor[1]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	//ゲーム3
	if (g_ModeFl == MODE_GAME3)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygonFloor[2]);
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

}