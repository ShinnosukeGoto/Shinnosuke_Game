//====================================================================
//
// �V�ѕ��̏��� (manual.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _MANUAL_H_
#define _MANUAL_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene2d.h"

//================================================
// �O���錾(�s���S�ȃN���X�́Acpp�̕��Ńw�b�_���Ăяo��)
//================================================

//================================================
// �}�N����`
//================================================
#define MANUAL_POS D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f) // �V�ѕ��̈ʒu
#define MANUAL_SIZE (D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f))	  // �V�ѕ��̃T�C�Y
#define MANUAL_INACTIVE_COLOR D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f)           // �V�ѕ��̔�A�N�e�B�u�F

//================================================
// �N���X�錾
//================================================

// �V�ѕ��N���X
class CManual : public CScene2D
{
public:
    CManual();
    ~CManual();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CManual *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   // �ǂݍ��񂾃e�N�X�`����ۑ�����
    bool m_bDisp;                           // �\�����邩�ǂ���
};

#endif