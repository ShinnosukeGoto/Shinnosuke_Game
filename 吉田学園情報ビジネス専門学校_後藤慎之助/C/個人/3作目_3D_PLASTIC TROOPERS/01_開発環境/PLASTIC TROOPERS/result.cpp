//=============================================================================
//
// PLASTIC TROOPERSの処理 [result.cpp] (ゲームクリアかゲームオーバー)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "result.h"
#include "player.h"
//#include "enemy.h"
#include "input.h"
#include "campaign.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "controller.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureResult[2] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;//頂点バッファへのポインタ

PLAYER*g_pPlayer;
//MODE g_NextModeResult;  //リザルトの次のモード

//=============================================================================
// リザルト初期化処理
//=============================================================================
void InitResult(void)
{
    //変数宣言
    VERTEX_2D*pVtx;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();
    //ハンターの状況を取得
    g_pPlayer = GetPlayer();

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * 1,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffResult,//変数名が変わると変更が必要
        NULL);

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/gameclear000.png", &g_pTextureResult[0]);	//ゲームクリア
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/gameover000.png", &g_pTextureResult[1]);	//ゲームオーバー

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
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
    int nFade = GetFade();	//フェードの情報を獲得

    if (nFade == FADE_NONE)
    {
        if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) ||
            GetJoypadTrigger(BUTTON_X) || GetJoypadTrigger(BUTTON_Y) || GetJoypadTrigger(BUTTON_START))
        {
            //PlaySound(SOUND_LABEL_SE_ENTER);	//プレスエンター音
            //InitResult2();		//リザルト2を初期化
            SetFade(MODE_RANKING);//ランキング画面に移動
        }
    }

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
    //テクスチャの開放
    for (int nCntResult = 0; nCntResult < 2; nCntResult++)
    {
        if (g_pTextureResult[nCntResult] != NULL)
        {
            g_pTextureResult[nCntResult]->Release();
            g_pTextureResult[nCntResult] = NULL;
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
// 描画処理
//=============================================================================
void DrawResult(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

    //ハンターが生存しているなら、ゲームクリア
    if (g_pPlayer->bDisp == true)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_pTextureResult[0]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }

    //ハンターが全滅しているならゲームオーバー
    if (g_pPlayer->bDisp == false)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_pTextureResult[1]);
        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
    }
}