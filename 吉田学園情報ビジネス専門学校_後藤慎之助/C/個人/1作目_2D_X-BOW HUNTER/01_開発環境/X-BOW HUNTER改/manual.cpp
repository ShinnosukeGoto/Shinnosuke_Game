//=============================================================================
//
// X-Bow Hunter�̏��� [manual.cpp] (�V�ѕ�)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "input.h"
#include "game.h"
#include "main.h"
#include "fade.h"
#include "sound.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureManual = NULL;//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManual = NULL;//���_�o�b�t�@�ւ̃|�C���^

						//=============================================================================
						// �}�j���A������������
						//=============================================================================
void InitManual(void)
{
	//�ϐ��錾
	VERTEX_2D*pVtx;//���_���̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "manual000.png", &g_pTextureManual);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffManual,//�ϐ������ς��ƕύX���K�v
		NULL);

								//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffManual->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

												   //���_���W�̐ݒ�i�E���Őݒ肷��j
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�A�����b�N
	g_pVtxBuffManual->Unlock();
}
//=============================================================================
// �}�j���A���X�V����
//=============================================================================
void UpdateManual(void)
{
	int nFade = GetFade();	//�t�F�[�h�̏����l��

	//Enter�L�[����������
	if (GetKeyboardPress(DIK_RETURN)&&nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾���Ńv���X�G���^�[���̑�p
		InitGame2();		//�Q�[���̏�����2
		SetFade(MODE_GAME);//�Q�[����ʂɈړ�
		StopSound(SOUND_LABEL_BGM000);//�Ȃ̒�~(����̕�)
	}
}
//=============================================================================
// �I������
//=============================================================================
void UninitManual(void)
{
	//�e�N�X�`���̊J��
	if (g_pTextureManual != NULL)
	{
		g_pTextureManual->Release();
		g_pTextureManual = NULL;
	}
	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffManual != NULL)
	{
		g_pVtxBuffManual->Release();
		g_pVtxBuffManual = NULL;
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawManual(void)
{
	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffManual, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

								   //�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureManual);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * 0, NUM_POLYGON);
}