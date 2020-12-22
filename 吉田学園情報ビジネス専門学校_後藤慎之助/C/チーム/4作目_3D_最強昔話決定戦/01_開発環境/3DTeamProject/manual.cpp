//=============================================================================
//
// 最強昔話決定戦の処理 [manual.cpp] (遊び方)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "input.h"
#include "game.h"
#include "manual.h"
#include "fade.h"
#include "controller.h"
#include "sound.h"
#include "setting.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureManual = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManual = NULL;//頂点バッファへのポインタ

bool g_bUseHiddenCommand;  //隠しコマンドの使用判定
bool g_bHiddenCommand1L;   //隠しコマンド1
bool g_bHiddenCommand2R;   //隠しコマンド2
bool g_bHiddenCommand3L;   //隠しコマンド3
bool g_bHiddenCommand4R;   //隠しコマンド4
bool g_bHiddenCommand5X;   //隠しコマンド5
bool g_bHiddenCommand6X;   //隠しコマンド6
bool g_bHiddenCommand7Y;   //隠しコマンド7

bool g_bUseHiddenCommand2;  //隠しコマンドの使用判定
bool g_bHiddenCommand1L2;   //隠しコマンド1
bool g_bHiddenCommand2R2;   //隠しコマンド2
bool g_bHiddenCommand3L2;   //隠しコマンド3
bool g_bHiddenCommand4R2;   //隠しコマンド4
bool g_bHiddenCommand5X2;   //隠しコマンド5
bool g_bHiddenCommand6X2;   //隠しコマンド6
bool g_bHiddenCommand7Y2;   //隠しコマンド7

bool g_bUseHiddenCommand3;  //隠しコマンドの使用判定
bool g_bHiddenCommand1L3;   //隠しコマンド1
bool g_bHiddenCommand2R3;   //隠しコマンド2
bool g_bHiddenCommand3L3;   //隠しコマンド3
bool g_bHiddenCommand4R3;   //隠しコマンド4
bool g_bHiddenCommand5X3;   //隠しコマンド5
bool g_bHiddenCommand6X3;   //隠しコマンド6
bool g_bHiddenCommand7Y3;   //隠しコマンド7

bool g_bUseHiddenCommand4;  //隠しコマンドの使用判定
bool g_bHiddenCommand1L4;   //隠しコマンド1
bool g_bHiddenCommand2R4;   //隠しコマンド2
bool g_bHiddenCommand3L4;   //隠しコマンド3
bool g_bHiddenCommand4R4;   //隠しコマンド4
bool g_bHiddenCommand5X4;   //隠しコマンド5
bool g_bHiddenCommand6X4;   //隠しコマンド6
bool g_bHiddenCommand7Y4;   //隠しコマンド7

//=============================================================================
// マニュアル初期化処理
// Author:後藤慎之助
//=============================================================================
void InitManual(void)
{
    //変数宣言
    VERTEX_2D *pVtx = NULL;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //隠しコマンドの初期化
    g_bUseHiddenCommand = false;
    g_bHiddenCommand1L = false;
    g_bHiddenCommand2R = false;
    g_bHiddenCommand3L = false;
    g_bHiddenCommand4R = false;
    g_bHiddenCommand5X = false;
    g_bHiddenCommand6X = false;
    g_bHiddenCommand7Y = false;

    g_bUseHiddenCommand2 = false;
    g_bHiddenCommand1L2 = false;
    g_bHiddenCommand2R2 = false;
    g_bHiddenCommand3L2 = false;
    g_bHiddenCommand4R2 = false;
    g_bHiddenCommand5X2 = false;
    g_bHiddenCommand6X2 = false;
    g_bHiddenCommand7Y2 = false;

    g_bUseHiddenCommand3 = false;
    g_bHiddenCommand1L3 = false;
    g_bHiddenCommand2R3 = false;
    g_bHiddenCommand3L3 = false;
    g_bHiddenCommand4R3 = false;
    g_bHiddenCommand5X3 = false;
    g_bHiddenCommand6X3 = false;
    g_bHiddenCommand7Y3 = false;

    g_bUseHiddenCommand4 = false;
    g_bHiddenCommand1L4 = false;
    g_bHiddenCommand2R4 = false;
    g_bHiddenCommand3L4 = false;
    g_bHiddenCommand4R4 = false;
    g_bHiddenCommand5X4 = false;
    g_bHiddenCommand6X4 = false;
    g_bHiddenCommand7Y4 = false;

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/manual000.png", &g_pTextureManual);

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffManual,//変数名が変わると変更が必要
        NULL);

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

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
    g_pVtxBuffManual->Unlock();
}

