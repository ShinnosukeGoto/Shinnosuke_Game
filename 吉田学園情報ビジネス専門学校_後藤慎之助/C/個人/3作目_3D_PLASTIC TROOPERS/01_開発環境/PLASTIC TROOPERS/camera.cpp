//=============================================================================
//
// PLASTIC TROOPERSの処理 [camera.cpp] (カメラ)
// Author : 後藤慎之助
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "controller.h"
#include "effect.h"
#include "boss.h"
#include "sound.h"

//=============================================================================
// グローバル変数宣言
//=============================================================================
CAMERA g_Camera;	//カメラ情報

bool g_bUnderTrigger;   //十字キー下のトリガー
bool g_bLeftTrigger;    //十字キー左のトリガー
bool g_bRightTrigger;   //十字キー右のトリガー
float g_fAngleCamera;   //ロックオン時のカメラの向きを変える角度
float g_fCamera_posR_Height;  //カメラの注視点の高さ

int g_nCntReachBoss;    //ボスを注視するカウンター

//=============================================================================
// カメラ初期化処理
//=============================================================================
void InitCamera(void)
{
    // カメラの位置・注視点・上方向を設定する
    g_Camera.posV = D3DXVECTOR3(0.0f, CAMERA_POS_Y, CAMERA_POS_Z);      //位置
    g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          //注視点
    g_Camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);          //法線ベクトル
    g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);           //向き

    //グローバル変数の初期化
    g_bUnderTrigger = false;
    g_bLeftTrigger = false;
    g_bRightTrigger = false;
    g_fAngleCamera = 0.0f;
    g_fCamera_posR_Height = 0.0f;
    g_nCntReachBoss = 0;

    //構造体の初期化
    g_Camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    g_Camera.state = CAMERASTATE_NORMAL;
    g_Camera.bLockOn = false;
    g_Camera.bBossMode = false;

    // 距離
    g_Camera.fDistance = sqrtf(
        powf((g_Camera.posV.x - g_Camera.posR.x), 2) +
        powf((g_Camera.posV.z - g_Camera.posR.z), 2));
}

//=============================================================================
// カメラ終了処理
//=============================================================================
void UninitCamera(void)
{

}

