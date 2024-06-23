//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "inputx.h"
#include "camera.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float		POS_X_CAM = 0.0f;								// カメラの初期位置(X座標)
static const float		POS_Y_CAM = 300.0f;								// カメラの初期位置(Y座標)
static const float		POS_Z_CAM = -700.0f;							// カメラの初期位置(Z座標)

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
const static float		VIEW_NEAR_Z = 10.0f;							// ビュー平面のNearZ値
const static float		VIEW_FAR_Z = 20000.0f;							// ビュー平面のFarZ値

static const float		VALUE_ROTATE_CAMERA = D3DX_PI * 0.01f;			// カメラの回転量

static const float		STICK_DEADZONE = 0.15f;
static const float		CAMERA_SPEED = 0.05f;
//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA			g_Camera;		// カメラデータ
static float			g_Speed;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	g_Camera.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(-D3DX_PI * 0.15f, 0.0f, 0.0f);

	g_Speed = 0.0f;

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);
}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	//左右回転---------------------------------------------------------------------------
	//右スティック
	if (GetThumbRightX(0) < -STICK_DEADZONE ||
		GetThumbRightX(0) > STICK_DEADZONE) {
		g_Camera.rot.y += GetThumbRightX(0) * CAMERA_SPEED;

		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}
	else if (GetKeyboardPress(DIK_L)){// 視点旋回「左」
		g_Camera.rot.y += VALUE_ROTATE_CAMERA;

		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}
	else if (GetKeyboardPress(DIK_J)){// 視点旋回「右」
		g_Camera.rot.y -= VALUE_ROTATE_CAMERA;

		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	//上下回転---------------------------------------------------------------------------
	//右スティック
	if (GetThumbRightY(0) < -STICK_DEADZONE ||
		GetThumbRightY(0) > STICK_DEADZONE) {
		g_Camera.rot.x += GetThumbRightY(0) * CAMERA_SPEED;

		g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	}
	else if (GetKeyboardPress(DIK_I)){// 視点移動「上」
		g_Camera.rot.x += VALUE_ROTATE_CAMERA;

		g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	}
	else if (GetKeyboardPress(DIK_K)){// 視点移動「下」
		g_Camera.rot.x -= VALUE_ROTATE_CAMERA;

		g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	}


	if (g_Camera.rot.x > D3DX_PI * 0.3f){
		g_Camera.rot.x = D3DX_PI * 0.3f;
	}
	if (g_Camera.rot.x < -D3DX_PI * 0.3f){
		g_Camera.rot.x = -D3DX_PI * 0.3f;
	}

	// カメラを初期に戻す
	if (GetKeyboardPress(DIK_P) ||
		IsButtonTriggered(0, XINPUT_GAMEPAD_Y))	{
		g_Camera.rot.y = GetRot().y;
	}
}


//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void) 
{
	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&g_Camera.mtxView, &g_Camera.pos, &g_Camera.at, &g_Camera.up);

	SetViewMatrix(&g_Camera.mtxView);


	float det;
	D3DXMatrixInverse(&g_Camera.mtxInvView, &det, &g_Camera.mtxView);


	// プロジェクションマトリックス設定
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&g_Camera.mtxProjection);
}


//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}


// カメラの視点と注視点をセット
void SetCameraAT(D3DXVECTOR3 pos)
{
	// カメラの注視点をプレイヤーの座標にしてみる
	g_Camera.at = pos;

	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len * cosf(g_Camera.rot.x);
	g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len * cosf(g_Camera.rot.x);
}

