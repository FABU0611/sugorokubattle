//pushspace.cpp	プッシュスペース表示管理
//20106_田中蓮
//23_09_22
#include "pushspace.h"
#include "sprite.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float TEX_WIDTH = 603.0f;	//テクスチャの横幅
static const float TEX_HEIGHT = 265.0f;	//テクスチャの縦幅

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
struct PUSH{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR	color;		//色
	float		flame;		//フレーム
	int			tex_no;		//テクスチャ番号
	bool		lerp_end;	//移動が終わったか
};

float easeOutElastic(float x);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
PUSH g_push;

//=============================================================================
// 初期化処理
//=============================================================================
void InitPushspace(void)
{
	//テクスチャの読み込み
	g_push.tex_no = LoadTexture((char*)"data/TEXTURE/pushspace.png");

	g_push.pos = D3DXVECTOR3(960.0f, 1590.0f, 0.0f);
	g_push.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_push.flame = 0.0f;
	g_push.lerp_end = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPushspace(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePushspace(void)
{
	g_push.pos = D3DXVECTOR3(960.0f, 800.0f, 0.0f);

	//点滅処理
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
// 描画処理
//=============================================================================
void DrawPushspace(void)
{	
	//描画
	DrawSpriteColor(g_push.tex_no,
		g_push.pos.x, g_push.pos.y,
		TEX_WIDTH, TEX_HEIGHT,
		0.0f, 0.0f,
		1.0f, 1.0f,
		g_push.color);
}

//線形補間で移動
void LerpPush(void) {
	if (g_push.lerp_end) {
		return;
	}
	//開始位置
	D3DXVECTOR3 start = D3DXVECTOR3(960.0f, 1592.0f, 0.0f);
	//終了位置
	D3DXVECTOR3 end = D3DXVECTOR3(960.0f, 800.0f, 0.0f);
	float		t = g_push.flame / 90.0f;	//ある程度の数で割る
	g_push.flame += 1.0f;

	//線形補間
	D3DXVec3Lerp(&g_push.pos, &start, &end, easeOutBounce(t));

	if (t >= 1.0f) {
		g_push.flame = 0.0f;
		g_push.pos = end;
		g_push.lerp_end = true;
	}
}

//線形補間が終わったか
bool GetEndLerp(void) {
	return g_push.lerp_end;
}

//イージング関数
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