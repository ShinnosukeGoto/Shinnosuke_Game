//===================================================================
//
// カード処理 (card.cpp)
// Author : 後藤慎之助
//
//===================================================================

//===================================================================
// インクルードファイル
//===================================================================
#include "card.h"
#include "manager.h"
#include "renderer.h"
#include "library.h"
#include "game.h"
#include "input.h"
#include "card_info.h"
#include "card_name.h"
#include "sound.h"

//===================================================================
// マクロ定義
//===================================================================
#define INACTIVE_COLOR D3DXCOLOR(0.431f, 0.431f, 0.388f, 1.0f)                       // 非アクティブ色

///==================================================================
// 静的メンバ変数宣言
//===================================================================
LPDIRECT3DTEXTURE9 CCard::m_apTexture[CCard::TYPE_MAX] = {};
int CCard::m_nNumAll = 0;
bool CCard::m_bDispDefenceCard = false;
bool CCard::m_bDispAttackCard = false;

//===================================================================
// カードのコンストラクタ
//===================================================================
CCard::CCard() :CScene2D(OBJTYPE::OBJTYPE_CARD)
{
    m_posDest = DEFAULT_VECTOR;
    m_nType = 0;
    m_bUse = true;  // 使用中
    m_nMoveTime = 0;

    m_nAttackValue = 0;
    m_nDefenceValue = 0;
}

//===================================================================
// カードのデストラクタ
//===================================================================
CCard::~CCard()
{

}

//===================================================================
// カードの初期化処理
//===================================================================
HRESULT CCard::Init(D3DXVECTOR3 pos, D3DXVECTOR3 size)
{
    // 初期化
    CScene2D::Init(pos, size);

    // テクスチャを割り当て
    BindTexture(m_apTexture[m_nType]);

    return S_OK;
}

//===================================================================
// カードの終了処理
//===================================================================
void CCard::Uninit(void)
{
    // 手札の最大数を更新
    m_nNumAll--;

    // 終了処理
    CScene2D::Uninit();
}

//===================================================================
// カードの更新処理
//===================================================================
void CCard::Update(void)
{
    // 変数宣言
    D3DXVECTOR3 pos = GetPosition();    // 位置を取得
    D3DXVECTOR3 size = GetSize();       // サイズを取得

    // 攻撃ターンなら
    if (CGame::GetAttackTurn() == true)
    {
        // 攻撃力のあるカードをアクティブに
        if (m_nAttackValue > 0)
        {
            SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        }
        // ないカードは、非アクティブに
        else
        {
            SetColor(INACTIVE_COLOR);
        }
    }

    // 防御ターンなら
    if (CGame::GetDefenceTurn() == true)
    {
        // 防御力のあるカードをアクティブに
        if (m_nDefenceValue > 0)
        {
            SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
        }
        // ないカードは、非アクティブに
        else
        {
            SetColor(INACTIVE_COLOR);
        }
    }

    // 攻撃ターンでも、防御ターンでもないなら、どのカードもアクティブに
    if (CGame::GetAttackTurn() == false && CGame::GetDefenceTurn() == false)
    {
        SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // 目的の位置に移動
    pos = Move(pos);

    // カーソルとの当たり判定
    size = CollisionCursor(pos, size);

    // 位置を反映
    SetPosition(pos);

    // 大きさを反映
    SetSize(size);

    // 頂点座標を変更
    SetVertex();

    // 使用フラグがfalse、またはゲーム終了したなら消す
    if (m_bUse == false || CGame::GetFinishBattle() == true)
    {
        Uninit();
    }
}

//===================================================================
// カードの描画処理
//===================================================================
void CCard::Draw(void)
{
    // 描画処理
    CScene2D::Draw();
}

//===================================================================
// カードの画像ロード処理
//===================================================================
HRESULT CCard::Load(void)
{
    // レンダラーからデバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

    // テクスチャの読み込み
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_01_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_000]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_02_Hood.png", &m_apTexture[CCard::TYPE_ARMOR_001]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_03_Mail.png", &m_apTexture[CCard::TYPE_ARMOR_002]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_05_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_003]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_06_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_004]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_07_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_005]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_08_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_006]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_09_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_007]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/ARMOR/Guard_04_Shield.png", &m_apTexture[CCard::TYPE_ARMOR_FINAL]);

    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_01_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_000]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_02_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_001]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_03_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_002]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_05_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_003]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_06_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_004]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_07_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_005]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_08_club.png", &m_apTexture[CCard::TYPE_WEAPON_006]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_09_axe.png", &m_apTexture[CCard::TYPE_WEAPON_007]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_10_whip.png", &m_apTexture[CCard::TYPE_WEAPON_008]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_11_boomerang.png", &m_apTexture[CCard::TYPE_WEAPON_009]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_12_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_010]);
    D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/CARD/WEAPON/Attack_04_Sword.png", &m_apTexture[CCard::TYPE_WEAPON_FINAL]);

    return S_OK;
}

