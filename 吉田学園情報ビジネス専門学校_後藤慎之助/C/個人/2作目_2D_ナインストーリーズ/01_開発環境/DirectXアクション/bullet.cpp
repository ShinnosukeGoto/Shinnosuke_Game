//=============================================================================
//
// ナインストーリーズの処理 [bullet.cpp] (敵の弾)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "input.h"
#include "block.h"
#include "bg.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBullet[BULLETTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffBullet = NULL;
BULLET g_aBullet[MAX_BULLET];

bool g_bBgXBU;					//ステージ1の横が動くかどうか
bool g_bBlockXBU;				//ブロックに横から当たった

MODE g_ModeBullet;				//ゲーム画面に対応

int g_nCounterAnimBo;			//ボスの弾用
int g_nPatternAnimBo;			//ボスの弾のアニメーション

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
		g_aBullet[nCntBullet].nCounterAnim = 0;
		g_aBullet[nCntBullet].nPatternAnim = 0;
		g_aBullet[nCntBullet].bUse = false;
	}

	//グローバル変数の初期化
	g_bBgXBU = false;
	g_bBlockXBU = false;
	g_ModeBullet = MODE_GAME;
	g_nCounterAnimBo = 0;
	g_nPatternAnimBo = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bullet000.png", &g_pTextureBullet[0]);	//ガイコツの弾
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bullet001.png", &g_pTextureBullet[1]);	//ダークガイコツの弾
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/bullet002.png", &g_pTextureBullet[2]);	//ボスの弾

	//頂点バッファ生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX*MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

													//弾の数分
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
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
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// バレット終了処理
//=============================================================================
void UninitBullet(void)
{
	//テクスチャの開放
	for (int nCntBullet = 0; nCntBullet < BULLETTYPE_MAX; nCntBullet++)
	{
		if (g_pTextureBullet[nCntBullet] != NULL)
		{
			g_pTextureBullet[nCntBullet]->Release();
			g_pTextureBullet[nCntBullet] = NULL;
		}
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
	PLAYER*pPlayer;

	//ゲームのステージを取得
	g_ModeBullet = GetMode();

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//アニメーション
			g_aBullet[nCntBullet].nCounterAnim++;	//カウンタ加算
			if (g_aBullet[nCntBullet].nCounterAnim == 1)//速さ
			{
				g_aBullet[nCntBullet].nCounterAnim = 0;
				g_aBullet[nCntBullet].nPatternAnim++;
				if (g_aBullet[nCntBullet].nPatternAnim == 6)//枚数
				{
					g_aBullet[nCntBullet].nPatternAnim = 0;
				}
			}

			//敵を取得
			pEnemy = GetEnemy();

			//自機を取得
			pPlayer = GetPlayer();

			//背景の横が動くかどうか取得
			g_bBgXBU = GetBg();
			//ブロックに横から当たったかどうか取得
			g_bBlockXBU = GetBlockX();

			for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
			{
				//敵がやられていても、自機がいれば当たる
				if (pPlayer->bDisp == true)
				{
					//自機の当たり判定
					if (g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + (PLAYER_SIZE / 2) - 10 &&
						g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - (PLAYER_SIZE / 2) + 10 &&
						g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + (PLAYER_SIZE / 2) - 10 &&
						g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - (PLAYER_SIZE / 2) + 10)
					{
						if (pPlayer->state == PLAYERSTATE_NORMAL && g_aBullet[nCntBullet].type == BULLETTYPE_OCT)//ガイコツの弾
						{
							//自機にダメージ
							HitPlayer(20);
						}

						if (pPlayer->state == PLAYERSTATE_NORMAL && g_aBullet[nCntBullet].type == BULLETTYPE_GHO)//ダークガイコツの弾
						{
							//自機にダメージ
							HitPlayer(25);
						}

						if (pPlayer->state == PLAYERSTATE_NORMAL && g_aBullet[nCntBullet].type == BULLETTYPE_BOSS)//ボスの弾
						{
							//上に吹き飛ばす(一定距離上に吹き飛ばされたら、特殊演出ゲームオーバー)
							pPlayer->move.y = 0.0f;
							pPlayer->move.y -= 35.5f;
							//飛ばされた際、もう一度だけ空中ジャンプをできるようにする
							pPlayer->bJump = true;
							pPlayer->bSecondJump = true;
						}

						//当たった時点で弾のHPを0にする
						g_aBullet[nCntBullet].nLife = 0;
					}
				}
			}

			//ダークガイコツの弾は途中で追従
			if (g_aBullet[nCntBullet].type == BULLETTYPE_GHO)
			{
				//途中で自機の位置を取得し、そちらに飛んでいく
				if (g_aBullet[nCntBullet].nLife == 120)
				{
					//自機を取得
					pPlayer = GetPlayer();

					float fHposx = pPlayer->pos.x, fHposy = pPlayer->pos.y;							//目標(プレイヤー)の座標
					float fBposx = g_aBullet[nCntBullet].pos.x, fBposy = g_aBullet[nCntBullet].pos.y;	//敵の座標
					float fAngle;																	//角度

					fAngle = atan2f((fBposx - fHposx), (fBposy - fHposy));	//角度を決める

					g_aBullet[nCntBullet].move.x = -sin(fAngle)*11.0f;
					g_aBullet[nCntBullet].move.y = -cos(fAngle)*11.0f;
				}
			}

			//弾の移動速度(moveが等倍だと、全体的に速く、難易度が高かったため、0.5をかけている)
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move*0.5;

			//ゲーム1で、プレイヤーの操作に対応した移動
			if (g_ModeBullet == MODE_GAME && pPlayer->bDisp == true && g_bBgXBU == true && g_bBlockXBU == false)
			{
				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_D))
				{
					g_aBullet[nCntBullet].pos.x -= VALUE_MOVE;
				}

				if (pPlayer->nAttackTime == 0 && GetKeyboardPress(DIK_A))
				{
					g_aBullet[nCntBullet].pos.x += VALUE_MOVE;
				}
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y - (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y + (g_aBullet[nCntBullet].nSize / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + (g_aBullet[nCntBullet].nSize / 2), g_aBullet[nCntBullet].pos.y + (g_aBullet[nCntBullet].nSize / 2), 0.0f);

			//弾の射程を決める
			g_aBullet[nCntBullet].nLife--;

			//骨
			if (g_aBullet[nCntBullet].type == BULLETTYPE_OCT || g_aBullet[nCntBullet].type == BULLETTYPE_GHO)
			{
				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(g_aBullet[nCntBullet].nPatternAnim*0.166f + 0.166f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_aBullet[nCntBullet].nPatternAnim*0.166f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_aBullet[nCntBullet].nPatternAnim*0.166f + 0.166f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_aBullet[nCntBullet].nPatternAnim*0.166f, 1.0f);

				//ライフがなくなったら未使用に
				if (g_aBullet[nCntBullet].nLife <= 0)
				{
					g_aBullet[nCntBullet].bUse = false;	//使用しない状態にする
				}
			}

			//ボスの弾
			if (g_aBullet[nCntBullet].type == BULLETTYPE_BOSS)
			{
				//アニメーション
				g_nCounterAnimBo++;	//カウンタ加算
				if (g_nCounterAnimBo == 3)//速さ
				{
					g_nCounterAnimBo = 0;
					g_nPatternAnimBo++;
					if (g_nPatternAnimBo == 2)//枚数
					{
						g_nPatternAnimBo = 0;
					}
				}

				//テクスチャ座標の設定
				if (g_nPatternAnimBo == 0)
				{
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
				}
				else if (g_nPatternAnimBo == 1)
				{
					pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
					pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
					pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
					pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
				}

				//時間でトルネードが終わる
				if (g_aBullet[nCntBullet].nLife <= 0)
				{
					g_aBullet[nCntBullet].col.a -= 0.05;

					//透明度が0.1を下回ったら未使用に
					if (g_aBullet[nCntBullet].col.a <= 0.1)
					{
						g_aBullet[nCntBullet].bUse = false;
					}
				}
				//頂点カラーの更新
				pVtx[0].col = D3DXCOLOR(g_aBullet[nCntBullet].col.r,g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, g_aBullet[nCntBullet].col.a);
				pVtx[1].col = D3DXCOLOR(g_aBullet[nCntBullet].col.r,g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, g_aBullet[nCntBullet].col.a);
				pVtx[2].col = D3DXCOLOR(g_aBullet[nCntBullet].col.r,g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, g_aBullet[nCntBullet].col.a);
				pVtx[3].col = D3DXCOLOR(g_aBullet[nCntBullet].col.r,g_aBullet[nCntBullet].col.g, g_aBullet[nCntBullet].col.b, g_aBullet[nCntBullet].col.a);
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
			pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].type]);
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
			g_aBullet[nCntBullet].col.r = 1.0f;
			g_aBullet[nCntBullet].col.g = 1.0f;
			g_aBullet[nCntBullet].col.b = 1.0f;
			g_aBullet[nCntBullet].col.a = 1.0f;

			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].nLife = nLife;
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