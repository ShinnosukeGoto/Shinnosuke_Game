//=============================================================================
//
// 最強昔話決定戦の処理 [menu.cpp] (メニュー選択)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "input.h"
#include "game.h"
#include "menu.h"
#include "fade.h"
#include "sound.h"
#include "manual.h"
#include "controller.h"
#include "setting.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureMenu[MENU_UI] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMenu = NULL;//頂点バッファへのポインタ

MENU g_Menu;	//メニュー選択

float g_fMoveMenuBgX;   //メニュー背景が動く(横)
float g_fMoveMenuBgY;   //メニュー背景が動く(縦)
//=============================================================================
// メニュー初期化処理
// Author:後藤慎之助
//=============================================================================
void InitMenu(void)
{
    //変数宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //グローバル変数の初期化
    g_Menu = MENU_GAME;	//最初のカーソルはゲーム
    g_fMoveMenuBgX = 0.0f;
    g_fMoveMenuBgY = 0.0f;

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu000.png", &g_pTextureMenu[0]);	//背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu001.png", &g_pTextureMenu[1]);	//遊ぶ
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu002.png", &g_pTextureMenu[2]);	//設定
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu003.png", &g_pTextureMenu[3]);	//ランキング
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/menu004.png", &g_pTextureMenu[4]);	//EXIT

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX*MENU_UI,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffMenu,//変数名が変わると変更が必要
        NULL);
    VERTEX_2D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //頂点座標の設定（右回りで設定する）
    //メニューの背景
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
    //ゲーム
    pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
    pVtx[6].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    //設定
    pVtx[8].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, 0.0f, 0.0f);
    pVtx[9].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[10].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[11].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
    //ランキング
    pVtx[12].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[14].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0.0f);
    //EXIT
    pVtx[16].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[17].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT / 2, 0.0f);
    pVtx[18].pos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT, 0.0f);
    pVtx[19].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
    pVtx[12].rhw = 1.0f;
    pVtx[13].rhw = 1.0f;
    pVtx[14].rhw = 1.0f;
    pVtx[15].rhw = 1.0f;
    pVtx[16].rhw = 1.0f;
    pVtx[17].rhw = 1.0f;
    pVtx[18].rhw = 1.0f;
    pVtx[19].rhw = 1.0f;

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

    pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

    pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

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
    pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
    pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);

    //頂点データアンロック
    g_pVtxBuffMenu->Unlock();

}

