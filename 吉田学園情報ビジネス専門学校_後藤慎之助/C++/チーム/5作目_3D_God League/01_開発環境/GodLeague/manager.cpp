//===============================================
//
// 生成の管理処理 (manager.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "manager.h"
#include "scene2d.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "mode.h"
#include "title.h"
#include "game.h"
#include "ui.h"
#include <typeinfo.h>
#include "cursor.h"
#include "card.h"
#include "fade.h"
#include "number.h"
#include "light.h"
#include "camera.h"
#include "scene3d.h"
#include "character.h"
#include "tcp_client.h"
#include "card_name.h"
#include "sky.h"
#include "manual.h"
#include "button.h"
#include "field.h"
#include "ui3d.h"
#include "bg.h"
#include "hit3d.h"

//========================================
// マクロ定義
//========================================

//========================================
// 静的メンバ変数宣言
//========================================
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CSound *CManager::m_pSound = NULL;
CMode *CManager::m_pMode = NULL;
CFade *CManager::m_pFade = NULL;
CCamera *CManager::m_pCamera = NULL;
CLight *CManager::m_pLight = NULL;
bool CManager::m_bStartMatching = false;
bool CManager::m_bMatching = false;
bool CManager::m_bAttackTurn = false;
bool CManager::m_bDefenceTurn = false;
bool CManager::m_bDefenceCardUse = false;
bool CManager::m_bRecvCard = false;
bool CManager::m_bDisconnect = false;
bool CManager::m_bGameEnd = false;
int CManager::m_nAttackOrder = 0;
int CManager::m_nEnemyUseCard = 0;
int CManager::m_nPlayerUseCard = 0;

//========================================
// 全ての通信にかかわるものの初期化
//========================================
void CManager::ResetAll(void)
{
    m_bStartMatching = false;
    m_bMatching = false;
    m_bAttackTurn = false;
    m_bDefenceTurn = false;
    m_bDefenceCardUse = false;
    m_bRecvCard = false;
	m_bDisconnect = false;
	m_bGameEnd = false;
    m_nAttackOrder = 0;
    m_nEnemyUseCard = 0;
    m_nPlayerUseCard = 0;

	m_pCamera->ResetCamera();
}

//========================================
// 生成の管理のデフォルトコンストラクタ
//========================================
CManager::CManager()
{

}

//========================================
// 生成の管理のデストラクタ
//========================================
CManager::~CManager()
{

}

