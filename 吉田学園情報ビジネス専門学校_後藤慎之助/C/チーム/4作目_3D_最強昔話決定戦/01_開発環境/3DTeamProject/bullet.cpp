//=============================================================================
//
// 最強昔話決定戦 [bullet.cpp]の処理 (敵の弾)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "bullet.h"
#include "block.h"
#include "collision.h"
#include "effect.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
#include "camera.h"
#include "sound.h"
#include "boss.h"
#include "shadow.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBullet[BULLETTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//頂点バッファへのポインタ
BULLET g_aBullet[MAX_BULLET];//アイテムの情報

//=============================================================================
// バレット初期化処理
// Author:後藤慎之助
//=============================================================================
void InitBullet(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの取得

    //頂点バッファの生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLET,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffBullet,//変数名が変わると変更が必要
        NULL);

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/particle007.png", &g_pTextureBullet[BULLETTYPE_ENEMY]);   //敵の弾
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/particle007.png", &g_pTextureBullet[BULLETTYPE_BOSS]);    //ボスの弾


    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     
        g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  
        g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBullet[nCntBullet].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBullet[nCntBullet].type = BULLETTYPE_ENEMY;
        g_aBullet[nCntBullet].nLife = 0;
        g_aBullet[nCntBullet].fDamage = 0.0f;
        g_aBullet[nCntBullet].bUse = false;
        g_aBullet[nCntBullet].nShadow = 0;

        //頂点座標の設定（右回りで設定する）
        pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
    g_pVtxBuffBullet->Unlock();

}

//=============================================================================
// バレット終了処理
// Author:後藤慎之助
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
// Author:後藤慎之助
//=============================================================================
void UpdateBullet(void)
{
    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_aBullet[nCntBullet].bUse == true)
        {
            //1F前の位置
            g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

            //移動量を反映させる
            g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

            //影を設定
            SetPositionShadow(g_aBullet[nCntBullet].nShadow,
                D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0, g_aBullet[nCntBullet].pos.z), 0.0f);

            //弾の射程を決める
            g_aBullet[nCntBullet].nLife--;

            //ブロックに当たったら弾のライフをなくす
            if (HitAllTypeBlock(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld,
                g_aBullet[nCntBullet].size) == true)
            {
                g_aBullet[nCntBullet].nLife = 0;

                //ヒットエフェクト
                float fAngle = 0.0f;    //角度
                float fSpeed = 6.0f;    //速さ
                for (int nCnt = 0; nCnt < COUNT_EFFECT_HIT; nCnt++)
                {
                    //0～3.14を出す*(0 or 1)*-1で正負を決める
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*fSpeed, fSpeed, -cosf(fAngle)*fSpeed);
                    SetEffect(g_aBullet[nCntBullet].pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //殺意の波動
                }
            }

            //バレットの軌跡を設定
            SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aBullet[nCntBullet].size.x, EFFECTTYPE_BULLET_TRAIL);

            //ライフがなくなったら未使用に
            if (g_aBullet[nCntBullet].nLife <= 0)
            {
                g_aBullet[nCntBullet].bUse = false;	//使用しない状態にする

                //影の削除
                SHADOW *pShadow = GetShadow();//影の情報を取得
                pShadow[g_aBullet[nCntBullet].nShadow].use = false;

                //初期化
                g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }

    //頂点データアンロック
    g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// バレット描画処理
// Author:後藤慎之助
//=============================================================================
void DrawBullet(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスのポインタを取得
    D3DXMATRIX mtxRot, mtxTrans;//行列計算用のマトリクス

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
    //アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //ライトの影響を受けなくする
    DWORD ambient;
    pDevice->GetRenderState(D3DRS_AMBIENT, &ambient);
    pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);
    pDevice->LightEnable(0, FALSE);

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        //使用しているなら
        if (g_aBullet[nCntBullet].bUse == true)
        {
            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);
            //回転の逆行列を生成
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxRot);
            g_aBullet[nCntBullet].mtxWorld._41 = 0;//移動はさせたくないので0を代入
            g_aBullet[nCntBullet].mtxWorld._42 = 0;//移動はさせたくないので0を代入
            g_aBullet[nCntBullet].mtxWorld._43 = 0;//移動はさせたくないので0を代入

                                                  //位置を反映
            D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
            D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);
            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);
            //テクスチャの設定
            pDevice->SetTexture(0, g_pTextureBullet[g_aBullet[nCntBullet].type]);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, NUM_POLYGON);

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
// バレットの設定
// Author:後藤慎之助
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, int nLife, float fDamage, BULLETTYPE type)
{
    VERTEX_3D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_aBullet[nCntBullet].bUse == false)
        {
            g_aBullet[nCntBullet].bUse = true;
            g_aBullet[nCntBullet].pos = pos;
            g_aBullet[nCntBullet].size = size;
            g_aBullet[nCntBullet].move = move;
            g_aBullet[nCntBullet].nLife = nLife;
            g_aBullet[nCntBullet].fDamage = fDamage;
            g_aBullet[nCntBullet].type = type;

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_aBullet[nCntBullet].size.x / 2), (g_aBullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_aBullet[nCntBullet].size.x / 2), (g_aBullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_aBullet[nCntBullet].size.x / 2), -(g_aBullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_aBullet[nCntBullet].size.x / 2), -(g_aBullet[nCntBullet].size.y / 2), 0.0f);

            //弾の影の設定
            g_aBullet[nCntBullet].nShadow = SetShadow(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_aBullet[nCntBullet].size.x, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));

            break;
        }
        pVtx += 4;
    }

    //頂点データアンロック
    g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// バレットの取得
