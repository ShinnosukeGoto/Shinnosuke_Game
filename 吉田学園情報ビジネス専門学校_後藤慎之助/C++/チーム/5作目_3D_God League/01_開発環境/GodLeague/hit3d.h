//=============================================================================
//
// ダメージ表示処理 [damage.h]
// Author : 樋宮 匠
//
//=============================================================================

// 2重インクルード防止用
#ifndef _HIT3D_H_
#define _HIT3D_H_
//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DAMAGE_BILL 2
#define MAX_HIT_TEX 2
#define NUM_SIZE 20

#define PLAYER_DAMAGE_POS D3DXVECTOR3(90.0f, 20.0f, 20.0f)
#define ENEMY_DAMAGE_POS D3DXVECTOR3(-90.0f, 20.0f, 20.0f)

//*****************************************************************************
// 背景クラス定義
//*****************************************************************************
class CHit3D :public CBillboard
{
public:
    CHit3D();
    ~CHit3D();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Update(void);
    void Draw(void);
    void Uninit(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CHit3D * Create(D3DXVECTOR3 pos, int value);
private:
    static LPDIRECT3DTEXTURE9 m_apTexture[MAX_HIT_TEX];
    int m_nValue;                                           // ダメージの値
    CBillboard *m_apNumBill[MAX_DAMAGE_BILL];
    D3DXVECTOR3 m_pos;
    D3DXCOLOR m_col;
    float	  m_fAlpha;
};
#endif