//========================================
// 生成の管理の初期化処理
//========================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// レンダラーの生成(一回しか生成しないものは、Createを用意していない)
	m_pRenderer = new CRenderer;

	// レンダラー初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{
		return E_FAIL;
	}

	// キーボードの生成(newとInitは連続させて書く)
	m_pInputKeyboard = new CInputKeyboard;

	// キーボードの初期化処理
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// コントローラの生成
	m_pInputJoypad = new CInputJoypad;

	// コントローラの初期化処理
	if (FAILED(m_pInputJoypad->Init(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// サウンドの生成
	m_pSound = new CSound;
	if (FAILED(m_pSound->Init(hWnd)))
	{
		return E_FAIL;
	}

	// ライトの生成
	m_pLight = new CLight;
	if (FAILED(m_pLight->Init()))
	{
		return E_FAIL;
	}

	// カメラの生成
	m_pCamera = new CCamera;
	if (FAILED(m_pCamera->Init()))
	{
		return E_FAIL;
	}

	// フェードの生成
	m_pFade = CFade::Create(MODE_TITLE);    // 最初はタイトル

	// テクスチャのロード
	CUI::Load();
	CCursor::Load();
	CCard::Load();
	CNumber::Load();
	CCharacter::Load();
    CCardName::Load();
    CSky::Load();
    CManual::Load();
    CButton::Load();
    CBg::Load();
	CField::Load();
	CUi3D::Load();
    CHit3D::Load();

	return S_OK;
}

//========================================
// 生成の管理の終了処理
//========================================
void CManager::Uninit(void)
{
	// 全てのオブジェクトを破棄
	CScene::ReleaseAll();

	// テクスチャのアンロード
	CUI::Unload();
	CCursor::Unload();
	CCard::Unload();
	CNumber::Unload();
	CCharacter::Unload();
    CCardName::Unload();
    CSky::Unload();
    CManual::Unload();
    CButton::Unload();
    CBg::Unload();
	CField::Unload();
	CUi3D::Unload();
    CHit3D::Unload();

	// モード破棄
	if (m_pMode != NULL)
	{
		// モードの終了処理
		m_pMode->Uninit();

		// モードのメモリの開放
		delete m_pMode;
		m_pMode = NULL;
	}

	// フェード破棄
	if (m_pFade != NULL)
	{
		// フェードの終了処理
		m_pFade->Uninit();

		// フェードのメモリの開放
		delete m_pFade;
		m_pFade = NULL;
	}

	// サウンド破棄
	if (m_pSound != NULL)
	{
		// サウンド終了処理
		m_pSound->Uninit();

		// サウンドのメモリの開放
		delete m_pSound;
		m_pSound = NULL;
	}

	// コントローラ破棄
	if (m_pInputJoypad != NULL)
	{
		// コントローラ終了処理
		m_pInputJoypad->Uninit();

		// コントローラのメモリの開放
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	// キーボード破棄（Initで生成した逆順で破棄する）
	if (m_pInputKeyboard != NULL)
	{
		// キーボード終了処理
		m_pInputKeyboard->Uninit();

		// キーボードのメモリの開放
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	// レンダラー破棄
	if (m_pRenderer != NULL)
	{
		// レンダラー終了処理
		m_pRenderer->Uninit();

		// レンダラーのメモリの開放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//========================================
// 生成の管理の更新処理
//========================================
void CManager::Update(void)
{
	// キーボード更新処理(最初に行う)
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	// コントローラ更新処理
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}

	// レンダラー更新処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	// モードの更新処理
	if (m_pMode != NULL)
	{
		m_pMode->Update();
	}

	// フェードの更新処理
	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}

    // カメラの更新処理
    if (m_pCamera != NULL)
    {
        // ゲーム中なら
        if (typeid(*m_pMode) == typeid(CGame))
        {
            m_pCamera->Update();
        }
    }
}

//========================================
// 生成の管理の描画処理
//========================================
void CManager::Draw(void)
{
	// 描画処理
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//========================================
// モードの取得
//========================================
CManager::MODE CManager::GetMode(void)
{
	// 返り値に使うモード
	MODE mode = MODE_NONE;

	// 何のモードが入っているか見て、型名ごとに場合分け
	if (typeid(*m_pMode) == typeid(CTitle))
	{
		mode = MODE_TITLE;
	}
	else if (typeid(*m_pMode) == typeid(CGame))
	{
		mode = MODE_GAME;
	}

	return mode;
}

//========================================
// モードの設定
// mode	:	設定するモード
//========================================
void CManager::SetMode(MODE mode)
{
	// モードの中身があるなら
	if (m_pMode != NULL)
	{
		// 終了処理
		m_pMode->Uninit();

		// メモリの破棄
		delete m_pMode;
		m_pMode = NULL;
	}

	// シーン上のオブジェクトを全て終了処理
	CScene::ReleaseAll();

	// 受け取ったモードに画面遷移
	switch (mode)
	{
	case MODE_TITLE:
		m_pMode = new CTitle;
		break;

	case MODE_GAME:
		m_pMode = new CGame;
		break;
	}

	// 確保したモードの初期化
	m_pMode->Init();
}

//========================================
// マッチングのスタート
// bMatching	:	マッチングをスタートするフラグ
//========================================
void CManager::SetStartMatching(bool bMatching)
{
	m_bStartMatching = bMatching;

	if (m_bStartMatching)
	{
		//スレッド生成
		std::thread Connection(ConnectionThread);
		// スレッドを切断
		Connection.detach();
	}
}

//========================================
// 接続スレッドの処理
//========================================
void CManager::ConnectionThread(void)
{
	WSADATA wsaData;
	char aDestIPAddr[128] = IP_ADDRESS;
	char aRecvBuff[256];
	char aSendBuff[256];
	int nPlayerUseCard;
	int nEnemyUseCard;
	int nTypeCard;

	// 受信用変数を0クリア
	memset(aRecvBuff, 0, sizeof(aRecvBuff));
	memset(aSendBuff, 0, sizeof(aSendBuff));

	// winsock初期化処理
	int nErr = WSAStartup(WINSOCK_VERSION, &wsaData);

	//CTcpClientを生成して、サーバーに接続
	CTcpClient *pTcpClient = NULL;

	//インスタンス生成
	pTcpClient = CTcpClient::Create(aDestIPAddr, SERVER_PORT_NUM);

	// 接続できたら受信
	pTcpClient->Recv(aRecvBuff, sizeof(aRecvBuff));

	// 受信した先攻か後攻かの情報を受け取る
	char aRecvAttack;

	// 受信した先攻後攻の情報をコピー
	memcpy(&aRecvAttack, &aRecvBuff[0], sizeof(aRecvAttack));

	//// エンディアン変換
	//m_nAttackOrder = ntohl(m_nAttackOrder);

	// 先攻後攻の情報を取得
	m_nAttackOrder = (int)aRecvAttack;

	// マッチングできたかどうか
	if (pTcpClient != NULL)
	{
		// できた
		m_bMatching = true;
	}
	else
	{
		// できてない
		m_bMatching = false;
	}

	// 後攻なら防御ターンをtrueにする
	if (m_nAttackOrder == 2)
	{
		m_bDefenceTurn = true;
	}

	// バトルするためwhile文
	while (1)
	{

		// ゲームが終了したかどうか取得
		m_bGameEnd = CGame::GetFinishBattle();

		if (!m_bGameEnd)
		{
			// 攻撃ターン
			if (m_bAttackTurn)
			{
				// 初期化
				memset(aSendBuff, 0, sizeof(aSendBuff));


				// 自分が使用した攻撃カードを変数に受け取る
				nPlayerUseCard = m_nPlayerUseCard;


				// エンディアン変換
				nPlayerUseCard = htonl(nPlayerUseCard);


				// 自分が使用したカード情報をaSendBuffにコピー
				memcpy(&aSendBuff[0], &nPlayerUseCard, sizeof(aSendBuff));


				// send(自分が使用したカード情報をサーバーに送る)
				pTcpClient->Send(aSendBuff, sizeof(int));


				//中身を初期化
				memset(aRecvBuff, 0, sizeof(aRecvBuff));


				//recv(相手の防御カードを待つ)
				if (pTcpClient->Recv(aRecvBuff, sizeof(aRecvBuff)) <= 0)
				{// 切断された場合

					m_bDisconnect = true;

				}


				// 敵が使ったカード番号を取得
				nTypeCard = *(int*)&aRecvBuff[0];


				// エンディアン変換
				nTypeCard = ntohl(nTypeCard);

				// 相手が使用したカード情報を送る
				// nTypeCard: 相手が使用した防御カードの情報
				// TURN: 自分の今のターンを入れる
				RecvCard(nTypeCard, TURN_ATTACK);

				//=======================================
				// プレイヤーの攻撃演出
                CGame::SetState(CGame::STATE_PLAYER_ATTACK);
				//=======================================
			}
			// 防御ターン
			else if (m_bDefenceTurn)
			{

				// 中身を初期化
				memset(aRecvBuff, 0, sizeof(aRecvBuff));

				// 相手のカードを受け取れていない
				if (!m_bRecvCard)
				{
					// recv(相手の攻撃カードを待つ)
					if (pTcpClient->Recv(aRecvBuff, sizeof(aRecvBuff)) <= 0)
					{// 切断された場合
						m_bDisconnect = true;
					}

					// 敵が使ったカード情報を取得
					nEnemyUseCard = *(int*)&aRecvBuff[0];


					// エンディアン変換
					nEnemyUseCard = ntohl(nEnemyUseCard);


					// 相手が使用したカードを取得
					RecvCard(nEnemyUseCard, TURN_DEFENCE);


					// 相手からカードを受け取れたのでtrueにする
					m_bRecvCard = true;

					//====================================================
					// 防御開始
					CGame::SetDefenceTurn(true);
					//====================================================
				}

				// 防御カードを使用したなら
				if (m_bDefenceCardUse)
				{

					// 自分が使用したカードを変数に入れる
					nPlayerUseCard = m_nPlayerUseCard;


					// エンディアン変換
					nPlayerUseCard = htonl(nPlayerUseCard);


					// 使うカード情報をコピー
					memcpy(&aSendBuff[0], &nPlayerUseCard, sizeof(aSendBuff));


					//send(自分が使用した防御カードを送る)
					pTcpClient->Send(aSendBuff, sizeof(int));

					// 防御ターンを終える
					CGame::SetDefenceTurn(false);

					m_bDefenceCardUse = false;
					m_bRecvCard = false;

					// 防御ターンが終了
					m_bDefenceTurn = false;

					//=======================================
					// エネミーの攻撃演出
                    CGame::SetState(CGame::STATE_ENEMY_ATTACK);
					//=======================================
				}

			}

			// 切断された場合
			if (m_bDisconnect)
			{
				// While文を抜ける
				break;
			}
		}
		// ゲームが終了した場合
		else
		{
			// While文を抜ける
			break;
		}
	}

	// サーバーとの接続を切る
	pTcpClient->Release();
}

//========================================
// カードを受け取る処理
//========================================
void CManager::RecvCard(int nTypeCard, TURN Turn)
{
	
	switch (Turn)
	{
		// 攻撃ターンの場合
	case TURN_ATTACK:

		// 攻撃ターン終了
		m_bAttackTurn = false;

		// 攻撃ターンが終了したので防御ターン
		m_bDefenceTurn = true;

		// 取得したカードを保存する
		m_nEnemyUseCard = nTypeCard;

        // 敵が使用したカードをゲームに結びつける
        CGame::SetEnemyUseCard(nTypeCard);

		break;

		// 防御ターンの場合
	case TURN_DEFENCE:

		// 敵の使用したカードを取得
		m_nEnemyUseCard = nTypeCard;

        // 敵が使用したカードをゲームに結びつける
        CGame::SetEnemyUseCard(nTypeCard);

		break;
	}

}

void CManager::SendCard(int nType)
{
	//***************************************************************
	// 一番下の武器カードかつ一番最後の武器カード、または祈る
	//***************************************************************
	// 攻撃カードなら
	if (nType >= CCard::TYPE_WEAPON_000 && nType <= CCard::TYPE_WEAPON_FINAL || nType == PRAY_ID)
	{
		// 使用したカードを取得
		m_nPlayerUseCard = nType;

        // 自分が使用したカードをゲームに結びつける
        CGame::SetPlayerUseCard(nType);

		// 攻撃する
		m_bAttackTurn = true;

	}

	//***************************************************************
	// 一番下の防具カードかつ一番最後の防御カード、または許す
	//***************************************************************
	// 防御カードなら
	else if (nType >= CCard::TYPE_ARMOR_000 && nType <= CCard::TYPE_ARMOR_FINAL || nType == FORGIVE_ID)
	{

		// 使用したカードを取得
		m_nPlayerUseCard = nType;

        // 自分が使用したカードをゲームに結びつける
        CGame::SetPlayerUseCard(nType);

		// 防御カードを使用
		m_bDefenceCardUse = true;
	}

}