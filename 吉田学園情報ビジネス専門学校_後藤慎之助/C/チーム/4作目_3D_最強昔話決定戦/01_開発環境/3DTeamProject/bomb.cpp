//=============================================================================
//
// 最強昔話決定戦の処理 [bomb.cpp](爆弾)
// Author:後藤慎之助
//
//=============================================================================
#include "bomb.h"
#include "player.h"
#include "player2.h"
#include "player3.h"
#include "player4.h"
#include "collision.h"
#include "stage.h"
#include "player.h"
#include "fire.h"
#include "block.h"
#include "sound.h"
#include "effect.h"
#include "item.h"
#include "controller.h"
#include "shadow.h"
#include "sound.h"
#include "setting.h"
#include "enemy.h"
#include "bullet.h"
#include "input.h"
#include "boss.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define XFILE_NEME "MODEL/bomb.x"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshBomb = NULL;                     //メッシュ情報へのポインタ
DWORD g_nNumMatBomb = 0;                           //マテリアル情報の数
LPD3DXBUFFER g_pBuffMatBomb = NULL;                //マテリアル情報のポインタ
LPDIRECT3DTEXTURE9 *g_pTexturehBomb = NULL;        //ポリゴンのテクスチャへのポインタ

BOMB g_aBomb[MAX_BOMB]; //爆弾の情報

//=============================================================================
// ボム初期化処理
// Author:後藤慎之助
//=============================================================================
void InitBomb(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの読み込み

    //Xファイルの読み込み
    HRESULT result = D3DXLoadMeshFromX
    (
        XFILE_NEME,         //xファイルのパス
        D3DXMESH_SYSTEMMEM, //メッシュ作成用のオプションを指定
        pDevice,            //デバイス
        NULL,               //(未使用)
        &g_pBuffMatBomb,   //マテリアル情報
        NULL,               //(未使用)
        &g_nNumMatBomb,    //Xファイルに使用されているマテリアルの数
        &g_pMeshBomb       //xファイルのメッシュデータ
    );

    // マテリアルとテクスチャの設定
    D3DXMATERIAL    *materials = (D3DXMATERIAL*)g_pBuffMatBomb->GetBufferPointer();
    g_pTexturehBomb = new LPDIRECT3DTEXTURE9[g_nNumMatBomb];

    // テクスチャの読み出し
    for (int nCntBomb = 0; nCntBomb < (int)g_nNumMatBomb; nCntBomb++)
    {
        g_pTexturehBomb[nCntBomb] = NULL;
        if (materials[nCntBomb].pTextureFilename != NULL)
        {
            // テクスチャの読み込み
            HRESULT hr = D3DXCreateTextureFromFile(pDevice, materials[nCntBomb].pTextureFilename, &g_pTexturehBomb[nCntBomb]);
            int i = 0;
            i++;
        }
    }
    for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        g_aBomb[nCntBomb].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].posNew = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aBomb[nCntBomb].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
        g_aBomb[nCntBomb].sizeVisual = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
        g_aBomb[nCntBomb].type = SELECT_MOMO;
        g_aBomb[nCntBomb].KickType = SELECT_MAX;
        g_aBomb[nCntBomb].bUse = false;
        g_aBomb[nCntBomb].bFire = false;
        g_aBomb[nCntBomb].nCntTime = TIME_BOMB;
        g_aBomb[nCntBomb].nFire = 0;
        g_aBomb[nCntBomb].nShadow = 0;//影の設定
        g_aBomb[nCntBomb].bIncreaseSize = false;

        g_aBomb[nCntBomb].bCheckGridUp = false;
        g_aBomb[nCntBomb].bCheckGridDown = false;
        g_aBomb[nCntBomb].bCheckGridLeft = false;
        g_aBomb[nCntBomb].bCheckGridRight = false;
    }
}

//=============================================================================
// ボム終了処理
// Author:後藤慎之助
//=============================================================================
void UninitBomb(void)
{
    //メッシュの開放
    if (g_pMeshBomb != NULL)
    {
        g_pMeshBomb->Release();
        g_pMeshBomb = NULL;
    }
    //マテリアルの破棄
    if (g_pBuffMatBomb != NULL)
    {
        g_pBuffMatBomb->Release();
        g_pBuffMatBomb = NULL;
    }
    if (g_pTexturehBomb != NULL)
    {
        delete g_pTexturehBomb;
        g_pTexturehBomb = NULL;
    }
}

