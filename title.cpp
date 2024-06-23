//title.cpp	タイトル画面管理
//20106_田中蓮
//23_06_30
#include "title.h"
#include "renderer.h"
#include "inputx.h"
#include "sprite.h"
#include "texture.h"

#include "logo.h"
#include "pushspace.h"
#include "star.h"

#include "scene.h"
#include "fade.h"

#include "sound.h"


//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
enum TITLE_PHASE {
	TP_INIT,		//初期化
	TP_BG,			//背景表示
	TP_LOGO,		//タイトルロゴ表示
	TP_PUSH,		//PushStart 表示
	TP_LOOP,		 //タイトルループ
	TP_INFO,		//説明画面
	TP_SCENECHANGE, //シーン移行


	TP_NUM,
};

struct TITLE_IMAGE {
	D3DXVECTOR3	pos;		//位置
	D3DXCOLOR	color;		//色
	float		flame;		//フレーム
	int			tex_no;		//テクスチャ番号
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int			g_SoundNo = 0;		//サウンド番号

static TITLE_PHASE	g_Phase = TP_INIT;	//フェーズ

static TITLE_IMAGE	g_BG;				//背景
static TITLE_IMAGE	g_INFO;				//操作説明
//=============================================================================
// 初期化処理
//=============================================================================
void InitTitle(void)
{
	//テクスチャの読み込み
	g_BG.tex_no = LoadTexture((char*)"data/TEXTURE/title00.png");
	g_INFO.tex_no = LoadTexture((char*)"data/TEXTURE/info.png");

	//サウンドの読み込み
	g_SoundNo = LoadSound((char*)"data/sound/bgm_title.wav");

	//サウンド再生
	SetVolume(g_SoundNo, 0.3f);
	PlaySound(g_SoundNo, -1);

	//初期化フェーズからスタートする
	g_Phase = TP_INIT;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	//音をすべて止める
	StopSoundAll();
	UninitStar();
	UninitPushspace();
	UninitLogo();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	switch (g_Phase)	{
		//初期化処理
	case TP_INIT:
		g_BG.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_BG.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_BG.flame = 0;

		InitLogo();
		InitPushspace();
		InitStar();

		g_Phase = TP_BG;
		break;
		//背景だけ表示
	case TP_BG:
		//フェード処理が完了したら
		if (GetFadeState() == FADE_NONE) {
			g_Phase = TP_LOGO;
		}
		if (GetKeyboardTrigger(DIK_RETURN) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_LOOP;
		}
		break;
		//ロゴを表示
	case TP_LOGO:
		LerpLogo();
		if(GetEndLerpLogo()){
			g_Phase = TP_PUSH;
		}
		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_LOOP;
		}
		break;
		//プッシュスペースを表示
	case TP_PUSH:
		LerpPush();
		//下から上がりきったらロープに移行
		if (GetEndLerp()) {
			g_Phase = TP_LOOP;
		}
		//スペースキーが押されたら飛ばす
		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_LOOP;
		}
		break;
		//ループさせる
	case TP_LOOP:
		//キーが押されたらアニメーションが終わったとの画面にする
		UpdateLogo();
		UpdatePushspace();
		UpdateStar();

		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_INFO;
		}
		break;
		//説明画面表示
	case TP_INFO:
		UpdatePushspace();
		if (GetKeyboardTrigger(DIK_SPACE) ||
			IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
			g_Phase = TP_SCENECHANGE;
		}
		break;
		//ゲームシーンに移行
	case TP_SCENECHANGE:
		if (GetFadeState() == FADE_NONE) {
			SetFadeColor(0.0f, 0.0f, 0.0f);
			SceneTransition(SCENE_GAME, 0.03f);
		}
		break;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	DrawSpriteLeftTop(g_BG.tex_no, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
	DrawPushspace();
	if (g_Phase == TP_INFO) {
		DrawSpriteLeftTop(g_INFO.tex_no, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);
	}
	else {
		// １枚のポリゴンの頂点とテクスチャ座標を設定
		DrawStar();
		DrawLogo();
	}
}
