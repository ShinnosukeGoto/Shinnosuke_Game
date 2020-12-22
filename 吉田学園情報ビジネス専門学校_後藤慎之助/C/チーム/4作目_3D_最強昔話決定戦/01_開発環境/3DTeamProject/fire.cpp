//=============================================================================
//
// �ŋ��̘b�����̏��� [fire.cpp](��)
// Author:�㓡�T�V��
//
//=============================================================================
#include "fire.h"
#include "collision.h"
#include "stage.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define XFILE_NEME "MODEL/fire.x"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPD3DXMESH g_pMeshFire = NULL;                     //���b�V�����ւ̃|�C���^
DWORD g_nNumMatFire = 0;                           //�}�e���A�����̐�
LPD3DXBUFFER g_pBuffMatFire = NULL;                //�}�e���A�����̃|�C���^
LPDIRECT3DTEXTURE9 *g_pTexturehFire = NULL;        //�|���S���̃e�N�X�`���ւ̃|�C���^

FIRE g_aFire[MAX_FIRE]; //���̏��

//=============================================================================
// ������������
// Author:�㓡�T�V��
//=============================================================================
void InitFire(void)
{
    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̓ǂݍ���

    //X�t�@�C���̓ǂݍ���
    HRESULT result = D3DXLoadMeshFromX
    (
        XFILE_NEME,         //x�t�@�C���̃p�X
        D3DXMESH_SYSTEMMEM, //���b�V���쐬�p�̃I�v�V�������w��
        pDevice,            //�f�o�C�X
        NULL,               //(���g�p)
        &g_pBuffMatFire,   //�}�e���A�����
        NULL,               //(���g�p)
        &g_nNumMatFire,    //X�t�@�C���Ɏg�p����Ă���}�e���A���̐�
        &g_pMeshFire       //x�t�@�C���̃��b�V���f�[�^
    );

    // �}�e���A���ƃe�N�X�`���̐ݒ�
    D3DXMATERIAL    *materials = (D3DXMATERIAL*)g_pBuffMatFire->GetBufferPointer();
    g_pTexturehFire = new LPDIRECT3DTEXTURE9[g_nNumMatFire];

    // �e�N�X�`���̓ǂݏo��
    for (int nCntFire = 0; nCntFire < (int)g_nNumMatFire; nCntFire++)
    {
        g_pTexturehFire[nCntFire] = NULL;
        if (materials[nCntFire].pTextureFilename != NULL)
        {
            // �e�N�X�`���̓ǂݍ���
            HRESULT hr = D3DXCreateTextureFromFile(pDevice, materials[nCntFire].pTextureFilename, &g_pTexturehFire[nCntFire]);
            int i = 0;
            i++;
        }
    }
    for (int nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        g_aFire[nCntFire].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aFire[nCntFire].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        g_aFire[nCntFire].size = D3DXVECTOR3(GRID_SIZE, GRID_SIZE, GRID_SIZE);
        g_aFire[nCntFire].nCntTime = TIME_FIRE;
        g_aFire[nCntFire].bUse = false;
    }
}

//=============================================================================
// ���I������
// Author:�㓡�T�V��
//=============================================================================
void UninitFire(void)
{
    //���b�V���̊J��
    if (g_pMeshFire != NULL)
    {
        g_pMeshFire->Release();
        g_pMeshFire = NULL;
    }
    //�}�e���A���̔j��
    if (g_pBuffMatFire != NULL)
    {
        g_pBuffMatFire->Release();
        g_pBuffMatFire = NULL;
    }
    if (g_pTexturehFire != NULL)
    {
        delete g_pTexturehFire;
        g_pTexturehFire = NULL;
    }
}

