//=============================================================================
//
// 最強昔話決定戦 [main.cpp]の処理 (メイン)
// Author :後藤慎之助
//
//=============================================================================
#include "main.h"
#include "input.h"		//キー入力処理
#include "result.h"
#include "sound.h"
#include "fade.h"
#include "title.h"
#include "game.h"
#include "ranking.h"
#include "manual.h"
#include "controller.h"
#include "menu.h"
#include "setting.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	CLASS_NAME		"WindowClass"
#define WINDOW_NAME		"最強昔話決定戦"

//=============================================================================
// プロトタイプ宣言
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;

extern int g_aColor[4] = { 0,0,0,255 };	//色の情報＋不透明度格納

MODE g_mode = MODE_TITLE;//モード(最初はタイトル)

//=============================================================================
// メイン関数
// Author:後藤慎之助
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex =
    {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WindowProc,
        0,
        0,
        hInstance,
        LoadIcon(NULL, IDI_APPLICATION),
        LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL,
        CLASS_NAME,
        LoadIcon(NULL, IDI_APPLICATION)
    };
    HWND hWnd;
    MSG msg;
    RECT rect =
    { -
        WINDOW_POS_X,
        WINDOW_POS_Y,
        (WINDOW_POS_X + SCREEN_WIDTH),
        (WINDOW_POS_Y + SCREEN_HEIGHT)
    };
    DWORD dwCurrentTime;
    DWORD dwExecLastTime;

    RegisterClassEx(&wcex);

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

    hWnd = CreateWindowEx(0,
        CLASS_NAME,
        WINDOW_NAME,
        WS_OVERLAPPEDWINDOW,
        WINDOW_POS_X,
        WINDOW_POS_Y,
        (rect.right - rect.left),
        (rect.bottom - rect.top),
        NULL,
        NULL,
        hInstance,
        NULL);

    dwCurrentTime = 0;
    dwExecLastTime = timeGetTime();

    if (FAILED(Init(hInstance, hWnd, TRUE)))	//FALSEにすると、全画面表示
    {
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            else
            {

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            dwCurrentTime = timeGetTime();

            if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
            {
                dwExecLastTime = dwCurrentTime;

                Update();

                Draw();
            }
        }

        if (g_mode == MODE_EXIT)
        {
            break;
        }
    }

    Uninit();

    UnregisterClass(CLASS_NAME, wcex.hInstance);

    return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
// Author:後藤慎之助
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:

        //// マウスカーソルを非表示に
        //ShowCursor(FALSE);

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:

            DestroyWindow(hWnd);

            break;
        }
        break;

    default:
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
// Author:後藤慎之助
// hInstance : インスタンスのハンドル
// hWnd      : ウインドウのハンドル
// bWindow   : ウインドウモードにするかどうか
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
    D3DDISPLAYMODE d3ddm;
    D3DPRESENT_PARAMETERS d3dpp;

    g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (g_pD3D == NULL)
    {
        return E_FAIL;
    }

    if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
    {
        return E_FAIL;
    }

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferWidth = SCREEN_WIDTH;
    d3dpp.BackBufferHeight = SCREEN_HEIGHT;
    d3dpp.BackBufferFormat = d3ddm.Format;
    d3dpp.BackBufferCount = 1;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.Windowed = bWindow;
    d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
    //	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &g_pD3DDevice)))
    {

        if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
            &d3dpp,
            &g_pD3DDevice)))
        {

            if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                D3DDEVTYPE_REF,
                hWnd,
                D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                &d3dpp,
                &g_pD3DDevice)))
            {

                return E_FAIL;
            }
        }
    }

    //レンダーステートの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //NONEで両面描画
    g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    //g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  //ワイヤー

    //マテリアルの設定
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient.r = 1.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 1.0f;
    material.Ambient.a = 1.0f;
    g_pD3DDevice->SetMaterial(&material);
    g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

    //テクスチャステージステートの設定
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

    InitFirstSetting();           //最初の設定の初期化
    InitSound(hWnd);
    InitKeyboard(hInstance, hWnd);//キーボードの初期化
    InitJoypad(hInstance, hWnd);    //コントローラーの初期化
    InitFade(g_mode);				//フェード初期化

    return S_OK;
}

