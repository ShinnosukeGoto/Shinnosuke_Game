//===============================================
//
// CardName���� (card_name.cpp)
// Author : �㓡�T�V��
//
//===============================================

//========================
// �C���N���[�h�t�@�C��
//========================
#include "card_name.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "button.h"

//=======================================
// �ÓI�����o�ϐ��錾
//=======================================
LPDIRECT3DTEXTURE9 CCardName::m_apTexture[CCard::TYPE_MAX + BUTTON_MAX] = {};

//========================================
// CardName�̃R���X�g���N�^
//========================================
CCardName::CCardName() :CScene2D(OBJTYPE::OBJTYPE_CARD_NAME)
{
    m_nType = 0;
    m_bUse = true;
    m_useType = USETYPE_MAX;
}

//========================================
// CardName�̃f�X�g���N�^
//========================================
CCardName::~CCardName()
{

}

//========================================
// CardName�̏���������
//========================================
HRESULT CCardName::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // ������
    CScene2D::Init(pos, size);

    // �e�N�X�`�������蓖��
    BindTexture(m_apTexture[m_nType]);

    return S_OK;
}

//========================================
// CardName�̏I������
//========================================
void CCardName::Uninit(void)
{
    // �I������
    CScene2D::Uninit();
}

//========================================
// CardName�̍X�V����
//========================================
void CCardName::Update(void)
{
    // �ϐ��錾
    D3DXVECTOR3 pos = GetPosition();                   // �ʒu���擾
    D3DXVECTOR3 size = GetSize();                      // �T�C�Y���擾

    // �ʒu�𔽉f
    SetPosition(pos);

    // �T�C�Y�𔽉f
    SetSize(size);

    // ���_���W��ݒ�
    SetVertex();

    // �g�p�t���O��false�A�܂��̓Q�[���I�������Ȃ����
    if (m_bUse == false || CGame::GetFinishBattle() == true)
    {
        Uninit();
    }
}

//========================================
// CardName�̕`�揈��
//========================================
void CCardName::Draw(void)
{
    // �`�揈��
    CScene2D::Draw();
}

//========================================
// CardName�̉摜���[�h����
//========================================
HRESULT CCardName::Load(void)
{
    // �����_���[����f�o�C�X�̎擾
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // �e�N�X�`���̓ǂݍ���
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/000.png", &m_apTexture[CCard::TYPE_NONE]);

    // �U���J�[�h
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_01_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_000]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_02_Hood.png", &m_apTexture[CCard::TYPE_ARMOR_001]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_03_Mail.png", &m_apTexture[CCard::TYPE_ARMOR_002]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_05_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_003]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_06_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_004]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_07_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_005]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_08_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_006]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_09_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_007]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_04_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_FINAL]);

    // �h��J�[�h
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_01_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_000]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_02_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_001]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_03_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_002]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_05_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_003]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_06_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_004]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_07_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_005]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_08_club.png", &m_apTexture[CCard::TYPE_WEAPON_006]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_09_axe.png", &m_apTexture[CCard::TYPE_WEAPON_007]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_10_whip.png", &m_apTexture[CCard::TYPE_WEAPON_008]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_11_boomerang.png", &m_apTexture[CCard::TYPE_WEAPON_009]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_12_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_010]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Attack_04_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_FINAL]);

    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/forgive.png", &m_apTexture[FORGIVE_ID]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/pray.png", &m_apTexture[PRAY_ID]);

    return S_OK;
}

//========================================
// CardName�̉摜�j������
//========================================
void CCardName::Unload(void)
{
    // �e�N�X�`���̔j��
    for (int nCntTexture = 0; nCntTexture < CCard::TYPE_MAX + BUTTON_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// CardName�̍쐬
//================================
CCardName *CCardName::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int type, USETYPE useType)
{
    CCardName *pCardName = NULL;

    // ���������m��
    pCardName = new CCardName;

    // �^�C�v�̂݁A�e�N�X�`�������蓖�Ă̂��߂Ɍ��т��Ă���
    pCardName->m_nType = type;

    // ������
    pCardName->Init(pos, size);
    pCardName->m_useType = useType;

    return pCardName;
}