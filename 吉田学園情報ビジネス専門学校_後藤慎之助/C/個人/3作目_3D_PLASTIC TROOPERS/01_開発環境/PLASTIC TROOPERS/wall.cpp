//=============================================================================
//
// PLASTIC TROOPERSの処理 [wall.h]（黄色透明の壁）
// Author:後藤慎之助
//
//=============================================================================
#include "wall.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureWall[WALLTYPE_MAX] = {};		//ポリゴンのテクスチャへのポインタ
WALL g_Wall[MAX_WALL];//壁の情報

int g_nCounterAnimWall;		//アニメーションカウンター
int g_nPatternAnimWall;		//アニメーションパターン
//=============================================================================
//壁の初期化処理
//=============================================================================
void InitWall(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

    //グローバル変数の初期化
    g_nCounterAnimWall = 0;
    g_nPatternAnimWall = 0;

    //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D)*NUM_VERTEX*MAX_WALL,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffWall,//変数名が変わると変更が必要
        NULL);

    D3DXCreateTextureFromFile(pDevice, "TEXTURE/wall000.png", &g_pTextureWall[WALLTYPE_YELLOW]);//テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/wall001.png", &g_pTextureWall[WALLTYPE_RED1]);//テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/wall001.png", &g_pTextureWall[WALLTYPE_RED2]);//テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/wall002.png", &g_pTextureWall[WALLTYPE_RED3]);//テクスチャの読み込み

    VERTEX_3D *pVtx;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
    {
        g_Wall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Wall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Wall[nCntWall].type = WALLTYPE_YELLOW;
        g_Wall[nCntWall].bUse = false;

        //頂点座標の初期化（右回りで設定する）
        pVtx[0].pos = D3DXVECTOR3(0.0f - WALL_SIZE, 0.0f, 0.0f + WALL_SIZE_Y);
        pVtx[1].pos = D3DXVECTOR3(0.0f + WALL_SIZE, 0.0f, 0.0f + WALL_SIZE_Y);
        pVtx[2].pos = D3DXVECTOR3(0.0f - WALL_SIZE, 0.0f, 0.0f - WALL_SIZE_Y);
        pVtx[3].pos = D3DXVECTOR3(0.0f + WALL_SIZE, 0.0f, 0.0f - WALL_SIZE_Y);

        //法線ベクトル
        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

        //頂点カラーの設定
        pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
        pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }
    //頂点データアンロック
    g_pVtxBuffWall->Unlock();
}
//=============================================================================
//壁の更新処理
//=============================================================================
void UpdateWall(void)
{
    VERTEX_3D *pVtx = NULL;	//頂点情報へのポインタ

    g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

                                                    //下に流れるアニメーション
    g_nCounterAnimWall++;					//カウンタ加算
    if (g_nCounterAnimWall == 2)			//速さ
    {
        g_nCounterAnimWall = 0;
        g_nPatternAnimWall++;
        if (g_nPatternAnimWall == 1000)	//枚数
        {
            g_nPatternAnimWall = 0;
        }
    }

    //テクスチャ座標を更新
    pVtx[0].tex = D3DXVECTOR2(0.0f, g_nPatternAnimWall *0.004f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, g_nPatternAnimWall *0.004f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, g_nPatternAnimWall *0.004f - 0.996f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, g_nPatternAnimWall *0.004f - 0.996f);

    //頂点データをアンロックする
    g_pVtxBuffWall->Unlock();
}
//=============================================================================
//壁の設定
//=============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, WALLTYPE type)
{
    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
    {
        if (g_Wall[nCntWall].bUse == false)
        {
            g_Wall[nCntWall].bUse = true;
            g_Wall[nCntWall].pos = pos;
            g_Wall[nCntWall].rot = rot;
            g_Wall[nCntWall].type = type;
            break;
        }
    }
}
//=============================================================================
//壁の描画処理
//=============================================================================
void DrawWall(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
    //アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //ライトの影響を受けなくする
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
    {
        if (g_Wall[nCntWall].bUse == true)
        {
            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_Wall[nCntWall].mtxWorld);
            //向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCntWall].rot.y, g_Wall[nCntWall].rot.x, g_Wall[nCntWall].rot.z);
            D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxRot);
            //位置を反映
            D3DXMatrixTranslation(&mtxTrans, g_Wall[nCntWall].pos.x, g_Wall[nCntWall].pos.y, g_Wall[nCntWall].pos.z);
            D3DXMatrixMultiply(&g_Wall[nCntWall].mtxWorld, &g_Wall[nCntWall].mtxWorld, &mtxTrans);
            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCntWall].mtxWorld);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);
            //テクスチャの設定
            pDevice->SetTexture(0, g_pTextureWall[g_Wall[nCntWall].type]);
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
        }
    }

    //アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //ライトの影響を受けるようにする
    pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
    pDevice->LightEnable(0, TRUE);

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}
//=============================================================================
//壁の終了処理
//=============================================================================
void UninitWall(void)
{
    //頂点バッファの開放
    if (g_pVtxBuffWall != NULL)
    {
        g_pVtxBuffWall->Release();
        g_pVtxBuffWall = NULL;
    }
    //テクスチャの開放
    for (int nCntWall = 0; nCntWall < WALLTYPE_MAX; nCntWall++)
    {
        if (g_pTextureWall[nCntWall] != NULL)
        {
            g_pTextureWall[nCntWall]->Release();
            g_pTextureWall[nCntWall] = NULL;
        }
    }
}

//=============================================================================
//壁の取得
//=============================================================================
WALL*GetWall(void)
{
    return &g_Wall[0];
}