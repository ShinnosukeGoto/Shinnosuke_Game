//===============================================
//
// �V�ѕ����� (manual.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "manual.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CManual::m_pTexture = NULL;

//========================================
// �V�ѕ��̃R���X�g���N�^
//========================================
CManual::CManual() :CScene2D(OBJTYPE::OBJTYPE_MANUAL)
{
    m_bDisp = false;
}

//========================================
// �V�ѕ��̃f�X�g���N�^
//========================================
CManual::~CManual()
{

}

//========================================
// �V�ѕ��̏���������
//========================================
HRESULT CManual::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// �V�ѕ��̏I������
//========================================
void CManual::Uninit(void)
{
    // �I������
    CScene2D::Uninit();
}

//========================================
// �V�ѕ��̍X�V����
//========================================
void CManual::Update(void)
{
    // �L�[�{�[�h�̊m�ۂ������������擾
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

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

    // �V�ѕ��̕\���̃g�O��
    if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    {
        m_bDisp = !m_bDisp;
    }

    // �\�����邩�ǂ���
    if (m_bDisp == true)
    {
        SetColor(DEFAULT_COLOR);
    }
    else
    {
        SetColor(MANUAL_INACTIVE_COLOR);
    }

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
// �V�ѕ��̕`�揈��
//========================================
void CManual::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//========================================
// �V�ѕ��̉摜���[�h����
//========================================
HRESULT CManual::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/manual.png", &m_pTexture);

    return S_OK;
}

//========================================
// �V�ѕ��̉摜�j������
//========================================
void CManual::Unload(void)
{
    // �e�N�X�`���̔j��
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// �V�ѕ��̍쐬
//================================
CManual *CManual::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CManual *pManual = NULL;

    // ���������m��
    // �R���X�g���N�^�Ŋe����R�Â�
    pManual = new CManual;

    // ������
    pManual->Init(pos, size);

    return pManual;
}