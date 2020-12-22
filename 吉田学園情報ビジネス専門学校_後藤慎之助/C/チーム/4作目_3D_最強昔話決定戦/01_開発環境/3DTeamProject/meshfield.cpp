//=============================================================================
//
//  �ŋ��̘b�����̏���  [meshfield.cpp](�v���C���[�̕����鏰)
//  Author:KUDO AYANO
//
//=============================================================================
#include "meshfield.h"
#include "setting.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MESHFILD_TEXTURE "TEXTURE/check.png"

//*****************************************************************************
// �O���[�o���ϐ�
//*********a********************************************************************
LPDIRECT3DTEXTURE9 g_pVtxTextureMeshField = NULL;        //���b�V���t�B�[���h�̃e�N�X�`��
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;      //���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;       //�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posMeshField;	//���b�V���t�B�[���h�̈ʒu
D3DXVECTOR3 g_rotMashField;	//���b�V���t�B�[���h�̌���
D3DXMATRIX g_mtxWorldMeshField;	//���b�V���t�B�[���h�̃��[���h�}�g���b�N�X

int g_nMeshBlockX;   //���b�V���̃u���b�N��(x����)
int g_nMeshBlockZ;   //���b�V���̃u���b�N��(z����)
int g_nMaxMeshBlock; //���b�V���u���b�N�̍��v
int g_nMeshVertex;   //���_�̐�
int g_nMeshIndex;    //�C���f�b�N�X��
int g_nMeshPolygon;  //�|���S���̐�

//=============================================================================
// ���b�V���t�B�[���h����������
// Author:KUDO AYANO
//=============================================================================
void InitMeshField(void)
{
    //�o�g�����C�������X�g�[���[�����擾����
    SETTING *pSetting = GetSetting();

    //�o�g�����C�����Ȃ�
    if (pSetting->nMaxPlayer >= 2)
    {
        g_nMeshBlockX = MESH_BLOCK_X;
        g_nMeshBlockZ = MESH_BLOCK_Z;
        g_nMaxMeshBlock = MAX_MESH_BLOCK;
        g_nMeshVertex = MESH_VERTEX;
        g_nMeshIndex = MESH_INDEX;
        g_nMeshPolygon = MESH_POLYGON;
    }

    //�X�g�[���[�Ȃ�
    else
    {
        g_nMeshBlockX = STORY_MESH_BLOCK_X;
        g_nMeshBlockZ = STORY_MESH_BLOCK_Z;
        g_nMaxMeshBlock = MAX_STORY_MESH_BLOCK;
        g_nMeshVertex = STORY_MESH_VERTEX;
        g_nMeshIndex = STORY_MESH_INDEX;
        g_nMeshPolygon = STORY_MESH_POLYGON;
    }

    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̎擾

    g_posMeshField = D3DXVECTOR3(-GRID_SIZE/2, 0.0f, -GRID_SIZE/2);
    g_rotMashField = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    VERTEX_3D *pVtx;//���_���̃|�C���^
    int nCntIndex = 0;
    int cylinder_v_count = 9;
    int cylinder_h_count = 8;
    //���_�o�b�t�@�̐���
    pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nMeshVertex,
        D3DUSAGE_WRITEONLY,
        FVF_VERTEX_3D,		//���_�t�H�[�}�b�g
        D3DPOOL_MANAGED,//�i�Œ�j
        &g_pVtxBuffMeshField,//�ϐ������ς��ƕύX���K�v
        NULL);


    HRESULT hr = D3DXCreateTextureFromFile(pDevice, MESHFILD_TEXTURE, &g_pVtxTextureMeshField);//�e�N�X�`���̓ǂݍ���

    //���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
    g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);//���̏����͕ς��Ȃ�����

    //���_���̏�����
    for (int nCntMeshZ = 0; nCntMeshZ <= g_nMeshBlockZ; nCntMeshZ++)
    {
        for (int nCntMeshX = 0; nCntMeshX <= g_nMeshBlockX; nCntMeshX++)
        {
            pVtx[nCntIndex].pos = D3DXVECTOR3(nCntMeshX * GRID_SIZE, 0.0f, nCntMeshZ * GRID_SIZE);
            pVtx[nCntIndex].tex = D3DXVECTOR2((float)nCntMeshX / 2.0f, (float)nCntMeshZ / 2.0f);
            nCntIndex++;

        }
    }

    //���_���̐ݒ�
    for (nCntIndex = 0; nCntIndex < g_nMeshVertex; nCntIndex++)
    {
        pVtx[nCntIndex].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//�@���x�N�g���̐ݒ�@
        pVtx[nCntIndex].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);//���_�J���[�̐ݒ�

    }


    //���_�f�[�^�A�����b�N
    g_pVtxBuffMeshField->Unlock();


    //�C���f�b�N�X�o�b�t�@(�|���S���o�b�t�@)�̐���
    pDevice->CreateIndexBuffer(sizeof(WORD) * g_nMeshIndex,
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX16,
        D3DPOOL_MANAGED,
        &g_pIdxBuffMeshField,
        NULL);

    WORD *pIdx = NULL;
    g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);//�C���f�b�N�X�o�b�t�@�����b�N
    nCntIndex = 0;//�J�E���g���Z�b�g
    for (int nCntIndexZ = 0; nCntIndexZ < g_nMeshBlockZ; nCntIndexZ++)
    {
        if (nCntIndex != 0)
        {
            pIdx[nCntIndex] = (g_nMeshBlockX + 1) * (nCntIndexZ + 1);
            nCntIndex++;
        }
        for (int nCntIndexX = 0; nCntIndexX <= g_nMeshBlockX; nCntIndexX++)
        {
            pIdx[nCntIndex] = nCntIndexX + (g_nMeshBlockX + 1)* (nCntIndexZ + 1);
            pIdx[nCntIndex + 1] = pIdx[nCntIndex] - (g_nMeshBlockX + 1);
            nCntIndex += 2;//�C���f�b�N�X�̃|�C���^��2�i�߂�
        }
        if (nCntIndex != g_nMeshIndex)
        {
            //Z���Ō�ȊO�̏ꍇ�A�k�ރ|���S���p�̃C���f�b�N�X��ݒ肷��
            pIdx[nCntIndex] = pIdx[nCntIndex - 1];
            nCntIndex++;
        }
    }

    g_pIdxBuffMeshField->Unlock();//�C���f�b�N�X�o�b�t�@�̃A�����b�N
}

