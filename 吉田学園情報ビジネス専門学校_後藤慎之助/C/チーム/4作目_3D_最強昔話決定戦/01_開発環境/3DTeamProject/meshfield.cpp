//=============================================================================
//
//  最強昔話決定戦の処理  [meshfield.cpp](プレイヤーの歩ける床)
//  Author:KUDO AYANO
//
//=============================================================================
#include "meshfield.h"
#include "setting.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFILD_TEXTURE "TEXTURE/check.png"

//*****************************************************************************
// グローバル変数
//*********a********************************************************************
LPDIRECT3DTEXTURE9 g_pVtxTextureMeshField = NULL;        //メッシュフィールドのテクスチャ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;      //頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;       //インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshField;	//メッシュフィールドの位置
D3DXVECTOR3 g_rotMashField;	//メッシュフィールドの向き
D3DXMATRIX g_mtxWorldMeshField;	//メッシュフィールドのワールドマトリックス

int g_nMeshBlockX;   //メッシュのブロック数(x方向)
int g_nMeshBlockZ;   //メッシュのブロック数(z方向)
int g_nMaxMeshBlock; //メッシュブロックの合計
int g_nMeshVertex;   //頂点の数
int g_nMeshIndex;    //インデックス数
int g_nMeshPolygon;  //ポリゴンの数

//=============================================================================
// メッシュフィールド初期化処理
// Author:KUDO AYANO
//=============================================================================
void InitMeshField(void)
{
    //バトルロイヤルかストーリーかを取得する
    SETTING *pSetting = GetSetting();

    //バトルロイヤルなら
    if (pSetting->nMaxPlayer >= 2)
    {
        g_nMeshBlockX = MESH_BLOCK_X;
        g_nMeshBlockZ = MESH_BLOCK_Z;
        g_nMaxMeshBlock = MAX_MESH_BLOCK;
        g_nMeshVertex = MESH_VERTEX;
        g_nMeshIndex = MESH_INDEX;
        g_nMeshPolygon = MESH_POLYGON;
    }

    //ストーリーなら
    else
    {
        g_nMeshBlockX = STORY_MESH_BLOCK_X;
        g_nMeshBlockZ = STORY_MESH_BLOCK_Z;
        g_nMaxMeshBlock = MAX_STORY_MESH_BLOCK;
        g_nMeshVertex = STORY_MESH_VERTEX;
        g_nMeshIndex = STORY_MESH_INDEX;
        g_nMeshPolygon = STORY_MESH_POLYGON;
    }

    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

    g_posMeshField = D3DXVECTOR3(-GRID_SIZE/2, 0.0f, -GRID_SIZE/2);
    g_rotMashField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    VERTEX_3D *pVtx;//頂点情報のポインタ
    int nCntIndex = 0;
    int cylinder_v_count = 9;
    int cylinder_h_count = 8;
    //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nMeshVertex,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffMeshField,//変数名が変わると変更が必要
        NULL);


    HRESULT hr = D3DXCreateTextureFromFile(pDevice, MESHFILD_TEXTURE, &g_pVtxTextureMeshField);//テクスチャの読み込み

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //頂点情報の初期化
    for (int nCntMeshZ = 0; nCntMeshZ <= g_nMeshBlockZ; nCntMeshZ++)
    {
        for (int nCntMeshX = 0; nCntMeshX <= g_nMeshBlockX; nCntMeshX++)
        {
            pVtx[nCntIndex].pos = D3DXVECTOR3(nCntMeshX * GRID_SIZE, 0.0f, nCntMeshZ * GRID_SIZE);
            pVtx[nCntIndex].tex = D3DXVECTOR2((float)nCntMeshX / 2.0f, (float)nCntMeshZ / 2.0f);
            nCntIndex++;

        }
    }

    //頂点情報の設定
    for (nCntIndex = 0; nCntIndex < g_nMeshVertex; nCntIndex++)
    {
        pVtx[nCntIndex].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//法線ベクトルの設定　
        pVtx[nCntIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//頂点カラーの設定

    }


    //頂点データアンロック
    g_pVtxBuffMeshField->Unlock();


    //インデックスバッファ(ポリゴンバッファ)の生成
    pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshIndex,
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &g_pIdxBuffMeshField,
        NULL);

    WORD *pIdx = NULL;
    g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);//インデックスバッファをロック
    nCntIndex = 0;//カウントリセット
    for (int nCntIndexZ = 0; nCntIndexZ < g_nMeshBlockZ; nCntIndexZ++)
    {
        if (nCntIndex != 0)
        {
            pIdx[nCntIndex] = (g_nMeshBlockX + 1) * (nCntIndexZ + 1);
            nCntIndex++;
        }
        for (int nCntIndexX = 0; nCntIndexX <= g_nMeshBlockX; nCntIndexX++)
        {
            pIdx[nCntIndex] = nCntIndexX + (g_nMeshBlockX + 1)* (nCntIndexZ + 1);
            pIdx[nCntIndex + 1] = pIdx[nCntIndex] - (g_nMeshBlockX + 1);
            nCntIndex += 2;//インデックスのポインタを2進める
        }
        if (nCntIndex != g_nMeshIndex)
        {
            //Zが最後以外の場合、縮退ポリゴン用のインデックスを設定する
            pIdx[nCntIndex] = pIdx[nCntIndex - 1];
            nCntIndex++;
        }
    }

    g_pIdxBuffMeshField->Unlock();//インデックスバッファのアンロック
}

//=============================================================================
// メッシュフィールド終了処理
//  Author:KUDO AYANO
//=============================================================================
void UninitMeshField(void)
{

    if (g_pVtxBuffMeshField != NULL)
    {
        g_pVtxBuffMeshField->Release();
        g_pVtxBuffMeshField = NULL;
    }
    //インデックスバッファの開放
    if (g_pIdxBuffMeshField != NULL)
    {
        g_pIdxBuffMeshField->Release();
        g_pIdxBuffMeshField = NULL;
    }

    if (g_pVtxTextureMeshField != NULL)
    {
        g_pVtxTextureMeshField->Release();
        g_pVtxTextureMeshField = NULL;
    }
}

//=============================================================================
// メッシュフィールド描画処理
// Author:KUDO AYANO
//=============================================================================
void DrawMeshField(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス
                                //ワールドマトリクスの初期化
    D3DXMatrixIdentity(&g_mtxWorldMeshField);
    //向きを反転
    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMashField.y, g_rotMashField.x, g_rotMashField.z);
    D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);
    //位置を反転
    D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
    D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);
    //ワールドマトリックスの設定
    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);
    //頂点バッファをデバイスのデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));
    //インデックスバッファをデータストリームに設定
    pDevice->SetIndices(g_pIdxBuffMeshField);
    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_3D);
    pDevice->SetTexture(0, g_pVtxTextureMeshField);
    //ポリゴンの描画
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nMeshVertex, 0, g_nMeshPolygon);

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}