//===================================================================
// カードの画像破棄処理
//===================================================================
void CCard::Unload(void)
{
    // テクスチャの破棄
    for (int nCntTexture = 0; nCntTexture < CCard::TYPE_MAX; nCntTexture++)
    {
        if (m_apTexture[nCntTexture] != NULL)
        {
            m_apTexture[nCntTexture]->Release();
            m_apTexture[nCntTexture] = NULL;
        }
    }
}

//===================================================================
// カードの作成
//===================================================================
CCard *CCard::Create(D3DXVECTOR3 pos, D3DXVECTOR3 size, int type)
{
    CCard *pCard = NULL;

    // メモリを確保
    pCard = new CCard;

    // タイプのみ、テクスチャ割り当てのために結びつけておく
    pCard->m_nType = type;

    // 初期化
    pCard->Init(pos, size);

    // メンバ変数の初期化
    pCard->m_posDest = pos;

    // 攻撃力、防御力を取得
    STATUS status;
    status = List(type);
    pCard->m_nAttackValue = status.nAttack;
    pCard->m_nDefenceValue = status.nDefence;

    return pCard;
}

//===================================================================
// カーソルとの当たり判定
//===================================================================
D3DXVECTOR3 CCard::CollisionCursor(D3DXVECTOR3 pos, D3DXVECTOR3 size)
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

                // カードの当たり判定のサイズを設定
                D3DXVECTOR3 cardCollisionSize = CARD_COLLISION_SIZE;

                // 当たり判定を計算
                if (RectangleCollision2D(&pos, &pScene2D->GetPosition(),
                    &cardCollisionSize, &pScene2D->GetSize()) == true)
                {// 当たったなら

                    // カード情報を更新
                    SetCardInfo();

                    // 拡大したサイズを返す
                    returnSize = CARD_EXTEND_SIZE;

                    // カードの選択が可能
                    SelectCard();

                    // カーソルは一つなのでforを抜ける
                    break;
                }
                else
                {// 当たってないなら

                    // 普通のサイズを返す
                    returnSize = CARD_SIZE;

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
void CCard::SetCardInfo(void)
{
    // カードの攻撃や防御の数値を設定
    CCardInfo *pCardInfo = CGame::GetCardInfo();
    pCardInfo->SetCardInfo(m_nAttackValue, m_nDefenceValue);

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
                    if (pCardName->GetType() != m_nType)
                    {
                        // 対象を未使用に
                        pCardName->SetDontUse();

                        // 自身の名前を生成
                        CCardName::Create(CARD_NAME_POS, CARD_NAME_SIZE, m_nType, CCardName::USETYPE_PLAYER);
                    }
                }
            }
        }
    }
}

