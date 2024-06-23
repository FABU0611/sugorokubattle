//player.cpp	プレイヤー表示処理
//20106_田中　蓮
//23_12_07
#include "main.h"
#include "renderer.h"
#include "inputx.h"
#include "texture.h"
#include "model.h"

#include "camera.h"
#include "player.h"
#include "bullet.h"
#include "shadow.h"

#include "fade.h"

#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float VALUE_MOVE = 5.0f;				// 移動量
static const float PLAYER_RAD = 40.0f;				// 半径

static const int PLAYER_LIFE = 10;					//最大ライフ
static const int PLAYER_FRASH_FRAME = 30;			//点滅時間
static const int PLAYER_NOHIT_FRAME = 120;			//無敵時間

static const int WALK_ANIM_ROT = 20;				//歩くアニメーションの手足の角度
static const int ANIME_NUM = 5;

static const float		STICK_DEADZONE = 0.15f;
static const float		ROTATE_SPEED = 0.15f;

struct PLAYER{
	D3DXVECTOR3 pos;		//ポジション
	D3DXVECTOR3 shot_pos;	//弾が出る位置
	D3DXVECTOR3 rot;		//角度
	D3DXVECTOR3 dir;		//角度
	D3DXVECTOR3 scl;		//スケール

	float		rad;		//当たり判定用半径

	float		speed;		//移動速度
	float		front;		//向いている方向

	int			frashframe;	//点滅
	int			nohitframe;	//無敵時間
	int			walkframe;	//歩いているときのフレーム

	int			life;		//ライフ
	bool		hit;		//ヒット状態か

	bool		isuse;		//使っている状態か

	int			shadowID;	//影ID

	int			coin;		//拾ったコインの枚数
	int			killedenemy;//倒した敵の数


	unsigned int second;	//生きている時間
	unsigned int starttime;	//スタート時間
	unsigned int endtime;	//終了時間


	int			state;
	D3DXMATRIX  mtxWorld;
};

