//=============================================================================
//
// 最強昔話決定戦の処理 [bullet.h] (敵の弾)
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include"main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	MAX_BULLET (64)	//バレットの最大数

//弾の大きさ
#define BULLET_SIZE_ENEMY (D3DXVECTOR3 (30.0f,30.0f,30.0f))
#define BULLET_SIZE_BOSS (D3DXVECTOR3 (80.0f,80.0f,80.0f))  //もとは49

//弾の速さ
#define BULLET_SPEED_ENEMY (10.0f)
#define BULLET_SPEED_BOSS (8.0f)

//ダメージ量
#define BULLET_DAMAGE_ENEMY (25.0f)
#define BULLET_DAMAGE_BOSS (50.0f)

//ライフ
#define BULLET_LIFE_ENEMY 60
#define BULLET_LIFE_BOSS 300

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
    BULLETTYPE_ENEMY,		//エネミーの弾
    BULLETTYPE_BOSS,		//ボスの弾
    BULLETTYPE_MAX
}BULLETTYPE;//アイテムの種類

typedef struct
{
    D3DXVECTOR3 pos;	//現在の位置
    D3DXVECTOR3 posOld; //1F前の位置
    D3DXVECTOR3 size;   //大きさ
    D3DXVECTOR3 move;	//移動量
    D3DXMATRIX mtxWorld;//行列計算用
    BULLETTYPE type;	//種類
    int nLife;			//寿命
    float fDamage;      //ダメージ
    bool bUse;			//使用しているかどうか
    int nShadow;                    //影の番号
}BULLET;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXVECTOR3 move, int nLife, float fDamage, BULLETTYPE type);
BULLET*GetBullet(void);
bool CollisionBullet(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool HitBullet(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);

#endif