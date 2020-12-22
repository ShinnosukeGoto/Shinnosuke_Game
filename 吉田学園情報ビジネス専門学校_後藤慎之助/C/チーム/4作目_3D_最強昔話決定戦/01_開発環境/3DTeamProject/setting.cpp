//=============================================================================
//
// 最強昔話決定戦の処理 [setting.cpp] (設定選択)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "input.h"
#include "game.h"
#include "setting.h"
#include "fade.h"
#include "sound.h"
#include "manual.h"
#include "controller.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureSetting[SETTING_UI] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSetting = NULL;//頂点バッファへのポインタ

SETTING g_Setting;	//設定選択

//カーソルを動かせるまでのカウンター
int g_nCntCursor;   //カーソルを動かせるまでの時間
bool g_bMoveCursor; //カーソルを動かせる

//=============================================================================
// 設定初期化処理
// Author:後藤慎之助
//=============================================================================
void InitSetting(void)
{
    //変数宣言
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //構造体の初期化(最初のカーソルのみ初期化する)
    g_Setting.type = SETTINGTYPE_PLAYER;

    //グローバル変数の初期化
    g_nCntCursor = 0;
    g_bMoveCursor = true;

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting000.png", &g_pTextureSetting[0]);	//背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting001.png", &g_pTextureSetting[1]);	//プレイヤー
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting004.png", &g_pTextureSetting[2]);	//ラウンド
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting002.png", &g_pTextureSetting[3]);	//制限時間
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting003.png", &g_pTextureSetting[4]);	//開始位置
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting005.png", &g_pTextureSetting[5]);	//サドンデス
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting006.png", &g_pTextureSetting[6]);	//ステージ
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting007.png", &g_pTextureSetting[7]);	//プレイヤーの選択肢
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting013.png", &g_pTextureSetting[8]);	//ラウンドの選択肢
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting008.png", &g_pTextureSetting[9]);	//制限時間の選択肢
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting009.png", &g_pTextureSetting[10]);	//開始位置の選択肢
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting010.png", &g_pTextureSetting[11]);	//サドンデスの選択肢
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting014.png", &g_pTextureSetting[12]);	//ステージの選択肢
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/OK.png", &g_pTextureSetting[13]);	        //OK
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting011.png", &g_pTextureSetting[14]);	//左矢印
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/setting012.png", &g_pTextureSetting[15]);	//右矢印

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX*SETTING_UI,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffSetting,//変数名が変わると変更が必要
        NULL);

    VERTEX_2D *pVtx = NULL;//頂点情報のポインタ

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffSetting->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //頂点座標の設定（右回りで設定する）
    //設定の背景
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //プレイヤー
    pVtx[4].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_PLAYER_POS;
    pVtx[5].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_PLAYER_POS;
    pVtx[6].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_PLAYER_POS;
    pVtx[7].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_PLAYER_POS;

    //ラウンド
    pVtx[8].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_ROUND_POS;
    pVtx[9].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_ROUND_POS;
    pVtx[10].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_ROUND_POS;
    pVtx[11].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_ROUND_POS;

    //制限時間
    pVtx[12].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
    pVtx[13].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
    pVtx[14].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
    pVtx[15].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;

    //開始位置
    pVtx[16].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
    pVtx[17].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
    pVtx[18].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
    pVtx[19].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;

    //サドンデス
    pVtx[20].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
    pVtx[21].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
    pVtx[22].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
    pVtx[23].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;

    //ステージ
    pVtx[24].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
    pVtx[25].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
    pVtx[26].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
    pVtx[27].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;

    //プレイヤー数の選択肢
    pVtx[28].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_PLAYER_POS;
    pVtx[29].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_PLAYER_POS;
    pVtx[30].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_PLAYER_POS;
    pVtx[31].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_PLAYER_POS;

    //ラウンドの選択肢
    pVtx[32].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_ROUND_POS;
    pVtx[33].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_ROUND_POS;
    pVtx[34].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_ROUND_POS;
    pVtx[35].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_ROUND_POS;

    //制限時間の選択肢
    pVtx[36].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
    pVtx[37].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
    pVtx[38].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
    pVtx[39].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;

    //開始位置の選択肢
    pVtx[40].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
    pVtx[41].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
    pVtx[42].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
    pVtx[43].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;

    //サドンデスの選択肢
    pVtx[44].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
    pVtx[45].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
    pVtx[46].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
    pVtx[47].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;

    //ステージの選択肢
    pVtx[48].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
    pVtx[49].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
    pVtx[50].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
    pVtx[51].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;

    //OK
    pVtx[52].pos = D3DXVECTOR3(-OK_SIZE_X, -OK_SIZE_Y, 0.0f) + OK_POS;
    pVtx[53].pos = D3DXVECTOR3(OK_SIZE_X, -OK_SIZE_Y, 0.0f) + OK_POS;
    pVtx[54].pos = D3DXVECTOR3(-OK_SIZE_X, OK_SIZE_Y, 0.0f) + OK_POS;
    pVtx[55].pos = D3DXVECTOR3(OK_SIZE_X, OK_SIZE_Y, 0.0f) + OK_POS;

    //左矢印
    pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

    //右矢印
    pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
    pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

    //各種共通の設定
    for (int nCntSetting = 0; nCntSetting < SETTING_UI; nCntSetting++)
    {
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

        //テクスチャの座標の設定
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

        pVtx += 4;
    }

    //頂点データアンロック
    g_pVtxBuffSetting->Unlock();
}
//=============================================================================
// 設定更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateSetting(void)
{
    VERTEX_2D *pVtx = NULL;	//頂点情報へのポインタ
    int nFade = GetFade();	//フェードの情報を獲得

    //コントローラーの情報を取得する
    DIJOYSTATE2 Controller = GetController(PLAYER_1);

    //頂点バッファをロック
    g_pVtxBuffSetting->Lock(0, 0, (void**)&pVtx, 0);

    //十字キーやスティックでカーソルを動かせるまでのカウンター
    if (g_bMoveCursor == false)
    {
        g_nCntCursor++;
        if (g_nCntCursor > COUNT_MOVE_CURSOR)
        {
            g_nCntCursor = 0;
            g_bMoveCursor = true;   //動かせる
        }
    }

    //OKの透明度を下げる
    pVtx[52].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[53].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[54].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    pVtx[55].col = D3DCOLOR_RGBA(255, 255, 255, 55);

    //設定選択
    switch (g_Setting.type)
    {
        //===================
        //人数選択
        //===================
    case SETTINGTYPE_PLAYER:

        if (nFade == FADE_NONE)
        {
            //↑キーが押された（上移動,OK選択へ）
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_OK;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //↓キーが押された（下移動,ラウンド選択へ）
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1,BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_ROUND;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //←キーが押された
            if (g_Setting.nMaxPlayer > 1)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.nMaxPlayer--;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }

            //→キーが押された
            if (g_Setting.nMaxPlayer < 4)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.nMaxPlayer++;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }

        }

        //一番左の選択肢でないなら
        if (g_Setting.nMaxPlayer != 1)
        {
            //左カーソルの位置
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + LEFT_CURSOR_POS;
        }
        else
        {
            //左カーソルの位置
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        //一番右の選択肢でないなら
        if (g_Setting.nMaxPlayer != 4)
        {
            //右カーソルの位置
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_PLAYER_POS + RIGHT_CURSOR_POS;
        }
        else
        {
            //右カーソルの位置
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        break;

        //===================
        //ラウンド選択/難易度選択
        //===================
    case SETTINGTYPE_ROUND:

        if (nFade == FADE_NONE)
        {
            //↑キーが押された（上移動,プレイヤー選択へ）
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_PLAYER;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //みんなで遊ぶなら
            if (g_Setting.nMaxPlayer != 1)
            {
                //↓キーが押された（下移動,制限時間選択へ）
                if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                    || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                    || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                    g_Setting.type = SETTINGTYPE_TIME;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }

                //←キーが押された
                if (g_Setting.nMaxRound > 1)
                {
                    if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                        || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                    {
                        PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                        g_Setting.nMaxRound--;

                        g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                    }
                }

                //→キーが押された
                if (g_Setting.nMaxRound < 3)
                {
                    if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                        || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                    {
                        PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                        g_Setting.nMaxRound++;

                        g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                    }
                }
            }

            //ひとりで遊ぶなら
            else
            {
                //↓キーが押された（下移動,OK選択へ）
                if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                    || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                    || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                    g_Setting.type = SETTINGTYPE_OK;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }

                //←キーが押された
                if (g_Setting.nLevel > SETTINGLEVEL_NORMAL)
                {
                    if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                        || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                    {
                        PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                        g_Setting.nLevel--;

                        g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                    }
                }

                //→キーが押された
                if (g_Setting.nLevel < SETTINGLEVEL_EXPERT)
                {
                    if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                        || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                    {
                        PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                        g_Setting.nLevel++;

                        g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                    }
                }
            }
        }

        //みんなで遊ぶなら
        if (g_Setting.nMaxPlayer != 1)
        {
            //一番左の選択肢でないなら
            if (g_Setting.nMaxRound != 1)
            {
                //左カーソルの位置
                pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
            }
            else
            {
                //左カーソルの位置
                pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            }

            //一番右の選択肢でないなら
            if (g_Setting.nMaxRound != 3)
            {
                //右カーソルの位置
                pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
            }
            else
            {
                //右カーソルの位置
                pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            }
        }

        //ひとりで遊ぶなら
        else
        {
            //一番左の選択肢でないなら
            if (g_Setting.nLevel != SETTINGLEVEL_NORMAL)
            {
                //左カーソルの位置
                pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
                pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + LEFT_CURSOR_POS;
            }
            else
            {
                //左カーソルの位置
                pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            }

            //一番右の選択肢でないなら
            if (g_Setting.nLevel != SETTINGLEVEL_EXPERT)
            {
                //右カーソルの位置
                pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
                pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_ROUND_POS + RIGHT_CURSOR_POS;
            }
            else
            {
                //右カーソルの位置
                pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
                pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            }
        }

        break;

        //===================
        //制限時間選択
        //===================
    case SETTINGTYPE_TIME:

        if (nFade == FADE_NONE)
        {
            //↑キーが押された（上移動,ラウンド選択へ）
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_ROUND;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //↓キーが押された（下移動,開始位置選択へ）
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_START_POS;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //←キーが押された
            if (g_Setting.nTimeLimit > 3)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.nTimeLimit--;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }

            //→キーが押された
            if (g_Setting.nTimeLimit < 5)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.nTimeLimit++;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }
        }

        //一番左の選択肢でないなら
        if (g_Setting.nTimeLimit != 3)
        {
            //左カーソルの位置
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + LEFT_CURSOR_POS;
        }
        else
        {
            //左カーソルの位置
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        //一番右の選択肢でないなら
        if (g_Setting.nTimeLimit != 5)
        {
            //右カーソルの位置
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_TIME_LIMIT_POS + RIGHT_CURSOR_POS;
        }
        else
        {
            //右カーソルの位置
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }


        break;

        //===================
        //開始位置選択
        //===================
    case SETTINGTYPE_START_POS:

        if (nFade == FADE_NONE)
        {
            //↑キーが押された（上移動,制限時間選択へ）
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_TIME;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //↓キーが押された（下移動,サドンデス選択へ）
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_SUDDEN_DEATH;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //←キーが押された
            if (g_Setting.bRandomStartPos != false)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.bRandomStartPos = false;  //固定

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }

            //→キーが押された
            if (g_Setting.bRandomStartPos != true)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.bRandomStartPos = true;   //ランダム

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }
        }

        //左の選択肢
        if (g_Setting.bRandomStartPos == false)
        {
            //左矢印
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

            //右カーソルの位置
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + RIGHT_CURSOR_POS;
        }

        //右の選択肢
        else
        {
            //左カーソルの位置
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_RANDOM_START_POS + LEFT_CURSOR_POS;

            //右矢印
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        }

        break;

        //===================
        //サドンデス選択
        //===================
    case SETTINGTYPE_SUDDEN_DEATH:

        if (nFade == FADE_NONE)
        {
            //↑キーが押された（上移動,開始位置選択へ）
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_START_POS;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //↓キーが押された（下移動,ステージ選択へ）
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_STAGE;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //←キーが押された
            if (g_Setting.bSuddenDeath != true)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.bSuddenDeath = true;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }

            //→キーが押された
            if (g_Setting.bSuddenDeath != false)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.bSuddenDeath = false;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }
        }

        //左の選択肢
        if (g_Setting.bSuddenDeath == true)
        {
            //左矢印
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

            //右カーソルの位置
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + RIGHT_CURSOR_POS;
        }

        //右の選択肢
        else
        {
            //左カーソルの位置
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_SUDDEN_DEATH_POS + LEFT_CURSOR_POS;

            //右矢印
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        break;

        //===================
        //ステージ選択
        //===================
    case SETTINGTYPE_STAGE:

        if (nFade == FADE_NONE)
        {
            //↑キーが押された（上移動,サドンデス選択へ）
            if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_SUDDEN_DEATH;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //↓キーが押された（下移動,OK選択へ）
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || GetJoypadTrigger(PLAYER_1, BUTTON_B) || GetJoypadTrigger(PLAYER_1, BUTTON_A)
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_OK;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //←キーが押された
            if (g_Setting.nStage != SETTINGSTAGE_NORMAL)
            {
                if (GetKeyboardTrigger(DIK_LEFT) || Controller.lX < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_LEFT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.nStage--;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }

            //→キーが押された
            if (g_Setting.nStage != SETTINGSTAGE_RANDOM)
            {
                if (GetKeyboardTrigger(DIK_RIGHT) || Controller.lX > 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_RIGHT && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音

                    g_Setting.nStage++;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }
        }

        //一番左の選択肢でないなら
        if (g_Setting.nStage != SETTINGSTAGE_NORMAL)
        {
            //左カーソルの位置
            pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + LEFT_CURSOR_POS;
            pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + LEFT_CURSOR_POS;
            pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + LEFT_CURSOR_POS;
            pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + LEFT_CURSOR_POS;
        }
        else
        {
            //左矢印
            pVtx[56].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[57].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[58].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[59].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        //一番右の選択肢でないなら
        if (g_Setting.nStage != SETTINGSTAGE_RANDOM)
        {
            //右カーソルの位置
            pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + RIGHT_CURSOR_POS;
            pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + RIGHT_CURSOR_POS;
            pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + RIGHT_CURSOR_POS;
            pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + CHOICE_STAGE_POS + RIGHT_CURSOR_POS;
        }
        else
        {
            //右矢印
            pVtx[60].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[61].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[62].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
            pVtx[63].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        }

        break;

        //===================
        //OK選択
        //===================
    case SETTINGTYPE_OK:

        //選択中は、OKの透明度を上げる
        pVtx[52].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[53].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[54].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[55].col = D3DCOLOR_RGBA(255, 255, 255, 255);

        //左右カーソルを、通常とは逆の位置にして、選択していることをわかりやすくする
        //左カーソルの位置
        pVtx[56].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + OK_POS + RIGHT_CURSOR_POS;
        pVtx[57].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + OK_POS + RIGHT_CURSOR_POS;
        pVtx[58].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + OK_POS + RIGHT_CURSOR_POS;
        pVtx[59].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + OK_POS + RIGHT_CURSOR_POS;
        //右カーソルの位置
        pVtx[60].pos = D3DXVECTOR3(-CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + OK_POS + LEFT_CURSOR_POS;
        pVtx[61].pos = D3DXVECTOR3(CURSOR_SIZE, -CURSOR_SIZE, 0.0f) + OK_POS + LEFT_CURSOR_POS;
        pVtx[62].pos = D3DXVECTOR3(-CURSOR_SIZE, CURSOR_SIZE, 0.0f) + OK_POS + LEFT_CURSOR_POS;
        pVtx[63].pos = D3DXVECTOR3(CURSOR_SIZE, CURSOR_SIZE, 0.0f) + OK_POS + LEFT_CURSOR_POS;

        if (nFade == FADE_NONE)
        {

            //みんなで遊ぶなら
            if (g_Setting.nMaxPlayer != 1)
            {
                //↑キーが押された（上移動,ステージ選択へ）
                if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                    g_Setting.type = SETTINGTYPE_STAGE;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }

            //ひとりで遊ぶなら
            else
            {
                //↑キーが押された（上移動,ラウンド選択へ）
                if (GetKeyboardTrigger(DIK_UP) || Controller.lY < 0 && g_bMoveCursor == true
                    || Controller.rgdwPOV[0] == BUTTON_UP && g_bMoveCursor == true)
                {
                    PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                    g_Setting.type = SETTINGTYPE_ROUND;

                    g_bMoveCursor = false;   //カーソルを連続で動かせなくする
                }
            }

            //↓キーが押された（下移動,プレイ人数選択へ）
            if (GetKeyboardTrigger(DIK_DOWN) || Controller.lY > 0 && g_bMoveCursor == true
                || Controller.rgdwPOV[0] == BUTTON_DOWN && g_bMoveCursor == true)
            {
                PlaySound(SOUND_LABEL_SE_SELECT);   //選択音
                g_Setting.type = SETTINGTYPE_PLAYER;

                g_bMoveCursor = false;   //カーソルを連続で動かせなくする
            }

            //決定キーが押された
            if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_B)
                || GetJoypadTrigger(PLAYER_1, BUTTON_A))
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //決定音
                SetFade(MODE_MENU); //メニューへ戻る
            }

        }

        break;
    }

    //===========================================
    //選択肢のテクスチャの反映
    //===========================================
    //===================
    //プレイヤー選択
    //===================
    if (g_Setting.nMaxPlayer == 1)
    {
        //テクスチャの座標の設定
        pVtx[28].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[29].tex = D3DXVECTOR2(0.25f, 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[31].tex = D3DXVECTOR2(0.25f, 1.0f);
    }
    else if (g_Setting.nMaxPlayer == 2)
    {
        //テクスチャの座標の設定
        pVtx[28].tex = D3DXVECTOR2(0.25f, 0.0f);
        pVtx[29].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.25f, 1.0f);
        pVtx[31].tex = D3DXVECTOR2(0.5f, 1.0f);
    }
    else if (g_Setting.nMaxPlayer == 3)
    {
        //テクスチャの座標の設定
        pVtx[28].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[29].tex = D3DXVECTOR2(0.75f, 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[31].tex = D3DXVECTOR2(0.75f, 1.0f);
    }
    else if (g_Setting.nMaxPlayer == 4)
    {
        //テクスチャの座標の設定
        pVtx[28].tex = D3DXVECTOR2(0.75f, 0.0f);
        pVtx[29].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.75f, 1.0f);
        pVtx[31].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    //===================
    //ラウンド選択
    //===================
    //みんなで遊ぶなら
    if (g_Setting.nMaxPlayer != 1)
    {
        if (g_Setting.nMaxRound == 1)
        {
            //テクスチャの座標の設定
            pVtx[32].tex = D3DXVECTOR2(0.0f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(0.2f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.0f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(0.2f, 1.0f);
        }
        else if (g_Setting.nMaxRound == 2)
        {
            //テクスチャの座標の設定
            pVtx[32].tex = D3DXVECTOR2(0.2f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(0.4f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.2f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(0.4f, 1.0f);
        }
        else if (g_Setting.nMaxRound == 3)
        {
            //テクスチャの座標の設定
            pVtx[32].tex = D3DXVECTOR2(0.4f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(0.6f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.4f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(0.6f, 1.0f);
        }
    }

    //===================
    //難易度選択
    //===================
    //ひとりで遊ぶなら
    if (g_Setting.nMaxPlayer == 1)
    {
        if (g_Setting.nLevel == SETTINGLEVEL_NORMAL)
        {
            //テクスチャの座標の設定
            pVtx[32].tex = D3DXVECTOR2(0.6f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(0.8f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.6f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(0.8f, 1.0f);
        }
        else if (g_Setting.nLevel == SETTINGLEVEL_EXPERT)
        {
            //テクスチャの座標の設定
            pVtx[32].tex = D3DXVECTOR2(0.8f, 0.0f);
            pVtx[33].tex = D3DXVECTOR2(1.0f, 0.0f);
            pVtx[34].tex = D3DXVECTOR2(0.8f, 1.0f);
            pVtx[35].tex = D3DXVECTOR2(1.0f, 1.0f);
        }
    }

    //===================
    //制限時間選択
    //===================
    if (g_Setting.nTimeLimit == 3)
    {
        //テクスチャの座標の設定
        pVtx[36].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[37].tex = D3DXVECTOR2(0.3333f, 0.0f);
        pVtx[38].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[39].tex = D3DXVECTOR2(0.3333f, 1.0f);
    }
    else if (g_Setting.nTimeLimit == 4)
    {
        //テクスチャの座標の設定
        pVtx[36].tex = D3DXVECTOR2(0.3333f, 0.0f);
        pVtx[37].tex = D3DXVECTOR2(0.6666f, 0.0f);
        pVtx[38].tex = D3DXVECTOR2(0.3333f, 1.0f);
        pVtx[39].tex = D3DXVECTOR2(0.6666f, 1.0f);
    }
    else if (g_Setting.nTimeLimit == 5)
    {
        //テクスチャの座標の設定
        pVtx[36].tex = D3DXVECTOR2(0.6666f, 0.0f);
        pVtx[37].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[38].tex = D3DXVECTOR2(0.6666f, 1.0f);
        pVtx[39].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    //===================
    //開始位置選択
    //===================
    if (g_Setting.bRandomStartPos == true)
    {
        //テクスチャの座標の設定
        pVtx[40].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[41].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[42].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[43].tex = D3DXVECTOR2(1.0f, 1.0f);
    }
    else
    {
        //テクスチャの座標の設定
        pVtx[40].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[41].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[42].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[43].tex = D3DXVECTOR2(0.5f, 1.0f);
    }

    //===================
    //サドンデス選択
    //===================
    if (g_Setting.bSuddenDeath == true)
    {
        //テクスチャの座標の設定
        pVtx[44].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[45].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[46].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[47].tex = D3DXVECTOR2(0.5f, 1.0f);
    }
    else
    {
        //テクスチャの座標の設定
        pVtx[44].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[45].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[46].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[47].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    //===================
    //ステージ選択
    //===================
    if (g_Setting.nStage == SETTINGSTAGE_NORMAL)
    {
        //テクスチャの座標の設定
        pVtx[48].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(0.2f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(0.2f, 1.0f);
    }
    else if (g_Setting.nStage == SETTINGSTAGE_PLAIN)
    {
        //テクスチャの座標の設定
        pVtx[48].tex = D3DXVECTOR2(0.2f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(0.4f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.2f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(0.4f, 1.0f);
    }
    else if (g_Setting.nStage == SETTINGSTAGE_RUIN)
    {
        //テクスチャの座標の設定
        pVtx[48].tex = D3DXVECTOR2(0.4f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(0.6f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.4f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(0.6f, 1.0f);
    }
    else if (g_Setting.nStage == SETTINGSTAGE_CROSS)
    {
        //テクスチャの座標の設定
        pVtx[48].tex = D3DXVECTOR2(0.6f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(0.8f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.6f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(0.8f, 1.0f);
    }
    else if (g_Setting.nStage == SETTINGSTAGE_RANDOM)
    {
        //テクスチャの座標の設定
        pVtx[48].tex = D3DXVECTOR2(0.8f, 0.0f);
        pVtx[49].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[50].tex = D3DXVECTOR2(0.8f, 1.0f);
        pVtx[51].tex = D3DXVECTOR2(1.0f, 1.0f);
    }

    //===========================================
    //みんなで遊ぶか、ひとりで遊ぶかで選択肢を変える
    //===========================================
    //みんなで遊ぶなら
    if (g_Setting.nMaxPlayer != 1)
    {
        //ラウンド選択
        pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[9].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[11].tex = D3DXVECTOR2(0.5f, 1.0f);

        //制限時間
        pVtx[12].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
        pVtx[13].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
        pVtx[14].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;
        pVtx[15].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_TIME_LIMIT_POS;

        //開始位置
        pVtx[16].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
        pVtx[17].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
        pVtx[18].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;
        pVtx[19].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_RANDOM_START_POS;

        //サドンデス
        pVtx[20].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
        pVtx[21].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
        pVtx[22].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;
        pVtx[23].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_SUDDEN_DEATH_POS;

        //ステージ
        pVtx[24].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
        pVtx[25].pos = D3DXVECTOR3(SUBJECT_SIZE_X, -SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
        pVtx[26].pos = D3DXVECTOR3(-SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;
        pVtx[27].pos = D3DXVECTOR3(SUBJECT_SIZE_X, SUBJECT_SIZE_Y, 0.0f) + SUBJECT_STAGE_POS;

        //制限時間の選択肢
        pVtx[36].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
        pVtx[37].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
        pVtx[38].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;
        pVtx[39].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_TIME_LIMIT_POS;

        //開始位置の選択肢
        pVtx[40].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
        pVtx[41].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
        pVtx[42].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;
        pVtx[43].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_RANDOM_START_POS;

        //サドンデスの選択肢
        pVtx[44].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
        pVtx[45].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
        pVtx[46].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;
        pVtx[47].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_SUDDEN_DEATH_POS;

        //ステージの選択肢
        pVtx[48].pos = D3DXVECTOR3(-CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
        pVtx[49].pos = D3DXVECTOR3(CHOICE_SIZE_X, -CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
        pVtx[50].pos = D3DXVECTOR3(-CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
        pVtx[51].pos = D3DXVECTOR3(CHOICE_SIZE_X, CHOICE_SIZE_Y, 0.0f) + CHOICE_STAGE_POS;
    }

    //ひとりで遊ぶなら
    else
    {
        //難易度選択
        pVtx[8].tex = D3DXVECTOR2(0.5f, 0.0f);
        pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
        pVtx[10].tex = D3DXVECTOR2(0.5f, 1.0f);
        pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);

        //制限時間
        pVtx[12].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[13].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[14].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[15].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //開始位置
        pVtx[16].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[17].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[18].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[19].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //サドンデス
        pVtx[20].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[21].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[22].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[23].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //ステージ
        pVtx[24].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[25].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[26].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[27].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //制限時間の選択肢
        pVtx[36].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[37].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[38].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[39].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //開始位置の選択肢
        pVtx[40].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[41].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[42].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[43].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //サドンデスの選択肢
        pVtx[44].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[45].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[46].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[47].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

        //ステージの選択肢
        pVtx[48].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[49].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[50].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);
        pVtx[51].pos = D3DXVECTOR3(-500.0f, 0.0f, 0.0f);

    }


    //頂点データをアンロックする
    g_pVtxBuffSetting->Unlock();

}

//=============================================================================
// 設定終了処理
// Author:後藤慎之助
//=============================================================================
void UninitSetting(void)
{
    //テクスチャの開放
    for (int nCntSetting = 0; nCntSetting < SETTING_UI; nCntSetting++)
    {
        if (g_pTextureSetting[nCntSetting] != NULL)
        {
            g_pTextureSetting[nCntSetting]->Release();
            g_pTextureSetting[nCntSetting] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffSetting != NULL)
    {
        g_pVtxBuffSetting->Release();
        g_pVtxBuffSetting = NULL;
    }
}
//=============================================================================
// 設定描画処理
// Author:後藤慎之助
//=============================================================================
void DrawSetting(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffSetting, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

    for (int nCntSetting = 0; nCntSetting < SETTING_UI; nCntSetting++)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_pTextureSetting[nCntSetting]);

        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSetting, NUM_POLYGON);
    }
}

//=============================================================================
// 設定選択の取得
// Author:後藤慎之助
//=============================================================================
SETTING *GetSetting(void)
{
    return &g_Setting;
}

//=============================================================================
// 設定画面に入らなくても、最初に初期化するもの
// Author:後藤慎之助
//=============================================================================
void InitFirstSetting(void)
{
    //構造体の初期化
    g_Setting.nMaxPlayer = FIRST_SETTING_MAX_PLAYER;
    g_Setting.nMaxRound = FIRST_SETTING_MAX_ROUND;
    g_Setting.nTimeLimit = FIRST_SETTING_TIME_LIMIT;
    g_Setting.bRandomStartPos = FIRST_SETTING_RANDOM_START_POS;
    g_Setting.bSuddenDeath = FIRST_SETTING_SUDDEN_DEATH;
    g_Setting.nStage = FIRST_SETTING_STAGE;
    g_Setting.nLevel = FIRST_SETTING_LEVEL;

    g_Setting.type = SETTINGTYPE_PLAYER;
}