struct PARTS{
	D3DXVECTOR3		pos;	//位置情報
	D3DXVECTOR3		scl;	//大きさ情報
	D3DXVECTOR3		rot;	//回転情報	
	D3DXMATRIX		mtxWorld;	//マトリックス
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
struct KeyFrame {
	D3DXVECTOR3	key;	//回転角度
	float		frame;	//フレーム
};
struct MotionSet {
	KeyFrame	LegLMotion[ANIME_NUM];
	KeyFrame	LegRMotion[ANIME_NUM];
	KeyFrame	ArmLMotion[ANIME_NUM];
	KeyFrame	ArmRMotion[ANIME_NUM];
	KeyFrame	HeadMotion[ANIME_NUM];
};

//歩きモーション
MotionSet WalkMotion = {
{//左足
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.5f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{-0.5f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
{//右足
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{-0.5f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.5f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
{//左腕
	{{ 0.0f, -3.0f, -0.2f},   0.0f},
	{{ 0.5f, -3.0f, -0.2f},  60.0f},
	{{ 0.0f, -3.0f, -0.2f}, 120.0f},
	{{-0.5f, -3.0f, -0.2f}, 180.0f},
	{{ 0.0f, -3.0f, -0.2f}, 240.0f},
},
{//右腕
	{{ 0.0f,  3.0f,  0.2f},   0.0f},
	{{-0.5f,  3.0f,  0.2f},  60.0f},
	{{ 0.0f,  3.0f,  0.2f}, 120.0f},
	{{ 0.5f,  3.0f,  0.2f}, 180.0f},
	{{ 0.0f,  3.0f,  0.2f}, 240.0f},
},
{//頭
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.0f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.0f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
};

//待機モーション
MotionSet WaitMotion = {
{//左足
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.0f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.0f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
{//右足
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.0f,  0.0f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.0f,  0.0f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
{//左腕
	{{ 0.0f, -3.0f, -0.0f},   0.0f},
	{{ 0.0f, -3.0f, -0.5f},  60.0f},
	{{ 0.0f, -3.0f, -0.0f}, 120.0f},
	{{ 0.0f, -3.0f, -0.5f}, 180.0f},
	{{ 0.0f, -3.0f, -0.0f}, 240.0f},
},
{//右腕
	{{ 0.0f,  3.0f,  0.0f},   0.0f},
	{{ 0.0f,  3.0f,  0.5f},  60.0f},
	{{ 0.0f,  3.0f,  0.0f}, 120.0f},
	{{ 0.0f,  3.0f,  0.5f}, 180.0f},
	{{ 0.0f,  3.0f,  0.0f}, 240.0f},
},
{//頭
	{{ 0.0f,  0.0f,  0.0f},   0.0f},
	{{ 0.0f,  0.2f,  0.0f},  60.0f},
	{{ 0.0f,  0.0f,  0.0f}, 120.0f},
	{{ 0.0f, -0.2f,  0.0f}, 180.0f},
	{{ 0.0f,  0.0f,  0.0f}, 240.0f},
},
};

void HitToEnemy(void);
D3DXVECTOR3 Anime(KeyFrame* keyframe, int size);
D3DXVECTOR3 StopAnime(KeyFrame* keyframe, int state);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
//サウンド用
enum SOUND_NUM {
	walk,
	hit,
	dead,

	sound_max,
};

static D3DXVECTOR3	g_rotate;

static DX11_MODEL	g_BodyModel;	// モデル読み込み	親
static DX11_MODEL	g_HeadModel;	// モデル読み込み
static DX11_MODEL	g_ArmRModel;	// モデル読み込み
static DX11_MODEL	g_ArmLModel;	// モデル読み込み
static DX11_MODEL	g_LegRModel;	// モデル読み込み
static DX11_MODEL	g_LegLModel;	// モデル読み込み

static PLAYER		g_Body;		// プレイヤー頭情報		親
static PARTS		g_Head;
static PARTS		g_ArmR;
static PARTS		g_ArmL;
static PARTS		g_LegR;
static PARTS		g_LegL;

static LIGHT		g_DirectionalLight;		//ディレクショナルライト
static LIGHT		g_PointLight[4];		//ポイントライト

static int			g_SoundNo[sound_max];	//サウンド

static float		g_frame = 0.0f;
static int			g_keyindex = 0;

MotionSet*			g_pMotion;
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPlayer(void)
{
	//objモデルの読み込み
	LoadModel((char*)"data/MODEL/frog_body.obj", &g_BodyModel);
	LoadModel((char*)"data/MODEL/frog_head.obj", &g_HeadModel);
	LoadModel((char*)"data/MODEL/frog_armR.obj", &g_ArmRModel);
	LoadModel((char*)"data/MODEL/frog_armL.obj", &g_ArmLModel);
	LoadModel((char*)"data/MODEL/frog_legR.obj", &g_LegRModel);
	LoadModel((char*)"data/MODEL/frog_legL.obj", &g_LegLModel);

	//サウンドの読み込み
	g_SoundNo[walk] = LoadSound((char*)"data/sound/player_walk.wav");
	SetVolume(g_SoundNo[walk], 0.2f);
	g_SoundNo[hit] = LoadSound((char*)"data/sound/player_hit.wav");
	SetVolume(g_SoundNo[hit], 0.4f);
	g_SoundNo[dead] = LoadSound((char*)"data/sound/player_dead.wav");
	SetVolume(g_SoundNo[dead], 0.2f);


	// ライトを有効化
	SetLightEnable(true);

	// 並行光源の設定（世界を照らす光）
	g_DirectionalLight.Type = LIGHT_TYPE_DIRECTIONAL;				// 並行光源
	g_DirectionalLight.Enable = true;								// このライトをON
	g_DirectionalLight.Direction = D3DXVECTOR4(0.0f, -1.0f, 0.1f, 0.0f);// 光の向き
	g_DirectionalLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 光の色
	SetLight(0, &g_DirectionalLight);								// これで設定している

	// 点光源の設定
	g_PointLight[0].Position = D3DXVECTOR4( 700.0f, 300.0f, -1000.0f, 0.0f);// 光の座標
	g_PointLight[1].Position = D3DXVECTOR4(-700.0f, 300.0f, -1000.0f, 0.0f);// 光の座標
	g_PointLight[2].Position = D3DXVECTOR4( 700.0f, 300.0f,  2000.0f, 0.0f);// 光の座標
	g_PointLight[3].Position = D3DXVECTOR4(-700.0f, 300.0f,  2000.0f, 0.0f);// 光の座標
	g_PointLight[0].Direction = D3DXVECTOR4(-0.5f, -1.0f, 0.0f, 0.5f);// 光の向き
	g_PointLight[1].Direction = D3DXVECTOR4(0.5f, -1.0f, 0.0f, 0.5f);// 光の向き
	g_PointLight[2].Direction = D3DXVECTOR4(-0.5f, -1.0f, 0.0f, -0.5f);// 光の向き
	g_PointLight[3].Direction = D3DXVECTOR4(0.5f, -1.0f, 0.0f, -0.5f);// 光の向き
	for (int i = 0; i < 4; i++) {
		g_PointLight[i].Type = LIGHT_TYPE_POINT;						// 並行光源
		g_PointLight[i].Enable = true;									// このライトをON
		g_PointLight[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 光の色
		g_PointLight[i].Attenuation = 1000.0f;							// 減衰距離
		SetLight(i + 1, &g_PointLight[i]);									// これで設定している
	}

	// 位置・回転・スケールの初期設定
	g_Body.pos = D3DXVECTOR3(0.0f, -30.0f, -1700.0f);
	g_Body.shot_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Body.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Body.dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Body.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Body.rad = PLAYER_RAD;

	g_Body.speed = 0.0f;
	g_Body.front = 0.0f;

	g_Body.frashframe = 0;
	g_Body.nohitframe = 0;
	g_Body.walkframe = 0;

	g_Body.life = PLAYER_LIFE;
	g_Body.hit = false;

	g_Body.isuse = true;


	g_Body.coin = 0;
	g_Body.killedenemy = 0;

	g_Body.second = 0;
	//開始時刻を保存する
	g_Body.starttime = timeGetTime();
	g_Body.endtime = 0;

	//影のセット
	g_Body.shadowID = SetShadow(g_Body.pos, 200.0f);


	g_Head.pos = {0.0f, 20.0f, 0.0f};	// プレイヤー体位置情報
	g_Head.scl = {1.0f, 1.0f, 1.0f};	// プレイヤー体大きさ情報
	g_Head.rot = {0.0f, 0.0f, 0.0f};	// プレイヤー体回転情報

	g_ArmR.pos = {-20.0f, 15.0f, 0.0f};	// プレイヤー右腕位置情報
	g_ArmR.scl = {1.0f, 1.0f, 1.0f};	// プレイヤー右腕大きさ情報
	g_ArmR.rot = {0.0f, 0.0f, 0.0f};	// プレイヤー右腕回転情報

	g_ArmL.pos = {20.0f, 15.0f, 0.0f};	// プレイヤー左腕位置情報
	g_ArmL.scl = {1.0f, 1.0f, 1.0f};	// プレイヤー左腕大きさ情報
	g_ArmL.rot = {0.0f, 0.0f, 0.0f};	// プレイヤー左腕回転情報

	g_LegR.pos = {-10.0f, -25.0f, -5.0f};	// プレイヤー右足位置情報
	g_LegR.scl = {1.0f, 1.0f, 1.0f};	// プレイヤー右足大きさ情報
	g_LegR.rot = {0.0f, 0.0f, 0.0f};	// プレイヤー右足回転情報

	g_LegL.pos = {10.0f, -25.0f, -5.0f};	// プレイヤー左足位置情報
	g_LegL.scl = {1.0f, 1.0f, 1.0f};	// プレイヤー左足大きさ情報
	g_LegL.rot = {0.0f, 0.0f, 0.0f};	// プレイヤー左足回転情報

	g_rotate = { 0.0f, 0.0f, 0.0f };

	g_pMotion = &WaitMotion;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	//終了時間を入れる
	g_Body.endtime = g_Body.second;
	if (&g_BodyModel != NULL) {
		//objモデルの解放
		UnloadModel(&g_BodyModel);
	}
	if (&g_HeadModel != NULL) {
		//objモデルの解放
		UnloadModel(&g_HeadModel);
	}
	if (&g_ArmRModel != NULL) {
		//objモデルの解放
		UnloadModel(&g_ArmRModel);
	}
	if (&g_ArmLModel != NULL) {
		//objモデルの解放
		UnloadModel(&g_ArmLModel);
	}
	if (&g_LegRModel != NULL) {
		//objモデルの解放
		UnloadModel(&g_LegRModel);
	}
	if (&g_LegLModel != NULL) {
		//objモデルの解放
		UnloadModel(&g_LegLModel);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	if (!GetPlayerUse()) {
		//体力が０になったらリザルトに遷移
		SceneTransition(SCENE_RESULT, 0.03f);
		return;
	}

	//リザルトに行くまでに経過した秒数を更新する
	g_Body.second = (timeGetTime() - g_Body.starttime) / 1000;

	//カメラを受け取る
	CAMERA *cam = GetCamera();

	//カメラの注視点をセット
	SetCameraAT(g_Body.pos);

	//速度リセット
	g_Body.speed *= 0.9f;

	// 移動-----------------------------------------------------------------------------------
	//スティック
	if (GetThumbLeftY(0) < -STICK_DEADZONE ||				//右スティック
		GetThumbLeftY(0) > STICK_DEADZONE ||
		GetThumbLeftX(0) < -STICK_DEADZONE ||				//右スティック
		GetThumbLeftX(0) > STICK_DEADZONE) {
		g_Body.speed = VALUE_MOVE;
		//スティックの倒した方向を向く
		g_Body.rot.y = g_Body.front = atan2f(GetThumbLeftY(0), -GetThumbLeftX(0)) + (D3DX_PI * 1.5f) + cam->rot.y;
		if (g_Body.state != 1) {
			g_Body.state = 0;
		}
	}

	if (GetKeyboardPress(DIK_A)) {
		g_Body.speed = VALUE_MOVE;							//速度を入れる
		if (GetKeyboardPress(DIK_W)) {						//左上
			g_Body.rot.y = g_Body.front = D3DX_PI * 1.75f + cam->rot.y;
		}
		else if (GetKeyboardPress(DIK_S)) {					//左下
			g_Body.rot.y = g_Body.front = D3DX_PI * 1.25f + cam->rot.y;
		}
		else {												//左
			g_Body.rot.y = g_Body.front = D3DX_PI * 1.5f + cam->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_D))	{
		g_Body.speed = VALUE_MOVE;							//速度を入れる
		if (GetKeyboardPress(DIK_W)) {						//右上
			g_Body.rot.y = g_Body.front = D3DX_PI * 0.25f + cam->rot.y;
		}
		else if (GetKeyboardPress(DIK_S)) {					//右下
			g_Body.rot.y = g_Body.front = D3DX_PI * 0.75f + cam->rot.y;
		}
		else {												//右
			g_Body.rot.y = g_Body.front = D3DX_PI * 0.5f + cam->rot.y;
		}
	}
	else if (GetKeyboardPress(DIK_W))	{
		g_Body.speed = VALUE_MOVE;							//速度を入れる
		g_Body.rot.y = g_Body.front = D3DX_PI * 0.0f + cam->rot.y;		//上
	}
	else if (GetKeyboardPress(DIK_S))	{
		g_Body.speed = VALUE_MOVE;							//速度を入れる
		g_Body.rot.y = g_Body.front = D3DX_PI * 1.0f + cam->rot.y;		//下
	}

	//カメラを向いている方向を正面にする
	g_Body.front = cam->rot.y;


	//座標更新----------------------------------------------------------------------------------
	g_Body.pos.x += sinf(g_Body.rot.y) * g_Body.speed;
	g_Body.pos.z += cosf(g_Body.rot.y) * g_Body.speed;

	//弾が出る位置をキャラの目の前にする
	g_Body.shot_pos.x = g_Body.pos.x + sinf(g_Body.rot.y) * 40.0f;
	g_Body.shot_pos.z = g_Body.pos.z + cosf(g_Body.rot.y) * 40.0f;



	//影の座標更新
	D3DXVECTOR3 shadowpos = g_Body.pos;
	shadowpos.y = - 75.0f;
	SetPositionShadow(g_Body.shadowID, shadowpos);

	//弾を撃つ
	if (GetKeyboardTrigger(DIK_M) ||
		IsButtonTriggered(0, XINPUT_GAMEPAD_A)) {
		//向いている方向に撃つ
		g_Body.front = g_Body.rot.y;
		SetBullet(g_Body.shot_pos);
	}

	//-----------------------------------------------------------------------------------
	//アニメーションの状態を変える
	if (GetKeyboardPress(DIK_W) ||
		GetKeyboardPress(DIK_A) ||
		GetKeyboardPress(DIK_S) ||
		GetKeyboardPress(DIK_D)) {
		if (GetKeyboardTrigger(DIK_W)) {
			g_Body.state = 0;
		}
		if (GetKeyboardTrigger(DIK_A)) {
			g_Body.state = 0;
		}
		if (GetKeyboardTrigger(DIK_S)) {
			g_Body.state = 0;
		}
		if (GetKeyboardTrigger(DIK_D)) {
			g_Body.state = 0;
		}
	}
	else if (GetThumbLeftY(0) < -STICK_DEADZONE ||				//右スティック
		GetThumbLeftY(0) > STICK_DEADZONE ||
		GetThumbLeftX(0) < -STICK_DEADZONE ||				//右スティック
		GetThumbLeftX(0) > STICK_DEADZONE) {
		//歩きモーション中は何もしない
		if (g_Body.state != 1) {
			g_Body.state = 0;
		}
	}
	else {
		if (g_Body.state == 3) {
			g_Body.state = 3;
		}
		else if (g_Body.state != 2) {
			g_Body.state = 2;
		}
	}

	//アニメーション
	if (g_Body.state == 0) {
		//手足をそろえてから歩きに行く
		g_LegL.rot = StopAnime(g_pMotion->LegLMotion, 1);
		g_LegR.rot = StopAnime(g_pMotion->LegRMotion, 1);
		g_ArmL.rot = StopAnime(g_pMotion->ArmLMotion, 1);
		g_ArmR.rot = StopAnime(g_pMotion->ArmRMotion, 1);
		g_Head.rot = StopAnime(g_pMotion->HeadMotion, 1);
	}
	//歩き
	if (g_Body.state == 1) {
		g_pMotion = &WalkMotion;
	}
	if (g_Body.state == 2) {
		//歩きから手足をそろえる
		g_LegL.rot = StopAnime(g_pMotion->LegLMotion, 3);
		g_LegR.rot = StopAnime(g_pMotion->LegRMotion, 3);
		g_ArmL.rot = StopAnime(g_pMotion->ArmLMotion, 3);
		g_ArmR.rot = StopAnime(g_pMotion->ArmRMotion, 3);
		g_Head.rot = StopAnime(g_pMotion->HeadMotion, 3);
	}
	//待機モーション
	if (g_Body.state == 3) {
		g_pMotion = &WaitMotion;
	}

	g_LegL.rot = Anime(g_pMotion->LegLMotion, sizeof(g_pMotion->LegLMotion) / sizeof(KeyFrame));
	g_LegR.rot = Anime(g_pMotion->LegRMotion, sizeof(g_pMotion->LegRMotion) / sizeof(KeyFrame));
	g_ArmL.rot = Anime(g_pMotion->ArmLMotion, sizeof(g_pMotion->ArmLMotion) / sizeof(KeyFrame));
	g_ArmR.rot = Anime(g_pMotion->ArmRMotion, sizeof(g_pMotion->ArmRMotion) / sizeof(KeyFrame));
	g_Head.rot = Anime(g_pMotion->HeadMotion, sizeof(g_pMotion->HeadMotion) / sizeof(KeyFrame));




	//歩いた時の音再生-------------------------------------------------------------------
	if (GetKeyboardPress(DIK_D) || GetKeyboardPress(DIK_W) ||
		GetKeyboardPress(DIK_S) || GetKeyboardPress(DIK_A) ||
		GetThumbLeftY(0) < -STICK_DEADZONE ||				//右スティック
		GetThumbLeftY(0) > STICK_DEADZONE ||
		GetThumbLeftX(0) < -STICK_DEADZONE ||				//右スティック
		GetThumbLeftX(0) > STICK_DEADZONE) {
		g_Body.walkframe++;
		if (g_Body.walkframe > 25) {
			//足音を再生
			PlaySound(g_SoundNo[walk], 0);
			g_Body.walkframe = 0;
		}
	}
	//歩いていないとき
	else {
		g_Body.walkframe = 0;
		StopSound(g_SoundNo[walk]);
	}

	//敵との当たり判定-------------------------------------------------------------------
	HitToEnemy();

	//ライフが０以下になったら動けない
	if (GetPlayerLife() <= 0) {
		//ダメージを受けたときの音は再生しない
		StopSound(g_SoundNo[hit]);
		//倒された時の音を再生
		PlaySound(g_SoundNo[dead], 0);
		SetPlayerUse(false);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	if (!GetPlayerUse()) {
		return;
	}
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	//親のマトリックス
	D3DXMATRIX mtxParent;


	//体パーツの変換と描画---------------------------------------------------------------
	D3DXMatrixIdentity(&g_Body.mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_Body.scl.x, g_Body.scl.y, g_Body.scl.z);
	g_Body.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Body.rot.y, g_Body.rot.x, g_Body.rot.z);
	g_Body.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_Body.pos.x, g_Body.pos.y, g_Body.pos.z);
	g_Body.mtxWorld *= mtxTranslate;

	//親のマトリックスの移動と回転だけを更新
	mtxParent = mtxRot;
	mtxParent *= mtxTranslate;

	SetWorldMatrix(&g_Body.mtxWorld);

	if (g_Body.frashframe == 0) {
		//objモデルの描画
		DrawModel(&g_BodyModel);
	}



	//頭パーツの変換と描画---------------------------------------------------------------
	D3DXMatrixIdentity(&g_Head.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_Head.scl.x, g_Head.scl.y, g_Head.scl.z);
	g_Head.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Head.rot.y, g_Head.rot.x, g_Head.rot.z);
	g_Head.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_Head.pos.x, g_Head.pos.y, g_Head.pos.z);
	g_Head.mtxWorld *= mtxTranslate;

	//子のワールドマトリックスに親のマトリックスを反映させる
	g_Head.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_Head.mtxWorld);

	if (g_Body.frashframe == 0) {
		DrawModel(&g_HeadModel);
	}


	//右腕パーツの変換と描画-------------------------------------------------------------
	D3DXMatrixIdentity(&g_ArmR.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_ArmR.scl.x, g_ArmR.scl.y, g_ArmR.scl.z);
	g_ArmR.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ArmR.rot.y, g_ArmR.rot.x, g_ArmR.rot.z);
	g_ArmR.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_ArmR.pos.x, g_ArmR.pos.y, g_ArmR.pos.z);
	g_ArmR.mtxWorld *= mtxTranslate;

	g_ArmR.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_ArmR.mtxWorld);

	if (g_Body.frashframe == 0) {
		DrawModel(&g_ArmRModel);
	}



	//左腕パーツの変換と描画-------------------------------------------------------------
	D3DXMatrixIdentity(&g_ArmL.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_ArmL.scl.x, g_ArmL.scl.y, g_ArmL.scl.z);
	g_ArmL.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_ArmL.rot.y, g_ArmL.rot.x, g_ArmL.rot.z);
	g_ArmL.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_ArmL.pos.x, g_ArmL.pos.y, g_ArmL.pos.z);
	g_ArmL.mtxWorld *= mtxTranslate;

	g_ArmL.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_ArmL.mtxWorld);

