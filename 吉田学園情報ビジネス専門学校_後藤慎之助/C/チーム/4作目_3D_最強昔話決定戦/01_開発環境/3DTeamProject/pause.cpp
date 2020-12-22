//=============================================================================
//
// 最強昔話決定戦の処理 [pause.cpp] (ポーズ機能)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "pause.h"
#include "fade.h"
#include "controller.h"
#include "game.h"
#include "sound.h"
#include "setting.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTexturePause[3] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;//頂点バッファへのポインタ

PAUSE_SELECT g_PauseSelect;  //ポーズ中の選択肢
PAUSE g_Pause;               //ポーズの情報

//=============================================================================
// ポーズ初期化処理
// Author:後藤慎之助
//=============================================================================
void InitPause(void)
{
    //変数宣言
    VERTEX_2D*pVtx = NULL;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //グローバル変数の初期化
    g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;

    //構造体の初期化
    g_Pause.who = SELECT_MAX;

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
// ポーズ更新処理
// Author:後藤慎之助
//=============================================================================
void UpdatePause(void)
{
    //ゲームと設定を取得
    GAME *pGame = GetGame();
    SETTING *pSetting = GetSetting();

    int nFade = GetFade();	//フェードの情報を獲得

    VERTEX_2D *pVtx = NULL;

    g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    if (nFade == FADE_NONE)
    {
        //プレイヤー1が押した場合
        if (g_Pause.who == SELECT_MOMO)
        {
            //各コントローラーの取得
            DIJOYSTATE2 Controller = GetController(PLAYER_1);

            //選択肢を上へ(ゲームをつづける)
            if (g_PauseSelect == PAUSE_SELECT_QUIT_GAME && Controller.lY < 0 || Controller.rgdwPOV[0] == BUTTON_UP)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;
            }

            //選択肢を下へ(ゲームをやめる)
            else if (g_PauseSelect == PAUSE_SELECT_CONTINUE_GAME && Controller.lY > 0 || Controller.rgdwPOV[0] == BUTTON_DOWN)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_PauseSelect = PAUSE_SELECT_QUIT_GAME;
            }

            //A/Bボタンで選択
            if (GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //決定音

                //ゲームをつづける
                if (g_PauseSelect == PAUSE_SELECT_CONTINUE_GAME)
                {
                    //モードを取得
                    MODE mode = GetMode();

                    //ゲームなら
                    if (mode == MODE_GAME || mode == MODE_NEXT_GAME)
                    {
                        SetGameState(GAMESTATE_NORMAL);
                    }
                }

                //ゲームをやめる
                else
                {
                    //一人用で変わった設定を戻す
                    pSetting->nTimeLimit = pGame->nMemoryTimeLimit;
                    pSetting->bRandomStartPos = pGame->bMemoryRandomStartPos;
                    pSetting->bSuddenDeath = pGame->bMemorySuddenDeath;

                    StopSound(SOUND_LABEL_BGM_GAME);//ゲーム曲を停止
                    StopSound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //サドンデスの音楽を停止
                    //タイトルへ
                    SetFade(MODE_TITLE);
                }
            }

            //スタートボタンでゲームに戻る
            else if (GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                //モードを取得
                MODE mode = GetMode();

                //ゲームなら
                if (mode == MODE_GAME || mode == MODE_NEXT_GAME)
                {
                    g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;  //カーソルをゲームをつづけるに戻す
                    SetGameState(GAMESTATE_NORMAL); //ポーズ解除
                }
            }
        }

        //プレイヤー2
        else if (g_Pause.who == SELECT_URA)
        {
            //各コントローラーの取得
            DIJOYSTATE2 Controller2 = GetController(PLAYER_2);

            //選択肢を上へ(ゲームをつづける)
            if (g_PauseSelect == PAUSE_SELECT_QUIT_GAME && Controller2.lY < 0 || Controller2.rgdwPOV[0] == BUTTON_UP)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;
            }

            //選択肢を下へ(ゲームをやめる)
            else if (g_PauseSelect == PAUSE_SELECT_CONTINUE_GAME && Controller2.lY > 0 || Controller2.rgdwPOV[0] == BUTTON_DOWN)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_PauseSelect = PAUSE_SELECT_QUIT_GAME;
            }

            //A/Bボタンで選択
            if (GetJoypadTrigger(PLAYER_2, BUTTON_A) || GetJoypadTrigger(PLAYER_2, BUTTON_B))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //決定音

                //ゲームをつづける
                if (g_PauseSelect == PAUSE_SELECT_CONTINUE_GAME)
                {
                    //モードを取得
                    MODE mode = GetMode();

                    //ゲームなら
                    if (mode == MODE_GAME || mode == MODE_NEXT_GAME)
                    {
                        SetGameState(GAMESTATE_NORMAL);
                    }
                }

                //ゲームをやめる
                else
                {
                    //一人用で変わった設定を戻す
                    pSetting->nTimeLimit = pGame->nMemoryTimeLimit;
                    pSetting->bRandomStartPos = pGame->bMemoryRandomStartPos;
                    pSetting->bSuddenDeath = pGame->bMemorySuddenDeath;

                    StopSound(SOUND_LABEL_BGM_GAME);//ゲーム曲を停止
                    StopSound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //サドンデスの音楽を停止
                    //タイトルへ
                    SetFade(MODE_TITLE);
                }
            }

            //スタートボタンでゲームに戻る
            else if (GetJoypadTrigger(PLAYER_2, BUTTON_START))
            {
                //モードを取得
                MODE mode = GetMode();

                //ゲームなら
                if (mode == MODE_GAME || mode == MODE_NEXT_GAME)
                {
                    g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;  //カーソルをゲームをつづけるに戻す
                    SetGameState(GAMESTATE_NORMAL); //ポーズ解除
                }
            }
        }

        //プレイヤー3
        else if (g_Pause.who == SELECT_KIN)
        {
            //各コントローラーの取得
            DIJOYSTATE2 Controller3 = GetController(PLAYER_3);

            //選択肢を上へ(ゲームをつづける)
            if (g_PauseSelect == PAUSE_SELECT_QUIT_GAME && Controller3.lY < 0 || Controller3.rgdwPOV[0] == BUTTON_UP)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;
            }

            //選択肢を下へ(ゲームをやめる)
            else if (g_PauseSelect == PAUSE_SELECT_CONTINUE_GAME && Controller3.lY > 0 || Controller3.rgdwPOV[0] == BUTTON_DOWN)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_PauseSelect = PAUSE_SELECT_QUIT_GAME;
            }

            //A/Bボタンで選択
            if (GetJoypadTrigger(PLAYER_3, BUTTON_A) || GetJoypadTrigger(PLAYER_3, BUTTON_B))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //決定音

                //ゲームをつづける
                if (g_PauseSelect == PAUSE_SELECT_CONTINUE_GAME)
                {
                    //モードを取得
                    MODE mode = GetMode();

                    //ゲームなら
                    if (mode == MODE_GAME || mode == MODE_NEXT_GAME)
                    {
                        SetGameState(GAMESTATE_NORMAL);
                    }
                }

                //ゲームをやめる
                else
                {
                    //一人用で変わった設定を戻す
                    pSetting->nTimeLimit = pGame->nMemoryTimeLimit;
                    pSetting->bRandomStartPos = pGame->bMemoryRandomStartPos;
                    pSetting->bSuddenDeath = pGame->bMemorySuddenDeath;

                    StopSound(SOUND_LABEL_BGM_GAME);//ゲーム曲を停止
                    StopSound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //サドンデスの音楽を停止
                    //タイトルへ
                    SetFade(MODE_TITLE);
                }
            }

            //スタートボタンでゲームに戻る
            else if (GetJoypadTrigger(PLAYER_3, BUTTON_START))
            {
                //モードを取得
                MODE mode = GetMode();

                //ゲームなら
                if (mode == MODE_GAME || mode == MODE_NEXT_GAME)
                {
                    g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;  //カーソルをゲームをつづけるに戻す
                    SetGameState(GAMESTATE_NORMAL); //ポーズ解除
                }
            }
        }

        //プレイヤー4
        else if (g_Pause.who == SELECT_ISUN)
        {
            //各コントローラーの取得
            DIJOYSTATE2 Controller4 = GetController(PLAYER_4);

            //選択肢を上へ(ゲームをつづける)
            if (g_PauseSelect == PAUSE_SELECT_QUIT_GAME && Controller4.lY < 0 || Controller4.rgdwPOV[0] == BUTTON_UP)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;
            }

            //選択肢を下へ(ゲームをやめる)
            else if (g_PauseSelect == PAUSE_SELECT_CONTINUE_GAME && Controller4.lY > 0 || Controller4.rgdwPOV[0] == BUTTON_DOWN)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_PauseSelect = PAUSE_SELECT_QUIT_GAME;
            }

            //A/Bボタンで選択
            if (GetJoypadTrigger(PLAYER_4, BUTTON_A) || GetJoypadTrigger(PLAYER_4, BUTTON_B))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //決定音

                //ゲームをつづける
                if (g_PauseSelect == PAUSE_SELECT_CONTINUE_GAME)
                {
                    //モードを取得
                    MODE mode = GetMode();

                    //ゲームなら
                    if (mode == MODE_GAME || mode == MODE_NEXT_GAME)
                    {
                        SetGameState(GAMESTATE_NORMAL);
                    }
                }

                //ゲームをやめる
                else
                {
                    //一人用で変わった設定を戻す
                    pSetting->nTimeLimit = pGame->nMemoryTimeLimit;
                    pSetting->bRandomStartPos = pGame->bMemoryRandomStartPos;
                    pSetting->bSuddenDeath = pGame->bMemorySuddenDeath;

                    StopSound(SOUND_LABEL_BGM_GAME);//ゲーム曲を停止
                    StopSound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //サドンデスの音楽を停止
                    //タイトルへ
                    SetFade(MODE_TITLE);
                }
            }

            //スタートボタンでゲームに戻る
            else if (GetJoypadTrigger(PLAYER_4, BUTTON_START))
            {
                //モードを取得
                MODE mode = GetMode();

                //ゲームなら
                if (mode == MODE_GAME || mode == MODE_NEXT_GAME)
                {
                    g_PauseSelect = PAUSE_SELECT_CONTINUE_GAME;  //カーソルをゲームをつづけるに戻す
                    SetGameState(GAMESTATE_NORMAL); //ポーズ解除
                }
            }
        }

        //選択肢の透明度を変える
        switch (g_PauseSelect)
        {
        case PAUSE_SELECT_CONTINUE_GAME:

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

        case PAUSE_SELECT_QUIT_GAME:

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
// ポーズ終了処理
// Author:後藤慎之助
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
// ポーズ描画処理
// Author:後藤慎之助
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

//=============================================================================
// ポーズ状況の取得
// Author:後藤慎之助
//=============================================================================
PAUSE*GetPause(void)
{
    return &g_Pause;
}