//trout.h	マス基底クラス
//20106_田中　蓮
//23_12_08
#pragma once

#include "main.h"
#include "sprite.h"

enum TYPE {
	TYPE_RED,	//赤、敵が出る
	TYPE_BLUE,	//青、コインが出る
	TYPE_YELLOW,//黄、近くの黄色にワープ
	TYPE_WHITE,	//白、何も起こらない
	TYPE_END,	//紫、終わる

	TYPE_NUM,
};


class TROUT {
protected:
	D3DXVECTOR3 _pos;					//ポジション
	D3DXVECTOR3 _rot;					//角度
	D3DXVECTOR3 _scl;					//スケール
	D3DXMATRIX  _mtxWorld;
	ID3D11Buffer* _VertexBuffer = NULL;	// 頂点バッファ
	ID3D11Buffer* _IndexBuffer = NULL;	// インデックスバッファ
	TYPE		_type;					//種類
	bool		_step = false;			//駒が到着したか
	bool		_event = false;			//イベントを発生させたか
	bool		_eventend = false;		//イベントが終わったか
	int			_tex_no = 0;			//テクスチャ番号
	int			_sound_no = 0;			//サウンド番号
	int			_vertexnum = 4;			//頂点数
	int			_indexnum = 4;			//インデックス数

public:
	TROUT() = default;
	TROUT(D3DXVECTOR3 pos, TYPE type) :
		_pos(pos), _type(type), _event(false), _eventend(false) {}
	virtual ~TROUT() {
		ReleaseVerBuf();
		ReleaseIndBuf();
	}

	virtual void Init() = 0 {}
	virtual void Uninit() = 0 {}
	virtual void Update() = 0 {}
	virtual void Draw() = 0 {}

	virtual void Event() = 0 {}


	D3DXVECTOR3 GetPos()const { return _pos; }
	D3DXVECTOR3 GetRot()const { return _rot; }
	D3DXVECTOR3 GetScl()const { return _scl; }
	TYPE GetType()const { return _type; }
	bool GetStep()const { return _step; }
	bool GetEvent()const { return _event; }
	bool GetEventEnd()const { return _eventend; }
	int GetTexNo()const { return _tex_no; }
	int GetSoundNo()const { return _sound_no; }
	int GetVertexNum()const { return _vertexnum; }
	int GetIndexNum()const { return _indexnum; }
	D3DXMATRIX* GetMtxWorld() { return &_mtxWorld; }
	ID3D11Buffer* GetVerBuf()const { return _VertexBuffer; }
	ID3D11Buffer** GetVerBufp() { return &_VertexBuffer; }
	ID3D11Buffer* GetIndBuf()const { return _IndexBuffer; }
	ID3D11Buffer** GetIndBufp() { return &_IndexBuffer; }

	void SetPos(const D3DXVECTOR3& pos) { _pos = pos; }
	void SetRot(const D3DXVECTOR3& rot) { _rot = rot; }
	void SetScl(const D3DXVECTOR3& scl) { _scl = scl; }
	void SetType(const TYPE& type) { _type = type; }
	void SetStep(const bool& step) { _step = step; }
	void SetEvent(const bool& event) { _event = event; }
	void SetEventEnd(const bool& eventend) { _eventend = eventend; }
	void SetTex(const int& texno) { _tex_no = texno; }
	void SetSound(const int& soundno) { _sound_no = soundno; }

	void ReleaseVerBuf() {
		//頂点バッファの解放
		if (_VertexBuffer != NULL) {
			_VertexBuffer->Release();
			_VertexBuffer = NULL;
		}
	}
	void ReleaseIndBuf() {
		//頂点バッファの解放
		if (_IndexBuffer != NULL) {
			_IndexBuffer->Release();
			_IndexBuffer = NULL;
		}
	}
};

