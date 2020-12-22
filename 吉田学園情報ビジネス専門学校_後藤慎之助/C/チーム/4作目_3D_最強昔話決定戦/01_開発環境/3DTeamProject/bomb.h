//=============================================================================
//
//	最強昔話決定戦の処理 [bomb.h](爆弾)
//	Author:後藤慎之助
//
//=============================================================================
#ifndef _BOMB_H_
#define _BOMB_H_

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_BOMB (256)
#define KICK_MOVE 12.5f //蹴られたボムの速さ
#define BOMB_TRIGGER_TIME 8 //誘発されたボムの、爆発までの時間

//=============================================================================
//構造体定義
//=============================================================================

typedef struct
{
    DWORD MatBomb = 0;              //マテリアル情報の数
    D3DXVECTOR3 pos;                //位置
    D3DXVECTOR3 posGrid;            //グリッド計算用
    D3DXVECTOR3 posNew;             //爆風がブロックに当たるかどうか
    D3DXVECTOR3 posOld;             //1F前の位置を記憶
    D3DXVECTOR3 move;	            //移動
    D3DXVECTOR3 rot;                //向き
    D3DXVECTOR3 size;               //大きさ
    D3DXVECTOR3 sizeVisual;         //見かけの大きさ
    SELECT type;                    //タイプ
    SELECT KickType;                //蹴った人を記録する
    D3DXMATRIX mtxWorld;            //行列計算用
    LPD3DXMESH pMeshBomb;           //マテリアル情報のポインタ
    DWORD nNumMatBomb;	            //マテリアル情報の数
    bool bUse;                      //使用しているかどうか
    int nCntTime;                   //時間を数える
    int nFire;                      //火力
    bool bFire;                     //燃えているかどうか
    int nShadow;                    //影の番号
    bool bIncreaseSize;             //見かけの大きさが増えているかどうか
    bool bCheckGridUp;              //上のグリッドにモノがあるかどうか
    bool bCheckGridDown;            //下のグリッドにモノがあるかどうか
    bool bCheckGridLeft;            //左のグリッドにモノがあるかどうか
    bool bCheckGridRight;           //右のグリッドにモノがあるかどうか
}BOMB;

//=============================================================================
//プロトタイプ宣言
//=============================================================================
void InitBomb(void);
void UninitBomb(void);
void UpdateBomb(void);
void DrawBomb(void);
BOMB* GetBomb(void);
bool CollisionBombs(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size ,int nNumberBomb);    //爆弾同士の当たり判定
bool CollisionBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool HitBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBomb);
void KickBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size ,SELECT type);
void SetBomb(D3DXVECTOR3 pos,int nFire, int nCntTime , SELECT type);

//キック用の、周囲のグリッドをチェックする関数
bool CheckGridUp(int nCntBomb);
bool CheckGridDown(int nCntBomb);
bool CheckGridLeft(int nCntBomb);
bool CheckGridRight(int nCntBomb);

void BreakBomb(int nCntBomb);   //ボムを破壊する
#endif