//=============================================================================
// ボム更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateBomb(void)
{
    //設定の情報を取得
    SETTING *pSetting = GetSetting();
    //各プレイヤーの取得
    PLAYER*pPlayer = GetPlayer();
    PLAYER2*pPlayer2 = GetPlayer2();
    PLAYER3*pPlayer3 = GetPlayer3();
    PLAYER4*pPlayer4 = GetPlayer4();
    //エフェクト関連の変数
    float fAngle = 0.0f;    //角度
    float fSpeedY = 0.0f;   //速さ
    float fRadius = 0.0f;   //エフェクト発生半径

    for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        //使用しているなら
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //爆発まで数える
            g_aBomb[nCntBomb].nCntTime--;

            //見かけの大きさを変える
            //減少中
            if (g_aBomb[nCntBomb].bIncreaseSize == false)
            {
                //爆破までの残り時間で、ボムの見た目の激しさを変える
                //穏やか
                if (g_aBomb[nCntBomb].nCntTime > TIME_BOMB / 3)
                {
                    g_aBomb[nCntBomb].sizeVisual.x -= 1.0f;
                    g_aBomb[nCntBomb].sizeVisual.y -= 1.0f;
                    g_aBomb[nCntBomb].sizeVisual.z -= 1.0f;
                }
                //激しめ
                else
                {
                    g_aBomb[nCntBomb].sizeVisual.x -= 10.0f;
                    g_aBomb[nCntBomb].sizeVisual.y -= 10.0f;
                    g_aBomb[nCntBomb].sizeVisual.z -= 10.0f;
                }

                //一定量で、増加させる
                if (g_aBomb[nCntBomb].sizeVisual.x < g_aBomb[nCntBomb].size.x * 2 / 3)
                {
                    g_aBomb[nCntBomb].bIncreaseSize = true;
                }
            }
            //増加中
            if (g_aBomb[nCntBomb].bIncreaseSize == true)
            {
                //爆破までの残り時間で、ボムの見た目の激しさを変える
                //穏やか
                if (g_aBomb[nCntBomb].nCntTime > TIME_BOMB / 3)
                {
                    g_aBomb[nCntBomb].sizeVisual.x += 1.0f;
                    g_aBomb[nCntBomb].sizeVisual.y += 1.0f;
                    g_aBomb[nCntBomb].sizeVisual.z += 1.0f;
                }
                //激しめ
                else
                {
                    g_aBomb[nCntBomb].sizeVisual.x += 10.0f;
                    g_aBomb[nCntBomb].sizeVisual.y += 10.0f;
                    g_aBomb[nCntBomb].sizeVisual.z += 10.0f;
                }

                //一定量で、減少させる
                if (g_aBomb[nCntBomb].sizeVisual.x > g_aBomb[nCntBomb].size.x)
                {
                    g_aBomb[nCntBomb].bIncreaseSize = false;
                }
            }

            //1F前の位置を記憶
            g_aBomb[nCntBomb].posOld = g_aBomb[nCntBomb].pos;

            //グリッドの中心の値を得る
            g_aBomb[nCntBomb].posGrid = ConversionGrid(&g_aBomb[nCntBomb].pos);

            //ボムブレーキ
            //プレイヤー1
            if (GetJoypadTrigger(PLAYER_1, BUTTON_Y) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                && g_aBomb[nCntBomb].KickType == SELECT_MOMO)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //プレイヤー2
            if (GetJoypadTrigger(PLAYER_2, BUTTON_Y) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                && g_aBomb[nCntBomb].KickType == SELECT_URA)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //プレイヤー3
            if (GetJoypadTrigger(PLAYER_3, BUTTON_Y) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                && g_aBomb[nCntBomb].KickType == SELECT_KIN)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //プレイヤー4
            if (GetJoypadTrigger(PLAYER_4, BUTTON_Y) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                && g_aBomb[nCntBomb].KickType == SELECT_ISUN)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //ブロックの当たり判定
            if (CollisionBlock(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //爆弾の同士の当たり判定
            if (CollisionBombs(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //ストーリーなら
            if (pSetting->nMaxPlayer == 1)
            {
                //エネミーの当たり判定
                if (CollisionEnemy(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE), false) == true)
                {
                    g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                }

                //ボムブレーキ
                if (GetKeyboardTrigger(DIK_BACKSPACE) && g_aBomb[nCntBomb].type == g_aBomb[nCntBomb].KickType
                    && g_aBomb[nCntBomb].KickType == SELECT_MOMO)
                {
                    g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                }

                //桃太郎の爆弾がボスに当たった時、未使用にする
                if (g_aBomb[nCntBomb].type == SELECT_MOMO)
                {
                    //弾の当たり判定
                    if (HitBullet(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        BreakBomb(nCntBomb);    //ボムを破壊する
                    }

                    //ボスとの当たり判定
                    if (CollisionBoss(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE), false) == true)
                    {
                        BreakBomb(nCntBomb);    //ボムを破壊する
                    }
                }
            }

            //プレイヤー1に対する当たり判定
            if (CollisionPlayer(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
            {
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
            }

            //バトルロイヤルなら
            if (pSetting->nMaxPlayer >= 2)
            {
                //プレイヤー2に対する当たり判定
                if (CollisionPlayer2(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                {
                    g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                    g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                }

                //3人以上なら
                if (pSetting->nMaxPlayer >= 3)
                {
                    //プレイヤー3に対する当たり判定
                    if (CollisionPlayer3(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                    {
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                        g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                    }

                    //4人なら
                    if (pSetting->nMaxPlayer >= 4)
                    {
                        //プレイヤー4に対する当たり判定
                        if (CollisionPlayer4(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE)) == true)
                        {
                            g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                            g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;
                        }
                    }
                }
            }

            //炎の当たり判定
            if (HitFire(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].pos,
                D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID * 2)) == true
                && g_aBomb[nCntBomb].bFire == false)
            {
                g_aBomb[nCntBomb].nCntTime = BOMB_TRIGGER_TIME;
                g_aBomb[nCntBomb].bFire = true;
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f); //誘爆の軸をずらさないため
                g_aBomb[nCntBomb].pos = g_aBomb[nCntBomb].posGrid;      //誘爆の軸をずらさないため
            }

            //上下左右のマスを確認し、ボムキックを制御する
            g_aBomb[nCntBomb].bCheckGridUp = CheckGridUp(nCntBomb);
            g_aBomb[nCntBomb].bCheckGridDown = CheckGridDown(nCntBomb);
            g_aBomb[nCntBomb].bCheckGridLeft = CheckGridLeft(nCntBomb);
            g_aBomb[nCntBomb].bCheckGridRight = CheckGridRight(nCntBomb);

            //移動量と位置を紐づける
            g_aBomb[nCntBomb].pos += g_aBomb[nCntBomb].move;

            //影を設定
            SetPositionShadow(g_aBomb[nCntBomb].nShadow
                , D3DXVECTOR3(g_aBomb[nCntBomb].pos.x, 0, g_aBomb[nCntBomb].pos.z), 0.0f);

            //ブロックのクラッシュ判定 (サドンデスがあるなら、判定を使用）
            if (pSetting->bSuddenDeath == true)
            {
                if (CrushingBlock(&g_aBomb[nCntBomb].pos, &g_aBomb[nCntBomb].posOld,
                    D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT * 2, GRID_SIZE), MAX_BLOCK + 1) == true)
                {
                    g_aBomb[nCntBomb].nCntTime = -1;    //即爆発
                }
            }

            //爆発
            if (g_aBomb[nCntBomb].nCntTime < 0)
            {
                //爆発音
                PlaySound(SOUND_LABEL_SE_EXPLOSION);

                //真ん中
                SetFire(g_aBomb[nCntBomb].posGrid, g_aBomb[nCntBomb].type);

                //炎エフェクト
                for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                {
                    fSpeedY = float(rand() % 800 + 300) / 100.0f;
                    //0～3.14を出す*(0 or 1)*-1で正負を決める
                    fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                    fRadius = float(rand() % 5000 + 0) / 100.0f;
                    D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posGrid.z - cosf(fAngle)*fRadius);
                    D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                    SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //炎エフェクト
                }

                //火力分、右に広げる
                for (int nCntExplosion = 1; nCntExplosion < g_aBomb[nCntBomb].nFire + 1; nCntExplosion++)
                {
                    //爆風の広がりを予測
                    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x + GRID_SIZE * nCntExplosion
                        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z);

                    //爆風が壊せないブロックに当たっていないかどうか判定
                    //当たっていないなら、爆風を設置
                    if (HitNormalBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == false)
                    {
                        //炎を設置
                        SetFire(D3DXVECTOR3(g_aBomb[nCntBomb].posNew), g_aBomb[nCntBomb].type);

                        //炎エフェクト
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                        {
                            fSpeedY = float(rand() % 800 + 300) / 100.0f;
                            //0～3.14を出す*(0 or 1)*-1で正負を決める
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            fRadius = float(rand() % 5000 + 0) / 100.0f;
                            D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posNew.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posNew.z - cosf(fAngle)*fRadius);
                            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                            SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //炎エフェクト
                        }
                    }

                    //当たっているなら、forから抜ける
                    else
                    {
                        break;
                    }

                    //壊せるブロックに爆風が当たったら、炎を置きながらforから抜ける
                    if (HitBrokenBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //アイテムに爆風が当たっても、炎を置きながらforを抜ける
                    if (HitAllTypeItem(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //爆弾に爆風が当たっていても、炎を置きながらforを抜ける
                    if (HitBomb(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT, GRID_SIZE - COLLISION_AVOID * 2), nCntBomb) == true)
                    {
                        break;
                    }

                    //ストーリー時、爆風を止めることができる敵に当たっていても、炎を置きながらforを抜ける
                    if (pSetting->nMaxPlayer == 1)
                    {
                        if (StopFireEnemy(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                        {
                            break;
                        }
                    }
                }

                //火力分、左に広げる
                for (int nCntExplosion = 1; nCntExplosion < g_aBomb[nCntBomb].nFire + 1; nCntExplosion++)
                {
                    //爆風の広がりを予測
                    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x - GRID_SIZE * nCntExplosion
                        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z);

                    //爆風が壊せないブロックに当たっていないかどうか判定
                    //当たっていないなら、爆風を設置
                    if (HitNormalBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == false)
                    {
                        //炎を設置
                        SetFire(D3DXVECTOR3(g_aBomb[nCntBomb].posNew), g_aBomb[nCntBomb].type);

                        //炎エフェクト
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                        {
                            fSpeedY = float(rand() % 800 + 300) / 100.0f;
                            //0～3.14を出す*(0 or 1)*-1で正負を決める
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            fRadius = float(rand() % 5000 + 0) / 100.0f;
                            D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posNew.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posNew.z - cosf(fAngle)*fRadius);
                            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                            SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //炎エフェクト
                        }
                    }

                    //当たっているなら、forから抜ける
                    else
                    {
                        break;
                    }

                    //壊せるブロックに爆風が当たったら、炎を置きながらforから抜ける
                    if (HitBrokenBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //アイテムに爆風が当たっても、炎を置きながらforを抜ける
                    if (HitAllTypeItem(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //爆弾に爆風が当たっていても、炎を置きながらforを抜ける
                    if (HitBomb(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT, GRID_SIZE - COLLISION_AVOID * 2), nCntBomb) == true)
                    {
                        break;
                    }

                    //ストーリー時、爆風を止めることができる敵に当たっていても、炎を置きながらforを抜ける
                    if (pSetting->nMaxPlayer == 1)
                    {
                        if (StopFireEnemy(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                        {
                            break;
                        }
                    }
                }

                //火力分、上に広げる
                for (int nCntExplosion = 1; nCntExplosion < g_aBomb[nCntBomb].nFire + 1; nCntExplosion++)
                {
                    //爆風の広がりを予測
                    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x
                        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z + GRID_SIZE * nCntExplosion);

                    //爆風が壊せないブロックに当たっていないかどうか判定
                    //当たっていないなら、爆風を設置
                    if (HitNormalBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == false)
                    {
                        //炎を設置
                        SetFire(D3DXVECTOR3(g_aBomb[nCntBomb].posNew), g_aBomb[nCntBomb].type);

                        //炎エフェクト
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                        {
                            fSpeedY = float(rand() % 800 + 300) / 100.0f;
                            //0～3.14を出す*(0 or 1)*-1で正負を決める
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            fRadius = float(rand() % 5000 + 0) / 100.0f;
                            D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posNew.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posNew.z - cosf(fAngle)*fRadius);
                            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                            SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //炎エフェクト
                        }
                    }

                    //当たっているなら、forから抜ける
                    else
                    {
                        break;
                    }

                    //壊せるブロックに爆風が当たったら、炎を置きながらforから抜ける
                    if (HitBrokenBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //アイテムに爆風が当たっても、炎を置きながらforを抜ける
                    if (HitAllTypeItem(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //爆弾に爆風が当たっていても、炎を置きながらforを抜ける
                    if (HitBomb(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT, GRID_SIZE - COLLISION_AVOID * 2), nCntBomb) == true)
                    {
                        break;
                    }

                    //ストーリー時、爆風を止めることができる敵に当たっていても、炎を置きながらforを抜ける
                    if (pSetting->nMaxPlayer == 1)
                    {
                        if (StopFireEnemy(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                        {
                            break;
                        }
                    }
                }

                //火力分、下に広げる
                for (int nCntExplosion = 1; nCntExplosion < g_aBomb[nCntBomb].nFire + 1; nCntExplosion++)
                {
                    //爆風の広がりを予測
                    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x
                        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z - GRID_SIZE * nCntExplosion);

                    //爆風が壊せないブロックに当たっていないかどうか判定
                    //当たっていないなら、爆風を設置
                    if (HitNormalBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == false)
                    {
                        //炎を設置
                        SetFire(D3DXVECTOR3(g_aBomb[nCntBomb].posNew), g_aBomb[nCntBomb].type);

                        //炎エフェクト
                        for (int nCnt = 0; nCnt < COUNT_EFFECT_FIRE; nCnt++)
                        {
                            fSpeedY = float(rand() % 800 + 300) / 100.0f;
                            //0～3.14を出す*(0 or 1)*-1で正負を決める
                            fAngle = float(rand() % 314) / 100.0f - float(rand() % 314) / 100.0f;
                            fRadius = float(rand() % 5000 + 0) / 100.0f;
                            D3DXVECTOR3 pos = D3DXVECTOR3(g_aBomb[nCntBomb].posNew.x - sinf(fAngle)*fRadius, 0.0f, g_aBomb[nCntBomb].posNew.z - cosf(fAngle)*fRadius);
                            D3DXVECTOR3 move = D3DXVECTOR3(0.0f, fSpeedY, 0.0f);
                            SetEffect(pos, move, EFFECT_SIZE_FIRE, EFFECTTYPE_FIRE); //炎エフェクト
                        }
                    }

                    //当たっているなら、forから抜ける
                    else
                    {
                        break;
                    }

                    //壊せるブロックに爆風が当たったら、炎を置きながらforから抜ける
                    if (HitBrokenBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //アイテムに爆風が当たっても、炎を置きながらforを抜ける
                    if (HitAllTypeItem(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                    {
                        break;
                    }

                    //爆弾に爆風が当たっていても、炎を置きながらforを抜ける
                    if (HitBomb(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID * 2, PLAYER_HEIGHT, GRID_SIZE - COLLISION_AVOID * 2), nCntBomb) == true)
                    {
                        break;
                    }

                    //ストーリー時、爆風を止めることができる敵に当たっていても、炎を置きながらforを抜ける
                    if (pSetting->nMaxPlayer == 1)
                    {
                        if (StopFireEnemy(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
                            D3DXVECTOR3(GRID_SIZE, PLAYER_HEIGHT, GRID_SIZE)) == true)
                        {
                            break;
                        }
                    }
                }

                //未使用に
                g_aBomb[nCntBomb].bUse = false;

                //影の削除
                SHADOW *pShadow = GetShadow();//影の情報を取得
                pShadow[g_aBomb[nCntBomb].nShadow].use = false;

                //初期化
                g_aBomb[nCntBomb].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].posNew = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
                g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

                //誰が置いたかに応じて、そのプレイヤーが再度ボムを置けるようにする
                switch (g_aBomb[nCntBomb].type)
                {
                    //桃太郎
                case SELECT_MOMO:
                    pPlayer->nBomb++;
                    break;

                    //浦島太郎
                case SELECT_URA:
                    pPlayer2->nBomb++;
                    break;

                    //金太郎
                case SELECT_KIN:
                    pPlayer3->nBomb++;
                    break;

                    //一寸法師
                case SELECT_ISUN:
                    pPlayer4->nBomb++;
                    break;
                }
            }
        }
    }
}
//=============================================================================
// ボム描画処理
// Author:後藤慎之助
//=============================================================================
void DrawBomb(void)
{

    LPDIRECT3DDEVICE9 pDevice = GetDevice();//デバイスの読み込み
    D3DXMATRIX mtxRot, mtxTrans, mtxscale;
    D3DMATERIAL9 matDef;//現在のマテリアル保持用
    D3DXMATERIAL *pMat;//マテリアルデータへのポインタ

    for (int nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        //ワールドマトリックスの初期化
        D3DXMatrixIdentity(&g_aBomb[nCntBomb].mtxWorld);
        if (g_aBomb[nCntBomb].bUse)
        {
            //拡縮を反映
            D3DXMatrixScaling(&mtxscale, g_aBomb[nCntBomb].sizeVisual.x / g_aBomb[nCntBomb].size.x,
                g_aBomb[nCntBomb].sizeVisual.y / g_aBomb[nCntBomb].size.y
                , g_aBomb[nCntBomb].sizeVisual.z / g_aBomb[nCntBomb].size.z);
            D3DXMatrixMultiply(&g_aBomb[nCntBomb].mtxWorld, &g_aBomb[nCntBomb].mtxWorld, &mtxscale);
            //向きを反映
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBomb[nCntBomb].rot.y, g_aBomb[nCntBomb].rot.x, g_aBomb[nCntBomb].rot.z);
            D3DXMatrixMultiply(&g_aBomb[nCntBomb].mtxWorld, &g_aBomb[nCntBomb].mtxWorld, &mtxRot);
            //位置を反映
            D3DXMatrixTranslation(&mtxTrans, g_aBomb[nCntBomb].pos.x, g_aBomb[nCntBomb].pos.y, g_aBomb[nCntBomb].pos.z);
            D3DXMatrixMultiply(&g_aBomb[nCntBomb].mtxWorld, &g_aBomb[nCntBomb].mtxWorld, &mtxTrans);

            //ワールドマトリックスの設定
            pDevice->SetTransform(D3DTS_WORLD, &g_aBomb[nCntBomb].mtxWorld);

            //現在のマテリアルを取得する
            pDevice->GetMaterial(&matDef);

            //マテリアルデータのポインタを取得
            pMat = (D3DXMATERIAL*)g_pBuffMatBomb->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatBomb; nCntMat++)
            {
                //マテリアルの設定
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pDevice->SetTexture(0, g_pTexturehBomb[nCntMat]);
                //モデルパーツの描画
                g_pMeshBomb->DrawSubset(nCntMat);
            }

            //保持していたマテリアルを戻す
            pDevice->SetMaterial(&matDef);
        }
    }

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// ボムの情報の取得
// Author:後藤慎之助
//=============================================================================
BOMB *GetBomb(void)
{
    return &g_aBomb[0];
}

//=====================================================
// ボムの同士の衝突判定の設定
// Author:後藤慎之助
//=====================================================
bool CollisionBombs(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBomb)
{
    bool bCollision = false;
    int nCntBomb;					// ローカル変数

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // ボムが使われたら
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //番号が違うなら
            if (nCntBomb != nNumberBomb)
            {
                //当たり判定を計算し、当たっているなら(隣り合わせのグリッドにあるものを省くため、XとZは-1してある)
                if (BoxCollision(pPos, pPosOld, size, &g_aBomb[nCntBomb].pos,
                    D3DXVECTOR3(g_aBomb[nCntBomb].size.x - 1, g_aBomb[nCntBomb].size.y * 4, g_aBomb[nCntBomb].size.z - 1)) == true)
                {
                    //お互いに蹴られたボムなら
                    if (g_aBomb[nCntBomb].move.x > 0 && g_aBomb[nNumberBomb].move.x < 0 ||
                        g_aBomb[nCntBomb].move.x > 0 && g_aBomb[nNumberBomb].move.z > 0 ||
                        g_aBomb[nCntBomb].move.x > 0 && g_aBomb[nNumberBomb].move.z < 0 ||
                        g_aBomb[nCntBomb].move.x < 0 && g_aBomb[nNumberBomb].move.x > 0 ||
                        g_aBomb[nCntBomb].move.x < 0 && g_aBomb[nNumberBomb].move.z > 0 ||
                        g_aBomb[nCntBomb].move.x < 0 && g_aBomb[nNumberBomb].move.z < 0 ||
                        g_aBomb[nCntBomb].move.z > 0 && g_aBomb[nNumberBomb].move.x < 0 ||
                        g_aBomb[nCntBomb].move.z > 0 && g_aBomb[nNumberBomb].move.x > 0 ||
                        g_aBomb[nCntBomb].move.z > 0 && g_aBomb[nNumberBomb].move.z < 0 ||
                        g_aBomb[nCntBomb].move.z < 0 && g_aBomb[nNumberBomb].move.x > 0 ||
                        g_aBomb[nCntBomb].move.z < 0 && g_aBomb[nNumberBomb].move.z > 0 ||
                        g_aBomb[nCntBomb].move.z < 0 && g_aBomb[nNumberBomb].move.x < 0 )
                    {
                        //ぶつかった爆弾同士の火力を調べる
                        if (g_aBomb[nNumberBomb].nFire > g_aBomb[nCntBomb].nFire)       //ぶつかった方の火力が高いなら
                        {
                            BreakBomb(nCntBomb);    //ぶつかられたほうを破壊
                        }
                        else if (g_aBomb[nNumberBomb].nFire == g_aBomb[nCntBomb].nFire) //火力が同じなら
                        {
                            //どちらも破壊
                            BreakBomb(nNumberBomb);
                            BreakBomb(nCntBomb);
                        }
                        else if (g_aBomb[nNumberBomb].nFire < g_aBomb[nCntBomb].nFire)  //ぶつかられた方の火力が高いなら
                        {
                            BreakBomb(nNumberBomb); //ぶつかったほうを破壊
                        }
                    }

                    bCollision = true;
                }
            }
        }
    }

    return bCollision;
}

//=====================================================
// ボムの衝突判定の設定
// Author:後藤慎之助
//=====================================================
bool CollisionBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    bool bCollision = false;
    int nCntBomb;					// ローカル変数

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // ボムが使われたら
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //当たり判定を計算し、当たっているなら
            if (BoxCollision(pPos, pPosOld, size, &g_aBomb[nCntBomb].pos,
                D3DXVECTOR3(g_aBomb[nCntBomb].size.x, g_aBomb[nCntBomb].size.y * 4, g_aBomb[nCntBomb].size.z)) == true)
            {
                bCollision = true;
            }
        }
    }

    return bCollision;
}

//=====================================================
// ボムの当たり判定の設定
// Author:後藤慎之助
//=====================================================
bool HitBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBomb)
{
    bool bHit = false;
    int nCntBomb;					// ローカル変数

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // ボムが使われたら
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //番号が違うなら
            if (nCntBomb != nNumberBomb)
            {
                if (RectangleCollision(pPos, pPosOld, size, &g_aBomb[nCntBomb].pos, g_aBomb[nCntBomb].size) == true)
                {
                    bHit = true;
                }
            }
        }
    }

    return bHit;
}

