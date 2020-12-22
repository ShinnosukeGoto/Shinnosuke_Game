//=============================================================================
//
//	�ŋ��̘b�����̏��� [bomb.h](���e)
//	Author:�㓡�T�V��
//
//=============================================================================
#ifndef _BOMB_H_
#define _BOMB_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_BOMB (256)
#define KICK_MOVE 12.5f //�R��ꂽ�{���̑���
#define BOMB_TRIGGER_TIME 8 //�U�����ꂽ�{���́A�����܂ł̎���

//=============================================================================
//�\���̒�`
//=============================================================================

typedef struct
{
    DWORD MatBomb = 0;              //�}�e���A�����̐�
    D3DXVECTOR3 pos;                //�ʒu
    D3DXVECTOR3 posGrid;            //�O���b�h�v�Z�p
    D3DXVECTOR3 posNew;             //�������u���b�N�ɓ����邩�ǂ���
    D3DXVECTOR3 posOld;             //1F�O�̈ʒu���L��
    D3DXVECTOR3 move;	            //�ړ�
    D3DXVECTOR3 rot;                //����
    D3DXVECTOR3 size;               //�傫��
    D3DXVECTOR3 sizeVisual;         //�������̑傫��
    SELECT type;                    //�^�C�v
    SELECT KickType;                //�R�����l���L�^����
    D3DXMATRIX mtxWorld;            //�s��v�Z�p
    LPD3DXMESH pMeshBomb;           //�}�e���A�����̃|�C���^
    DWORD nNumMatBomb;	            //�}�e���A�����̐�
    bool bUse;                      //�g�p���Ă��邩�ǂ���
    int nCntTime;                   //���Ԃ𐔂���
    int nFire;                      //�Η�
    bool bFire;                     //�R���Ă��邩�ǂ���
    int nShadow;                    //�e�̔ԍ�
    bool bIncreaseSize;             //�������̑傫���������Ă��邩�ǂ���
    bool bCheckGridUp;              //��̃O���b�h�Ƀ��m�����邩�ǂ���
    bool bCheckGridDown;            //���̃O���b�h�Ƀ��m�����邩�ǂ���
    bool bCheckGridLeft;            //���̃O���b�h�Ƀ��m�����邩�ǂ���
    bool bCheckGridRight;           //�E�̃O���b�h�Ƀ��m�����邩�ǂ���
}BOMB;

//=============================================================================
//�v���g�^�C�v�錾
//=============================================================================
void InitBomb(void);
void UninitBomb(void);
void UpdateBomb(void);
void DrawBomb(void);
BOMB* GetBomb(void);
bool CollisionBombs(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size ,int nNumberBomb);    //���e���m�̓����蔻��
bool CollisionBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size);
bool HitBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size, int nNumberBomb);
void KickBomb(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size ,SELECT type);
void SetBomb(D3DXVECTOR3 pos,int nFire, int nCntTime , SELECT type);

//�L�b�N�p�́A���͂̃O���b�h���`�F�b�N����֐�
bool CheckGridUp(int nCntBomb);
bool CheckGridDown(int nCntBomb);
bool CheckGridLeft(int nCntBomb);
bool CheckGridRight(int nCntBomb);

void BreakBomb(int nCntBomb);   //�{����j�󂷂�
#endif