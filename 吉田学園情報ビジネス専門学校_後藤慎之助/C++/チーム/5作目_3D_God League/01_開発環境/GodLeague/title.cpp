//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "title.h"
#include "input.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "fade.h"
#include "game.h"
#include "bg.h"

//=============================================================================
// タイトルのコンストラクタ
//=============================================================================
CTitle::CTitle()
{
    m_nCntTime = 0;
    m_bNextScene = false;
    m_bStartMatching = false;
    memset(m_apScene2D, 0, sizeof(m_apScene2D));
}

//=============================================================================
// タイトルのデストラクタ
//=============================================================================
CTitle::~CTitle()
{
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
    // ゲームの通信にかかわるものの初期化
    CGame::ResetGame();

    // 背景を生成
    CBg::Create(BG_POS, BG_SIZE, DEFAULT_COLOR, CBg::TYPE_TITLE_BG);

    // ロゴを生成
    CUI::Create(TITLE_LOGO_FIRST_POS, TITLE_LOGO_SIZE, DEFAULT_COLOR, CUI::TYPE_TITLE_LOGO);

    // バージョンを生成
    CUI::Create(VERSION_POS, VERSION_SIZE, DEFAULT_COLOR, CUI::TYPE_VERSION);

    // プレススタート
    CUI::Create(PRESS_START_POS, PRESS_START_SIZE, DEFAULT_COLOR, CUI::TYPE_PRESS_START);

    // BGMを再生
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_BGM_TITLE);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
    // BGMを停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::LABEL_BGM_TITLE);
}

//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
{
    // タイトルの時間をカウント
    m_nCntTime++;

    // タイトルの最大時間
    if (m_nCntTime > TITLE_MAX_TIME)
    {
        m_nCntTime = TITLE_MAX_TIME;
    }

    // キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 音取得
    CSound *pSound = CManager::GetSound();

    // 遷移のフラグがtrueなら
    if (m_bNextScene == true)
    {
        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // フェードしていないなら
            if (CFade::GetFade() == CFade::FADE_NONE)
            {

                // マッチングを開始していないなら
                if (m_bStartMatching == false)
                {
                    // 決定音再生
                    pSound->Play(CSound::LABEL_SE_000);

                    // 吹き出しを生成
                    m_apScene2D[0] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
                        D3DXVECTOR3(480.0f, 135.0f, 0.0f),
                        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
                        CUI::TYPE_TITLE_UI03);

                    // 探していますを作成
                    m_apScene2D[1] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 30.0f, SCREEN_HEIGHT / 2, 0.0f),
                        D3DXVECTOR3(339.0f, 26.5f, 0.0f),
                        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
                        CUI::TYPE_TITLE_UI00);

                    //  ロード中の回転を作成
                    m_apScene2D[3] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 + 180.0f, SCREEN_HEIGHT / 2, 0.0f),
                        UI_NOW_LOADING_SIZE,
                        D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
                        CUI::TYPE_TITLE_UI04);

                    // サーバーに接続
                    CManager::SetStartMatching(true);

                    // マッチング開始
                    m_bStartMatching = true;
                }
            }
        }
    }
    // 遷移のフラグがfalseなら
    else
    {
        // 一定時間経過で遷移可能に
        if (m_nCntTime >= TITLE_NEXT_SCENE)
        {
            m_bNextScene = true;
        }
        // 一定時間経過していなくても
        else
        {
            // エンター、またはスタートボタンを押したら
            if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
            {
                // 遷移可能に
                m_bNextScene = true;
            }
        }
    }

    // マッチングしたら
    if (CManager::GetMatching() == true)
    {
        // 探していますを削除
        if (m_apScene2D[1] != NULL)
        {
            m_apScene2D[1]->Uninit();
            m_apScene2D[1] = NULL;
        }

        // ロード中の回転を削除
        if (m_apScene2D[3] != NULL)
        {
            m_apScene2D[3]->Uninit();
            m_apScene2D[3] = NULL;
        }

        // 対戦相手が見つかりましたを作成
        if (m_apScene2D[2] == NULL)
        {
            m_apScene2D[2] = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
                D3DXVECTOR3(314.0f, 26.5f, 0.0f),
                D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
                CUI::TYPE_TITLE_UI01);
        }

        // フェードしていないなら
        if (CFade::GetFade() == CFade::FADE_NONE)
        {
            // マッチング音再生
            pSound->Play(CSound::LABEL_SE_001);

            // ゲームモードに移行
            CFade::SetFade(CManager::MODE_GAME);
        }
    }
}
