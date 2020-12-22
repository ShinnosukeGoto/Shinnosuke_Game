//=============================================================================
//
// PLASTIC TROOPERS [main.cpp]�̏��� (���C��)
// Author :�㓡�T�V��
//
//=============================================================================
#include "main.h"
#include "input.h"		//�L�[���͏���
#include "result.h"
#include "sound.h"
//#include "fade.h"
//#include "manual.h"
//#include "result2.h"
//#include "select.h"
//#include "game2.h"
//#include "game3.h"
//#include "floor.h"
//#include "camera.h"
//#include "light.h"
//#include "ui.h"
//#include "model.h"
#include "fade.h"
#include "title.h"
#include "campaign.h"
#include "survival.h"
#include "versus.h"
#include "practice.h"
#include "ranking.h"
#include "select.h"
#include "manual.h"
#include "controller.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define	CLASS_NAME		"WindowClass"
#define WINDOW_NAME		"PLASTIC TROOPERS"

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================
LPDIRECT3D9			g_pD3D = NULL;
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;

extern int g_aColor[4] = { 0,0,0,255 };	//�F�̏��{�s�����x�i�[

MODE g_mode = MODE_TITLE;//���[�h

						 //=============================================================================
						 // ���C���֐�
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

	if (FAILED(Init(hInstance, hWnd, TRUE)))	//FALSE�ɂ���ƁA�S��ʕ\��
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
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:

			DestroyWindow(hWnd);

		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// ����������
// hInstance : �C���X�^���X�̃n���h��
// hWnd      : �E�C���h�E�̃n���h��
// bWindow   : �E�C���h�E���[�h�ɂ��邩�ǂ���
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

	//�����_�[�X�e�[�g�̐ݒ�
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //NONE�ŗ��ʕ`��
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    //g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);  //���C���[

    //�}�e���A���̐ݒ�
    D3DMATERIAL9 material;
    ZeroMemory(&material, sizeof(D3DMATERIAL9));
    material.Ambient.r = 1.0f;
    material.Ambient.g = 1.0f;
    material.Ambient.b = 1.0f;
    material.Ambient.a = 1.0f;
    g_pD3DDevice->SetMaterial(&material);
    g_pD3DDevice->SetRenderState(D3DRS_AMBIENT, 0x44444444);

	//// �T���v���[�X�e�[�g�̐ݒ�
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	InitSound(hWnd);
	InitKeyboard(hInstance, hWnd);//�L�[�{�[�h�̏�����
    InitJoypad(hInstance, hWnd);    //�R���g���[���[�̏�����
	InitTitle();
	InitFade(g_mode);				//�t�F�[�h������

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void Uninit(void)
{
	//�e�I�u�W�F�N�g�̏I������
	UninitKeyboard();	//�L�[�{�[�h
    ReleaseDInput();		//�p�b�h
    UninitFade();
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_SELECT:
		//UninitSelect();
		break;

	case MODE_MANUAL:
		UninitManual();
		break;

	case MODE_CAMPAIGN:
		UninitCampaign();
		break;

	case MODE_SURVIVAL:
		//UninitSurvival();
		break;

	case MODE_VERSUS:
		//UninitVersus();
		break;

	case MODE_PRACTICE:
		//UninitPractice();
		break;

	case MODE_RANKING:
		UninitRanking();
		break;

	case MODE_RESULT:
		UninitResult();
		break;
	}

	UninitSound();//�T�E���h�̏I��
	UninitFade();//�t�F�[�h�̏I��

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
// �X�V����
//=============================================================================
void Update(void)
{
	//�e��I�u�W�F�N�g�̍X�V����
	UpdateKeyboard();	//�L�[�{�[�h
    UpdateJoypad();
	UpdateFade();//�t�F�[�h��ʂ̍X�V

				 //UpdateCamera();		//�J����
				 //UpdateLight();		//���C�g
				 //UpdatePlayer();	//�v���C���[(������R�����g�A�E�g)
				 //UpdateModel();
				 //UpdateUI();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;

	case MODE_SELECT:
		//UpdateSelect();
		break;

	case MODE_MANUAL:
		UpdateManual();
		break;

	case MODE_CAMPAIGN:
		UpdateCampaign();
		break;

	case MODE_SURVIVAL:
		//UpdateSurvival();
		break;

	case MODE_VERSUS:
		//UpdateVersus();
		break;

	case MODE_PRACTICE:
		//UpdatePractice();
		break;

	case MODE_RANKING:
		UpdateRanking();
		break;

	case MODE_RESULT:
		UpdateResult();
		break;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void Draw(void)
{
	//�o�b�t�@&Z�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255, 255, 255, 0), 1.0f, 0);

	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//�e��`�揈��
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_SELECT:
			//DrawSelect();
			break;

		case MODE_MANUAL:
			DrawManual();
			break;

		case MODE_CAMPAIGN:
			DrawCampaign();
			break;

		case MODE_SURVIVAL:
			//DrawSurvival();
			break;

		case MODE_VERSUS:
			//DrawVersus();
			break;

		case MODE_PRACTICE:
			//DrawPractice();
			break;

		case MODE_RANKING:
			DrawRanking();
			break;

		case MODE_RESULT:
			DrawResult();
			break;
		}

		DrawFade();//�t�F�[�h�̕`��

		//DirectX�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================================================
//�f�o�C�X�̎擾
//==============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==============================================================================
//���[�h�̐ݒ�
//==============================================================================
void SetMode(MODE mode)
{
	//�e��ʂ̏I������
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;

	case MODE_SELECT:
		//UninitSelect();
		break;

	case MODE_MANUAL:
		UninitManual();
		break;

	case MODE_CAMPAIGN:
		UninitCampaign();
		break;

	case MODE_SURVIVAL:
		//UninitSurvival();
		break;

	case MODE_VERSUS:
		//UninitVersus();
		break;

	case MODE_PRACTICE:
		//UninitPractice();
		break;

	case MODE_RANKING:
		UninitRanking();
		break;

	case MODE_RESULT:
		UninitResult();
		break;
	}

	g_mode = mode;

	//���̉�ʂ̏���������
	switch (g_mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;

	case MODE_SELECT:
		//InitSelect();
		break;

	case MODE_MANUAL:
		InitManual();
		break;

	case MODE_CAMPAIGN:
		InitCampaign();
		break;

	case MODE_SURVIVAL:
		//InitSurvival();
		break;

	case MODE_VERSUS:
		//InitVersus();
		break;

	case MODE_PRACTICE:
		//InitPractice();
		break;

	case MODE_RANKING:
		InitRanking();
		break;

	case MODE_RESULT:
		InitResult();
		break;
	}
}

//=============================================================================
// ���[�h���擾
//=============================================================================
MODE GetMode(void)
{
	return g_mode;
}