	if (g_Body.frashframe == 0) {
		DrawModel(&g_ArmLModel);
	}


	//右脚パーツの変換と描画-------------------------------------------------------------
	D3DXMatrixIdentity(&g_LegR.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_LegR.scl.x, g_LegR.scl.y, g_LegR.scl.z);
	g_LegR.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_LegR.rot.y, g_LegR.rot.x, g_LegR.rot.z);
	g_LegR.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_LegR.pos.x, g_LegR.pos.y, g_LegR.pos.z);
	g_LegR.mtxWorld *= mtxTranslate;

	g_LegR.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_LegR.mtxWorld);


	if (g_Body.frashframe == 0) {
		DrawModel(&g_LegRModel);
	}



	//左脚パーツの変換と描画-------------------------------------------------------------
	D3DXMatrixIdentity(&g_LegL.mtxWorld);

	D3DXMatrixScaling(&mtxScl, g_LegL.scl.x, g_LegL.scl.y, g_LegL.scl.z);
	g_LegL.mtxWorld *= mtxScl;

	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_LegL.rot.y, g_LegL.rot.x, g_LegL.rot.z);
	g_LegL.mtxWorld *= mtxRot;

	D3DXMatrixTranslation(&mtxTranslate, g_LegL.pos.x, g_LegL.pos.y, g_LegL.pos.z);
	g_LegL.mtxWorld *= mtxTranslate;

	g_LegL.mtxWorld *= mtxParent;

	SetWorldMatrix(&g_LegL.mtxWorld);

	if (g_Body.frashframe == 0) {
		DrawModel(&g_LegLModel);
	}
}

