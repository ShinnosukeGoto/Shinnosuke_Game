//=============================================================================
//
// �_���[�W�\������ [damage.h]
// Author : ��{ ��
//
//=============================================================================

// 2�d�C���N���[�h�h�~�p
#ifndef _HIT3D_H_
#define _HIT3D_H_
//*****************************************************************************
// �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "billboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DAMAGE_BILL 2
#define MAX_HIT_TEX 2
#define NUM_SIZE 20

#define PLAYER_DAMAGE_POS D3DXVECTOR3(90.0f, 20.0f, 20.0f)
#define ENEMY_DAMAGE_POS D3DXVECTOR3(-90.0f, 20.0f, 20.0f)

//*****************************************************************************
// �w�i�N���X��`
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
    int m_nValue;                                           // �_���[�W�̒l
    CBillboard *m_apNumBill[MAX_DAMAGE_BILL];
    D3DXVECTOR3 m_pos;
    D3DXCOLOR m_col;
    float	  m_fAlpha;
};
#endif
