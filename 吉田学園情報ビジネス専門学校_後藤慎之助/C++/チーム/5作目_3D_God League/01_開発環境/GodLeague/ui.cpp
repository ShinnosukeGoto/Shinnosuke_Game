//===============================================
//
// UI処理 (ui.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "title.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CUI::m_apTexture[CUI::TYPE_MAX] = {};

//========================================
// UIのコンストラクタ
//========================================
CUI::CUI() :CScene2D(OBJTYPE::OBJTYPE_UI)
{
    m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_col = DEFAULT_COLOR;
    m_type = CUI::TYPE_NONE;
    m_fAngle = 0.0f;
    m_bUse = true;
    m_bBlinking = false;
    m_nCntTime = 0;
    m_bFadeOut = false;
}

//========================================
// UIのデストラクタ
//========================================
CUI::~CUI()
{

}

//========================================
// UIの初期化処理
//========================================
HRESULT CUI::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_type]);

    return S_OK;
}

//========================================
// UIの終了処理
//========================================
void CUI::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// UIの更新処理
//========================================
void CUI::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();                   // 位置を取得
    D3DXVECTOR3 size = GetSize();                      // サイズを取得
    int nPatternAnim = 0;                              // パターンアニメのカウンタを利用するとき

    //キーボードの確保したメモリを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    switch (m_type)
    {
    case TYPE_TITLE_LOGO:

        // 流れ落ちる
        pos.y += TITLE_LOGO_FLOW_DOWN_SPEED;

        // 一定の値で止めておく
        if (pos.y >= TITLE_LOGO_TARGET_POS_Y)
        {
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // タイトルロゴをすぐに下す
            pos.y = TITLE_LOGO_TARGET_POS_Y;
        }

        break;

    case TYPE_PRESS_START:

        // タイトルの時間をカウント
        m_nCntTime++;

        // タイトルの最大時間
        if (m_nCntTime > TITLE_MAX_TIME)
        {
            m_nCntTime = TITLE_MAX_TIME;
        }

        // 色を変える
        if (m_bFadeOut == true)
        {
            m_col.r -= PRESS_START_FADE_RATE;
            m_col.g -= PRESS_START_FADE_RATE;
            m_col.b -= PRESS_START_FADE_RATE;
        }
        else
        {
            m_col.r += PRESS_START_FADE_RATE;
            m_col.g += PRESS_START_FADE_RATE;
            m_col.b += PRESS_START_FADE_RATE;
        }
        if (m_col.r <= PRESS_START_FADE_OUT_MIN)
        {
            m_bFadeOut = false;
        }
        else if (m_col.r >= 1.0f)
        {
            m_bFadeOut = true;
        }

        // エンター、またはスタートボタンを押したら
        if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_START))
        {
            // 一定時間経過で点滅可能に
            if (m_nCntTime >= TITLE_NEXT_SCENE)
            {
                // 点滅のフラグをtrueに
                m_bBlinking = true;
            }
            // それ以外は、カウンタを遷移可能まで一気に進める
            else
            {
                m_nCntTime = TITLE_NEXT_SCENE;
            }
        }

        // 点滅するなら
        if (m_bBlinking == true)
        {
            // マッチング待機中、目がチカチカするのでやめた
            //// カウンターを進めて、パターンを切り替える
            //nPatternAnim = CountAnimation(2, 2);

            //// 色を2パターンで切り替える
            //if (nPatternAnim == 1)
            //{
            //    m_col.r = PRESS_START_FADE_OUT_MIN;
            //    m_col.g = PRESS_START_FADE_OUT_MIN;
            //    m_col.b = PRESS_START_FADE_OUT_MIN;
            //}
            //else
            //{
            //    m_col.r = 1.0f;
            //    m_col.g = 1.0f;
            //    m_col.b = 1.0f;
            //}
        }

        break;

    case TYPE_GAME_UNDER:

        // 戦闘終了時、消す
        GameFinishFade();

        break;

    case TYPE_CARD_INFO_BG:

        // 戦闘終了時、消す
        GameFinishFade();

        break;

    case TYPE_GAME_START:

        // 拡大
        size = Extend(size);

        // 戦闘終了時、消す
        GameFinishFade();

        break;

    case TYPE_ATTACK_START:

        // 拡大
        size = Extend(size);

        // 戦闘終了時、消す
        GameFinishFade();

        break;

    case TYPE_DEFENCE_START:

        // 拡大
        size = Extend(size);

        // 戦闘終了時、消す
        GameFinishFade();

        break;

    case TYPE_YOU_LOSE:

        // 拡大
        size = Extend(size);

        break;

    case TYPE_YOU_WIN:

        // 拡大
        size = Extend(size);

        break;

    case TYPE_DISCONNECT:

        // 拡大
        size = Extend(size);

        break;

    case TYPE_TITLE_UI04:

        // 待機中
        Wait();

        // 回す
        m_fAngle += UI_NOW_LOADING_ROT_SPEED;

        break;

    case TYPE_WAIT:

        // 待機中
        Wait();

        break;

    case TYPE_ENEMY_USE_ATTACK:

        // 戦闘終了時、消す
        GameFinishFade();

        break;

    case TYPE_ENEMY_USE_DEFENCE:

        // 戦闘終了時、消す
        GameFinishFade();

        break;

    case TYPE_MANUAL:

        // 戦闘終了時、消す
        GameFinishFade();

        break;
    }

    // 色を反映
    SetColor(m_col);

    // 位置を反映
    SetPosition(pos);

    // サイズを反映
    SetSize(size);

    // 頂点座標を設定
    if (m_type == TYPE_TITLE_UI04)
    {
        // ロード中の回転
        SetRotVertex(m_fAngle);
    }
    else
    {
        SetVertex();
    }

    // 使用フラグがfalseなら消す
    if (m_bUse == false)
    {
        Uninit();
    }
}

