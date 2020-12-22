//=============================================================================
//
// ナインストーリーズの処理 [block.cpp](ブロック)
// Author:後藤慎之助
//
//=============================================================================
#include "block.h"
#include "player.h"
#include "input.h"
#include "bg.h"
#include "game.h"
#include "game2.h"
#include "fade.h"
#include "explosion.h"
#include "item.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureBlock[BLOCKTYPE_MAX] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// 頂点バッファへのポインタ

int g_nCounterAnimBlock;	//アニメーションカウンター
int g_nPatternAnimBlock;		//アニメーションパターン
BLOCK g_aBlock[MAX_BLOCK];
bool g_bBgXB;					//ステージ1の横が動くかどうか
bool g_bBlockX;					//ブロックの横に触れているかどうか
bool g_bBlockY;					//ステージ2でブロックが停止位置までたどり着いたかどうか
MODE g_ModeBlock;				//ゲーム画面に対応

//=====================================================
// 初期化処理
//=====================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntBlock;		// ローカル変数

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].nSizeX = 0;
		g_aBlock[nCntBlock].nSizeY = 0;
		g_aBlock[nCntBlock].type = BLOCKTYPE_000;
		g_aBlock[nCntBlock].nLife = 0;
		g_aBlock[nCntBlock].bUse = false;
	}

	//グローバル変数の初期化
	g_nCounterAnimBlock = 0;
	g_nPatternAnimBlock = 0;
	g_bBgXB = false;
	g_bBlockX = false;
	g_bBlockY = false;
	g_ModeBlock = MODE_GAME;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block000.png",&g_pTextureBlock[0]);		//通り抜け不可能なブロック
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block002.png", &g_pTextureBlock[1]);	//通り抜け可能な板
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object000.png", &g_pTextureBlock[2]);	//Wやじるし看板
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object001.png", &g_pTextureBlock[3]);	//家
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/entrance000.png", &g_pTextureBlock[4]);	//塔入口
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block002.png", &g_pTextureBlock[5]);	//通り抜け不可能なブロック
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block004.png", &g_pTextureBlock[6]);	//つらら
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/effect002.png", &g_pTextureBlock[7]);	//塔の頂へのワープゾーン
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object002.png", &g_pTextureBlock[8]);	//W看板
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block005.png", &g_pTextureBlock[9]);	//壊せるブロック
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/tower000.png", &g_pTextureBlock[10]);	//タワー
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block003.png", &g_pTextureBlock[11]);	//水
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object003.png", &g_pTextureBlock[12]);	//縦に長い家
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object004.png", &g_pTextureBlock[13]);	//Sやじるし看板
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/object005.png", &g_pTextureBlock[14]);	//アスレチックの柱
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/block006.png", &g_pTextureBlock[15]);	//トゲブロック

	// 頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BLOCK,		// バッファサイズ
		D3DUSAGE_WRITEONLY,					// (固定)
		FVF_VERTEX_2D,						// 頂点フォーマット
		D3DPOOL_MANAGED,					// (固定)
		&g_pVtxBuffBlock,					// 変数名が変わると変更が必要
		NULL);

	VERTEX_2D * pVtx;		// 頂点情報へのポインタ

	// 頂点データの範囲をロックし、頂点バッファのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);	// この書式は変えないこと

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		// 頂点座標の設定( Zの形に配置する)  / 四角形の頂点の位置
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// rhwの設定(値は1.0で固定)		/ あまり変えることはない
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		// 頂点カラーの設定(0～255の数値で設定)	/ 全体の色
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
															// テクスチャ座標の設定		画面に対しての分割数
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点データをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// ブロックの終了処理
//=============================================================================
void UninitBlock(void)
{
	//テクスチャの開放
	for (int nCntBlock = 0; nCntBlock < BLOCKTYPE_MAX; nCntBlock++)
	{
		if (g_pTextureBlock[nCntBlock] != NULL)
		{
			g_pTextureBlock[nCntBlock]->Release();
			g_pTextureBlock[nCntBlock] = NULL;
		}
	}
	//頂点バッファの開放
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=============================================================================
// ブロックの更新処理
//=============================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;//頂点情報のポインタ
	PLAYER*pPlayer;
	ITEM*pItem;

	//ゲームのステージを取得
	g_ModeBlock = GetMode();

	//頂点バッファロック
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//ゲーム1(雪の平原)
			if (g_ModeBlock == MODE_GAME)
			{
				//背景の横が動くかどうか取得
				g_bBgXB = GetBg();
				pPlayer = GetPlayer();

				//プレイヤーの操作に対応した移動
				if (pPlayer->bDisp == true && g_bBgXB == true && g_bBlockX == false)
				{
					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
					{
						g_aBlock[nCntBlock].pos.x -= VALUE_MOVE;
					}

					if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
					{
						g_aBlock[nCntBlock].pos.x += VALUE_MOVE;
					}
				}

				//壊せるブロック
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_009)
				{
					//攻撃が当たると壊れる
					if (g_aBlock[nCntBlock].nLife <= 0)
					{
						//爆発生成
						SetExplosion(g_aBlock[nCntBlock].pos, D3DXCOLOR(255, 255, 255, 255), g_aBlock[nCntBlock].nSizeX);
						g_aBlock[nCntBlock].bUse = false;
					}
				}

				//水
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_011)
				{
					//パターンを切り替える
					g_nCounterAnimBlock++;
					if (g_nCounterAnimBlock == 20)
					{
						g_nCounterAnimBlock = 0;
						g_nPatternAnimBlock++;
						if (g_nPatternAnimBlock == 3)
						{
							g_nPatternAnimBlock = 0;
						}
					}

					//テクスチャ座標を更新
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.333f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.333f + 0.333f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.333f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.333f + 0.333f, 1.0f);
				}
				else
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
			}

			//ゲーム2(塔内部)
			if (g_ModeBlock == MODE_GAME2)
			{
				//リフトに乗っているから、ブロックは下に移動
				g_aBlock[nCntBlock].pos.y += 1.0;

				//リフトが止まるから、ブロックも止まる
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_001)	//左右の足場
				{
					if (g_aBlock[nCntBlock].pos.y > 480)
					{
						g_aBlock[nCntBlock].pos.y = 480;
					}
				}

				//リフトが止まるから、ブロックも止まる
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_005)	//真ん中の足場
				{
					if (g_aBlock[nCntBlock].pos.y > 280)
					{
						g_aBlock[nCntBlock].pos.y = 280;
					}
				}

				//リフトが止まるから、W看板も止まる
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_008)	//W看板
				{
					if (g_aBlock[nCntBlock].pos.y > 240)
					{
						g_aBlock[nCntBlock].pos.y = 240;
					}
				}

				//リフトが止まるから、ワープゾーンも止まる
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_007)	//真ん中のワープゾーン
				{
					if (g_aBlock[nCntBlock].pos.y > 230)
					{
						g_aBlock[nCntBlock].pos.y = 230;
						g_bBlockY = true;
					}

					//パターンを切り替える
					g_nCounterAnimBlock++;
					if (g_nCounterAnimBlock == 3)
					{
						g_nCounterAnimBlock = 0;
						g_nPatternAnimBlock++;
						if (g_nPatternAnimBlock == 10)
						{
							g_nPatternAnimBlock = 0;
						}
					}

					//テクスチャ座標を更新
					pVtx[0].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.1f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.1f + 0.1f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.1f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(g_nPatternAnimBlock * 0.1f + 0.1f, 1.0f);
				}
				else
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
			}

			//ゲーム3(塔の頂)
			if (g_ModeBlock == MODE_GAME3)
			{
				pItem = GetItem();	//姫の状況を取得

				//つらら
				if (g_aBlock[nCntBlock].type == BLOCKTYPE_006)
				{
					//重力
					g_aBlock[nCntBlock].pos.y += 2.5;	//つららの落下速度(2.0)

					//床に当たる
					if (g_aBlock[nCntBlock].pos.y > 650 - (g_aBlock[nCntBlock].nSizeY / 2))
					{
						g_aBlock[nCntBlock].pos.y = 650 - (g_aBlock[nCntBlock].nSizeY / 2);
						g_aBlock[nCntBlock].nLife--;//床に当たることでライフを減らす
					}

					//姫を救出したらつららを溶かす
					if (pItem->type == ITEMTYPE_005 && pItem->bUse == true)
					{
						g_aBlock[nCntBlock].nLife = 0;
					}

					//時間でつららが溶ける
					if (g_aBlock[nCntBlock].nLife <= 0)
					{
						g_aBlock[nCntBlock].col.r -= 0.01;
						g_aBlock[nCntBlock].col.a -= 0.02;

						//透明度が0.1を下回ったら未使用に
						if (g_aBlock[nCntBlock].col.a <= 0.1)
						{
							g_aBlock[nCntBlock].bUse = false;
						}
					}

					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

					//頂点カラーの更新
					pVtx[0].col = D3DXCOLOR(g_aBlock[nCntBlock].col.r, g_aBlock[nCntBlock].col.g, g_aBlock[nCntBlock].col.b, g_aBlock[nCntBlock].col.a);
					pVtx[1].col = D3DXCOLOR(g_aBlock[nCntBlock].col.r, g_aBlock[nCntBlock].col.g, g_aBlock[nCntBlock].col.b, g_aBlock[nCntBlock].col.a);
					pVtx[2].col = D3DXCOLOR(g_aBlock[nCntBlock].col.r, g_aBlock[nCntBlock].col.g, g_aBlock[nCntBlock].col.b, g_aBlock[nCntBlock].col.a);
					pVtx[3].col = D3DXCOLOR(g_aBlock[nCntBlock].col.r, g_aBlock[nCntBlock].col.g, g_aBlock[nCntBlock].col.b, g_aBlock[nCntBlock].col.a);
				}
			}

			//頂点座標の設定（右回りで設定する）
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);

		}
		pVtx += 4;
	}

	//頂点データアンロック
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// ブロックの描画処理
//=============================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBlock[g_aBlock[nCntBlock].type]);

			// 描画の種類		描画開始頂点	何個描画するか
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBlock, NUM_POLYGON);
		}
	}
}

