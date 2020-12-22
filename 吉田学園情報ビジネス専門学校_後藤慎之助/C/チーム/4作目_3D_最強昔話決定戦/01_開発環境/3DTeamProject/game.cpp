//=============================================================================
//
// 最強昔話決定戦の処理 [game.cpp] (ゲーム画面)
// Author:後藤慎之助
//
//=============================================================================

//=============================================================================
// インクルードファイル
//=============================================================================
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "stage.h"
#include "block.h"
#include "camera.h"
#include "light.h"
#include "meshfield.h"
#include "player.h"
#include "player2.h"
#include "player3.h"
#include "player4.h"
#include "bomb.h"
#include "fire.h"
#include "item.h"
#include "effect.h"
#include "pause.h"
#include "controller.h"
#include "ui.h"
#include "time.h"
#include "shadow.h"
#include "setting.h"
#include "score.h"
#include "enemy.h"
#include "bullet.h"
#include "boss.h"

//=============================================================================
//グローバル変数宣言
//=============================================================================

//ブロックのランダム配置
int g_nCntBrokenBlock;  //ただの壊せるブロックの数
int g_nCntFireBlock;    //炎出現ブロックの数
int g_nCntBombBlock;    //ボム出現ブロックの数
int g_nCntSpeedBlock;   //スピード出現ブロックの数
int g_nCntKickBlock;    //キック出現ブロックの数

//ゲームの管理
GAME g_Game;            //ゲームの情報
bool g_bFinishRound;    //ラウンド終了判定
bool g_bFinishGame;     //ゲーム終了判定
int g_nCunterGamestate;	//次の画面へ行くまでの時間

//=============================================================================
// ゲーム初期化処理
// Author:後藤慎之助
//=============================================================================
void InitGame(void)
{
    //各プレイヤーのポイントの初期化(最初のみ)
    g_Game.nCntPointPlayer1 = 0;
    g_Game.nCntPointPlayer2 = 0;
    g_Game.nCntPointPlayer3 = 0;
    g_Game.nCntPointPlayer4 = 0;

    //一人用モードを選択しても、設定が変わらないように、設定を記憶
    //設定の情報取得
    SETTING *pSetting = GetSetting();
    g_Game.nMemoryTimeLimit = pSetting->nTimeLimit;
    g_Game.bMemoryRandomStartPos = pSetting->bRandomStartPos;
    g_Game.bMemorySuddenDeath = pSetting->bSuddenDeath;

    ResetScore();   //スコアの初期化
    ResetGame();    //ゲームの初期化
}

//=============================================================================
// 次のラウンドへの初期化処理
// Author:後藤慎之助
//=============================================================================
void InitNextGame(void)
{
    ResetScore();   //スコアの初期化(バトルロイヤルではスコアがいらないので初期化)
    ResetGame();    //ゲームの初期化
}

