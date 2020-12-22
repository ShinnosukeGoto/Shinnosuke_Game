//=====================================================
//
// PLASTIC TROOPERSの処理 [block.cpp](ビルなどのブロック)
// Author:後藤慎之助
//
//=====================================================

//=====================================================
// インクルードファイル
//=====================================================
#include "block.h"
#include "camera.h"
#include "player.h"
#include "collision.h"
#include "enemy.h"
#include "boss.h"
#include "effect2.h"

//=====================================================
// グローバル変数
//=====================================================
BLOCK g_Block[MAX_BLOCK];

// 頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock[BLOCKTYPE_MAX] = {};

// メッシュ情報へのポインタ
LPD3DXMESH g_pMeshBlock[BLOCKTYPE_MAX] = {};
// マテリアル情報へのポインタ
LPD3DXBUFFER g_pBuffMatBlock[BLOCKTYPE_MAX] = {};
// マテリアル情報の数
DWORD g_nNumMatBlock[BLOCKTYPE_MAX] = {};


//=====================================================
// 初期化処理
//=====================================================
void InitBlock(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ


    int nCntBlock;


    // 配列初期化
    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Block[nCntBlock].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Block[nCntBlock].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Block[nCntBlock].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Block[nCntBlock].type = BLOCKTYPE_000;
        g_Block[nCntBlock].bUse = false;

    }

    // Xファイルの読み込み
    D3DXLoadMeshFromX("MODEL/block000.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatBlock[BLOCKTYPE_000],
        NULL,
        &g_nNumMatBlock[BLOCKTYPE_000],
        &g_pMeshBlock[BLOCKTYPE_000]);

    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        D3DXMATERIAL* d3dxmatrs = (D3DXMATERIAL*)g_pBuffMatBlock[BLOCKTYPE_000]->GetBufferPointer();
        for (int nCntMat = 0; nCntMat < (int)g_nNumMatBlock[BLOCKTYPE_000]; nCntMat++)
        {
            if (d3dxmatrs[nCntMat].pTextureFilename != NULL)
            {
                D3DXCreateTextureFromFile(pDevice, d3dxmatrs[nCntMat].pTextureFilename, &g_Block[nCntBlock].textureModel[nCntMat]);
            }
        }
    }
}

//=====================================================
// 終了処理
//=====================================================
void UninitBlock(void)
{
    // メッシュの破棄
    for (int nCntBlock = 0; nCntBlock < BLOCKTYPE_MAX; nCntBlock++)
    {
        if (g_pMeshBlock[nCntBlock] != NULL)
        {
            g_pMeshBlock[nCntBlock]->Release();
            g_pMeshBlock[nCntBlock] = NULL;
        }
        // マテリアルの破棄
        if (g_pBuffMatBlock[nCntBlock] != NULL)
        {
            g_pBuffMatBlock[nCntBlock]->Release();
            g_pBuffMatBlock[nCntBlock] = NULL;
        }
        // テクスチャの破棄
        for (int nCntMat = 0; nCntMat < (int)g_nNumMatBlock[BLOCKTYPE_000]; nCntMat++)
        {

            if (g_Block[nCntBlock].textureModel[nCntMat] != NULL)
            {
                g_Block[nCntBlock].textureModel[nCntMat]->Release();
                g_Block[nCntBlock].textureModel[nCntMat] = NULL;
            }
        }
    }
}

