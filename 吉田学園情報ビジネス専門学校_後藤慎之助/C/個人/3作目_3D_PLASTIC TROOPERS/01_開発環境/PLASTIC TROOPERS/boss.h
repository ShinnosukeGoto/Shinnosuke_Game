//=============================================================================
//
// 3Dゲーム制作の処理 [boss.h]
// Author:後藤慎之助
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_BOSS_PARTS (14)					// パーツの数

#define BOSS_SIZE (D3DXVECTOR3(150.0f,250.0f,120.0f))     //ワイバーンのサイズ
#define BOSS_LIFE (1000.0f)		                         //ワイバーンの体力
#define BOSS_ANGER_LIFE (BOSS_LIFE / 2)                  //ワイバーンの怒り移行体力
#define BOSS_EXANGER_LIFE (BOSS_LIFE / 4)                //ワイバーン激昂移行体力
#define BOSS_HEIGHT (125.0f)                             //ワイバーンの高さ

#define BOSS_FIRST_POS_Z (3300.0f)  //ボスの初期位置

#define HIT_BOSS_DAMAGE (4.5f)  //ボスに当たった時の、自機へのダメージ

#define BOSS_CONSIDER_SHORT (450.0f)    //近距離とみなす
#define BOSS_CONSIDER_MEDIUM (650.0f)   //中距離とみなす

//=====================
//ボスの各行動の時間
//=====================
//向き取得待機
#define NORMAL_STAY_TIME (120) //向き取得待機時間
#define ANGER_STAY_TIME (95)  //怒り時の待機時間
#define EXANGER_STAY_TIME (70) //激昂時の待機時間
#define ANGER_ROT_SPEED (0.1f)//怒り時の振り向きスピード
#define EXANGER_ROT_SPEED (0.15f)//激昂時の振り向きスピード
//タックル
#define ATTACK_TIME_TACKLE (90) //タックルの攻撃フレーム
#define CHANCE_TIME_TACKLE (55) //タックルの隙フレーム
#define NORMAL_TACKLE_SPEED (10.0f)  //通常時のタックル速度
#define ANGER_TACKLE_SPEED (9.0f)   //怒り時の追従タックル速度
#define EXANGER_TACKLE_SPEED (11.0f)  //激昂時の追従タックル速度
//スピン
#define ATTACK_TIME_SPIN (70)   //スピンの攻撃フレーム
#define CHANCE_TIME_SPIN (85)   //スピンの隙フレーム
#define DAMAGE_SPIN (20.0f)     //スピンのダメージ
#define RANGE_SPIN_XZ (400.0f)  //スピンの横攻撃範囲
#define RANGE_SPIN_Y (120.0f)  //スピンの縦攻撃範囲
#define CHARGE_SPIN (20)//スピンのチャージ時間
#define KNOCKBACK_SPIN (80.0f)  //スピンのノックバック量
#define UPPER_SPIN (15.0f)      //スピンのY軸上昇量
//バレット
#define ATTACK_TIME_BULLET (52) //バレットの攻撃フレーム
#define CHANCE_TIME_BULLET (25) //バレットの隙フレーム
#define BULLET_MOMENT (20)      //射撃する瞬間のフレーム
#define BULLET_SMASH (20.0f)    //バレットの移動のY軸をあえて、XZ軸速度より下げて、たたきつける
//怯み/スタン
#define BOSS_REACH_RIGID (100)   //ボスの怯み/スタン値
#define FLINCH_TIME (90)    //怯み時間
#define STUN_TIME (300) //スタン時間
#define FLINCH_MOVE (4.0f)  //怯み時の移動量

//=============================================================================
//構造体定義
//=============================================================================

// アニメーションキーセット情報
typedef struct
{
    D3DXVECTOR3 pos[MAX_BOSS_PARTS];
    D3DXVECTOR3 rot[MAX_BOSS_PARTS];

    int nFrame;     // 1モーションのキーフレーム数
}KEY_INFO_BOSS;

// アニメーション情報
typedef struct
{
    bool nLoop;		// ループするか
    int nNum_Key;			// モーションのキー数
    KEY_INFO_BOSS KeyInfo[MOTION_KEYSET_MAX];		// アニメーションに含むキーセット情報
}ANIMATION_BOSS;

//モーションの状態
typedef enum
{
    MOTION_IDLE_BOSS = 0,		// 待機モーション
    MOTION_WALK_BOSS,			// 歩きモーション
    MOTION_ATTACK_BULLET_BOSS,	// 銃攻撃モーション
    MOTION_STUN_BOSS,			// スタンモーション
    MOTION_FLINCH_BOSS,			// 怯みモーション
    MOTION_ATTACK_SPIN_BOSS,    // 回転しっぽ攻撃
    MOTION_MAX_BOSS
}MOTIONSTATE_BOSS;

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
}MODEL_BOSS;

// モデルパーツの構造体
typedef struct
{
    char *pFileName;
    int nData;
    int nParents;			// モデルナンバー
    D3DXVECTOR3 pos;			// 位置
    D3DXVECTOR3 posOrigin;		// 始まりの位置
    D3DXVECTOR3 rot;			// 角度

}ModelParts_BOSS;

//ボスの状態
typedef enum
{
    BOSSSTATE_STAY = 0,	    //向き取得待機
    BOSSSTATE_ATTACK_TACKLE,//突進攻撃中
    BOSSSTATE_ATTACK_SPIN,  //回転しっぽ攻撃中
    BOSSSTATE_ATTACK_BULLET,//銃攻撃中
    BOSSSTATE_CHANCE,	    //向き固定待機
    BOSSSTATE_MAX			//状態の最大数
}BOSSSTATE;

//ボスの情報
typedef struct
{
    D3DXVECTOR3 pos;	            //現在の位置
    D3DXVECTOR3 posOld;	            //1F前の位置
    D3DXVECTOR3 size;               //ボスの大きさ
    D3DXVECTOR3 rot;	            //向き
    D3DXVECTOR3 rotDest;	        //向き(目的値)
    D3DXVECTOR3 rotMin;             //向きを変えるときに、最短で振り向くため使用
    D3DXMATRIX mtxWorld;			//行列計算用
    D3DXVECTOR3 move;	            //移動
    BOSSSTATE state;	            //状態
    MOTIONSTATE_BOSS MotionState;	//モーションの状態
    MODEL_BOSS modelParts[MAX_BOSS_PARTS];	//プレイヤーを構成椅子つパーツ群
    float fLife;		            //ライフ
    int nCounterRigid;              //怯みやスタンまでのカウント
    int nStayTime;                  //向き取得待機時間
    int nAttackTime;	            //攻撃時間
    int nChanceTime;                //向き固定待機時間
    int nCntFlinch;                 //怯み回数をカウント
    int nStunTime;                  //スタン時間
    bool bStun;                     //ダウンしているかどうか
    bool bUse;			            //使用しているかどうか
    float fDistance;    //自機との距離
}BOSS;

//=====================================================
// プロトタイプ宣言
//=====================================================
void InitBoss(void);
void UninitBoss(void);
void UpdateBoss(void);
void DrawBoss(void);
BOSS*GetBoss(void);
void HitBoss(int nCntRigid, float fDamage);
bool CollisionBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

void UpdateBossMotion(void);
void LoadBossAnimation(void);
void LoadBoss(void);
#endif