//=============================================================================
// マニュアル更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateManual(void)
{
    int nFade = GetFade();	            //フェードの情報を獲得
    SETTING *pSetting = GetSetting();   //設定の情報を取得

    //バトルロイヤルなら、隠しコマンドが使える
    if (pSetting->nMaxPlayer >= 2)
    {
        //====================================
        //プレイヤー1
        //====================================
        //8つ目Y
        if (g_bHiddenCommand7Y == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_Y))
            {
                g_bUseHiddenCommand = true; //隠しコマンド適用
            }
        }

        //7つ目Y
        if (g_bHiddenCommand6X == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_Y))
            {
                g_bHiddenCommand7Y = true;
            }
        }

        //6つ目X
        if (g_bHiddenCommand5X == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_X))
            {
                g_bHiddenCommand6X = true;
            }
        }

        //5つ目X
        if (g_bHiddenCommand4R == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_X))
            {
                g_bHiddenCommand5X = true;
            }
        }

        //4つ目R
        if (g_bHiddenCommand3L == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_R1) || GetJoypadTrigger(PLAYER_1, BUTTON_R2))
            {
                g_bHiddenCommand4R = true;
            }
        }

        //3つ目L
        if (g_bHiddenCommand2R == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_L1) || GetJoypadTrigger(PLAYER_1, BUTTON_L2))
            {
                g_bHiddenCommand3L = true;
            }
        }

        //2つ目R
        if (g_bHiddenCommand1L == true)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_R1) || GetJoypadTrigger(PLAYER_1, BUTTON_R2))
            {
                g_bHiddenCommand2R = true;
            }
        }

        //1つ目L
        if (g_bHiddenCommand1L == false)
        {
            if (GetJoypadTrigger(PLAYER_1, BUTTON_L1) || GetJoypadTrigger(PLAYER_1, BUTTON_L2))
            {
                g_bHiddenCommand1L = true;
            }
        }

        //====================================
        //プレイヤー2
        //====================================
        //8つ目Y
        if (g_bHiddenCommand7Y2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_Y))
            {
                g_bUseHiddenCommand2 = true; //隠しコマンド適用
            }
        }

        //7つ目Y
        if (g_bHiddenCommand6X2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_Y))
            {
                g_bHiddenCommand7Y2 = true;
            }
        }

        //6つ目X
        if (g_bHiddenCommand5X2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_X))
            {
                g_bHiddenCommand6X2 = true;
            }
        }

        //5つ目X
        if (g_bHiddenCommand4R2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_X))
            {
                g_bHiddenCommand5X2 = true;
            }
        }

        //4つ目R
        if (g_bHiddenCommand3L2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_R1) || GetJoypadTrigger(PLAYER_2, BUTTON_R2))
            {
                g_bHiddenCommand4R2 = true;
            }
        }

        //3つ目L
        if (g_bHiddenCommand2R2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_L1) || GetJoypadTrigger(PLAYER_2, BUTTON_L2))
            {
                g_bHiddenCommand3L2 = true;
            }
        }

        //2つ目R
        if (g_bHiddenCommand1L2 == true)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_R1) || GetJoypadTrigger(PLAYER_2, BUTTON_R2))
            {
                g_bHiddenCommand2R2 = true;
            }
        }

        //1つ目L
        if (g_bHiddenCommand1L2 == false)
        {
            if (GetJoypadTrigger(PLAYER_2, BUTTON_L1) || GetJoypadTrigger(PLAYER_2, BUTTON_L2))
            {
                g_bHiddenCommand1L2 = true;
            }
        }

        //====================================
        //プレイヤー3
        //====================================
        //8つ目Y
        if (g_bHiddenCommand7Y3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_Y))
            {
                g_bUseHiddenCommand3 = true; //隠しコマンド適用
            }
        }

        //7つ目Y
        if (g_bHiddenCommand6X3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_Y))
            {
                g_bHiddenCommand7Y3 = true;
            }
        }

        //6つ目X
        if (g_bHiddenCommand5X3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_X))
            {
                g_bHiddenCommand6X3 = true;
            }
        }

        //5つ目X
        if (g_bHiddenCommand4R3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_X))
            {
                g_bHiddenCommand5X3 = true;
            }
        }

        //4つ目R
        if (g_bHiddenCommand3L3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_R1) || GetJoypadTrigger(PLAYER_3, BUTTON_R2))
            {
                g_bHiddenCommand4R3 = true;
            }
        }

        //3つ目L
        if (g_bHiddenCommand2R3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_L1) || GetJoypadTrigger(PLAYER_3, BUTTON_L2))
            {
                g_bHiddenCommand3L3 = true;
            }
        }

        //2つ目R
        if (g_bHiddenCommand1L3 == true)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_R1) || GetJoypadTrigger(PLAYER_3, BUTTON_R2))
            {
                g_bHiddenCommand2R3 = true;
            }
        }

        //1つ目L
        if (g_bHiddenCommand1L3 == false)
        {
            if (GetJoypadTrigger(PLAYER_3, BUTTON_L1) || GetJoypadTrigger(PLAYER_3, BUTTON_L2))
            {
                g_bHiddenCommand1L3 = true;
            }
        }

        //====================================
        //プレイヤー4
        //====================================
        //8つ目Y
        if (g_bHiddenCommand7Y4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_Y))
            {
                g_bUseHiddenCommand4 = true; //隠しコマンド適用
            }
        }

        //7つ目Y
        if (g_bHiddenCommand6X4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_Y))
            {
                g_bHiddenCommand7Y4 = true;
            }
        }

        //6つ目X
        if (g_bHiddenCommand5X4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_X))
            {
                g_bHiddenCommand6X4 = true;
            }
        }

        //5つ目X
        if (g_bHiddenCommand4R4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_X))
            {
                g_bHiddenCommand5X4 = true;
            }
        }

        //4つ目R
        if (g_bHiddenCommand3L4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_R1) || GetJoypadTrigger(PLAYER_4, BUTTON_R2))
            {
                g_bHiddenCommand4R4 = true;
            }
        }

        //3つ目L
        if (g_bHiddenCommand2R4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_L1) || GetJoypadTrigger(PLAYER_4, BUTTON_L2))
            {
                g_bHiddenCommand3L4 = true;
            }
        }

        //2つ目R
        if (g_bHiddenCommand1L4 == true)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_R1) || GetJoypadTrigger(PLAYER_4, BUTTON_R2))
            {
                g_bHiddenCommand2R4 = true;
            }
        }

        //1つ目L
        if (g_bHiddenCommand1L4 == false)
        {
            if (GetJoypadTrigger(PLAYER_4, BUTTON_L1) || GetJoypadTrigger(PLAYER_4, BUTTON_L2))
            {
                g_bHiddenCommand1L4 = true;
            }
        }
    }

    //Enterキーを押した時
    if (nFade == FADE_NONE)
    {
        if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
            || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_START))
        {
            StopSound(SOUND_LABEL_BGM_MENU);	//メニューのBGM
            PlaySound(SOUND_LABEL_SE_DECISION);	//決定音
            SetFade(MODE_GAME);//ゲーム画面に移動
        }
    }

}

