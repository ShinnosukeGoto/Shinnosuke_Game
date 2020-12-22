//=====================================================
//
// PLASTIC TROOPERS [meshfield.cpp]�̏��� (��)
// Author:�㓡�T�V��
//
//=====================================================

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "meshfield.h"

////=============================================================================
//// �v���g�^�C�v�錾
////=============================================================================
//void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// �O���[�o���ϐ��錾
//=============================================================================
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;		// �e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;

D3DXVECTOR3 g_posMeshField;				// �ʒu
D3DXVECTOR3 g_rotMeshField;				// �����i��]�j
D3DXMATRIX g_mtxWorldMeshField;			// ���[���h�}�g���b�N�X

int g_nNumVertexMeshField;				// �����_��
int g_nNumIndexMeshField;				// ���C���f�b�N�X��
int g_nNumPolygonMeshField;				// ���|���S����

float g_fOne_SizeW;
float g_fOne_SizeH;

//=============================================================================
// ����������
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	VERTEX_3D *pVtx;

	int nCount;

	int nCntIndex;
	int nCntIndex01 = 0;

	int nCntH;//X

	int nCntV;//Z

    //�N�������p
    /*bool bW=false;
    int nCntW=0;*/

	// �ʒu�E��]�̏����ݒ�
	g_posMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotMeshField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_nNumVertexMeshField = (FIELD_WIDTH + 1) * (FIELD_HEIGHT + 1);
	g_nNumIndexMeshField = (FIELD_WIDTH + 1) * FIELD_HEIGHT * 2 + (FIELD_HEIGHT - 1) * 2;
	g_nNumPolygonMeshField = FIELD_WIDTH * FIELD_HEIGHT * 2 + (FIELD_HEIGHT - 1) * 4;

	g_fOne_SizeW = FIELD_WIDTH_SIZE * 2 / FIELD_WIDTH;
	g_fOne_SizeH = FIELD_HEIGHT_SIZE * 2 / FIELD_HEIGHT;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "TEXTURE/sand4.jpg", &g_pTextureMeshField);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexMeshField, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, &g_pVtxBuffMeshField, NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexMeshField, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
		D3DPOOL_MANAGED, &g_pIdxBuffMeshField, NULL);





	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntV = 0; nCntV < FIELD_HEIGHT + 1; nCntV++)
	{
		for (nCntH = 0; nCntH < FIELD_WIDTH + 1; nCntH++)
		{
            /*bW = !bW;

            if (bW == true)
            {
                nCntW = 1;
            }
            else
            {
                nCntW = 0;
            }*/

			// ���_�̐ݒ�
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].pos = D3DXVECTOR3(-FIELD_WIDTH_SIZE + (nCntH * g_fOne_SizeW), 0.0f, FIELD_HEIGHT_SIZE - (nCntV * g_fOne_SizeH));

			// �@���x�N�g���̐ݒ�
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�̐ݒ�
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[(nCntV * (FIELD_HEIGHT + 1)) + nCntH].tex = D3DXVECTOR2(float(nCntH), float(nCntV));
		}
	}

	// ���_�f�[�^���A�����b�N����
	g_pVtxBuffMeshField->Unlock();




	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (nCount = 0, nCntIndex = 0; nCount < g_nNumIndexMeshField; nCount += 2)
	{
        //�k�ރ|���S�������
		if (((nCount + 2) % (((FIELD_WIDTH + 1) * 2) + 2)) == 0 && nCount != 0)
		{
			// �C���f�b�N�X�̐ݒ�
			pIdx[nCount] = nCntIndex - 1;

			// �C���f�b�N�X�̐ݒ�
			pIdx[nCount + 1] = FIELD_WIDTH + nCntIndex + 1;
		}
		else
		{
			// �C���f�b�N�X�̐ݒ�
			pIdx[nCount] = FIELD_WIDTH + nCntIndex + 1;

			// �C���f�b�N�X�̐ݒ�
			pIdx[nCount + 1] = nCntIndex;

			nCntIndex++;
		}

	}



	// ���_�f�[�^���A�����b�N����
	g_pIdxBuffMeshField->Unlock();

}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	// ���_�o�b�t�@�̊J��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̊J��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	// �g��E�k���𔽉f


	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(
		&mtxRot,
		g_rotMeshField.y,
		g_rotMeshField.x,
		g_rotMeshField.z);

	D3DXMatrixMultiply(
		&g_mtxWorldMeshField,
		&g_mtxWorldMeshField,
		&mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(
		&mtxTrans,
		g_posMeshField.x,
		g_posMeshField.y,
		g_posMeshField.z);

	D3DXMatrixMultiply(
		&g_mtxWorldMeshField,
		&g_mtxWorldMeshField,
		&mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetIndices(g_pIdxBuffMeshField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive
	(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexMeshField, 0, g_nNumPolygonMeshField);

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�


	// ���_���̐ݒ�

}
