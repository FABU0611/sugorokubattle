//enemy.cpp		敵表示処理
//20106_田中　蓮
//23_12_18
#include "main.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "shadow.h"
#include "bullet.h"
#include "collision.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	VALUE_ROTATE	(D3DX_PI * 0.02f)			// 回転量

const float ENEMY::SIZE = 5.0f;						//サイズ
const float ENEMY::SPEED = 2.5f;					//スピード


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************


//=============================================================================
// 初期化処理
//=============================================================================
void ENEMY::Init(void)
{
	//objモデルの読み込み
	LoadModel((char*)"data/MODEL/sword.obj", GetEnemyModel());

	//サウンド読み込み
	_sound_no = LoadSound((char*)"data/sound/enemy_dead.wav");
	SetVolume(_sound_no, 0.3f);

	// 位置・回転・スケールの初期設定
	SetEnemyShadowPos({ GetEnemyPos().x ,GetEnemyPos().y - 75.0f, GetEnemyPos().z });
	SetEnmeyRot({ 0.0f, 0.0f, 0.0f });
	SetEnmeyScl({ SIZE, SIZE, SIZE });

	SetEnemySpeed(SPEED);
	SetEnemyFlont(0.0f);
	SetEnemyUse(true);

	SetEnemyShadowID(_shadowpos, 150.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void ENEMY::Uninit(void)
{
	//objモデルの解放
	UnloadModel(GetEnemyModel());
}

//=============================================================================
// 更新処理
//=============================================================================
void ENEMY::Update(void)
{
	if (!_isUse) {
		//使っていないものは更新しない
		return;
	}
	//プレイヤーの方向に進む-------------------------------------------------------------
	//角度を出す
	SetEnemyFlont(atan2f(GetEnemyPos().z - GetPlayerPos().z, GetEnemyPos().x - GetPlayerPos().x));
	//角度を入れる
	SetEnmeyDir({ -cosf(GetEnemyFront()), 0.0f, -sinf(GetEnemyFront()) });
	//進む方向に向かせる
	SetEnmeyRot({ 0.0f, atan2f(-GetEnemyDir().z, GetEnemyDir().x) + (D3DX_PI / 2), 0.0f });


	//座標更新
	_pos.x += sinf(GetEnemyRot().y) * GetEnemySpeed();
	_pos.z += cosf(GetEnemyRot().y) * GetEnemySpeed();

	//影の位置を更新
	SetEnemyShadowPos({ GetEnemyPos().x ,GetEnemyPos().y - 75.0f, GetEnemyPos().z });
	//位置を反映させる
	SetPositionShadow(GetEnemyShadow(), GetEnemyShadowPos());

	//弾との当たり判定
	for (int b = 0; b < NUM_BULLET; b++) {
		if (!GetBulletUse(b)) {
			continue;
		}
		if (CheckHitBS(GetEnemyPos(), GetEnemyRad(),
			GetBulletPos(b), GetBulletRad())) {
			EraseBullet(b);
			DeadEnemy();
		}
	}

	//プレイヤーとの当たり判定
	{
		//生きている間だけ判定する
		if (!GetPlayerUse()) {
			SetEnemyUse(false);
			return;
		}
		//当たってない状態で判定
		if (!GetPlayerHit() &&
			CheckHitBS(GetEnemyPos(), GetEnemyRad(),
			GetPlayerPos(), GetPlayerRad())) {
			PlayerLifeDim();
			AddPlayerkilledNum();
			SetPlayerHit(true);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void ENEMY::Draw(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	if (!_isUse) {
		//使っていないものは描画しない
		return;
	}
	// ワールドマトリックスの初期化（単位行列を作る関数）
	D3DXMatrixIdentity(GetEnemymtxworld());

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, GetEnemyScl().x, GetEnemyScl().y, GetEnemyScl().z);

	//第1引数 ＝          第2引数      ＊    第3引数
	D3DXMatrixMultiply(GetEnemymtxworld(), GetEnemymtxworld(), &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetEnemyRot().y, GetEnemyRot().x, GetEnemyRot().z);
	D3DXMatrixMultiply(GetEnemymtxworld(), GetEnemymtxworld(), &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, GetEnemyPos().x, GetEnemyPos().y, GetEnemyPos().z);
	D3DXMatrixMultiply(GetEnemymtxworld(), GetEnemymtxworld(), &mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(GetEnemymtxworld());

	//objモデルの描画
	DrawModel(GetEnemyModel());
}

//敵が倒される
void ENEMY::DeadEnemy(){
	//倒された時の音を再生
	PlaySound(_sound_no, 0);
	//使っていない状態にする
	SetEnemyUse(false);
}
