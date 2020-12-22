//===============================================
//
// �J�[�\������ (cursor.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "cursor.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "sound.h"
#include "game.h"

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CCursor::m_pTexture = NULL;

//========================================
// �J�[�\���̃R���X�g���N�^
//========================================
CCursor::CCursor() :CScene2D(OBJTYPE::OBJTYPE_CURSOR)
{

}

//========================================
// �J�[�\���̃f�X�g���N�^
//========================================
CCursor::~CCursor()
{

}

//========================================
// �J�[�\���̏���������
//========================================
HRESULT CCursor::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// �J�[�\���̏I������
//========================================
void CCursor::Uninit(void)
{
    // �I������
    CScene2D::Uninit();
}

//========================================
// �J�[�\���̍X�V����
//========================================
void CCursor::Update(void)
{
    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // �ϐ��錾
    D3DXVECTOR3 pos;    // �ʒu
    D3DXVECTOR3 size;   // �T�C�Y

    // �ʒu���擾
    pos = GetPosition();

    // �T�C�Y���擾
    size = GetSize();

    // �J�[�\���̈ړ�����
    pos = ControlCursor(pos);

    // ��ʊO�ɏo�Ȃ��悤�ɂ���
    SteyInScreen2D(&pos, &size);

    // �ʒu�𔽉f
    SetPosition(pos);

    // ���_���W��ύX
    SetVertex();

    // �퓬�I�����A����
    if (CGame::GetFinishBattle() == true)
    {
        Uninit();
    }
}

//========================================
// �J�[�\���̕`�揈��
//========================================
void CCursor::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//========================================
// �J�[�\���̈ړ�����ׂ̍��Ȋ֐�
//========================================
D3DXVECTOR3 CCursor::ControlCursor(D3DXVECTOR3 pos)
{
    // �ϐ��錾
    D3DXVECTOR3 movePos = pos;      // �ړ��̈ʒu

    // �L�[�{�[�h���擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // �R���g���[�����擾
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    if (Controller.lY != 0 || Controller.lX != 0)
    {// �X�e�B�b�N���|��Ă���Ȃ�ړ�

     // �X�e�B�b�N�̊p�x�����߂�
        float fAngle = atan2(Controller.lX, Controller.lY * (-1));

        movePos.x += sinf(fAngle)* CURSOR_SPEED;
        movePos.y += -cosf(fAngle)* CURSOR_SPEED;
    }
    else
    {// �L�[�{�[�h����

     // A�L�[�������ꂽ(���ړ�)
        if (pInputKeyboard->GetKeyboardPress(DIK_A))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))// A��W�������ɉ����ꂽ
            {
                movePos.x -= sinf(D3DX_PI / 4) * CURSOR_SPEED;
                movePos.y -= cosf(D3DX_PI / 4) * CURSOR_SPEED;
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))// A��S�������ɉ����ꂽ
            {
                movePos.x -= sinf(D3DX_PI / 4) * CURSOR_SPEED;
                movePos.y += cosf(D3DX_PI / 4) * CURSOR_SPEED;
            }
            else				// ������������
            {
                movePos.x -= CURSOR_SPEED;
            }

        }

        // D�L�[�������ꂽ(�E�ړ�)
        else if (pInputKeyboard->GetKeyboardPress(DIK_D))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))// D��W�������ɉ����ꂽ
            {
                movePos.x += sinf(D3DX_PI / 4) * CURSOR_SPEED;
                movePos.y -= cosf(D3DX_PI / 4) * CURSOR_SPEED;
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))// D��S�������ɉ����ꂽ
            {
                movePos.x += sinf(D3DX_PI / 4) * CURSOR_SPEED;
                movePos.y += cosf(D3DX_PI / 4) * CURSOR_SPEED;
            }
            else				// �E����������
            {
                movePos.x += CURSOR_SPEED;
            }
        }

        // W�L�[�������ꂽ(��ړ�)
        else if (pInputKeyboard->GetKeyboardPress(DIK_W))
        {
            movePos.y -= CURSOR_SPEED;
        }

        // S�L�[�������ꂽ(���ړ�)
        else if (pInputKeyboard->GetKeyboardPress(DIK_S))
        {
            movePos.y += CURSOR_SPEED;
        }
    }

    return movePos;
}

//========================================
// �J�[�\���̉摜���[�h����
//========================================
HRESULT CCursor::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor000.png", &m_pTexture);

    return S_OK;
}

//========================================
// �J�[�\���̉摜�j������
//========================================
void CCursor::Unload(void)
{
    // �e�N�X�`���̔j��
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// �J�[�\���̍쐬
//================================
CCursor *CCursor::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CCursor *pCursor = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pCursor = new CCursor;

    // ������
    pCursor->Init(pos, size);

    return pCursor;
}