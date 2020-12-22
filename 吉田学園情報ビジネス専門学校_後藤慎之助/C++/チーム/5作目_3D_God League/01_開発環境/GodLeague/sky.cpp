//===============================================
//
// �X�J�C�{�b�N�X���� (sky.cpp)
// Author : ��{��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "sky.h"
#include "manager.h"
#include "renderer.h"

//========================
// �ÓI�����o�ϐ��錾
//========================
LPD3DXMESH	CSky::m_pMesh = NULL;
LPD3DXBUFFER CSky::m_pBuffMat = NULL;
DWORD CSky::m_nNumMat = NULL;
LPDIRECT3DTEXTURE9 CSky::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CSky::CSky()
{
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSky::~CSky()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CSky::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
	CScene3D::Init(pos, size);

	SetScale(size);

	BindMesh(m_pMesh, m_pBuffMat, m_nNumMat);
	BindTexture(m_pTexture);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CSky::Uninit(void)
{
	CScene3D::Uninit();
}

//=============================================================================
// �X�V����
//=============================================================================
void CSky::Update(void)
{
	CScene3D::Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CSky::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	CScene3D::Draw();

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
// �e�N�X�`���E���f���f�[�^�ǂݍ��ݏ���
//=============================================================================
HRESULT CSky::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX(LPCSTR("./data/MODEL/Sky.x"), D3DXMESH_SYSTEMMEM, pDevice, NULL, &m_pBuffMat, NULL, &m_nNumMat, &m_pMesh);
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/audience.png", &m_pTexture);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���E���f���f�[�^�j������
//=============================================================================
void CSky::Unload(void)
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �C���X�^���X��������
//=============================================================================
CSky * CSky::Create(void)
{
	CSky *pSky = NULL;
	pSky = new CSky;
	pSky->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(60.0f, 60.0f, 60.0f));
	return pSky;
}