//=====================================================
// 更新処理
//=====================================================
void UpdateBlock(void)
{
    for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        if (g_Block[nCntBlock].bUse == true)
        {
            g_Block[nCntBlock].posOld = g_Block[nCntBlock].pos;

            //ボスとブロックが当たったら
            if (CollisionBoss(&g_Block[nCntBlock].pos, &g_Block[nCntBlock].posOld,
                g_Block[nCntBlock].size) == true)
            {
                //爆発エフェクト
                float fSpeedX = 0.0f;   //速さX
                float fSpeedY = 0.0f;   //速さY
                float fSpeedZ = 0.0f;   //速さZ
                int nPlusMinusX = 0;
                int nPlusMinusZ = 0;
                for (int nCnt = 0; nCnt < 200; nCnt++)
                {
                    fSpeedX = float(rand() % 800 + 300) / 100.0f;
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    fSpeedZ = float(rand() % 800 + 300) / 100.0f;
                    nPlusMinusX = int(rand() % 2 + 1);
                    nPlusMinusZ = int(rand() % 2 + 1);
                    if (nPlusMinusX == 1)
                    {
                        nPlusMinusX = 1;
                    }
                    else
                    {
                        nPlusMinusX = -1;
                    }

                    if (nPlusMinusZ == 1)
                    {
                        nPlusMinusZ = 1;
                    }
                    else
                    {
                        nPlusMinusZ = -1;
                    }

                    D3DXVECTOR3 move = D3DXVECTOR3(fSpeedX*nPlusMinusX, fSpeedY, fSpeedZ*nPlusMinusZ);
                    SetEffect2(g_Block[nCntBlock].pos, move, EFFECT2_SIZE_EXPLOSION, EFFECT2_INIT_COLOR_EXPLOSION, EFFECT2TYPE_EXPLOSION); //爆発エフェクト
                }

                g_Block[nCntBlock].bUse = false;
            }

        }
    }
}

//=====================================================
// 描画処理
//=====================================================
void DrawBlock(void)
{
    int nCntBlock;
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスへのポインタ
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;		// 現在のマテリアル保持用
    D3DXMATERIAL* pMat;			// マテリアルデータへのポインタ


    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        if (g_Block[nCntBlock].bUse == true)
        {
            // ワールドマトリックスの初期化
            D3DXMatrixIdentity(&g_Block[nCntBlock].mtxWorld);

            // スケールを反映
            D3DXMatrixScaling(&mtxScale,
                g_Block[nCntBlock].size.x / g_Block[nCntBlock].size.x,
                g_Block[nCntBlock].size.y / g_Block[nCntBlock].size.y,
                g_Block[nCntBlock].size.z / g_Block[nCntBlock].size.z);
            D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorld, &g_Block[nCntBlock].mtxWorld, &mtxScale);
            // 向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_Block[nCntBlock].rot.y,
                g_Block[nCntBlock].rot.x,
                g_Block[nCntBlock].rot.z);
            D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorld, &g_Block[nCntBlock].mtxWorld, &mtxRot);
            // 位置を反映
            D3DXMatrixTranslation(&mtxTrans,
                g_Block[nCntBlock].pos.x,
                g_Block[nCntBlock].pos.y,
                g_Block[nCntBlock].pos.z);
            D3DXMatrixMultiply(&g_Block[nCntBlock].mtxWorld, &g_Block[nCntBlock].mtxWorld, &mtxTrans);


            // ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Block[nCntBlock].mtxWorld);
            // テクスチャの設定
            pDevice->SetTexture(0, NULL);
            // 現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);
            // マテリアルデータへのポインタを取得
            pMat = (D3DXMATERIAL*)g_pBuffMatBlock[g_Block[nCntBlock].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatBlock[g_Block[nCntBlock].type]; nCntMat++)
            {
                // テクスチャの設定
                pDevice->SetTexture(0, g_Block[nCntBlock].textureModel[nCntMat]);
                pMat[nCntMat].MatD3D.Ambient.r = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.g = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.b = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.a = 1.0f;
                // マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                // モデルパーツ
                g_pMeshBlock[g_Block[nCntBlock].type]->DrawSubset(nCntMat);
            }
            // 保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=====================================================
// セットブロック
//=====================================================
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 size, BLOCKTYPE type)
{

    int nCntBlock;				// ローカル変数

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われてないとき
        if (g_Block[nCntBlock].bUse == false)
        {
            // 対象の配列の要素に、usedやposやsizeをセット

            // ブロックを使用している状態にする

            g_Block[nCntBlock].pos = pos;
            g_Block[nCntBlock].size = size;
            g_Block[nCntBlock].type = type;
            g_Block[nCntBlock].bUse = true;
            break;
        }
    }
}