//===================================================================
// 使うカードの選択
//===================================================================
void CCard::SelectCard(void)
{
    // キーボードを取得
    CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();

    // コントローラを取得
    CInputJoypad *pInputJoypad = CManager::GetInputJoypad();
    DIJOYSTATE2 Controller = pInputJoypad->GetController(PLAYER_1);

    // 決定キーを押したら
    if (pInputKeyboard->GetKeyboardTrigger(DIK_RETURN) || pInputJoypad->GetJoypadTrigger(PLAYER_1, CInputJoypad::BUTTON_B))
    {
        //***********************************************************************************
        // 攻撃ターンなら
        //***********************************************************************************
        if (CGame::GetAttackTurn() == true)
        {
            // 攻撃カードのみ
            if (m_nType >= TYPE_WEAPON_000 && m_nType <= TYPE_WEAPON_FINAL)
            {
                // カードを相手に送る
                CManager::SendCard(m_nType);

                // 攻撃ターンを終える
                CGame::SetAttackTurn(false);

                // 消去
                m_bUse = false;

                // カード決定音
                CSound *pSound = CManager::GetSound();
                pSound->Play(CSound::LABEL_SE_008);

                //===============================================================
                // ランダムな値で、カードを得る
                DrawCard(1);
                //===============================================================
            }
        }
        
        //***********************************************************************************
        // 防御ターンなら
        //***********************************************************************************
        if (CGame::GetDefenceTurn() == true)
        {
            // 防御カードのみ
            if (m_nType >= TYPE_ARMOR_000 && m_nType <= TYPE_ARMOR_FINAL)
            {
                // カードを相手に送る
                CManager::SendCard(m_nType);

                // 防御ターンを終える
                CGame::SetDefenceTurn(false);

                // 消去
                m_bUse = false;

                // カード決定音
                CSound *pSound = CManager::GetSound();
                pSound->Play(CSound::LABEL_SE_008);

                //===============================================================
                // ランダムな値で、カードを得る
                DrawCard(1);
                //===============================================================
            }
        }
    }
}

//===================================================================
// カードの並び替え
//===================================================================
void CCard::SortCards(void)
{
    // 変数宣言
    CCard *apCard[MAX_OBJECT];   // カードの位置を変えるため、配列で情報を記憶
    int nIndex = 0;              // 取得とソートのインデックス
    CCard *pCard;                // ソート時に、若い番号の値を記憶用

    // 配列の初期化
    memset(apCard, 0, sizeof(apCard));

    // カードの取得
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // シーンを取得
        CScene *pScene = GetScene(nCntScene);

        // 中身があるなら
        if (pScene != NULL)
        {
            // タイプを取得
            OBJTYPE objType = pScene->GetObjType();

            // カードなら、
            if (objType == CScene::OBJTYPE_CARD)
            {
                // カード型にキャスト
                CCard *pCard = (CCard*)pScene;

                // 配列に記憶
                apCard[nIndex] = pCard;

                // インデックスを一つ進める
                nIndex++;                  
            }
        }
    }

    // カードの順をソート(選択法)を使って、タイプ順に並び替える（昇順）
    for (int nNum01 = 0; nNum01 < nIndex - 1; nNum01++)
    {
        for (int nNum02 = nNum01 + 1; nNum02 < nIndex; nNum02++)
        {
            // どちらも中身があるなら
            if (apCard[nNum01] != NULL && apCard[nNum02] != NULL)
            {
                // 若い番号のタイプのほうが大きいなら
                if (apCard[nNum01]->GetType() > apCard[nNum02]->GetType())
                {
                    // 若い番号を記憶
                    pCard = apCard[nNum01];

                    // 若い番号に次の番号を代入
                    apCard[nNum01] = apCard[nNum02];

                    // 次の番号に記憶したものを代入
                    apCard[nNum02] = pCard;
                }
            }
        }
    }

    // 変数宣言
    int nIndexPos = 0;           // 位置用のインデックス

    // カードを配置していく
    for (int nCnt = 0; nCnt < nIndex; nCnt++)
    {
        // シーン2Dにキャスト
        CScene2D *pScene2D = (CScene2D*)apCard[nCnt];

        // 位置を取得
        D3DXVECTOR3 pos = pScene2D->GetPosition();
        
        // 位置を間隔をあけて計算
        D3DXVECTOR3 spacePos = D3DXVECTOR3(CARD_FIRST_POS_X + CARD_SPACE_X * nIndexPos, CARD_POS_Y, 0.0f);

        // 目的の位置を設定
        apCard[nCnt]->SetPosDest(spacePos);

        // 位置を反映
        pScene2D->SetPosition(spacePos);

        // 頂点座標を変更
        pScene2D->SetVertex();

        // インデックスを一つ進める
        nIndexPos++;
    }
}

