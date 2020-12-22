//=============================================================================
//
// �ŋ��̘b����� [main.h]�̏��� (���C��)
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
#define CAMERA_VIEW_RANGE (10000.0f)        //�J�����̎���
#define AVOID_INFINITE_LOOP 300             //�������[�v���

//���_�t�H�[�}�b�g2D		(���_���W|���_�J���[|�e�N�X�`�����W)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
//���_�t�H�[�}�b�g3D		(���_���W|�m�[�}��|���_�J���[|�e�N�X�`�����W)
#define FVF_VERTEX_3D	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//�Q�[����Ԃ̊Ǘ��t���[��
#define COUNT_WAIT_START_GAME (120)	        //�Q�[���J�n�֍s���܂ł̎���
#define COUNT_WAIT_FINISH_GAME (180)	    //���U���g��ʂ֍s���܂ł̎���

//�v���C���[�̊�{�ݒ�֘A
#define MAX_PARTS (10)				//�p�[�c�̐�
#define MOTION_KEYSET_MAX (32)		//�L�[�Z�b�g�ő吔
#define MAX_MATERIAL (64)           //���f���̃}�e���A���ő吔
#define PLAYER_WIDTH (50.0f)        //�v���C���[�̉�
#define PLAYER_HEIGHT (50.0f)       //�v���C���[�̏c
#define PLAYER_LIFE (5.0f)          //�v���C���[�̗̑�
#define PLAYER_STORY_LIFE (250.0f)  //�X�g�[���[�̗̑�
#define PLAYER_FIRST_FIRE (2)       //�v���C���[�̏����Η�
#define PLAYER_FIRST_BOMB (1)       //�v���C���[�̏����{����
#define PLAYER_FIRST_SPEEDRANK (1)  //�v���C���[�̏����X�s�[�h�����N
#define PLAYER_FIRST_KICK (0)       //�v���C���[�̏����L�b�N��
#define GRAVITY 0.4f                //�d��
#define VALUE_KNOCKBACK (25.0f)     //�m�b�N�o�b�N�̋���
#define PLAYER_INVINCIBLE_FRAME 90  //���G����

#define ENEMY_INVINCIBLE_FRAME 60   //�G�̖��G����

//�v���C���[�̈ړ��֘A
#define VALUE_MOVE (5.0f)		    //�v���C���[�̏����ړ���(5.0f)
#define VALUE_UPMOVE (1.0f)         //�X�s�[�h�����N�ɉ����Ă�����ړ���
#define COLLISION_AVOID 49.0f       //�Փ˂𒲐�

//��}�X�̑傫��
#define GRID_SIZE (100.0f)

//�}�X�̐�
//�o�g�����C�����̏ꍇ
#define MESH_BLOCK_X 15                                                                 //���b�V���̃u���b�N��(x����)
#define MESH_BLOCK_Z 13                                                                 //���b�V���̃u���b�N��(z����)
#define MAX_MESH_BLOCK (MESH_BLOCK_Z * MESH_BLOCK_X)                                    //���b�V���u���b�N�̍��v
#define MESH_VERTEX (MESH_BLOCK_X + 1) * (MESH_BLOCK_Z + 1)                             //���_�̐�
#define MESH_INDEX (((MESH_BLOCK_X + 1) * 2) * MESH_BLOCK_Z + ((MESH_BLOCK_Z - 1) * 2)) //�C���f�b�N�X��
#define MESH_POLYGON (MESH_BLOCK_X * 2 * MESH_BLOCK_Z) + (MESH_BLOCK_Z - 1) * 4         //�|���S���̐�

//�X�g�[���[�̏ꍇ
#define STORY_MESH_BLOCK_X 60    //���b�V���̃u���b�N��(x����)
#define STORY_MESH_BLOCK_Z 13    //���b�V���̃u���b�N��(z����)
#define MAX_STORY_MESH_BLOCK (STORY_MESH_BLOCK_X * STORY_MESH_BLOCK_Z)
#define STORY_MESH_VERTEX (STORY_MESH_BLOCK_X + 1) * (STORY_MESH_BLOCK_Z + 1)//���_�̐�
#define STORY_MESH_INDEX (((STORY_MESH_BLOCK_X + 1) * 2) * STORY_MESH_BLOCK_Z + ((STORY_MESH_BLOCK_Z - 1) * 2))  //�C���f�b�N�X��
#define STORY_MESH_POLYGON (STORY_MESH_BLOCK_X * 2 * STORY_MESH_BLOCK_Z) + (STORY_MESH_BLOCK_Z - 1) * 4       //�|���S���̐�

//�{���֘A
#define TIME_BOMB 180       //�{���̔����܂ł̎���
#define TIME_FIRE 30        //���̎�������
#define BOMB_DAMAGE 25.0f   //���e�̃_���[�W
#define BOMB_FLINCH 50      //�{���̋��ݒl

//�󂹂�u���b�N�֘A
#define MAX_BROKEN_BLOCK 92 //�󂹂�u���b�N�̍ő吔
#define MAX_FIRE_BLOCK 6    //���o���u���b�N�̍ő吔
#define MAX_BOMB_BLOCK 10   //�{���o���u���b�N�̍ő吔
#define MAX_SPEED_BLOCK 4   //�X�s�[�h�o���u���b�N�̍ő吔
#define MAX_KICK_BLOCK 4    //�L�b�N�o���u���b�N�̍ő吔

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
    MODE_MENU,	    //���j���[
    MODE_SETTING,   //�ݒ�
    MODE_MANUAL,	//�V�ѕ�
    MODE_GAME,	    //�Q�[��
    MODE_NEXT_GAME,	//���̃Q�[��(���E���h)
    MODE_RESULT,	//���U���g
    MODE_RANKING,	//�����L���O
    MODE_EXIT,		//EXIT
    MODE_MAX		//���[�h�̍ő吔
}MODE;//���[�h�̎��

typedef enum
{
    SELECT_MOMO = 0,	//�����Y
    SELECT_URA,		    //�Y�����Y
    SELECT_KIN,		    //�����Y
    SELECT_ISUN,		//�ꐡ�@�t
    SELECT_BOSS,        //�{�X
    SELECT_MAX			//�L�����̍ő吔
}SELECT;	//�L�����̑I��

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
