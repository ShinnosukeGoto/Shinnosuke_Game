//===============================================
//
// 光源処理 (light.cpp)
// Author : 樋宮匠
//
//===============================================

//========================
// インクルードファイル
//========================
#include "light.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CLight::CLight()
{
    ZeroMemory(&m_Light, sizeof(D3DLIGHT9));
}

//=============================================================================
// デストラクタ
//=============================================================================
CLight::~CLight()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CLight::Init(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    m_Light.Type = D3DLIGHT_DIRECTIONAL;
    m_Light.Diffuse.r = 1.0f;
    m_Light.Diffuse.g = 1.0f;
    m_Light.Diffuse.b = 1.0f;
    m_Light.Diffuse.a = 1.0f;
    m_Light.Direction = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
    m_Light.Range = 1000.0f;

    pDevice->SetLight(0, &m_Light);
    pDevice->LightEnable(0, TRUE);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CLight::Uninit(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void CLight::Update(void)
{
}

//=============================================================================
// インスタンス生成処理
//=============================================================================
CLight * CLight::Create(void)
{
    CLight *pLight = NULL;
    pLight = new CLight;
    pLight->Init();
    return pLight;
}
