//=============================================================================
//
// �i�C���X�g�[���[�Y�̏��� [select.cpp] (����Ɩ��@�̑I��)
// Author:�㓡�T�V��
//
//=============================================================================

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "input.h"
#include "game.h"
#include "select.h"
#include "fade.h"
#include "sound.h"
#include "manual.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureSelect[SELECT_UI] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelect = NULL;//���_�o�b�t�@�ւ̃|�C���^

WSELECT g_nWeapon;	//����
MSELECT g_nMagic;	//���@
DIRECT g_nSelectPos;//�I���J�[�\���̈ʒu
//=============================================================================
// �Z���N�g����������
//=============================================================================
void InitSelect(void)
{
	//�ϐ��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�I���̏�����
	g_nWeapon = WSELECT_SWORD;	//�ŏ��̃J�[�\���͌�
	g_nMagic = MSELECT_FIRE;	//�ŏ��̃J�[�\���͉�
	g_nSelectPos = DIRECT_UP;	//�ŏ��̃J�[�\���͏�

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select999.png", &g_pTextureSelect[0]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select000.png", &g_pTextureSelect[1]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select001.png", &g_pTextureSelect[2]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select002.png", &g_pTextureSelect[3]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select003.png", &g_pTextureSelect[4]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select004.png", &g_pTextureSelect[5]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select005.png", &g_pTextureSelect[6]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select100.png", &g_pTextureSelect[7]);
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/select100.png", &g_pTextureSelect[8]);

	//���_�o�b�t�@����
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * NUM_VERTEX*SELECT_UI,//�o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,//(�Œ�j
		FVF_VERTEX_2D,//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,//�i�Œ�j
		&g_pVtxBuffSelect,//�ϐ������ς��ƕύX���K�v
		NULL);
	VERTEX_2D*pVtx;//���_���̃|�C���^

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

	//���_���W�̐ݒ�i�E���Őݒ肷��j
	//�Z���N�g�̔w�i
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//��
	pVtx[4].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(426.6f, 0.0f,0.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, 360.0f, 0.0f);
	pVtx[7].pos = D3DXVECTOR3(426.3f, 360.0f, 0.0f);
	//��
	pVtx[8].pos = D3DXVECTOR3(426.6f, 0.0f, 0.0f);
	pVtx[9].pos = D3DXVECTOR3(853.3f, 0.0f, 0.0f);
	pVtx[10].pos = D3DXVECTOR3(426.6f, 360.0f, 0.0f);
	pVtx[11].pos = D3DXVECTOR3(853.3f, 360.0f, 0.0f);
	//�|
	pVtx[12].pos = D3DXVECTOR3(853.3f, 0.0f, 0.0f);
	pVtx[13].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[14].pos = D3DXVECTOR3(853.3f, 360.0f, 0.0f);
	pVtx[15].pos = D3DXVECTOR3(SCREEN_WIDTH, 360.0f, 0.0f);
	//��
	pVtx[16].pos = D3DXVECTOR3(0.0f, 360.0f, 0.0f);
	pVtx[17].pos = D3DXVECTOR3(426.6f, 360.0f, 0.0f);
	pVtx[18].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[19].pos = D3DXVECTOR3(426.3f, SCREEN_HEIGHT, 0.0f);
	//��
	pVtx[20].pos = D3DXVECTOR3(426.6f, 360.0f, 0.0f);
	pVtx[21].pos = D3DXVECTOR3(853.3f, 360.0f, 0.0f);
	pVtx[22].pos = D3DXVECTOR3(426.6f, SCREEN_HEIGHT, 0.0f);
	pVtx[23].pos = D3DXVECTOR3(853.3f, SCREEN_HEIGHT, 0.0f);
	//�z��
	pVtx[24].pos = D3DXVECTOR3(853.3f, 360.0f, 0.0f);
	pVtx[25].pos = D3DXVECTOR3(SCREEN_WIDTH, 360.0f, 0.0f);
	pVtx[26].pos = D3DXVECTOR3(853.3f, SCREEN_HEIGHT, 0.0f);
	pVtx[27].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//�����w�i�i��j
	pVtx[28].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[29].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[30].pos = D3DXVECTOR3(0.0f, 360, 0.0f);
	pVtx[31].pos = D3DXVECTOR3(SCREEN_WIDTH, 360, 0.0f);
	//�����w�i�i���j
	pVtx[32].pos = D3DXVECTOR3(0.0f, 360, 0.0f);
	pVtx[33].pos = D3DXVECTOR3(SCREEN_WIDTH, 360, 0.0f);
	pVtx[34].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[35].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	pVtx[4].rhw = 1.0f;
	pVtx[5].rhw = 1.0f;
	pVtx[6].rhw = 1.0f;
	pVtx[7].rhw = 1.0f;
	pVtx[8].rhw = 1.0f;
	pVtx[9].rhw = 1.0f;
	pVtx[10].rhw = 1.0f;
	pVtx[11].rhw = 1.0f;
	pVtx[12].rhw = 1.0f;
	pVtx[13].rhw = 1.0f;
	pVtx[14].rhw = 1.0f;
	pVtx[15].rhw = 1.0f;
	pVtx[16].rhw = 1.0f;
	pVtx[17].rhw = 1.0f;
	pVtx[18].rhw = 1.0f;
	pVtx[19].rhw = 1.0f;
	pVtx[20].rhw = 1.0f;
	pVtx[21].rhw = 1.0f;
	pVtx[22].rhw = 1.0f;
	pVtx[23].rhw = 1.0f;
	pVtx[24].rhw = 1.0f;
	pVtx[25].rhw = 1.0f;
	pVtx[26].rhw = 1.0f;
	pVtx[27].rhw = 1.0f;
	pVtx[28].rhw = 1.0f;
	pVtx[29].rhw = 1.0f;
	pVtx[30].rhw = 1.0f;
	pVtx[31].rhw = 1.0f;
	pVtx[32].rhw = 1.0f;
	pVtx[33].rhw = 1.0f;
	pVtx[34].rhw = 1.0f;
	pVtx[35].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);

	pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 0);

	pVtx[32].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[33].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[34].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	pVtx[35].col = D3DCOLOR_RGBA(255, 255, 255, 155);

	//�e�N�X�`���̍��W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[4].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[10].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[12].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[13].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[14].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[15].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[16].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[17].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[18].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[19].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[20].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[21].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[22].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[23].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[24].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[25].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[26].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[27].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[28].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[29].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[30].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[31].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx[32].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[33].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[34].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[35].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�f�[�^�A�����b�N
	g_pVtxBuffSelect->Unlock();
}
//=============================================================================
// �Z���N�g�X�V����
//=============================================================================
void UpdateSelect(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^
	int nFade = GetFade();	//�t�F�[�h�̏����l��

	//���_�o�b�t�@�����b�N
	g_pVtxBuffSelect->Lock(0, 0, (void**)&pVtx, 0);

	//����I����
	if (g_nSelectPos == DIRECT_UP)
	{
		//��̍����w�i
		pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 0);

		//���̍����w�i
		pVtx[32].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[33].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[34].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[35].col = D3DCOLOR_RGBA(255, 255, 255, 155);
	}

	//���@�I����
	else if (g_nSelectPos == DIRECT_DOWN)
	{
		//��̍����w�i
		pVtx[28].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[29].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[30].col = D3DCOLOR_RGBA(255, 255, 255, 155);
		pVtx[31].col = D3DCOLOR_RGBA(255, 255, 255, 155);

		//���̍����w�i
		pVtx[32].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[33].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[34].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[35].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	}

	//���I�����A�������邢
	if (g_nWeapon == WSELECT_SWORD)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//���I�����A�������邢
	else if (g_nWeapon == WSELECT_LANCE)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//�|�I�����A�|�����邢
	else if (g_nWeapon == WSELECT_BOW)
	{
		pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[15].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	//���I�����A�������邢
	if (g_nMagic == MSELECT_FIRE)
	{
		pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//���I�����A�������邢
	else if (g_nMagic == MSELECT_LIGHTNING)
	{
		pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 55);
	}

	//�z���I�����A�z�������邢
	else if (g_nMagic == MSELECT_DRAIN)
	{
		pVtx[16].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[17].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[18].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[19].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[20].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[21].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[22].col = D3DCOLOR_RGBA(255, 255, 255, 55);
		pVtx[23].col = D3DCOLOR_RGBA(255, 255, 255, 55);

		pVtx[24].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[25].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[26].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[27].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}


	//���_�f�[�^���A�����b�N����
	g_pVtxBuffSelect->Unlock();

	//W�L�[�������ꂽ�i��ړ�,����I���j
	if (GetKeyboardTrigger(DIK_W) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_UP;
	}
	//S�L�[�������ꂽ�i���ړ�,���@�I���j
	else if (GetKeyboardTrigger(DIK_S) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_DOWN;
	}
	//���L�[�������ꂽ�i��ړ�,����I���j
	else if (GetKeyboardTrigger(DIK_UP) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_UP;
	}
	//���L�[�������ꂽ�i���ړ�,���@�I���j
	else if (GetKeyboardTrigger(DIK_DOWN) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_DOWN;
	}

	//����I��
	if (g_nSelectPos == DIRECT_UP)
	{
		//A�L�[�������ꂽ�i���ړ��j
		if (GetKeyboardTrigger(DIK_A) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nWeapon == WSELECT_LANCE)
			{
				g_nWeapon = WSELECT_SWORD;
			}
			else if (g_nWeapon == WSELECT_BOW)
			{
				g_nWeapon = WSELECT_LANCE;
			}
		}
		//D�L�[�������ꂽ�i�E�ړ��j
		else if (GetKeyboardTrigger(DIK_D) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nWeapon == WSELECT_SWORD)
			{
				g_nWeapon = WSELECT_LANCE;
			}
			else if (g_nWeapon == WSELECT_LANCE)
			{
				g_nWeapon = WSELECT_BOW;
			}
		}
		//���L�[�������ꂽ�i���ړ��j
		else if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nWeapon == WSELECT_LANCE)
			{
				g_nWeapon = WSELECT_SWORD;
			}
			else if (g_nWeapon == WSELECT_BOW)
			{
				g_nWeapon = WSELECT_LANCE;
			}
		}
		//���L�[�������ꂽ�i�E�ړ��j
		else if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nWeapon == WSELECT_SWORD)
			{
				g_nWeapon = WSELECT_LANCE;
			}
			else if (g_nWeapon == WSELECT_LANCE)
			{
				g_nWeapon = WSELECT_BOW;
			}
		}
	}

	//���@�I��
	else if (g_nSelectPos == DIRECT_DOWN)
	{
		//A�L�[�������ꂽ�i���ړ��j
		if (GetKeyboardTrigger(DIK_A) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nMagic == MSELECT_LIGHTNING)
			{
				g_nMagic = MSELECT_FIRE;
			}
			else if (g_nMagic == MSELECT_DRAIN)
			{
				g_nMagic = MSELECT_LIGHTNING;
			}
		}
		//D�L�[�������ꂽ�i�E�ړ��j
		else if (GetKeyboardTrigger(DIK_D) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nMagic == MSELECT_FIRE)
			{
				g_nMagic = MSELECT_LIGHTNING;
			}
			else if (g_nMagic == MSELECT_LIGHTNING)
			{
				g_nMagic = MSELECT_DRAIN;
			}
		}
		//���L�[�������ꂽ�i���ړ��j
		else if (GetKeyboardTrigger(DIK_LEFT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nMagic == MSELECT_LIGHTNING)
			{
				g_nMagic = MSELECT_FIRE;
			}
			else if (g_nMagic == MSELECT_DRAIN)
			{
				g_nMagic = MSELECT_LIGHTNING;
			}
		}
		//���L�[�������ꂽ�i�E�ړ��j
		else if (GetKeyboardTrigger(DIK_RIGHT) && nFade == FADE_NONE)
		{
			PlaySound(SOUND_LABEL_SE_JUMP);
			if (g_nMagic == MSELECT_FIRE)
			{
				g_nMagic = MSELECT_LIGHTNING;
			}
			else if (g_nMagic == MSELECT_LIGHTNING)
			{
				g_nMagic = MSELECT_DRAIN;
			}
		}
	}

	//Enter�L�[����������
	//����I�𒆂́A���@�I���Ɉڍs����
	if (g_nSelectPos == DIRECT_UP && GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_JUMP);
		g_nSelectPos = DIRECT_DOWN;
	}

	//���@�I�𒆂ɉ����ƁA�}�j���A����ʂֈڍs
	else if (g_nSelectPos == DIRECT_DOWN && GetKeyboardTrigger(DIK_RETURN) && nFade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ITEM);	//�A�C�e���擾���Ńv���X�G���^�[���̑�p
		InitManual();		//�}�j���A����ʂ̏�����
		InitGameSecond();	//�Q�[���̏�����2
		SetFade(MODE_MANUAL);//�}�j���A����ʂɈړ�
	}
}
//=============================================================================
// �Z���N�g�I������
//=============================================================================
void UninitSelect(void)
{
	//�e�N�X�`���̊J��
	for (int nCntSelect = 0; nCntSelect < SELECT_UI; nCntSelect++)
	{
		if (g_pTextureSelect[nCntSelect] != NULL)
		{
			g_pTextureSelect[nCntSelect]->Release();
			g_pTextureSelect[nCntSelect] = NULL;
		}
	}

	//���_�o�b�t�@�̊J��
	if (g_pVtxBuffSelect != NULL)
	{
		g_pVtxBuffSelect->Release();
		g_pVtxBuffSelect = NULL;
	}
}
//=============================================================================
// �Z���N�g�`�揈��
//=============================================================================
void DrawSelect(void)
{
	//���_�t�H�[�}�b�g�̐ݒ�
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffSelect, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCntSelect = 0; nCntSelect < SELECT_UI; nCntSelect++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSelect[nCntSelect]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntSelect, NUM_POLYGON);
	}
}

//=============================================================================
// ����I���̎擾
//=============================================================================
WSELECT GetWSelect(void)
{
	return g_nWeapon;
}

//=============================================================================
// ���@�I���̎擾
//=============================================================================
MSELECT GetMSelect(void)
{
	return g_nMagic;
}