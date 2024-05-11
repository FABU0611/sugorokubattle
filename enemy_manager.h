//enemy_manager.h		エネミーマネージャー
//20106_田中　蓮
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

	//初期化処理
	void InitEnemyManager() {
		for (auto e : _enemys) {
			e->Init();
		}
	}
	//終了処理
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
	//更新処理
	void UpdateEnemyManager() {
		for (auto e : _enemys) {
			if (!e->GetEnemyUse()) {
				continue;
			}
			e->Update();
		}
		EraseEnemy();
	}
	//描画処理
	void DrawEnemyManager() {
		for (auto e : _enemys) {
			if (!e->GetEnemyUse()) {
				continue;
			}
			e->Draw();
		}
	}

	//追加
	void Add(ENEMY* enemy) {
		_enemys.emplace_back(enemy);
	}

	//使ってないものをリストから消す
	void EraseEnemy() {
		_enemys.remove_if([](ENEMY* e) {
			if (!e->GetEnemyUse()) {
				//影を消して
				ReleaseShadow(e->GetEnemyShadow());
				//モデルを解放
				e->Uninit();
				//存在を消す
				delete e;
				return true;
			}
			return false;
			}
		);
	}

	//リスト内にある要素数を渡す
	size_t GetSize() {
		return _enemys.size();
	}
};