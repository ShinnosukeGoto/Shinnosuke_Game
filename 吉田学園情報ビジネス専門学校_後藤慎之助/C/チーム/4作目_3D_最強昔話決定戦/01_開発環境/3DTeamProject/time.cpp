//=============================================================================
//
// 最強昔話決定戦の処理 [time.cpp] (制限時間、サドンデス)
// Author:後藤慎之助
//
//=============================================================================
#include "time.h"
#include "item.h"
#include "player.h"
#include "ui.h"
#include "game.h"
#include "block.h"
#include "stage.h"
#include "setting.h"
#include "sound.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;//頂点バッファへのポインタ

int g_nCounterTime;	    //時間を数える
int g_nTimeSecond;	    //制限時間の秒
bool g_bTimeUp;         //タイムアップしたかどうか

bool g_bUseSuddenDeath; //サドンデスを使うかどうか
bool g_bSuddenDeath;    //サドンデスに突入したかどうか
bool g_bSoundWarning;       //警告の音を使う
bool g_bSoundSuddenDeath;   //サドンデスの音を使う
bool g_bSoundFinish;        //フィニッシュ音を使う
int g_nCntSuddenDeath;  //サドンデスの時間を数える
float g_fSuddenDeathBlockX;  //サドンデスブロックのX座標
float g_fSuddenDeathBlockZ;  //サドンデスブロックのZ座標
bool g_bAroundTheCorner1;     //サドンデスブロックが角を曲がる1
bool g_bAroundTheCorner2;     //サドンデスブロックが角を曲がる2
bool g_bAroundTheCorner3;     //サドンデスブロックが角を曲がる3
bool g_bAroundTheCorner4;     //サドンデスブロックが角を曲がる4
bool g_bAroundTheCorner5;     //サドンデスブロックが角を曲がる5
bool g_bAroundTheCorner6;     //サドンデスブロックが角を曲がる6
bool g_bAroundTheCorner7;     //サドンデスブロックが角を曲がる7
bool g_bAroundTheCorner8;     //サドンデスブロックが角を曲がる8
bool g_bAroundTheCorner9;     //サドンデスブロックが角を曲がる9
bool g_bAroundTheCorner10;    //サドンデスブロックが角を曲がる10
bool g_bAroundTheCorner11;    //サドンデスブロックが角を曲がる11
bool g_bSetBlockFinish;       //サドンデスブロック配置終了