//=====================================================
// 着地判定の設定
//=====================================================
bool LandBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntBlock;					// ローカル変数
    PLAYER *pPlayer;
    bool isGround = false;

    pPlayer = GetPlayer();
    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われたら
        if (g_Block[nCntBlock].bUse == true)
        {
            //プレイヤーの位置
            D3DXVECTOR3 box1Max = D3DXVECTOR3(size.x / 2, size.y / 2, size.z / 2) + *pPos;
            D3DXVECTOR3 box1Min = D3DXVECTOR3(-size.x / 2, -size.y / 2, -size.z / 2) + *pPos;

            //ブロックの位置
            D3DXVECTOR3 box2Max = D3DXVECTOR3(g_Block[nCntBlock].size.x / 2, g_Block[nCntBlock].size.y / 2, g_Block[nCntBlock].size.z / 2) + g_Block[nCntBlock].pos;
            D3DXVECTOR3 box2Min = D3DXVECTOR3(-g_Block[nCntBlock].size.x / 2, -g_Block[nCntBlock].size.y / 2, -g_Block[nCntBlock].size.z / 2) + g_Block[nCntBlock].pos;

            // ブロックとの当たり判定
            if ((box1Max.y > box2Min.y) &&
                (box1Min.y < box2Max.y) &&
                (box1Max.x > box2Min.x) &&
                (box1Min.x < box2Max.x) &&
                (box1Min.z < box2Max.z) &&
                (box1Max.z > box2Min.z))
            {

                if (box1Max.y + size.y > box2Min.y &&
                    pPlayer->posOld.y + (size.y / 2) <= box2Min.y)
                {
                    // ブロックに下から当たった時
                    pPlayer->pos.y = box2Min.y - (size.y / 2);		// プレイヤーがブロックにめり込まないようにする
                    pPlayer->move.y = 0;
                }
                else if (box1Min.x < box2Max.x &&
                    pPlayer->posOld.x - (size.x / 2) >= box2Max.x)
                {
                    // ブロックに左から当たったとき
                    pPlayer->pos.x = box2Max.x + (size.x / 2);	// プレイヤーを押し戻す

                }
                else if (box1Max.x >  box2Min.x &&
                    pPlayer->posOld.x + (size.x / 2) <= box2Min.x)
                {
                    // ブロックに右から当たった時
                    pPlayer->pos.x = box2Min.x - (size.x / 2);
                }
                else if ((box1Min.y + size.y / 2) < box2Max.y &&
                    pPlayer->posOld.y - (0.0f) >= box2Max.y)
                {
                    // ブロックに上から当たった時
                    pPlayer->pos.y = box2Max.y + (0.0f);
                    isGround = true;			// 着地した
                }
                else if (box1Min.z < box2Max.z &&
                    pPlayer->posOld.z - (size.z / 2) >= box2Max.z)
                {
                    // ブロックに奥から当たった時
                    pPlayer->pos.z = box2Max.z + (size.z / 2);

                }
                else if (box1Max.z > box2Min.z &&
                    pPlayer->posOld.z + (size.z / 2) <= box2Min.z)
                {
                    // ブロックに奥から当たった時
                    pPlayer->pos.z = box2Min.z - (size.z / 2);

                }
            }
        }
    }
    return isGround;
}

//=====================================================
// 敵のブロックに対する判定
//=====================================================
bool EnemyHitBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntBlock;					// ローカル変数
    bool isGround = false;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // ブロックが使われたら
        if (g_Block[nCntBlock].bUse == true)
        {
            //当たり判定を計算
            BoxCollision(pPos, pPosOld, size, &g_Block[nCntBlock].pos, g_Block[nCntBlock].size);
        }
    }
    return isGround;
}

//=====================================================
// ブロックの取得
//=====================================================
BLOCK*GetBlock(void)
{
    return &g_Block[0];		// 値を返す(渡す)
}

//=====================================================
// 当たり判定の設定
//=====================================================
bool HitBlock(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntBlock;					// ローカル変数
    bool bHit = false;

    for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
    {
        // アイテムが使われたら
        if (g_Block[nCntBlock].bUse == true)
        {
            if (RectangleCollision(pPos, pPosOld, size, &g_Block[nCntBlock].pos, g_Block[nCntBlock].size) == true)
            {
                bHit = true;
            }
        }
    }

    return bHit;
}