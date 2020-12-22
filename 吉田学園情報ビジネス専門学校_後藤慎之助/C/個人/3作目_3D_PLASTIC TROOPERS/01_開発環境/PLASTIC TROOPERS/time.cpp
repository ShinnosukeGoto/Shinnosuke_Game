//=============================================================================
//
// ナインストーリーズの処理 [time.cpp] (制限時間)
// Author:後藤慎之助
//
//=============================================================================
#include "time.h"
#include "item.h"
#include "player.h"
//#include "explosion.h"
//#include "game.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;//頂点バッファへのポインタ

int g_nCounterTime;	//時間を数える
int g_nPatternTime;	//時間を反映させる

int g_nTime;	//時間の情報
                //=============================================================================
                // タイム初期化処理
                //=============================================================================
void InitTime(void)
{
    //変数宣言
    VERTEX_2D*pVtx;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //制限時間の初期化
    g_nCounterTime = 0;
    g_nPatternTime = 0;
    g_nTime = TIME_LIMIT;	//制限時間は300秒

                            //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTextureTime);

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * MAX_TIME,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffTime,//変数名が変わると変更が必要
        NULL);

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nTime = 0; nTime < MAX_TIME; nTime++)
    {
        //頂点座標の設定（右回りで設定する）
        pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) - (TIME_SIZE_X / 2) - 595, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) - (TIME_SIZE_Y / 2) - 620, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) + (TIME_SIZE_X / 2) - 595, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) - (TIME_SIZE_Y / 2) - 620, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) - (TIME_SIZE_X / 2) - 595, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) + (TIME_SIZE_Y / 2) - 620, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) + (TIME_SIZE_X / 2) - 595, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) + (TIME_SIZE_Y / 2) - 620, 0.0f);

        //rhwの設定
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //頂点カラーの設定
        pVtx[0].col = D3DCOLOR_RGBA(173, 73, 225, 255);
        pVtx[1].col = D3DCOLOR_RGBA(173, 73, 225, 255);
        pVtx[2].col = D3DCOLOR_RGBA(173, 73, 225, 255);
        pVtx[3].col = D3DCOLOR_RGBA(173, 73, 225, 255);

        //テクスチャの座標
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

        pVtx += 4;//頂点データのポインタを4つ分進める
    }

    //頂点データアンロック
    g_pVtxBuffTime->Unlock();

}
//=============================================================================
// タイム更新処理
//=============================================================================
void UpdateTime(void)
{
    //変数宣言
    VERTEX_2D*pVtx;//頂点情報のポインタ
    float fIndex;		//指数　(スコア表示用)
    float fRadix = 10.0f;	//基数　(スコア表示用)
    ITEM *pItem = GetItem();		//アイテムの情報を取得するため
    PLAYER*pPlayer;

    pPlayer = GetPlayer();	//ハンターを取得

                            //時間の概念を獲得
    if (pPlayer->bDisp == true)
    {
        g_nCounterTime++;	//カウンタ加算
        if (g_nCounterTime == 59)	//59fps+1fpsで1秒
        {
            g_nCounterTime = 0;
            g_nPatternTime++;
            if (g_nPatternTime == 1)//テクスチャを更新する間隔が1秒
            {
                //姫を救出したらカウントダウンをやめる
                if (pItem->type == ITEMTYPE_005 && pItem->bUse == true)
                {

                }
                else
                {
                    //カウントダウン
                    g_nTime -= g_nPatternTime;
                }
                g_nPatternTime = 0;
            }
        }
    }

    //タイムアップ
    if (g_nTime <= 0)
    {
        g_nTime = 0;
        //if (pPlayer->nStock > -1)	//ストックが残っているとき
        //{
        //    pPlayer->nStock = -1;	//ゲームオーバーにするため、ストックを直接調整
        //    HitPlayer(999);		//爆発やゲームオーバー等を呼び出す
        //}
    }

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (fIndex = 0; fIndex < MAX_TIME; fIndex++)
    {
        int nTime = (int)powf(fRadix, MAX_TIME - fIndex);
        int nTime2 = (int)powf(fRadix, MAX_TIME - fIndex - 1);
        int nAnswer = g_nTime % nTime / nTime2;

        //テクスチャの座標
        pVtx[8].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[9].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[10].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[11].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        pVtx -= 4;//頂点データのポインタを4つ分進める
    }
    //頂点データアンロック
    g_pVtxBuffTime->Unlock();
}
//=============================================================================
// タイム終了処理
//=============================================================================
void UninitTime(void)
{
    //テクスチャの開放
    if (g_pTextureTime != NULL)
    {
        g_pTextureTime->Release();
        g_pTextureTime = NULL;
    }

    //頂点バッファの開放
    if (g_pVtxBuffTime != NULL)
    {
        g_pVtxBuffTime->Release();
        g_pVtxBuffTime = NULL;
    }
}

//=============================================================================
// タイム描画処理
//=============================================================================
void DrawTime(void)
{

    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

                                   //スコアの数字の描画
    for (int nTime = 0; nTime < MAX_TIME; nTime++)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_pTextureTime);

        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nTime, NUM_POLYGON);
    }

    //テクスチャの設定
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// タイム情報の送信
//=============================================================================
int GetTime(void)
{
    return g_nTime;
}