//=============================================================================
//
// 最強昔話決定戦の処理 [sound.cpp] (サウンド)
// Author:後藤慎之助
//
//=============================================================================
#include "sound.h"

//*****************************************************************************
// パラメータ構造体定義
//*****************************************************************************
typedef struct
{
    char *pFilename;	// ファイル名
    int nCntLoop;		// ループカウント (-1でループ、0でループなし)
} SOUNDPARAM;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ

                                                            // 各音素材のパラメータ
SOUNDPARAM g_aParam[SOUND_LABEL_MAX] =
{
    { "SOUND/BGM_TITLE.wav", -1 },		        // BGM タイトル
    { "SOUND/BGM_STAGE.wav", -1 },		        // BGM メニュー
    { "SOUND/BGM_HUCKER.wav", -1 },		        // BGM ゲーム
    { "SOUND/BGM_SUDDEN_DEATH.wav", -1 },		// BGM サドンデス
    { "SOUND/BGM_RESULT.wav", -1 },		        // BGM リザルト
    { "SOUND/SE_SELECT.wav", 0 },	            // 選択音
    { "SOUND/SE_DECISION.wav", 0 },			    // 決定音
    { "SOUND/SE_EXPLOSION.wav", 0 },	        // 爆発音
    { "SOUND/SE_SET_BOMB.wav", 0 },		        // ボム設置音
    { "SOUND/SE_MAN_VOICE.wav", 0 },			// 男性がやられた時のボイス
    { "SOUND/SE_ITEM.wav", 0 },	                // アイテム取得音
    { "SOUND/SE_START.wav", 0 },			    // 開始音
    { "SOUND/SE_WARNING.wav", 0 },			    // 警告音
    { "SOUND/SE_FINISH.wav", 0 },		        // 試合終了音
    { "SOUND/SE_KICK.wav", 0 },		            // ボムキック音
    { "SOUND/SE_WOMAN_VOICE.wav", 0 },			// 女性がやられた時のボイス
    { "SOUND/SE_PINOCCHIO_VOICE.wav", 0 },		// ピノキオがやられた時のボイス
    { "SOUND/SE_MAN_VOICE2.wav", 0 },			// 男性がやられた時のボイス2
    { "SOUND/SE_START_MAN_VOICE.wav", 0 },		// 開始の掛け声(男)
    { "SOUND/SE_START_MAN_VOICE2.wav", 0 },		// 開始の掛け声(男2)
    { "SOUND/SE_START_MAN_VOICE3.wav", 0 },		// 開始の掛け声(男3)
    { "SOUND/SE_START_WOMAN_VOICE.wav", 0 },	// 開始の掛け声(女)
    { "SOUND/SE_TAKE_DAMAGE_VOICE.wav", 0 },	// ダメージを受けた時の声
    { "SOUND/SE_TAKE_DAMAGE2.wav", 0 },	        // ダメージを受けた時の声2
    { "SOUND/SE_TAKE_DAMAGE3.wav", 0 },	        // ダメージを受けた時の声3
    { "SOUND/SE_TAKE_DAMAGE4.wav", 0 },	        // ダメージを受けた時の声4
    { "SOUND/SE_EXPLOSION2.wav", 0 },	        // 爆発音2
    { "SOUND/SE_ROAR.wav", 0 },	                // ボスの咆哮
    { "SOUND/SE_BOSS_EXPLOSION.wav", 0 },	    // ボスの爆発音
    { "SOUND/SE_JUMP.wav", 0 },	                // ジャンプ音
    { "SOUND/SE_FALL.wav", 0 },	                // 落下音
    { "SOUND/SE_GROUND.wav", 0 },	            // 地響き
    { "SOUND/SE_SHOT.wav", 0 },	                // ショット音
};

//=============================================================================
// サウンド初期化処理
// Author:後藤慎之助
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
    HRESULT hr;

    // COMライブラリの初期化
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // XAudio2オブジェクトの作成
    hr = XAudio2Create(&g_pXAudio2, 0);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // マスターボイスの生成
    hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
    if (FAILED(hr))
    {
        MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

        if (g_pXAudio2)
        {
            // XAudio2オブジェクトの開放
            g_pXAudio2->Release();
            g_pXAudio2 = NULL;
        }

        // COMライブラリの終了処理
        CoUninitialize();

        return E_FAIL;
    }

    // サウンドデータの初期化
    for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        HANDLE hFile;
        DWORD dwChunkSize = 0;
        DWORD dwChunkPosition = 0;
        DWORD dwFiletype;
        WAVEFORMATEXTENSIBLE wfx;
        XAUDIO2_BUFFER buffer;

        // バッファのクリア
        memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

        // サウンドデータファイルの生成
        hFile = CreateFile(g_aParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }
        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
        {// ファイルポインタを先頭に移動
            MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // WAVEファイルのチェック
        hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        if (dwFiletype != 'EVAW')
        {
            MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // フォーマットチェック
        hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // オーディオデータ読み込み
        hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }
        g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
        hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
        if (FAILED(hr))
        {
            MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // ソースボイスの生成
        hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
        if (FAILED(hr))
        {
            MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
            return S_FALSE;
        }

        // バッファの値設定
        memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
        buffer.AudioBytes = g_aSizeAudio[nCntSound];
        buffer.pAudioData = g_apDataAudio[nCntSound];
        buffer.Flags = XAUDIO2_END_OF_STREAM;
        buffer.LoopCount = g_aParam[nCntSound].nCntLoop;

        // オーディオバッファの登録
        g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

        // ファイルをクローズ
        CloseHandle(hFile);
    }

    return S_OK;
}

