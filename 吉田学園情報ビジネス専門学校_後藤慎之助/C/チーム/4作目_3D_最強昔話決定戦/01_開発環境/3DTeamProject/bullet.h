//=============================================================================
//
// �ŋ��̘b�����̏��� [bullet.h] (�G�̒e)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include"main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_BULLET (64)	//�o���b�g�̍ő吔

//�e�̑傫��
#define BULLET_SIZE_ENEMY (D3DXVECTOR3 (30.0f,30.0f,30.0f))
#define BULLET_SIZE_BOSS (D3DXVECTOR3 (80.0f,80.0f,80.0f))  //���Ƃ�49

//�e�̑���
#define BULLET_SPEED_ENEMY (10.0f)
#define BULLET_SPEED_BOSS (8.0f)

//�_���[�W��
#define BULLET_DAMAGE_ENEMY (25.0f)
#define BULLET_DAMAGE_BOSS (50.0f)

//���C�t
#define BULLET_LIFE_ENEMY 60
#define BULLET_LIFE_BOSS 300

//=============================================================================
//�\���̒�`
//=============================================================================
typedef enum
{
    BULLETTYPE_ENEMY,		//�G�l�~�[�̒e
    BULLETTYPE_BOSS,		//�{�X�̒e
    BULLETTYPE_MAX
}BULLETTYPE;//�A�C�e���̎��

typedef struct
{
    D3DXVECTOR3 pos;	//���݂̈ʒu
    D3DXVECTOR3 posOld; //1F�O�̈ʒu
    D3DXVECTOR3 size;   //�傫��
    D3DXVECTOR3 move;	//�ړ���
    D3DXMATRIX mtxWorld;//�s��v�Z�p
    BULLETTYPE type;	//���
    int nLife;			//����
    float fDamage;      //�_���[�W
    bool bUse;			//�g�p���Ă��邩�ǂ���
    int nShadow;                    //�e�̔ԍ�
}BULLET;

//=============================================================================
// �v���g�^�C�v�錾
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