//=============================================================================
//
// 最強昔話決定戦の処理 [stage.cpp](ステージをグリッドで管理)
// Author:KUDO AYANO
//
//=============================================================================
#include "stage.h"
#include "block.h"
#include "setting.h"

//*****************************************************************************
//グローバル変数
//*****************************************************************************
OBJECT_PLACED_INFO objectPlacedInfo[MAX_GRID];    //グリッドの数分の情報

//=============================================================================
// [InitStage] ステージ情報の初期化
// Author:KUDO AYANO
//=============================================================================
void InitStage(void)
{
    for (int nCntstage = 0; nCntstage < MAX_GRID; nCntstage++)
    {
        objectPlacedInfo[nCntstage].isPlaced = false;
    }
}

//=============================================================================
// [ConversionGrid] グリット上の座標をもとめる
// Author:KUDO AYANO
// [in / out] pos : 現在座標を受け取り、グリット座標に変換して渡す
//=============================================================================
D3DXVECTOR3 ConversionGrid(D3DXVECTOR3* pos)
{
    D3DXVECTOR3 posGrid = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

    // 一旦座標を10で割る
    float conversionX = pos->x / GRID_SIZE;
    float conversionY = pos->y / GRID_SIZE;
    float conversionZ = pos->z / GRID_SIZE;

    // 座標を四捨五入する
    conversionX = (float)round(conversionX);
    conversionZ = (float)round(conversionZ);
    // y軸は切り捨て
    conversionY = (float)floor(conversionY);

    posGrid = D3DXVECTOR3((float)conversionX * GRID_SIZE,
        (float)conversionY * GRID_SIZE,
        (float)conversionZ * GRID_SIZE);

    return posGrid;
}

//=============================================================================
// [ConfirmationGrid] オブジェクトが置かれているかどうかをか確認
// Author:KUDO AYANO
// [in] pos : 調べたい座標
// 返り値：座標の位置にオブジェクトが置かれているかどうかを返す
//=============================================================================
bool ConfirmationGrid(D3DXVECTOR3 pos)
{
    D3DXVECTOR3 Grid = D3DXVECTOR3(pos.x / GRID_SIZE, pos.y / GRID_SIZE, pos.z / GRID_SIZE);
    return objectPlacedInfo[(int)Grid.z * MESH_BLOCK_X + (int)Grid.x].isPlaced;
}

//=============================================================================
// [SetGridInfo] ステージのマス上の配置情報を変更
// Author:KUDO AYANO
// [in] pos    : オブジェクトの座標
// [Placed]    : 変更後の配置情報
//=============================================================================
void SetGridInfo(D3DXVECTOR3 pos, bool Placed)
{
    //設定の情報取得
    SETTING *pSetting = GetSetting();

    //みんなで遊ぶなら
    if (pSetting->nMaxPlayer >= 2)
    {
        D3DXVECTOR3 Grid = D3DXVECTOR3(pos.x / GRID_SIZE, pos.y / GRID_SIZE, pos.z / GRID_SIZE);
        int GridNum = (int)Grid.z * MESH_BLOCK_X + (int)Grid.x;
        objectPlacedInfo[GridNum].isPlaced = Placed;
    }
}

//=============================================================================
// [InputStageData] ファイルの読み込み
// Author:KUDO AYANO
//=============================================================================
void InputStageData(void)
{
    char cRead[MAX_BLOCK];	//文字読み取り用
    char cHead[MAX_BLOCK];	//比較用
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    BLOCKTYPE type = BLOCKTYPE_000;
    FILE *pFile = NULL;//ファイルオープン

    //設定の情報取得
    SETTING *pSetting = GetSetting();

    //みんなで遊ぶなら
    if (pSetting->nMaxPlayer != 1)
    {
        //ノーマルステージ読み込み
        if (pSetting->nStage == SETTINGSTAGE_NORMAL)
        {
            pFile = fopen("TXT/stage.txt", "r");//ファイルオープン
        }

        //平原ステージ読み込み
        else if (pSetting->nStage == SETTINGSTAGE_PLAIN)
        {
            pFile = fopen("TXT/stage2.txt", "r");//ファイルオープン
        }

        //城跡ステージ読み込み
        else if (pSetting->nStage == SETTINGSTAGE_RUIN)
        {
            pFile = fopen("TXT/stage3.txt", "r");//ファイルオープン
        }

        //クロスステージ読み込み
        else if (pSetting->nStage == SETTINGSTAGE_CROSS)
        {
            pFile = fopen("TXT/stage4.txt", "r");//ファイルオープン
        }

        //ランダムステージ読み込み
        else if (pSetting->nStage == SETTINGSTAGE_RANDOM)
        {
            int nRandom = 0;

            nRandom = rand() % 4 + 1;    //1～4を得る

            if (nRandom == 1)
            {
                pFile = fopen("TXT/stage.txt", "r");//ファイルオープン
            }
            else if (nRandom == 2)
            {
                pFile = fopen("TXT/stage2.txt", "r");//ファイルオープン
            }
            else if (nRandom == 3)
            {
                pFile = fopen("TXT/stage3.txt", "r");//ファイルオープン
            }
            else if (nRandom == 4)
            {
                pFile = fopen("TXT/stage4.txt", "r");//ファイルオープン
            }
        }
    }

    //ひとりで遊ぶなら
    else
    {
        pFile = fopen("TXT/story.txt", "r");//ファイルオープン
    }

    if (pFile == NULL)
    {
        return;//ファイルを開けなかったら終了
    }
    while (strcmp(cHead, "STAGE_DATA"))//STAGE_DATAを読み込むまでループ
    {
        fgets(cRead, sizeof(cRead), pFile);
        sscanf(cRead, "%s", &cHead);//読み込んだテキストを比較用変数に代入
    }
    if (strcmp(cHead, "STAGE_DATA") == 0)//SCRIPTを読み込んだ時
    {
        while (strcmp(cHead, "END_STAGE_DATA") != 0)//END_STAGE_DATAを読み込むまでループ
        {
            fgets(cRead, sizeof(cRead), pFile);//位置行単位で文字列を読み込み
            sscanf(cRead, "%s", &cHead);//読み込んだテキストを比較用変数に代入
            sscanf(cRead, "%g,%g,%g,%d", &pos.x, &pos.y, &pos.z, &type);

            //ブロックを配置
            SetBlock(pos, type);
        }
        fclose(pFile);//ファイルを閉じる
    }
}