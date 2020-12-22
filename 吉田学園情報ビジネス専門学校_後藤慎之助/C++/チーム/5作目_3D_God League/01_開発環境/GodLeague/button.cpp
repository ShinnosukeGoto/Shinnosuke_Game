//===============================================
//
// Button処理 (button.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "button.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "card.h"
#include "scene.h"
#include "card_info.h"
#include "card_name.h"
#include "input.h"
#include "sound.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CButton::m_apTexture[CButton::TYPE_MAX] = {};

//========================================
// Buttonのコンストラクタ
//========================================
CButton::CButton() :CScene2D(OBJTYPE::OBJTYPE_BUTTON)
{
    m_col = DEFAULT_COLOR;
    m_type = CButton::TYPE_MAX;

    m_nID = 0;
}

//========================================
// Buttonのデストラクタ
//========================================
CButton::~CButton()
{

}

//========================================
// Buttonの初期化処理
//========================================
HRESULT CButton::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// Buttonの終了処理
//========================================
void CButton::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// Buttonの更新処理
//========================================
void CButton::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得

    switch (m_type)
    {
    case TYPE_FORGIVE:

        // 防御ターンなら
        if (CGame::GetDefenceTurn() == true)
        {
            // いつでも使えるように
            m_col.a = 1.0f;
        }
        else
        {
            m_col.a = 0.0f;
        }

        break;

    case TYPE_PRAY:

        // 攻撃ターンなら
        if (CGame::GetAttackTurn() == true)
        {
            // 攻撃カードがあるかどうかのフラグを更新
            CCard::CheckDispAttackCard();

            // 攻撃カードがないなら、アクティブに
            if (CCard::GetDispAttackCard() == false)
            {
                m_col.a = 1.0f;
            }
            else
            {
                m_col.a = 0.0f;
            }
        }
        else
        {
            m_col.a = 0.0f;
        }

        break;
    }

    // ゲーム終了時に、透明にする
    GameFinishFade();

    // アニメーションパートでの、切断対応
    DisconnectInAnim();

    // アクティブなら、選択可能
    if (m_col.a == 1.0f)
    {
        size = CollisionCursor(pos, size);
    }

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
// Buttonの描画処理
//========================================
void CButton::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// Buttonの画像ロード処理
//========================================
HRESULT CButton::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/button000.png", &m_apTexture[CButton::TYPE_FORGIVE]);	// 許す
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/button001.png", &m_apTexture[CButton::TYPE_PRAY]);     // 祈る

    return S_OK;
}

//========================================
// Buttonの画像破棄処理
//========================================
void CButton::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CButton::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// Buttonの作成
//================================
CButton *CButton::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CButton::TYPE type)
{
    CButton *pButton = NULL;

    // メモリを確保
    pButton = new CButton;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pButton->m_type = type;

    // 初期化
    pButton->Init(pos, size);

    // メンバ変数を結びつける
    pButton->m_col = col;

    // IDを結びつける
    switch (type)
    {
    case TYPE_FORGIVE:
        pButton->m_nID = FORGIVE_ID;
        break;

    case TYPE_PRAY:
        pButton->m_nID = PRAY_ID;
        break;
    }

    return pButton;
}

//================================
// ゲーム終了時、消すButton
//================================
void CButton::GameFinishFade(void)
{
    if (CGame::GetFinishBattle() == true)
    {
        m_col.a = 0.0f;
    }
}

//================================
// ボタンを選択したとき
//================================
D3DXVECTOR3 CButton::CollisionCursor(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 returnSize = size;  // 返すサイズ

    // カーソルとの当たり判定
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // シーンを取得
        CScene *pScene = GetScene(nCntScene);

        // 中身があるなら
        if (pScene != NULL)
        {
            // タイプを取得
            OBJTYPE objType = pScene->GetObjType();

            // カーソルなら、
            if (objType == CScene::OBJTYPE_CURSOR)
            {
                // シーン2Dにキャスト
                CScene2D *pScene2D = (CScene2D*)pScene;

                // ボタンの当たり判定のサイズを設定
                D3DXVECTOR3 buttonCollisionSize = BUTTON_COLLISION_SIZE;

                // 当たり判定を計算
                if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                    &buttonCollisionSize, &pScene2D->GetSize()) == true)
                {// 当たったなら

                    // カード情報を更新
                    SetCardInfo();

                    // 拡大したサイズを返す
                    returnSize = BUTTON_EXTEND_SIZE;

                    // 選択可能
                    Select();

                    // カーソルは一つなのでforを抜ける
                    break;
                }
                else
                {// 当たってないなら

                    // 普通のサイズを返す
                    returnSize = BUTTON_SIZE;

                    // カーソルは一つなのでforを抜ける
                    break;
                }
            }
        }
    }

    return returnSize;
}

//===================================================================
// カード情報の設定
//===================================================================
void CButton::SetCardInfo(void)
{
    // 攻撃、防御、どちらも0
    CCardInfo *pCardInfo = CGame::GetCardInfo();
    pCardInfo->SetCardInfo(0, 0);

    // カード名を設定
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // シーンを取得
        CScene *pScene = GetScene(nCntScene);

        // 中身があるなら
        if (pScene != NULL)
        {
            // タイプを取得
            OBJTYPE objType = pScene->GetObjType();

            // カード名なら
            if (objType == CScene::OBJTYPE_CARD_NAME)
            {
                // カード名にキャスト
                CCardName *pCardName = (CCardName*)pScene;

                // 自身のカードタイプなら
                if (pCardName->GetUseType() == CCardName::USETYPE_PLAYER)
                {
                    // 自身と違うカードなら
                    if (pCardName->GetType() != m_nID)
                    {
                        // 対象を未使用に
                        pCardName->SetDontUse();

                        // 自身の名前を生成
                        CCardName::Create(CARD_NAME_POS, CARD_NAME_SIZE, m_nID, CCardName::USETYPE_PLAYER);
                    }
                }
            }
        }
    }
}

//===================================================================
// 選択
//===================================================================
void CButton::Select(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 音を取得
    CSound *pSound = CManager::GetSound();

    // 決定キーを押したら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B))
    {
        switch (m_type)
        {
        case TYPE_FORGIVE:

            // カードを相手に送る
            CManager::SendCard(FORGIVE_ID);

            // 防御ターンを終える
            CGame::SetDefenceTurn(false);

            // カード決定音
            pSound->Play(CSound::LABEL_SE_008);

            break;

        case TYPE_PRAY:

            // カードを相手に送る
            CManager::SendCard(PRAY_ID);

            // 攻撃ターンを終える
            CGame::SetAttackTurn(false);

            // カード決定音
            pSound->Play(CSound::LABEL_SE_008);

            //===============================================================
            // ランダムな値で、カードを得る
            CCard::DrawCard(1);
            //===============================================================

            break;
        }
    }
}

//================================
// アニメーションパートでの、切断対応
//================================
void CButton::DisconnectInAnim(void)
{
    // アニメーションパートの切断対応
    if (CManager::GetEnemyDisconnect() == true && CGame::GetState() == CGame::STATE_PLAYER_ATTACK ||
        CManager::GetEnemyDisconnect() == true && CGame::GetState() == CGame::STATE_ENEMY_ATTACK)
    {
        m_col.a = 0.0f;
    }
}