//=============================================================================
// カメラ更新処理
//=============================================================================
void UpdateCamera(void)
{
    DIJOYSTATE2 Controller = GetController();
    PLAYER*pPlayer = GetPlayer();
    ENEMY*pEnemy = GetEnemy();
    BOSS*pBoss = GetBoss();

    //通常時
    if (g_Camera.state == CAMERASTATE_NORMAL)
    {
        //注視点の転回
        if (g_Camera.bLockOn == false)  //ロックオンしていないときに使える
        {
            if (Controller.lZ > 0)//右
            {
                g_Camera.rot.y += 0.05f;
            }
            if (Controller.lZ < 0)//左
            {
                g_Camera.rot.y -= 0.05f;
            }

            // 注視点をモデルが向いている方向へ移動する
            g_Camera.posRDest = D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.y)), g_Camera.posR.y,
                (pPlayer->pos.z + cosf(pPlayer->rot.y)));
        }

        //ロックオン
        else
        {
            //通常モード
            if (g_Camera.bBossMode == false)
            {
                for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
                {

                    //使われているなら
                    if (pEnemy->bUse == true)
                    {
                        //一番近い敵に向く
                        if (pEnemy->nRank == 1)
                        {
                            //距離が近いなら
                            if (pEnemy->fDistance < CONSIDER_CLOSE)
                            {
                                float fEposx = pEnemy->pos.x, fEposz = pEnemy->pos.z;		//敵の座標
                                float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	    //プレイヤーの座標

                                g_fAngleCamera = atan2f((fEposx - fPposx), (fEposz - fPposz));	//角度を決める

                                                                                                // 注視点をモデルが向いている方向へ移動する
                                g_Camera.posRDest = D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.y)), g_Camera.posR.y
                                    , (pPlayer->pos.z + cosf(pPlayer->rot.y))); //縦方向の調整

                                                                                //ターゲットマーク
                                EFFECT*pEffect = GetEffect();
                                for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
                                {
                                    //未使用なら
                                    if (pEffect->bUse == false)
                                    {
                                        SetEffect(D3DXVECTOR3(pEnemy->pos.x - sinf(g_fAngleCamera)*pEnemy->size.x, pEnemy->pos.y + pEnemy->size.y / 2, pEnemy->pos.z - cosf(g_fAngleCamera)*pEnemy->size.z), TARGET_SIZE
                                            , D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_000);
                                    }
                                }

                                break;
                            }
                        }
                    }

                }
            }

            //ボスモード
            else
            {
                float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//敵の座標
                float fPposx = pPlayer->pos.x, fPposz = pPlayer->pos.z;	    //プレイヤーの座標

                g_fAngleCamera = atan2f((fBposx - fPposx), (fBposz - fPposz));	//角度を決める

                // 注視点をモデルが向いている方向へ移動する
                g_Camera.posRDest = D3DXVECTOR3((pPlayer->pos.x + sinf(pPlayer->rot.y)), g_Camera.posR.y
                    , (pPlayer->pos.z + cosf(pPlayer->rot.y))); //縦方向の調整

                //ターゲットマーク
                EFFECT*pEffect = GetEffect();
                for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
                {
                    //未使用なら
                    if (pEffect->bUse == false)
                    {
                        SetEffect(D3DXVECTOR3(pBoss->pos.x - sinf(g_fAngleCamera)*pBoss->size.x, pBoss->pos.y + pBoss->size.y / 2, pBoss->pos.z - cosf(g_fAngleCamera)*pBoss->size.z), TARGET_SIZE
                            , D3DXVECTOR3(0.0f, 0.0f, 0.0f), EFFECTTYPE_000);
                    }
                }
            }

            //ロックオンした敵のほうを向く
            g_Camera.rot.y = g_fAngleCamera;    //横方向の調整

        }

        //カメラの位置をキャラに追従させる
        g_Camera.posVDest = D3DXVECTOR3(pPlayer->pos.x - sinf((g_Camera.rot.y)) * g_Camera.fDistance, g_Camera.posV.y, (pPlayer->pos.z - cosf((g_Camera.rot.y))* g_Camera.fDistance));

        // キャラ移動時のカメラの位置
        g_Camera.posV += (g_Camera.posVDest - g_Camera.posV) * CAMERA_MOVE;

        // キャラ移動時のカメラの向き
        g_Camera.posR.x += (g_Camera.posRDest.x - g_Camera.posR.x) * CAMERA_MOVE;
        g_Camera.posR.z += (g_Camera.posRDest.z - g_Camera.posR.z) * CAMERA_MOVE;

        //カメラの向きを右スティックで変える
        if (Controller.lRz > 0)//カメラの向き上
        {
            g_fCamera_posR_Height -= 2.5f;
        }
        else if (Controller.lRz < 0)//カメラの向き下
        {
            g_fCamera_posR_Height += 2.5f;
        }

        //カメラの向きの制限
        if (g_fCamera_posR_Height > 80.0f)
        {
            g_fCamera_posR_Height = 80.0f;
        }
        else if (g_fCamera_posR_Height < -30.0f)
        {
            g_fCamera_posR_Height = -30.0f;
        }

        g_Camera.posR.y = CAMERA_ROT_Y + pPlayer->pos.y + g_fCamera_posR_Height;    //カメラの向き

                                                                                    // ジャンプの時の視点移動
        g_Camera.posV.y = CAMERA_POS_Y + pPlayer->pos.y;    //カメラの位置

                                                            //L1/L2でロックオンする
        if (GetJoypadPress(BUTTON_L1) || GetJoypadPress(BUTTON_L2))
        {
            g_Camera.bLockOn = true;
        }
        else
        {
            g_Camera.bLockOn = false;
        }
    }

    //ボス戦にたどり着いたとき
    else
    {
        //ボス注視のカウント
        g_nCntReachBoss++;

        float fBposx = pBoss->pos.x, fBposz = pBoss->pos.z;		//ボスの座標

        g_fAngleCamera = atan2f((CAMERA_BOSS_REACH_X - fBposx), (CAMERA_BOSS_REACH_Z-fBposz));	//角度を決める

        // 注視点をモデルが向いている方向へ移動する
        g_Camera.posRDest = D3DXVECTOR3(CAMERA_BOSS_REACH_X, CAMERA_BOSS_REACH_Y+50
            , BOSS_FIRST_POS_Z); //縦方向の調整
        
        //カメラの位置をキャラに追従させる
        g_Camera.posVDest = D3DXVECTOR3(CAMERA_BOSS_REACH_X, CAMERA_BOSS_REACH_Y, CAMERA_BOSS_REACH_Z);

        // キャラ移動時のカメラの位置
        g_Camera.posV += (g_Camera.posVDest - g_Camera.posV) * CAMERA_MOVE;

        // キャラ移動時のカメラの向き
        g_Camera.posR += (g_Camera.posRDest - g_Camera.posR) * CAMERA_MOVE;

        //注視をやめるとともに、カメラ外で処理を進める
        if (g_nCntReachBoss > CAMERA_BOSS_TIME)
        {
            //ゲーム音止める
            StopSound(SOUND_LABEL_BGM001);

            //カメラを通常状態へ
            g_Camera.state = CAMERASTATE_NORMAL;
            g_nCntReachBoss = 0;
            g_Camera.bBossMode = true;

            //ボス曲へ
            PlaySound(SOUND_LABEL_BGM004);

            //エネミーをすべて未使用に
            pEnemy = GetEnemy();
            for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
            {
                if (pEnemy->bUse == true)
                {
                    pEnemy->bUse = false;
                }
            }
        }
    }
}

//=============================================================================
// カメラの設定
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
//カメラの状況を取得
//=============================================================================
CAMERA * GetCamera(void)
{
    return &g_Camera;
}