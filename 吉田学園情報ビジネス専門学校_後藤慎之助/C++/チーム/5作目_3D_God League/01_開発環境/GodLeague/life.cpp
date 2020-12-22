//===============================================
//
// ライフの処理 (life.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "life.h"
#include "number.h"
#include "card.h"
#include "game.h"
#include "card_name.h"
#include "hit3d.h"
#include "sound.h"
#include "manager.h"

//========================================
// マクロ定義
//========================================
#define JUSTIFIED_NUMBER 30.0f  // 数字を詰める

//========================================
// 静的メンバ変数宣言
//========================================
CNumber *CLife::m_apNumber[MAX_LIFE_NUMBER] = {};

//========================================
// ライフのコンストラクタ
//========================================
CLife::CLife() :CScene(OBJTYPE::OBJTYPE_LIFE)
{
    m_nPlayerLife = FIRST_LIFE;
    m_nEnemyLife = FIRST_LIFE;
}

//========================================
// ライフのデストラクタ
//========================================
CLife::~CLife()
{

}

//========================================
// ライフの生成
//========================================
CLife* CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CLife *pLife = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pLife = new CLife;

    // 初期化
    pLife->Init(pos, size);

    return pLife;
}

//========================================
// ライフの初期化処理
//========================================
HRESULT CLife::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 左上のライフ
    // 大きい位から、それぞれCreate
    m_apNumber[0] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_LIFE_DIGITS - 1) / MAX_LIFE_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + LIFE_PLAYER_POS, D3DXVECTOR3(size.x / MAX_LIFE_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[1] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_LIFE_DIGITS + 1) / MAX_LIFE_DIGITS / 2, 0.0f, 0.0f) + LIFE_PLAYER_POS,
        D3DXVECTOR3(size.x / MAX_LIFE_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[2] = CNumber::Create(D3DXVECTOR3(-size.x * (MAX_LIFE_DIGITS - 1) / MAX_LIFE_DIGITS / 2 + JUSTIFIED_NUMBER, 0.0f, 0.0f)
        + LIFE_ENEMY_POS, D3DXVECTOR3(size.x / MAX_LIFE_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);
    m_apNumber[3] = CNumber::Create(D3DXVECTOR3(size.x * (MAX_LIFE_DIGITS + 1) / MAX_LIFE_DIGITS / 2, 0.0f, 0.0f) + LIFE_ENEMY_POS,
        D3DXVECTOR3(size.x / MAX_LIFE_DIGITS, size.y, 0.0f), DEFAULT_COLOR, false);

    return S_OK;
}

//========================================
// ライフの終了処理
//========================================
void CLife::Uninit(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_LIFE_NUMBER; nCntNumber++)
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
// ライフクラスが持っている、数字の開放処理
//========================================
void CLife::ReleaseNumber(int nCntNumber)
{
    // メモリの開放
    delete m_apNumber[nCntNumber];
    m_apNumber[nCntNumber] = NULL;
}

//========================================
// ライフの更新処理
//========================================
void CLife::Update(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_LIFE_NUMBER; nCntNumber++)
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
}

//========================================
// ライフの描画処理
//========================================
void CLife::Draw(void)
{
    for (int nCntNumber = 0; nCntNumber < MAX_LIFE_NUMBER; nCntNumber++)
    {
        // 中身があるなら
        if (m_apNumber[nCntNumber] != NULL)
        {
            m_apNumber[nCntNumber]->Draw();
        }
    }
}

//========================================
// プレイヤーのダメージ計算処理
//========================================
void CLife::PlayerDamageCalculation(void)
{
    // 変数宣言
    int nDamage = 0;        // 与えるダメージ
    int nAttackValue = 0;   // 攻撃力
    int nDefenceValue = 0;  // 防御力

    // 敵の攻撃力を取得
    nAttackValue = GetAttackValue(TYPE_ENEMY);

    // プレイヤーの防御力を取得
    nDefenceValue = GetDefenceValue(TYPE_PLAYER);

    // ダメージ計算
    nDamage = nAttackValue - nDefenceValue;

    // ダメージが0未満なら
    if (nDamage < 0)
    {
        // 0に固定
        nDamage = 0;
    }

    // ライフからダメージ分引く
    m_nPlayerLife -= nDamage;

    // 敵が使用したカードが祈るでないなら
    if (CGame::GetEnemyUseCard() != PRAY_ID)
    {
        // 攻撃音再生
        PlayAttackSound(nDamage);

        // ダメージ演出
        CHit3D::Create(PLAYER_DAMAGE_POS, nDamage);
    }

    // ライフが0以下なら、0で固定
    if (m_nPlayerLife <= 0)
    {
        m_nPlayerLife = 0;
    }

    // 変数宣言
    int nIndex;		        // 指数　(ライフ表示用)
    int nRadix = 10;	    // 基数　(ライフ表示用)

    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_LIFE_DIGITS; nIndex++)
    {
        int nLife = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex);
        int nLife2 = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = m_nPlayerLife % nLife / nLife2;

        // 番号を設定
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}

