//=============================================================================
//
// PLASTIC TROOPERSの処理 [ranking.cpp] (ランキング)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "main.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "score.h"
#include "controller.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureRanking[2] = {};//テクスチャへのポインタ(使うテクスチャは2つ)
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;//頂点バッファへのポインタ

int g_nRankingScore[RANKING_SCORE_MAX];		//比較するスコア
int g_nRank[RANKING_SCORE_MAX];             //ランク付けするため
int g_nCampaignScore;                       //ゲームモードのスコア

//=============================================================================
// ランキング初期化処理
//=============================================================================
void InitRanking(void)
{
    //変数宣言
    VERTEX_2D*pVtx;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //タイトルのモード選択を取得
    MODE Title = GetModeTitle();

    //キャンペーンから遷移したなら
    if (Title == MODE_CAMPAIGN)
    {
        g_nCampaignScore = GetScore();
    }

    //ランキングから遷移したなら
    else if (Title == MODE_RANKING)
    {
        g_nCampaignScore = 0;
    }

    //スコアの読み込み
    ReadScore();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/ranking000.png", &g_pTextureRanking[0]);//スコアの背景
    D3DXCreateTextureFromFile(pDevice, "TEXTURE/number000.png", &g_pTextureRanking[1]);	//数字

    //頂点バッファ生成
    pDevice->CreateVertexBuffer(
        sizeof(VERTEX_2D) * NUM_VERTEX * /*RANKING_SCORE_MAX * MAX_SCORE + 4*/200,//バッファサイズ(+4は、ランキングの背景の分)
        D3DUSAGE_WRITEONLY,//(固定）
        FVF_VERTEX_2D,//頂点フォーマット
        D3DPOOL_MANAGED,//（固定）
        &g_pVtxBuffRanking,//変数名が変わると変更が必要
        NULL);

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //頂点座標の設定（右回りで設定する）
    //ランキングの背景
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

    //1位
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //頂点座標の設定（右回りで設定する）
        pVtx[4].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT, 0.0f);
        pVtx[5].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT, 0.0f);
        pVtx[6].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT, 0.0f);
        pVtx[7].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT, 0.0f);

        //rhwの設定
        pVtx[4].rhw = 1.0f;
        pVtx[5].rhw = 1.0f;
        pVtx[6].rhw = 1.0f;
        pVtx[7].rhw = 1.0f;

        //ランクインしたら、カラーを変える
        if (g_nRank[RANKING_SCORE_MAX - 1] == 0)
        {
            //頂点カラーの設定
            pVtx[4].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[5].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[6].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[7].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //頂点カラーの設定
            pVtx[4].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[5].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[6].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[7].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //テクスチャの座標
        pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[5].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[7].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//頂点データのポインタを4つ分進める
        }    
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //2位
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //頂点座標の設定（右回りで設定する）
        pVtx[36].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE, 0.0f);
        pVtx[37].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE, 0.0f);
        pVtx[38].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE, 0.0f);
        pVtx[39].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE, 0.0f);

        //rhwの設定
        pVtx[36].rhw = 1.0f;
        pVtx[37].rhw = 1.0f;
        pVtx[38].rhw = 1.0f;
        pVtx[39].rhw = 1.0f;

        //ランクインしたら、カラーを変える
        if (g_nRank[RANKING_SCORE_MAX - 1] == 1)
        {
            //頂点カラーの設定
            pVtx[36].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[37].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[38].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[39].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //頂点カラーの設定
            pVtx[36].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[37].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[38].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[39].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //テクスチャの座標
        pVtx[36].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[37].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[38].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[39].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //3位
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //頂点座標の設定（右回りで設定する）
        pVtx[68].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 2, 0.0f);
        pVtx[69].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 2, 0.0f);
        pVtx[70].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 2, 0.0f);
        pVtx[71].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 2, 0.0f);

        //rhwの設定
        pVtx[68].rhw = 1.0f;
        pVtx[69].rhw = 1.0f;
        pVtx[70].rhw = 1.0f;
        pVtx[71].rhw = 1.0f;

        //ランクインしたら、カラーを変える
        if (g_nRank[RANKING_SCORE_MAX - 1] == 2)
        {
            //頂点カラーの設定
            pVtx[68].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[69].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[70].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[71].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //頂点カラーの設定
            pVtx[68].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[69].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[70].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[71].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //テクスチャの座標
        pVtx[68].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[69].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[70].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[71].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //4位
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //頂点座標の設定（右回りで設定する）
        pVtx[100].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 3, 0.0f);
        pVtx[101].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 3, 0.0f);
        pVtx[102].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 3, 0.0f);
        pVtx[103].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 3, 0.0f);

        //rhwの設定
        pVtx[100].rhw = 1.0f;
        pVtx[101].rhw = 1.0f;
        pVtx[102].rhw = 1.0f;
        pVtx[103].rhw = 1.0f;

        //ランクインしたら、カラーを変える
        if (g_nRank[RANKING_SCORE_MAX - 1] == 3)
        {
            //頂点カラーの設定
            pVtx[100].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[101].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[102].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[103].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //頂点カラーの設定
            pVtx[100].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[101].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[102].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[103].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //テクスチャの座標
        pVtx[100].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[101].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[102].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[103].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //5位
    for (int nResultScore = 0; nResultScore < MAX_SCORE; nResultScore++)
    {
        //頂点座標の設定（右回りで設定する）
        pVtx[132].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 4, 0.0f);
        pVtx[133].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) - (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 4, 0.0f);
        pVtx[134].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) - (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 4, 0.0f);
        pVtx[135].pos = D3DXVECTOR3(SCREEN_WIDTH - (nResultScore*RANKINGSCORE_SIZE_X + RANKINGSCORE_SIZE_X / 2) + (RANKINGSCORE_SIZE_X / 2) - 300, (SCREEN_HEIGHT - RANKINGSCORE_SIZE_Y / 2) + (RANKINGSCORE_SIZE_Y / 2) - RANKING_HEIGHT + RANKING_SPACE * 4, 0.0f);

        //rhwの設定
        pVtx[132].rhw = 1.0f;
        pVtx[133].rhw = 1.0f;
        pVtx[134].rhw = 1.0f;
        pVtx[135].rhw = 1.0f;

        //ランクインしたら、カラーを変える
        if (g_nRank[RANKING_SCORE_MAX - 1] == 4)
        {
            //頂点カラーの設定
            pVtx[132].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[133].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[134].col = D3DCOLOR_RGBA(255, 0, 0, 255);
            pVtx[135].col = D3DCOLOR_RGBA(255, 0, 0, 255);
        }
        else
        {
            //頂点カラーの設定
            pVtx[132].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[133].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[134].col = D3DCOLOR_RGBA(0, 0, 0, 255);
            pVtx[135].col = D3DCOLOR_RGBA(0, 0, 0, 255);
        }

        //テクスチャの座標
        pVtx[132].tex = D3DXVECTOR2(0.0f, 0.0f);
        pVtx[133].tex = D3DXVECTOR2(0.1f, 0.0f);
        pVtx[134].tex = D3DXVECTOR2(0.0f, 1.0f);
        pVtx[135].tex = D3DXVECTOR2(0.1f, 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx += 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //変数宣言
    float fIndex;		//指数　(スコア表示用)
    float fRadix = 10.0f;	//基数　(スコア表示用)

    //1位のスコアの計算
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[0] % nResultScore / nResultScore2;

        //テクスチャの座標
        pVtx[32].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[33].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[34].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[35].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);


        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //2位のスコアの計算
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[1] % nResultScore / nResultScore2;

        //テクスチャの座標
        pVtx[64].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[65].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[66].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[67].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //3位のスコアの計算
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[2] % nResultScore / nResultScore2;

        //テクスチャの座標
        pVtx[96].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[97].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[98].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[99].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //4位のスコアの計算
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[3] % nResultScore / nResultScore2;

        //テクスチャの座標
        pVtx[128].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[129].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[130].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[131].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();
    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    //5位のスコアの計算
    for (fIndex = 0; fIndex < MAX_SCORE; fIndex++)
    {
        int nResultScore = (int)powf(fRadix, MAX_SCORE - fIndex);
        int nResultScore2 = (int)powf(fRadix, MAX_SCORE - fIndex - 1);
        int nAnswer = g_nRankingScore[4] % nResultScore / nResultScore2;

        //テクスチャの座標
        pVtx[160].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[161].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[162].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[163].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        if (nResultScore != MAX_SCORE - 1)
        {
            pVtx -= 4;//頂点データのポインタを4つ分進める
        }
    }

    //頂点データアンロック
    g_pVtxBuffRanking->Unlock();

    //PlaySound(SOUND_LABEL_BGM003);	//リザルト曲
}

