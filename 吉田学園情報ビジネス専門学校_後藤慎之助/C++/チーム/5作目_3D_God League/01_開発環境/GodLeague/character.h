//====================================================================
//
// キャラクターの処理 (character.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "scene.h"
#include "model.h"

//================================================
// クラス宣言
//================================================
class CAnimation;

// キャラクタークラス
class CCharacter : public CScene
{
public:
	CCharacter();
	~CCharacter();

	// 列挙型宣言（親→子→孫の順で宣言）
	typedef enum
	{
		PARTS_BODY = 0,
		PARTS_HEAD,
		PARTS_RARM,
		PARTS_RHAND,
		PARTS_LARM,
		PARTS_LHAND,
		PARTS_RLEG,
		PARTS_RFOOT,
		PARTS_LLEG,
		PARTS_LFOOT,
		PARTS_WEAPON,
		PARTS_SHIELD,
		PARTS_MAX,
	}PARTS;

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	HRESULT LoadModelData(void);
	static void Unload(void);
	static CCharacter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static D3DXVECTOR3 GetDefaultPos(int index) { return m_posDefault[index]; }
	D3DXVECTOR3 GetPartsRot(int index) { return m_apModel[index]->GetRot(); }
	CModel * GetParts(int index) { return m_apModel[index]; }
	void SetPos(int index, D3DXVECTOR3 pos) { m_apModel[index]->SetPos(pos); }
	void SetRot(int index, D3DXVECTOR3 rot) { m_apModel[index]->SetRot(rot); }
	D3DXVECTOR3 GetPos(void) { return m_pos; }

	CAnimation * GetAnimation(void) { return m_pAnimation; }

private:
	static LPD3DXMESH		m_pMesh[PARTS_MAX];
	static LPD3DXBUFFER		m_pBuffMat[PARTS_MAX];
	static DWORD			m_nNumMat[PARTS_MAX];
	static D3DXVECTOR3		m_posDefault[PARTS_MAX];
	static 	int				m_nIndexParent[PARTS_MAX];
	D3DXMATRIX		m_mtxWorld;
	D3DXVECTOR3		m_pos;
	D3DXVECTOR3		m_rot;

	CModel			*m_apModel[PARTS_MAX];
	CAnimation		*m_pAnimation;
};

#endif