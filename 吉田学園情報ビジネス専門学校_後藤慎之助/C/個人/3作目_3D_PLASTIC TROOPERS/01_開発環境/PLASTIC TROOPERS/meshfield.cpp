//=====================================================
//
// PLASTIC TROOPERS [meshfield.cpp]の処理 (床)
// Author:後藤慎之助
//
//=====================================================

//=====================================================
// インクルードファイル
//=====================================================
#include "meshfield.h"

////=============================================================================
//// プロトタイプ宣言
////=============================================================================
//void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;		// テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;

D3DXVECTOR3 g_posMeshField;				// 位置
D3DXVECTOR3 g_rotMeshField;				// 向き（回転）
D3DXMATRIX g_mtxWorldMeshField;			// ワールドマトリックス

int g_nNumVertexMeshField;				// 総頂点数
int g_nNumIndexMeshField;				// 総インデックス数
int g_nNumPolygonMeshField;				// 総ポリゴン数

float g_fOne_SizeW;
float g_fOne_SizeH;

//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_3D *pVtx;

	int nCount;

	int nCntIndex;
	int nCntIndex01 = 0;

	int nCntH;//X

	int nCntV;//Z

    //起伏試し用
    /*bool bW=false;
    int nCntW=0;*/

	// 位置・回転の初期設定
	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumVertexMeshField = (FIELD_WIDTH + 1) * (FIELD_HEIGHT + 1);
	g_nNumIndexMeshField = (FIELD_WIDTH + 1) * FIELD_HEIGHT * 2 + (FIELD_HEIGHT - 1) * 2;
	g_nNumPolygonMeshField = FIELD_WIDTH * FIELD_HEIGHT * 2 + (FIELD_HEIGHT - 1) * 4;

	g_fOne_SizeW = FIELD_WIDTH_SIZE * 2 / FIELD_WIDTH;
	g_fOne_SizeH = FIELD_HEIGHT_SIZE * 2 / FIELD_HEIGHT;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/sand4.jpg", &g_pTextureMeshField);

	// オブジェクトの頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexMeshField, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffMeshField, NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexMeshField, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &g_pIdxBuffMeshField, NULL);





	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntV = 0; nCntV < FIELD_HEIGHT + 1; nCntV++)
	{
		for (nCntH = 0; nCntH < FIELD_WIDTH + 1; nCntH++)
		{
            /*bW = !bW;

            if (bW == true)
            {
                nCntW = 1;
            }
            else
            {
                nCntW = 0;
            }*/

			// 頂点の設定
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].pos = D3DXVECTOR3(-FIELD_WIDTH_SIZE + (nCntH * g_fOne_SizeW), 0.0f, FIELD_HEIGHT_SIZE - (nCntV * g_fOne_SizeH));

			// 法線ベクトルの設定
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// 色の設定
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].tex = D3DXVECTOR2(float(nCntH), float(nCntV));
		}
	}

	// 頂点データをアンロックする
	g_pVtxBuffMeshField->Unlock();




	// インデックスバッファの設定
	WORD *pIdx;

	// インデックスバッファをロックし、インデックスデータへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (nCount = 0, nCntIndex = 0; nCount < g_nNumIndexMeshField; nCount += 2)
	{
        //縮退ポリゴンを作る
		if (((nCount + 2) % (((FIELD_WIDTH + 1) * 2) + 2)) == 0 && nCount != 0)
		{
			// インデックスの設定
			pIdx[nCount] = nCntIndex - 1;

			// インデックスの設定
			pIdx[nCount + 1] = FIELD_WIDTH + nCntIndex + 1;
		}
		else
		{
			// インデックスの設定
			pIdx[nCount] = FIELD_WIDTH + nCntIndex + 1;

			// インデックスの設定
			pIdx[nCount + 1] = nCntIndex;

			nCntIndex++;
		}

	}



	// 頂点データをアンロックする
	g_pIdxBuffMeshField->Unlock();

}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{
	// 頂点バッファの開放
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// インデックスバッファの開放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	// 拡大・縮小を反映


	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		g_rotMeshField.y,
		g_rotMeshField.x,
		g_rotMeshField.z);

	D3DXMatrixMultiply(
		&g_mtxWorldMeshField,
		&g_mtxWorldMeshField,
		&mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(
		&mtxTrans,
		g_posMeshField.x,
		g_posMeshField.y,
		g_posMeshField.z);

	D3DXMatrixMultiply(
		&g_mtxWorldMeshField,
		&g_mtxWorldMeshField,
		&mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	// 頂点バッファをデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームにバインド
	pDevice->SetIndices(g_pIdxBuffMeshField);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// 頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureMeshField);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexMeshField, 0, g_nNumPolygonMeshField);

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}
