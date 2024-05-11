//logo.cpp	���S�\���Ǘ�
//20106_�c���@
//23_09_22
#include "logo.h"
#include "sprite.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
const static float TEX_WIDTH = 1000.0f;	//�e�N�X�`���̉���
const static float TEX_HEIGHT = 200.0f;	//�e�N�X�`���̏c��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
struct LOGO{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR2 size;		//�T�C�Y
	D3DXCOLOR	color;		//�F
	float		flame;		//�t���[��
	int			tex_no;		//�e�N�X�`���ԍ�
	bool		lerp_end;	//�A�j���[�V�������I�������
};

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LOGO g_logo;

//=============================================================================
// ����������
//=============================================================================
void InitLogo(void)
{
	//�e�N�X�`���̓ǂݍ���
	g_logo.tex_no = LoadTexture((char*)"data/TEXTURE/logo.png");

	g_logo.pos = D3DXVECTOR3(960.0f, -400.0f, 0.0f);
	g_logo.size = D3DXVECTOR2(TEX_WIDTH * 2, TEX_HEIGHT * 2);
	g_logo.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_logo.flame = 0.0f;
	g_logo.lerp_end = false;
}

//=============================================================================
// �I������
//=============================================================================
void UninitLogo(void)
{

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLogo(void)
{
	g_logo.pos = D3DXVECTOR3(960.0f, 300.0f, 0.0f);
	g_logo.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_logo.size = D3DXVECTOR2(TEX_WIDTH, TEX_HEIGHT);
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLogo(void)
{	
	//�w�i�̕`��
	DrawSpriteColor(g_logo.tex_no,
		g_logo.pos.x, g_logo.pos.y,
		g_logo.size.x, g_logo.size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		g_logo.color);
}

//���`���
void LerpLogo(void) {
	if (g_logo.lerp_end) {
		return;
	}
	//�J�n�n�_
	D3DXVECTOR3 start = D3DXVECTOR3(960.0f, 0.0f, 0.0f);
	//�I���n�_
	D3DXVECTOR3 end = D3DXVECTOR3(960.0f, 300.0f, 0.0f);

	//�J�n���̐F
	D3DXCOLOR	scolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//�I�����̐F
	D3DXCOLOR	ecolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�J�n���̃T�C�Y
	D3DXVECTOR2 ssize = D3DXVECTOR2(TEX_WIDTH * 2, TEX_HEIGHT * 2);
	//�I�����̃T�C�Y
	D3DXVECTOR2 esize = D3DXVECTOR2(TEX_WIDTH, TEX_HEIGHT);


	float		t = g_logo.flame / 60.0f;	//������x�̐��Ŋ���
	g_logo.flame += 1.0f;

	t = 1.0f - (1.0f - t) * (1.0f - t);

	//�ʒu�̐��`���
	D3DXVec3Lerp(&g_logo.pos, &start, &end, t);
	//�T�C�Y�̐��`���
	D3DXVec2Lerp(&g_logo.size, &ssize, &esize, t);
	//�F�̐��`���
	D3DXColorLerp(&g_logo.color, &scolor, &ecolor, t);
	if (t >= 1.0f) {
		g_logo.flame = 0.0f;
		g_logo.pos = end;
		g_logo.size = esize;
		g_logo.color = ecolor;
		g_logo.lerp_end = true;
	}
}

//���`��Ԃ��I���������ς���
bool GetEndLerpLogo(void) {
	return g_logo.lerp_end;
}