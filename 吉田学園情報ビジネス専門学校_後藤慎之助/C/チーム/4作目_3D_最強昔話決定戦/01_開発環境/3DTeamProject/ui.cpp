//=============================================================================
//
// 最強昔話決定戦 [ui.cpp]の処理 (ゲーム画面のUI)
// Author :後藤慎之助
//
//=============================================================================
#include "ui.h"
#include "player.h"
#include "player2.h"
#include "player3.h"
#include "player4.h"
#include "time.h"
#include "game.h"
#include "setting.h"
#include "boss.h"

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9			g_pTexturePolygonUI[UITYPE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffPolygonUI = NULL;

UI g_aUI[MAX_UI];   //UIの情報

int g_nTimeMinute;  //制限時間の分
int g_nCntStart;    //ゲーム開始を数える
//=============================================================================
// UI初期化処理
// Author:後藤慎之助
//=============================================================================
void InitUI(void)
{
    //宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //制限時間の初期化
    SETTING *pSetting = GetSetting();
    g_nTimeMinute = pSetting->nTimeLimit;
    g_nCntStart = 0;

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTexturePolygonUI[UITYPE_000]);	//制限時間の分
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui001.png", &g_pTexturePolygonUI[UITYPE_001]);	    //:
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui002.png", &g_pTexturePolygonUI[UITYPE_002]);	    //プレイヤー1
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui003.png", &g_pTexturePolygonUI[UITYPE_003]);	    //プレイヤー2
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui004.png", &g_pTexturePolygonUI[UITYPE_004]);	    //プレイヤー3
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui005.png", &g_pTexturePolygonUI[UITYPE_005]);	    //プレイヤー4
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui006.png", &g_pTexturePolygonUI[UITYPE_006]);	    //プレイヤー1の得点
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui006.png", &g_pTexturePolygonUI[UITYPE_007]);	    //プレイヤー2の得点
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui006.png", &g_pTexturePolygonUI[UITYPE_008]);	    //プレイヤー3の得点
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui006.png", &g_pTexturePolygonUI[UITYPE_009]);	    //プレイヤー4の得点
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui007.png", &g_pTexturePolygonUI[UITYPE_010]);	    //よーい、ドン!
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui008.png", &g_pTexturePolygonUI[UITYPE_011]);	    //急げ～
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui009.png", &g_pTexturePolygonUI[UITYPE_012]);	    //引き分け
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui010.png", &g_pTexturePolygonUI[UITYPE_013]);	    //nセット先取
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ui011.png", &g_pTexturePolygonUI[UITYPE_014]);	    //バトルロイヤル/ストーリー
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting013.png", &g_pTexturePolygonUI[UITYPE_015]); //ふつう/鬼
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar003.png", &g_pTexturePolygonUI[UITYPE_016]);	    //HPバーの背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar000.png", &g_pTexturePolygonUI[UITYPE_017]);	    //HPバー
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/HP.png", &g_pTexturePolygonUI[UITYPE_018]);         //HPの文字
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/score000.png", &g_pTexturePolygonUI[UITYPE_019]);   //スコアの背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar002.png", &g_pTexturePolygonUI[UITYPE_020]);	    //ボスのHPバーの背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/bar001.png", &g_pTexturePolygonUI[UITYPE_021]);     //ボスのHPバー
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/BOSS.png", &g_pTexturePolygonUI[UITYPE_022]);       //BOSS

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*NUM_VERTEX * MAX_UI, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &g_pVtxBuffPolygonUI, NULL);	//スコア、タイム、バー、武器、魔法、難易度
    VERTEX_2D *pVtx = NULL;	//バックグラウンドの頂点情報へのポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        g_aUI[nCntUI].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aUI[nCntUI].fSizeX = 0;
        g_aUI[nCntUI].fSizeY = 0;
        g_aUI[nCntUI].nCounterAnim = 0;
        g_aUI[nCntUI].nPatternAnim = 0;
        g_aUI[nCntUI].type = UITYPE_000;
        g_aUI[nCntUI].bUse = false;

        //頂点座標の設定（右回りで設定する）
        pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(0, 0, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(0, 0, 0.0f);

        //rhwの設定（値は1.0で設定する）
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //頂点カラーを設定（0～255の値で設定する）
        pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        //テクスチャ座標の設定
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffPolygonUI->Unlock();

}

