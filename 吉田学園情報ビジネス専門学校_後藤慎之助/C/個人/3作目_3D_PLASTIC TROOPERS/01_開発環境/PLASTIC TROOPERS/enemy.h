//=============================================================================
//
// PLASTIC TROOPERS [enemy.h]�̏��� (�Q�[���ɏo�Ă���G)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_
#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	MAX_ENEMY (32)			//�G�̍ő吔

#define ENEMY_SIZE_CANNON (D3DXVECTOR3(50.0f,50.0f,50.0f))  //�L���m���̃T�C�Y
#define ENEMY_HEIGHT_CANNON (50.0f)                         //�L���m���̍���
#define ENEMY_LIFE_CANNON (40.0f)		                    //�L���m���̗̑�

#define ENEMY_SIZE_SPIDER (D3DXVECTOR3(80.0f,50.0f,80.0f))  //�X�p�C�_�[�̃T�C�Y
#define ENEMY_LIFE_SPIDER (30.0f)		                    //�X�p�C�_�[�̗̑�
#define ENEMY_LIFE_GOLDENSPIDER (30.0f)		                //�S�[���f���X�p�C�_�[�̗̑�

#define ENEMY_SIZE_BUTTERFLY (D3DXVECTOR3(30.0f,40.0f,30.0f))//�o�^�t���C�̃T�C�Y
#define ENEMY_HEIGHT_BUTTERFLY (100.0f)                         //�o�^�t���C�̍���
#define ENEMY_LIFE_BUTTERFLY (50.0f)		                    //�o�^�t���C�̗̑�

#define ENEMY_SIZE_JUMPBOX (D3DXVECTOR3(50.0f,50.0f,50.0f))  //�W�����v�{�b�N�X�̃T�C�Y
#define ENEMY_LIFE_JUMPBOX (20.0f)		                     //�W�����v�{�b�N�X�̗̑�

#define ENEMY_SIZE_FISH (D3DXVECTOR3(50.0f,50.0f,80.0f))     //�t�B�b�V���̃T�C�Y
#define ENEMY_LIFE_FISH (60.0f)		                     //�t�B�b�V���̗̑�

#define HIT_ENEMY_DAMAGE (3.5f)     //�G�ɓ����������́A���@�ւ̃_���[�W

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
    ENEMYTYPE_000 = 0,		//�L���m��
    ENEMYTYPE_001,			//�X�p�C�_�[
    ENEMYTYPE_002,			//�o�^�t���C
    ENEMYTYPE_003,			//�S�[���f���X�p�C�_�[
    ENEMYTYPE_004,			//�W�����v�{�b�N�X
    ENEMYTYPE_005,			//�t�B�b�V��
    ENEMYTYPE_MAX			//��ނ̍ő吔
}ENEMYTYPE;//�G�̎��

typedef struct
{
    D3DXVECTOR3 pos;	//���݂̈ʒu
    D3DXVECTOR3 posOld;	//1F�O�̈ʒu
    D3DXVECTOR3 posMemory;	//�z�u���ꂽ���̈ʒu
    D3DXVECTOR3 move;	//�ړ�
    D3DXVECTOR3 rot;	//����
    D3DXVECTOR3 rotDest;//����(�ړI�l)
    D3DXVECTOR3 rotMin; //������ς���Ƃ��ɁA�ŒZ�ŐU��������ߎg�p
    D3DXVECTOR3 size;	// �傫��
    D3DXMATRIX mtxWorld;//���[���h�}�g���N�X
    ENEMYSTATE state;	//���
    float fLife;		//���C�t
    int nCounterState;	//��ԃJ�E���^
    ENEMYTYPE type;		//���
    int nPosX;			//�ړ��ł��鉡�͈�
    int nPosZ;			//�ړ��ł��鉜�͈�
    int nCounterAnim;	//�A�j���[�V�����J�E���^�[
    int nPatternAnim;	//�A�j���[�V�����p�^�[��
    bool bUse;			//�g�p���Ă��邩�ǂ���
    int nAttackTime;	//�U������
    float fDistance;    //���@�Ƃ̋���
    int nRank;          //�����̏���
    bool bFollow;       //�Ǐ]�����邩�ǂ���
}ENEMY;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(D3DXVECTOR3 pos,D3DXVECTOR3 move, ENEMYTYPE type);
ENEMY*GetEnemy(void);
void HitEnemy(int nCntEnemy,int nCntRigid, float nDamage);
bool CollisionEnemy(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
void UpperEnemy(float fUp, int nCnt);
#endif