//=============================================================================
// ���X�V����
// Author:�㓡�T�V��
//=============================================================================
void UpdateFire(void)
{
    for (int nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        //�g�p���Ă���Ȃ�
        if (g_aFire[nCntFire].bUse == true)
        {
            //����������܂Ő�����
            g_aFire[nCntFire].nCntTime--;

            //���̌��h���̂��߁A�����ŉ�]
            g_aFire[nCntFire].rot += D3DXVECTOR3(1.0f, 1.0f, 1.0f);

            //����
            if (g_aFire[nCntFire].nCntTime < 0)
            {
                //���g�p��
                g_aFire[nCntFire].bUse = false;

                //������
                g_aFire[nCntFire].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            }
        }
    }
}
//=============================================================================
// ���`�揈��
// Author:�㓡�T�V��
//=============================================================================
void DrawFire(void)
{

    LPDIRECT3DDEVICE9 pDevice = GetDevice();//�f�o�C�X�̓ǂݍ���
    D3DXMATRIX mtxRot, mtxTrans, mtxscale;
    D3DMATERIAL9 matDef;//���݂̃}�e���A���ێ��p
    D3DXMATERIAL *pMat;//�}�e���A���f�[�^�ւ̃|�C���^

    for (int nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        //���[���h�}�g���b�N�X�̏�����
        D3DXMatrixIdentity(&g_aFire[nCntFire].mtxWorld);
        if (g_aFire[nCntFire].bUse)
        {
            //�g�k�𔽉f
            D3DXMatrixScaling(&mtxscale, g_aFire[nCntFire].size.x / g_aFire[nCntFire].size.x, 
                g_aFire[nCntFire].size.y / g_aFire[nCntFire].size.y, g_aFire[nCntFire].size.z / g_aFire[nCntFire].size.z);
            D3DXMatrixMultiply(&g_aFire[nCntFire].mtxWorld, &g_aFire[nCntFire].mtxWorld, &mtxscale);
            //�����𔽉f
            D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aFire[nCntFire].rot.y, g_aFire[nCntFire].rot.x, g_aFire[nCntFire].rot.z);
            D3DXMatrixMultiply(&g_aFire[nCntFire].mtxWorld, &g_aFire[nCntFire].mtxWorld, &mtxRot);
            //�ʒu�𔽉f
            D3DXMatrixTranslation(&mtxTrans, g_aFire[nCntFire].pos.x, g_aFire[nCntFire].pos.y, g_aFire[nCntFire].pos.z);
            D3DXMatrixMultiply(&g_aFire[nCntFire].mtxWorld, &g_aFire[nCntFire].mtxWorld, &mtxTrans);

            //���[���h�}�g���b�N�X�̐ݒ�
            pDevice->SetTransform(D3DTS_WORLD, &g_aFire[nCntFire].mtxWorld);

            //���݂̃}�e���A�����擾����
            pDevice->GetMaterial(&matDef);

            //�}�e���A���f�[�^�̃|�C���^���擾
            pMat = (D3DXMATERIAL*)g_pBuffMatFire->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)g_nNumMatFire; nCntMat++)
            {
                //�}�e���A���̐ݒ�
                pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
                pDevice->SetTexture(0, g_pTexturehFire[nCntMat]);
                //���f���p�[�c�̕`��
                g_pMeshFire->DrawSubset(nCntMat);
            }

            //�ێ����Ă����}�e���A����߂�
            pDevice->SetMaterial(&matDef);
        }
    }

    //�e�N�X�`���̐ݒ�̉���
    pDevice->SetTexture(0, NULL);
}

//=============================================================================
// �����̑��M
// Author:�㓡�T�V��
//=============================================================================
FIRE *GetFire(void)
{
    return &g_aFire[0];
}

//=====================================================
// ���̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitFire(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntFire;					// ���[�J���ϐ�
    bool bHit = false;

    for (nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        // �����g��ꂽ��
        if (g_aFire[nCntFire].bUse == true)
        {
            //�����蔻����v�Z
            if (RectangleCollision(pPos, pPosOld, size, &g_aFire[nCntFire].pos, g_aFire[nCntFire].size) == true)
            {
                bHit = true;    //�������Ă���
            }
        }
    }

    return bHit;
}

//=====================================================
// ���̐ݒ�
// Author:�㓡�T�V��
//=====================================================
void SetFire(D3DXVECTOR3 pos, SELECT type)
{

    int nCntFire;				// ���[�J���ϐ�

    for (nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        // �����g���ĂȂ��Ƃ�
        if (g_aFire[nCntFire].bUse == false)
        {
            // �����g�p���Ă����Ԃɂ���
            g_aFire[nCntFire].pos = pos;
            g_aFire[nCntFire].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
            g_aFire[nCntFire].nCntTime = TIME_FIRE;  //���̎������Ԃ͌Œ�
            g_aFire[nCntFire].size = D3DXVECTOR3(GRID_SIZE - 10.0f, GRID_SIZE, GRID_SIZE - 10.0f);
            g_aFire[nCntFire].type = type;
            g_aFire[nCntFire].bUse = true;
            break;
        }
    }
}

//=====================================================
// �����Y�̉��̓����蔻��̐ݒ�
// Author:�㓡�T�V��
//=====================================================
bool HitMomoFire(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3 size)
{
    int nCntFire;					// ���[�J���ϐ�
    bool bHit = false;

    for (nCntFire = 0; nCntFire < MAX_FIRE; nCntFire++)
    {
        // �����g��ꂽ��
        if (g_aFire[nCntFire].bUse == true)
        {
            //�����Y�̉��Ȃ�
            if (g_aFire[nCntFire].type == SELECT_MOMO)
            {
                //�����蔻����v�Z
                if (RectangleCollision(pPos, pPosOld, size, &g_aFire[nCntFire].pos, g_aFire[nCntFire].size) == true)
                {
                    bHit = true;    //�������Ă���
                }
            }
        }
    }

    return bHit;
}