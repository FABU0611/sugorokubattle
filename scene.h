//scene.h	�V�[���Ǘ�
//20106_�c���@�@
//23_11_30
#pragma once

enum SCENE {
	SCENE_NONE,		//�V�[���Ȃ�

	SCENE_TITLE,	//�^�C�g���V�[��
	SCENE_GAME,		//�Q�[���V�[��
	SCENE_RESULT,	//���U���g�V�[��

	SCENE_NUM,		//�I�[�R�[�h
};

void InitScene();
void UninitScene();
void UpdateScene();
void DrawScene();


void SetScene(SCENE next);		//�ڍs��V�[����ݒ�
void CheckScene();				//�ڍs��V�[�����Z�b�g����Ă��邩