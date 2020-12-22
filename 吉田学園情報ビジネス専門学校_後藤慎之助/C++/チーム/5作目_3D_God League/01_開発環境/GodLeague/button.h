//====================================================================
//
// BUTTON�̏��� (ui.h)
// Author : �㓡�T�V��
//
//====================================================================
#ifndef _BUTTON_H_
#define _BUTTON_H_

//================================================
// �C���N���[�h�t�@�C��
//================================================
#include "main.h"
#include "scene2d.h"
#include "card.h"

//================================================
// �}�N����`
//================================================

#define BUTTON_MAX 2

#define BUTTON_FORGIVE_POS (D3DXVECTOR3(SCREEN_WIDTH / 2 - 255.0f, SCREEN_HEIGHT / 2 + 150.0f, 0.0f))
#define BUTTON_PRAY_POS (D3DXVECTOR3(SCREEN_WIDTH / 2 + 255.0f, SCREEN_HEIGHT / 2 + 150.0f, 0.0f))
#define BUTTON_SIZE (D3DXVECTOR3(150.0f, 75.0f, 0.0f))
#define BUTTON_COLLISION_SIZE D3DXVECTOR3(120.0f, 55.0f, 0.0f)	// �{�^���̓����蔻��̃T�C�Y
#define BUTTON_EXTEND_SIZE D3DXVECTOR3(200.0f, 100.0f, 0.0f)	// �{�^���̊g��T�C�Y

#define FORGIVE_ID (CCard::TYPE_WEAPON_FINAL + (BUTTON_MAX - 1))  // ����ID
#define PRAY_ID (CCard::TYPE_WEAPON_FINAL + (BUTTON_MAX))         // �F��ID

//================================================
// �N���X�錾
//================================================

// �{�^���N���X
class CButton : public CScene2D
{
public:
    // BUTTON�̎��
    typedef enum
    {
        TYPE_FORGIVE = 0,	    // ����
        TYPE_PRAY,              // �F��
        TYPE_MAX			    // ��ނ̍ő吔
    }TYPE;
    CButton();
    ~CButton();
    HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 size);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static HRESULT Load(void);
    static void Unload(void);
    static CButton *Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, TYPE type);

    void GameFinishFade(void);                                      // �Q�[���I�����ɁA�����{�^��
    D3DXVECTOR3 CollisionCursor(D3DXVECTOR3 pos, D3DXVECTOR3 size); // �J�[�\���̓����蔻��
    void SetCardInfo(void);                                         // �J�[�h���̐ݒ�
    void Select(void);                                              // �I��

    void DisconnectInAnim(void);                    // �A�j���[�V�����p�[�g�ł́A�ؒf�Ή�

private:
    static LPDIRECT3DTEXTURE9 m_apTexture[TYPE_MAX];   // �ǂݍ��񂾃e�N�X�`����ۑ�����
    int m_nID;                                         // ID
    D3DXCOLOR m_col;                                   // �F
    TYPE m_type;                                       // �{�^���̎��
};

#endif