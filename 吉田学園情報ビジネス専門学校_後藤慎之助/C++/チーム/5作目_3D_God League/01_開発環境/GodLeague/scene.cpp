//===============================================
//
// シーン上のオブジェクト処理 (scene.cpp)
// Author : 後藤慎之助
//
//===============================================

//========================
// インクルードファイル
//========================
#include "scene.h"
#include "renderer.h"

//========================================
// 静的メンバ変数宣言
//========================================
int CScene::m_nNumAll = 0;
CScene *CScene::m_apScene[MAX_OBJECT] = {};

//========================================
// シーン上のオブジェクトのデフォルトコンストラクタ
//========================================
CScene::CScene()
{
    for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
    {
        // 空なら
        if (m_apScene[nCntObject] == NULL)
        {
            // 配列に入れる
            m_apScene[nCntObject] = this;    // staticなので、0初期化されている、thisはCreateのreturnを受け取る

                                             // 増えたオブジェクトをカウント
            m_nNumAll++;

            // オブジェクトのIDを保存
            m_nID = nCntObject;

            // オブジェクトの種類を初期化
            m_objType = OBJTYPE_NULL;

            // 空いているメモリ番地を取得したらforを抜ける
            break;
        }
    }
}

//========================================
// シーン上のオブジェクトのオーバーライドされたコンストラクタ
//========================================
CScene::CScene(OBJTYPE objType)
{
    for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
    {
        // 空なら
        if (m_apScene[nCntObject] == NULL)
        {
            // 配列に入れる
            m_apScene[nCntObject] = this;    // staticなので、0初期化されている、thisはCreateのreturnを受け取る

                                             // 増えたオブジェクトをカウント
            m_nNumAll++;

            // オブジェクトのIDを保存
            m_nID = nCntObject;

            // オブジェクトの種類を保存
            m_objType = objType;

            // 空いているメモリ番地を取得したらforを抜ける
            break;
        }
    }
}

//========================================
// シーン上のオブジェクトのデストラクタ
//========================================
CScene::~CScene()
{

}

//==============================================
// シーン上のオブジェクトを全て更新
//==============================================
void CScene::UpdateAll(void)
{
    for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
    {
        // 中身があるなら
        if (m_apScene[nCntObject] != NULL)
        {
            // 更新処理
            m_apScene[nCntObject]->Update();
        }
    }
}

//==============================================
// シーン上のオブジェクトを全て描画
//==============================================
void CScene::DrawAll(void)
{
    // 変数宣言
    CScene *apScene[MAX_OBJECT];   // 描画の順番を変えるため、配列の情報を記憶
    CScene *pScene;                 // ソート時に、若い番号の値を記憶

    // 配列の初期化
    for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
    {
        // シーン上のオブジェクトの情報を記憶
        apScene[nCntObject] = m_apScene[nCntObject];
    }

    // 描画順をソート(選択法)を使って、タイプ順に並び替える（昇順）
    for (int nNum01 = 0; nNum01 < MAX_OBJECT - 1; nNum01++)
    {
        for (int nNum02 = nNum01 + 1; nNum02 < MAX_OBJECT; nNum02++)
        {
            // どちらも中身があるなら
            if (apScene[nNum01] != NULL && apScene[nNum02] != NULL)
            {
                // 若い番号のタイプのほうが大きいなら
                if (apScene[nNum01]->m_objType > apScene[nNum02]->m_objType)
                {
                    // 若い番号を記憶
                    pScene = apScene[nNum01];

                    // 若い番号に次の番号を代入
                    apScene[nNum01] = apScene[nNum02];

                    // 次の番号に記憶したものを代入
                    apScene[nNum02] = pScene;
                }
            }
        }
    }

    // デバッグ用
    int nCntAll = 0;    // シーン上のすべてのポリゴンの数を確認

    // 描画
    for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
    {
        // 中身があるなら
        if (apScene[nCntObject] != NULL)
        {
            // 描画処理
            apScene[nCntObject]->Draw();
        }
        else
        {
            nCntAll = nCntObject;
        }
    }
}

//==============================================
// シーン上のオブジェクトを全て開放
//==============================================
void CScene::ReleaseAll(void)
{
    // 終了処理
    for (int nCntData = 0; nCntData < MAX_OBJECT; nCntData++)
    {
        if (m_apScene[nCntData] != NULL)
        {
            m_apScene[nCntData]->Uninit();
        }
    }
}

//==============================================
// シーン上のオブジェクトを取得
//==============================================
CScene * CScene::GetScene(int nCntScene)
{
    if (m_apScene[nCntScene] == NULL)
    {
        return NULL;
    }

    return m_apScene[nCntScene];
}

//==============================================
// シーン上のオブジェクトを全て開放
//==============================================
void CScene::Release(void)
{
    // 中身があるなら、
    if (m_apScene[m_nID] != NULL)
    {
        // インスタンスごとに持っているIDをdeleteした後も使うので保存
        int nID = m_nID;

        // メモリの開放
        delete m_apScene[nID];
        m_apScene[nID] = NULL;

        // 総数を減らす
        m_nNumAll--;
    }
}
