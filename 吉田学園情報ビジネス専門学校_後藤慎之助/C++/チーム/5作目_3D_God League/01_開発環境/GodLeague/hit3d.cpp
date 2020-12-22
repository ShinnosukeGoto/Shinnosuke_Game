#include "hit3d.h"
#include "manager.h"
#include "renderer.h"

LPDIRECT3DTEXTURE9 CHit3D::m_apTexture[MAX_HIT_TEX] = {};

CHit3D::CHit3D()
{
    m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
    m_fAlpha = 1.0f;

    m_nValue = 0;
}

CHit3D::~CHit3D()
{
}

HRESULT CHit3D::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    m_pos = pos;
    m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

    CBillboard::Init(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
    SetCol(m_col);
    BindTexture(m_apTexture[0]);

    for (int nCount = 0; nCount < MAX_DAMAGE_BILL; nCount++)
    {
        m_apNumBill[nCount] = new CBillboard;

        int nNum = (m_nValue % (int)pow(10.0f, (float)(MAX_DAMAGE_BILL - nCount))) / (int)pow(10.0f, (float)(MAX_DAMAGE_BILL - nCount - 1));

        m_apNumBill[nCount]->SetSize(NUM_SIZE, NUM_SIZE / 2);
        m_apNumBill[nCount]->SetTexUVX(nNum * 0.1f, nNum * 0.1f + 0.1f);
        m_apNumBill[nCount]->BindTexture(m_apTexture[1]);

        m_apNumBill[nCount]->Init(pos, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
        m_apNumBill[nCount]->SetCol(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
    }

    m_apNumBill[0]->SetPos(D3DXVECTOR3(m_pos.x + NUM_SIZE / 4, m_pos.y, m_pos.z));
    m_apNumBill[1]->SetPos(D3DXVECTOR3(m_pos.x - NUM_SIZE / 4, m_pos.y, m_pos.z));

    return S_OK;
}

void CHit3D::Update(void)
{
    m_fAlpha -= 0.0125f;
    m_pos.y += 0.5f;

    if (m_fAlpha <= 0.0f)
    {
        m_fAlpha = 0.0f;
        for (int nCount = 0; nCount < MAX_DAMAGE_BILL; nCount++)
        {
            m_apNumBill[nCount]->Uninit();
        }
        Uninit();
    }
    else
    {
        //SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
        SetPos(m_pos);
        m_apNumBill[0]->SetPos(D3DXVECTOR3(m_pos.x + NUM_SIZE / 4, m_pos.y, m_pos.z));
        m_apNumBill[1]->SetPos(D3DXVECTOR3(m_pos.x - NUM_SIZE / 4, m_pos.y, m_pos.z));

        for (int nCount = 0; nCount < MAX_DAMAGE_BILL; nCount++)
        {
            //m_apNumBill[nCount]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fAlpha));
        }
        CBillboard::Update();
    }
}

void CHit3D::Draw(void)
{
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    CBillboard::Draw();


}

void CHit3D::Uninit(void)
{
    CBillboard::Uninit();
}

HRESULT CHit3D::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/hitDamageUI000.png", &m_apTexture[0]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/number.png", &m_apTexture[1]);

    return S_OK;
}

void CHit3D::Unload(void)
{
    for (int nCount = 0; nCount < MAX_HIT_TEX; nCount++)
    {
        if (m_apTexture[nCount] != NULL)
        {
            m_apTexture[nCount]->Release();
            m_apTexture[nCount] = NULL;
        }
    }
}

CHit3D * CHit3D::Create(D3DXVECTOR3 pos, int value)
{
    CHit3D *pHit3D;
    pHit3D = new CHit3D;
    pHit3D->m_nValue = value;
    pHit3D->Init(pos, DEFAULT_VECTOR);
    return nullptr;
}
