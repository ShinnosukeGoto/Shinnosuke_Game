//=============================================================================
//
// ゲーム処理 [game.cpp]
// Author : 後藤慎之助
//
//=============================================================================
#include "game.h"
#include "manager.h"
#include "sound.h"
#include "ui.h"
#include "cursor.h"
#include "card.h"
#include "life.h"
#include "character.h"
#include "card_info.h"
#include "card_name.h"
#include "sky.h"
#include "ui3d.h"
#include "animation.h"
#include "camera.h"
#include "fade.h"
#include "manual.h"
#include "button.h"
#include "field.h"

#include "input.h"

//========================================
// マクロ定義
//========================================

// スタートでゲーム状態の管理
#define START_ATTACK 210    // 先攻の攻撃開始

// アニメーションでゲーム状態の管理
#define ANIM_SET_CAMERA 2   // カメラセット
#define ANIM_START 60       // アニメーションスタート
#define ANIM_DAMAGE 150     // ダメージ計算、演出
#define ANIM_END 240        // アニメーション終了

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
CLife *CGame::m_pLife = NULL;
CCardInfo *CGame::m_pCardInfo = NULL;
CCharacter *CGame::m_apCharacter[MAX_CHARACTER] = {};
bool CGame::m_bAttackTurn = false;
bool CGame::m_bDefenceTurn = false;

int CGame::m_nEnemyUseCard = 0;
int CGame::m_nPlayerUseCard = 0;

bool CGame::m_bFinishBattle = false;

bool CGame::m_bUpdateEnemyUseCardInfo = false;
bool CGame::m_bUpdateAttackTurn = false;
bool CGame::m_bUpdateDefenceTurn = false;

CGame::STATE CGame::m_state = STATE_NORMAL;

//=============================================================================
// ゲームで通信にかかわるものの初期化
//=============================================================================
void CGame::ResetGame(void)
{
    m_bFinishBattle = false;

    m_bUpdateAttackTurn = false;
    m_bUpdateDefenceTurn = false;

    m_state = STATE_START;
}

//=============================================================================
// ゲームのコンストラクタ
//=============================================================================
CGame::CGame()
{
    m_nCntGameTime = 0;
    m_nCntFinishGame = 0;

    m_pLife = NULL;
    m_pCardInfo = NULL;
    memset(m_apCharacter, 0, sizeof(m_apCharacter));

    // お互い、攻撃ターンでも防御ターンでもない
    m_bAttackTurn = false;
    m_bDefenceTurn = false;

    // 静的メンバ変数を初期化
    m_nEnemyUseCard = 0;
    m_nPlayerUseCard = 0;
    m_bFinishBattle = false;
    m_bUpdateEnemyUseCardInfo = false;
    m_bUpdateAttackTurn = false;
    m_bUpdateDefenceTurn = false;
    m_state = STATE_START;
}

