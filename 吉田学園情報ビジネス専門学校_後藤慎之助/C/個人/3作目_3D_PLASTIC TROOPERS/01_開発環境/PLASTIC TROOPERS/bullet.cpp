//=============================================================================
//
// PLASTIC TROOPERS [bullet.cpp]の処理 (弾)
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
#include "effect2.h"
#include "camera.h"
#include "sound.h"
#include "boss.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTextureBullet[BULLETTYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;//頂点バッファへのポインタ
BULLET g_Bullet[MAX_BULLET];//アイテムの情報

//=============================================================================
//バレット初期化処理
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
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/particle006.png", &g_pTextureBullet[0]);   //プレイヤーの弾
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/particle007.png", &g_pTextureBullet[1]);   //敵の弾
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/particle007.png", &g_pTextureBullet[2]);   //敵の弾


    VERTEX_3D *pVtx;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        g_Bullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//ポリゴンの位置
        g_Bullet[nCntBullet].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Bullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//ポリゴンの移動量
        g_Bullet[nCntBullet].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//ポリゴンの大きさ
        g_Bullet[nCntBullet].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//ポリゴンの色
        g_Bullet[nCntBullet].type = BULLETTYPE_PLAYER;
        g_Bullet[nCntBullet].nLife = 0;
        g_Bullet[nCntBullet].nCntRigid = 0;
        g_Bullet[nCntBullet].fDamage = 0.0f;
        g_Bullet[nCntBullet].bUse = false;

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
//バレット更新処理
//=============================================================================
void UpdateBullet(void)
{
    VERTEX_3D *pVtx;//頂点情報のポインタ

                    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_Bullet[nCntBullet].bUse == true)
        {
            //1F前の位置
            g_Bullet[nCntBullet].posOld = g_Bullet[nCntBullet].pos;

            //移動量を反映させる
            g_Bullet[nCntBullet].pos += g_Bullet[nCntBullet].move;

            //弾の射程を決める
            g_Bullet[nCntBullet].nLife--;

            //ブロックに当たったら弾のライフをなくす
            if (HitBlock(&g_Bullet[nCntBullet].pos, &g_Bullet[nCntBullet].posOld,
                BULLET_SIZE_PLAYER) == true)
            {
                //ヒットエフェクト
                float fAngle = 0.0f;    //角度
                float fSpeed = 5.0f;   //速さ
                CAMERA*pCamera = GetCamera();
                for (int nCnt = 0; nCnt < 50; nCnt++)
                {
                    //0～3.14を出す*(0 or 1)*-1で正負を決める
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                    SetEffect2(g_Bullet[nCntBullet].pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                }
                g_Bullet[nCntBullet].nLife = 0;
            }

            //ボスの弾の火柱
            if (g_Bullet[nCntBullet].type == BULLETTYPE_BOSS)
            {
                if (g_Bullet[nCntBullet].pos.y < 0)
                {
                    float fAngle = 0.0f;    //角度
                    float fSpeedY = 0.0f;   //速さ
                    float fRadius = 0.0f; //エフェクト発生半径
                    for (int nCnt = 0; nCnt < 300; nCnt++)
                    {
                        fSpeedY = float(rand() % 800 + 300) / 100.0f;
                        //0～3.14を出す*(0 or 1)*-1で正負を決める
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        fRadius = float(rand() % 25000 + 0) / 100.0f;
                        D3DXVECTOR3 pos = D3DXVECTOR3(g_Bullet[nCntBullet].pos.x - sinf(fAngle)*fRadius, 0.0f, g_Bullet[nCntBullet].pos.z - cosf(fAngle)*fRadius);
                        D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                        SetEffect2(pos, move, EFFECT2_SIZE_HEAL, EFFECT2_INIT_COLOR_HEAL, EFFECT2TYPE_FIRE2); //炎エフェクト
                    }
                    g_Bullet[nCntBullet].nLife = 0;
                }
            }

            //ライフがなくなったら未使用に
            if (g_Bullet[nCntBullet].nLife <= 0)
            {
                g_Bullet[nCntBullet].bUse = false;	//使用しない状態にする
            }

        }
    }

    //頂点データアンロック
    g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//バレット描画処理
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
        if (g_Bullet[nCntBullet].bUse == true)
        {
            //ワールドマトリクスの初期化
            D3DXMatrixIdentity(&g_Bullet[nCntBullet].mtxWorld);
            //回転の逆行列を生成
            pDevice->GetTransform(D3DTS_VIEW, &mtxRot);
            D3DXMatrixInverse(&g_Bullet[nCntBullet].mtxWorld, NULL, &mtxRot);
            g_Bullet[nCntBullet].mtxWorld._41 = 0;//移動はさせたくないので0を代入
            g_Bullet[nCntBullet].mtxWorld._42 = 0;//移動はさせたくないので0を代入
            g_Bullet[nCntBullet].mtxWorld._43 = 0;//移動はさせたくないので0を代入

            //位置を反映
            D3DXMatrixTranslation(&mtxTrans, g_Bullet[nCntBullet].pos.x, g_Bullet[nCntBullet].pos.y, g_Bullet[nCntBullet].pos.z);
            D3DXMatrixMultiply(&g_Bullet[nCntBullet].mtxWorld, &g_Bullet[nCntBullet].mtxWorld, &mtxTrans);
            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_Bullet[nCntBullet].mtxWorld);
            //テクスチャの設定
            pDevice->SetTexture(0, g_pTextureBullet[g_Bullet[nCntBullet].type]);
            //頂点バッファをデバイスのデータストリームに設定
            pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));
            //頂点フォーマットの設定
            pDevice->SetFVF(FVF_VERTEX_3D);
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet*4, NUM_POLYGON);

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
//バレットの設定
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move,int nLife,int nCntRigid, float fDamage, BULLETTYPE type)
{
    VERTEX_3D *pVtx;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        if (g_Bullet[nCntBullet].bUse == false)
        {
            g_Bullet[nCntBullet].bUse = true;
            g_Bullet[nCntBullet].pos = pos;
            g_Bullet[nCntBullet].size = size;
            g_Bullet[nCntBullet].move = move;
            g_Bullet[nCntBullet].nLife = nLife;
            g_Bullet[nCntBullet].nCntRigid = nCntRigid;
            g_Bullet[nCntBullet].fDamage = fDamage;
            g_Bullet[nCntBullet].type = type;

            switch (g_Bullet[nCntBullet].type)
            {
            case BULLETTYPE_PLAYER:
                g_Bullet[nCntBullet].col = BULLET_COLOR_PLAYER;

                break;
            case BULLETTYPE_ENEMY:
                g_Bullet[nCntBullet].col = BULLET_COLOR_ENEMY;

                break;
            }

            //頂点座標の設定
            pVtx[0].pos = D3DXVECTOR3(-(g_Bullet[nCntBullet].size.x / 2), (g_Bullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[1].pos = D3DXVECTOR3((g_Bullet[nCntBullet].size.x / 2), (g_Bullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[2].pos = D3DXVECTOR3(-(g_Bullet[nCntBullet].size.x / 2), -(g_Bullet[nCntBullet].size.y / 2), 0.0f);
            pVtx[3].pos = D3DXVECTOR3((g_Bullet[nCntBullet].size.x / 2), -(g_Bullet[nCntBullet].size.y / 2), 0.0f);

            pVtx[0].col = g_Bullet[nCntBullet].col;
            pVtx[1].col = g_Bullet[nCntBullet].col;
            pVtx[2].col = g_Bullet[nCntBullet].col;
            pVtx[3].col = g_Bullet[nCntBullet].col;

            break;
        }
        pVtx += 4;
    }

    //頂点データアンロック
    g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//バレットの取得
//=============================================================================
BULLET*GetBullet(void)
{
    return &g_Bullet[0];
}

//=====================================================
// 敵の弾の当たり判定の設定
//=====================================================
bool CollisionBulletEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size,int nCnt)
{
    bool bHit = false;  //当たったかどうか

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        // 弾が使われたら
        if (g_Bullet[nCntBullet].bUse == true)
        {

            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_Bullet[nCntBullet].pos, g_Bullet[nCntBullet].size) == true)
            {
                //プレイヤーの弾
                if (g_Bullet[nCntBullet].type == BULLETTYPE_PLAYER)
                {
                    HitEnemy(nCnt, g_Bullet[nCntBullet].nCntRigid, g_Bullet[nCntBullet].fDamage);

                    PLAYER*pPlayer = GetPlayer();
                    //剣の必殺技
                    if (pPlayer->bChanceAttack == true && pPlayer->bBullet == false)
                    {
                        UpperEnemy(10.0f, nCnt);
                    }

                    //剣のヒット音
                    if (pPlayer->bBullet == false)
                    {
                        PlaySound(SOUND_LABEL_SE_EXPLOSION);
                    }

                    //ヒットエフェクト
                    float fAngle = 0.0f;    //角度
                    float fSpeed = 10.0f;   //速さ
                    CAMERA*pCamera = GetCamera();
                    for (int nCnt = 0; nCnt < 50; nCnt++)
                    {
                        //0～3.14を出す*(0 or 1)*-1で正負を決める
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                        SetEffect2(g_Bullet[nCntBullet].pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                    }

                    bHit = true;    //当たった
                    g_Bullet[nCntBullet].bUse = false;  //当たったら未使用に
                }
            }
        }
    }

    return bHit;    //当たったかどうかを返す
}

//=====================================================
// 自機の弾の当たり判定の設定
//=====================================================
bool CollisionBulletPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;  //当たったかどうか

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        // 弾が使われたら
        if (g_Bullet[nCntBullet].bUse == true)
        {

            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_Bullet[nCntBullet].pos, g_Bullet[nCntBullet].size) == true)
            {
                //エネミーの弾(当たったら未使用に)
                if (g_Bullet[nCntBullet].type == BULLETTYPE_ENEMY)
                {
                    PLAYER*pPlayer = GetPlayer();

                    //必殺中は無敵
                    if (pPlayer->bChanceAttack == false)
                    {
                        //自機にダメージ
                        HitPlayer(g_Bullet[nCntBullet].nCntRigid, g_Bullet[nCntBullet].fDamage);

                        //ノックバック
                        if (pPlayer->bChanceAttack == false)    //必殺攻撃中以外
                        {
                            float fKnockback = VALUE_KNOCKBACK;
                            pPlayer->move.x = pPlayer->pos.x - g_Bullet[nCntBullet].posOld.x;
                            pPlayer->move.z = pPlayer->pos.z - g_Bullet[nCntBullet].posOld.z;
                            D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //ベクトルを正規化
                            pPlayer->move.x = pPlayer->move.x*fKnockback;
                            pPlayer->move.z = pPlayer->move.z*fKnockback;
                        }


                        //ヒットエフェクト
                        float fAngle = 0.0f;    //角度
                        float fSpeed = 10.0f;   //速さ
                        CAMERA*pCamera = GetCamera();
                        for (int nCnt = 0; nCnt < 50; nCnt++)
                        {
                            //0～3.14を出す*(0 or 1)*-1で正負を決める
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                            SetEffect2(g_Bullet[nCntBullet].pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                        }
                    }

                    bHit = true;    //当たった
                    g_Bullet[nCntBullet].bUse = false;  //当たったら未使用に

                }

                //ボスの弾(当たっても未使用にならない)
                else if (g_Bullet[nCntBullet].type == BULLETTYPE_BOSS)
                {
                    PLAYER*pPlayer = GetPlayer();

                    //必殺中は無敵
                    if (pPlayer->bChanceAttack == false)
                    {
                        //自機にダメージ
                        HitPlayer(g_Bullet[nCntBullet].nCntRigid, g_Bullet[nCntBullet].fDamage);

                        //ノックバック
                        if (pPlayer->bChanceAttack == false)    //必殺攻撃中以外
                        {
                            float fKnockback = VALUE_KNOCKBACK;
                            pPlayer->move.x = pPlayer->pos.x - g_Bullet[nCntBullet].posOld.x;
                            pPlayer->move.z = pPlayer->pos.z - g_Bullet[nCntBullet].posOld.z;
                            D3DXVec3Normalize(&pPlayer->move, &pPlayer->move);  //ベクトルを正規化
                            pPlayer->move.x = pPlayer->move.x*fKnockback;
                            pPlayer->move.z = pPlayer->move.z*fKnockback;
                        }


                        //ヒットエフェクト
                        float fAngle = 0.0f;    //角度
                        float fSpeed = 10.0f;   //速さ
                        CAMERA*pCamera = GetCamera();
                        for (int nCnt = 0; nCnt < 50; nCnt++)
                        {
                            //0～3.14を出す*(0 or 1)*-1で正負を決める
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                            SetEffect2(g_Bullet[nCntBullet].pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                        }
                    }

                    bHit = true;    //当たった

                }
            }
        }
    }

    return bHit;    //当たったかどうかを返す
}