//=============================================================================
// ランキング更新処理
//=============================================================================
void UpdateRanking(void)
{
    int nFade = GetFade();	//フェードの情報を獲得

                            //Enterキーを押した時
    if (nFade == FADE_NONE)
    {
        if (GetKeyboardTrigger(DIK_RETURN) || GetJoypadTrigger(BUTTON_A) || GetJoypadTrigger(BUTTON_B) || GetJoypadTrigger(BUTTON_START))
        {
            //PlaySound(SOUND_LABEL_SE_ITEM);	//アイテム取得音でプレスエンター音の代用
            SetFade(MODE_TITLE);//タイトル画面に移動
                                   //StopSound(SOUND_LABEL_BGM000);//曲の停止(特定の物)
        }
    }
}
//=============================================================================
// ランキング終了処理
//=============================================================================
void UninitRanking(void)
{
    //テクスチャの開放
    for (int nCntResultScore = 0; nCntResultScore < 2; nCntResultScore++)
    {
        if (g_pTextureRanking[nCntResultScore] != NULL)
        {
            g_pTextureRanking[nCntResultScore]->Release();
            g_pTextureRanking[nCntResultScore] = NULL;
        }
    }
    //頂点バッファの開放
    if (g_pVtxBuffRanking != NULL)
    {
        g_pVtxBuffRanking->Release();
        g_pVtxBuffRanking = NULL;
    }
}