//===================================================================
// カードの並び替え(移動する感じ)
//===================================================================
void CCard::SortMoveCards(void)
{
    // 変数宣言
    CCard *apCard[MAX_OBJECT];   // カードの位置を変えるため、配列で情報を記憶
    int nIndex = 0;              // 取得とソートのインデックス
    CCard *pCard;                // ソート時に、若い番号の値を記憶用

    // 配列の初期化
    memset(apCard, 0, sizeof(apCard));

    // カードの取得
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // シーンを取得
        CScene *pScene = GetScene(nCntScene);

        // 中身があるなら
        if (pScene != NULL)
        {
            // タイプを取得
            OBJTYPE objType = pScene->GetObjType();

            // カードなら、
            if (objType == CScene::OBJTYPE_CARD)
            {
                // カード型にキャスト
                CCard *pCard = (CCard*)pScene;

                // 配列に記憶
                apCard[nIndex] = pCard;

                // インデックスを一つ進める
                nIndex++;
            }
        }
    }

    // カードの順をソート(選択法)を使って、タイプ順に並び替える（昇順）
    for (int nNum01 = 0; nNum01 < nIndex - 1; nNum01++)
    {
        for (int nNum02 = nNum01 + 1; nNum02 < nIndex; nNum02++)
        {
            // どちらも中身があるなら
            if (apCard[nNum01] != NULL && apCard[nNum02] != NULL)
            {
                // 若い番号のタイプのほうが大きいなら
                if (apCard[nNum01]->GetType() > apCard[nNum02]->GetType())
                {
                    // 若い番号を記憶
                    pCard = apCard[nNum01];

                    // 若い番号に次の番号を代入
                    apCard[nNum01] = apCard[nNum02];

                    // 次の番号に記憶したものを代入
                    apCard[nNum02] = pCard;
                }
            }
        }
    }

    // 変数宣言
    int nIndexPos = 0;           // 位置用のインデックス

    // カードを配置していく
    for (int nCnt = 0; nCnt < nIndex; nCnt++)
    {
        // 位置を取得
        D3DXVECTOR3 pos = apCard[nCnt]->GetPosition();

        // 位置を間隔をあけて計算
        D3DXVECTOR3 spacePos = D3DXVECTOR3(CARD_FIRST_POS_X + CARD_SPACE_X * nIndexPos, CARD_POS_Y, 0.0f);

        // 目的の位置を設定
        apCard[nCnt]->SetPosDest(spacePos);

        // インデックスを一つ進める
        nIndexPos++;
    }
}

