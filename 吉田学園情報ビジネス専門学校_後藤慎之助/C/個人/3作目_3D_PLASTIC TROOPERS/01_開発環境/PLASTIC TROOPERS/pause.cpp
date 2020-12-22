//=============================================================================
//
// PLASTIC TROOPERSの処理 [pause.cpp] (ポーズ機能)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "pause.h"
#include "fade.h"
#include "controller.h"
#include "campaign.h"
#include "sound.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[3] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//頂点バッファへのポインタ

PAUSE g_Pause;  //ポーズ中の選択肢

//=============================================================================
// リザルト初期化処理
//=============================================================================
void InitPause(void)
{
    //変数宣言
    VERTEX_2D*pVtx = NULL;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //グローバル変数の初期化
    g_Pause = PAUSE_CONTINUE_GAME;

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * 3,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffPause,//変数名が変わると変更が必要
        NULL);

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/pause000.png", &g_pTexturePause[0]);	//ポーズのタイトルと背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/pause001.png", &g_pTexturePause[1]);	//ゲームをつづける
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/pause002.png", &g_pTexturePause[2]);	//ゲームをやめる

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //頂点座標の設定（右回りで設定する）
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //ゲームをつづける
    pVtx[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (460 / 2), (SCREEN_HEIGHT / 1.5) - (80 / 2) + 46, 0.0f);
    pVtx[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (460 / 2), (SCREEN_HEIGHT / 1.5) - (80 / 2) + 46, 0.0f);
    pVtx[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (460 / 2), (SCREEN_HEIGHT / 1.5) + (80 / 2) + 46, 0.0f);
    pVtx[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (460 / 2), (SCREEN_HEIGHT / 1.5) + (80 / 2) + 46, 0.0f);

    pVtx[8].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (460 / 2), (SCREEN_HEIGHT / 1.5) - (80 / 2) + 146, 0.0f);
    pVtx[9].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (460 / 2), (SCREEN_HEIGHT / 1.5) - (80 / 2) + 146, 0.0f);
    pVtx[10].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (460 / 2), (SCREEN_HEIGHT / 1.5) + (80 / 2) + 146, 0.0f);
    pVtx[11].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (460 / 2), (SCREEN_HEIGHT / 1.5) + (80 / 2) + 146, 0.0f);

    //rhwの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;
    pVtx[4].rhw = 1.0f;
    pVtx[5].rhw = 1.0f;
    pVtx[6].rhw = 1.0f;
    pVtx[7].rhw = 1.0f;
    pVtx[8].rhw = 1.0f;
    pVtx[9].rhw = 1.0f;
    pVtx[10].rhw = 1.0f;
    pVtx[11].rhw = 1.0f;

    //頂点カラーの設定
    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

    //テクスチャの座標
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

    //頂点データアンロック
    g_pVtxBuffPause->Unlock();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{
    int nFade = GetFade();	//フェードの情報を獲得
    DIJOYSTATE2 Controller = GetController();  //コントローラーの情報を取得
    VERTEX_2D *pVtx = NULL;

    g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    if (nFade == FADE_NONE)
    {
        //選択肢を上へ(ゲームをつづける)
        if (Controller.lY < 0 || Controller.rgdwPOV[0] == BUTTON_UP)
        {
            g_Pause = PAUSE_CONTINUE_GAME;
        }

        //選択肢を下へ(ゲームをやめる)
        else if (Controller.lY > 0 || Controller.rgdwPOV[0] == BUTTON_DOWN)
        {
            g_Pause = PAUSE_QUIT_GAME;
        }

        //A/Bボタンで選択
        if (GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B))
        {
            //ゲームをつづける
            if (g_Pause == PAUSE_CONTINUE_GAME)
            {
                //モードを取得
                MODE mode = GetMode();

                //キャンペーン
                if (mode == MODE_CAMPAIGN)
                {
                    SetCampaignState(CAMPAIGNSTATE_NORMAL);
                }
            }

            //ゲームをやめる
            else
            {
                //ゲームのBGMをとめる
                StopSound(SOUND_LABEL_BGM001);
                StopSound(SOUND_LABEL_BGM004);
                //タイトルへ
                SetFade(MODE_TITLE);
            }
        }

        //スタートボタンでゲームに戻る
        else if (GetJoypadTrigger(BUTTON_START))
        {
            //モードを取得
            MODE mode = GetMode();

            //キャンペーン
            if (mode == MODE_CAMPAIGN)
            {
                g_Pause = PAUSE_CONTINUE_GAME;  //カーソルをゲームをつづけるに戻す
                SetCampaignState(CAMPAIGNSTATE_NORMAL);//ポーズ解除
            }
        }

        //選択肢の透明度を変える
        switch (g_Pause)
        {
        case PAUSE_CONTINUE_GAME:

            //ゲームをつづけるを明るく
            pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

            //ゲームをやめるを暗く
            pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

            break;

        case PAUSE_QUIT_GAME:

            //ゲームをつづけるを暗く
            pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
            pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

            //ゲームをやめるを明るく
            pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
            pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

            break;
        }

    }

    //頂点データアンロック
    g_pVtxBuffPause->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
    //テクスチャの開放
    for (int nCntPause = 0; nCntPause < 3; nCntPause++)
    {
        if (g_pTexturePause[nCntPause] != NULL)
        {
            g_pTexturePause[nCntPause]->Release();
            g_pTexturePause[nCntPause] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffPause != NULL)
    {
        g_pVtxBuffPause->Release();
        g_pVtxBuffPause = NULL;
    }
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPause(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
 
    for (int nCntPause = 0; nCntPause < 3; nCntPause++)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_pTexturePause[nCntPause]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntPause, NUM_POLYGON);
    }

}