//=====================================================
// ボムキック判定の設定
// Author:後藤慎之助
//=====================================================
void KickBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, SELECT type)
{
    int nCntBomb;					// ローカル変数

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // ボムが使われたら
        if (g_aBomb[nCntBomb].bUse == true)
        {
            //プレイヤーの位置
            D3DXVECTOR3 box1Max = D3DXVECTOR3(size.x / 2, size.y / 2, size.z / 2) + *pPos;
            D3DXVECTOR3 box1Min = D3DXVECTOR3(-size.x / 2, -size.y / 2, -size.z / 2) + *pPos;

            //ボムの位置
            D3DXVECTOR3 box2Max = D3DXVECTOR3(g_aBomb[nCntBomb].size.x / 2, g_aBomb[nCntBomb].size.y / 2, g_aBomb[nCntBomb].size.z / 2) + g_aBomb[nCntBomb].pos;
            D3DXVECTOR3 box2Min = D3DXVECTOR3(-g_aBomb[nCntBomb].size.x / 2, -g_aBomb[nCntBomb].size.y / 2, -g_aBomb[nCntBomb].size.z / 2) + g_aBomb[nCntBomb].pos;

            // ボムとの当たり判定
            if ((box1Max.y > box2Min.y) &&
                (box1Min.y < box2Max.y) &&
                (box1Max.x > box2Min.x) &&
                (box1Min.x < box2Max.x) &&
                (box1Min.z < box2Max.z) &&
                (box1Max.z > box2Min.z))
            {

                if (box1Max.y + size.y > box2Min.y &&
                    pPosOld->y + (size.y / 2) <= box2Min.y)
                {
                    // ボムに下から当たった時

                }
                else if (box1Min.x < box2Max.x &&
                    pPosOld->x - (size.x / 2) >= box2Max.x)
                {
                    if (g_aBomb[nCntBomb].bCheckGridLeft == false)
                    {
                        //蹴った人を記録する
                        g_aBomb[nCntBomb].KickType = type;
                        //ボムキック音
                        PlaySound(SOUND_LABEL_SE_KICK);
                        // ボムに右から当たったとき
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(-KICK_MOVE, 0.0f, 0.0f);
                    }
                }
                else if (box1Max.x >  box2Min.x &&
                    pPosOld->x + (size.x / 2) <= box2Min.x)
                {
                    if (g_aBomb[nCntBomb].bCheckGridRight == false)
                    {
                        //蹴った人を記録する
                        g_aBomb[nCntBomb].KickType = type;
                        //ボムキック音
                        PlaySound(SOUND_LABEL_SE_KICK);
                        // ボムに左から当たった時
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(KICK_MOVE, 0.0f, 0.0f);
                    }
                }
                else if ((box1Min.y + size.y / 2) < box2Max.y &&
                    pPosOld->y - (0.0f) >= box2Max.y)
                {
                    // ボムに上から当たった時

                }
                else if (box1Min.z < box2Max.z &&
                    pPosOld->z - (size.z / 2) >= box2Max.z)
                {
                    if (g_aBomb[nCntBomb].bCheckGridDown == false)
                    {
                        //蹴った人を記録する
                        g_aBomb[nCntBomb].KickType = type;
                        //ボムキック音
                        PlaySound(SOUND_LABEL_SE_KICK);
                        // ボムに奥から当たった時
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, -KICK_MOVE);
                    }

                }
                else if (box1Max.z > box2Min.z &&
                    pPosOld->z + (size.z / 2) <= box2Min.z)
                {
                    if (g_aBomb[nCntBomb].bCheckGridUp == false)
                    {
                        //蹴った人を記録する
                        g_aBomb[nCntBomb].KickType = type;
                        //ボムキック音
                        PlaySound(SOUND_LABEL_SE_KICK);
                        // ボムに手前から当たった時
                        g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, KICK_MOVE);
                    }
                }
            }
        }
    }

}

