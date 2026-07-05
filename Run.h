#pragma once

// 2次元ベクトル
struct VEC2 {
	int x;
	int y;
};

// 体のパーツに使う
struct Object {
	VEC2 pos;	// 座標
	VEC2 size;	// サイズ。カメラにおいては描画範囲
	// shape	// 表示図形の種類(未使用)
	// img		// 画像でやるなら
	int color;	//		色指定
	int alpha;	//		透明度に使うかも
};

// このゲームに必要になる変数
struct Game {
	DINPUT_JOYSTATE inputState;		// キー入力の保持
	Object camera;	// 世界を撮影するカメラ
	Object body;	// 胴体
	Object legL;	// 左足
	Object legR;	// 右足
};

// 関数
VEC2 NewVEC2(int x, int y);
void Init(Game &game);	// ゲームを初期状態にする
void Init(Object& object, VEC2 pos, VEC2 size, int c);	// 各Objectにパラメータ設定
void Draw(Game& game);	// ゲーム中のオブジェクトを全描画
void Draw(Object& obj);	// オブジェクトを描画(ワールド座標＝画面座標)
void Draw(Object& cam, Object& obj);	// オブジェクトを描画(カメラ概念利用)