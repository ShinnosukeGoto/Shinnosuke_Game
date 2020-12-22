//====================================================================
//
// �J�[�h��E�̏��� (card_name.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _CARD_NAME_H_
#define _CARD_NAME_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene2d.h"
#include "card.h"
#include "button.h"

//================================================
// �}�N����`
//================================================

// �J�[�h��
#define CARD_NAME_POS (D3DXVECTOR3(160.0f, 262.5f, 0.0f))
#define CARD_NAME_SIZE (D3DXVECTOR3(220.0f, 72.5f, 0.0f))

#define CARD_NAME_ENEMY_POS (D3DXVECTOR3(SCREEN_WIDTH - 139.5f, 335.5f, 0.0f))

//================================================
// �N���X�錾
//================================================

// CARD_NAME�N���X
class CCardName : public CScene2D
{
public:

    // �v���C���[���G��
    typedef enum
    {
        USETYPE_PLAYER = 0,          // �v���C���[
        USETYPE_ENEMY,               // �G
        USETYPE_MAX
    }USETYPE;

    CCardName();
    ~CCardName();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CCardName *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int type, USETYPE useType);

    int GetType(void) { return m_nType; }          // �^�C�v�̎擾
    USETYPE GetUseType(void) { return m_useType; } // �g�����l�̎�ނ��擾
    void SetDontUse(void) { m_bUse = false; }      // �g��Ȃ����̂̔j��

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[CCard::TYPE_MAX + BUTTON_MAX];   // �ǂݍ��񂾃e�N�X�`����ۑ�����
    USETYPE m_useType;                                                     // �g�����l�̎��
    int m_nType;                                                           // CARD_NAME�̎��
    bool m_bUse;                                                           // �g�p���邩�ǂ���
};

#endif