//=============================================================================
// ランキング描画処理
//=============================================================================
void DrawRanking(void)
{
    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));
    pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

    //ランキングの背景
    //テクスチャの設定
    pDevice->SetTexture(0, g_pTextureRanking[0]);
    //リザルト背景の描画
    pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);

    //スコアの数字の描画
    for (int nResultScore = 1; nResultScore < /*RANKING_SCORE_MAX * MAX_SCORE + 1*/200; nResultScore++)	//pVtx0～3を読み込まないようにするため、1からforを始める
    {
        //テクスチャの設定
        pDevice->SetTexture(0, g_pTextureRanking[1]);
        //数字の描画
        pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nResultScore, NUM_POLYGON);
    }

}

//==============================================================
// スコアの読み込み
//==============================================================
void ReadScore(void)
{
    FILE *pFile = NULL; //ファイルポインタ

    //出力処理
    pFile = fopen("score.txt", "r");

    if (pFile != NULL)
    {//ファイルオープン成功
        for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
        { //上位スコアの出力処理
            fscanf(pFile, "%d", &g_nRankingScore[nCntScore]);
        }
        fclose(pFile);
    }

    g_nRankingScore[RANKING_SCORE_MAX-1] = g_nCampaignScore;

    //ランクソート
    for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
    {
        g_nRank[nCntScore] = 0;
        for (int nData = 0; nData < RANKING_SCORE_MAX; nData++)
        {
            if (g_nRankingScore[nCntScore] < g_nRankingScore[nData])
            {
                g_nRank[nCntScore]++;
            }
        }
    }

    //スコア入力処理
    pFile = fopen("score.txt", "w");
    if (pFile != NULL)
    {//ファイルオープン成功
        for (int nCntRank = 0; nCntRank < RANKING_SCORE_MAX; nCntRank++)
        {
            for (int nCntScore = 0; nCntScore < RANKING_SCORE_MAX; nCntScore++)
            {
                if (g_nRank[nCntScore] == nCntRank)
                {
                    fprintf(pFile, "%d\n", g_nRankingScore[nCntScore]);
                }
            }
        }
        fclose(pFile);
    }

    //スコア出力処理
    pFile = fopen("score.txt", "r");
    if (pFile != NULL)
    {//ファイルオープン成功
        for (int nCntScore = 0; nCntScore < MAX_SCORE; nCntScore++)
        { //上位スコアの出力処理
            fscanf(pFile, "%d", &g_nRankingScore[nCntScore]);
        }
        fclose(pFile);
    }

}