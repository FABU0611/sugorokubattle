//scene.h	シーン管理
//20106_田中　蓮
//23_11_30
#pragma once

enum SCENE {
	SCENE_NONE,		//シーンなし

	SCENE_TITLE,	//タイトルシーン
	SCENE_GAME,		//ゲームシーン
	SCENE_RESULT,	//リザルトシーン

	SCENE_NUM,		//終端コード
};

void InitScene();
void UninitScene();
void UpdateScene();
void DrawScene();


void SetScene(SCENE next);		//移行先シーンを設定
void CheckScene();				//移行先シーンがセットされているか