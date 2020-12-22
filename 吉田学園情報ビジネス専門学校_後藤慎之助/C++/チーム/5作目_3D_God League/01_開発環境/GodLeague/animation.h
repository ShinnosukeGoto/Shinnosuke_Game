//====================================================================
//
// アニメーション処理 (animation.h)
// Author : 樋宮匠
//
//====================================================================
#ifndef _ANIMATION_H_
#define _ANIMATION_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"
#include "character.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define KEY_MAX 10

//================================================
// クラス宣言
//================================================

//アニメーションクラス
class CAnimation
{
public:
	CAnimation();
	~CAnimation();

	//列挙型宣言
	typedef enum
	{
		ANIM_IDLE = 0,
		ANIM_USE,
		ANIM_ATTACK,
		ANIM_DIFENCE,
		ANIM_PRAY,
        ANIM_PLAYER_WIN,
        ANIM_ENEMY_WIN,
        ANIM_LOSE,
		ANIM_MAX,
	}ANIMATION;

	// 構造体宣言
	typedef struct
	{
		D3DXVECTOR3 pos[KEY_MAX];
		D3DXVECTOR3 rot[KEY_MAX];
	}PARTS_INFO;

	typedef struct
	{
		PARTS_INFO  parts[CCharacter::PARTS_MAX];
		int			nFrame[KEY_MAX];
		int			nLoop;
		int			nKeyNum;
	}KEY_INFO;

	HRESULT Init(CCharacter *pCharacter);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CAnimation * Create(CCharacter *pCharacter);
	void LoadAnimation(void);
	void SetAnimation(ANIMATION anim) { m_animation = anim; }
private:
	KEY_INFO				m_keyInfo[ANIM_MAX];
	ANIMATION				m_animation;

	D3DXVECTOR3				m_pos[CCharacter::PARTS_MAX];			// 座標
	D3DXVECTOR3				m_posDest[CCharacter::PARTS_MAX];
	D3DXVECTOR3				m_rot[CCharacter::PARTS_MAX];			// 回転
	D3DXVECTOR3				m_rotDest[CCharacter::PARTS_MAX];
	D3DXVECTOR3				m_scale;
	ANIMATION				m_animationOld;
	CCharacter				*m_pCharacter;
	int						m_nFrame;
	int						m_nKey;
};

#endif