//=============================================================================
// ゲームのデストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
    // 画面下の背景を生成
    CUI::Create(GAME_UNDER_POS, GAME_UNDER_SIZE, DEFAULT_COLOR, CUI::TYPE_GAME_UNDER);

    // UIを生成
    CUI::Create(UI_HP_PLAYER_POS, UI_HP_PLAYER_SIZE, DEFAULT_COLOR, CUI::TYPE_HP);
    CUI::Create(UI_HP_ENEMY_POS, UI_HP_ENEMY_SIZE, DEFAULT_COLOR, CUI::TYPE_HP);
    CUI::Create(UI_YOU_POS, UI_YOU_SIZE, DEFAULT_COLOR, CUI::TYPE_YOU);
    CUI::Create(UI_ENEMY_POS, UI_ENEMY_SIZE, DEFAULT_COLOR, CUI::TYPE_ENEMY);
    CUI::Create(UI_CARD_INFO_POS, UI_CARD_INFO_SIZE, DEFAULT_COLOR, CUI::TYPE_CARD_INFO_BG);
    CUI::Create(UI_MANUAL_POS, UI_MANUAL_SIZE, DEFAULT_COLOR, CUI::TYPE_MANUAL);
    CUI::Create(UI_WAIT_POS, UI_WAIT_SIZE, DEFAULT_COLOR, CUI::TYPE_WAIT);
    CUI::Create(UI_NOW_LOADING_IN_GAME_POS, UI_NOW_LOADING_SIZE, DEFAULT_COLOR, CUI::TYPE_TITLE_UI04);

    // バトルスタート表示
    CUI::Create(UI_YOUR_TURN_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_GAME_START);

    // 遊び方生成
    CManual::Create(MANUAL_POS, MANUAL_SIZE);

    // ライフを生成
    m_pLife = CLife::Create(DEFAULT_VECTOR, LIFE_SIZE);
    m_pLife->AddPlayerLife(0);  // プレイヤーのライフ表示の初期化
    m_pLife->AddEnemyLife(0);   // 敵のライフ表示の初期化

    // カーソルを生成
    CCursor::Create(CURSOR_POS, CURSOR_SIZE);

    // カードをランダムに生成
    CCard::DrawCard(FIRST_CARD);

    // カード情報を生成
    m_pCardInfo = CCardInfo::Create(DEFAULT_VECTOR, CARD_INFO_NUMBER_SIZE);
    m_pCardInfo->SetCardInfo(0, 0);                                     // 何もカードを選択していないので、攻撃も防御も0
    m_pCardInfo->SetEnemyCardInfo(CARD_INFO_ENEMY_TRANSPARENT);         // -1で数字を透明に

    // カードの中に、武器や防具があるかどうかチェック
    CCard::CheckDispAttackCard();
    CCard::CheckDispDefenceCard();

    // カード名を生成
    CCardName::Create(CARD_NAME_POS, CARD_NAME_SIZE, CCard::TYPE_NONE, CCardName::USETYPE_PLAYER); // 何もカードを選択していないので、「なし」

    // ボタンの生成
    CButton::Create(BUTTON_FORGIVE_POS, BUTTON_SIZE, DEFAULT_COLOR, CButton::TYPE_FORGIVE);
    CButton::Create(BUTTON_PRAY_POS, BUTTON_SIZE, DEFAULT_COLOR, CButton::TYPE_PRAY);

    // キャラクターの生成
    m_apCharacter[0] = CCharacter::Create(D3DXVECTOR3(100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(90), 0.0f));
    m_apCharacter[1] = CCharacter::Create(D3DXVECTOR3(-100.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DXToRadian(270), 0.0f));

    // カメラのロックオン場所
    CCamera::SetCameraLookAt(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

    // ビルボードカードの生成
	CUi3D::Create(D3DXVECTOR3(100.0f, 60.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0);
	CUi3D::Create(D3DXVECTOR3(-100.0f, 60.0f, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 1);

    // スカイボックスの生成
    CSky::Create();

	// フィールドの生成
	CField::Create();

    // カードを一瞬で並べ替えておく
    CCard::SortCards();

    // BGM、歓声を再生
    CSound *pSound = CManager::GetSound();
    pSound->Play(CSound::LABEL_BGM_GAME);
    pSound->Play(CSound::LABEL_SE_005);

    return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
    // BGM、歓声を停止
    CSound *pSound = CManager::GetSound();
    pSound->Stop(CSound::LABEL_BGM_GAME);
    pSound->Stop(CSound::LABEL_SE_005);

    // マネージャーの静的メンバ変数を全て初期化
    CManager::ResetAll();
}

//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
    // カードの動的な並び替え
    CCard::SortMoveCards();

    // 勝敗判定
    Judgment();

    // 敵が使用したカード情報を更新
    SetEnemyUseCardInfo();

    // ターン表示を更新
    SetYourTurn();

    // ゲーム状態の管理
    ManageState();
}

//=============================================================================
// 勝敗判定
//=============================================================================
void CGame::Judgment(void)
{
    // 変数宣言
    int nPlayerLife = 0;    // 自分のHP
    int nEnemyLife = 0;     // 相手のHP

    // 各ライフを取得
    nPlayerLife = m_pLife->GetPlayerLife();
    nEnemyLife = m_pLife->GetEnemyLife();

    // 自分負け
    if (nPlayerLife <= 0)
    {
        // 勝敗がついてないなら
        if (m_bFinishBattle == false)
        {
            // ゴング複数
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_007);

            // 負け
            CUI::Create(UI_JUDGMENT_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_YOU_LOSE);

            // 自分負けモーション
            m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_LOSE);

            // 相手勝ちモーション
            m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_ENEMY_WIN);

            // 勝敗がついた
            m_bFinishBattle = true;

            // タイトルに行くまでの時間を設定
            m_nCntFinishGame = COUNT_WAIT_FINISH;
        }
    }

    // 自分勝ち
    if (nEnemyLife <= 0)
    {
        // 勝敗がついてないなら
        if (m_bFinishBattle == false)
        {
            // ゴング複数
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_007);

            // 勝ち
            CUI::Create(UI_JUDGMENT_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_YOU_WIN);

            // 自分勝ちモーション
            m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_PLAYER_WIN);

            // 相手負けモーション
            m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_LOSE);

            // 勝敗がついた
            m_bFinishBattle = true;

            // タイトルに行くまでの時間を設定
            m_nCntFinishGame = COUNT_WAIT_FINISH;
        }
    }

    // リザルトに行くまでのカウンタ
    if (m_nCntFinishGame > 0)
    {
        m_nCntFinishGame--;

        // 0になった時に
        if (m_nCntFinishGame == 0)
        {
            // フェードしていないなら
            if (CFade::GetFade() == CFade::FADE_NONE)
            {
                // タイトルに移行
                CFade::SetFade(CManager::MODE_TITLE);
            }
        }
    }
}

