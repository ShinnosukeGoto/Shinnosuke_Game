//=============================================================================
//
// DirectX3D�̏��� [light.cpp] (�Ɩ�)
// Author : �㓡�T�V��
//
//=============================================================================
#include "light.h"

//=============================================================================
//�O���[�o���ϐ��錾
//=============================================================================
D3DLIGHT9 g_Light;	//���C�g�̏��

					//=============================================================================
					// ���C�g����������
					//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�|�C���^
	D3DXVECTOR3 vecDir;	//���C�g�̕����x�N�g��

						//���C�g���N���A����
	ZeroMemory(&g_Light, sizeof(D3DLIGHT9));

	//���C�g�̎�ނ�ݒ�
	g_Light.Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U����ݒ�
	g_Light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);	//���K������(�傫��1�̃x�N�g���ɂ���K�v������)
	g_Light.Direction = vecDir;

	//���C�g��ݒ肷��
	pDevice->SetLight(0, &g_Light);

	//���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);

	//// �e�N�X�`���E�t�B���^�����O
	//pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �g��p(�o�C���j�A�E�t�B���^�����O)
	//pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �k���p(�o�C���j�A�E�t�B���^�����O)

	//pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �g��p(�o�C���j�A�E�t�B���^�����O)
	//pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �k���p(�o�C���j�A�E�t�B���^�����O)


	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);	// Z�o�b�t�@�g��
	//pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);	// ���_�@���̎������K��


	//															// �X�e���V���o�b�t�@�̐ݒ�
	//pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);
	//pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	//pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	//pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);

}

//=============================================================================
//���C�g�I������
//=============================================================================
void UninitLight(void)
{

}

//=============================================================================
// ���C�g�X�V����
//=============================================================================
void UpdateLight(void)
{

}