//=============================================================================
// メニュー更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateMenu(void)
{
    VERTEX_2D *pVtx = NULL;	//頂点情報へのポインタ
    int nFade = GetFade();	//フェードの情報を獲得

    //コントローラーの情報を取得する
    DIJOYSTATE2 Controller = GetController(PLAYER_1);

    //頂点バッファをロック
    g_pVtxBuffMenu->Lock(0, 0, (void**)&pVtx, 0);

    //背景を動かす
    g_fMoveMenuBgX++;
    g_fMoveMenuBgY++;
    if (g_fMoveMenuBgX == 1000)
    {
        g_fMoveMenuBgX = 0;
    }

    if (g_fMoveMenuBgY == 1000)
    {
        g_fMoveMenuBgY = 0;
    }
    pVtx[0].tex = D3DXVECTOR2(g_fMoveMenuBgX *0.001f, g_fMoveMenuBgY *0.001f);
    pVtx[1].tex = D3DXVECTOR2(g_fMoveMenuBgX *0.001f + 0.999f, g_fMoveMenuBgY *0.001f);
    pVtx[2].tex = D3DXVECTOR2(g_fMoveMenuBgX *0.001f, g_fMoveMenuBgY *0.001f + 0.999f);
    pVtx[3].tex = D3DXVECTOR2(g_fMoveMenuBgX *0.001f + 0.999f, g_fMoveMenuBgY *0.001f + 0.999f);

    //設定を、遊ぶに反映
    SETTING *pSetting = GetSetting();
    if (pSetting->nMaxPlayer == 1)
    {
        pVtx[4].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[6].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
    }
    else
    {
        pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[5].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
    }

    //メニュー選択
    switch (g_Menu)
    {
    case MENU_GAME:
        //ゲームが明るい
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        if (nFade == FADE_NONE)
        {
            //→キーが押された（右移動,設定選択）
            if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 || Controller.rgdwPOV[0] == BUTTON_RIGHT)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Menu = MENU_SETTING;
            }

            //↓キーが押された（下移動,ランキング選択）
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 || Controller.rgdwPOV[0] == BUTTON_DOWN)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Menu = MENU_RANKING;
            }

            //Enterキーを押した時
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);	//決定音
                SetFade(MODE_MANUAL);   //遊び方画面に移動
            }
        }

        break;

    case MENU_SETTING:
        //設定が明るい
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        if (nFade == FADE_NONE)
        {
            //←キーが押された（左移動,ゲーム選択）
            if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 || Controller.rgdwPOV[0] == BUTTON_LEFT)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Menu = MENU_GAME;
            }

            //↓キーが押された（下移動,EXIT選択）
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 || Controller.rgdwPOV[0] == BUTTON_DOWN)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Menu = MENU_EXIT;
            }

            //Enterキーを押した時
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);	//決定音
                SetFade(MODE_SETTING);//設定画面に移動
            }
        }

        break;

    case MENU_RANKING:
        //ランキングが明るい
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        if (nFade == FADE_NONE)
        {
            //→キーが押された（右移動,EXIT選択）
            if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 || Controller.rgdwPOV[0] == BUTTON_RIGHT)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Menu = MENU_EXIT;
            }

            //↑キーが押された（上移動,ゲーム選択）
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 || Controller.rgdwPOV[0] == BUTTON_UP)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Menu = MENU_GAME;
            }

            //Enterキーを押した時
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);	//決定音
                SetFade(MODE_RANKING);//ランキング画面に移動
            }
        }

        break;

    case MENU_EXIT:
        //EXITが明るい
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

        pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        if (nFade == FADE_NONE)
        {
            //←キーが押された（左移動,ランキング選択）
            if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 || Controller.rgdwPOV[0] == BUTTON_LEFT)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Menu = MENU_RANKING;
            }

            //↑キーが押された（上移動,設定選択）
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 || Controller.rgdwPOV[0] == BUTTON_UP)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Menu = MENU_SETTING;
            }

            //Enterキーを押した時
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
                GetJoypadTrigger(PLAYER_1, BUTTON_START))
            {
                StopSound(SOUND_LABEL_BGM_MENU);	//メニューのBGM
                PlaySound(SOUND_LABEL_SE_DECISION);	//決定音
                SetFade(MODE_EXIT);//ゲーム全体を終了
            }
        }

        break;
    }

    //頂点データをアンロックする
    g_pVtxBuffMenu->Unlock();

}

//=============================================================================
// メニュー終了処理
// Author:後藤慎之助
//=============================================================================
void UninitMenu(void)
{
    //テクスチャの開放
    for (int nCntMenu = 0; nCntMenu < MENU_UI; nCntMenu++)
    {
        if (g_pTextureMenu[nCntMenu] != NULL)
        {
            g_pTextureMenu[nCntMenu]->Release();
            g_pTextureMenu[nCntMenu] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffMenu != NULL)
    {
        g_pVtxBuffMenu->Release();
        g_pVtxBuffMenu = NULL;
    }
}
//=============================================================================
// メニュー描画処理
// Author:後藤慎之助
//=============================================================================
void DrawMenu(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffMenu, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

    for (int nCntMenu = 0; nCntMenu < MENU_UI; nCntMenu++)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_pTextureMenu[nCntMenu]);

        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntMenu, NUM_POLYGON);
    }
}

//=============================================================================
// メニュー選択の取得
// Author:後藤慎之助
//=============================================================================
MENU GetMenu(void)
{
    return g_Menu;
}