//使っている状態かを返す
bool GetPlayerUse(void) {
	return g_Body.isuse;
}
//使っている状態を変える
void SetPlayerUse(const bool& use) {
	g_Body.isuse = use;
}

//ポジションを返す
D3DXVECTOR3 GetPlayerPos() {
	return g_Body.pos;
}

//半径を返す
float GetPlayerRad() {
	return g_Body.rad;
}

//向いている方向を返す
float GetFront(void) {
	return g_Body.front;
}

//角度を返す
D3DXVECTOR3 GetRot(void) {
	return g_Body.rot;
}

//当たったかを入れる
void SetPlayerHit(const bool& hit) {
	g_Body.hit = hit;
}
//当たったかを返す
bool GetPlayerHit(void) {
	return g_Body.hit;
}

//ライフを減らす
void PlayerLifeDim(void) {
	//ダメージを受けたときの音を再生
	PlaySound(g_SoundNo[hit], 0);
	g_Body.life--;
}
//ライフを返す
int GetPlayerLife(void) {
	return g_Body.life;
}

//持っているコインを増やす
void AddPlayerCoinNum(void) {
	g_Body.coin++;
}
//持っているコインを返す
int GetPlayerCoinNum(void) {
	return g_Body.coin;
}

//倒した敵の数を増やす
void AddPlayerkilledNum(void) {
	g_Body.killedenemy++;
}
//倒した敵の数を返す
int GetPlayerkilledNum(void) {
	return g_Body.killedenemy;
}