// Author:後藤慎之助
//=============================================================================
BULLET*GetBullet(void)
{
    return &g_aBullet[0];
}

//=====================================================
// 弾の衝突判定の設定
// Author:後藤慎之助
//=====================================================
bool CollisionBullet(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bCollision = false;  //衝突したかどうか

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        // 弾が使われたら
        if (g_aBullet[nCntBullet].bUse == true)
        {

            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size) == true)
            {
                //エネミーの弾(当たったら未使用に)
                if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
                {
                    //ノックバック
                    PLAYER *pPlayer = GetPlayer();
                    if (pPlayer->nCntInvincibleFrame == 0)  //無敵でないなら
                    {
                        float fKnockback = VALUE_KNOCKBACK;
                        pPlayer->move.x = pPlayer->pos.x - g_aBullet[nCntBullet].posOld.x;
                        pPlayer->move.z = pPlayer->pos.z - g_aBullet[nCntBullet].posOld.z;
                        D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //ベクトルを正規化
                        pPlayer->move.x = pPlayer->move.x*fKnockback;
                        pPlayer->move.z = pPlayer->move.z*fKnockback;
                    }

                    //自機にダメージ
                    HitPlayer(g_aBullet[nCntBullet].fDamage);

                    bCollision = true;    //衝突
                    g_aBullet[nCntBullet].nLife = 0;  //当たったらライフを0に

                    //ヒットエフェクト
                    float fAngle = 0.0f;    //角度
                    float fSpeed = 6.0f;    //速さ
                    for (int nCnt = 0; nCnt < COUNT_EFFECT_HIT; nCnt++)
                    {
                        //0～3.14を出す*(0 or 1)*-1で正負を決める
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*fSpeed, fSpeed, -cosf(fAngle)*fSpeed);
                        SetEffect(g_aBullet[nCntBullet].pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //殺意の波動
                    }
                }

                //ボスの弾(当たっても未使用にならない)
                else if (g_aBullet[nCntBullet].type == BULLETTYPE_BOSS)
                {

                    //ノックバックとヒットエフェクト
                    PLAYER *pPlayer = GetPlayer();
                    if (pPlayer->nCntInvincibleFrame == 0)  //無敵でないなら
                    {
                        float fKnockback = VALUE_KNOCKBACK;
                        pPlayer->move.x = pPlayer->pos.x - g_aBullet[nCntBullet].posOld.x;
                        pPlayer->move.z = pPlayer->pos.z - g_aBullet[nCntBullet].posOld.z;
                        D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //ベクトルを正規化
                        pPlayer->move.x = pPlayer->move.x*fKnockback;
                        pPlayer->move.z = pPlayer->move.z*fKnockback;

                        float fAngle = 0.0f;    //角度
                        float fSpeed = 6.0f;    //速さ
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_HIT; nCnt++)
                        {
                            //0～3.14を出す*(0 or 1)*-1で正負を決める
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*fSpeed, fSpeed, -cosf(fAngle)*fSpeed);
                            SetEffect(g_aBullet[nCntBullet].pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //殺意の波動
                        }
                    }

                    //自機にダメージ
                    HitPlayer(g_aBullet[nCntBullet].fDamage);

                    bCollision = true;    //衝突
                }
            }
        }
    }
    return bCollision;    //衝突したかどうかを返す
}

//=====================================================
// 弾の当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitBullet(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;  //衝突したかどうか

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        // 弾が使われたら
        if (g_aBullet[nCntBullet].bUse == true)
        {

            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].size) == true)
            {
                bHit = true;    //衝突

                //ヒットエフェクト
                float fAngle = 0.0f;    //角度
                float fSpeed = 6.0f;    //速さ
                for (int nCnt = 0; nCnt < COUNT_EFFECT_HIT; nCnt++)
                {
                    //0～3.14を出す*(0 or 1)*-1で正負を決める
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle)*fSpeed, fSpeed, -cosf(fAngle)*fSpeed);
                    SetEffect(g_aBullet[nCntBullet].pos, move, EFFECT_SIZE_EVIL, EFFECTTYPE_EVIL); //殺意の波動
                }

                //エネミーの弾なら
                if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
                {
                    g_aBullet[nCntBullet].nLife = 0;  //当たったらライフを0に
                }
            }
        }
    }
    return bHit;    //衝突したかどうかを返す
}