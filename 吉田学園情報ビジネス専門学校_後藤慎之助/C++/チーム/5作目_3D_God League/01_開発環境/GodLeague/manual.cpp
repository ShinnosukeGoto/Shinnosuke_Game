//===============================================
//
// 遊び方処理 (manual.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "manual.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "sound.h"
#include "game.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CManual::m_pTexture = NULL;

//========================================
// 遊び方のコンストラクタ
//========================================
CManual::CManual() :CScene2D(OBJTYPE::OBJTYPE_MANUAL)
{
    m_bDisp = false;
}

//========================================
// 遊び方のデストラクタ
//========================================
CManual::~CManual()
{

}

//========================================
// 遊び方の初期化処理
//========================================
HRESULT CManual::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// 遊び方の終了処理
//========================================
void CManual::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 遊び方の更新処理
//========================================
void CManual::Update(void)
{
    // キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 変数宣言
    D3DXVECTOR3 pos;    // 位置
    D3DXVECTOR3 size;   // サイズ

    // 位置を取得
    pos = GetPosition();

    // サイズを取得
    size = GetSize();

    // 遊び方の表示のトグル
    if (pInputKeyboard->GetKeyboardTrigger(DIK_P) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
    {
        m_bDisp = !m_bDisp;
    }

    // 表示するかどうか
    if (m_bDisp == true)
    {
        SetColor(DEFAULT_COLOR);
    }
    else
    {
        SetColor(MANUAL_INACTIVE_COLOR);
    }

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    SetVertex();

    // 戦闘終了時、消す
    if (CGame::GetFinishBattle() == true)
    {
        Uninit();
    }
}

//========================================
// 遊び方の描画処理
//========================================
void CManual::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// 遊び方の画像ロード処理
//========================================
HRESULT CManual::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/manual.png", &m_pTexture);

    return S_OK;
}

//========================================
// 遊び方の画像破棄処理
//========================================
void CManual::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// 遊び方の作成
//================================
CManual *CManual::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CManual *pManual = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pManual = new CManual;

    // 初期化
    pManual->Init(pos, size);

    return pManual;
}