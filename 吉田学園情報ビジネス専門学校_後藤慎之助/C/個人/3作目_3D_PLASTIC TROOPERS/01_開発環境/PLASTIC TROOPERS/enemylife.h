////=============================================================================
////
//// 体力の処理 [enemylife.h]
//// Author : 青野　慎平
////
////=============================================================================
//#ifndef _ENEMYLIFE_H_
//#define _ENEMYLIFE_H_
//
//#define MAX_ENEMYLIFE (128)
//#define ENEMYLIFE_SIZE_X (2.0f)
//#define ENEMYLIFE_SIZE_Y (5.0f)
//#define BOSSLIFE_SIZE_X (25.0f)
//#define BOSSLIFE_SIZE_Y (10.0f)
//#define ENEMYLIFE_POS_Y_000 (80.0f)//位置の微調整
//#define ENEMYLIFE_POS_Y_001 (50.0f)
//#define BOSSLIFE_POS_Y_001 (10.0f)
//
//typedef enum
//{
//    ENEMYLIFETYPE_LIFE = 0,
//    ENEMYLIFETYPE_FLAME,
//    MAX_ENEMYLIFETYPE
//}ENEMYLIFETYPE;
//
//typedef struct
//{
//    D3DXVECTOR3 pos;	//位置
//    D3DXCOLOR col;
//    D3DXMATRIX mtxWorld;//行列計算用
//    ENEMYLIFETYPE type;
//    int nEnemyIndex;
//    float fMaxLife;
//    bool bUse;	//使用しているかどうか
//    bool bFlame;//フレームがついているか
//}ENEMYLIFE;
//
////=============================================================================
////プロトタイプ宣言
////=============================================================================
//void InitEnemyLife(void);
//void UninitEnemyLife(void);
//void UpdateEnemyLife(void);
//void DrawEnemyLife(void);
//void SetEnemyLife(float fMaxLife, int nCntEnemy);
//void UnsetEnemyLife(int nCntEnemy);
//
//
//#endif