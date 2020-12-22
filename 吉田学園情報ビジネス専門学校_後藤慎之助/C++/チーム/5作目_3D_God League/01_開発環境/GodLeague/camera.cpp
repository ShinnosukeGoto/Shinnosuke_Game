//===============================================
//
// �������� (light.cpp)
// Author : ��{��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "game.h"
#include "character.h"

//========================
// �ÓI�����o�ϐ��錾
//========================
D3DXVECTOR3 CCamera::m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CCamera::m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
CCamera::CAMERA_STATE CCamera::m_state = CCamera::CAMERA_OVER_LOOK;
float CCamera::m_fR = CAMERA_OFFSET;
float CCamera::m_f�� = 0;
float CCamera::m_f�� = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	D3DXMatrixIdentity(&m_mtxView);
	D3DXMatrixIdentity(&m_mtxProjection);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_state = CAMERA_OVER_LOOK;
	m_fR = CAMERA_OFFSET;
	m_f�� = 1;
	m_f�� = 1;
	m_nCountState = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CCamera::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_state = CAMERA_OVER_LOOK;
	m_fR = CAMERA_OFFSET;
	m_f�� = 1;
	m_f�� = 1;
	m_nCountState = 0;

	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 2000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CCamera::Uninit(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void CCamera::Update(void)
{
	//  �J�����ʒu�C������
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	switch (m_state)
	{
	case CAMERA_PLAYER_01:
		m_pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
		m_posRDest = m_pos;
		m_fR = CAMERA_OFFSET / 2;
		m_f�� += D3DXToRadian(0.5f);
		m_nCountState++;
		if (m_nCountState > CAMERA_STATE_LENGTH)
		{
			m_nCountState = 0;
			m_state = CAMERA_PLAYER_02;
		}
		break;
	case CAMERA_PLAYER_02:
		m_pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
		m_posRDest = m_pos;
		m_fR = CAMERA_OFFSET / 2;
		m_f�� -= D3DXToRadian(0.5f);
		m_nCountState++;
		if (m_nCountState > CAMERA_STATE_LENGTH)
		{
			m_nCountState = 0;
			m_state = CAMERA_OVER_LOOK;
		}
		break;
	case CAMERA_OVER_LOOK:
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_fR = CAMERA_OFFSET;
		m_f�� = D3DXToRadian(90);
		m_nCountState++;
		if (m_nCountState > CAMERA_STATE_LENGTH)
		{
			m_nCountState = 0;
			m_state = CAMERA_PLAYER_01;
		}
		break;
    case CAMERA_BATTLE:
        m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
        m_fR = CAMERA_OFFSET;
        m_f�� = D3DXToRadian(90);
        break;
	default:
		break;
	}

	m_posVDest.x = m_fR * (sin(m_f��) * cos(m_f��)) + m_pos.x;
	m_posVDest.y = (m_fR / 2.0f) * cos(m_f��) + m_pos.y;
	m_posVDest.z = m_fR * (sin(m_f��) * sin(m_f��)) + m_pos.z;

	m_posR += (m_posRDest - m_posR) * 0.1f;
	m_posV += (m_posVDest - m_posV) * 0.1f;

	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	D3DXMatrixPerspectiveFovLH(&m_mtxProjection, D3DXToRadian(90), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 10000.0f);
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}
//=============================================================================
// �C���X�^���X��������
//=============================================================================
CCamera * CCamera::Create(void)
{
	CCamera *pCamera = NULL;
	pCamera = new CCamera;
	pCamera->Init();
	return pCamera;
}

//=============================================================================
// �����_�ύX����
//=============================================================================
void CCamera::SetCameraLookAt(D3DXVECTOR3 pos)
{
	m_posRDest = D3DXVECTOR3(pos.x, pos.y, pos.z);
}

//=============================================================================
// �J�������Z�b�g����
//=============================================================================
void CCamera::ResetCamera(void)
{
	m_state = CAMERA_OVER_LOOK;
}