//===================================================================
// カードの移動
//===================================================================
D3DXVECTOR3 CCard::Move(D3DXVECTOR3 pos)
{
    // 変数宣言
    D3DXVECTOR3 posMove = pos;  // 移動の位置

    float fTargetPosX = 0.0f;                  // 目標のX座標
    float fTargetPosY = 0.0f;                  // 目標のY座標
    float fMyPosX = 0.0f;                      // 自身のX座標
    float fMyPosY = 0.0f;                      // 自身のY座標
    float fAngle = 0.0f;                       // 求める角度

    // 目標の座標と自身の座標
    fTargetPosX = pos.x, fTargetPosY = pos.y;
    fMyPosX = m_posDest.x, fMyPosY = m_posDest.y;

    // 角度を決める
    fAngle = atan2f((fMyPosX - fTargetPosX), (fMyPosY - fTargetPosY));

    // 目標の位置との、現時点での距離
    float fDistance = sqrtf(
        powf((pos.x - m_posDest.x), 2) +
        powf((pos.y - m_posDest.y), 2));

    // 目的の位置と近いかどうか
    if (fDistance <= CARD_CONSIDER_CLOSE_DEST)
    {
        // 近いなら目的の位置にする
        posMove = m_posDest;

        // 移動のカウンタをリセット
        m_nMoveTime = 0;
    }
    else
    {
        // カードのスピードを距離とソート時間から求める
        float fSpeed = fDistance / (CARD_SORT_TIME - (float)m_nMoveTime);

        // 遠いなら近づけていく
        posMove = D3DXVECTOR3(pos.x + sinf(fAngle)*fSpeed, pos.y + cosf(fAngle)*fSpeed, 0.0f);

        // 移動の経過時間
        m_nMoveTime++;
    }

    return posMove;
}

//===================================================================
// カードリスト
//===================================================================
CCard::STATUS CCard::List(int nType)
{
    // 変数宣言
    STATUS status;          // ステータス
    status.nAttack = 0;     // 攻撃力
    status.nDefence = 0;    // 防御力

     //***********************************************************************************
    // カードごとに、固定の値を結びつける
    switch (nType)
    {
        //==========================================================
        // 防具
    case TYPE_ARMOR_000:
        status.nDefence = CARD_ARMOR_000;
        break;
    case TYPE_ARMOR_001:
        status.nDefence = CARD_ARMOR_001;
        break;
    case TYPE_ARMOR_002:
        status.nDefence = CARD_ARMOR_002;
        break;
    case TYPE_ARMOR_003:
        status.nDefence = CARD_ARMOR_003;
        break;
    case TYPE_ARMOR_004:
        status.nDefence = CARD_ARMOR_004;
        break;
    case TYPE_ARMOR_005:
        status.nDefence = CARD_ARMOR_005;
        break;
    case TYPE_ARMOR_006:
        status.nDefence = CARD_ARMOR_006;
        break;
    case TYPE_ARMOR_007:
        status.nDefence = CARD_ARMOR_007;
        break;
    case TYPE_ARMOR_FINAL:
        status.nDefence = CARD_ARMOR_FINAL;
        break;
        //==========================================================

        //==========================================================
        // 武器
    case TYPE_WEAPON_000:
        status.nAttack = CARD_WEAPON_000;
        break;
    case TYPE_WEAPON_001:
        status.nAttack = CARD_WEAPON_001;
        break;
    case TYPE_WEAPON_002:
        status.nAttack = CARD_WEAPON_002;
        break;
    case TYPE_WEAPON_003:
        status.nAttack = CARD_WEAPON_003;
        break;
    case TYPE_WEAPON_004:
        status.nAttack = CARD_WEAPON_004;
        break;
    case TYPE_WEAPON_005:
        status.nAttack = CARD_WEAPON_005;
        break;
    case TYPE_WEAPON_006:
        status.nAttack = CARD_WEAPON_006;
        break;
    case TYPE_WEAPON_007:
        status.nAttack = CARD_WEAPON_007;
        break;
    case TYPE_WEAPON_008:
        status.nAttack = CARD_WEAPON_008;
        break;
    case TYPE_WEAPON_009:
        status.nAttack = CARD_WEAPON_009;
        break;
    case TYPE_WEAPON_010:
        status.nAttack = CARD_WEAPON_010;
        break;
    case TYPE_WEAPON_FINAL:
        status.nAttack = CARD_WEAPON_FINAL;
        break;
        //==========================================================
    }
    //***********************************************************************************

    return status;
}