//========================================
// UIの描画処理
//========================================
void CUI::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// UIの画像ロード処理
//========================================
HRESULT CUI::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    //テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ver.png", &m_apTexture[CUI::TYPE_VERSION]);	             // バージョン
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui014.png", &m_apTexture[CUI::TYPE_TITLE_LOGO]);	         // タイトルロゴ
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui015.png", &m_apTexture[CUI::TYPE_PRESS_START]);	         // スタートボタンを押してね
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/game000.png", &m_apTexture[CUI::TYPE_GAME_UNDER]);	         // ゲームの下のバー
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui000.png", &m_apTexture[CUI::TYPE_HP]);	                 // HP
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui001.png", &m_apTexture[CUI::TYPE_YOU]);	                 // あなた
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui002.png", &m_apTexture[CUI::TYPE_ENEMY]);	             // あいて
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui003.png", &m_apTexture[CUI::TYPE_FIRST]);	             // 先攻
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui004.png", &m_apTexture[CUI::TYPE_SECOND]);	             // 後攻
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui016.png", &m_apTexture[CUI::TYPE_GAME_START]);	         // ゲーム開始
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui005.png", &m_apTexture[CUI::TYPE_ATTACK_START]);	         // 攻撃開始
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui006.png", &m_apTexture[CUI::TYPE_DEFENCE_START]);         // 防御開始
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui007.png", &m_apTexture[CUI::TYPE_CARD_INFO_BG]);	         // カード情報の背景
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui008.png", &m_apTexture[CUI::TYPE_YOU_WIN]);               // 勝ち
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui009.png", &m_apTexture[CUI::TYPE_YOU_LOSE]);	             // 負け
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui010.png", &m_apTexture[CUI::TYPE_ENEMY_USE_ATTACK]);      // 敵の攻撃
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui011.png", &m_apTexture[CUI::TYPE_ENEMY_USE_DEFENCE]);	 // 敵の防御
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui012.png", &m_apTexture[CUI::TYPE_MANUAL]);	             // 遊び方
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui013.png", &m_apTexture[CUI::TYPE_WAIT]);	                 // 相手を待機中
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/ui017.png", &m_apTexture[CUI::TYPE_DISCONNECT]);	         // 切断

    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI000.png", &m_apTexture[CUI::TYPE_TITLE_UI00]);// 対戦相手を探しています・・・
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI001.png", &m_apTexture[CUI::TYPE_TITLE_UI01]);// 対戦相手が見つかりました！
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI002.png", &m_apTexture[CUI::TYPE_TITLE_UI02]);// 対戦相手が見つかりませんでした
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI003.png", &m_apTexture[CUI::TYPE_TITLE_UI03]);// 吹き出し
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/titleUI004.png", &m_apTexture[CUI::TYPE_TITLE_UI04]);// ロード中に回転するやつ

    return S_OK;
}

//========================================
// UIの画像破棄処理
//========================================
void CUI::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CUI::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//================================
// UIの作成
//================================
CUI *CUI::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, D3DXCOLOR col, CUI::TYPE type)
{
    CUI *pUI = NULL;

    // メモリを確保
    pUI = new CUI;

    // タイプのみ、テクスチャを割り当てのために結びつけておく
    pUI->m_type = type;

    // 初期化
    pUI->Init(pos, size);

    // メンバ変数を結びつける
    pUI->m_col = col;

    return pUI;
}

