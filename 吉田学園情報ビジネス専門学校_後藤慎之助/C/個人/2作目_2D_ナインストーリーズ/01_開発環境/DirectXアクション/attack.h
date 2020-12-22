//=============================================================================
//
// ナインストーリーズ [attack.h]の処理（プレイヤーの武器攻撃）
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _ATTACK_H_
#define _ATTACK_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
//マクロ定義
//=============================================================================
#define MAX_ATTACK (32)		//攻撃の最大数

//=============================================================================
//構造体定義
//=============================================================================
typedef enum
{
	ATTACKTYPE_SWORD = 0,		//剣
	ATTACKTYPE_LANCE,			//槍
	ATTACKTYPE_LANCE2,			//槍
	ATTACKTYPE_BOW,				//弓
	ATTACKTYPE_BOW2,			//弓の下攻撃用
	ATTACKTYPE_MAX				//種類の最大数
}ATTACKTYPE;	//武器選択

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	float fLife;		//攻撃全体のフレーム数
	D3DXCOLOR col;		//色
	ATTACKTYPE type;	//種類
	int nSize;			//大きさ
	DIRECT nDirect;		//方向
	bool bUse;			//使用しているかどうか
}ATTACK;	//武器攻撃の情報

//=============================================================================
// プロトタイプ宣言
//=============================================================================
void InitAttack(void);
void UninitAttack(void);
void UpdateAttack(void);
void DrawAttack(void);
void SetAttack(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fLife, D3DXCOLOR col, ATTACKTYPE type, int nSize, DIRECT nDirect);
ATTACK*GetAttack(void);
int GetUseAttack(void);	//攻撃を使った回数
#endif