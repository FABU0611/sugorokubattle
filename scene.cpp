//scene.cpp	シーン管理
//20106_田中　蓮
//23_11_30
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"

SCENE g_Scene = SCENE_NONE;		//現在実行中のシーン
SCENE g_SceneNext = SCENE_NONE;	//移行先のシーン

//=============================================================================
// 初期化処理
//=============================================================================
void InitScene(){
	switch (g_Scene) {
	case SCENE_TITLE:
		InitTitle();
		break;
	case SCENE_GAME:
		InitGame();
		break;
	case SCENE_RESULT:
		InitResult();
		break;
	}
}


//=============================================================================
// 終了処理
//=============================================================================
void UninitScene(){
	switch (g_Scene) {
	case SCENE_TITLE:
		UninitTitle();
		break;
	case SCENE_GAME:
		UninitGame();
		break;
	case SCENE_RESULT:
		UninitResult();
		break;
	}
}


//=============================================================================
// 更新処理
//=============================================================================
void UpdateScene(){
	switch (g_Scene) {
	case SCENE_TITLE:
		UpdateTitle();
		break;
	case SCENE_GAME:
		UpdateGame();
		break;
	case SCENE_RESULT:
		UpdateResult();
		break;
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawScene(){
	switch (g_Scene) {
	case SCENE_TITLE:
		DrawTitle();
		break;
	case SCENE_GAME:
		DrawGame();
		break;
	case SCENE_RESULT:
		DrawResult();
		break;
	}
}


//移行したいシーンをセットする
void SetScene(SCENE next){
	g_SceneNext = next;
}

//次のシーンがあるかを見てあるなら切り替える
void CheckScene(){
	if (g_Scene == g_SceneNext) {
		return;
	}
	//現在実行中のシーンを終了させる
	UninitScene();

	//実行中のシーンを更新する
	g_Scene = g_SceneNext;
	
	//現在実行中のシーンを初期化する
	InitScene();
}
