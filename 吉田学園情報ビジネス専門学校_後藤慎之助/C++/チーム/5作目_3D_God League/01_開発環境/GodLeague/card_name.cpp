//===============================================
//
// CardName処理 (card_name.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "card_name.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "button.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CCardName::m_apTexture[CCard::TYPE_MAX + BUTTON_MAX] = {};

//========================================
// CardNameのコンストラクタ
//========================================
CCardName::CCardName() :CScene2D(OBJTYPE::OBJTYPE_CARD_NAME)
{
    m_nType = 0;
    m_bUse = true;
    m_useType = USETYPE_MAX;
}

//========================================
// CardNameのデストラクタ
//========================================
CCardName::~CCardName()
{

}

//========================================
// CardNameの初期化処理
//========================================
HRESULT CCardName::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_nType]);

    return S_OK;
}

//========================================
// CardNameの終了処理
//========================================
void CCardName::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// CardNameの更新処理
//========================================
void CCardName::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 頂点座標を設定
    SetVertex();

    // 使用フラグがfalse、またはゲーム終了したなら消す
    if (m_bUse == false || CGame::GetFinishBattle() == true)
    {
        Uninit();
    }
}

//========================================
// CardNameの描画処理
//========================================
void CCardName::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// CardNameの画像ロード処理
//========================================
HRESULT CCardName::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/000.png", &m_apTexture[CCard::TYPE_NONE]);

    // 攻撃カード
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_01_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_000]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_02_Hood.png", &m_apTexture[CCard::TYPE_ARMOR_001]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_03_Mail.png", &m_apTexture[CCard::TYPE_ARMOR_002]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_05_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_003]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_06_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_004]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_07_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_005]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_08_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_006]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_09_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_007]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/INFO/Guard_04_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_FINAL]);

    // 防御カード
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
// CardNameの画像破棄処理
//========================================
void CCardName::Unload(void)
{
    // テクスチャの破棄
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
// CardNameの作成
//================================
CCardName *CCardName::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int type, USETYPE useType)
{
    CCardName *pCardName = NULL;

    // メモリを確保
    pCardName = new CCardName;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pCardName->m_nType = type;

    // 初期化
    pCardName->Init(pos, size);
    pCardName->m_useType = useType;

    return pCardName;
}