//=============================================================================
// UI終了処理
// Author:後藤慎之助
//=============================================================================
void UninitUI(void)
{
    //テクスチャの開放
    for (int nCntUI = 0; nCntUI < UITYPE_MAX; nCntUI++)
    {
        if (g_pTexturePolygonUI[nCntUI] != NULL)
        {
            g_pTexturePolygonUI[nCntUI]->Release();
            g_pTexturePolygonUI[nCntUI] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffPolygonUI != NULL)
    {
        g_pVtxBuffPolygonUI->Release();
        g_pVtxBuffPolygonUI = NULL;
    }
}

//=============================================================================
// UI更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateUI(void)
{
    VERTEX_2D *pVtx = NULL;	            //バックグラウンドの頂点情報へのポインタ
    GAME *pGame = GetGame();            //ゲーム状況を取得
    SETTING *pSetting = GetSetting();   //設定情報を取得
    PLAYER *pPlayer = GetPlayer();      //プレイヤーを取得

    //ストーリーなら
    if (pSetting->nMaxPlayer == 1)
    {
        //難易度ふつうなら
        if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
        {
            //HPが250以上にならないようにする
            if (pPlayer->fLife > PLAYER_STORY_LIFE)
            {
                pPlayer->fLife = PLAYER_STORY_LIFE;
            }
        }

        //難易度鬼なら
        else
        {
            //HPが5以上にならないようにする
            if (pPlayer->fLife > PLAYER_LIFE)
            {
                pPlayer->fLife = PLAYER_LIFE;
            }
        }
    }

    //プレイヤーの体力が0を下回らないようにする
    if (pPlayer->fLife < 0)
    {
        pPlayer->fLife = 0;
    }

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);	//この書式は変えないこと

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == true)
        {
            //制限時間の分の数字
            if (g_aUI[nCntUI].type == UITYPE_000)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = TIME_COLOR;
                pVtx[1].col = TIME_COLOR;
                pVtx[2].col = TIME_COLOR;
                pVtx[3].col = TIME_COLOR;

                //0分
                if (g_nTimeMinute == 0)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
                }

                //1分
                else if (g_nTimeMinute == 1)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.1f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.1f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
                }

                //2分
                else if (g_nTimeMinute == 2)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.2f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.3f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.2f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.3f, 1.0f);
                }

                //3分
                else if (g_nTimeMinute == 3)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.3f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.4f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.3f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.4f, 1.0f);
                }

                //4分
                else if (g_nTimeMinute == 4)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.4f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.4f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //5分
                else if (g_nTimeMinute == 5)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
                }
            }

            //:
            else if (g_aUI[nCntUI].type == UITYPE_001)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = TIME_COLOR;
                pVtx[1].col = TIME_COLOR;
                pVtx[2].col = TIME_COLOR;
                pVtx[3].col = TIME_COLOR;

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //プレイヤー1
            else if (g_aUI[nCntUI].type == UITYPE_002)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //プレイヤー1が生存しているなら
                if (pPlayer->bDisp == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //プレイヤー2
            else if (g_aUI[nCntUI].type == UITYPE_003)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //プレイヤー2が生存しているなら
                PLAYER2 *pPlayer2 = GetPlayer2();
                if (pPlayer2->bDisp == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //プレイヤー3
            else if (g_aUI[nCntUI].type == UITYPE_004)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //プレイヤー3が生存しているなら
                PLAYER3 *pPlayer3 = GetPlayer3();
                if (pPlayer3->bDisp == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //プレイヤー4
            else if (g_aUI[nCntUI].type == UITYPE_005)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //プレイヤー4が生存しているなら
                PLAYER4 *pPlayer4 = GetPlayer4();
                if (pPlayer4->bDisp == true)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }
                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 55);
                }

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //プレイヤー1の得点
            else if (g_aUI[nCntUI].type == UITYPE_006)
            {
                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //0点
                if (pGame->nCntPointPlayer1 == 0)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* 0.0f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* 0.0f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
                }

                //1点
                else if (pGame->nCntPointPlayer1 == 1)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
                }

                //2点
                else if (pGame->nCntPointPlayer1 == 2)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //3点
                else if (pGame->nCntPointPlayer1 == 3)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
                }
            }

            //プレイヤー2の得点
            else if (g_aUI[nCntUI].type == UITYPE_007)
            {
                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //0点
                if (pGame->nCntPointPlayer2 == 0)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
                }

                //1点
                else if (pGame->nCntPointPlayer2 == 1)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
                }

                //2点
                else if (pGame->nCntPointPlayer2 == 2)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //3点
                else if (pGame->nCntPointPlayer2 == 3)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
                }
            }

            //プレイヤー3の得点
            else if (g_aUI[nCntUI].type == UITYPE_008)
            {
                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //0点
                if (pGame->nCntPointPlayer3 == 0)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
                }

                //1点
                else if (pGame->nCntPointPlayer3 == 1)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
                }

                //2点
                else if (pGame->nCntPointPlayer3 == 2)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //3点
                else if (pGame->nCntPointPlayer3 == 3)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
                }
            }

            //プレイヤー4の得点
            else if (g_aUI[nCntUI].type == UITYPE_009)
            {
                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //0点
                if (pGame->nCntPointPlayer4 == 0)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.0f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.0f, 0.0f);
                }

                //1点
                else if (pGame->nCntPointPlayer4 == 1)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.25f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
                }

                //2点
                else if (pGame->nCntPointPlayer4 == 2)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.5f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //3点
                else if (pGame->nCntPointPlayer4 == 3)
                {
                    //頂点座標の設定
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX) * 0.75f,
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.75f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.75f, 1.0f);
                }
            }

            //よーい、ドン!
            else if (g_aUI[nCntUI].type == UITYPE_010)
            {
                g_nCntStart++;

                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                if (g_nCntStart <= COUNT_WAIT_START_GAME * 2)
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                }

                else
                {
                    //頂点カラーを設定（0～255の値で設定する）
                    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
                }

                //よーい
                if (g_nCntStart <= COUNT_WAIT_START_GAME)
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }

                //はじめ!
                else
                {
                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
                }
            }

            //急げ～
            else if (g_aUI[nCntUI].type == UITYPE_011)
            {
                //画面外に出ていくまで動かす
                if (g_aUI[nCntUI].pos.x > -g_aUI[nCntUI].fSizeX)
                {
                    //2秒で画面外に出す
                    g_aUI[nCntUI].pos.x -= SCREEN_WIDTH / 120.0f + g_aUI[nCntUI].fSizeX / 120.0f;
                }

                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            //nセット先取
            else if (g_aUI[nCntUI].type == UITYPE_013)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //設定によって、セット先取の表示を変える
                if (pSetting->nMaxRound == 1)
                {
                    //テクスチャの座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.3333f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.3333f, 1.0f);
                }
                else if (pSetting->nMaxRound == 2)
                {
                    //テクスチャの座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.3333f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.6666f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.3333f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.6666f, 1.0f);
                }
                else if (pSetting->nMaxRound == 3)
                {
                    //テクスチャの座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.6666f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.6666f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
                }
            }

            //バトルロイヤル/ストーリー
            else if (g_aUI[nCntUI].type == UITYPE_014)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //設定によって、バトルロイヤルかストーリーを変える
                //ストーリー
                if (pSetting->nMaxPlayer == 1)
                {
                    //テクスチャの座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
                }
                //バトルロイヤル
                else
                {
                    //テクスチャの座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
                }
            }

            //ふつう/鬼
            else if (g_aUI[nCntUI].type == UITYPE_015)
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //設定によって、ふつうか鬼を変える
                if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
                {
                    //テクスチャの座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.6f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(0.8f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.6f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(0.8f, 1.0f);
                }
                else
                {
                    //テクスチャの座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.8f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.8f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
                }
            }

            //HPゲージ
            else if (g_aUI[nCntUI].type == UITYPE_017)
            {
                //難易度ふつうなら
                if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
                {
                    //頂点座標の設定（横ゲージ）
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_STORY_LIFE)),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_STORY_LIFE)),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //HPに応じてカラーを変える（満タンから半分まで緑）
                    if (pPlayer->fLife <= PLAYER_STORY_LIFE && pPlayer->fLife > PLAYER_STORY_LIFE / 2)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(70, 185, 127, 255);
                    }

                    //HPに応じてカラーを変える（半分から4分の1まで黄色）
                    else if (pPlayer->fLife <= PLAYER_STORY_LIFE / 2 && pPlayer->fLife > PLAYER_STORY_LIFE / 4)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(254, 236, 0, 255);
                    }

                    //HPに応じてカラーを変える（4分の1からは赤色）
                    else if (pPlayer->fLife <= PLAYER_STORY_LIFE / 4)
                    {
                        pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                        pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    }

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_STORY_LIFE), 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_STORY_LIFE), 1.0f);
                }

                //難易度鬼なら
                else
                {
                    //頂点座標の設定（横ゲージ）
                    pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_LIFE)),
                        g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                    pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2) + (g_aUI[nCntUI].fSizeX)* ((pPlayer->fLife / PLAYER_LIFE)),
                        g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                    //難易度鬼は、常に赤ゲージ
                    pVtx[0].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    pVtx[1].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    pVtx[2].col = D3DCOLOR_RGBA(254, 83, 12, 255);
                    pVtx[3].col = D3DCOLOR_RGBA(254, 83, 12, 255);

                    //テクスチャ座標の設定
                    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                    pVtx[1].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_LIFE), 0.0f);
                    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                    pVtx[3].tex = D3DXVECTOR2((pPlayer->fLife / PLAYER_LIFE), 1.0f);
                }

            }

            //ボスのHPゲージ
            else if (g_aUI[nCntUI].type == UITYPE_021)
            {
                BOSS *pBoss = GetBoss();

                //ボスの体力が、0を下回らないようにする
                if (pBoss->fLife < 0.0f)
                {
                    pBoss->fLife = 0.0f;
                }

                //頂点座標の設定（縦ゲージ）
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) - (g_aUI[nCntUI].fSizeY)* (pBoss->fLife / BOSS_LIFE), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2) - (g_aUI[nCntUI].fSizeY)* (pBoss->fLife / BOSS_LIFE), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2),
                    g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）(紫)
                pVtx[0].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                pVtx[1].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                pVtx[2].col = D3DCOLOR_RGBA(188, 78, 189, 255);
                pVtx[3].col = D3DCOLOR_RGBA(188, 78, 189, 255);

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f - (pBoss->fLife / BOSS_LIFE));
                pVtx[1].tex = D3DXVECTOR2(1.0f, 1.0f - (pBoss->fLife / BOSS_LIFE));
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

            else
            {
                //頂点座標の設定
                pVtx[0].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[1].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y - (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[2].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x - (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);
                pVtx[3].pos = D3DXVECTOR3(g_aUI[nCntUI].pos.x + (g_aUI[nCntUI].fSizeX / 2), g_aUI[nCntUI].pos.y + (g_aUI[nCntUI].fSizeY / 2), 0.0f);

                //頂点カラーを設定（0～255の値で設定する）
                pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
                pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

                //テクスチャ座標の設定
                pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
                pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
                pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
                pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
            }

        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffPolygonUI->Unlock();
}