//=============================================================================
// 終了処理
// Author:後藤慎之助
//=============================================================================
void Uninit(void)
{
    //各オブジェクトの終了処理
    UninitKeyboard();	//キーボード
    ReleaseDInput();		//パッド
    UninitFade();
    switch (g_mode)
    {
    case MODE_TITLE:
        UninitTitle();
        break;

    case MODE_MENU:
        UninitMenu();
        break;

    case MODE_SETTING:
        UninitSetting();
        break;

    case MODE_MANUAL:
        UninitManual();
        break;

    case MODE_GAME:
        UninitGame();
        break;

    case MODE_NEXT_GAME:
        UninitGame();
        break;

    case MODE_RESULT:
        UninitResult();
        break;

    case MODE_RANKING:
        UninitRanking();
        break;
    }

    UninitSound();//サウンドの終了
    UninitFade();//フェードの終了

    if (g_pD3DDevice != NULL)
    {
        g_pD3DDevice->Release();
        g_pD3DDevice = NULL;
    }

    if (g_pD3D != NULL)
    {
        g_pD3D->Release();
        g_pD3D = NULL;
    }
}

//=============================================================================
// 更新処理
// Author:後藤慎之助
//=============================================================================
void Update(void)
{
    //各種オブジェクトの更新処理
    UpdateKeyboard();	//キーボード
    UpdateJoypad();
    UpdateFade();//フェード画面の更新

    switch (g_mode)
    {
    case MODE_TITLE:
        UpdateTitle();
        break;

    case MODE_MENU:
        UpdateMenu();
        break;

    case MODE_SETTING:
        UpdateSetting();
        break;

    case MODE_MANUAL:
        UpdateManual();
        break;

    case MODE_GAME:
        UpdateGame();
        break;

    case MODE_NEXT_GAME:
        UpdateGame();
        break;

    case MODE_RESULT:
        UpdateResult();
        break;

    case MODE_RANKING:
        UpdateRanking();
        break;
    }

}

//=============================================================================
// 描画処理
// Author:後藤慎之助
//=============================================================================
void Draw(void)
{
    //バッファ&Zバッファのクリア
    g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(152, 223, 120, 0), 1.0f, 0);

    if (SUCCEEDED(g_pD3DDevice->BeginScene()))
    {
        //各種描画処理
        switch (g_mode)
        {
        case MODE_TITLE:
            DrawTitle();
            break;

        case MODE_MENU:
            DrawMenu();
            break;

        case MODE_SETTING:
            DrawSetting();
            break;

        case MODE_MANUAL:
            DrawManual();
            break;

        case MODE_GAME:
            DrawGame();
            break;

        case MODE_NEXT_GAME:
            DrawGame();
            break;

        case MODE_RESULT:
            DrawResult();
            break;

        case MODE_RANKING:
            DrawRanking();
            break;
        }

        DrawFade();//フェードの描画

                   //DirectXによる描画の終了
        g_pD3DDevice->EndScene();
    }

    g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================================================
// デバイスの取得
// Author:後藤慎之助
//==============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
    return g_pD3DDevice;
}

//==============================================================================
// モードの設定
// Author:後藤慎之助
//==============================================================================
void SetMode(MODE mode)
{
    //各画面の終了処理
    switch (g_mode)
    {
    case MODE_TITLE:
        UninitTitle();
        break;

    case MODE_MENU:
        UninitMenu();
        break;

    case MODE_SETTING:
        UninitSetting();
        break;

    case MODE_MANUAL:
        UninitManual();
        break;

    case MODE_GAME:
        UninitGame();
        break;

    case MODE_NEXT_GAME:
        UninitGame();
        break;

    case MODE_RESULT:
        UninitResult();
        break;

    case MODE_RANKING:
        UninitRanking();
        break;
    }

    g_mode = mode;

    //次の画面の初期化処理
    switch (g_mode)
    {
    case MODE_TITLE:
        InitTitle();
        break;

    case MODE_MENU:
        InitMenu();
        break;

    case MODE_SETTING:
        InitSetting();
        break;

    case MODE_MANUAL:
        InitManual();
        break;

    case MODE_GAME:
        InitGame();
        break;

    case MODE_NEXT_GAME:
        InitNextGame();
        break;

    case MODE_RESULT:
        InitResult();
        break;

    case MODE_RANKING:
        InitRanking();
        break;
    }
}

//=============================================================================
// モードを取得
// Author:後藤慎之助
//=============================================================================
MODE GetMode(void)
{
    return g_mode;
}