//=====================================================
// ボスの弾の当たり判定の設定
//=====================================================
bool CollisionBulletBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bHit = false;  //当たったかどうか

    for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
    {
        // 弾が使われたら
        if (g_Bullet[nCntBullet].bUse == true)
        {

            //当たり判定を計算
            if (RectangleCollision(pPos, pPosOld, size, &g_Bullet[nCntBullet].pos, g_Bullet[nCntBullet].size) == true)
            {
                //プレイヤーの弾
                if (g_Bullet[nCntBullet].type == BULLETTYPE_PLAYER)
                {
                    HitBoss(g_Bullet[nCntBullet].nCntRigid, g_Bullet[nCntBullet].fDamage);

                    PLAYER*pPlayer = GetPlayer();

                    //剣のヒット音
                    if (pPlayer->bBullet == false)
                    {
                        PlaySound(SOUND_LABEL_SE_EXPLOSION);
                    }

                    //ヒットエフェクト
                    float fAngle = 0.0f;    //角度
                    float fSpeed = 10.0f;   //速さ
                    CAMERA*pCamera = GetCamera();
                    for (int nCnt = 0; nCnt < 50; nCnt++)
                    {
                        //0～3.14を出す*(0 or 1)*-1で正負を決める
                        fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                        D3DXVECTOR3 move = D3DXVECTOR3(-sinf(fAngle + pCamera->rot.y)*fSpeed, cosf(fAngle)*fSpeed, -cosf(fAngle + pCamera->rot.y)*fSpeed);
                        SetEffect2(g_Bullet[nCntBullet].pos, move, EFFECT2_SIZE_HIT, EFFECT2_INIT_COLOR_HIT, EFFECT2TYPE_HIT);
                    }

                    bHit = true;    //当たった
                    g_Bullet[nCntBullet].bUse = false;  //当たったら未使用に
                }
            }
        }
    }

    return bHit;    //当たったかどうかを返す
}