//=============================================================================
// UI描画処理
// Author:後藤慎之助
//=============================================================================
void DrawUI(void)
{
    //宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 30);
    //アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffPolygonUI, 0, sizeof(VERTEX_2D));
    //頂点フォーマットの設定
    pDevice->SetFVF(FVF_VERTEX_2D);

    //ポリゴンの描画
    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == true)//敵が使用中なら描画
        {
            //テクスチャの設定
            pDevice->SetTexture(0, g_pTexturePolygonUI[g_aUI[nCntUI].type]);

            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUI, NUM_POLYGON);
        }
    }

    //アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// UIの設定処理
// Author:後藤慎之助
//=============================================================================
void SetUI(D3DXVECTOR3 pos, float fSizeX, float fSizeY, UITYPE type)
{
    //変数宣言
    VERTEX_2D *pVtx = NULL;

    //頂点バッファをロック
    g_pVtxBuffPolygonUI->Lock(0, 0, (void**)&pVtx, 0);

    for (int nCntUI = 0; nCntUI < MAX_UI; nCntUI++)
    {
        if (g_aUI[nCntUI].bUse == false)
        {
            //位置を設定
            g_aUI[nCntUI].pos = pos;
            g_aUI[nCntUI].fSizeX = fSizeX;
            g_aUI[nCntUI].fSizeY = fSizeY;
            g_aUI[nCntUI].type = type;
            g_aUI[nCntUI].bUse = true;
            break;
        }
        pVtx += 4;
    }

    //頂点データをアンロックする
    g_pVtxBuffPolygonUI->Unlock();

}