//========================================
// 敵のダメージ計算処理
//========================================
void CLife::EnemyDamageCalculation(void)
{
    // 変数宣言
    int nDamage = 0;        // 与えるダメージ
    int nAttackValue = 0;   // 攻撃力
    int nDefenceValue = 0;  // 防御力

    // プレイヤーの攻撃力を取得
    nAttackValue = GetAttackValue(TYPE_PLAYER);

    // 敵の防御力を取得
    nDefenceValue = GetDefenceValue(TYPE_ENEMY);

    // ダメージ計算
    nDamage = nAttackValue - nDefenceValue;

    // ダメージが0未満なら
    if (nDamage < 0)
    {
        // 0に固定
        nDamage = 0;
    }

    // ライフからダメージ分引く
    m_nEnemyLife -= nDamage;

    // プレイヤーが使用したカードが祈るでないなら
    if (CGame::GetPlayerUseCard() != PRAY_ID)
    {
        // 攻撃音再生
        PlayAttackSound(nDamage);

        // ダメージ演出
        CHit3D::Create(ENEMY_DAMAGE_POS, nDamage);
    }

    // ライフが0以下なら、0で固定
    if (m_nEnemyLife <= 0)
    {
        m_nEnemyLife = 0;
    }

    // 変数宣言
    int nIndex;		        // 指数　(ライフ表示用)
    int nRadix = 10;	    // 基数　(ライフ表示用)

    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_LIFE_DIGITS; nIndex++)
    {
        int nLife = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex);
        int nLife2 = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = m_nEnemyLife % nLife / nLife2;

        // 番号を設定
        m_apNumber[nIndex + MAX_LIFE_DIGITS]->SetNumber(nAnswer);
    }
}

//==========================================
// タイプから攻撃力を取得
//==========================================
int CLife::GetAttackValue(const TYPE type)
{
    // 変数宣言
    int nAttack = 0;
    int nType = 0;

    switch (type)
    {
    case TYPE_PLAYER:
        // プレイヤーの使ったカードを取得
        nType = CGame::GetPlayerUseCard();
        break;

    case TYPE_ENEMY:
        // エネミーの使ったカードを取得
        nType = CGame::GetEnemyUseCard();
        break;
    }

    // ステータスを取得
    CCard::STATUS status;
    status = CCard::List(nType);

    // 攻撃力を結びつける
    nAttack = status.nAttack;

    return nAttack;
}

//==========================================
// タイプから防御力を取得
//==========================================
int CLife::GetDefenceValue(const TYPE type)
{
    // 変数宣言
    int nDefence = 0;
    int nType = 0;

    switch (type)
    {
    case TYPE_PLAYER:
        // プレイヤーの使ったカードを取得
        nType = CGame::GetPlayerUseCard();
        break;

    case TYPE_ENEMY:
        // エネミーの使ったカードを取得
        nType = CGame::GetEnemyUseCard();
        break;
    }

    // ステータスを取得
    CCard::STATUS status;
    status = CCard::List(nType);

    // 防御力を結びつける
    nDefence = status.nDefence;

    return nDefence;
}

//========================================
// プレイヤーの回復処理
//========================================
void CLife::AddPlayerLife(const int nValue)
{
    // ライフ加算
    m_nPlayerLife += nValue;

    // 変数宣言
    int nIndex;		        // 指数　(ライフ表示用)
    int nRadix = 10;	    // 基数　(ライフ表示用)

    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_LIFE_DIGITS; nIndex++)
    {
        int nLife = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex);
        int nLife2 = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = m_nPlayerLife % nLife / nLife2;

        // 番号を設定
        m_apNumber[nIndex]->SetNumber(nAnswer);
    }
}

//========================================
// エネミーの回復処理
//========================================
void CLife::AddEnemyLife(const int nValue)
{
    // ライフ加算
    m_nEnemyLife += nValue;

    // 変数宣言
    int nIndex;		        // 指数　(ライフ表示用)
    int nRadix = 10;	    // 基数　(ライフ表示用)

    // 各桁について計算
    for (nIndex = 0; nIndex < MAX_LIFE_DIGITS; nIndex++)
    {
        int nLife = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex);
        int nLife2 = (int)powf((float)nRadix, MAX_LIFE_DIGITS - (float)nIndex - 1);

        // 
        int nAnswer = m_nPlayerLife % nLife / nLife2;

        // 番号を設定
        m_apNumber[nIndex + MAX_LIFE_DIGITS]->SetNumber(nAnswer);
    }
}

//========================================
// 攻撃音再生処理
//========================================
void CLife::PlayAttackSound(int nDamage)
{
    // 音取得
    CSound *pSound = CManager::GetSound();

    if (nDamage == 0)
    {
        // 完全防御再生
        pSound->Play(CSound::LABEL_SE_004);
    }
    else if (nDamage > 0 && nDamage < BORDER_HIT_SOUND)
    {
        // 斬撃小再生
        pSound->Play(CSound::LABEL_SE_002);
    }
    else if (nDamage >= BORDER_HIT_SOUND)
    {
        // 斬撃大再生
        pSound->Play(CSound::LABEL_SE_003);
    }
}
