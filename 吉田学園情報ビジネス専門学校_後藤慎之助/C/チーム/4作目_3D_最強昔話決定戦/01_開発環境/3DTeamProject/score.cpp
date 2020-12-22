//=============================================================================
//
// 最強昔話決定戦の処理 [score.cpp] (ストーリーのスコア)
// Author:後藤慎之助
//
//=============================================================================
#include "score.h"
#include "game.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;//頂点バッファへのポインタ

int g_nScore;               //スコアの情報

//計算用の変数
float g_fIndex;             //指数　(スコア表示用)
float g_fRadix;             //基数　(スコア表示用)
int g_nScoreCalculation;    //スコア計算用
int g_nScoreCalculation2;   //スコア計算用2
int g_nAnswer;              //計算結果

//=============================================================================
// スコア初期化処理
// Author:後藤慎之助
//=============================================================================
void InitScore(void)
{
    //変数宣言
    VERTEX_2D *pVtx = NULL;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //グローバル変数の初期化
    g_fIndex = 0.0f;	//指数　(スコア表示用)
    g_fRadix = 10.0f;	//基数　(スコア表示用)

    //モードの取得
    MODE mode = GetMode();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTextureScore);

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * MAX_SCORE,//バッファサイズ
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffScore,//変数名が変わると変更が必要
        NULL);

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (int nScore = 0; nScore < MAX_SCORE; nScore++)
    {
        //ゲームの場合
        if (mode == MODE_GAME)
        {
            //頂点座標の設定（右回りで設定する）
            pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) - (SCORE_SIZE_X / 2) - 250, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) - (SCORE_SIZE_Y / 2) - 649, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) + (SCORE_SIZE_X / 2) - 250, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) - (SCORE_SIZE_Y / 2) - 649, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) - (SCORE_SIZE_X / 2) - 250, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) + (SCORE_SIZE_Y / 2) - 649, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) + (SCORE_SIZE_X / 2) - 250, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) + (SCORE_SIZE_Y / 2) - 649, 0.0f);
        }

        //リザルトの場合
        else if (mode == MODE_RESULT)
        {
            //頂点座標の設定（右回りで設定する）
            pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) - (SCORE_SIZE_X / 2) + 250, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) - (SCORE_SIZE_Y / 2) - 170, 0.0f);
            pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) + (SCORE_SIZE_X / 2) + 250, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) - (SCORE_SIZE_Y / 2) - 170, 0.0f);
            pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) - (SCORE_SIZE_X / 2) + 250, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) + (SCORE_SIZE_Y / 2) - 170, 0.0f);
            pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - (nScore*SCORE_SIZE_X + SCORE_SIZE_X / 2) + (SCORE_SIZE_X / 2) + 250, (SCREEN_HEIGHT - SCORE_SIZE_Y / 2) + (SCORE_SIZE_Y / 2) - 170, 0.0f);
        }

        //rhwの設定
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //頂点カラーの設定
        pVtx[0].col = D3DCOLOR_RGBA(254, 110, 165, 255);
        pVtx[1].col = D3DCOLOR_RGBA(254, 110, 165, 255);
        pVtx[2].col = D3DCOLOR_RGBA(254, 110, 165, 255);
        pVtx[3].col = D3DCOLOR_RGBA(254, 110, 165, 255);

        //テクスチャの座標
        pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

        pVtx += 4;//頂点データのポインタを4つ分進める
    }

    //頂点データアンロック
    g_pVtxBuffScore->Unlock();

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //スコア表示の計算
    for (g_fIndex = 0; g_fIndex < MAX_SCORE; g_fIndex++)
    {
        g_nScoreCalculation = (int)powf(g_fRadix, MAX_SCORE - g_fIndex);
        g_nScoreCalculation2 = (int)powf(g_fRadix, MAX_SCORE - g_fIndex - 1);
        g_nAnswer = g_nScore % g_nScoreCalculation / g_nScoreCalculation2;

        //テクスチャの座標
        pVtx[28].tex = D3DXVECTOR2(0.0f + (0.1f* g_nAnswer), 0.0f);
        pVtx[29].tex = D3DXVECTOR2(0.1f + (0.1f* g_nAnswer), 0.0f);
        pVtx[30].tex = D3DXVECTOR2(0.0f + (0.1f* g_nAnswer), 1.0f);
        pVtx[31].tex = D3DXVECTOR2(0.1f + (0.1f* g_nAnswer), 1.0f);

        pVtx -= 4;//頂点データのポインタを4つ分進める

    }

    //頂点データアンロック
    g_pVtxBuffScore->Unlock();
}