//終了時点での時間を返す
unsigned int GetEndTime(void) {
	return g_Body.endtime;
}

void HitToEnemy(void) {
	if (GetPlayerHit()) {
		//無敵時間をカウントし始める
		g_Body.nohitframe++;
		g_Body.frashframe++;
	}
	//120を越えたら無敵時間終了
	if (g_Body.nohitframe > PLAYER_NOHIT_FRAME) {
		SetPlayerHit(false);
		g_Body.nohitframe = 0;
		g_Body.frashframe = 0;
	}
	//300を越えたら点滅終了
	if (g_Body.frashframe > PLAYER_FRASH_FRAME) {
		g_Body.frashframe = 0;
	}
}

D3DXVECTOR3 Anime(KeyFrame* keyframe, int size) {
	for (int i = 0; i < size; i++) {
		if (g_frame < keyframe[i].frame) {
			g_keyindex = i - 1;
			break;
		}
	}
	//	  t = (現在のフレーム - スタートキーフレーム)    / (エンドキーフレーム - スタートキーフレーム)
	float t = (g_frame - keyframe[g_keyindex].frame) / (keyframe[g_keyindex + 1].frame - keyframe[g_keyindex].frame);

	D3DXVECTOR3 move;
	D3DXVec3Lerp(&move, &keyframe[g_keyindex].key, &keyframe[g_keyindex + 1].key, t);

	g_frame += 1.0f;
	if (g_frame >= keyframe[size - 1].frame) {
		g_frame = 0.0f;
		g_keyindex++;
	}
	if (g_keyindex >= size - 1) {
		g_keyindex = 0;
	}
	return move;
}

D3DXVECTOR3 StopAnime(KeyFrame* keyframe, int state) {
	float t = g_frame / 25.0f;

	D3DXVECTOR3 move;
	D3DXVec3Lerp(&move, &move, &keyframe[0].key, t);

	g_frame += 1.0f;
	if (g_frame >= 25.0f) {
		g_Body.state = state;
		g_frame = 0.0f;
		g_keyindex++;
	}
	if (g_keyindex >= ANIME_NUM - 1) {
		g_keyindex = 0;
	}
	return move;
}