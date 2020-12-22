//=============================================================================
//
// X-Bow Hunter [main.h]�̏��� (���C��)
// Author : �㓡�T�V��
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

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
#define NUM_VERTEX		(4)
#define NUM_POLYGON	(2)
#define RATE_MOVE (0.12)
//���_�t�H�[�}�b�g		(���_���W|���_�J���[|�e�N�X�`�����W)
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

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
	MODE_TITLE,
	MODE_MANUAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_RESULT2,
	MODE_MAX
}MODE;//���[�h�̎��

	  //=============================================================================
	  // �v���g�^�C�v�錾
	  //=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void);

void SetMode(MODE mode);
MODE GetMode(void);
#endif