//=============================================================================
// ゲーム更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateGame(void)
{
    //ポーズの情報取得
    PAUSE *pPause = GetPause();

    //設定の情報取得
    SETTING *pSetting = GetSetting();

    switch (g_Game.state)
    {
        //開始状態
    case GAMESTATE_START:
        UpdateCamera();
        UpdateLight();
        UpdateBlock();
        UpdateBomb();
        UpdateFire();
        UpdateItem();
        UpdateEffect();
        UpdatePlayer();
        if (pSetting->nMaxPlayer >= 2)
        {
            UpdatePlayer2();
            if (pSetting->nMaxPlayer >= 3)
            {
                UpdatePlayer3();
                if (pSetting->nMaxPlayer >= 4)
                {
                    UpdatePlayer4();
                }
            }
        }
        UpdateUI();
        UpdateShadow();
        UpdateScore();
        //ストーリーなら
        if (pSetting->nMaxPlayer == 1)
        {
            UpdateEnemy();
            UpdateBullet();
            UpdateBoss();
        }

        g_nCunterGamestate++;
        if (g_nCunterGamestate >= COUNT_WAIT_START_GAME)
        {
            SetGameState(GAMESTATE_NORMAL);    //ゲーム中にする
                     
            PlaySound(SOUND_LABEL_SE_START);    //スタートの笛
            PlaySound(SOUND_LABEL_BGM_GAME);    //ゲームのBGM
        }
        break;

        //ゲーム中
    case GAMESTATE_NORMAL:
        UpdateCamera();
        UpdateLight();
        UpdateBlock();
        UpdateBomb();
        UpdateFire();
        UpdateItem();
        UpdateEffect();
        UpdatePlayer();
        if (pSetting->nMaxPlayer >= 2)
        {
            UpdatePlayer2();
            if (pSetting->nMaxPlayer >= 3)
            {
                UpdatePlayer3();
                if (pSetting->nMaxPlayer >= 4)
                {
                    UpdatePlayer4();
                }
            }
        }
        UpdateUI();
        UpdateTime();
        UpdateShadow();
        UpdateScore();
        //ストーリーなら
        if (pSetting->nMaxPlayer == 1)
        {
            UpdateEnemy();
            UpdateBullet();
            UpdateBoss();
        }

        //ポーズへ
        if (GetJoypadTrigger(PLAYER_1, BUTTON_START))
        {
            PlaySound(SOUND_LABEL_SE_DECISION);   //決定音
            pPause->who = SELECT_MOMO;
            g_Game.state = GAMESTATE_PAUSE;
        }
        else if (GetJoypadTrigger(PLAYER_2, BUTTON_START))
        {
            if (pSetting->nMaxPlayer >= 2)
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //決定音
                pPause->who = SELECT_URA;
                g_Game.state = GAMESTATE_PAUSE;
            }
        }
        else if (GetJoypadTrigger(PLAYER_3, BUTTON_START))
        {
            if (pSetting->nMaxPlayer >= 3)
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //決定音
                pPause->who = SELECT_KIN;
                g_Game.state = GAMESTATE_PAUSE;
            }
        }
        else if (GetJoypadTrigger(PLAYER_4, BUTTON_START))
        {
            if (pSetting->nMaxPlayer >= 4)
            {
                PlaySound(SOUND_LABEL_SE_DECISION);   //決定音
                pPause->who = SELECT_ISUN;
                g_Game.state = GAMESTATE_PAUSE;
            }
        }

        break;

    	//次のラウンドへ
    case GAMESTATE_FINSIH_ROUND:
        UpdateCamera();
        UpdateLight();
        UpdateBlock();
        UpdateBomb();
        UpdateFire();
        UpdateItem();
        UpdateEffect();
        UpdatePlayer();
        if (pSetting->nMaxPlayer >= 2)
        {
            UpdatePlayer2();
            if (pSetting->nMaxPlayer >= 3)
            {
                UpdatePlayer3();
                if (pSetting->nMaxPlayer >= 4)
                {
                    UpdatePlayer4();
                }
            }
        }
        UpdateUI();
        UpdateShadow();
        UpdateScore();
        //ストーリーなら
        if (pSetting->nMaxPlayer == 1)
        {
            UpdateEnemy();
            UpdateBullet();
            UpdateBoss();
        }

    	g_nCunterGamestate++;
    	if (g_nCunterGamestate >= COUNT_WAIT_FINISH_GAME)
    	{
            g_Game.state = GAMESTATE_NONE;
    		SetFade(MODE_NEXT_GAME);                    //次のラウンドへ
            StopSound(SOUND_LABEL_BGM_GAME);            //ゲーム曲を停止
            StopSound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //サドンデスの音楽を停止
    	}
    	break;

        //ゲーム終了
    case GAMESTATE_FINSIH_GAME:
        UpdateCamera();
        UpdateLight();
        UpdateBlock();
        UpdateBomb();
        UpdateFire();
        UpdateItem();
        UpdateEffect();
        UpdatePlayer();
        if (pSetting->nMaxPlayer >= 2)
        {
            UpdatePlayer2();
            if (pSetting->nMaxPlayer >= 3)
            {
                UpdatePlayer3();
                if (pSetting->nMaxPlayer >= 4)
                {
                    UpdatePlayer4();
                }
            }
        }
        UpdateUI();
        UpdateShadow();
        UpdateScore();
        //ストーリーなら
        if (pSetting->nMaxPlayer == 1)
        {
            UpdateEnemy();
            UpdateBullet();
            UpdateBoss();
        }

        g_nCunterGamestate++;
        if (g_nCunterGamestate >= COUNT_WAIT_FINISH_GAME)
        {
            //一人用で変わった設定を戻す
            pSetting->nTimeLimit = g_Game.nMemoryTimeLimit;
            pSetting->bRandomStartPos = g_Game.bMemoryRandomStartPos;
            pSetting->bSuddenDeath = g_Game.bMemorySuddenDeath;

            g_Game.state = GAMESTATE_NONE;
            SetFade(MODE_RESULT);                       //リザルトへ
            StopSound(SOUND_LABEL_BGM_GAME);            //ゲーム曲を停止
            StopSound(SOUND_LABEL_BGM_SUDDEN_DEATH);    //サドンデスの音楽を停止
        }
        break;

        //ポーズ
    case GAMESTATE_PAUSE:
        UpdatePause();

        break;
    }

    //マルチプレイ時
    if (pSetting->nMaxPlayer >= 2)
    {
        //ラウンド終了判定
        if (g_bFinishRound == false)
        {
            //誰かが勝った場合
            if (g_Game.nCounter == 1)
            {
                PlaySound(SOUND_LABEL_SE_FINISH);    //フィニッシュの効果音

                                                     //2度処理を通らなくする
                g_bFinishRound = true;

                //各プレイヤーの取得
                PLAYER*pPlayer = GetPlayer();
                PLAYER2*pPlayer2 = GetPlayer2();
                PLAYER3*pPlayer3 = GetPlayer3();
                PLAYER4*pPlayer4 = GetPlayer4();

                //誰が勝ったか
                if (pPlayer->bDisp == true)
                {
                    g_Game.nCntPointPlayer1++;
                }
                else if (pPlayer2->bDisp == true)
                {
                    g_Game.nCntPointPlayer2++;
                }
                else if (pPlayer3->bDisp == true)
                {
                    g_Game.nCntPointPlayer3++;
                }
                else if (pPlayer4->bDisp == true)
                {
                    g_Game.nCntPointPlayer4++;
                }

                //次のラウンドへ
                SetGameState(GAMESTATE_FINSIH_ROUND);
            }

            //引き分けの場合
            else if (g_Game.nCounter <= 0)
            {
                SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 400.0f, 200.0f, UITYPE_012);  //引き分け

                PlaySound(SOUND_LABEL_SE_FINISH);    //フィニッシュの効果音

                                                     //2度処理を通らなくする
                g_bFinishRound = true;

                //次のラウンドへ
                SetGameState(GAMESTATE_FINSIH_ROUND);
            }
        }

        //ゲーム終了判定
        if (g_bFinishGame == false)
        {
            //プレイヤー1が勝った場合
            if (g_Game.nCntPointPlayer1 == pSetting->nMaxRound)
            {
                //2度処理を通らなくする
                g_bFinishGame = true;

                //プレイヤー1が勝った
                g_Game.winner = SELECT_MOMO;

                //ゲーム終了
                SetGameState(GAMESTATE_FINSIH_GAME);
            }

            //プレイヤー2が勝った場合
            else if (g_Game.nCntPointPlayer2 == pSetting->nMaxRound)
            {
                //2度処理を通らなくする
                g_bFinishGame = true;

                //プレイヤー1が勝った
                g_Game.winner = SELECT_URA;

                //ゲーム終了
                SetGameState(GAMESTATE_FINSIH_GAME);
            }

            //プレイヤー3が勝った場合
            else if (g_Game.nCntPointPlayer3 == pSetting->nMaxRound)
            {
                //2度処理を通らなくする
                g_bFinishGame = true;

                //プレイヤー1が勝った
                g_Game.winner = SELECT_KIN;

                //ゲーム終了
                SetGameState(GAMESTATE_FINSIH_GAME);
            }

            //プレイヤー4が勝った場合
            else if (g_Game.nCntPointPlayer4 == pSetting->nMaxRound)
            {
                //2度処理を通らなくする
                g_bFinishGame = true;

                //プレイヤー1が勝った
                g_Game.winner = SELECT_ISUN;

                //ゲーム終了
                SetGameState(GAMESTATE_FINSIH_GAME);
            }
        }
    }

}

