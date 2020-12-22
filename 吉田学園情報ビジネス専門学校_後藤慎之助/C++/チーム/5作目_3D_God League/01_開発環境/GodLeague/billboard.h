//====================================================================
//
// �r���{�[�h���� (billboard.h)
// Author : ��{��
//
//====================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BILL_SIZE 50

//================================================
// �N���X�錾
//================================================

// �r���{�[�h�N���X
class CBillboard : public CScene
{
public:
    CBillboard();
    virtual ~CBillboard();
    virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    virtual void Uninit(void);
    virtual void Update(void);
    virtual void Draw(void);
    void BindTexture(const LPDIRECT3DTEXTURE9 pTex) { m_pTexture = pTex; }
    void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
    void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
    void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
    void SetSize(float height, float width) { m_fHeight = height; m_fWidth = width; }
    void SetCol(D3DXCOLOR col) { m_col = col; }
    void SetTexUVX(float x1, float x2) { m_fTexX1 = x1; m_fTexX2 = x2; }
    D3DXVECTOR3 GetPos(void) { return m_pos; }
private:
    LPDIRECT3DTEXTURE9		m_pTexture;
    D3DXMATRIX			    m_mtxWorld;		// ���[���h�}�g���b�N�X
    LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;		// ���_�o�b�t�@�[
    D3DXVECTOR3				m_pos;			// ���W
    D3DXVECTOR3				m_rot;			// ��]
    D3DXVECTOR3				m_scale;		// �g�嗦
    D3DXCOLOR				m_col;
    float					m_fHeight;		// ����
    float					m_fWidth;		// ��
    float					m_fTexX1;
    float					m_fTexX2;
};

#endif