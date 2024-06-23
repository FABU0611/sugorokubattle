//piece.cpp	��\���Ǘ�
//20106_�c���@
//23_12_07
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "model.h"
#include "sound.h"

#include "piece.h"
#include "shadow.h"
#include "dice.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
struct PIECE{
	D3DXVECTOR3 pos;		//�|�W�V����
	D3DXVECTOR3 start_pos;	//�o���|�W�V����
	D3DXVECTOR3 next_pos;	//�����|�W�V����
	D3DXVECTOR3 dir;		//����
	D3DXVECTOR3 rot;		//�p�x
	D3DXVECTOR3 scl;		//�X�P�[��

	float		front;		//�����Ă������
	float		frame;		//�t���[���J�E���g�p

	int			move_num;	//�i�ރ}�X��

	bool		can_move;	//�����邩
	bool		play_sound;	//�����Đ�������

	int			shadowID;	//�eID
	int			polylineID;	//�|�����C��ID

	D3DXMATRIX  mtxWorld;
};


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MovePiece(const D3DXVECTOR3& s_pos, const D3DXVECTOR3& e_pos);


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static DX11_MODEL	g_Model;	// ���f���ǂݍ���
static PIECE		g_Piece;	// �v���C���[���		
static int			g_SoundNo;	//�T�E���h�ԍ�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPiece(void)
{
	//obj���f���̓ǂݍ���
	LoadModel((char*)"data/MODEL/kuma.obj", &g_Model);

	//�T�E���h�̓ǂݍ���
	g_SoundNo = LoadSound((char*)"data/sound/piece_move.wav");
	SetVolume(g_SoundNo, 0.2f);

	//�����ݒ�
	g_Piece.pos = D3DXVECTOR3(0.0f, 0.0f, -1500.0f);

	g_Piece.start_pos = g_Piece.pos;
	g_Piece.next_pos = D3DXVECTOR3(0.0f, 0.0f, -300.0f);

	g_Piece.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Piece.dir = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Piece.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	g_Piece.front = 0.0f;
	g_Piece.frame = 0.0f;

	g_Piece.move_num = 1;
	
	g_Piece.can_move = true;
	g_Piece.play_sound = false;

	//�e�̃Z�b�g
	g_Piece.shadowID = SetShadow(g_Piece.pos, 200.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPiece(void)
{
	if (&g_Model != NULL) {
		//obj���f���̉��
		UnloadModel(&g_Model);
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePiece(void)
{
	//���ݒn�Ǝ��̃}�X�̈ʒu�������Ȃ�X�V����
	if (g_Piece.pos == g_Piece.next_pos) {
		g_Piece.start_pos = g_Piece.next_pos;
	}
	//�ړI�n�ɓ���
	if (GetMovenum() > 0) {
		MovePiece(g_Piece.start_pos, g_Piece.next_pos);
	}

	//�i�ރ}�X�����O�ȉ��ɂȂ�����~�܂�
	if (GetMovenum() <= 0) {
		g_Piece.can_move = false;
	}

	//�e�̍��W�X�V
	D3DXVECTOR3 shadowpos = g_Piece.pos;
	shadowpos.y = - 75.0f;
	SetPositionShadow(g_Piece.shadowID, shadowpos);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPiece(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏������i�P�ʍs������֐��j
	D3DXMatrixIdentity(&g_Piece.mtxWorld);

	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, g_Piece.scl.x, g_Piece.scl.y, g_Piece.scl.z);

	                   //��1���� ��          ��2����      ��    ��3����
	D3DXMatrixMultiply(&g_Piece.mtxWorld, &g_Piece.mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Piece.rot.y, g_Piece.rot.x, g_Piece.rot.z);
	D3DXMatrixMultiply(&g_Piece.mtxWorld, &g_Piece.mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_Piece.pos.x, g_Piece.pos.y, g_Piece.pos.z);
	D3DXMatrixMultiply(&g_Piece.mtxWorld, &g_Piece.mtxWorld, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	SetWorldMatrix(&g_Piece.mtxWorld);

	//obj���f���̕`��
	DrawModel(&g_Model);
}


//�����Ă��������Ԃ�
float GetPieceFront(void) {
	return g_Piece.front;
}

//�R�}�𓮂���
void MovePiece(const D3DXVECTOR3& s_pos, const D3DXVECTOR3& n_pos) {
	if (!g_Piece.can_move) {
		return;
	}
	//���Đ�
	if (!g_Piece.play_sound) {
		PlaySound(g_SoundNo, 0); 
		g_Piece.play_sound = true;
	}
	//�p�x���o��
	g_Piece.front = atan2f(s_pos.z - n_pos.z, s_pos.x - n_pos.x);
	//�p�x������
	g_Piece.dir.x = -cosf(g_Piece.front);
	g_Piece.dir.z = -sinf(g_Piece.front);
	//�i�ޕ����Ɍ�������
	g_Piece.rot.y = atan2f(-g_Piece.dir.z, g_Piece.dir.x) + (D3DX_PI / 2);

	//���`��Ԃňړ�
	float t = g_Piece.frame / 60.0f;
	g_Piece.frame += 1.0f;
	D3DXVec3Lerp(&g_Piece.pos, &s_pos, &n_pos, t);
	if (t >= 1.0f) {
		g_Piece.play_sound = false;
		g_Piece.frame = 0.0f;
		g_Piece.move_num--;
		g_Piece.pos = g_Piece.next_pos;
	}
}

//�ʒu�����炤
void SetNextPos(D3DXVECTOR3 pos) {
	if (g_Piece.pos == pos) {
		return;
	}
	g_Piece.next_pos = pos;
}
//�ʒu��Ԃ�
D3DXVECTOR3 GetNowPos(void) {
	return g_Piece.pos;
}

//������Ԃ������炤
void SetCanMove(const bool& canmove) {
	g_Piece.can_move = canmove;
}
//������Ԃ���Ԃ�
bool GetCanMove(void) {
	return g_Piece.can_move;
}

//�i�߂鐔�����炤
void SetMovenum(const int& movenum) {
	g_Piece.move_num = movenum;
}
//�i�߂鐔��n��
const int& GetMovenum(void) {
	return g_Piece.move_num;
}