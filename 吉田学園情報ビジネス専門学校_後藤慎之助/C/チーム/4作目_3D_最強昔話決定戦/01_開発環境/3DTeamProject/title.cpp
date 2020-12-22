//=============================================================================
//
// 最強昔話決定戦の処理 [title.cpp] (タイトル)
// Author:後藤慎之助
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "manual.h"
#include "sound.h"
#include "controller.h"
#include "petal.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_apTextureTitle[2] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;//頂点バッファへのポインタ

int g_nCounterTitle;//アニメーションカウンター
int g_nPatternEnter;//アニメーションパターン

int g_nCntSetPetal; //桜の花びらをまくまでのカウント

//=============================================================================
// タイトル初期化処理
// Author:後藤慎之助
//=============================================================================
void InitTitle(void)
{
    //乱数の種の初期化
    srand((unsigned int)time(0));

    //花びらを初期化
    InitPetal();

    //変数宣言
    VERTEX_2D *pVtx = NULL;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/title000.png", &g_apTextureTitle[0]);
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/PRESS_ANY_BUTTON.png", &g_apTextureTitle[1]);

    //グローバル変数の初期化
    g_nCounterTitle = 0;
    g_nPatternEnter = 0;
    g_nCntSetPetal = SET_PETAL_FRAME;

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * 2,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffTitle,//変数名が変わると変更が必要
        NULL);

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

                                                  //頂点座標の設定（右回りで設定する）
    pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
    pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
    pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

    //プレスエンターの位置
    pVtx[4].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
    pVtx[5].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) - (PRESS_ENTER_Y / 2) + 146, 0.0f);
    pVtx[6].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) - (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);
    pVtx[7].pos = D3DXVECTOR3((SCREEN_WIDTH / 2) + (PRESS_ENTER_X / 2), (SCREEN_HEIGHT / 1.5) + (PRESS_ENTER_Y / 2) + 146, 0.0f);
    //rhwの設定
    pVtx[0].rhw = 1.0f;
    pVtx[1].rhw = 1.0f;
    pVtx[2].rhw = 1.0f;
    pVtx[3].rhw = 1.0f;

    pVtx[4].rhw = 1.0f;
    pVtx[5].rhw = 1.0f;
    pVtx[6].rhw = 1.0f;
    pVtx[7].rhw = 1.0f;
    //頂点カラーの設定
    pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

    pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    //テクスチャの座標
    pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

    pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
    pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
    pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
    pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);

    //頂点データアンロック
    g_pVtxBuffTitle->Unlock();

    PlaySound(SOUND_LABEL_BGM_TITLE);	//タイトルのBGM
}
//=============================================================================
// タイトル更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateTitle(void)
{
    //花びらを更新
    UpdatePetal();

    //花びら関連の変数
    float fSpeedX = 0.0f;   //速さX
    float fSpeedY = 0.0f;   //速さY
    float fSize = 0.0f;     //大きさ
    float fAlpha = 0.0f;    //透明度
    float fAngle = 0.0f;    //角度
    float fRotSpeed = 0.0f; //回転速度
    int nRotDirect;         //回転方向

    //桜の花びらを一定間隔で設定
    g_nCntSetPetal++;
    if(g_nCntSetPetal >= SET_PETAL_FRAME)
    {
        g_nCntSetPetal = 0;

        fSpeedX = float(rand() % 450 + 40) / 100.0f;
        fSpeedY = float(rand() % 270 + 40) / 100.0f;
        fSize = float(rand() % 4500 + 2000) / 100.0f;
        fAlpha = float(rand() % 100 + 65) / 100.0f;
        fAngle = float(rand() % 360 + 0) / 1.0f;
        fRotSpeed = float(rand() % 1000 + 5) / 10000.0f;
        nRotDirect = rand() % 2 + 1;

        //回転方向の正負を得る
        if (nRotDirect == 2)
        {
            nRotDirect = -1;
        }

        D3DXVECTOR3 pos = D3DXVECTOR3(SCREEN_WIDTH + 50.0f, 0.0f , 0.0f);   //発生位置
        D3DXVECTOR3 move = D3DXVECTOR3(-fSpeedX, fSpeedY, 0.0f);            //移動量
        D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);                //色

        SetPetal(pos, move, col, fSize, fAngle, fRotSpeed * nRotDirect,PETALTYPE_000); //花びらを設定
    }

    VERTEX_2D *pVtx = NULL;	//頂点情報へのポインタ
    int nFade = GetFade();	//フェードの情報を獲得

                            //点滅するアニメーション
    g_nCounterTitle++;
    if (g_nCounterTitle == 6)//速さ
    {
        g_nCounterTitle = 0;
        g_nPatternEnter++;
        if (g_nPatternEnter == 9)//透明度のパターン
        {
            g_nPatternEnter = 0;
        }
    }

    //頂点バッファをロック
    g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

    ////頂点カラーを設定（0～255の値で設定する）
    if (g_nPatternEnter == 0)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    }

    if (g_nPatternEnter == 1)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 205);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 205);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 205);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 205);
    }

    if (g_nPatternEnter == 2)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 155);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 155);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 155);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 155);
    }

    if (g_nPatternEnter == 3)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 105);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 105);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 105);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 105);
    }

    if (g_nPatternEnter == 4)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);
    }

    if (g_nPatternEnter == 5)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 105);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 105);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 105);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 105);
    }

    if (g_nPatternEnter == 6)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 155);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 155);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 155);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 155);
    }

    if (g_nPatternEnter == 7)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 205);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 205);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 205);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 205);
    }

    if (g_nPatternEnter == 8)
    {
        pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
        pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
    }

    //頂点データをアンロックする
    g_pVtxBuffTitle->Unlock();

    //Enterキーを押した時
    if (nFade == FADE_NONE)
    {
        if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(PLAYER_1, BUTTON_A) || GetJoypadTrigger(PLAYER_1, BUTTON_B) ||
            GetJoypadTrigger(PLAYER_1, BUTTON_X) || GetJoypadTrigger(PLAYER_1, BUTTON_Y) || GetJoypadTrigger(PLAYER_1, BUTTON_START))
        {
            StopSound(SOUND_LABEL_BGM_TITLE);	//タイトルのBGMを止める
            PlaySound(SOUND_LABEL_SE_DECISION);	//決定音
            PlaySound(SOUND_LABEL_BGM_MENU);	//メニューのBGM
            SetFade(MODE_MENU); //メニュー画面へ
        }
    }
}
//=============================================================================
// タイトル終了処理
// Author:後藤慎之助
//=============================================================================
void UninitTitle(void)
{
    //花びらのテクスチャを開放
    UninitPetal();

    //テクスチャの開放
    for (int nCntTitle = 0; nCntTitle < 2; nCntTitle++)
    {
        if (g_apTextureTitle[nCntTitle] != NULL)
        {
            g_apTextureTitle[nCntTitle]->Release();
            g_apTextureTitle[nCntTitle] = NULL;
        }
    }

    //頂点バッファの開放
    if (g_pVtxBuffTitle != NULL)
    {
        g_pVtxBuffTitle->Release();
        g_pVtxBuffTitle = NULL;
    }
}
//=============================================================================
// タイトル描画処理
// Author:後藤慎之助
//=============================================================================
void DrawTitle(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

    for (int nCntTitle = 0; nCntTitle < 2; nCntTitle++)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_apTextureTitle[nCntTitle]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntTitle, NUM_POLYGON);
    }

    //花びらを描画
    DrawPetal();

}