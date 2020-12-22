//===============================================
//
// カーソル処理 (cursor.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "cursor.h"
#include "manager.h"
#include "input.h"
#include "renderer.h"
#include "library.h"
#include "sound.h"
#include "game.h"

//=======================================
// 静的メンバ変数宣言
//=======================================
LPDIRECT3DTEXTURE9 CCursor::m_pTexture = NULL;

//========================================
// カーソルのコンストラクタ
//========================================
CCursor::CCursor() :CScene2D(OBJTYPE::OBJTYPE_CURSOR)
{

}

//========================================
// カーソルのデストラクタ
//========================================
CCursor::~CCursor()
{

}

//========================================
// カーソルの初期化処理
//========================================
HRESULT CCursor::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_pTexture);

    return S_OK;
}

//========================================
// カーソルの終了処理
//========================================
void CCursor::Uninit(void)
{
    // 終了処理
    CScene2D::Uninit();
}

//========================================
// カーソルの更新処理
//========================================
void CCursor::Update(void)
{
    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 変数宣言
    D3DXVECTOR3 pos;    // 位置
    D3DXVECTOR3 size;   // サイズ

    // 位置を取得
    pos = GetPosition();

    // サイズを取得
    size = GetSize();

    // カーソルの移動操作
    pos = ControlCursor(pos);

    // 画面外に出ないようにする
    SteyInScreen2D(&pos, &size);

    // 位置を反映
    SetPosition(pos);

    // 頂点座標を変更
    SetVertex();

    // 戦闘終了時、消す
    if (CGame::GetFinishBattle() == true)
    {
        Uninit();
    }
}

//========================================
// カーソルの描画処理
//========================================
void CCursor::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//========================================
// カーソルの移動操作の細かな関数
//========================================
D3DXVECTOR3 CCursor::ControlCursor(D3DXVECTOR3 pos)
{
    // 変数宣言
    D3DXVECTOR3 movePos = pos;      // 移動の位置

    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    if (Controller.lY != 0 || Controller.lX != 0)
    {// スティックが倒れているなら移動

     // スティックの角度を求める
        float fAngle = atan2(Controller.lX, Controller.lY * (-1));

        movePos.x += sinf(fAngle)* CURSOR_SPEED;
        movePos.y += -cosf(fAngle)* CURSOR_SPEED;
    }
    else
    {// キーボード操作

     // Aキーが押された(左移動)
        if (pInputKeyboard->GetKeyboardPress(DIK_A))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))// AとWが同時に押された
            {
                movePos.x -= sinf(D3DX_PI / 4) * CURSOR_SPEED;
                movePos.y -= cosf(D3DX_PI / 4) * CURSOR_SPEED;
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))// AとSが同時に押された
            {
                movePos.x -= sinf(D3DX_PI / 4) * CURSOR_SPEED;
                movePos.y += cosf(D3DX_PI / 4) * CURSOR_SPEED;
            }
            else				// 左だけ押した
            {
                movePos.x -= CURSOR_SPEED;
            }

        }

        // Dキーが押された(右移動)
        else if (pInputKeyboard->GetKeyboardPress(DIK_D))
        {
            if (pInputKeyboard->GetKeyboardPress(DIK_W))// DとWが同時に押された
            {
                movePos.x += sinf(D3DX_PI / 4) * CURSOR_SPEED;
                movePos.y -= cosf(D3DX_PI / 4) * CURSOR_SPEED;
            }
            else if (pInputKeyboard->GetKeyboardPress(DIK_S))// DとSが同時に押された
            {
                movePos.x += sinf(D3DX_PI / 4) * CURSOR_SPEED;
                movePos.y += cosf(D3DX_PI / 4) * CURSOR_SPEED;
            }
            else				// 右だけ押した
            {
                movePos.x += CURSOR_SPEED;
            }
        }

        // Wキーが押された(上移動)
        else if (pInputKeyboard->GetKeyboardPress(DIK_W))
        {
            movePos.y -= CURSOR_SPEED;
        }

        // Sキーが押された(下移動)
        else if (pInputKeyboard->GetKeyboardPress(DIK_S))
        {
            movePos.y += CURSOR_SPEED;
        }
    }

    return movePos;
}

//========================================
// カーソルの画像ロード処理
//========================================
HRESULT CCursor::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/cursor000.png", &m_pTexture);

    return S_OK;
}

//========================================
// カーソルの画像破棄処理
//========================================
void CCursor::Unload(void)
{
    // テクスチャの破棄
    if (m_pTexture != NULL)
    {
        m_pTexture->Release();
        m_pTexture = NULL;
    }
}

//================================
// カーソルの作成
//================================
CCursor *CCursor::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    CCursor *pCursor = NULL;

    // メモリを確保
    // コンストラクタで各情報を紐づけ
    pCursor = new CCursor;

    // 初期化
    pCursor->Init(pos, size);

    return pCursor;
}