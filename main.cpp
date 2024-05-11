//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"

#include "sprite.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"DX22game"				// ウインドウのクラス名
#define WINDOW_NAME		"すごろくバトル"	// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************

#ifdef _DEBUG
int		g_CountFPS;							// FPSカウンタ
char	g_DebugStr[2048] = WINDOW_NAME;		// デバッグ文字表示用

#endif

//=============================================================================
// メイン関数
//=============================================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）

	// 時間計測用
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;

	WNDCLASSEX	wcex = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	HWND		hWnd;
	MSG			msg;
	
	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	int window_width = 1280;
	int window_height = 720;

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,																		// ウィンドウの左座標
		CW_USEDEFAULT,																		// ウィンドウの上座標
		window_width + GetSystemMetrics(SM_CXDLGFRAME) * 2,									// ウィンドウ横幅
		window_height + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION),	// ウィンドウ縦幅
		NULL,
		NULL,
		hInstance,
		NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				g_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(g_DebugStr, WINDOW_NAME);
				wsprintf(&g_DebugStr[strlen(g_DebugStr)], " FPS:%d", g_CountFPS);
#endif
				Update();			// 更新処理
				Draw();				// 描画処理

				CheckScene();

				dwFrameCount++;
			}
		}
	}

	timeEndPeriod(1);				// 分解能を戻す

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 終了処理
	Uninit();
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	InitRenderer(hInstance, hWnd, bWindow);

	// 入力処理の初期化
	InitInput(hInstance, hWnd);

	//2Dスプライトの初期化
	InitSprite();

	//サウンド処理の初期化
	InitSound(hWnd);

	//フェードの初期化
	InitFade();


	SceneFadeIn(SCENE_TITLE, 0.03f);

	// 背面ポリゴンをカリング
	SetCullingMode(CULL_MODE_BACK);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	//現在実行中のシーン終了処理
	UninitScene();

	//フェードの終了処理
	UninitFade();

	//サウンド処理の終了処理
	UninitSound();

	//2Dスプライトの終了処理
	UninitSprite();

	//テクスチャの解放
	UninitTexture();

	//入力の終了処理
	UninitInput();

	UninitRenderer();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力の更新処理
	UpdateInput();



	UpdateScene();

	UpdateFade();

}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファクリア
	Clear();

	//シーンの描画
	DrawScene();

	SetWorldViewProjection2D();
	//フェードの描画
	DrawFade();


	

	// バックバッファ、フロントバッファ入れ替え
	Present();
}


#ifdef _DEBUG
char* GetDebugStr(void)
{
	return g_DebugStr;
}
#endif

float frand(void) {
	return ((float)rand() / RAND_MAX);
}

float easeOutBounce(float x) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5f / d1) * x + 0.75f;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25f / d1) * x + 0.9375f;
	}
	else {
		return n1 * (x -= 2.625f / d1) * x + 0.984375f;
	}
}