//===================================================================
// カードを引く
//===================================================================
void CCard::DrawCard(int nSheets)
{
    // 詰まないように、カードをランダムに生成
    for (int nCnt = 0; nCnt < nSheets; nCnt++)
    {
        // カードが最大数-1なら
        if (m_nNumAll >= MAX_CARD - 1)
        {
            // 攻撃カードがあるかどうかのフラグを更新
            CheckDispAttackCard();

            // 攻撃カードがないなら
            if (m_bDispAttackCard == false)
            {
                // 攻撃カードからランダムに手に入れる
                int nRandNum = GetRandNum(TYPE_WEAPON_FINAL, TYPE_WEAPON_000);
                CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNum);
            }
            // 攻撃カードがあるなら通常の引き
            else
            {
                // 1から100を得る
                int nRandNum = GetRandNum(100, 1);

                // 防具が出る
                if (nRandNum <= ARMOR_PERCENTAGE)
                {
                    // 防御カードからランダムに手に入れる
                    int nRandNumber = GetRandNum(TYPE_ARMOR_FINAL, TYPE_ARMOR_000);
                    CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNumber);
                }
                // 武器が出る
                else
                {
                    // 攻撃カードからランダムに手に入れる
                    int nRandNumber = GetRandNum(TYPE_WEAPON_FINAL, TYPE_WEAPON_000);
                    CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNumber);
                }
            }
        }
        else
        {
            // 1から100を得る
            int nRandNum = GetRandNum(100, 1);

            // 防具が出る
            if (nRandNum <= ARMOR_PERCENTAGE)
            {
                // 防御カードからランダムに手に入れる
                int nRandNumber = GetRandNum(TYPE_ARMOR_FINAL, TYPE_ARMOR_000);
                CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNumber);
            }
            // 武器が出る
            else
            {
                // 攻撃カードからランダムに手に入れる
                int nRandNumber = GetRandNum(TYPE_WEAPON_FINAL, TYPE_WEAPON_000);
                CCard::Create(CARD_CREATE_POS, CARD_SIZE, nRandNumber);
            }
        }

        // 手札の最大数を更新
        m_nNumAll++;
    }
}

//===================================================================
// 防御カードがあるかどうかチェック
//===================================================================
void CCard::CheckDispDefenceCard(void)
{
    // メンバ変数のフラグを一度falseに
    m_bDispDefenceCard = false;

    // カードの取得
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // シーンを取得
        CScene *pScene = GetScene(nCntScene);

        // 中身があるなら
        if (pScene != NULL)
        {
            // タイプを取得
            OBJTYPE objType = pScene->GetObjType();

            // カードなら、
            if (objType == CScene::OBJTYPE_CARD)
            {
                // カード型にキャスト
                CCard *pCard = (CCard*)pScene;

                // 防御カードなら
                if (pCard->GetType() >= CCard::TYPE_ARMOR_000 && pCard->GetType() <= CCard::TYPE_ARMOR_FINAL)
                {
                    // フラグをtrueに
                    m_bDispDefenceCard = true;

                    break;
                }
            }
        }
    }
}

//===================================================================
// 攻撃カードがあるかどうかチェック
//===================================================================
void CCard::CheckDispAttackCard(void)
{
    // メンバ変数のフラグを一度falseに
    m_bDispAttackCard = false;

    // カードの取得
    for (int nCntScene = 0; nCntScene < MAX_OBJECT; nCntScene++)
    {
        // シーンを取得
        CScene *pScene = GetScene(nCntScene);

        // 中身があるなら
        if (pScene != NULL)
        {
            // タイプを取得
            OBJTYPE objType = pScene->GetObjType();

            // カードなら、
            if (objType == CScene::OBJTYPE_CARD)
            {
                // カード型にキャスト
                CCard *pCard = (CCard*)pScene;

                // 攻撃カードなら
                if (pCard->GetType() >= CCard::TYPE_WEAPON_000 && pCard->GetType() <= CCard::TYPE_WEAPON_FINAL)
                {
                    // フラグをtrueに
                    m_bDispAttackCard = true;

                    break;
                }
            }
        }
    }
}
