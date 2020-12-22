//=============================================================================
//
// �i�C���X�g�[���[�Y [main.h]�̏��� (���C��)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xaudio2.h>

//=============================================================================
// ���C�u�����̃����N
//=============================================================================
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"winmm.lib")

//=============================================================================
// �}�N����`
//=============================================================================
#define	WINDOW_POS_X	(0)
#define	WINDOW_POS_Y	(0)
#define SCREEN_WIDTH	(1280)
#define SCREEN_HEIGHT	(720)
#define SCREEN_HEIGHT�U	(713)
#define NUM_VERTEX		(4)
#define NUM_POLYGON	(2)
#define RATE_MOVE (0.12)
//���_�t�H�[�}�b�g		(���_���W|���_�J���[|�e�N�X�`�����W)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

#define FIRE_MP (25)			//����MP����
#define LIGHTNING_MP (35)		//����MP����
#define DRAIN_MP (30)			//�z����MP����
#define VALUE_MOVE (5.0f)		//�v���C���[�̈ړ���(5.0f)
#define VALUE_JUMP (20.0f)		//�W�����v�̋���
#define VALUE_JUMP2 (17.5f)		//2�i�ڂ̃W�����v�̋���
#define VALUE_LANCEJUMP (16.0f)	//���̏�U���̃W�����v�̋���
#define VALUE_SMALLJUMP (11.5f)	//���W�����v�̋���
#define SNOW_CD (15)			//���̐�̑����܂ł̎���
#define ATTACK_CD (22)			//���̍U���܂ł̎���
#define INVINCIBLE_FRAME (15)	//���G�t���[��
#define INVINCIBLE_FRAME�U (23)	//�{�X�̖��G�t���[��
#define GRAVITY (1.1f)			//�d��
#define STAGE1_X (2000)			//�X�e�[�W1�̉���
#define COUNT_WAIT_NEXTSTAGE_GAME (1)	//���̃X�e�[�W�֍s���܂ł̎���
#define COUNT_WAIT_FINISH_GAME (30)	//���U���g��ʂ֍s���܂ł̎���

//=============================================================================
//�\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

typedef enum
{
	MODE_TITLE,		//�^�C�g��
	MODE_MANUAL,	//�V�ѕ�
	MODE_SELECT,	//�I�����
	MODE_GAME,		//�Q�[��1(��̕���)
	MODE_GAME2,		//�Q�[��2(������)
	MODE_GAME3,		//�Q�[��3(���̒�)
	MODE_RESULT,	//���U���g
	MODE_RESULT2,	//�����L���O�I�ȃ��U���g2
	MODE_MAX		//���[�h�̍ő吔
}MODE;//���[�h�̎��

typedef enum
{
	WSELECT_SWORD = 0,	//��
	WSELECT_LANCE,		//��
	WSELECT_BOW,		//�|
	WSELECT_MAX			//����̍ő吔
}WSELECT;	//����̑I��

typedef enum
{
	MSELECT_FIRE = 0,	//��
	MSELECT_LIGHTNING,	//��
	MSELECT_DRAIN,		//�z��
	MSELECT_MAX			//���@�̍ő吔
}MSELECT;	//���@�̑I��

typedef enum
{
	DIRECT_RIGHT,	//�E
	DIRECT_LEFT,	//��
	DIRECT_UP,		//��
	DIRECT_DOWN,	//��
	DIRECT_MAX
}DIRECT;//�����̎��

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void);

void SetMode(MODE mode);
MODE GetMode(void);
#endif