//=============================================================================
// サウンド終了処理
// Author:後藤慎之助
//=============================================================================
void UninitSound(void)
{
    // 一時停止
    for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        if (g_apSourceVoice[nCntSound])
        {
            // 一時停止
            g_apSourceVoice[nCntSound]->Stop(0);

            // ソースボイスの破棄
            g_apSourceVoice[nCntSound]->DestroyVoice();
            g_apSourceVoice[nCntSound] = NULL;

            // オーディオデータの開放
            free(g_apDataAudio[nCntSound]);
            g_apDataAudio[nCntSound] = NULL;
        }
    }

    // マスターボイスの破棄
    g_pMasteringVoice->DestroyVoice();
    g_pMasteringVoice = NULL;

    if (g_pXAudio2)
    {
        // XAudio2オブジェクトの開放
        g_pXAudio2->Release();
        g_pXAudio2 = NULL;
    }

    // COMライブラリの終了処理
    CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
// Author:後藤慎之助
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;
    XAUDIO2_BUFFER buffer;

    // バッファの値設定
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = g_aSizeAudio[label];
    buffer.pAudioData = g_apDataAudio[label];
    buffer.Flags = XAUDIO2_END_OF_STREAM;
    buffer.LoopCount = g_aParam[label].nCntLoop;

    // 状態取得
    g_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中
     // 一時停止
        g_apSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        g_apSourceVoice[label]->FlushSourceBuffers();
    }

    // オーディオバッファの登録
    g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

    // 再生
    g_apSourceVoice[label]->Start(0);

    return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
// Author:後藤慎之助
//=============================================================================
void StopSound(SOUND_LABEL label)
{
    XAUDIO2_VOICE_STATE xa2state;

    // 状態取得
    g_apSourceVoice[label]->GetState(&xa2state);
    if (xa2state.BuffersQueued != 0)
    {// 再生中
     // 一時停止
        g_apSourceVoice[label]->Stop(0);

        // オーディオバッファの削除
        g_apSourceVoice[label]->FlushSourceBuffers();
    }
}

//=============================================================================
// セグメント停止(全て)
// Author:後藤慎之助
//=============================================================================
void StopSound(void)
{
    // 一時停止
    for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
    {
        if (g_apSourceVoice[nCntSound])
        {
            // 一時停止
            g_apSourceVoice[nCntSound]->Stop(0);
        }
    }
}

//=============================================================================
// チャンクのチェック
// Author:後藤慎之助
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
    HRESULT hr = S_OK;
    DWORD dwRead;
    DWORD dwChunkType;
    DWORD dwChunkDataSize;
    DWORD dwRIFFDataSize = 0;
    DWORD dwFileType;
    DWORD dwBytesRead = 0;
    DWORD dwOffset = 0;

    if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを先頭に移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    while (hr == S_OK)
    {
        if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
        {// チャンクデータの読み込み
            hr = HRESULT_FROM_WIN32(GetLastError());
        }

        switch (dwChunkType)
        {
        case 'FFIR':
            dwRIFFDataSize = dwChunkDataSize;
            dwChunkDataSize = 4;
            if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
            {// ファイルタイプの読み込み
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
            break;

        default:
            if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
            {// ファイルポインタをチャンクデータ分移動
                return HRESULT_FROM_WIN32(GetLastError());
            }
        }

        dwOffset += sizeof(DWORD) * 2;
        if (dwChunkType == format)
        {
            *pChunkSize = dwChunkDataSize;
            *pChunkDataPosition = dwOffset;

            return S_OK;
        }

        dwOffset += dwChunkDataSize;
        if (dwBytesRead >= dwRIFFDataSize)
        {
            return S_FALSE;
        }
    }

    return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
// Author:後藤慎之助
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
    DWORD dwRead;

    if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
    {// ファイルポインタを指定位置まで移動
        return HRESULT_FROM_WIN32(GetLastError());
    }

    if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
    {// データの読み込み
        return HRESULT_FROM_WIN32(GetLastError());
    }

    return S_OK;
}

