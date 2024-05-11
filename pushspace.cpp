//pushspace.cpp	�v�b�V���X�y�[�X�\���Ǘ�
//20106_�c���@
//23_09_22
#include "pushspace.h"
#include "sprite.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
static const float TEX_WIDTH = 603.0f;	//�e�N�X�`���̉���
static const float TEX_HEIGHT = 265.0f;	//�e�N�X�`���̏c��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
struct PUSH{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR	color;		//�F
	float		flame;		//�t���[��
	int			tex_no;		//�e�N�X�`���ԍ�
	bool		lerp_end;	//�ړ����I�������
};

float easeOutElastic(float x);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PUSH g_push;

//=============================================================================
// ����������
//=============================================================================
void InitPushspace(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_push.tex_no = LoadTexture((char*)"data/TEXTURE/pushspace.png");

	g_push.pos = D3DXVECTOR3(960.0f, 1590.0f, 0.0f);
	g_push.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_push.flame = 0.0f;
	g_push.lerp_end = false;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPushspace(void)
{
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePushspace(void)
{
	g_push.pos = D3DXVECTOR3(960.0f, 800.0f, 0.0f);

	//�_�ŏ���
	g_push.flame += 1.0f;
	if (g_push.flame == 20) {
		g_push.color.a = 1.0f;
	}
	if (g_push.flame == 40) {
		g_push.color.a = 0.0f;
		g_push.flame = 0;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPushspace(void)
{	
	//�`��
	DrawSpriteColor(g_push.tex_no,
		g_push.pos.x, g_push.pos.y,
		TEX_WIDTH, TEX_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f,
		g_push.color);
}

//���`��Ԃňړ�
void LerpPush(void) {
	if (g_push.lerp_end) {
		return;
	}
	//�J�n�ʒu
	D3DXVECTOR3 start = D3DXVECTOR3(960.0f, 1592.0f, 0.0f);
	//�I���ʒu
	D3DXVECTOR3 end = D3DXVECTOR3(960.0f, 800.0f, 0.0f);
	float		t = g_push.flame / 90.0f;	//������x�̐��Ŋ���
	g_push.flame += 1.0f;

	//���`���
	D3DXVec3Lerp(&g_push.pos, &start, &end, easeOutBounce(t));

	if (t >= 1.0f) {
		g_push.flame = 0.0f;
		g_push.pos = end;
		g_push.lerp_end = true;
	}
}

//���`��Ԃ��I�������
bool GetEndLerp(void) {
	return g_push.lerp_end;
}

//�C�[�W���O�֐�
float easeOutElastic(float x) {
	const float c4 = (2 * D3DX_PI) / 3;

	if (x <= 0) {
		return 0;
	}
	if (x >= 1) {
		return 1;
	}
	
	return powf(-3 * x, 2) * sinf((x * 3 - 0.75f) * c4) + 1;
	
}