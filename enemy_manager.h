//enemy_manager.h		�G�l�~�[�}�l�[�W���[
//20106_�c���@�@
//23_12_17
#pragma once

#include <list>
#include "enemy.h"
#include "shadow.h"

#include "scene.h"
#include "fade.h"

class ENEMY_MANAGER {
	std::list<ENEMY*> _enemys;

public:
	ENEMY_MANAGER() {}
	~ENEMY_MANAGER() {
		for (auto e : _enemys) {
			if (!e) {
				continue;
			}
			delete e;
		}
	}

	//����������
	void InitEnemyManager() {
		for (auto e : _enemys) {
			e->Init();
		}
	}
	//�I������
	void UninitEnemyManager() {
		for (auto e : _enemys) {
			if (!e) {
				continue;
			}
			e->Uninit();
			e->SetEnemyUse(false);
		}
		EraseEnemy();
	}
	//�X�V����
	void UpdateEnemyManager() {
		for (auto e : _enemys) {
			if (!e->GetEnemyUse()) {
				continue;
			}
			e->Update();
		}
		EraseEnemy();
	}
	//�`�揈��
	void DrawEnemyManager() {
		for (auto e : _enemys) {
			if (!e->GetEnemyUse()) {
				continue;
			}
			e->Draw();
		}
	}

	//�ǉ�
	void Add(ENEMY* enemy) {
		_enemys.emplace_back(enemy);
	}

	//�g���ĂȂ����̂����X�g�������
	void EraseEnemy() {
		_enemys.remove_if([](ENEMY* e) {
			if (!e->GetEnemyUse()) {
				//�e��������
				ReleaseShadow(e->GetEnemyShadow());
				//���f�������
				e->Uninit();
				//���݂�����
				delete e;
				return true;
			}
			return false;
			}
		);
	}

	//���X�g���ɂ���v�f����n��
	size_t GetSize() {
		return _enemys.size();
	}
};