//=====================================================
// ボムの設定
// Author:後藤慎之助
//=====================================================
void SetBomb(D3DXVECTOR3 pos, int nFire, int nCntTime, SELECT type)
{

    int nCntBomb;				// ローカル変数

    for (nCntBomb = 0; nCntBomb < MAX_BOMB; nCntBomb++)
    {
        // ボムが使われてないとき
        if (g_aBomb[nCntBomb].bUse == false)
        {
            // ボムを使用している状態にする
            g_aBomb[nCntBomb].pos = pos;
            g_aBomb[nCntBomb].nFire = nFire;
            g_aBomb[nCntBomb].nCntTime = nCntTime;  //爆発までの時間を紐づけ
            g_aBomb[nCntBomb].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
            g_aBomb[nCntBomb].sizeVisual = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
            g_aBomb[nCntBomb].type = type;
            g_aBomb[nCntBomb].KickType = SELECT_MAX;
            g_aBomb[nCntBomb].bFire = false;
            g_aBomb[nCntBomb].nShadow = SetShadow(g_aBomb[nCntBomb].pos, g_aBomb[nCntBomb].rot, 
                g_aBomb[nCntBomb].size.x, 0.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, SHADOW_ALPHA_MAX));//影の設定
            g_aBomb[nCntBomb].bUse = true;
            g_aBomb[nCntBomb].bIncreaseSize = false;

            g_aBomb[nCntBomb].bCheckGridUp = false;
            g_aBomb[nCntBomb].bCheckGridDown = false;
            g_aBomb[nCntBomb].bCheckGridLeft = false;
            g_aBomb[nCntBomb].bCheckGridRight = false;

            break;
        }
    }
}

