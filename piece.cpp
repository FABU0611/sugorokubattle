//piece.cpp	駒表示管理
//20106_田中蓮
//23_12_07
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "model.h"
#include "input.h"
#include "sound.h"

#include "piece.h"
#include "shadow.h"
#include "dice.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
struct PIECE{
	D3DXVECTOR3 pos;		//ポジション
	D3DXVECTOR3 start_pos;	//出発ポジション
	D3DXVECTOR3 next_pos;	//到着ポジション
	D3DXVECTOR3 dir;		//向き
	D3DXVECTOR3 rot;		//角度
	D3DXVECTOR3 scl;		//スケール

	float		front;		//向いている方向
	float		frame;		//フレームカウント用

	int			move_num;	//進むマス数

	bool		can_move;	//動けるか
	bool		play_sound;	//音を再生したか

	int			shadowID;	//影ID
	int			polylineID;	//ポリラインID

	D3DXMATRIX  mtxWorld;
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MovePiece(const D3DXVECTOR3& s_pos, const D3DXVECTOR3& e_pos);


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static DX11_MODEL	g_Model;	// モデル読み込み
static PIECE		g_Piece;	// プレイヤー情報		
static int			g_SoundNo;	//サウンド番号

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPiece(void)
{
	//objモデルの読み込み
	LoadModel((char*)"data/MODEL/kuma.obj", &g_Model);

	//サウンドの読み込み
	g_SoundNo = LoadSound((char*)"data/sound/piece_move.wav");
	SetVolume(g_SoundNo, 0.2f);

	//初期設定
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

	//影のセット
	g_Piece.shadowID = SetShadow(g_Piece.pos, 200.0f);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPiece(void)
{
	if (&g_Model != NULL) {
		//objモデルの解放
		UnloadModel(&g_Model);
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePiece(void)
{
	//現在地と次のマスの位置が同じなら更新する
	if (g_Piece.pos == g_Piece.next_pos) {
		g_Piece.start_pos = g_Piece.next_pos;
	}
	//目的地に動く
	if (GetMovenum() > 0) {
		MovePiece(g_Piece.start_pos, g_Piece.next_pos);
	}

	//進むマス数が０以下になったら止まる
	if (GetMovenum() <= 0) {
		g_Piece.can_move = false;
	}

	//影の座標更新
	D3DXVECTOR3 shadowpos = g_Piece.pos;
	shadowpos.y = - 75.0f;
	SetPositionShadow(g_Piece.shadowID, shadowpos);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPiece(void)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	// ワールドマトリックスの初期化（単位行列を作る関数）
	D3DXMatrixIdentity(&g_Piece.mtxWorld);

	// スケールを反映
	D3DXMatrixScaling(&mtxScl, g_Piece.scl.x, g_Piece.scl.y, g_Piece.scl.z);

	                   //第1引数 ＝          第2引数      ＊    第3引数
	D3DXMatrixMultiply(&g_Piece.mtxWorld, &g_Piece.mtxWorld, &mtxScl);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Piece.rot.y, g_Piece.rot.x, g_Piece.rot.z);
	D3DXMatrixMultiply(&g_Piece.mtxWorld, &g_Piece.mtxWorld, &mtxRot);

	// 移動を反映
	D3DXMatrixTranslation(&mtxTranslate, g_Piece.pos.x, g_Piece.pos.y, g_Piece.pos.z);
	D3DXMatrixMultiply(&g_Piece.mtxWorld, &g_Piece.mtxWorld, &mtxTranslate);

	// ワールドマトリックスの設定
	SetWorldMatrix(&g_Piece.mtxWorld);

	//objモデルの描画
	DrawModel(&g_Model);
}


//向いている方向を返す
float GetPieceFront(void) {
	return g_Piece.front;
}

//コマを動かす
void MovePiece(const D3DXVECTOR3& s_pos, const D3DXVECTOR3& n_pos) {
	if (!g_Piece.can_move) {
		return;
	}
	//音再生
	if (!g_Piece.play_sound) {
		PlaySound(g_SoundNo, 0); 
		g_Piece.play_sound = true;
	}
	//角度を出す
	g_Piece.front = atan2f(s_pos.z - n_pos.z, s_pos.x - n_pos.x);
	//角度を入れる
	g_Piece.dir.x = -cosf(g_Piece.front);
	g_Piece.dir.z = -sinf(g_Piece.front);
	//進む方向に向かせる
	g_Piece.rot.y = atan2f(-g_Piece.dir.z, g_Piece.dir.x) + (D3DX_PI / 2);

	//線形補間で移動
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

//位置をもらう
void SetNextPos(D3DXVECTOR3 pos) {
	if (g_Piece.pos == pos) {
		return;
	}
	g_Piece.next_pos = pos;
}
//位置を返す
D3DXVECTOR3 GetNowPos(void) {
	return g_Piece.pos;
}

//動く状態かをもらう
void SetCanMove(const bool& canmove) {
	g_Piece.can_move = canmove;
}
//動く状態かを返す
bool GetCanMove(void) {
	return g_Piece.can_move;
}

//進める数をもらう
void SetMovenum(const int& movenum) {
	g_Piece.move_num = movenum;
}
//進める数を渡す
const int& GetMovenum(void) {
	return g_Piece.move_num;
}