//=============================================================================
// タイム初期化処理
// Author:後藤慎之助
//=============================================================================
void InitTime(void)
{
    //変数宣言
    VERTEX_2D *pVtx = NULL;//頂点情報のポインタ
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //設定の情報取得
    SETTING *pSetting = GetSetting();

    //グローバル変数の初期化
    g_nCounterTime = 0;
    g_nTimeSecond = TIME_FIREST_SECOND;	//制限時間の最初の秒の位は0
    g_bTimeUp = false;
    g_bUseSuddenDeath = pSetting->bSuddenDeath;
    g_bSuddenDeath = false;
    g_bSoundWarning = false;
    g_bSoundSuddenDeath = false;
    g_bSoundFinish = false;
    g_nCntSuddenDeath = 0;
    g_fSuddenDeathBlockX = 0.0f;
    g_fSuddenDeathBlockZ = 0.0f;
    g_bAroundTheCorner1 = false;
    g_bAroundTheCorner2 = false;
    g_bAroundTheCorner3 = false;
    g_bAroundTheCorner4 = false;
    g_bAroundTheCorner5 = false;
    g_bAroundTheCorner6 = false;
    g_bAroundTheCorner7 = false;
    g_bAroundTheCorner8 = false;
    g_bAroundTheCorner9 = false;
    g_bAroundTheCorner10 = false;
    g_bAroundTheCorner11 = false;
    g_bSetBlockFinish = false;

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
        pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) - (TIME_SIZE_X / 2) - 565, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) - (TIME_SIZE_Y / 2) - 640, 0.0f);
        pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) + (TIME_SIZE_X / 2) - 565, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) - (TIME_SIZE_Y / 2) - 640, 0.0f);
        pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) - (TIME_SIZE_X / 2) - 565, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) + (TIME_SIZE_Y / 2) - 640, 0.0f);
        pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH - (nTime*TIME_SIZE_X + TIME_SIZE_X / 2) + (TIME_SIZE_X / 2) - 565, (SCREEN_HEIGHT - TIME_SIZE_Y / 2) + (TIME_SIZE_Y / 2) - 640, 0.0f);

        //rhwの設定
        pVtx[0].rhw = 1.0f;
        pVtx[1].rhw = 1.0f;
        pVtx[2].rhw = 1.0f;
        pVtx[3].rhw = 1.0f;

        //頂点カラーの設定
        pVtx[0].col = TIME_COLOR;
        pVtx[1].col = TIME_COLOR;
        pVtx[2].col = TIME_COLOR;
        pVtx[3].col = TIME_COLOR;

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
// Author:後藤慎之助
//=============================================================================
void UpdateTime(void)
{
    //変数宣言
    VERTEX_2D *pVtx = NULL; //頂点情報のポインタ
    float fIndex;		    //指数
    float fRadix = 10.0f;	//基数

    //サドンデス用の変数
    float posX = 0.0f;
    float posZ = 0.0f;
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    
    //カウンタ加算
    if (g_bTimeUp == false)  //タイムアップしていないなら
    {
        g_nCounterTime++;    //カウンタ更新

        if (g_nCounterTime == 60)	//60fpsで1秒
        {
            //カウントダウン
            g_nTimeSecond--;

            //カウンタを戻す
            g_nCounterTime = 0;
        }

        //60秒数え終わったら、分を下げ、秒を59に戻す
        if (g_nTimeSecond < 0)
        {
            g_nTimeSecond = 59;
            g_nTimeMinute--;
        }
    }

    //1分で警告
    if (g_bUseSuddenDeath == true && g_nTimeSecond == 0 && g_nTimeMinute == 1)
    {
        //警告音を流す
        if (g_bSoundWarning == false)
        {
            g_bSoundWarning = true;

            SetUI(D3DXVECTOR3(1480.0f, SCREEN_HEIGHT / 2, 0.0f), 400.0f, 200.0f, UITYPE_011);  //急げ～
            StopSound(SOUND_LABEL_BGM_GAME);            //ゲームの音楽を止める
            PlaySound(SOUND_LABEL_SE_WARNING);          //警告音
        }
    }

    //制限時間残り55秒でサドンデス突入
    if (g_nTimeSecond == 55 && g_nTimeMinute == 0)
    {
        //設定で、サドンデスを使うなら
        if (g_bUseSuddenDeath == true)
        {
            g_bSuddenDeath = true;

            //サドンデスの音楽を流す
            if (g_bSoundSuddenDeath == false)
            {
                PlaySound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //サドンデスの音楽
                g_bSoundSuddenDeath = true;
            }
        }
    }

    //分と秒が0なら、タイムアップ
    if (g_nTimeSecond == 0 && g_nTimeMinute == 0)
    {
        //タイムアップにする
        g_bTimeUp = true;

        //フィニッシュの音楽を流す
        if (g_bSoundFinish == false)
        {
            PlaySound(SOUND_LABEL_SE_FINISH);    //フィニッシュの効果音
            g_bSoundFinish = true;
        }

        //引き分けにする
        GAME *pGame = GetGame();
        pGame->nCounter = 0;

        //設定の情報取得
        SETTING *pSetting = GetSetting();
        //ストーリーなら
        if (pSetting->nMaxPlayer == 1)
        {
            PlaySound(SOUND_LABEL_SE_FINISH);       //フィニッシュの効果音
            SetGameState(GAMESTATE_FINSIH_GAME);    //ゲームオーバー
        }
    }

    //サドンデスかつ、ブロック配置が終わっていないなら
    if (g_bSuddenDeath == true && g_bSetBlockFinish == false)
    {
        g_nCntSuddenDeath++;

        //ブロックを配置
        if (g_nCntSuddenDeath >= SUDDEN_DEATH_SET_BLOCK)
        {
            g_nCntSuddenDeath = 0;

            //X座標とZ座標を順に進める
            posX = 100.0f + 100.0f * g_fSuddenDeathBlockX;
            posZ = 100.0f + 100.0f * g_fSuddenDeathBlockZ;
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //グリッド座標に変換する
            pos = ConversionGrid(&pos);

            //サドンデスブロックを配置
            SetBlock(D3DXVECTOR3(pos.x, SUDDEN_DEATH_BLOCK_Y, pos.z), BLOCKTYPE_006);

            //三番下
            if (g_bAroundTheCorner11 == true)
            {
                g_fSuddenDeathBlockX -= 1.0f;

                if (g_fSuddenDeathBlockX == 2.0f)
                {
                    g_bSetBlockFinish = true;   //配置終了
                }
            }

            //三番右
            if (g_bAroundTheCorner10 == true && g_bAroundTheCorner11 == false)
            {
                g_fSuddenDeathBlockZ -= 1.0f;

                if (g_fSuddenDeathBlockZ == 2.0f)
                {
                    g_bAroundTheCorner11 = true;
                }
            }

            //三番上
            if (g_bAroundTheCorner9 == true && g_bAroundTheCorner10 == false)
            {
                g_fSuddenDeathBlockX += 1.0f;

                if (g_fSuddenDeathBlockX == 10.0f)
                {
                    g_bAroundTheCorner10 = true;
                }
            }

            //三番左
            if (g_bAroundTheCorner8 == true && g_bAroundTheCorner9 == false)
            {
                g_fSuddenDeathBlockZ += 1.0f;

                if (g_fSuddenDeathBlockZ == 8.0f)
                {
                    g_bAroundTheCorner9 = true;
                }
            }

            //二番下
            if (g_bAroundTheCorner7 == true && g_bAroundTheCorner8 == false)
            {
                g_fSuddenDeathBlockX -= 1.0f;

                if (g_fSuddenDeathBlockX == 2.0f)
                {
                    g_bAroundTheCorner8 = true;
                }
            }

            //二番右
            if (g_bAroundTheCorner6 == true && g_bAroundTheCorner7 == false)
            {
                g_fSuddenDeathBlockZ -= 1.0f;

                if (g_fSuddenDeathBlockZ == 1.0f)
                {
                    g_bAroundTheCorner7 = true;
                }
            }

            //二番上
            if (g_bAroundTheCorner5 == true && g_bAroundTheCorner6 == false)
            {
                g_fSuddenDeathBlockX += 1.0f;

                if (g_fSuddenDeathBlockX == 11.0f)
                {
                    g_bAroundTheCorner6 = true;
                }
            }

            //二番左
            if (g_bAroundTheCorner4 == true && g_bAroundTheCorner5 == false)
            {
                g_fSuddenDeathBlockZ += 1.0f;

                if (g_fSuddenDeathBlockZ == 9.0f)
                {
                    g_bAroundTheCorner5 = true;
                }
            }

            //一番下
            if (g_bAroundTheCorner3 == true && g_bAroundTheCorner4 == false)
            {
                g_fSuddenDeathBlockX -= 1.0f;

                if (g_fSuddenDeathBlockX == 1.0f)
                {
                    g_bAroundTheCorner4 = true;
                }
            }

            //一番右
            if (g_bAroundTheCorner2 == true && g_bAroundTheCorner3 == false)
            {
                g_fSuddenDeathBlockZ -= 1.0f;

                if (g_fSuddenDeathBlockZ == 0.0f)
                {
                    g_bAroundTheCorner3 = true;
                }
            }

            //一番上
            if (g_bAroundTheCorner1 == true && g_bAroundTheCorner2 == false)
            {
                g_fSuddenDeathBlockX += 1.0f;

                if (g_fSuddenDeathBlockX == 12.0f)
                {
                    g_bAroundTheCorner2 = true;
                }
            }

            //一番左
            if (g_bAroundTheCorner1 == false)
            {
                g_fSuddenDeathBlockZ += 1.0f;

                if (g_fSuddenDeathBlockZ == 10.0f)
                {
                    g_bAroundTheCorner1 = true;
                }
            }

        }
    }

    //頂点データの範囲をロックし、頂点バッファへのポインタを取得
    g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);//この書式は変えないこと

    for (fIndex = 0; fIndex < MAX_TIME; fIndex++)
    {
        int nTime = (int)powf(fRadix, MAX_TIME - fIndex);
        int nTime2 = (int)powf(fRadix, MAX_TIME - fIndex - 1);
        int nAnswer = g_nTimeSecond % nTime / nTime2;

        //テクスチャの座標
        pVtx[4].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 0.0f);
        pVtx[5].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 0.0f);
        pVtx[6].tex = D3DXVECTOR2(0.0f + (0.1f* nAnswer), 1.0f);
        pVtx[7].tex = D3DXVECTOR2(0.1f + (0.1f* nAnswer), 1.0f);

        pVtx -= 4;//頂点データのポインタを4つ分進める
    }

    //頂点データアンロック
    g_pVtxBuffTime->Unlock();
}
//=============================================================================
// タイム終了処理
// Author:後藤慎之助
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
// Author:後藤慎之助
//=============================================================================
void DrawTime(void)
{

    //頂点フォーマットの設定
    LPDIRECT3DDEVICE9 pDevice = GetDevice();

    //アルファテストを有効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //アルファテスト基準値の設定
    pDevice->SetRenderState(D3DRS_ALPHAREF, 30);
    //アルファテストの比較方法の設定（GREATERは基準値より大きい場合)
    pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

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

    //アルファテストを無効化
    pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

    //テクスチャの設定
    pDevice->SetTexture(0, NULL);
}