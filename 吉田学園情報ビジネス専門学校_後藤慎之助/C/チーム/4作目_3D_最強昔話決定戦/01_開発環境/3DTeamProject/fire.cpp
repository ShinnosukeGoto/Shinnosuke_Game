//=============================================================================
//
// 最強昔話決定戦の処理 [fire.cpp](炎)
// Author:後藤慎之助
//
//=============================================================================
#include "fire.h"
#include "collision.h"
#include "stage.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define XFILE_NEME "MODEL/fire.x"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshFire = NULL;                     //メッシュ情報へのポインタ
DWORD g_nNumMatFire = 0;                           //マテリアル情報の数
LPD3DXBUFFER g_pBuffMatFire = NULL;                //マテリアル情報のポインタ
LPDIRECT3DTEXTURE9 *g_pTexturehFire = NULL;        //ポリゴンのテクスチャへのポインタ

FIRE g_aFire[MAX_FIRE]; //炎の情報

//=============================================================================
// 炎初期化処理
// Author:後藤慎之助
//=============================================================================
void InitFire(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの読み込み

    //Xファイルの読み込み
    HRESULT result = D3DXLoadMeshFromX
    (
        XFILE_NEME,         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatFire,   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatFire,    //Xファイルに使用されているマテリアルの数
        &g_pMeshFire       //xファイルのメッシュデータ
    );

    // マテリアルとテクスチャの設定
    D3DXMATERIAL    *materials = (D3DXMATERIAL*)g_pBuffMatFire->GetBufferPointer();
    g_pTexturehFire = new LPDIRECT3DTEXTURE9[g_nNumMatFire];

    // テクスチャの読み出し
    for (int nCntFire = 0; nCntFire < (int)g_nNumMatFire; nCntFire++)
    {
        g_pTexturehFire[nCntFire] = NULL;
        if (materials[nCntFire].pTextureFilename != NULL)
        {
            // テクスチャの読み込み
            HRESULT hr = D3DXCreateTextureFromFile(pDevice, materials[nCntFire].pTextureFilename, &g_pTexturehFire[nCntFire]);
            int i = 0;
            i++;
        }
    }
    for (int nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        g_aFire[nCntFire].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aFire[nCntFire].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aFire[nCntFire].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
        g_aFire[nCntFire].nCntTime = TIME_FIRE;
        g_aFire[nCntFire].bUse = false;
    }
}

//=============================================================================
// 炎終了処理
// Author:後藤慎之助
//=============================================================================
void UninitFire(void)
{
    //メッシュの開放
    if (g_pMeshFire != NULL)
    {
        g_pMeshFire->Release();
        g_pMeshFire = NULL;
    }
    //マテリアルの破棄
    if (g_pBuffMatFire != NULL)
    {
        g_pBuffMatFire->Release();
        g_pBuffMatFire = NULL;
    }
    if (g_pTexturehFire != NULL)
    {
        delete g_pTexturehFire;
        g_pTexturehFire = NULL;
    }
}

//=============================================================================
// 炎更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateFire(void)
{
    for (int nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        //使用しているなら
        if (g_aFire[nCntFire].bUse == true)
        {
            //炎が消えるまで数える
            g_aFire[nCntFire].nCntTime--;

            //炎の見栄えのため、高速で回転
            g_aFire[nCntFire].rot += D3DXVECTOR3(1.0f, 1.0f, 1.0f);

            //消火
            if (g_aFire[nCntFire].nCntTime < 0)
            {
                //未使用に
                g_aFire[nCntFire].bUse = false;

                //初期化
                g_aFire[nCntFire].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }
}
//=============================================================================
// 炎描画処理
// Author:後藤慎之助
//=============================================================================
void DrawFire(void)
{

    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの読み込み
    D3DXMATRIX mtxRot, mtxTrans, mtxscale;
    D3DMATERIAL9 matDef;//現在のマテリアル保持用
    D3DXMATERIAL *pMat;//マテリアルデータへのポインタ

    for (int nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        //ワールドマトリックスの初期化
        D3DXMatrixIdentity(&g_aFire[nCntFire].mtxWorld);
        if (g_aFire[nCntFire].bUse)
        {
            //拡縮を反映
            D3DXMatrixScaling(&mtxscale, g_aFire[nCntFire].size.x / g_aFire[nCntFire].size.x, 
                g_aFire[nCntFire].size.y / g_aFire[nCntFire].size.y, g_aFire[nCntFire].size.z / g_aFire[nCntFire].size.z);
            D3DXMatrixMultiply(&g_aFire[nCntFire].mtxWorld, &g_aFire[nCntFire].mtxWorld, &mtxscale);
            //向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFire[nCntFire].rot.y, g_aFire[nCntFire].rot.x, g_aFire[nCntFire].rot.z);
            D3DXMatrixMultiply(&g_aFire[nCntFire].mtxWorld, &g_aFire[nCntFire].mtxWorld, &mtxRot);
            //位置を反映
            D3DXMatrixTranslation(&mtxTrans, g_aFire[nCntFire].pos.x, g_aFire[nCntFire].pos.y, g_aFire[nCntFire].pos.z);
            D3DXMatrixMultiply(&g_aFire[nCntFire].mtxWorld, &g_aFire[nCntFire].mtxWorld, &mtxTrans);

            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_aFire[nCntFire].mtxWorld);

            //現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);

            //マテリアルデータのポインタを取得
            pMat = (D3DXMATERIAL*)g_pBuffMatFire->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatFire; nCntMat++)
            {
                //マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pDevice->SetTexture(0, g_pTexturehFire[nCntMat]);
                //モデルパーツの描画
                g_pMeshFire->DrawSubset(nCntMat);
            }

            //保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// 炎情報の送信
// Author:後藤慎之助
//=============================================================================
FIRE *GetFire(void)
{
    return &g_aFire[0];
}

//=====================================================
// 炎の当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitFire(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntFire;					// ローカル変数
    bool bHit = false;

    for (nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        // 炎が使われたら
        if (g_aFire[nCntFire].bUse == true)
        {
            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_aFire[nCntFire].pos, g_aFire[nCntFire].size) == true)
            {
                bHit = true;    //当たっている
            }
        }
    }

    return bHit;
}

//=====================================================
// 炎の設定
// Author:後藤慎之助
//=====================================================
void SetFire(D3DXVECTOR3 pos, SELECT type)
{

    int nCntFire;				// ローカル変数

    for (nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        // 炎が使われてないとき
        if (g_aFire[nCntFire].bUse == false)
        {
            // 炎を使用している状態にする
            g_aFire[nCntFire].pos = pos;
            g_aFire[nCntFire].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aFire[nCntFire].nCntTime = TIME_FIRE;  //炎の持続時間は固定
            g_aFire[nCntFire].size = D3DXVECTOR3(GRID_SIZE - 10.0f, GRID_SIZE, GRID_SIZE - 10.0f);
            g_aFire[nCntFire].type = type;
            g_aFire[nCntFire].bUse = true;
            break;
        }
    }
}

//=====================================================
// 桃太郎の炎の当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitMomoFire(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntFire;					// ローカル変数
    bool bHit = false;

    for (nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        // 炎が使われたら
        if (g_aFire[nCntFire].bUse == true)
        {
            //桃太郎の炎なら
            if (g_aFire[nCntFire].type == SELECT_MOMO)
            {
                //当たり判定を計算
                if (RectangleCollision(pPos, pPosOld, size, &g_aFire[nCntFire].pos, g_aFire[nCntFire].size) == true)
                {
                    bHit = true;    //当たっている
                }
            }
        }
    }

    return bHit;
}