//=============================================================================
// プレイヤーの攻撃処理
//=============================================================================
void CGame::AttackPlayerToEnemy(void)
{
    // 敵のダメージ計算
    m_pLife->EnemyDamageCalculation();
}

//=============================================================================
// エネミーの攻撃処理
//=============================================================================
void CGame::AttackEnemyToPlayer(void)
{
    // プレイヤーのダメージ計算
    m_pLife->PlayerDamageCalculation();
}

//=============================================================================
// エネミーのカード情報設定処理
//=============================================================================
void CGame::SetEnemyUseCardInfo(void)
{
    // 情報を更新のフラグがtrueなら
    if (m_bUpdateEnemyUseCardInfo == true)
    {
        // 今ある敵のカード名を消す
        for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
        {
            // シーンを取得
            CScene *pScene = CScene::GetScene(nCntScene);

            // 中身があるなら
            if (pScene != NULL)
            {
                // タイプを取得
                CScene::OBJTYPE objType = pScene->GetObjType();

                // カード名なら
                if (objType == CScene::OBJTYPE_CARD_NAME)
                {
                    // カード名にキャスト
                    CCardName *pCardName = (CCardName*)pScene;

                    // 敵のカードなら
                    if (pCardName->GetUseType() == CCardName::USETYPE_ENEMY)
                    {
                        // 対象を未使用に
                        pCardName->SetDontUse();
                    }
                }
                // UIなら
                else if (objType == CScene::OBJTYPE_UI)
                {
                    // UIにキャスト
                    CUI *pUI = (CUI*)pScene;

                    // 敵の使用カードUIなら
                    if (pUI->GetType() == CUI::TYPE_ENEMY_USE_ATTACK || pUI->GetType() == CUI::TYPE_ENEMY_USE_DEFENCE)
                    {
                        // 対象を未使用に
                        pUI->SetDontUse();
                    }
                }
            }
        }

        // 相手が切断していないなら
        if (CManager::GetEnemyDisconnect() == false)
        {
            // 敵の使用したカード情報を取得
            CCard::STATUS status;
            status = CCard::List(m_nEnemyUseCard);

            // 攻撃力があるなら、または祈るなら
            if (status.nAttack > 0 || m_nEnemyUseCard == PRAY_ID)
            {
                // 攻撃用のUI
                CUI::Create(UI_ENEMY_USE_POS, UI_ENEMY_USE_SIZE, DEFAULT_COLOR, CUI::TYPE_ENEMY_USE_ATTACK);

                // 攻撃力設定
                m_pCardInfo->SetEnemyCardInfo(status.nAttack);
            }

            // 防御力があるなら、または許すなら
            if (status.nDefence > 0 || m_nEnemyUseCard == FORGIVE_ID)
            {
                // 防御用のUI
                CUI::Create(UI_ENEMY_USE_POS, UI_ENEMY_USE_SIZE, DEFAULT_COLOR, CUI::TYPE_ENEMY_USE_DEFENCE);

                // 防御力設定
                m_pCardInfo->SetEnemyCardInfo(status.nDefence);
            }

            // カード名を生成
            CCardName::Create(CARD_NAME_ENEMY_POS, CARD_NAME_SIZE, m_nEnemyUseCard, CCardName::USETYPE_ENEMY);
        }
        // 相手が切断したなら、相手のカード情報の数字を非表示に
        else
        {
            m_pCardInfo->SetEnemyCardInfo(CARD_INFO_ENEMY_TRANSPARENT);         // -1で数字を透明に
        }

        // フラグをリセット
        m_bUpdateEnemyUseCardInfo = false;
    }
}

