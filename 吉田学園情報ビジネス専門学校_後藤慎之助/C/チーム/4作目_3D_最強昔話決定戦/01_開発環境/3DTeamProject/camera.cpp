//=============================================================================
//
// 最強昔話決定戦の処理 [camera.cpp] (カメラ)
// Author : 後藤慎之助
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "controller.h"
#include "sound.h"
#include "player.h"
#include "setting.h"
#include "controller.h"
#include "boss.h"
#include "ui.h"

//=============================================================================
// グローバル変数宣言
//=============================================================================
CAMERA g_Camera;	//カメラ情報

//=============================================================================
// カメラ初期化処理
// Author:後藤慎之助
//=============================================================================
void InitCamera(void)
{
    // カメラの位置・注視点・上方向を設定する
    g_Camera.posV = D3DXVECTOR3(CAMERA_POS_X, CAMERA_POS_Y, CAMERA_POS_Z);      //位置
    g_Camera.posR = D3DXVECTOR3(CAMERA_ROT_X, CAMERA_ROT_Y, CAMERA_ROT_Z);      //注視点
    g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);          //法線ベクトル
    g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);           //向き


    //構造体の初期化
    g_Camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_Camera.state = CAMERASTATE_NORMAL;
    g_Camera.nCntReachBoss = 0;
    g_Camera.bBossBattle = false;

    // 距離
    g_Camera.fDistance = sqrtf(
        powf((g_Camera.posV.x - g_Camera.posR.x), 2) +
        powf((g_Camera.posV.z - g_Camera.posR.z), 2));

    //ストーリーかどうか
    SETTING* pSetting = GetSetting();
    if (pSetting->nMaxPlayer == 1)
    {
        g_Camera.state = CAMERASTATE_STORY;
    }
}

//=============================================================================
// カメラ終了処理
// Author:後藤慎之助
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// カメラ更新処理
// Author:後藤慎之助
//=============================================================================
void UpdateCamera(void)
{
    //自機の取得
    PLAYER*pPlayer = GetPlayer();

    //ストーリー時の、自機への追従
    switch (g_Camera.state)
    {
    case CAMERASTATE_STORY:

        //カメラの位置を自機に追従させるための、目的値
        g_Camera.posVDest = D3DXVECTOR3(pPlayer->pos.x - sinf((g_Camera.rot.z)) * g_Camera.fDistance, 
            g_Camera.posV.y, (pPlayer->pos.z - cosf((g_Camera.rot.z))* g_Camera.fDistance));

        //注視点を自機に追従させるための、目的値
        g_Camera.posRDest = D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.z)), g_Camera.posR.y,
            (pPlayer->pos.z + cosf(pPlayer->rot.z)));

        //カメラの位置を、目的値に近づける
        g_Camera.posV.x += (g_Camera.posVDest.x - g_Camera.posV.x) * CAMERA_MOVE;

        //カメラの注視点を、目的値に近づける
        g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * CAMERA_MOVE;

        break;

    case CAMERASTATE_REACH_BOSS:

        //ボス注視のカウント
        g_Camera.nCntReachBoss++;

        // 注視点をボスのほうへ移動する
        g_Camera.posRDest = D3DXVECTOR3(BOSS_FIRST_POS_X, CAMERA_BOSS_REACH_Y + 50.0f
            , CAMERA_BOSS_REACH_Z);

        // カメラの位置をボスのほうへ移動する
        g_Camera.posVDest = D3DXVECTOR3(CAMERA_BOSS_REACH_X, CAMERA_BOSS_REACH_Y, CAMERA_BOSS_REACH_Z);

        // カメラの位置を目的値へ近づける
        g_Camera.posV += (g_Camera.posVDest - g_Camera.posV) * CAMERA_MOVE;

        // カメラの向きを目的値へ近づける
        g_Camera.posR += (g_Camera.posRDest - g_Camera.posR) * CAMERA_MOVE;

        //1秒後に咆哮のモーション
        if (g_Camera.nCntReachBoss == CAMERA_BOSS_TIME / 3)
        {
            BOSS *pBoss = GetBoss();
            pBoss->MotionState = MOTION_ROAR_BOSS;
        }

        //1.5秒後に咆哮音
        else if (g_Camera.nCntReachBoss == CAMERA_BOSS_TIME / 2)
        {
            PlaySound(SOUND_LABEL_SE_ROAR);
        }

        //注視をやめるとともに、カメラ外で処理を進める
        else if (g_Camera.nCntReachBoss > CAMERA_BOSS_TIME)
        {
            //位置と注視点を戻す
            g_Camera.posV.y = CAMERA_POS_Y;
            g_Camera.posV.z = CAMERA_POS_Z;
            g_Camera.posR.y = CAMERA_ROT_Y;
            g_Camera.posR.z = CAMERA_ROT_Z;

            //ボスのバー
            SetUI(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT / 2, 0.0f), 50.0f, 500.0f, UITYPE_020);                               //ボスのHPバーの背景
            SetUI(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT / 2, 0.0f), 50.0f - BAR_EXTEND, 500.0f - BAR_EXTEND, UITYPE_021);     //ボスのHPバー
            SetUI(D3DXVECTOR3(SCREEN_WIDTH - 50.0f, 90.0f, 0.0f), 70.0f, 50.0f, UITYPE_022);                                            //BOSSの文字

            //カメラをストーリーモードの状態へ
            g_Camera.state = CAMERASTATE_STORY;
            g_Camera.nCntReachBoss = 0;

            //ボス曲へ
            PlaySound(SOUND_LABEL_BGM_SUDDEN_DEATH);

            //ボス戦開始
            g_Camera.bBossBattle = true;
        }

        break;
    }
}

//=============================================================================
// カメラの設定
// Author:後藤慎之助
//=============================================================================
void SetCamera(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスへのポインタ

                                                // プロジェクションマトリックスの初期化
    D3DXMatrixIdentity(&g_Camera.mtxProjection);
    // プロジェクションマトリックスの作成
    D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection,
        D3DXToRadian(45.0f),
        (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
        10.0f,
        CAMERA_VIEW_RANGE);		// 視野
                                // プロジェクションマトリックスの設定
    pDevice->SetTransform(D3DTS_PROJECTION,
        &g_Camera.mtxProjection);

    // ビューマトリックスの初期化
    D3DXMatrixIdentity(&g_Camera.mtxView);
    // ビューマトリックスの作成
    D3DXMatrixLookAtLH(&g_Camera.mtxView,
        &g_Camera.posV,
        &g_Camera.posR,
        &g_Camera.vecU);
    // ビューマトリックスの設定
    pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

//=============================================================================
// カメラの状況を取得
// Author:後藤慎之助
//=============================================================================
CAMERA * GetCamera(void)
{
    return &g_Camera;
}