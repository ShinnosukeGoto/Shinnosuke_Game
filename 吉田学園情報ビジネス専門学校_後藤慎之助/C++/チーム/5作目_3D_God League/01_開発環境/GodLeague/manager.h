//====================================================================
//
// 生成の管理処理 (manager.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// 前方宣言(不完全なクラスは、cppの方でヘッダを呼び出す)
//================================================
class CRenderer;
class CInputKeyboard;
class CInputJoypad;
class CSound;
class CMode;
class CFade;
class CCamera;
class CLight;

//================================================
// クラス宣言
//================================================

// マネージャークラス
class CManager
{
public:

    // モードの種類
    typedef enum
    {
        MODE_NONE = 0,
        MODE_TITLE,
        MODE_GAME,
        MODE_RESULT,
        MODE_MAX
    }MODE;

	// ターンの種類
	typedef enum
	{
		TURN_ATTACK = 0,
		TURN_DEFENCE,
		TURN_MAX
	}TURN;

    static void ResetAll(void);     // 全ての通信にかかわるものの静的メンバ変数を初期化
    CManager();
    ~CManager();
    HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
    void Uninit(void);
    void Update(void);
    void Draw(void);
    static CRenderer *GetRenderer(void) { return m_pRenderer; }
    static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }
    static CInputJoypad *GetInputJoypad(void) { return m_pInputJoypad; }
    static CSound *GetSound(void) { return m_pSound; }
    static MODE GetMode(void);
    static void SetMode(MODE mode);
	static void ConnectionThread(void);
	static void SetStartMatching(bool bMatching);
	static void SendCard(int nType);
	static void RecvCard(int nTypeCard, TURN Turn);
	static int GetEnemyUseCard(void) { return m_nEnemyUseCard; }

    static bool GetMatching(void) { return m_bMatching; }       // マッチングしたことを取得
    static int GetAttackOrder(void) { return m_nAttackOrder; }  // 先攻後攻を取得

    //static int GetEnemyUseCard(void) { return m_nEnemyUseCard; }    // 敵が使用したカードを取得
    static int GetPlayerUseCard(void) { return m_nPlayerUseCard; }    // 敵が使用したカードを取得

    static bool GetEnemyDisconnect(void) { return m_bDisconnect; }    // 敵が切断したかどうか

private:
    static CRenderer *m_pRenderer;              // レンダラーのポインタ
    static CInputKeyboard *m_pInputKeyboard;    // キーボードのポインタ
    static CInputJoypad *m_pInputJoypad;        // コントローラーのポインタ
    static CSound *m_pSound;                    // サウンドのポインタ
    static CMode *m_pMode;                      // モードのポインタ
    static CFade *m_pFade;                      // フェードのポインタ
    static CCamera *m_pCamera;                  // カメラのポインタ
    static CLight *m_pLight;                    // ライトのポインタ
	static bool m_bStartMatching;               // マッチング開始を通知
	static bool m_bMatching;                    // マッチングできたかどうか
	static bool m_bAttackTurn;                  // 攻撃したかどうか
	static bool m_bDefenceTurn;                 // 防御ターンかどうか
	static bool m_bDefenceCardUse;              // 防御カードを使った
	static bool m_bRecvCard;                    // 相手からカードを受け取れたかどうか
	static bool m_bDisconnect;                  // 切断フラグ
	static bool m_bGameEnd;                     // ゲーム終了フラグ
	static int m_nAttackOrder;                  // 先攻後攻の情報を保存、1だったら先攻、2だったら後攻
	static int m_nEnemyUseCard;                 // 敵が使用したカード情報
	static int m_nPlayerUseCard;                // プレイヤーが使用したカード
};

#endif