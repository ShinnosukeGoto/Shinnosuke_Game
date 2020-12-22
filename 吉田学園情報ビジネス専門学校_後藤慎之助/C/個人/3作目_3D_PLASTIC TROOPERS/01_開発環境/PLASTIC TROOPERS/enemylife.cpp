////=============================================================================
////
//// 体力処理 [enemylife.cpp]
//// Author : 青野　慎平
////
////=============================================================================
//
//#include "main.h"
//#include "enemylife.h"
//#include "player.h"
//#include "enemy.h"
//
////*****************************************************************************
//// グローバル変数
////*****************************************************************************
//LPDIRECT3DTEXTURE9 g_pTextureEnemyLife = NULL;
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemyLife = NULL;
//
//ENEMYLIFE g_aEnemyLife[MAX_ENEMYLIFE];
//
//D3DXCOLOR g_colorEnemyLife[MAX_ENEMYLIFE];
//
////=============================================================================
//// 初期化処理
////=============================================================================
//void InitEnemyLife(void)
//{
//    LPDIRECT3DDEVICE9 pDevice = GetDevice();
//    VERTEX_3D *pVtx;//頂点情報のポインタ
//                    //テクスチャの読み込み
//    int nCntEnemyLife;
//
//    D3DXCreateTextureFromFile(pDevice,
//        "TEXTURE/bar000.png",//ファイルの名前
//        &g_pTextureEnemyLife);
//    //頂点バッファ生成
//    pDevice->CreateVertexBuffer(
//        sizeof(VERTEX_3D)*NUM_VERTEX*MAX_ENEMYLIFE,//バッファサイズ
//        D3DUSAGE_WRITEONLY,//(固定)
//        FVF_VERTEX_3D,//(固定)
//        D3DPOOL_MANAGED,//(固定)
//        &g_pVtxBuffEnemyLife,//変数名が変わると変更が必要
//        NULL);//頂点情報へのポインタ
//
//              //頂点データの範囲をロックし、頂点バッファへのポインタを取得
//    g_pVtxBuffEnemyLife->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えない
//
//    for (nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        g_aEnemyLife[nCntEnemyLife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//        g_aEnemyLife[nCntEnemyLife].fMaxLife = 0.0f;
//        g_aEnemyLife[nCntEnemyLife].bUse = false;
//        g_aEnemyLife[nCntEnemyLife].nEnemyIndex = -1;
//
//        //頂点座標の設定(右回りで設定する)
//        pVtx[0].pos = D3DXVECTOR3((-ENEMYLIFE_SIZE_X / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//        pVtx[1].pos = D3DXVECTOR3((ENEMYLIFE_SIZE_X / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//        pVtx[2].pos = D3DXVECTOR3((-ENEMYLIFE_SIZE_X / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//        pVtx[3].pos = D3DXVECTOR3((ENEMYLIFE_SIZE_X / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//        //法線の設定
//        pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//        pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//        pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//        pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
//
//        //テクスチャの設定
//        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//
//        //頂点カラーの設定
//        pVtx[0].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
//        pVtx[1].col = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
//        pVtx[2].col = D3DXCOLOR(0.3f, 0.0f, 0.7f, 1.0f);
//        pVtx[3].col = D3DXCOLOR(0.3f, 0.0f, 0.7f, 1.0f);
//
//        pVtx += 4;
//    }
//    //頂点データをアンロックする
//    g_pVtxBuffEnemyLife->Unlock();
//}
////=============================================================================
//// 終了処理
////=============================================================================
//void UninitEnemyLife(void)
//{
//    //テクスチャの解放
//    if (g_pTextureEnemyLife != NULL)
//    {
//        g_pTextureEnemyLife->Release();
//        g_pTextureEnemyLife = NULL;
//    }
//    //
//    if (g_pVtxBuffEnemyLife != NULL)
//    {
//        g_pVtxBuffEnemyLife->Release();
//        g_pVtxBuffEnemyLife = NULL;
//    }
//}
////=============================================================================
//// 更新処理
////=============================================================================
//void UpdateEnemyLife(void)
//{
//    VERTEX_3D*pVtx;
//    //敵を所得
//    ENEMY *pEnemy;
//
//    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
//    g_pVtxBuffEnemyLife->Lock(0, 0, (void**)&pVtx, 0);
//
//    for (int nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        if (g_aEnemyLife[nCntEnemyLife].bUse == true)
//        {
//            for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
//            {
//                pEnemy = GetEnemy();
//
//                if (g_aEnemyLife[nCntEnemyLife].nEnemyIndex == nCntEnemy)
//                {//取り入れたインデックス数とnCntEnemyが一致したら
//                    if (pEnemy->bUse == false)
//                    {//ライフバーがいらない状態だったら
//
//                        g_aEnemyLife[nCntEnemyLife].bUse = false;
//                    }
//                    else
//                    {//ライフバーを使用する
//
//                        if (pEnemy->type == ENEMYTYPE_000)
//                        {
//                            g_aEnemyLife[nCntEnemyLife].pos = D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + ENEMYLIFE_POS_Y_000, pEnemy->pos.z);
//
//                            //位置更新(敵の最大の体力を引いているのは位置調整のため)
//                            pVtx[0].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[1].pos = D3DXVECTOR3(((float)pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[2].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[3].pos = D3DXVECTOR3((pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//                        }
//                        else if (pEnemy->type == ENEMYTYPE_001)
//                        {
//                            g_aEnemyLife[nCntEnemyLife].pos = D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + ENEMYLIFE_POS_Y_001, pEnemy->pos.z);
//
//                            //位置更新(敵の最大の体力を引いているのは位置調整のため)
//                            pVtx[0].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[1].pos = D3DXVECTOR3((pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[2].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[3].pos = D3DXVECTOR3((pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//                        }
//                        else
//                        {
//                            g_aEnemyLife[nCntEnemyLife].pos = D3DXVECTOR3(pEnemy->pos.x, pEnemy->pos.y + BOSSLIFE_POS_Y_001, pEnemy->pos.z);
//
//                            //位置更新(敵の最大の体力を引いているのは位置調整のため)
//                            pVtx[0].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[1].pos = D3DXVECTOR3((pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[2].pos = D3DXVECTOR3(-((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2), (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//
//                            pVtx[3].pos = D3DXVECTOR3(((float)pEnemy->fLife * ENEMYLIFE_SIZE_X / 2) - ((float)g_aEnemyLife[nCntEnemyLife].fMaxLife / 2),
//                                (-ENEMYLIFE_SIZE_Y / 2), 0.0f);
//                        }
//                    }
//
//                    break;
//                }
//            }
//
//
//        }
//        pVtx += 4;
//    }
//    //頂点データをアンロックする
//    g_pVtxBuffEnemyLife->Unlock();
//}
//
////=============================================================================
//// 描画処理
////=============================================================================
//void DrawEnemyLife(void)
//{
//    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得
//    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス
//
//                                //アルファテストを有効化
//    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
//    //アルファテスト基準値の設定
//    pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
//    //アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
//    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
//
//    //ライトの影響を受けなくする
//    DWORD ambient;
//    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
//    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
//    pDevice->LightEnable(0, FALSE);
//
//    for (int nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        if (g_aEnemyLife[nCntEnemyLife].bUse == true)
//        {
//            //ワールドマトリクスの初期化
//            D3DXMatrixIdentity(&g_aEnemyLife[nCntEnemyLife].mtxWorld);
//            //回転の逆行列を生成
//            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
//            D3DXMatrixInverse(&g_aEnemyLife[nCntEnemyLife].mtxWorld, NULL, &mtxRot);
//            g_aEnemyLife[nCntEnemyLife].mtxWorld._41 = 0;//移動はさせたくないので0を代入
//            g_aEnemyLife[nCntEnemyLife].mtxWorld._42 = 0;//移動はさせたくないので0を代入
//            g_aEnemyLife[nCntEnemyLife].mtxWorld._43 = 0;//移動はさせたくないので0を代入
//
//                                                  //位置を反映
//            D3DXMatrixTranslation(&mtxTrans, g_aEnemyLife[nCntEnemyLife].pos.x, g_aEnemyLife[nCntEnemyLife].pos.y, g_aEnemyLife[nCntEnemyLife].pos.z);
//            D3DXMatrixMultiply(&g_aEnemyLife[nCntEnemyLife].mtxWorld, &g_aEnemyLife[nCntEnemyLife].mtxWorld, &mtxTrans);
//            //ワールドマトリックスの設定
//            pDevice->SetTransform(D3DTS_WORLD, &g_aEnemyLife[nCntEnemyLife].mtxWorld);
//            //テクスチャの設定
//            pDevice->SetTexture(0, g_pTextureEnemyLife);
//            //頂点バッファをデバイスのデータストリームに設定
//            pDevice->SetStreamSource(0, g_pVtxBuffEnemyLife, 0, sizeof(VERTEX_3D));
//            //頂点フォーマットの設定
//            pDevice->SetFVF(FVF_VERTEX_3D);
//            //ポリゴンの描画
//            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemyLife * 4, NUM_POLYGON);
//
//        }
//    }
//
//    //アルファテストを無効化
//    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
//
//    //ライトの影響を受けるようにする
//    pDevice->SetRenderState(D3DRS_AMBIENT, ambient);
//    pDevice->LightEnable(0, TRUE);
//
//    //テクスチャの設定の解除
//    pDevice->SetTexture(0, NULL);
//
//}
////=============================================================================
//// 体力の設定
////=============================================================================
//void SetEnemyLife(float fMaxLife, int nCntEnemy)
//{
//    VERTEX_3D*pVtx;
//    int nCntEnemyLife;
//    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
//    g_pVtxBuffEnemyLife->Lock(0, 0, (void**)&pVtx, 0);
//
//    for (nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        if (g_aEnemyLife[nCntEnemyLife].bUse == false)//爆発が未使用の場合
//        {
//            g_aEnemyLife[nCntEnemyLife].bUse = true;//使用している状態にする
//            g_aEnemyLife[nCntEnemyLife].fMaxLife = fMaxLife;
//            g_aEnemyLife[nCntEnemyLife].nEnemyIndex = nCntEnemy;
//            break;
//        }
//        pVtx += 4;
//    }
//    //頂点データをアンロックする
//    g_pVtxBuffEnemyLife->Unlock();
//}
//
//void UnsetEnemyLife(int nCntEnemy)
//{
//    for (int nCntEnemyLife = 0; nCntEnemyLife < MAX_ENEMYLIFE; nCntEnemyLife++)
//    {
//        if (g_aEnemyLife[nCntEnemyLife].bUse == true &&
//            g_aEnemyLife[nCntEnemyLife].nEnemyIndex == nCntEnemy)
//        {
//            g_aEnemyLife[nCntEnemyLife].bUse = false;
//
//            break;
//        }
//    }
//}