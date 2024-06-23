//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "inputx.h"
#include "camera.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float		POS_X_CAM = 0.0f;								// �J�����̏����ʒu(X���W)
static const float		POS_Y_CAM = 300.0f;								// �J�����̏����ʒu(Y���W)
static const float		POS_Z_CAM = -700.0f;							// �J�����̏����ʒu(Z���W)

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��	
const static float		VIEW_NEAR_Z = 10.0f;							// �r���[���ʂ�NearZ�l
const static float		VIEW_FAR_Z = 20000.0f;							// �r���[���ʂ�FarZ�l

static const float		VALUE_ROTATE_CAMERA = D3DX_PI * 0.01f;			// �J�����̉�]��

static const float		STICK_DEADZONE = 0.15f;
static const float		CAMERA_SPEED = 0.05f;
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static CAMERA			g_Camera;		// �J�����f�[�^
static float			g_Speed;

//=============================================================================
// ����������
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	g_Camera.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(-D3DX_PI * 0.15f, 0.0f, 0.0f);

	g_Speed = 0.0f;

	// ���_�ƒ����_�̋������v�Z
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);
}


//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	//���E��]---------------------------------------------------------------------------
	//�E�X�e�B�b�N
	if (GetThumbRightX(0) < -STICK_DEADZONE ||
		GetThumbRightX(0) > STICK_DEADZONE) {
		g_Camera.rot.y += GetThumbRightX(0) * CAMERA_SPEED;

		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}
	else if (GetKeyboardPress(DIK_L)){// ���_����u���v
		g_Camera.rot.y += VALUE_ROTATE_CAMERA;

		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}
	else if (GetKeyboardPress(DIK_J)){// ���_����u�E�v
		g_Camera.rot.y -= VALUE_ROTATE_CAMERA;

		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	}

	//�㉺��]---------------------------------------------------------------------------
	//�E�X�e�B�b�N
	if (GetThumbRightY(0) < -STICK_DEADZONE ||
		GetThumbRightY(0) > STICK_DEADZONE) {
		g_Camera.rot.x += GetThumbRightY(0) * CAMERA_SPEED;

		g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	}
	else if (GetKeyboardPress(DIK_I)){// ���_�ړ��u��v
		g_Camera.rot.x += VALUE_ROTATE_CAMERA;

		g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	}
	else if (GetKeyboardPress(DIK_K)){// ���_�ړ��u���v
		g_Camera.rot.x -= VALUE_ROTATE_CAMERA;

		g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	}


	if (g_Camera.rot.x > D3DX_PI * 0.3f){
		g_Camera.rot.x = D3DX_PI * 0.3f;
	}
	if (g_Camera.rot.x < -D3DX_PI * 0.3f){
		g_Camera.rot.x = -D3DX_PI * 0.3f;
	}

	// �J�����������ɖ߂�
	if (GetKeyboardPress(DIK_P) ||
		IsButtonTriggered(0, XINPUT_GAMEPAD_Y))	{
		g_Camera.rot.y = GetRot().y;
	}
}


//=============================================================================
// �J�����̍X�V
//=============================================================================
void SetCamera(void) 
{
	// �r���[�}�g���b�N�X�ݒ�
	D3DXMatrixLookAtLH(&g_Camera.mtxView, &g_Camera.pos, &g_Camera.at, &g_Camera.up);

	SetViewMatrix(&g_Camera.mtxView);


	float det;
	D3DXMatrixInverse(&g_Camera.mtxInvView, &det, &g_Camera.mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);

	SetProjectionMatrix(&g_Camera.mtxProjection);
}


//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}


// �J�����̎��_�ƒ����_���Z�b�g
void SetCameraAT(D3DXVECTOR3 pos)
{
	// �J�����̒����_���v���C���[�̍��W�ɂ��Ă݂�
	g_Camera.at = pos;

	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len * cosf(g_Camera.rot.x);
	g_Camera.pos.y = g_Camera.at.y - sinf(g_Camera.rot.x) * g_Camera.len;
	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len * cosf(g_Camera.rot.x);
}

