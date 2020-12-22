//=============================================================================
//
// �u���b�N���� [block.h]
// Author : ��{ ��
//
//=============================================================================

// ��d�C���N���[�h�h�~�p
#ifndef _UI3D_H_
#define _UI3D_H_

//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "billboard.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NAME_HEIGHT 15
#define NAME_WIDTH  50
#define UI3D_MAX_TEX 2

//*****************************************************************************
// �u���b�N�N���X��`
//*****************************************************************************
class CUi3D :public CBillboard
{
public:
	CUi3D();
	~CUi3D();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Update(void);
	void Draw(void);
	void Uninit(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CUi3D * Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int player);
private:
	static LPDIRECT3DTEXTURE9 m_apTexture[UI3D_MAX_TEX];
};

#endif