//=============================================================================
// スコア更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateScore(void)
{

}

//=============================================================================
// スコア終了処理
// Author:後藤慎之助
//=============================================================================
void UninitScore(void)
{
    //テクスチャの開放
    if (g_pTextureScore != NULL)
    {
        g_pTextureScore->Release();
        g_pTextureScore = NULL;
    }
    //頂点バッファの開放
    if (g_pVtxBuffScore != NULL)
    {
        g_pVtxBuffScore->Release();
        g_pVtxBuffScore = NULL;
    }
}

//=============================================================================
// スコア描画処理
// Author:後藤慎之助
//=============================================================================
void DrawScore(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

                                   //スコアの数字の描画
    for (int nScore = 0; nScore < MAX_SCORE; nScore++)
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_pTextureScore);

        //ポリゴンの描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nScore, NUM_POLYGON);
    }

    //テクスチャの設定の解除
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// スコアの加算処理
// Author:後藤慎之助
//=============================================================================
void ScoreCalculation(int score)
{
    //ゲーム状況の取得
    GAME *pGame = GetGame();

    //ゲーム中なら
    if (pGame->state == GAMESTATE_NORMAL)
    {
        g_nScore += score;
        //変数宣言
        VERTEX_2D *pVtx = NULL;//頂点情報のポインタ
        g_fIndex = 0.0f;	//指数　(スコア表示用)
        g_fRadix = 10.0f;	//基数　(スコア表示用)

        //頂点データの範囲をロックし、頂点バッファへのポインタを取得
        g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

        //スコアが0点を下回らないようにする
        if (g_nScore < 0)
        {
            g_nScore = 0;
        }

        //スコアが99999999点を上回らないようにする
        if (g_nScore > 99999999)
        {
            g_nScore = 99999999;
        }

        //スコア表示の計算
        for (g_fIndex = 0; g_fIndex < MAX_SCORE; g_fIndex++)
        {
            g_nScoreCalculation = (int)powf(g_fRadix, MAX_SCORE - g_fIndex);
            g_nScoreCalculation2 = (int)powf(g_fRadix, MAX_SCORE - g_fIndex - 1);
            g_nAnswer = g_nScore % g_nScoreCalculation / g_nScoreCalculation2;

            //テクスチャの座標
            pVtx[28].tex = D3DXVECTOR2(0.0f + (0.1f* g_nAnswer), 0.0f);
            pVtx[29].tex = D3DXVECTOR2(0.1f + (0.1f* g_nAnswer), 0.0f);
            pVtx[30].tex = D3DXVECTOR2(0.0f + (0.1f* g_nAnswer), 1.0f);
            pVtx[31].tex = D3DXVECTOR2(0.1f + (0.1f* g_nAnswer), 1.0f);

            pVtx -= 4;//頂点データのポインタを4つ分進める

        }

        //頂点データアンロック
        g_pVtxBuffScore->Unlock();
    }
}

//=============================================================================
// スコアの取得
// Author:後藤慎之助
//=============================================================================
int GetScore(void)
{
    return g_nScore;
}

//=============================================================================
// スコアのリセット
// Author:後藤慎之助
//=============================================================================
void ResetScore(void)
{
    //スコアの初期化
    g_nScore = 0;
}
