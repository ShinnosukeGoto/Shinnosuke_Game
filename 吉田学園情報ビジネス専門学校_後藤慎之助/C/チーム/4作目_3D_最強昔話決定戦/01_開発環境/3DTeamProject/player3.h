//=============================================================================
//
// 最強昔話決定戦の処理[player3.h]（自機3）
// Author:後藤慎之助
//
//=============================================================================
#ifndef _PLAYER3_H_
#define _PLAYER3_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
//プレイヤーの初期位置
#define PLAYER3_FIRST_POS_X (100.0f)
#define PLAYER3_FIRST_POS_Z (100.0f)

//=============================================================================
//構造体定義
//=============================================================================

// アニメーションキーセット情報
typedef struct
{
    D3DXVECTOR3 pos[MAX_PARTS];
    D3DXVECTOR3 rot[MAX_PARTS];

    int nFrame;     // 1モーションのキーフレーム数
}KEY_INFO_PLAYER3;

// アニメーション情報
typedef struct
{
    bool nLoop;		// ループするか
    int nNum_Key;			// モーションのキー数
    KEY_INFO_PLAYER3 KeyInfo[MOTION_KEYSET_MAX];		// アニメーションに含むキーセット情報
}ANIMATION_PLAYER3;

//モーションの状態
typedef enum
{
    MOTIONSTATE_PLAYER3_IDLE = 0,		// 待機モーション
    MOTIONSTATE_PLAYER3_WALK,			// 歩きモーション
    MOTIONSTATE_PLAYER3_REGRET,		    // 悔しがりモーション
    MOTIONSTATE_PLAYER3_PLEASURE,		// 喜びモーション
    MOTIONSTATE_PLAYER3_,			    // 
    MOTIONSTATE_PLAYER3_MAX
}MOTIONSTATE_PLAYER3;

// モデルの構造体
typedef struct
{
    LPD3DXMESH pMesh;				// メッシュ情報へのポインタ
    LPD3DXBUFFER pBuffMat;			// マテリアル情報へのポインタ
    DWORD nNumMat;					// マテリアル情報の数
    D3DXVECTOR3 pos;				// 位置
    D3DXVECTOR3 rot;				// 回転
    D3DXVECTOR3 size;
    D3DXVECTOR3 AngleAddition;		// 角度の加算
    D3DXMATRIX mtxWorld;			// 行列計算用
    int nldxModelParent;			// 親モデルのインデックス
    bool bUse;
}MODEL_PLAYER3;

// モデルパーツの構造体
typedef struct
{
    char *pFileName;
    int nData;
    int nParents;			// モデルナンバー
    D3DXVECTOR3 pos;			// 位置
    D3DXVECTOR3 posOrigin;		// 始まりの位置
    D3DXVECTOR3 rot;			// 角度
}ModelParts_Player3;

//プレイヤーの情報
typedef struct
{
    D3DXVECTOR3 pos;	            //現在の位置
    D3DXVECTOR3 posOld;	            //1F前の位置
    D3DXVECTOR3 rot;	            //向き
    D3DXVECTOR3 posGrid;            //グリッドの中心の値を得る
    D3DXMATRIX mtxWorld;			// 行列計算用
    D3DXVECTOR3 move;	            //移動
    MOTIONSTATE_PLAYER3 MotionState;		//モーションの状態
    MODEL_PLAYER3 modelParts[MAX_PARTS];	//プレイヤーを構成椅子つパーツ群
    float fLife;		            //ライフ
    float fSpeed;                   //速さ
    int nSpeedRank;                 //速さランク
    int nFire;                      //火力
    int nBomb;                      //ボムの設置可能個数
    int nMaxBomb;                   //ボム設置の最大数
    int nCanKick;                   //1以上でキック可能
    bool bDisp;			            //表示切替
    int nShadow;                    //プレイヤーの影の番号
    int nCntInvincibleFrame;        //無敵時間
    bool bTransparence;             //透明かどうか
}PLAYER3;

//=====================================================
// プロトタイプ宣言
//=====================================================
void InitPlayer3(void);
void UninitPlayer3(void);
void UpdatePlayer3(void);
void DrawPlayer3(void);
PLAYER3*GetPlayer3(void);
void HitPlayer3(float fDamage);
bool CollisionPlayer3(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

void UpdatePlayer3Motion(void);
void LoadPlayer3Animation(void);
void LoadPlayer3(void);
void InitDispPlayer3(void);
#endif