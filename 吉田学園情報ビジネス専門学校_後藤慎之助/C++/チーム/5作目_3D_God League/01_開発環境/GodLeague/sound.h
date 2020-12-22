//====================================================================
//
// サウンド処理 (sound.h)
// Author : 後藤慎之助
//
//====================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

//================================================
// インクルードファイル
//================================================
#include "main.h"

//================================================
// クラス宣言
//================================================

// サウンドのクラス
class CSound
{
public:
    typedef enum
    {
        LABEL_BGM_TITLE = 0,	    // BGM タイトル
        LABEL_BGM_GAME,             // BGM ゲーム
        LABEL_SE_000,               // SE タイトル画面決定音
        LABEL_SE_001,               // SE マッチング音
        LABEL_SE_002,               // SE 斬撃小
        LABEL_SE_003,               // SE 斬撃大
        LABEL_SE_004,               // SE 完全防御音
        LABEL_SE_005,               // SE 歓声(ループ)
        LABEL_SE_006,               // SE ゴング1回
        LABEL_SE_007,               // SE ゴング複数
        LABEL_SE_008,               // SE カード決定音
        LABEL_MAX,
    } LABEL;
    CSound();
    ~CSound();  // 親クラスのデストラクタにvirtualをつけることで、メモリリークを抑える
    HRESULT Init(HWND hWnd);
    void Uninit(void);
    HRESULT Play(LABEL label);
    void Stop(LABEL label);
    void StopAll(void);

private:
    typedef struct
    {
        char *pFilename;	// ファイル名
        int nCntLoop;		// ループカウント (-1でループ、0でループなし)
    }PARAM;
    HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
    HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

    IXAudio2 *m_pXAudio2;								// XAudio2オブジェクトへのインターフェイス
    IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイス
    IXAudio2SourceVoice *m_apSourceVoice[LABEL_MAX];	// ソースボイス
    BYTE *m_apDataAudio[LABEL_MAX];					    // オーディオデータ
    DWORD m_aSizeAudio[LABEL_MAX];					    // オーディオデータサイズ
    static PARAM m_aParam[LABEL_MAX];                   // 各音素材のパラメータ
};

#endif