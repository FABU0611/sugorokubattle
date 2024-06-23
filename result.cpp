//result.cpp	リザルト画面管理
//20106_田中蓮
//23_12_05
#include "result.h"
#include "renderer.h"
#include "inputx.h"
#include "sprite.h"
#include "texture.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"
#include "coin_screen.h"
#include "second_screen.h"
#include "clearrunk.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int			g_TexNo0 = 0;	//テクスチャ番号
static int			g_TexNo1 = 0;	//テクスチャ番号
static int			g_SoundNo = 0;	//サウンド番号

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	//テクスチャの読み込み
	g_TexNo0 = LoadTexture((char*)"data/TEXTURE/result00.png");	//背景
	g_TexNo1 = LoadTexture((char*)"data/TEXTURE/result01.png");	//文字

	//サウンド読み込み
	g_SoundNo = LoadSound((char*)"data/sound/result_score.wav");

	InitCoinscreen();
	InitSecondscreen();
	InitClearrunk();

	SetVolume(g_SoundNo, 0.3f);
	//サウンド再生
	PlaySound(g_SoundNo, 0);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	//すべての音を止める
	StopSoundAll();
	//表示されているものたちの終了処理
	UninitClearrunk();
	UninitSecondscreen();
	UninitCoinscreen();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	//表示されているものたちの更新処理
	UpdateSecondscreen();
	UpdateCoinscreen();
	UpdateClearrunk();

	//spaceキーでタイトルに戻る
	if (GetKeyboardTrigger(DIK_SPACE) && GetFadeState() == FADE_NONE ||
		IsButtonTriggered(0, XINPUT_GAMEPAD_A) && GetFadeState() == FADE_NONE){
		SetFadeColor(0.0f, 0.0f, 0.0f);
		SceneFadeIn(SCENE_TITLE, 0.03f);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	//描画
	//背景
	DrawSprite(g_TexNo0, SCREEN_CENTER_X, SCREEN_CENTER_Y,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f);
	//文字
	DrawSprite(g_TexNo1, SCREEN_CENTER_X, SCREEN_CENTER_Y,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		0.0f, 0.0f, 1.0f, 1.0f);

	//表示されているものたち
	DrawSecondscreen();
	DrawCoinscreen();
	DrawClearrunk();
}