//=====================================================
// 上のグリッドを確認
// Author:後藤慎之助
//=====================================================
bool CheckGridUp(int nCntBomb)
{
    // 変数宣言
    bool bExist = false;    // 存在しているかどうか

    // 上のグリッドに入ったと、新しい位置を仮定する
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x
            , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z + GRID_SIZE);

    //爆弾の同士の当たり判定
    if (CollisionBombs(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
    {
        bExist = true;
    }

    //ブロックとの当たり判定
    if (bExist == false)
    {
        if (HitAllTypeBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID)) == true)
        {
            bExist = true;
        }
    }

    return bExist;
}

//=====================================================
// 下のグリッドを確認
// Author:後藤慎之助
//=====================================================
bool CheckGridDown(int nCntBomb)
{
    // 変数宣言
    bool bExist = false;    // 存在しているかどうか

     // 下のグリッドに入ったと、新しい位置を仮定する
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x
        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z - GRID_SIZE);

    //爆弾の同士の当たり判定
    if (CollisionBombs(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
    {
        bExist = true;
    }

    //ブロックとの当たり判定
    if (bExist == false)
    {
        if (HitAllTypeBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID)) == true)
        {
            bExist = true;
        }
    }

    return bExist;
}

//=====================================================
// 左のグリッドを確認
// Author:後藤慎之助
//=====================================================
bool CheckGridLeft(int nCntBomb)
{
    // 変数宣言
    bool bExist = false;    // 存在しているかどうか

    // 左のグリッドに入ったと、新しい位置を仮定する
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x - GRID_SIZE
        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z);

    //爆弾の同士の当たり判定
    if (CollisionBombs(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
    {
        bExist = true;
    }

    //ブロックとの当たり判定
    if (bExist == false)
    {
        if (HitAllTypeBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID)) == true)
        {
            bExist = true;
        }
    }

    return bExist;
}

