//=============================================================================
//
// 3Dゲーム制作の処理 [player.h]
// Author:後藤慎之助
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_PARTS (12)					// パーツの数
#define REACH_BOSS_Z (2020.0f)          //ボスの元にたどり着いた判定をするZ座標（2020.0f)

//=============================================================================
//構造体定義
//=============================================================================

// アニメーションキーセット情報
typedef struct
{
    D3DXVECTOR3 pos[MAX_PARTS];
    D3DXVECTOR3 rot[MAX_PARTS];

    int nFrame;     // 1モーションのキーフレーム数
}KEY_INFO;

// アニメーション情報
typedef struct
{
    bool nLoop;		// ループするか
    int nNum_Key;			// モーションのキー数
    KEY_INFO KeyInfo[MOTION_KEYSET_MAX];		// アニメーションに含むキーセット情報
}ANIMATION;

//モーションの状態
typedef enum
{
    MOTION_IDLE = 0,		// 待機モーション
    MOTION_WALK,			// 歩きモーション
    MOTION_BULLET,			// 銃攻撃モーション
    MOTION_JUMP,			// ジャンプモーション
    MOTION_GUARD,			// ガードモーション
    MOTION_SWORD1,			// 剣攻撃モーション1
    MOTION_SWORD2,			// 剣攻撃モーション1
    MOTION_SKYDASH,         //空中ダッシュモーション
    MOTION_MAX
}MOTIONSTATE;

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
}MODEL;

// モデルパーツの構造体
typedef struct
{
    char *pFileName;
    int nData;
    int nParents;			// モデルナンバー
    D3DXVECTOR3 pos;			// 位置
    D3DXVECTOR3 posOrigin;		// 始まりの位置
    D3DXVECTOR3 rot;			// 角度

}ModelParts;

//自機の状態
typedef enum
{
    PLAYERSTATE_NORMAL = 0,	//通常
    PLAYERSTATE_DAMAGE,		//ダメージ
    PLAYERSTATE_MAX			//状態の最大数
}PLAYERSTATE;

//プレイヤーの情報
typedef struct
{
    D3DXVECTOR3 pos;	            //現在の位置
    D3DXVECTOR3 posOld;	            //1F前の位置
    D3DXVECTOR3 rot;	            //向き
    D3DXMATRIX mtxWorld;			// 行列計算用
    D3DXVECTOR3 move;	            //移動
    PLAYERSTATE state;	            //状態
    MOTIONSTATE MotionState;		//モーションの状態
    MODEL modelParts[MAX_PARTS];	//プレイヤーを構成椅子つパーツ群
    float fLife;		            //ライフ
    float fEnergy;		            //エナジー
    float fOldEnergy;               //1F前のエナジー
    float fChanceMeter;             //チャンスゲージ
    int nChance;                    //チャンス値
    int nCntSkyDash;                //空中ダッシュの時間
    int nCounterState;	            //状態カウンタ
    int nAttackTime;	            //攻撃時間
    int nCntEXAttackCD;             //必殺技のクールタイム
    bool bJump;			            //ジャンプ
    bool bSecondJump;	            //二段目のジャンプ
    bool bSkyDash;                  //空中ダッシュ
    bool bDisp;			            //表示切替
    bool bLand;                     //ブロックに着地している
    bool bGuard;                    //ガードしている
    bool bChanceAttack;             //必殺攻撃中
    bool bBullet;                   //銃か剣か
                        //WSELECT Wtype;		//武器選択
                        //MSELECT Mtype;		//魔法選択
}PLAYER;

//=====================================================
// プロトタイプ宣言
//=====================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER*GetPlayer(void);
void HitPlayer(int nCntRigid, float fDamage);

void UpdatePlayerMotion(void);
void LoadAnimation(void);
void LoadPlayer(void);
#endif