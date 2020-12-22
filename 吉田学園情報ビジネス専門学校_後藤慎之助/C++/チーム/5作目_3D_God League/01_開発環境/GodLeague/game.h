//=============================================================================
//
// ゲーム処理 [game.h]
// Author : 後藤慎之助
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include "main.h"
#include "mode.h"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CLife;
class CCardInfo;
class CCharacter;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//ゲーム状態の管理フレーム
#define COUNT_WAIT_FINISH 300	    // ゲーム終了時に、タイトル画面へ行くまでの時間
#define FIRST_CARD 7                // 最初の手札
#define MAX_CARD 9                  // カードの最大数

#define PLAYER 0
#define ENEMY 1

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CGame : public CMode
{
public:

    // ゲームの状態
    typedef enum
    {
        STATE_START = 0,        // 開始
        STATE_NORMAL,           // 通常
        STATE_PLAYER_ATTACK,	// プレイヤーの攻撃演出
        STATE_ENEMY_ATTACK,	    // エネミーの攻撃演出
        STATE_MAX			    // 状態の最大数
    }STATE;

    static void ResetGame(void);                                            // ゲームをリセット
    CGame();
    ~CGame();
    HRESULT Init(void);
    void Uninit(void);
    void Update(void);
    void Judgment(void);                                                    // 勝敗判定
    static CLife *GetLife(void) { return m_pLife; }
    static CCardInfo *GetCardInfo(void) { return m_pCardInfo; }
    static bool GetAttackTurn(void) { return m_bAttackTurn; }
    static void SetAttackTurn(bool bAttack);
    static bool GetDefenceTurn(void) { return m_bDefenceTurn; }
    static void SetDefenceTurn(bool bDefence);
    static void AttackPlayerToEnemy(void);
    static void AttackEnemyToPlayer(void);
    void SetEnemyUseCardInfo(void);                                         // 敵が使用したカード情報を結びつける
    void SetYourTurn(void);

    static int GetEnemyUseCard(void) { return m_nEnemyUseCard; }            // 敵が使用したカードを取得
    static int GetPlayerUseCard(void) { return m_nPlayerUseCard; }          // プレイヤーが使用したカードを取得
    static void SetEnemyUseCard(int nCard);                                 // 敵が使用したカードを設定
    static void SetPlayerUseCard(int nCard);                                // プレイヤーが使用したカードを設定

    static bool GetFinishBattle(void) { return m_bFinishBattle; }           // 戦いが終わったかどうかを取得

    void ManageState(void);                                                 // ゲーム状態の管理
    static STATE GetState(void) { return m_state; }                         // ゲームの状態を取得
    static void SetState(STATE state) { m_state = state; }                  // ゲームの状態を設定

    void DisconnectWin(void);                                               // 切断勝利

private:
    int m_nCntGameTime;                              // ゲーム時間のカウンタ
    int m_nCntFinishGame;                            // ゲームを終わるまでのカウンタ

    static CLife *m_pLife;                           // ライフのポインタ
    static CCardInfo *m_pCardInfo;                   // カード情報のポインタ
    static CCharacter *m_apCharacter[MAX_CHARACTER]; // キャラクターのポインタ    
    static bool m_bAttackTurn;                       // 攻撃ターンかどうか
    static bool m_bDefenceTurn;                      // 防御ターンかどうか

    static int m_nEnemyUseCard;                      // 敵が使用したカード情報
    static int m_nPlayerUseCard;                     // プレイヤーが使用したカード

    static bool m_bFinishBattle;                     // 戦いが終わったかどうか

    static bool m_bUpdateEnemyUseCardInfo;           // 敵が使用したカード情報を更新するかどうか
    static bool m_bUpdateAttackTurn;                 // アタックターン表示するかどうか
    static bool m_bUpdateDefenceTurn;                // ディフェンスターン表示するかどうか

    static STATE m_state;                            // 状態
};

#endif