//=====================================================
// ブロックの設定
//=====================================================
void SetBlock(D3DXVECTOR3 pos, int nSizeX, int nSizeY, BLOCKTYPE type, int nLife)
{
	VERTEX_2D *pVtx;
	int nCntBlock;		// ローカル変数

	//頂点データをロック
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].nSizeX = nSizeX;
			g_aBlock[nCntBlock].nSizeY = nSizeY;
			g_aBlock[nCntBlock].type = type;
			g_aBlock[nCntBlock].nLife = nLife;

			g_aBlock[nCntBlock].col.r = 1.0f;
			g_aBlock[nCntBlock].col.g = 1.0f;
			g_aBlock[nCntBlock].col.b = 1.0f;
			g_aBlock[nCntBlock].col.a = 1.0f;

			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2), 0.0f);

			g_aBlock[nCntBlock].bUse = true;
			break;
		}
		pVtx += 4;
	}

	// 頂点データをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// ブロック情報の送信
//=============================================================================
BLOCK *GetBlock(void)
{
	return &g_aBlock[0];
}

//=============================================================================
//ブロックの当たり判定
//=============================================================================
bool CollisionBlock(D3DXVECTOR3 * pPos, D3DXVECTOR3 * pPosOld, int nSize)
{
	PLAYER *pPlayer;
	int nCntBlock;		// ローカル変数

	bool bLand = false;		//着地したのかどうか
	g_bBlockX = false;		//ブロックに横から当たっているかどうか
	pPlayer = GetPlayer();	//プレイヤーの状況を獲得
	int nFade = GetFade();	//フェードの情報を獲得

	//背景の横が動くかどうか取得
	g_bBgXB = GetBg();

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//通り抜けできないブロック
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_000 || g_aBlock[nCntBlock].type == BLOCKTYPE_009)
			{
				//背景が動かないとき
				if (g_bBgXB == false)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに上から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// ブロックの上に乗る
							bLand = true;		// ブロックの上に着地している
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに下から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeY / 2);		// プレイヤーがブロックにめり込まないようにする
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに右から当たった時
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeX / 2);
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに左から当たった時
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeX / 2);
						}
					}
				}

				//背景が動くとき
				if (g_bBgXB == true)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに上から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// ブロックの上に乗る
							bLand = true;		// ブロックの上に着地している
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに下から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeY / 2);		// プレイヤーがブロックにめり込まないようにする
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->pos.x + (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに右から当たった時
							//左向きなら左に動ける
							if (pPlayer->bLeft == true)
							{
								g_bBlockX = false;
							}
							else
							{
								g_bBlockX = true;
							}
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->pos.x - (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに左から当たった時
							//右向きなら右に動ける
							if (pPlayer->bLeft == false)
							{
								g_bBlockX = false;
							}
							else
							{
								g_bBlockX = true;
							}
						}
					}
				}
			}

			//通り抜けできるブロック
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_001 || g_aBlock[nCntBlock].type == BLOCKTYPE_005)
			{
				//背景が動かないとき
				if (g_bBgXB == false)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに上から当たった時
							if (GetKeyboardPress(DIK_S))
							{
								
							}
							else
							{
								pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// ブロックの上に乗る
							}
							bLand = true;		// ブロックの上に着地している
						}
					}
				}

				//背景が動くとき
				if (g_bBgXB == true)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに上から当たった時
							if (GetKeyboardPress(DIK_S))
							{
								
							}
							else
							{
								pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// ブロックの上に乗る
							}
							bLand = true;		// ブロックの上に着地している
						}
					}
				}
			}

			//塔入口
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_004)
			{
				//背景が動かないとき
				if (g_bBgXB == false)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (GetKeyboardPress(DIK_W) && nFade == FADE_NONE)
						{
							//塔内部へ
							SetGameState(GAMESTATE_NEXTSTAGE);
						}
					}
				}

				//背景が動くとき
				if (g_bBgXB == true)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (GetKeyboardPress(DIK_W) && nFade == FADE_NONE)
						{
							//塔内部へ
							SetGameState(GAMESTATE_NEXTSTAGE);
						}
					}
				}
			}

			//つらら
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_006)
			{
				//背景が動かないとき
				if (g_bBgXB == false)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))-50) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに上から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// つららの上に乗る
							bLand = true;		// ブロックの上に着地している
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに右から当たった時
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeX / 2);
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに左から当たった時
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeX / 2);
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2)-50 &&	//つららの下は攻撃判定
							pPlayer->pos.y - (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{
							//プレイヤーがノーマル状態なら
							if (pPlayer->state == PLAYERSTATE_NORMAL)
							{
								//自機にダメージ
								HitPlayer(15);
							}
						}
					}
				}
			}

			//塔の頂へのワープゾーン
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_007)
			{
				//背景が動かないとき
				if (g_bBgXB == false)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (GetKeyboardPress(DIK_W) && nFade == FADE_NONE)
						{
							//塔内部へ
							SetGameState2(GAMESTATE2_NEXTSTAGE);
						}
					}
				}

				//背景が動くとき
				if (g_bBgXB == true)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (GetKeyboardPress(DIK_W) && nFade == FADE_NONE)
						{
							//塔内部へ
							SetGameState2(GAMESTATE2_NEXTSTAGE);
						}
					}
				}
			}

			//水
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_011)
			{
				//背景が動かないとき
				if (g_bBgXB == false)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2)) + 70) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						//浮力
						pPlayer->move.y = 0.0f;
						pPlayer->move.y -= 4.0f;
						pPlayer->bJump = false;
					}
				}

				//背景が動くとき
				if (g_bBgXB == true)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))+70) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						//浮力
						pPlayer->move.y = 0.0f;
						pPlayer->move.y -= 4.0f;
						pPlayer->bJump = false;
					}
				}
			}

			//トゲブロック
			if (g_aBlock[nCntBlock].type == BLOCKTYPE_015)
			{
				//背景が動かないとき
				if (g_bBgXB == false)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに上から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// ブロックの上に乗る
							//プレイヤーがノーマル状態なら
							if (pPlayer->state == PLAYERSTATE_NORMAL)
							{
								//自機にダメージ
								HitPlayer(15);
							}
							bLand = true;		// ブロックの上に着地している
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに下から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeY / 2);		// プレイヤーがブロックにめり込まないようにする
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに右から当たった時
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeX / 2);
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->posOld.x - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに左から当たった時
							pPlayer->pos.x = g_aBlock[nCntBlock].pos.x + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeX / 2);
						}
					}
				}

				//背景が動くとき
				if (g_bBgXB == true)
				{
					// ブロックとの当たり判定
					if (((pPlayer->pos.y + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 上
						((pPlayer->pos.y - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))) &&	// 下
						((pPlayer->pos.x + (PLAYER_SIZE / 2)) > (g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))) &&	// 右
						((pPlayer->pos.x - (PLAYER_SIZE / 2)) < (g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))))	// 左
					{
						if (pPlayer->pos.y + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y + (PLAYER_SIZE / 2) <= g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに上から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y - (PLAYER_SIZE / 2) - (g_aBlock[nCntBlock].nSizeY / 2);		// ブロックの上に乗る
							//プレイヤーがノーマル状態なら
							if (pPlayer->state == PLAYERSTATE_NORMAL)
							{
								//自機にダメージ
								HitPlayer(15);
							}
							bLand = true;		// ブロックの上に着地している
						}
						else if (pPlayer->pos.y - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2) &&
							pPlayer->posOld.y - (PLAYER_SIZE / 2) >= g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].nSizeY / 2))
						{	// ブロックに下から当たった時
							pPlayer->pos.y = g_aBlock[nCntBlock].pos.y + (PLAYER_SIZE / 2) + (g_aBlock[nCntBlock].nSizeY / 2);		// プレイヤーがブロックにめり込まないようにする
						}
						else if (pPlayer->pos.x + (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->pos.x + (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに右から当たった時
							//左向きなら左に動ける
							if (pPlayer->bLeft == true)
							{
								g_bBlockX = false;
							}
							else
							{
								g_bBlockX = true;
							}
						}
						else if (pPlayer->pos.x - (PLAYER_SIZE / 2) < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].nSizeX / 2) &&
							pPlayer->pos.x - (PLAYER_SIZE / 2) > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].nSizeX / 2))
						{	// ブロックに左から当たった時
							//右向きなら右に動ける
							if (pPlayer->bLeft == false)
							{
								g_bBlockX = false;
							}
							else
							{
								g_bBlockX = true;
							}
						}
					}
				}
			}
		}
	}
	return bLand;
}

//=============================================================================
// ブロック情報の送信(ブロックに横から当たった時)
//=============================================================================
bool GetBlockX(void)
{
	return g_bBlockX;
}

//=============================================================================
// ブロック情報の送信(ブロックが塔で停止位置にたどり着いたかどうか)
//=============================================================================
bool GetBlockY(void)
{
	return g_bBlockY;
}