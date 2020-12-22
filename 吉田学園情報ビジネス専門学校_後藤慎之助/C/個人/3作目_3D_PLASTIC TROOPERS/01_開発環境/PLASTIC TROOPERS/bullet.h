//=============================================================================
//
// PLASTIC TROOPERS�̏��� [bullet.h] (�G�Ǝ��@�̒e)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_BULLET (128)	//�o���b�g�̍ő吔

//�e�̑傫��
#define BULLET_SIZE_PLAYER (D3DXVECTOR3 (10.0f,10.0f,10.0f))
#define BULLET_SIZE_ENEMY (D3DXVECTOR3 (15.0f,15.0f,15.0f))
#define BULLET_SIZE_BOSS (D3DXVECTOR3 (20.0f,20.0f,20.0f))

//�e�̑���
#define BULLET_SPEED_PLAYER (30.0f)
#define BULLET_SPEED_ENEMY (20.0f)
#define BULLET_SPEED_BOSS (35.0f)

//�e�̐F
#define BULLET_COLOR_PLAYER (D3DXCOLOR (0.9f,0.85f,0.38f,1.0f))
#define BULLET_COLOR_ENEMY (D3DXCOLOR (1.0f,0.0f,0.0f,1.0f))

//�e�̋��ݒl
#define BULLET_RIGID (5)

//�e�ʏ�U��
#define BULLET_DAMAGE_PLAYER (2.8f)
#define BULLET_DAMAGE_ENEMY (10.0f)
#define BULLET_DAMAGE_BOSS (15.0f)

//���ʏ�U��
#define SWORD_SPEED (30.0f)
#define SWORD_SIZE (D3DXVECTOR3(1.0f,1.0f,1.0f))
#define SWORD_RIGID (20)
#define SWORD_DAMAGE (30.0f)

//���K�E�U��
#define SWORD_EXDAMAGE (15.0f)
#define SWORD_EXRIGID (30)

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
    BULLETTYPE_PLAYER = 0,	//�v���C���[�̒e
    BULLETTYPE_ENEMY,		//
    BULLETTYPE_BOSS,		//
    BULLETTYPE_003,		//
    BULLETTYPE_004,		//
    BULLETTYPE_005,		//
    BULLETTYPE_MAX
}BULLETTYPE;//�A�C�e���̎��

typedef struct
{
    D3DXVECTOR3 pos;	//���݂̈ʒu
    D3DXVECTOR3 posOld; //1F�O�̈ʒu
    D3DXVECTOR3 size;   //�傫��
    D3DXVECTOR3 move;	//�ړ���
    D3DXCOLOR col;      //�F
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    BULLETTYPE type;	//���
    int nLife;			//����
    int nCntRigid;      //�q�b�g�d��
    float fDamage;      //�_���[�W
    bool bUse;			//�g�p���Ă��邩�ǂ���
}BULLET;

//=============================================================================
// �v���g�^�C�v�錾
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