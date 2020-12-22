//=============================================================================
//
// �i�C���X�g�[���[�Y [player.h]�̏��� (���@)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
//�}�N����`
//=============================================================================
#define PLAYER_SIZE (100.0f)			//���@�̃T�C�Y
#define PLAYER_LIFE (250.0f)		//���@�̃��C�t
#define PLAYER_MP (100.0f)			//���@��MP
#define COUNT_WAIT_NEXT_PLAYER (90)	//���̎��@���ݒu�����܂ł̎���

//=============================================================================
//�\���̒�`
//=============================================================================
//���@�̏��
typedef enum
{
	PLAYERSTATE_NORMAL = 0,	//�ʏ�
	PLAYERSTATE_DAMAGE,		//�_���[�W
	PLAYERSTATE_APPEAR,		//�o���܂ł̏��
	PLAYERSTATE_MAX			//��Ԃ̍ő吔
}PLAYERSTATE;

typedef struct
{
	D3DXVECTOR3 pos;	//���݂̈ʒu
	D3DXVECTOR3 posOld;	//�O�̈ʒu
	D3DXVECTOR3 move;	//�ړ�
	PLAYERSTATE state;	//���
	int nLife;			//���C�t
	int nMp;			//MP
	int nStock;			//�c�@
	int nCounterState;	//��ԃJ�E���^
	bool bJump;			//�W�����v
	bool bSecondJump;	//��i�ڂ̃W�����v
	bool bLeft;			//���������Ă���
	bool bDisp;			//�\���ؑ�
	WSELECT Wtype;		//����I��
	MSELECT Mtype;		//���@�I��
	int nAttackTime;	//�U������
}PLAYER;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER*GetPlayer(void);
void HitPlayer(int nDamage);
void SetPlayer(D3DXVECTOR3 pos);
bool HitPlayer2(void);
#endif