//=============================================================================
// ゲーム終了処理
// Author:後藤慎之助
//=============================================================================
void UninitGame(void)
{
    //設定の情報取得
    SETTING *pSetting = GetSetting();

    UninitPause();
    UninitCamera();
    UninitLight();
    UninitMeshField();
    UninitBlock();
    UninitBomb();
    UninitFire();
    UninitItem();
    UninitEffect();
    UninitPlayer();
    if (pSetting->nMaxPlayer >= 2)
    {
        UninitPlayer2();
        if (pSetting->nMaxPlayer >= 3)
        {
            UninitPlayer3();
            if (pSetting->nMaxPlayer >= 4)
            {
                UninitPlayer4();
            }
        }
    }
    UninitUI();
    UninitTime();
    UninitShadow();
    UninitScore();
    //ストーリーなら
    if (pSetting->nMaxPlayer == 1)
    {
        UninitEnemy();
        UninitBullet();
        UninitBoss();
    }
}

//=============================================================================
// ゲーム描画処理
// Author:後藤慎之助
//=============================================================================
void DrawGame(void)
{
    //設定の情報取得
    SETTING *pSetting = GetSetting();

    SetCamera();
    DrawMeshField();
    DrawBlock();
    DrawBomb();
    DrawFire();
    DrawItem();
    DrawPlayer();
    if (pSetting->nMaxPlayer >= 2)
    {
        DrawPlayer2();
        if (pSetting->nMaxPlayer >= 3)
        {
            DrawPlayer3();
            if (pSetting->nMaxPlayer >= 4)
            {
                DrawPlayer4();
            }
        }
    }
    DrawUI();
    DrawTime();
    DrawShadow();
    //ストーリーなら描画
    if (pSetting->nMaxPlayer == 1)
    {
        DrawScore();
        DrawEnemy();
        DrawBoss();
        DrawBullet();
    }
    DrawEffect();

    //ポーズの描画
    if (g_Game.state == GAMESTATE_PAUSE)
    {
        DrawPause();
    }
}

