//====================================================================
//
// �J�[�\���̏��� (cursor.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _CURSOR_H_
#define _CURSOR_H_

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
#define CURSOR_POS D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f) // �J�[�\���̈ʒu
#define CURSOR_SIZE D3DXVECTOR3(50.0f, 50.0f, 0.0f)		                  // �J�[�\���̃T�C�Y
#define CURSOR_SPEED 10.0f                                                // �J�[�\���̑���

//================================================
// �N���X�錾
//================================================

// �J�[�\���N���X
class CCursor : public CScene2D
{
public:
    CCursor();
    ~CCursor();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    D3DXVECTOR3 ControlCursor(D3DXVECTOR3 pos);
    static HRESULT Load(void);
    static void Unload(void);
    static CCursor *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size);

private:
    static LPDIRECT3DTEXTURE9 m_pTexture;   // �ǂݍ��񂾃e�N�X�`����ۑ�����
};

#endif