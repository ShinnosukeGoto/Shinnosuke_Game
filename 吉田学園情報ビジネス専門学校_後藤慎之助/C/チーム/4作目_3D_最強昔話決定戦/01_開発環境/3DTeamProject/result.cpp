//=============================================================================
//
// 最強昔話決定戦の処理 [result.cpp] (結果発表)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "result.h"
#include "input.h"
#include "game.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "controller.h"
#include "score.h"
#include "setting.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_apTextureResult[RESULTSTATE_MAX] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//頂点バッファへのポインタ

SELECT g_Winner;    //誰が勝ったか

//=============================================================================
// リザルト初期化処理
// Author:後藤慎之助
//=============================================================================
void InitResult(void)
{
    //スコアの初期化(得点は初期化しない)
    InitScore();

    //変数宣言
    VERTEX_2D *pVtx = NULL;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //ゲームの状況を取得
    GAME*pGame = GetGame();
    g_Winner = pGame->winner;   //勝者を結びつける

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * 1,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffResult,//変数名が変わると変更が必要
        NULL);

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result000.png", &g_apTextureResult[RESULTSTATE_GAMEOVER]);	//ゲームオーバー
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result001.png", &g_apTextureResult[RESULTSTATE_WIN1]);	    //プレイヤー1の勝ち
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result002.png", &g_apTextureResult[RESULTSTATE_WIN2]);	    //プレイヤー2の勝ち
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result003.png", &g_apTextureResult[RESULTSTATE_WIN3]);	    //プレイヤー3の勝ち
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result004.png", &g_apTextureResult[RESULTSTATE_WIN4]);	    //プレイヤー4の勝ち
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result005.png", &g_apTextureResult[RESULTSTATE_GAMECLEAR]);	//ゲームクリア
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/result006.png", &g_apTextureResult[RESULTSTATE_PERFECT]);	//パーフェクト

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

                                                   //頂点座標の設定（右回りで設定する）
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //rhwの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    //頂点カラーの設定
    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

    //テクスチャの座標
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    //頂点データアンロック
    g_pVtxBuffResult->Unlock();

    PlaySound(SOUND_LABEL_BGM_RESULT);	//リザルトのBGM
}
//=============================================================================
// リザルト更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateResult(void)
{
    //スコアの更新
    UpdateScore();

    int nFade = GetFade();	//フェードの情報を獲得

    if (nFade == FADE_NONE)
    {
        //設定を取得
        SETTING *pSetting = GetSetting();

        //バトルロイヤルなら
        if (pSetting->nMaxPlayer >= 2)
        {

            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_X) || GetJoypadTrigger(PLAYER_1, BUTTON_Y) || GetJoypadTrigger(PLAYER_1, BUTTON_START)
                || GetJoypadTrigger(PLAYER_2, BUTTON_A) || GetJoypadTrigger(PLAYER_2, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_2, BUTTON_X) || GetJoypadTrigger(PLAYER_2, BUTTON_Y) || GetJoypadTrigger(PLAYER_2, BUTTON_START)
                || GetJoypadTrigger(PLAYER_3, BUTTON_A) || GetJoypadTrigger(PLAYER_3, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_3, BUTTON_X) || GetJoypadTrigger(PLAYER_3, BUTTON_Y) || GetJoypadTrigger(PLAYER_3, BUTTON_START)
                || GetJoypadTrigger(PLAYER_4, BUTTON_A) || GetJoypadTrigger(PLAYER_4, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_4, BUTTON_X) || GetJoypadTrigger(PLAYER_4, BUTTON_Y) || GetJoypadTrigger(PLAYER_4, BUTTON_START))
            {
                StopSound(SOUND_LABEL_BGM_RESULT);	//リザルトのBGM
                PlaySound(SOUND_LABEL_SE_DECISION);	//決定音
                SetFade(MODE_TITLE);//タイトル画面に移動
            }
        }

        //ストーリーなら
        else
        {
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_X) || GetJoypadTrigger(PLAYER_1, BUTTON_Y) || GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);	//決定音
                SetFade(MODE_RANKING);//ランキング画面に移動
            }
        }
    }
}

//=============================================================================
// リザルト終了処理
// Author:後藤慎之助
//=============================================================================
void UninitResult(void)
{
    //スコアの終了処理
    UninitScore();

    //テクスチャの開放
    for (int nCntResult = 0; nCntResult < RESULTSTATE_MAX; nCntResult++)
    {
        if (g_apTextureResult[nCntResult] != NULL)
        {
            g_apTextureResult[nCntResult]->Release();
            g_apTextureResult[nCntResult] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffResult != NULL)
    {
        g_pVtxBuffResult->Release();
        g_pVtxBuffResult = NULL;
    }
}

//=============================================================================
// リザルト描画処理
// Author:後藤慎之助
//=============================================================================
void DrawResult(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

    //ゲームオーバー
    if (g_Winner == SELECT_MAX)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_GAMEOVER]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //1Pの勝利
    else if (g_Winner == SELECT_MOMO)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_WIN1]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //2Pの勝利
    else if (g_Winner == SELECT_URA)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_WIN2]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //3Pの勝利
    else if (g_Winner == SELECT_KIN)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_WIN3]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //4Pの勝利
    else if (g_Winner == SELECT_ISUN)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_WIN4]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //ゲームクリア
    else if (g_Winner == SELECT_BOSS)
    {
        //スコアが18万点を超えたら
        int nScore = GetScore();
        if (nScore > 180000)
        {
            //テクスチャの設定
            pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_PERFECT]);
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
        }
        //スコアが18万点以下なら
        else
        {
            //テクスチャの設定
            pDevice->SetTexture(0, g_apTextureResult[RESULTSTATE_GAMECLEAR]);
            //ポリゴンの描画
            pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
        }
    }

    //設定を取得
    SETTING *pSetting = GetSetting();

    //ストーリーなら
    if (pSetting->nMaxPlayer == 1)
    {
        //スコアの描画
        DrawScore();
    }
}