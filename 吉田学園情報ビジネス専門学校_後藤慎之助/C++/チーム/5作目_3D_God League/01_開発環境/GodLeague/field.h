//====================================================================
//
// フィールドの処理 (character.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene3d.h"

//================================================
// マクロ定義
//================================================
//================================================
// クラス宣言
//================================================

// フィールドクラス
class CField : public CScene3D
{
public:
	CField();
	~CField();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void Unload(void);
	static CField *Create(void);
private:
	static LPDIRECT3DTEXTURE9		m_pTexture;
	static LPD3DXMESH				m_pMesh;
	static LPD3DXBUFFER				m_pBuffMat;
	static DWORD					m_nNumMat;
};

#endif