//=============================================================================
// ゲーム状態の設定
// Author:後藤慎之助
//=============================================================================
void SetGameState(GAMESTATE state)
{
    g_Game.state = state;
    g_nCunterGamestate = 0;
}

//=============================================================================
// ゲーム状況の取得
// Author:後藤慎之助
//=============================================================================
GAME*GetGame(void)
{
    return &g_Game;
}

//=============================================================================
// 共通の初期化内容
// Author:後藤慎之助
//=============================================================================
void ResetGame(void)
{
    //乱数の種の初期化
    srand((unsigned int)time(0));

    //設定の情報取得
    SETTING *pSetting = GetSetting();

    //ストーリーなら、ここで設定を固定化する
    if (pSetting->nMaxPlayer == 1)
    {
        pSetting->nTimeLimit = 5;               //制限時間5分
        pSetting->bRandomStartPos = false;      //開始位置は固定
        pSetting->bSuddenDeath = false;         //サドンデスなし
    }

    //構造体の初期化
    g_Game.nCounter = pSetting->nMaxPlayer;                                             //プレイヤーの人数
    g_Game.state = GAMESTATE_START;                                                     //開始状態にする
    g_Game.winner = SELECT_MAX;                                                         //勝者を初期化

    //それぞれのプレイヤーの初期位置
    g_Game.startPos1 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
    g_Game.startPos2 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

    //ストーリーなら、プレイヤー1の初期位置を変える
    if (pSetting->nMaxPlayer == 1)
    {
        g_Game.startPos1 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z / 2 + GRID_SIZE / 2);
    }

    //開始位置がランダムなら
    if (pSetting->bRandomStartPos == true)
    {
        int nRandamPosPlayer1 = 0;    //プレイヤー1の初期位置
        int nRandamPosPlayer2 = 0;    //プレイヤー2の初期位置
        int nRandamPosPlayer3 = 0;    //プレイヤー3の初期位置
        int nRandamPosPlayer4 = 0;    //プレイヤー4の初期位置

        nRandamPosPlayer1 = rand() % 4 + 1;    //1～4を得る

        //プレイヤーの初期位置を決める
        if (nRandamPosPlayer1 == 1)
        {
            g_Game.startPos1 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

            nRandamPosPlayer2 = rand() % 3 + 1;    //1～3を得る

            if (nRandamPosPlayer2 == 1)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 2)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 3)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
        }
        else if (nRandamPosPlayer1 == 2)
        {
            g_Game.startPos1 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

            nRandamPosPlayer2 = rand() % 3 + 1;    //1～3を得る

            if (nRandamPosPlayer2 == 1)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 2)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 3)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
        }
        else if (nRandamPosPlayer1 == 3)
        {
            g_Game.startPos1 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

            nRandamPosPlayer2 = rand() % 3 + 1;    //1～3を得る

            if (nRandamPosPlayer2 == 1)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 2)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 3)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
            }
        }
        else if (nRandamPosPlayer1 == 4)
        {
            g_Game.startPos1 = D3DXVECTOR3(PLAYER4_FIRST_POS_X, 0.0f, PLAYER4_FIRST_POS_Z);

            nRandamPosPlayer2 = rand() % 3 + 1;    //1～3を得る

            if (nRandamPosPlayer2 == 1)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 2)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
            }
            else if (nRandamPosPlayer2 == 3)
            {
                g_Game.startPos2 = D3DXVECTOR3(PLAYER_FIRST_POS_X, 0.0f, PLAYER_FIRST_POS_Z);

                nRandamPosPlayer3 = rand() % 2 + 1;    //1～2を得る

                if (nRandamPosPlayer3 == 1)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);
                }
                else if (nRandamPosPlayer3 == 2)
                {
                    g_Game.startPos3 = D3DXVECTOR3(PLAYER2_FIRST_POS_X, 0.0f, PLAYER2_FIRST_POS_Z);

                    g_Game.startPos4 = D3DXVECTOR3(PLAYER3_FIRST_POS_X, 0.0f, PLAYER3_FIRST_POS_Z);
                }
            }
        }
    }

    //グローバル変数の初期化
    g_nCunterGamestate = 0;
    g_nCntBrokenBlock = 0;
    g_nCntFireBlock = 0;
    g_nCntBombBlock = 0;
    g_nCntSpeedBlock = 0;
    g_nCntKickBlock = 0;
    g_bFinishRound = false;
    g_bFinishGame = false;

    //プレイヤーの使用状況の初期化
    InitDispPlayer();
    InitDispPlayer2();
    InitDispPlayer3();
    InitDispPlayer4();

    //乱数で破壊できるブロックを作るとき用の変数
    float posX = 0.0f;
    float posZ = 0.0f;
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    InitPause();
    InitShadow();
    InitCamera();
    InitLight();
    InitBlock();
    InitBomb();
    InitFire();
    InitStage();
    InputStageData();    //ステージの読み込みは、ブロックの初期化が終わった後にやる
    InitMeshField();
    InitPlayer();
    if (pSetting->nMaxPlayer >= 2)
    {
        InitPlayer2();
        if (pSetting->nMaxPlayer >= 3)
        {
            InitPlayer3();
            if (pSetting->nMaxPlayer >= 4)
            {
                InitPlayer4();
            }
        }
    }
    InitItem();
    InitEffect();
    InitUI();
    InitTime();
    InitScore();
    //ストーリーなら
    if (pSetting->nMaxPlayer == 1)
    {
        InitEnemy();
        InitBullet();
        InitBoss();
    }

    //UIの配置
    SetUI(D3DXVECTOR3(610.0f, 43.0f, 0.0f), TIME_SIZE_X, TIME_SIZE_Y, UITYPE_000);             //制限時間の分
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, 43.0f, 0.0f), TIME_SIZE_X, TIME_SIZE_Y, UITYPE_001);   //:
    if (pSetting->nMaxPlayer >= 2)
    {
        SetUI(D3DXVECTOR3(140.0f, 200.0f, 0.0f), 200.0f, 200.0f, UITYPE_002);                                   //プレイヤー1
        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 200.0f, 0.0f), 200.0f, 200.0f, UITYPE_003);                    //プレイヤー2

        SetUI(D3DXVECTOR3(140.0f, 325.0f, 0.0f), 200.0f, 50.0f, UITYPE_006);                                    //プレイヤー1の得点
        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 325.0f, 0.0f), 200.0f, 50.0f, UITYPE_007);                     //プレイヤー2の得点

        if (pSetting->nMaxPlayer >= 3)
        {
            SetUI(D3DXVECTOR3(140.0f, SCREEN_HEIGHT - 190.0f, 0.0f), 200.0f, 200.0f, UITYPE_004);                   //プレイヤー3

            SetUI(D3DXVECTOR3(140.0f, SCREEN_HEIGHT - 65.0f, 0.0f), 200.0f, 50.0f, UITYPE_008);                     //プレイヤー3の得点

            if (pSetting->nMaxPlayer >= 4)
            {
                SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, SCREEN_HEIGHT - 190.0f, 0.0f), 200.0f, 200.0f, UITYPE_005);    //プレイヤー4

                SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, SCREEN_HEIGHT - 65.0f, 0.0f), 200.0f, 50.0f, UITYPE_009);      //プレイヤー4の得点
            }
        }

        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 40.0f, 0.0f), 240.0f, 60.0f, UITYPE_013);                  //nセット先取
    }
    SetUI(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 400.0f, 200.0f, UITYPE_010);              //よーいドン
    SetUI(D3DXVECTOR3(140.0f, 35.0f, 0.0f), 240.0f, 60.0f, UITYPE_014);                                     //バトルロイヤル/ストーリー

    //ストーリーのUI/敵
    if (pSetting->nMaxPlayer == 1)
    {
        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 120.0f, 35.0f, 0.0f), 120.0f, 60.0f, UITYPE_015);                                   //ふつう/鬼
        SetUI(D3DXVECTOR3(SCREEN_WIDTH / 4 + 115.0f, 44.0f, 0.0f), 250.0f, 50.0f, UITYPE_016);                               //HPバーの背景
        SetUI(D3DXVECTOR3(SCREEN_WIDTH / 4 + 115.0f, 44.0f, 0.0f), 250.0f - BAR_EXTEND, 50.0f - BAR_EXTEND, UITYPE_017);     //HPバー
        SetUI(D3DXVECTOR3(SCREEN_WIDTH / 5 + 25.0f, 44.0f, 0.0f), 60.0f, 40.0f, UITYPE_018);                                 //HPの文字
        SetUI(D3DXVECTOR3(SCREEN_WIDTH - 358.0f, 48.0f, 0.0f), 320.0f, 60.0f, UITYPE_019);                                   //スコアの背景

        SetEnemy(D3DXVECTOR3(1400.0f, 0.0f, 1000.0f), D3DXVECTOR3(ENEMY_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_GHOST);         //ゴースト
        SetEnemy(D3DXVECTOR3(1400.0f, 0.0f, 200.0f), D3DXVECTOR3(ENEMY_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_GHOST);          //ゴースト
        SetEnemy(D3DXVECTOR3(1400.0f, 0.0f, 600.0f), D3DXVECTOR3(ENEMY_UMBRELLA_SPEED, 0.0f, 0.0f), ENEMYTYPE_UMBRELLA);    //アンブレラ
        SetEnemy(D3DXVECTOR3(2300.0f, 0.0f, 900.0f), D3DXVECTOR3(ENEMY_UMBRELLA_SPEED, 0.0f, 0.0f), ENEMYTYPE_UMBRELLA);    //アンブレラ
        SetEnemy(D3DXVECTOR3(2300.0f, 0.0f, 300.0f), D3DXVECTOR3(ENEMY_UMBRELLA_SPEED, 0.0f, 0.0f), ENEMYTYPE_UMBRELLA);    //アンブレラ
        SetEnemy(D3DXVECTOR3(3000.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_CANNON);                      //キャノン
        SetEnemy(D3DXVECTOR3(3000.0f, 0.0f, 800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ENEMYTYPE_CANNON);                      //キャノン
        SetEnemy(D3DXVECTOR3(3600.0f, 0.0f, 700.0f), D3DXVECTOR3(ENEMY_RED_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_RED_GHOST);  //レッドゴースト
        SetEnemy(D3DXVECTOR3(3700.0f, 0.0f, 1100.0f), D3DXVECTOR3(ENEMY_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_GHOST);         //ゴースト
        SetEnemy(D3DXVECTOR3(4000.0f, 0.0f, 300.0f), D3DXVECTOR3(ENEMY_RED_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_RED_GHOST);  //レッドゴースト
        SetEnemy(D3DXVECTOR3(4300.0f, 0.0f, 100.0f), D3DXVECTOR3(ENEMY_UMBRELLA_SPEED, 0.0f, 0.0f), ENEMYTYPE_UMBRELLA);    //アンブレラ
        SetEnemy(D3DXVECTOR3(4400.0f, 0.0f, 700.0f), D3DXVECTOR3(ENEMY_RED_GHOST_SPEED, 0.0f, 0.0f), ENEMYTYPE_RED_GHOST);  //レッドゴースト

        //アイテムの配置
        //ハートは難易度ノーマルなら出現
        if (pSetting->nLevel == SETTINGLEVEL_NORMAL)
        {
            SetItem(D3DXVECTOR3(3200.0f, GRID_SIZE / 2.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_004);
            SetItem(D3DXVECTOR3(4700.0f, GRID_SIZE / 2.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_004);
        }
        //タイマー
        SetItem(D3DXVECTOR3(3300.0f, GRID_SIZE / 2.0f, 600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), ITEMTYPE_005);
    }

    //四つ角と、付近のブロックにもグリッドの使用情報をいれる(壊せるブロックを配置できないようにするため)
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 1100.0f), true);  //左上
    SetGridInfo(D3DXVECTOR3(200.0f, 0.0f, 1100.0f), true);
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 1000.0f), true);

    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 1100.0f), true);  //右上
    SetGridInfo(D3DXVECTOR3(1200.0f, 0.0f, 1100.0f), true);
    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 1000.0f), true);

    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 100.0f), true);  //左下
    SetGridInfo(D3DXVECTOR3(200.0f, 0.0f, 100.0f), true);
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 200.0f), true);

    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 100.0f), true);  //右下
    SetGridInfo(D3DXVECTOR3(1200.0f, 0.0f, 100.0f), true);
    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 200.0f), true);

    //マルチプレイ時は、ランダムに、ただの壊せるブロックを設置
    if (pSetting->nMaxPlayer >= 2)
    {
        while (g_nCntBrokenBlock != MAX_BROKEN_BLOCK
            - MAX_FIRE_BLOCK - MAX_BOMB_BLOCK - MAX_SPEED_BLOCK - MAX_KICK_BLOCK)//設置しきるまでループ(設置数が92 - アイテム数でないならループ)
        {
            //ランダムにX、Z座標を決める
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //グリッド座標に変換する
            pos = ConversionGrid(&pos);

            //そのグリッドに配置物がないなら配置
            if (ConfirmationGrid(pos) == false)
            {
                //破壊できるブロックを配置
                SetBlock(pos, BLOCKTYPE_001);
                g_nCntBrokenBlock++;
            }
        }

        //ランダムに、炎出現ブロックを設置
        while (g_nCntFireBlock != MAX_FIRE_BLOCK)//設置しきるまでループ
        {
            //ランダムにX、Z座標を決める
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //グリッド座標に変換する
            pos = ConversionGrid(&pos);

            //そのグリッドに配置物がないなら配置
            if (ConfirmationGrid(pos) == false)
            {
                //炎出現ブロックを配置
                SetBlock(pos, BLOCKTYPE_002);
                g_nCntFireBlock++;
            }
        }

        //ランダムに、ボム出現ブロックを設置
        while (g_nCntBombBlock != MAX_BOMB_BLOCK)//設置しきるまでループ
        {
            //ランダムにX、Z座標を決める
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //グリッド座標に変換する
            pos = ConversionGrid(&pos);

            //そのグリッドに配置物がないなら配置
            if (ConfirmationGrid(pos) == false)
            {
                //ボム出現ブロックを配置
                SetBlock(pos, BLOCKTYPE_003);
                g_nCntBombBlock++;
            }
        }

        //ランダムに、スピード出現ブロックを設置
        while (g_nCntSpeedBlock != MAX_SPEED_BLOCK)//設置しきるまでループ
        {
            //ランダムにX、Z座標を決める
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //グリッド座標に変換する
            pos = ConversionGrid(&pos);

            //そのグリッドに配置物がないなら配置
            if (ConfirmationGrid(pos) == false)
            {
                //スピード出現ブロックを配置
                SetBlock(pos, BLOCKTYPE_004);
                g_nCntSpeedBlock++;
            }
        }

        //ランダムに、キック出現ブロックを設置
        while (g_nCntKickBlock != MAX_KICK_BLOCK)//設置しきるまでループ
        {
            //ランダムにX、Z座標を決める
            posX = float(rand() % 1200 + 100);
            posZ = float(rand() % 1000 + 100);
            pos = D3DXVECTOR3(posX, 0.0f, posZ);

            //グリッド座標に変換する
            pos = ConversionGrid(&pos);

            //そのグリッドに配置物がないなら配置
            if (ConfirmationGrid(pos) == false)
            {
                //キック出現ブロックを配置
                SetBlock(pos, BLOCKTYPE_005);
                g_nCntKickBlock++;
            }
        }
    }

    //四つ角と、付近のブロックのグリッド情報を解除(プレイヤーがデスしたとき、アイテムを散りばめるため)
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 1100.0f), false);  //左上
    SetGridInfo(D3DXVECTOR3(200.0f, 0.0f, 1100.0f), false);
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 1000.0f), false);

    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 1100.0f), false);  //右上
    SetGridInfo(D3DXVECTOR3(1200.0f, 0.0f, 1100.0f), false);
    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 1000.0f), false);

    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 100.0f), false);  //左下
    SetGridInfo(D3DXVECTOR3(200.0f, 0.0f, 100.0f), false);
    SetGridInfo(D3DXVECTOR3(100.0f, 0.0f, 200.0f), false);

    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 100.0f), false);  //右下
    SetGridInfo(D3DXVECTOR3(1200.0f, 0.0f, 100.0f), false);
    SetGridInfo(D3DXVECTOR3(1300.0f, 0.0f, 200.0f), false);

    //開始人数に合わせた、開始時の掛け声
    if (g_Game.nCounter == 2)
    {
        int nStartSound = 0;    //ランダムに掛け声を流す

        nStartSound = rand() % 2 + 1;

        if (nStartSound == 1)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE);
        }
        else if (nStartSound == 2)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE2);
        }
    }
   
    else if (g_Game.nCounter == 3)
    {
        int nStartSound = 0;    //ランダムに掛け声を流す

        nStartSound = rand() % 3 + 1;

        if (nStartSound == 1)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE);
        }
        else if (nStartSound == 2)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE2);
        }
        else if (nStartSound == 3)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE3);
        }
    }

    else if (g_Game.nCounter == 4)
    {
        int nStartSound = 0;    //ランダムに掛け声を流す

        nStartSound = rand() % 4 + 1;

        if (nStartSound == 1)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE);
        }
        else if (nStartSound == 2)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE2);
        }
        else if (nStartSound == 3)
        {
            PlaySound(SOUND_LABEL_SE_START_MAN_VOICE3);
        }
        else if (nStartSound == 4)
        {
            PlaySound(SOUND_LABEL_SE_START_WOMAN_VOICE);
        }
    }
}