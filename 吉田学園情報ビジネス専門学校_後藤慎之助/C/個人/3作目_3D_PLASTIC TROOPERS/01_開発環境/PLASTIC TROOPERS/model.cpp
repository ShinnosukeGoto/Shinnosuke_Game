//=====================================================
//
// PLASTIC TROOPERS�̏��� [block.cpp](�r���Ȃǂ̃u���b�N)
// Author:�㓡�T�V��
//
//=====================================================

//=====================================================
// �C���N���[�h�t�@�C��
//=====================================================
#include "model.h"

//=====================================================
// �O���[�o���ϐ�
//=====================================================
ORNAMENT g_Model[MAX_MODEL];

// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffModel[MODELTYPE_MAX] = {};

// ���b�V�����ւ̃|�C���^
LPD3DXMESH g_pMeshModel[MODELTYPE_MAX] = {};
// �}�e���A�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatModel[MODELTYPE_MAX] = {};
// �}�e���A�����̐�
DWORD g_nNumMatModel[MODELTYPE_MAX] = {};


//=====================================================
// ����������
//=====================================================
void InitModel(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^


    int nCntModel;

    // �z�񏉊���
    for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
    {
        g_Model[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Model[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Model[nCntModel].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_Model[nCntModel].type = MODELTYPE_000;
        g_Model[nCntModel].bUse = false;
    }

    // X�t�@�C���̓ǂݍ���
    //��
    D3DXLoadMeshFromX("MODEL/model000.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_000],
        NULL,
        &g_nNumMatModel[MODELTYPE_000],
        &g_pMeshModel[MODELTYPE_000]);

    //����������
    D3DXLoadMeshFromX("MODEL/model001.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_001],
        NULL,
        &g_nNumMatModel[MODELTYPE_001],
        &g_pMeshModel[MODELTYPE_001]);

    //�Ƃ�
    D3DXLoadMeshFromX("MODEL/start.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_002],
        NULL,
        &g_nNumMatModel[MODELTYPE_002],
        &g_pMeshModel[MODELTYPE_002]);

    //��
    D3DXLoadMeshFromX("MODEL/moon.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_003],
        NULL,
        &g_nNumMatModel[MODELTYPE_003],
        &g_pMeshModel[MODELTYPE_003]);

    //�s���~�b�h
    D3DXLoadMeshFromX("MODEL/model002.x",
        D3DXMESH_SYSTEMMEM,
        pDevice,
        NULL,
        &g_pBuffMatModel[MODELTYPE_004],
        NULL,
        &g_nNumMatModel[MODELTYPE_004],
        &g_pMeshModel[MODELTYPE_004]);

    for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
    {
        D3DXMATERIAL* d3dxmatrs = (D3DXMATERIAL*)g_pBuffMatModel[MODELTYPE_004]->GetBufferPointer();
        for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[MODELTYPE_004]; nCntMat++)
        {
            if (d3dxmatrs[nCntMat].pTextureFilename != NULL)
            {
                D3DXCreateTextureFromFile(pDevice, d3dxmatrs[nCntMat].pTextureFilename, &g_Model[nCntModel].textureModel[nCntMat]);
            }
        }
    }
}

//=====================================================
// �I������
//=====================================================
void UninitModel(void)
{
    for (int nCntModel = 0; nCntModel < MODELTYPE_MAX; nCntModel++)
    {
        // ���b�V���̔j��
        if (g_pMeshModel[nCntModel] != NULL)
        {
            g_pMeshModel[nCntModel]->Release();
            g_pMeshModel[nCntModel] = NULL;
        }
        // �}�e���A���̔j��
        if (g_pBuffMatModel[nCntModel] != NULL)
        {
            g_pBuffMatModel[nCntModel]->Release();
            g_pBuffMatModel[nCntModel] = NULL;
        }

        // �e�N�X�`���̔j��
        for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[MODELTYPE_004]; nCntMat++)
        {
            
            if (g_Model[nCntModel].textureModel[nCntMat] != NULL)
            {
                g_Model[nCntModel].textureModel[nCntMat]->Release();
                g_Model[nCntModel].textureModel[nCntMat] = NULL;
            }
        }
    }
}

//=====================================================
// �X�V����
//=====================================================
void UpdateModel(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void DrawModel(void)
{
    int nCntModel;
    LPDIRECT3DDEVICE9 pDevice = GetDevice();		// �f�o�C�X�ւ̃|�C���^
    D3DXMATRIX mtxRot, mtxTrans, mtxScale;
    D3DMATERIAL9 matDef;		// ���݂̃}�e���A���ێ��p
    D3DXMATERIAL* pMat;			// �}�e���A���f�[�^�ւ̃|�C���^


    for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
    {
        if (g_Model[nCntModel].bUse == true)
        {
            // ���[���h�}�g���b�N�X�̏�����
            D3DXMatrixIdentity(&g_Model[nCntModel].mtxWorld);

            // �X�P�[���𔽉f
            D3DXMatrixScaling(&mtxScale,
                g_Model[nCntModel].size.x / g_Model[nCntModel].size.x,
                g_Model[nCntModel].size.y / g_Model[nCntModel].size.y,
                g_Model[nCntModel].size.z / g_Model[nCntModel].size.z);
            D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxScale);
            // �����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot,
                g_Model[nCntModel].rot.y,
                g_Model[nCntModel].rot.x,
                g_Model[nCntModel].rot.z);
            D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxRot);
            // �ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans,
                g_Model[nCntModel].pos.x,
                g_Model[nCntModel].pos.y,
                g_Model[nCntModel].pos.z);
            D3DXMatrixMultiply(&g_Model[nCntModel].mtxWorld, &g_Model[nCntModel].mtxWorld, &mtxTrans);


            // ���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_Model[nCntModel].mtxWorld);
            // �e�N�X�`���̐ݒ�
            pDevice->SetTexture(0, NULL);
            // ���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);
            // �}�e���A���f�[�^�ւ̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_pBuffMatModel[g_Model[nCntModel].type]->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[g_Model[nCntModel].type]; nCntMat++)
            {
                // �e�N�X�`���̐ݒ�
                pDevice->SetTexture(0, g_Model[nCntModel].textureModel[nCntMat]);
                pMat[nCntMat].MatD3D.Ambient.r = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.g = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.b = 1.0f;
                pMat[nCntMat].MatD3D.Ambient.a = 1.0f;
                // �}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pMat[nCntMat].MatD3D.Ambient = pMat[nCntMat].MatD3D.Diffuse;
                // ���f���p�[�c
                g_pMeshModel[g_Model[nCntModel].type]->DrawSubset(nCntMat);
            }
            // �ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }
}

//=====================================================
// �Z�b�g�u���b�N
//=====================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 size, MODELTYPE type)
{

    int nCntModel;				// ���[�J���ϐ�

    for (nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
    {
        // �u���b�N���g���ĂȂ��Ƃ�
        if (g_Model[nCntModel].bUse == false)
        {
            // �Ώۂ̔z��̗v�f�ɁAused��pos��size���Z�b�g

            // �u���b�N���g�p���Ă����Ԃɂ���

            g_Model[nCntModel].pos = pos;
            g_Model[nCntModel].size = size;
            g_Model[nCntModel].type = type;
            g_Model[nCntModel].bUse = true;
            break;
        }
    }
}