//=============================================================================
// ターン表示を設定
//=============================================================================
void CGame::SetYourTurn(void)
{
    // 攻撃ターン表示のフラグがtrueなら
    if (m_bUpdateAttackTurn == true)
    {
        // 2回通らないようfalseに
        m_bUpdateAttackTurn = false;

        // 攻撃ターン表示
        CUI::Create(UI_YOUR_TURN_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_ATTACK_START);
    }

    // 防御ターン表示のフラグがtrueなら
    if (m_bUpdateDefenceTurn == true)
    {
        // 2回通らないようfalseに
        m_bUpdateDefenceTurn = false;

        // 防御ターン表示
        CUI::Create(UI_YOUR_TURN_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_DEFENCE_START);
    }
}

//=============================================================================
// 敵が使用したカードを設定
//=============================================================================
void CGame::SetEnemyUseCard(int nCard)
{
    // 敵が使用したカードを結びつける
    m_nEnemyUseCard = nCard;

    // 敵が使用したカード情報の更新のフラグをtrueに
    m_bUpdateEnemyUseCardInfo = true;
}

//=============================================================================
// プレイヤーが使用したカードを設定
//=============================================================================
void CGame::SetPlayerUseCard(int nCard)
{
    m_nPlayerUseCard = nCard;
}

//=============================================================================
// 攻撃ターンかどうかを設定
//=============================================================================
void CGame::SetAttackTurn(bool bAttack)
{
    // ターンを結びつける
    m_bAttackTurn = bAttack;

    // trueなら、UIを生成のフラグをtrueに
    if (bAttack == true)
    {
        m_bUpdateAttackTurn = true;
    }
}

//=============================================================================
// 防御ターンかどうかを設定
//=============================================================================
void CGame::SetDefenceTurn(bool bDefence)
{
    // ターンを結びつける
    m_bDefenceTurn = bDefence;

    // trueなら、UIを生成のフラグをtrueに
    if (bDefence == true)
    {
        m_bUpdateDefenceTurn = true;
    }
}

