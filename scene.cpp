//scene.cpp	�V�[���Ǘ�
//20106_�c���@�@
//23_11_30
#include "scene.h"
#include "title.h"
#include "game.h"
#include "result.h"

SCENE g_Scene = SCENE_NONE;		//���ݎ��s���̃V�[��
SCENE g_SceneNext = SCENE_NONE;	//�ڍs��̃V�[��

//=============================================================================
// ����������
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
// �I������
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
// �X�V����
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
// �`�揈��
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


//�ڍs�������V�[�����Z�b�g����
void SetScene(SCENE next){
	g_SceneNext = next;
}

//���̃V�[�������邩�����Ă���Ȃ�؂�ւ���
void CheckScene(){
	if (g_Scene == g_SceneNext) {
		return;
	}
	//���ݎ��s���̃V�[�����I��������
	UninitScene();

	//���s���̃V�[�����X�V����
	g_Scene = g_SceneNext;
	
	//���ݎ��s���̃V�[��������������
	InitScene();
}
