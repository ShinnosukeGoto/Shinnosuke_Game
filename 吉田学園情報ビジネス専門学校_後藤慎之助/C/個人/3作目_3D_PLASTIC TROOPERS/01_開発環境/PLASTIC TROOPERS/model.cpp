//=====================================================
//
// PLASTIC TROOPERSの処理 [block.cpp](ビルなどのブロック)
// Author:後藤慎之助
//
//=====================================================

//=====================================================
// インクルードファイル
//=====================================================
#include "model.h"

//=====================================================
// グローバル変数
//=====================================================
ORNAMENT g_Model[MAX_MODEL];

// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffModel[MODELTYPE_MAX] = {};

// メッシュ情報へのポインタ
LPD3DXMESH g_pMeshModel[MODELTYPE_MAX] = {};
// マテリアル情報へのポインタ
LPD3DXBUFFER g_pBuffMatModel[MODELTYPE_MAX] = {};
// マテリアル情報の数
DWORD g_nNumMatModel[MODELTYPE_MAX] = {};


//=====================================================
// 初期化処理
//=====================================================
void InitModel(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ


    int nCntModel;

    // 配列初期化
    for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
    {
        g_Model[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Model[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Model[nCntModel].type = MODELTYPE_000;
        g_Model[nCntModel].bUse = false;
    }

    // Xファイルの読み込み
    //草
    D3DXLoadMeshFromX("MODEL/model000.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_000],
        NULL,
        &g_nNumMatModel[MODELTYPE_000],
        &g_pMeshModel[MODELTYPE_000]);

    //少し長い草
    D3DXLoadMeshFromX("MODEL/model001.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_001],
        NULL,
        &g_nNumMatModel[MODELTYPE_001],
        &g_pMeshModel[MODELTYPE_001]);

    //とげ
    D3DXLoadMeshFromX("MODEL/start.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_002],
        NULL,
        &g_nNumMatModel[MODELTYPE_002],
        &g_pMeshModel[MODELTYPE_002]);

    //月
    D3DXLoadMeshFromX("MODEL/moon.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_003],
        NULL,
        &g_nNumMatModel[MODELTYPE_003],
        &g_pMeshModel[MODELTYPE_003]);

    //ピラミッド
    D3DXLoadMeshFromX("MODEL/model002.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_004],
        NULL,
        &g_nNumMatModel[MODELTYPE_004],
        &g_pMeshModel[MODELTYPE_004]);

    for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
    {
        D3DXMATERIAL* d3dxmatrs = (D3DXMATERIAL*)g_pBuffMatModel[MODELTYPE_004]->GetBufferPointer();
        for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[MODELTYPE_004]; nCntMat++)
        {
            if (d3dxmatrs[nCntMat].pTextureFilename != NULL)
            {
                D3DXCreateTextureFromFile(pDevice, d3dxmatrs[nCntMat].pTextureFilename, &g_Model[nCntModel].textureModel[nCntMat]);
            }
        }
    }
}

//=====================================================
// 終了処理
//=====================================================
void UninitModel(void)
{
    for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
    {
        // メッシュの破棄
        if (g_pMeshModel[nCntModel] != NULL)
        {
            g_pMeshModel[nCntModel]->Release();
            g_pMeshModel[nCntModel] = NULL;
        }
        // マテリアルの破棄
        if (g_pBuffMatModel[nCntModel] != NULL)
        {
            g_pBuffMatModel[nCntModel]->Release();
            g_pBuffMatModel[nCntModel] = NULL;
        }

        // テクスチャの破棄
        for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[MODELTYPE_004]; nCntMat++)
        {
            
            if (g_Model[nCntModel].textureModel[nCntMat] != NULL)
            {
                g_Model[nCntModel].textureModel[nCntMat]->Release();
                g_Model[nCntModel].textureModel[nCntMat] = NULL;
            }
        }
    }
}

//=====================================================
// 更新処理
//=====================================================
void UpdateModel(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void DrawModel(void)
{
    int nCntModel;
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;		// 現在のマテリアル保持用
    D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ


    for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
    {
        if (g_Model[nCntModel].bUse == true)
        {
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_Model[nCntModel].mtxWorld);

            // スケールを反映
            D3DXMatrixScaling(&mtxScale,
                g_Model[nCntModel].size.x / g_Model[nCntModel].size.x,
                g_Model[nCntModel].size.y / g_Model[nCntModel].size.y,
                g_Model[nCntModel].size.z / g_Model[nCntModel].size.z);
            D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxScale);
            // 向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_Model[nCntModel].rot.y,
                g_Model[nCntModel].rot.x,
                g_Model[nCntModel].rot.z);
            D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxRot);
            // 位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_Model[nCntModel].pos.x,
                g_Model[nCntModel].pos.y,
                g_Model[nCntModel].pos.z);
            D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxTrans);


            // ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Model[nCntModel].mtxWorld);
            // テクスチャの設定
            pDevice->SetTexture(0, NULL);
            // 現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);
            // マテリアルデータへのポインタを取得
            pMat = (D3DXMATERIAL*)g_pBuffMatModel[g_Model[nCntModel].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[g_Model[nCntModel].type]; nCntMat++)
            {
                // テクスチャの設定
                pDevice->SetTexture(0, g_Model[nCntModel].textureModel[nCntMat]);
                pMat[nCntMat].MatD3D.Ambient.r = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.g = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.b = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.a = 1.0f;
                // マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                // モデルパーツ
                g_pMeshModel[g_Model[nCntModel].type]->DrawSubset(nCntMat);
            }
            // 保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=====================================================
// セットブロック
//=====================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 size, MODELTYPE type)
{

    int nCntModel;				// ローカル変数

    for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
    {
        // ブロックが使われてないとき
        if (g_Model[nCntModel].bUse == false)
        {
            // 対象の配列の要素に、usedやposやsizeをセット

            // ブロックを使用している状態にする

            g_Model[nCntModel].pos = pos;
            g_Model[nCntModel].size = size;
            g_Model[nCntModel].type = type;
            g_Model[nCntModel].bUse = true;
            break;
        }
    }
}