//=====================================================
// 右のグリッドを確認
// Author:後藤慎之助
//=====================================================
bool CheckGridRight(int nCntBomb)
{
    // 変数宣言
    bool bExist = false;    // 存在しているかどうか

    // 右のグリッドに入ったと、新しい位置を仮定する
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(g_aBomb[nCntBomb].posGrid.x + GRID_SIZE
        , g_aBomb[nCntBomb].posGrid.y, g_aBomb[nCntBomb].posGrid.z);

    //爆弾の同士の当たり判定
    if (CollisionBombs(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
        D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID), nCntBomb) == true)
    {
        bExist = true;
    }

    //ブロックとの当たり判定
    if (bExist == false)
    {
        if (HitAllTypeBlock(&g_aBomb[nCntBomb].posNew, &g_aBomb[nCntBomb].posNew,
            D3DXVECTOR3(GRID_SIZE - COLLISION_AVOID, PLAYER_HEIGHT * 2, GRID_SIZE - COLLISION_AVOID)) == true)
        {
            bExist = true;
        }
    }

    return bExist;
}

//=====================================================
// ボムを破壊する
// Author:後藤慎之助
//=====================================================
void BreakBomb(int nCntBomb)
{
    //未使用に
    g_aBomb[nCntBomb].bUse = false;

    //ボム破壊エフェクト
    float fSpeedX = 0.0f;   //速さX
    float fSpeedY = 0.0f;   //速さY
    float fSpeedZ = 0.0f;   //速さZ
    int nPlusMinusX = 0;    //符号X
    int nPlusMinusZ = 0;    //符号Z
    for (int nCnt = 0; nCnt < COUNT_EFFECT_EXPLODE_ITEM; nCnt++)
    {
        fSpeedX = float(rand() % 200) / 100.0f;
        fSpeedY = float(rand() % 800 + 300) / 100.0f;
        fSpeedZ = float(rand() % 200) / 100.0f;
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
        SetEffect(g_aBomb[nCntBomb].pos, move, EFFECT_SIZE_EXPLODE_ITEM, EFFECTTYPE_EXPLOSION);
    }

    //影の削除
    SHADOW *pShadow = GetShadow();//影の情報を取得
    pShadow[g_aBomb[nCntBomb].nShadow].use = false;

    //初期化
    g_aBomb[nCntBomb].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_aBomb[nCntBomb].posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_aBomb[nCntBomb].posNew = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_aBomb[nCntBomb].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_aBomb[nCntBomb].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    //誰が置いたかに応じて、そのプレイヤーが再度ボムを置けるようにする
    //各プレイヤーの取得
    PLAYER*pPlayer = GetPlayer();
    PLAYER2*pPlayer2 = GetPlayer2();
    PLAYER3*pPlayer3 = GetPlayer3();
    PLAYER4*pPlayer4 = GetPlayer4();
    switch (g_aBomb[nCntBomb].type)
    {
        //桃太郎
    case SELECT_MOMO:
        pPlayer->nBomb++;
        break;

        //浦島太郎
    case SELECT_URA:
        pPlayer2->nBomb++;
        break;

        //金太郎
    case SELECT_KIN:
        pPlayer3->nBomb++;
        break;

        //一寸法師
    case SELECT_ISUN:
        pPlayer4->nBomb++;
        break;
    }
}