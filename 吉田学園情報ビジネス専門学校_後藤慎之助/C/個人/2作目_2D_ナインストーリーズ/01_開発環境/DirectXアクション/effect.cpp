//=============================================================================
//
// ナインストーリーズの処理 [effect.cpp] (自機の攻撃のエフェクト)
// Author : 後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "effect.h"
#include "attack.h"
#include "magic.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;//頂点バッファへのポインタ

EFFECT g_aEffect[MAX_EFFECT];	//エフェクトの情報
//=============================================================================
// エフェクト初期化処理
//=============================================================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	VERTEX_2D *pVtx;//頂点情報のポインタ
	int nCntEffect;//弾のカウント

	//オブジェクトの頂点バッファ生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_EFFECT,//バッファサイズ
		D3DUSAGE_WRITEONLY,//(固定）
		FVF_VERTEX_2D,//頂点フォーマット
		D3DPOOL_MANAGED,//（固定）
		&g_pVtxBuffEffect,//変数名が変わると変更が必要
		NULL);

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//弾の情報の初期化
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].aRadius = 0;
		g_aEffect[nCntEffect].bUse = false;
		//頂点座標の設定（右回りで設定する）
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(0, 0, 0, 0);
		pVtx[1].col = D3DCOLOR_RGBA(0, 0, 0, 0);
		pVtx[2].col = D3DCOLOR_RGBA(0, 0, 0, 0);
		pVtx[3].col = D3DCOLOR_RGBA(0, 0, 0, 0);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データアンロック
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// エフェクト更新処理
//=============================================================================
void UpdateEffect(void)
{
	VERTEX_2D *pVtx;//頂点情報のポインタ
	int nCntEffect;//エフェクトのカウント
	//ATTACK*pAttack = GetAttack();
	//MAGIC*pMagic = GetMagic();

	//頂点バッファロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//エフェクトの持続時間減少
			g_aEffect[nCntEffect].nLife -=2;
			//エフェクトの半径を減らす
			g_aEffect[nCntEffect].aRadius--;
			//エフェクトの透明度を下げる
			g_aEffect[nCntEffect].col.a--;

			//上向き
			if (g_aEffect[nCntEffect].nDirect == DIRECT_UP)
			{
				//テクスチャ座標
				pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[0].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//下向き
			if (g_aEffect[nCntEffect].nDirect == DIRECT_DOWN)
			{
				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//右向き
			if (g_aEffect[nCntEffect].nDirect == DIRECT_RIGHT)
			{
				//テクスチャ座標
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//左向き
			if (g_aEffect[nCntEffect].nDirect == DIRECT_LEFT)
			{
				//テクスチャ座標
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].aRadius, 0.0f);

			if (g_aEffect[nCntEffect].nLife <= 0)	//エフェクトの半径が0になった時
			{
				g_aEffect[nCntEffect].bUse = false;	//未使用状態にする
			}

		}
		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点データアンロック
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		//テクスチャの開放
		if (g_pTextureEffect != NULL)
		{
			g_pTextureEffect->Release();
			g_pTextureEffect = NULL;
		}
		//頂点バッファの開放
		if (g_pVtxBuffEffect != NULL)
		{
			g_pVtxBuffEffect->Release();
			g_pVtxBuffEffect = NULL;
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEffect;//エフェクトのカウント

	//頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//レンダーステート(加算合成）
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			//弾が使用中なら描画
			// 描画の種類			描画開始頂点	何個描画するか
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntEffect, NUM_POLYGON);
		}
	}

	//レンダーステート(通常ブレンド処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
//エフェクトの設定
//=============================================================================
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float aRadius, int nLife, DIRECT nDirect)
{
	VERTEX_2D *pVtx;//頂点情報のポインタ
	int nCntEffect;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].aRadius = aRadius;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nDirect = nDirect;
											  //頂点カラーの更新
			pVtx[0].col = g_aEffect[nCntEffect].col;
			pVtx[1].col = g_aEffect[nCntEffect].col;
			pVtx[2].col = g_aEffect[nCntEffect].col;
			pVtx[3].col = g_aEffect[nCntEffect].col;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].aRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].aRadius, g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].aRadius, 0.0f);

			g_aEffect[nCntEffect].bUse = true;//使用している状態にする

			break;
		}
		pVtx += 4;//頂点データのポインタを4つ分進める
	}
	//頂点データアンロック
	g_pVtxBuffEffect->Unlock();
}

//=============================================================================
//エフェクトを取得
//=============================================================================
EFFECT *GetEffect(void)
{
	return &g_aEffect[0];
}