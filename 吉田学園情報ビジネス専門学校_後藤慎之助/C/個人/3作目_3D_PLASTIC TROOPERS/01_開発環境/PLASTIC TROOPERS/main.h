//=============================================================================
//
// PLASTIC TROOPERS [main.h]�̏��� (���C��)
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
#include <stdio.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xaudio2.h>
#include <time.h>

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
#define NUM_VERTEX		(4)
#define NUM_POLYGON	(2)
//���_�t�H�[�}�b�g2D		(���_���W|���_�J���[|�e�N�X�`�����W)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//���_�t�H�[�}�b�g3D		(���_���W|�m�[�}��|���_�J���[|�e�N�X�`�����W)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//�v���C���[�̊�{�ݒ�֘A
#define PLAYER_WIDTH (50.0f)        //�v���C���[�̉�
#define PLAYER_HEIGHT (50.0f)       //�v���C���[�̏c
#define PLAYER_LIFE (250.0f)        //�v���C���[�̗̑�

//�G�i�W�[�֘A
#define PLAYER_ENERGY (100.0f)      //�v���C���[�̃G�i�W�[
#define ENERGY_DASH (0.75f)          //�_�b�V�����̃G�i�W�[
#define ENERGY_BULLET_ATTACK (0.28f)       //�e�U�����̏���G�i�W�[
#define ENERGY_SWORD_ATTACK (0.25f)       //���U�����̏���G�i�W�[
#define START_HEAL_ENERGY (40)      //�G�i�W�[�񕜂��n�܂�܂ł̃t���[��
#define START_HEAL_OUT_ENERGY (180) //�G�i�W�[�؂ꂩ��A�G�i�W�[�񕜂��n�܂�܂ł̃t���[��
#define VALUE_HEAL_ENERGY (4.0f)    //�G�i�W�[�񕜗�

//�K�E�Q�[�W�֘A
#define PLAYER_CHANCE_METER_MAX (100.0f)  //�v���C���[�̕K�E�Q�[�W�̍ő�
#define PLAYER_CHANCE_MAX (3)       //�v���C���[�̕K�E�l�̍ő�
#define HIT_BULLET_CHARGE (1.5f)     //�e�q�b�g���̕K�E�Q�[�W������
#define HIT_SWORD_CHARGE (3.0f)     //���q�b�g���̕K�E�Q�[�W������
#define TAKE_DAMAGE_CHARGE (6.0f)   //�U�����󂯂����̕K�E�Q�[�W������
#define EXATTACK_CD (600)           //�K�E�Z�̃N�[���^�C��

//�v���C���[�̈ړ��֘A
#define VALUE_MOVE (5.0f)		    //�v���C���[�̈ړ���(5.0f)
#define VALUE_DASH (15.0f)          //�v���C���[�̃_�b�V��
#define VALUE_GUARD (2.5f)          //�K�[�h���̈ړ���
#define VALUE_AFTER_SKYDASH (7.5f)  //�v���C���[�̋󒆂̊���
#define VALUE_JUMP (10.5f)		    //�W�����v�̋���
#define SKY_DUSH_TIME (20)          //�󒆃_�b�V���̎���

#define VALUE_EXATTACK_FRAME (180)  //�X�g�[���\�[�h�̃t���[����

#define CAMERA_VIEW_RANGE (12000.0f)        //�J�����̎���

#define VALUE_KNOCKBACK (25.0f)     //�m�b�N�o�b�N�̋���

#define GRAVITY (0.46f)			    //�d��
#define MAX_MATERIAL 64             //���f���̃}�e���A���ő吔

#define COUNT_WAIT_NEXTSTAGE_GAME (1)	//���̃X�e�[�W�֍s���܂ł̎���
#define COUNT_WAIT_FINISH_GAME (60)	//���U���g��ʂ֍s���܂ł̎���

#define MOTION_KEYSET_MAX (32)			// �L�[�Z�b�g�ő吔

#define NORMAL_ROT_SPEED (0.05f)//�ʏ펞�̐U������X�s�[�h

//=============================================================================
//�\���̒�`
//=============================================================================
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	float rhw;			//���ʂɂ��邽��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`���p
}VERTEX_2D;	//2D���

typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`���p
}VERTEX_3D;	//3D���

typedef enum
{
	MODE_TITLE,		//�^�C�g��
	MODE_MANUAL,	//�V�ѕ�
	MODE_SELECT,	//�I��
	MODE_CAMPAIGN,	//�L�����y�[��
	MODE_SURVIVAL,	//�T�o�C�o��
	MODE_VERSUS,	//�o�[�T�X
	MODE_PRACTICE,	//�v���N�e�B�X
	MODE_RANKING,	//�����L���O
	MODE_RESULT,	//���U���g
	MODE_EXIT,		//EXIT
	MODE_MAX		//���[�h�̍ő吔
}MODE;//���[�h�̎��

typedef enum
{
	CSELECT_ART = 0,	//�A�[�T�[
	CSELECT_0,		//
	CSELECT_1,		//
	CSELECT_2,		//
	CSELECT_MAX			//�L�����̍ő吔
}CSELECT;	//�L�����̑I��

typedef enum
{
	WSELECT_SWORD = 0,	//�\�[�h
	WSELECT_LANCE,		//�����X
	WSELECT_HAMMER,		//�n���}�[
	WSELECT_DAGGER,		//�_�K�[
	WSELECT_HANDGUN,	//�n���h�K��
	WSELECT_SNIPER,		//�X�i�C�p�[
	WSELECT_MAX			//����I���̍ő吔
}WSELECT;	//����̑I���i�E��j

typedef enum
{
	SSELECT_NO = 0,		//���Ȃ�
	SSELECT_YES,		//������
	SSELECT_MAX			//���I���̍ő吔
}SSELECT;	//���̑I���i����j

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