//=============================================================================
// マニュアル終了処理
// Author:後藤慎之助
//=============================================================================
void UninitManual(void)
{
    //テクスチャの開放
    if (g_pTextureManual != NULL)
    {
        g_pTextureManual->Release();
        g_pTextureManual = NULL;
    }
    //頂点バッファの開放
    if (g_pVtxBuffManual != NULL)
    {
        g_pVtxBuffManual->Release();
        g_pVtxBuffManual = NULL;
    }
}

//=============================================================================
// マニュアル描画処理
// Author:後藤慎之助
//=============================================================================
void DrawManual(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffManual, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

                                   //テクスチャの設定
    pDevice->SetTexture(0, g_pTextureManual);

    //ポリゴンの描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
}

//=============================================================================
// 隠しコマンド処理
// Author:後藤慎之助
//=============================================================================
bool GetHiddenCommand(void)
{
    return g_bUseHiddenCommand;
}

//=============================================================================
// 隠しコマンド2処理
// Author:後藤慎之助
//=============================================================================
bool GetHiddenCommand2(void)
{
    return g_bUseHiddenCommand2;
}

//=============================================================================
// 隠しコマンド3処理
// Author:後藤慎之助
//=============================================================================
bool GetHiddenCommand3(void)
{
    return g_bUseHiddenCommand3;
}

//=============================================================================
// 隠しコマンド4処理
// Author:後藤慎之助
//=============================================================================
bool GetHiddenCommand4(void)
{
    return g_bUseHiddenCommand4;
}