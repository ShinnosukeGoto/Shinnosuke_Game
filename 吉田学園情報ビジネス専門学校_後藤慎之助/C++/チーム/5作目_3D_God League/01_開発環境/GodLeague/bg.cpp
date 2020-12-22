//===============================================
//
// 背景処理 (bg.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "bg.h"
#include "manager.h"
#include "renderer.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CBg::m_apTexture[CBg::TYPE_MAX] = {};

//========================================
// 背景のコンストラクタ
//========================================
CBg::CBg() :CScene2D(OBJTYPE::OBJTYPE_BG)
{
    m_col = DEFAULT_COLOR;
    m_type = CBg::TYPE_MAX;
}

//========================================
// 背景のデストラクタ
//========================================
CBg::~CBg()
{

}

//========================================
// 背景の初期化処理
//========================================
HRESULT CBg::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// 背景の終了処理
//========================================
void CBg::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// 背景の更新処理
//========================================
void CBg::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得

    // 色を反映
    SetColor(m_col);

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 頂点座標を設定
    SetVertex();
}

//========================================
// 背景の描画処理
//========================================
void CBg::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// 背景の画像ロード処理
//========================================
HRESULT CBg::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/title000.jpg", &m_apTexture[CBg::TYPE_TITLE_BG]);

    return S_OK;
}

//========================================
// 背景の画像破棄処理
//========================================
void CBg::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CBg::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// 背景の作成
//================================
CBg *CBg::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CBg::TYPE type)
{
    CBg *pBg = NULL;

    // メモリを確保
    pBg = new CBg;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pBg->m_type = type;

    // 初期化
    pBg->Init(pos, size);

    // メンバ変数を結びつける
    pBg->m_col = col;

    return pBg;
}