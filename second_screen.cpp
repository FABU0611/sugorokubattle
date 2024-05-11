//lcoin_screen.cpp		タイム表示処理
//20106_田中　蓮
//23_07_06

#include "second_screen.h"
#include "sprite.h"
#include "texture.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
static const float TIME_WIDTH = 100.0f;		//横幅
static const float TIME_HEIGHT = 150.0f;	//縦幅

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
static int			g_TexNo = 0;		//テクスチャ番号

unsigned int		g_second = 0;		//クリアタイム

//=============================================================================
// 初期化処理
//=============================================================================
void InitSecondscreen(void)
{
	//テクスチャの読み込み
	g_TexNo = LoadTexture((char*)"data/TEXTURE/number001.png");

	//クリアタイムをもらう
	g_second = GetEndTime();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSecondscreen(void)
{
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSecondscreen(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSecondscreen(void)
{
	int second = g_second;
	//表示
	for (int i = 0; i < 3; i++)	{
		//スコアの描画
		DrawSprite(g_TexNo,
			1500.0f - (TIME_WIDTH * i), 400.0f,
			TIME_WIDTH, TIME_HEIGHT,
			ANIME_PTN_U * (second % 10), 0.0f,//UV値の始点
			ANIME_PTN_U, ANIME_PTN_V);

		second = second / 10;
	}
}
