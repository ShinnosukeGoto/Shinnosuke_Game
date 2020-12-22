//=============================================================================
//
// 最強昔話決定戦の処理 [camera.h] (カメラ)
// Author:後藤慎之助
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
//マクロ定義
//=============================================================================

//カメラの位置
#define CAMERA_POS_X (700.0f)
#define CAMERA_POS_Y (2000.0f)
#define CAMERA_POS_Z (400.0f)

//カメラの注視点
#define CAMERA_ROT_X (700.0f)
#define CAMERA_ROT_Y (-400.0f)
#define CAMERA_ROT_Z (700.0f)

#define CAMERA_MOVE (0.15f) //カメラの移動量

//ボスにたどり着いたときのカメラの位置
#define CAMERA_BOSS_REACH_X (4900.0f)
#define CAMERA_BOSS_REACH_Y (100.0f)
#define CAMERA_BOSS_REACH_Z (600.0f)

//ボスにたどり着いた時の、ボスを眺める時間
#define CAMERA_BOSS_TIME (180)

//=============================================================================
//構造体定義
//=============================================================================

//カメラの状態
typedef enum
{
    CAMERASTATE_NORMAL = 0,	    //通常
    CAMERASTATE_STORY,		    //ストーリー
    CAMERASTATE_REACH_BOSS,	    //ボス戦に入るとき
    CAMERASTATE_MAX			    //状態の最大数
}CAMERASTATE;

typedef struct
{
    D3DXVECTOR3 posV;           //カメラの座標
    D3DXVECTOR3 posR;           //注視点
    D3DXVECTOR3 vecU;           //上方向のベクトル
    D3DXVECTOR3 posVDest;       //カメラの座標（目的値）
    D3DXVECTOR3 posRDest;       //カメラの注視点（目的値）
    D3DXMATRIX mtxProjection;   //プロジェクションマトリクス
    D3DXMATRIX mtxView;         //ビューマトリクス
    D3DXVECTOR3 rot;            //向き取得用(ローテーション,回転)
    CAMERASTATE state;          //カメラの状態
    float fDistance;            //視点-注視点の距離
    int nCntReachBoss;          //ボス戦にたどり着いた時のカウンター
    bool bBossBattle;           //ボス戦かどうか
}CAMERA;//カメラの情報

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);	//Drawと同じ
CAMERA*GetCamera(void);
#endif