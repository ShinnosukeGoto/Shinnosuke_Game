//=============================================================================
//
// �ŋ��̘b����� [enemy.h]�̏��� (�Q�[���ɏo�Ă���G)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_ENEMY (16)			//�G�̍ő吔

#define ENEMY_SIZE_CANNON (D3DXVECTOR3(50.0f,50.0f,50.0f))  //�L���m���̃T�C�Y
#define ENEMY_HEIGHT_CANNON (50.0f)                         //�L���m���̍���
#define ENEMY_LIFE_CANNON (20.0f)		                    //�L���m���̗̑�
#define ENEMY_CANNON_SCORE 3000                             //�L���m���̃X�R�A

#define ENEMY_SIZE_UMBRELLA (D3DXVECTOR3(65.0f,50.0f,65.0f))//�A���u�����̃T�C�Y
#define ENEMY_LIFE_UMBRELLA (20.0f)		                    //�A���u�����̗̑�
#define ENEMY_UMBRELLA_SCORE 4000                           //�A���u�����̃X�R�A
#define ENEMY_UMBRELLA_SPEED 2.0f                           //�A���u�����̃X�s�[�h

#define ENEMY_SIZE_GHOST (D3DXVECTOR3(50.0f,50.0f,50.0f))   //�S�[�X�g�̃T�C�Y
#define ENEMY_LIFE_GHOST (24.0f)		                    //�S�[�X�g�̗̑�
#define ENEMY_LIFE_RED_GHOST (40.0f)		                //���b�h�S�[�X�g�̗̑�
#define ENEMY_GHOST_SCORE 2000                              //�S�[�X�g�̃X�R�A
#define ENEMY_RED_GHOST_SCORE 5000                          //���b�h�S�[�X�g�̃X�R�A
#define ENEMY_GHOST_SPEED 1.5f                              //�S�[�X�g�̃X�s�[�h
#define ENEMY_RED_GHOST_SPEED 2.5f                          //���b�h�S�[�X�g�̃X�s�[�h

#define ENEMY_SIZE_SNAKE (D3DXVECTOR3(50.0f,50.0f,50.0f))   //�c�`�m�R�̃T�C�Y
#define ENEMY_LIFE_SNAKE (20.0f)		                    //�c�`�m�R�̗̑�
#define ENEMY_SNAKE_SCORE 10000                             //�c�`�m�R�̃X�R�A

#define HIT_ENEMY_DAMAGE (50.0f)                            //�G�ɓ����������́A���@�ւ̃_���[�W

#define ENEMY_FIELD_OF_VISION 800.0f                        //�G�̎���

#define ENEMY_TURN_SPEED 0.025f                             //�G�̐U��������x

#define ENEMY_ATTACK_COOL_DOWN 90                           //�U���̃N�[���_�E��

#define ENEMY_JUMP 15.0f                                    //�G�̃W�����v��

#define ENEMY_DISABLED_BOMB_FRAME 30                        //���e��ʂ蔲���ł���t���[����

//=============================================================================
//�\���̒�`
//=============================================================================

typedef enum
{
    ENEMYSTATE_NORMAL = 0,	//�ʏ�
    ENEMYSTATE_DAMAGE,		//�_���[�W
    ENEMYSTATE__MAX			//��Ԃ̍ő吔
}ENEMYSTATE;//�G�̏��

typedef enum
{
    ENEMYTYPE_CANNON = 0,		//�L���m��
    ENEMYTYPE_UMBRELLA,			//�A���u����
    ENEMYTYPE_GHOST,			//�S�[�X�g
    ENEMYTYPE_SNAKE,			//�c�`�m�R
    ENEMYTYPE_RED_GHOST,		//���b�h�S�[�X�g
    ENEMYTYPE_MAX			    //��ނ̍ő吔
}ENEMYTYPE;//�G�̎��

typedef struct
{
    D3DXVECTOR3 pos;	            //���݂̈ʒu
    D3DXVECTOR3 posOld;	            //1F�O�̈ʒu
    D3DXVECTOR3 move;	            //�ړ�
    D3DXVECTOR3 rot;	            //����
    D3DXVECTOR3 rotDest;            //����(�ړI�l)
    D3DXVECTOR3 rotMin;             //������ς���Ƃ��ɁA�ŒZ�ŐU��������ߎg�p
    D3DXVECTOR3 size;	            //�傫��
    D3DXMATRIX mtxWorld;            //���[���h�}�g���N�X
    ENEMYSTATE state;               //���
    ENEMYTYPE type;		            //���
    float fLife;		            //���C�t
    bool bUse;			            //�g�p���Ă��邩�ǂ���
    int nAttackTime;	            //�U������
    int nShadow;                    //�e�̔ԍ�
    int nCntInvincibleFrame;        //���G����
    bool bTransparence;             //�������ǂ���
    float fDistance;                //���@�Ƃ̋���
    bool bFindPlayer;               //���@�𔭌��������ǂ���
    int nCntDisabledBomb;           //���e�𖳌�������t���[��
}ENEMY;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, ENEMYTYPE type);
ENEMY*GetEnemy(void);
void HitEnemy(int nCntEnemy, float nDamage);
bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, bool bPlayer);

bool StopFireEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);          //�������~�߂邱�Ƃ��ł���G
#endif