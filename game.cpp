//game.cpp		ゲーム管理
//20106_田中　蓮
//23_11_30
#include "main.h"
#include "renderer.h"
#include "Texture.h"
#include "scene.h"
#include "fade.h"
#include "sound.h"
#include "game.h"

#include "camera.h"
#include "player.h"
#include "life.h"
#include "piece.h"
#include "bullet.h"
#include "dice.h"

#include "enemy_manager.h"

#include "coin.h"

#include "trout_manager.h"
#include "trout_red.h"
#include "trout_blue.h"
#include "trout_white.h"
#include "trout_end.h"

#include "meshfield.h"
#include "meshsky.h"
#include "meshmountain.h"

#include "shadow.h"
#include "polyline.h"


static int g_TexNo = 0;
static int g_SoundNo = 0;
TROUT_MANAGER* tm;

//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(){
	//カメラの初期化
	InitCamera();

	//影の初期化(プレイヤーの初期化などでセットされる前に初期化する)
	InitShadow();

	//線の初期化
	InitPolyLine();

	// 地面の初期化
	InitMeshField(D3DXVECTOR3(0.0f, -200.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, 50, 100.0f, 100.0f);

	// 空の初期化
	InitMeshSky(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3000.0f, 16, 8);

	// 山の初期化
	InitMeshMountain(D3DXVECTOR3(0.0f, -70.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 16, 1, 2500.0f, 400.0f);

	//マスマネージャー作成
	tm = new TROUT_MANAGER();

	//マス配置
	tm->Add(new TROUT_WHITE	({   0.0f,  -200.0f, -1000.0f }));
	tm->Add(new TROUT_BLUE	({  300.0f, -200.0f,  -900.0f }));
	tm->Add(new TROUT_BLUE	({  600.0f, -200.0f,  -700.0f }));
	tm->Add(new TROUT_RED	({  300.0f, -200.0f,  -400.0f }));
	tm->Add(new TROUT_WHITE	({    0.0f, -200.0f,  -500.0f }));
	tm->Add(new TROUT_WHITE	({ -300.0f, -200.0f,  -600.0f }));
	tm->Add(new TROUT_BLUE	({ -600.0f, -200.0f,  -400.0f }));
	tm->Add(new TROUT_WHITE	({ -300.0f, -200.0f,  -200.0f }));
	tm->Add(new TROUT_RED	({    0.0f, -200.0f,     0.0f }));
	tm->Add(new TROUT_BLUE	({  300.0f, -200.0f,   200.0f }));
	tm->Add(new TROUT_WHITE	({  600.0f, -200.0f,   400.0f }));
	tm->Add(new TROUT_BLUE	({  300.0f, -200.0f,   600.0f }));
	tm->Add(new TROUT_BLUE	({    0.0f, -200.0f,   500.0f }));
	tm->Add(new TROUT_RED	({ -300.0f, -200.0f,   400.0f }));
	tm->Add(new TROUT_WHITE	({ -600.0f, -200.0f,   600.0f }));
	tm->Add(new TROUT_RED	({ -300.0f, -200.0f,   800.0f }));
	tm->Add(new TROUT_BLUE	({    0.0f, -200.0f,  1000.0f }));
	tm->Add(new TROUT_WHITE	({  300.0f, -200.0f,  1200.0f }));
	tm->Add(new TROUT_RED	({  600.0f, -200.0f,  1400.0f }));
	tm->Add(new TROUT_BLUE	({  300.0f, -200.0f,  1600.0f }));
	tm->Add(new TROUT_END	({    0.0f, -200.0f,  2000.0f }));

	tm->InitTroutManager();

	// ポリゴンの初期化
	InitPlayer();
	InitLife();
	InitPiece();
	InitBullet();
	InitDice();
	InitCoin();

	//UIテクスチャ読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/info_game.png");
	//サウンド読み込み
	g_SoundNo = LoadSound((char*)"data/sound/bgm_game.wav");
	SetVolume(g_SoundNo, 0.3f);
	PlaySound(g_SoundNo, -1);
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitGame(){
	StopSoundAll();
	// ポリゴンの終了処理
	UninitCoin();
	UninitDice();
	UninitBullet();
	UninitPiece();
	UninitLife();
	UninitPlayer();

	//マスたちの終了処理
	tm->UninitTroutManager();

	//マスマネージャー解放
	delete tm;

	UninitMeshMountain();
	UninitMeshSky();
	UninitMeshField();

	//線の終了処理
	UninitPolyLine();

	//影の終了処理
	UninitShadow();

	// カメラの終了処理
	UninitCamera();
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateGame(){
	// カメラ更新
	UpdateCamera();

	// 地面の更新処理
	UpdateMeshField();
	UpdateMeshSky();
	UpdateMeshMountain();

	// ポリゴンの更新処理
	UpdatePlayer();
	UpdateLife();
	UpdatePiece();
	UpdateBullet();

	//マスの更新処理
	tm->UpdateTroutManager();

	if (tm->TroutEndEvent()) {
		//止まったマスのイベントが終わってるならさいころを回す
		DiceSetStop(false);
		UpdateDice();
	}
	else {
		DiceSetStop(true);
	}

	UpdateCoin();
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawGame(){

	SetCamera();

	// 地面の描画処理
	DrawMeshSky();
	DrawMeshField();
	DrawMeshMountain();

	//マスたちの描画処理
	tm->DrawTroutManager();

	//影の描画（Z比較無しのため描画する順番に注意する）
	DrawShadow();

	//線の描画
	DrawPolyLine();

	// ポリゴンの描画処理
	DrawPlayer();
	DrawPiece();
	DrawBullet();
	DrawDice();
	DrawCoin();

	//2Dになる(平行投影)
	SetWorldViewProjection2D();

	//操作ボタン表示
	DrawSpriteLeftTop(g_TexNo, 0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

	//プレイヤーの体力を表示
	DrawLife();
}