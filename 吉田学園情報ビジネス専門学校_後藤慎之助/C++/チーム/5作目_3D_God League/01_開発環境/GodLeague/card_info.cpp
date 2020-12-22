//===============================================
//
// カード情報の処理 (card_info.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "card_info.h"
#include "number.h"
#include "card.h"
#include "game.h"

//========================================
// マクロ定義
//========================================
#define JUSTIFIED_NUMBER 20.0f  // 数字を詰める

//========================================
// 静的メンバ変数宣言
//========================================
CNumber *CCardInfo::m_apNumber[MAX_CARD_INFO_NUMBER] = {};

//========================================
// カード情報のコンストラクタ
//========================================
CCardInfo::CCardInfo() :CScene(OBJTYPE::OBJTYPE_CARD_INFO)
{

}

//========================================
// カード情報のデストラクタ
//========================================
CCardInfo::~CCardInfo()
{

}

//========================================
// カード情報の生成
//========================================
CCardInfo* CCardInfo::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CCardInfo *pCardInfo = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pCardInfo = new CCardInfo;

    // 初期化
    pCardInfo->Init(pos, size);

    return pCardInfo;
}

//========================================
// カード情報の初期化処理
//========================================
HRESULT CCardInfo::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 自分のカード情報
    // 大きい位から、それぞれCreate
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_CARD_INFO_DIGITS - 1) / MAX_CARD_INFO_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + CARD_INFO_ATTACK_POS, D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_CARD_INFO_DIGITS + 1) / MAX_CARD_INFO_DIGITS / 2, 0.0f, 0.0f) + CARD_INFO_ATTACK_POS,
        D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_CARD_INFO_DIGITS - 1) / MAX_CARD_INFO_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + CARD_INFO_DEFENSE_POS, D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[3] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_CARD_INFO_DIGITS + 1) / MAX_CARD_INFO_DIGITS / 2, 0.0f, 0.0f) + CARD_INFO_DEFENSE_POS,
        D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);

    // 敵のカード情報
    m_apNumber[4] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_CARD_INFO_DIGITS - 1) / MAX_CARD_INFO_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + CARD_INFO_ENEMY_POS, D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[5] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_CARD_INFO_DIGITS + 1) / MAX_CARD_INFO_DIGITS / 2, 0.0f, 0.0f) + CARD_INFO_ENEMY_POS,
        D3DXVECTOR3(size.x / MAX_CARD_INFO_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// カード情報の終了処理
//========================================
void CCardInfo::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_CARD_INFO_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            // 数字情報の終了処理
            m_apNumber[nCntNumber]->Uninit();

            // 数字情報のメモリの開放
            ReleaseNumber(nCntNumber);
        }
    }

    // オブジェクトの破棄
    Release();
}

//========================================
// カード情報クラスが持っている、数字の開放処理
//========================================
void CCardInfo::ReleaseNumber(int nCntNumber)
{
    // メモリの開放
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// カード情報の更新処理
//========================================
void CCardInfo::Update(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_CARD_INFO_NUMBER; nCntNumber++)
    {
        // 変数宣言
        bool bUse = true;   // 使用中

        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            bUse = m_apNumber[nCntNumber]->Update();

            // 未使用になったら
            if (bUse == false)
            {
                // 数字情報のメモリの開放
                ReleaseNumber(nCntNumber);
            }
        }
    }

    // ゲーム終了したなら透明に
    if (CGame::GetFinishBattle() == true)
    {
        for (int nCntNumber = 0; nCntNumber < MAX_CARD_INFO_NUMBER; nCntNumber++)
        {
            // 中身があるなら
            if (m_apNumber[nCntNumber] != NULL)
            {
                // 数字を全て透明に
                m_apNumber[nCntNumber]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
            }
        }
    }
}

//========================================
// カード情報の描画処理
//========================================
void CCardInfo::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_CARD_INFO_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// カード情報の設定
//========================================
void CCardInfo::SetCardInfo(int nAttack, int nDefence)
{
    // 変数宣言
    int nIndex;		        // 指数　(カード情報表示用)
    int nRadix = 10;	    // 基数　(カード情報表示用)

    // 攻撃
    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_CARD_INFO_DIGITS; nIndex++)
    {
        int nCardInfo = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex);
        int nCardInfo2 = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = nAttack % nCardInfo / nCardInfo2;

        // 番号を設定
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }

    // 防御
    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_CARD_INFO_DIGITS; nIndex++)
    {
        int nCardInfo = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex);
        int nCardInfo2 = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = nDefence % nCardInfo / nCardInfo2;

        // 番号を設定
        m_apNumber[nIndex + MAX_CARD_INFO_DIGITS]->SetNumber(nAnswer);
    }
}

//========================================
// 敵のカード情報の設定
//========================================
void CCardInfo::SetEnemyCardInfo(int nValue)
{
    // 変数宣言
    int nIndex;		        // 指数　(カード情報表示用)
    int nRadix = 10;	    // 基数　(カード情報表示用)

    for (nIndex = 0; nIndex < MAX_CARD_INFO_DIGITS; nIndex++)
    {
        // -1なら透明
        if (nValue == -1)
        {
            m_apNumber[nIndex + MAX_CARD_INFO_DIGITS + MAX_CARD_INFO_DIGITS]->SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,0.0f));
        }
        else
        {
            int nCardInfo = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex);
            int nCardInfo2 = (int)powf((float)nRadix, MAX_CARD_INFO_DIGITS - (float)nIndex - 1);

            // 
            int nAnswer = nValue % nCardInfo / nCardInfo2;

            m_apNumber[nIndex + MAX_CARD_INFO_DIGITS + MAX_CARD_INFO_DIGITS]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

            // 番号を設定
            m_apNumber[nIndex + MAX_CARD_INFO_DIGITS + MAX_CARD_INFO_DIGITS]->SetNumber(nAnswer);
        }
    }
}
