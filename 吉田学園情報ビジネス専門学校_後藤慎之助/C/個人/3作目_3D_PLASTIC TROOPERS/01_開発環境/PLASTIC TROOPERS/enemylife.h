////=============================================================================
////
//// �̗͂̏��� [enemylife.h]
//// Author : ��@�T��
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
//#define ENEMYLIFE_POS_Y_000 (80.0f)//�ʒu�̔�����
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
//    D3DXVECTOR3 pos;	//�ʒu
//    D3DXCOLOR col;
//    D3DXMATRIX mtxWorld;//�s��v�Z�p
//    ENEMYLIFETYPE type;
//    int nEnemyIndex;
//    float fMaxLife;
//    bool bUse;	//�g�p���Ă��邩�ǂ���
//    bool bFlame;//�t���[�������Ă��邩
//}ENEMYLIFE;
//
////=============================================================================
////�v���g�^�C�v�錾
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