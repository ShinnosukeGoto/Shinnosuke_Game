//=============================================================================
//
// �J�������� [camera.h]
// Author : ��{��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include "main.h"

//================================================
// �}�N����`
//================================================
#define CAMERA_OFFSET 200
#define CAMERA_STATE_LENGTH 180

//================================================
// ���C�g�N���X�錾
//================================================
class CCamera
{
public:
	CCamera();
	~CCamera();

	typedef enum
	{
		CAMERA_PLAYER_01 = 0,
		CAMERA_PLAYER_02,
		CAMERA_OVER_LOOK,
        CAMERA_BATTLE,
	}CAMERA_STATE;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	CCamera *Create(void);
	static void SetCameraLookAt(D3DXVECTOR3 pos);
	static void SetCameraState(CAMERA_STATE state) { m_state = state; }
	static void ResetCamera(void);
private:
	static D3DXVECTOR3	m_posRDest;
	static D3DXVECTOR3	m_posVDest;
	static CAMERA_STATE m_state;
	D3DXVECTOR3			m_posV;
	D3DXVECTOR3			m_posR;
	D3DXVECTOR3			m_vecU;
	D3DXVECTOR3			m_pos;
	D3DXMATRIX			m_mtxProjection;
	D3DXMATRIX			m_mtxView;
	static float		m_fR;
	static float		m_f��;
	static float		m_f��;
	int					m_nCountState;
};

#endif