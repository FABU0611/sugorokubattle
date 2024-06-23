//life.cpp		ライフ表示処理
//20106_田中　蓮
//23_07_06

#include "life.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float LIFE_WIDTH = 50.0f;				//字の横幅
static const float LIFE_HEIGHT = 100.0f;			//字の縦幅

const static int ANIME_PTN_YOKO = 10;
const static int ANIME_PTN_TATE = 1;
const static int ANIME_PTN_WAIT = 5;

const static float ANIME_PTN = ANIME_PTN_YOKO * ANIME_PTN_TATE;
const static float ANIME_PTN_U = 1.0f / ANIME_PTN_YOKO;
const static float ANIME_PTN_V = 1.0f / ANIME_PTN_TATE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static int			g_TexNo[2];		//テクスチャ番号
static int			g_Life = 0;			//終了時のライフ

//=============================================================================
// 初期化処理
//=============================================================================
void InitLife(void)
{
	//テクスチャの読み込み
	g_TexNo[0] = LoadTexture((char*)"data/TEXTURE/number001.png");
	g_TexNo[1] = LoadTexture((char*)"data/TEXTURE/life.png");

	//終了時のライフをもらう
	g_Life = GetPlayerLife();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife(void)
{
	//随時更新
	g_Life = GetPlayerLife();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife(void)
{
	int life = g_Life;
	//表示
	for (int i = 0; i < 2; i++)	{
		//スコアの描画
		DrawSprite(g_TexNo[0],
			200.0f - (LIFE_WIDTH * i), 70.0f,
			LIFE_WIDTH, LIFE_HEIGHT,
			ANIME_PTN_U * (life % 10), 0.0f,//UV値の始点
			ANIME_PTN_U, ANIME_PTN_V);

		life = life / 10;
	}
	DrawSprite(g_TexNo[1],
		80.0f, 70.0f,
		100.0f, LIFE_HEIGHT,
		0.0f, 0.0f,//UV値の始点
		1.0f, 1.0f);
}