//================================
// ゲーム終了時、消すUI
//================================
void CUI::GameFinishFade(void)
{
    if (CGame::GetFinishBattle() == true)
    {
        m_col.a = 0.0f;
    }
}

//================================
// 相手を待機中
//================================
void CUI::Wait(void)
{
    // ゲーム中なら
    if (CManager::GetMode() == CManager::MODE_GAME)
    {
        // 攻撃ターンでも、防御ターンでもないなら
        if (CGame::GetAttackTurn() == false && CGame::GetDefenceTurn() == false)
        {
            m_col.a = 1.0f;
        }
        else
        {
            m_col.a = 0.0f;
        }

        // ゲームのステートが通常以外なら
        if (CGame::GetState() != CGame::STATE_NORMAL)
        {
            m_col.a = 0.0f;
        }

        // 戦闘終了時、消す
        GameFinishFade();
    }
}

//================================
// 拡大するUI
//================================
D3DXVECTOR3 CUI::Extend(D3DXVECTOR3 size)
{
    // 変数宣言
    D3DXVECTOR3 returnSize = size;  // 返すサイズ
    int nCnt = 0;                   // カウンタ

    // 切断以外なら
    if (m_type != TYPE_DISCONNECT)
    {
        // 横サイズ拡大
        returnSize.x += UI_DISCONNECT_EXTEND_RATE_X;

        // 縦サイズ拡大
        returnSize.y += UI_DISCONNECT_EXTEND_RATE_Y;

        // 横サイズの制限
        if (returnSize.x >= UI_DISCONNECT_MAX_SIZE_X)
        {
            returnSize.x = UI_DISCONNECT_MAX_SIZE_X;
        }

        // 縦サイズの制限
        if (returnSize.y >= UI_DISCONNECT_MAX_SIZE_Y)
        {
            returnSize.y = UI_DISCONNECT_MAX_SIZE_Y;
        }
    }
    else
    {
        // 横サイズ拡大
        returnSize.x += UI_YOUR_TURN_EXTEND_RATE_X;

        // 縦サイズ拡大
        returnSize.y += UI_YOUR_TURN_EXTEND_RATE_Y;

        // 横サイズの制限
        if (returnSize.x >= UI_YOUR_TURN_MAX_SIZE_X)
        {
            returnSize.x = UI_YOUR_TURN_MAX_SIZE_X;
        }

        // 縦サイズの制限
        if (returnSize.y >= UI_YOUR_TURN_MAX_SIZE_Y)
        {
            returnSize.y = UI_YOUR_TURN_MAX_SIZE_Y;
        }
    }

    // ターン表示は、各自のターンが終わったら消す
    switch (m_type)
    {
    case TYPE_GAME_START:

        // アニメーションのカウンタを利用して、UIのフェードアウトにつなげる
        nCnt = CountAnimation(1, MAX_ANIM_COUNTER);

        // フェードアウトを始めるカウントを超えたら、α値を下げていく
        if (nCnt >= UI_YOUR_TURN_FADE_OUT_COUNT)
        {
            m_col.a -= UI_YOUR_TURN_FADE_OUT_RATE;
        }

        break;

    case TYPE_ATTACK_START:

        // 攻撃ターンでないなら、透明にしていく
        if (CGame::GetAttackTurn() == false)
        {
            m_col.a -= UI_YOUR_TURN_FADE_OUT_RATE;
        }

        // アニメーションパートの切断対応
        DisconnectInAnim();

        break;

    case TYPE_DEFENCE_START:

        // 防御ターン出ないなら、だんだん透明にしていく
        if (CGame::GetDefenceTurn() == false)
        {
            m_col.a -= UI_YOUR_TURN_FADE_OUT_RATE;
        }

        // アニメーションパートの切断対応
        DisconnectInAnim();

        break;
    }

    // 透明度が0を下回ったら、消す
    if (m_col.a <= 0.0f)
    {
        m_bUse = false;
    }

    return returnSize;
}

//================================
// アニメーションパートでの、切断対応
//================================
void CUI::DisconnectInAnim(void)
{
    // アニメーションパートの切断対応
    if (CManager::GetEnemyDisconnect() == true && CGame::GetState() == CGame::STATE_PLAYER_ATTACK ||
        CManager::GetEnemyDisconnect() == true && CGame::GetState() == CGame::STATE_ENEMY_ATTACK)
    {
        m_col.a = 0.0f;
    }
}
