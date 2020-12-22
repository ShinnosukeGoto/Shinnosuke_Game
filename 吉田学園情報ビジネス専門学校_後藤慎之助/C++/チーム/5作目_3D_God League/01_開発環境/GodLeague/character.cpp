//===============================================
//
// �A�j���[�V�������� (animation.cpp)
// Author : ��{��
//
//===============================================
#define _CRT_SECURE_NO_WARNINGS

//========================
// �C���N���[�h�t�@�C��
//========================
#include "character.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "animation.h"

//========================================
// �ÓI�����o�ϐ��錾
//========================================
LPD3DXMESH CCharacter::m_pMesh[PARTS_MAX] = {};
LPD3DXBUFFER CCharacter::m_pBuffMat[PARTS_MAX] = {};
DWORD CCharacter::m_nNumMat[PARTS_MAX] = {};
int CCharacter::m_nIndexParent[PARTS_MAX] = {};
D3DXVECTOR3	CCharacter::m_posDefault[PARTS_MAX] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCharacter::CCharacter()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	memset(m_apModel, 0, sizeof(m_apModel));
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCharacter::~CCharacter()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCharacter::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	m_pos = pos;
	memset(m_apModel, 0, sizeof(m_apModel));

	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
	{
		m_apModel[nCount] = new CModel;
		m_apModel[nCount]->SetPos(pos);
		m_apModel[nCount]->SetRot(m_rot);
		m_apModel[nCount]->SetScale(size);
		m_apModel[nCount]->BindMesh(m_pMesh[nCount], m_pBuffMat[nCount], m_nNumMat[nCount]);
	}

	LoadModelData();

	m_pAnimation = CAnimation::Create(this);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCharacter::Uninit(void)
{
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CCharacter::Update(void)
{
	m_pAnimation->Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CCharacter::Draw(void)
{
	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
	{
		if (nCount == 0)
		{
			m_apModel[nCount]->Draw(m_pos, m_rot);
		}
		else
		{
			m_apModel[nCount]->Draw(m_apModel[m_nIndexParent[nCount]]);
		}
	}
}

//=============================================================================
// ���f���f�[�^�ǂݍ��ݏ���
//=============================================================================
HRESULT CCharacter::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/00_body.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_BODY], NULL, &m_nNumMat[PARTS_BODY], &m_pMesh[PARTS_BODY]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/01_head.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_HEAD], NULL, &m_nNumMat[PARTS_HEAD], &m_pMesh[PARTS_HEAD]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/02_armR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RARM], NULL, &m_nNumMat[PARTS_RARM], &m_pMesh[PARTS_RARM]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/03_handR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RHAND], NULL, &m_nNumMat[PARTS_RHAND], &m_pMesh[PARTS_RHAND]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/04_armL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LARM], NULL, &m_nNumMat[PARTS_LARM], &m_pMesh[PARTS_LARM]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/05_handL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LHAND], NULL, &m_nNumMat[PARTS_LHAND], &m_pMesh[PARTS_LHAND]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/06_legR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RLEG], NULL, &m_nNumMat[PARTS_RLEG], &m_pMesh[PARTS_RLEG]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/07_footR.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_RFOOT], NULL, &m_nNumMat[PARTS_RFOOT], &m_pMesh[PARTS_RFOOT]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/08_legL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LLEG], NULL, &m_nNumMat[PARTS_LLEG], &m_pMesh[PARTS_LLEG]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/09_footL.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_LFOOT], NULL, &m_nNumMat[PARTS_LFOOT], &m_pMesh[PARTS_LFOOT]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/10_weapon.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_WEAPON], NULL, &m_nNumMat[PARTS_WEAPON], &m_pMesh[PARTS_WEAPON]);

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/11_shield.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat[PARTS_SHIELD], NULL, &m_nNumMat[PARTS_SHIELD], &m_pMesh[PARTS_SHIELD]);

	return S_OK;
}

//=============================================================================
// ���f���f�[�^�i���W,�e�q�֌W�j�ǂݍ��ݏ���
//=============================================================================
HRESULT CCharacter::LoadModelData(void)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int  nCntLoad = 0;
	char cReedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[256];	//
	char cDie[128];
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �t�@�C���J
	pFile = fopen("./data/MODEL/motion_weapon.txt", "r");

	// �J����
	if (pFile != NULL)
	{
		while (strcmp(cHeadText, "CHARACTERSET") != 0)
		{
			fgets(cReedText, sizeof(cReedText), pFile);
			sscanf(cReedText, "%s", &cHeadText);
		}

		if (strcmp(cHeadText, "CHARACTERSET") == 0)
		{
			while (strcmp(cHeadText, "END_CHARACTERSET") != 0)
			{

				fgets(cReedText, sizeof(cReedText), pFile);
				sscanf(cReedText, "%s", &cHeadText);

				if (strcmp(cHeadText, "\n") == 0)
				{
				}
				else if (strcmp(cHeadText, "PARTSSET") == 0)
				{
					while (strcmp(cHeadText, "END_PARTSSET") != 0)
					{

						fgets(cReedText, sizeof(cReedText), pFile);
						sscanf(cReedText, "%s", &cHeadText);

						if (strcmp(cHeadText, "INDEX") == 0)
						{
							int nNum;
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &nNum);
						}

						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cReedText, "%s %s %d", &cDie, &cDie, &m_nIndexParent[nCntLoad]);
						}

						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &pos.x, &pos.y, &pos.z);

							m_apModel[nCntLoad]->SetPos(pos);
							m_posDefault[nCntLoad] = pos;
						}

						if (strcmp(cHeadText, "ROT") == 0)
						{
							//sscanf(cReedText, "%s %s %f %f %f", &cDie, &cDie, &rot.x, rot.y, &rot.z);

							m_apModel[nCntLoad]->SetRot(rot);
						}
					}
					nCntLoad++;
				}
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		printf("�J����܂���ł���\n");
	}
	return E_NOTIMPL;
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
void CCharacter::Unload(void)
{
	for (int nCount = 0; nCount < PARTS_MAX; nCount++)
	{
		if (m_pBuffMat[nCount] != NULL)
		{
			m_pBuffMat[nCount] = NULL;
		}

		if (m_nNumMat[nCount] != NULL)
		{
			m_nNumMat[nCount] = NULL;
		}

		if (m_pMesh[nCount] != NULL)
		{
			m_pMesh[nCount] = NULL;
		}
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CCharacter * CCharacter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CCharacter *pCharacter = NULL;
	pCharacter = new CCharacter;
	pCharacter->m_rot = rot;
	pCharacter->Init(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	return pCharacter;
}
