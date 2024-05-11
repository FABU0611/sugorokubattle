//logo.cpp	ロゴ表示管理
//20106_田中蓮
//23_09_22
#include "logo.h"
#include "sprite.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
const static float TEX_WIDTH = 1000.0f;	//テクスチャの横幅
const static float TEX_HEIGHT = 200.0f;	//テクスチャの縦幅

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
struct LOGO{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR2 size;		//サイズ
	D3DXCOLOR	color;		//色
	float		flame;		//フレーム
	int			tex_no;		//テクスチャ番号
	bool		lerp_end;	//アニメーションが終わったか
};

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LOGO g_logo;

//=============================================================================
// 初期化処理
//=============================================================================
void InitLogo(void)
{
	//テクスチャの読み込み
	g_logo.tex_no = LoadTexture((char*)"data/TEXTURE/logo.png");

	g_logo.pos = D3DXVECTOR3(960.0f, -400.0f, 0.0f);
	g_logo.size = D3DXVECTOR2(TEX_WIDTH * 2, TEX_HEIGHT * 2);
	g_logo.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_logo.flame = 0.0f;
	g_logo.lerp_end = false;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLogo(void)
{

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLogo(void)
{
	g_logo.pos = D3DXVECTOR3(960.0f, 300.0f, 0.0f);
	g_logo.color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_logo.size = D3DXVECTOR2(TEX_WIDTH, TEX_HEIGHT);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLogo(void)
{	
	//背景の描画
	DrawSpriteColor(g_logo.tex_no,
		g_logo.pos.x, g_logo.pos.y,
		g_logo.size.x, g_logo.size.y,
		0.0f, 0.0f,
		1.0f, 1.0f,
		g_logo.color);
}

//線形補間
void LerpLogo(void) {
	if (g_logo.lerp_end) {
		return;
	}
	//開始地点
	D3DXVECTOR3 start = D3DXVECTOR3(960.0f, 0.0f, 0.0f);
	//終了地点
	D3DXVECTOR3 end = D3DXVECTOR3(960.0f, 300.0f, 0.0f);

	//開始時の色
	D3DXCOLOR	scolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	//終了時の色
	D3DXCOLOR	ecolor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//開始時のサイズ
	D3DXVECTOR2 ssize = D3DXVECTOR2(TEX_WIDTH * 2, TEX_HEIGHT * 2);
	//終了時のサイズ
	D3DXVECTOR2 esize = D3DXVECTOR2(TEX_WIDTH, TEX_HEIGHT);


	float		t = g_logo.flame / 60.0f;	//ある程度の数で割る
	g_logo.flame += 1.0f;

	t = 1.0f - (1.0f - t) * (1.0f - t);

	//位置の線形補間
	D3DXVec3Lerp(&g_logo.pos, &start, &end, t);
	//サイズの線形補間
	D3DXVec2Lerp(&g_logo.size, &ssize, &esize, t);
	//色の線形補間
	D3DXColorLerp(&g_logo.color, &scolor, &ecolor, t);
	if (t >= 1.0f) {
		g_logo.flame = 0.0f;
		g_logo.pos = end;
		g_logo.size = esize;
		g_logo.color = ecolor;
		g_logo.lerp_end = true;
	}
}

//線形補間が終わったかを変えす
bool GetEndLerpLogo(void) {
	return g_logo.lerp_end;
}