//=============================================================================
// ���b�V���t�B�[���h�I������
//  Author:KUDO AYANO
//=============================================================================
void UninitMeshField(void)
{

    if (g_pVtxBuffMeshField != NULL)
    {
        g_pVtxBuffMeshField->Release();
        g_pVtxBuffMeshField = NULL;
    }
    //�C���f�b�N�X�o�b�t�@�̊J��
    if (g_pIdxBuffMeshField != NULL)
    {
        g_pIdxBuffMeshField->Release();
        g_pIdxBuffMeshField = NULL;
    }

    if (g_pVtxTextureMeshField != NULL)
    {
        g_pVtxTextureMeshField->Release();
        g_pVtxTextureMeshField = NULL;
    }
}

//=============================================================================
// ���b�V���t�B�[���h�`�揈��
// Author:KUDO AYANO
//=============================================================================
void DrawMeshField(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̃|�C���^���擾
    D3DXMATRIX mtxRot, mtxTrans;//�s��v�Z�p�̃}�g���N�X
                                //���[���h�}�g���N�X�̏�����
    D3DXMatrixIdentity(&g_mtxWorldMeshField);
    //�����𔽓]
    D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMashField.y, g_rotMashField.x, g_rotMashField.z);
    D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);
    //�ʒu�𔽓]
    D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);
    D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);
    //���[���h�}�g���b�N�X�̐ݒ�
    pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);
    //���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));
    //�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetIndices(g_pIdxBuffMeshField);
    //���_�t�H�[�}�b�g�̐ݒ�
    pDevice->SetFVF(FVF_VERTEX_3D);
    pDevice->SetTexture(0, g_pVtxTextureMeshField);
    //�|���S���̕`��
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nMeshVertex, 0, g_nMeshPolygon);

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}