//=============================================================================
// ゲーム状態の管理
//=============================================================================
void CGame::ManageState(void)
{
    // 音を取得
    CSound *pSound = CManager::GetSound();

    switch (m_state)
    {
    case STATE_START:

        // カウンタを加算
        m_nCntGameTime++;

        if (m_nCntGameTime == START_ATTACK)
        {
            // 先攻なら
            if (CManager::GetAttackOrder() == 1)
            {
                // 攻撃ターンに
                m_bAttackTurn = true;

                // 攻撃ターンのUIを生成
                CUI::Create(UI_YOUR_TURN_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_ATTACK_START);
            }

            // カウンタリセット
            m_nCntGameTime = 0;

            // 状態リセット
            m_state = STATE_NORMAL;
        }

        break;

    case STATE_NORMAL:

        // 切断勝利
        DisconnectWin();

        break;

    case STATE_PLAYER_ATTACK:

        // カウンタを加算
        m_nCntGameTime++;

        switch (m_nCntGameTime)
        {
        case ANIM_SET_CAMERA:

            // ゴング一回
            pSound->Play(CSound::LABEL_SE_006);

            // バトル用に設定
            CCamera::SetCameraState(CCamera::CAMERA_BATTLE);

            break;

        case ANIM_START:

            // 祈るなら
            if (m_nPlayerUseCard == PRAY_ID)
            {
                // 祈るモーション
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_PRAY);
            }
            // 普通の攻撃なら
            else
            {
                // 攻撃モーション
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_ATTACK);
            }

            // 許すなら
            if (m_nEnemyUseCard == FORGIVE_ID)
            {
                // 許すモーション
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_PRAY);
            }
            else
            {
                // 防御モーション
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_DIFENCE);
            }

            break;

        case ANIM_DAMAGE:

            // 敵へのダメージ
            CGame::AttackPlayerToEnemy();

            break;

        case ANIM_END:

            // 勝敗がついてないなら
            if (m_bFinishBattle == false)
            {
                // 祈るなら
                if (m_nPlayerUseCard == PRAY_ID)
                {
                    // 待機に変える
                    m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_IDLE);
                }

                // 待機に変える
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_IDLE);

                // カメラをオーバールックに
                CCamera::SetCameraState(CCamera::CAMERA_OVER_LOOK);

                // カウンタリセット
                m_nCntGameTime = 0;

                // 状態リセット
                m_state = STATE_NORMAL;
            }

            break;
        }

        break;

    case STATE_ENEMY_ATTACK:

        // カウンタを加算
        m_nCntGameTime++;

        switch (m_nCntGameTime)
        {
        case ANIM_SET_CAMERA:

            // ゴング一回
            pSound->Play(CSound::LABEL_SE_006);

            // バトル用に設定
            CCamera::SetCameraState(CCamera::CAMERA_BATTLE);

            break;

        case ANIM_START:

            // 祈るなら
            if (m_nEnemyUseCard == PRAY_ID)
            {
                // 祈るモーション
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_PRAY);
            }
            // 普通の攻撃なら
            else
            {
                // 攻撃モーション
                m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_ATTACK);
            }

            // 許すなら
            if (m_nPlayerUseCard == FORGIVE_ID)
            {
                // 許すモーション
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_PRAY);
            }
            else
            {
                // 防御モーション
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_DIFENCE);
            }

            break;

        case ANIM_DAMAGE:

            // プレイヤーへのダメージ
            CGame::AttackEnemyToPlayer();

            break;

        case ANIM_END:

            // 勝敗がついてないなら
            if (m_bFinishBattle == false)
            {
                // 祈るなら
                if (m_nEnemyUseCard == PRAY_ID)
                {
                    // 待機に変える
                    m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_IDLE);
                }

                // 待機に変える
                m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_IDLE);

                // カメラをオーバールックに
                CCamera::SetCameraState(CCamera::CAMERA_OVER_LOOK);

                // カウンタリセット
                m_nCntGameTime = 0;

                // 状態リセット
                m_state = STATE_NORMAL;

                // プレイヤーの攻撃ターンの開始
                CGame::SetAttackTurn(true);
            }

            break;
        }

        break;
    }
}

//=============================================================================
// 切断勝利処理
//=============================================================================
void CGame::DisconnectWin(void)
{
    // 敵が切断したなら
    if (CManager::GetEnemyDisconnect() == true)
    {
        // 勝敗がついてないなら
        if (m_bFinishBattle == false)
        {
            // ゴング複数
            CSound *pSound = CManager::GetSound();
            pSound->Play(CSound::LABEL_SE_007);

            // カメラをバトルに
            CCamera::SetCameraState(CCamera::CAMERA_BATTLE);

            // 切断文章
            CUI::Create(UI_DISCONNECT_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_DISCONNECT);

            // 勝ち
            CUI::Create(UI_JUDGMENT_POS, DEFAULT_VECTOR, DEFAULT_COLOR, CUI::TYPE_YOU_WIN);

            // 自分アイテム使用モーション
            m_apCharacter[PLAYER]->GetAnimation()->SetAnimation(CAnimation::ANIM_USE);

            // 相手負けモーション
            m_apCharacter[ENEMY]->GetAnimation()->SetAnimation(CAnimation::ANIM_LOSE);

            // 勝敗がついた
            m_bFinishBattle = true;

            // タイトルに行くまでの時間を設定
            m_nCntFinishGame = COUNT_WAIT_FINISH;
        }
    }
}
