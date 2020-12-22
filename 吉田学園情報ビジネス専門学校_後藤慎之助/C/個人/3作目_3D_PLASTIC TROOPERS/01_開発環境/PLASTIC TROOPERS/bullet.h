//=============================================================================
//
// PLASTIC TROOPERSの処理 [bullet.h] (敵と自機の弾)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_BULLET (128)	//バレットの最大数

//弾の大きさ
#define BULLET_SIZE_PLAYER (D3DXVECTOR3 (10.0f,10.0f,10.0f))
#define BULLET_SIZE_ENEMY (D3DXVECTOR3 (15.0f,15.0f,15.0f))
#define BULLET_SIZE_BOSS (D3DXVECTOR3 (20.0f,20.0f,20.0f))

//弾の速さ
#define BULLET_SPEED_PLAYER (30.0f)
#define BULLET_SPEED_ENEMY (20.0f)
#define BULLET_SPEED_BOSS (35.0f)

//弾の色
#define BULLET_COLOR_PLAYER (D3DXCOLOR (0.9f,0.85f,0.38f,1.0f))
#define BULLET_COLOR_ENEMY (D3DXCOLOR (1.0f,0.0f,0.0f,1.0f))

//銃の怯み値
#define BULLET_RIGID (5)

//銃通常攻撃
#define BULLET_DAMAGE_PLAYER (2.8f)
#define BULLET_DAMAGE_ENEMY (10.0f)
#define BULLET_DAMAGE_BOSS (15.0f)

//剣通常攻撃
#define SWORD_SPEED (30.0f)
#define SWORD_SIZE (D3DXVECTOR3(1.0f,1.0f,1.0f))
#define SWORD_RIGID (20)
#define SWORD_DAMAGE (30.0f)

//剣必殺攻撃
#define SWORD_EXDAMAGE (15.0f)
#define SWORD_EXRIGID (30)

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
    BULLETTYPE_PLAYER = 0,	//プレイヤーの弾
    BULLETTYPE_ENEMY,		//
    BULLETTYPE_BOSS,		//
    BULLETTYPE_003,		//
    BULLETTYPE_004,		//
    BULLETTYPE_005,		//
    BULLETTYPE_MAX
}BULLETTYPE;//アイテムの種類

typedef struct
{
    D3DXVECTOR3 pos;	//現在の位置
    D3DXVECTOR3 posOld; //1F前の位置
    D3DXVECTOR3 size;   //大きさ
    D3DXVECTOR3 move;	//移動量
    D3DXCOLOR col;      //色
    D3DXMATRIX mtxWorld;//行列計算用
    BULLETTYPE type;	//種類
    int nLife;			//寿命
    int nCntRigid;      //ヒット硬直
    float fDamage;      //ダメージ
    bool bUse;			//使用しているかどうか
}BULLET;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move,int nLife, int nCntRigid, float fDamage, BULLETTYPE type);
BULLET*GetBullet(void);
bool CollisionBulletEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size,int nCnt);
bool CollisionBulletPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool CollisionBulletBoss(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
#endif