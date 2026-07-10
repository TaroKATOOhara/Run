#include "DxLib.h"
#include "Run.h"

const int WIDTH = 1200;
const int HEIGHT = 720;
const int FPS = 60;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("RealRun");	// ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32);// ウィンドウサイズ、カラー設定
	ChangeWindowMode(true);		// ウィンドウモード
	if (DxLib_Init() == -1) return -1;	// ライブラリ初期化、エラーが出たら終了
	SetBackgroundColor(255, 255, 255);	// 背景色の指定
	SetDrawScreen(DX_SCREEN_BACK);	// 裏画面に描画

	Game game;
	Init(game);

	while (1)
	{
		ClearDrawScreen();

		//// ゲームの処理
		GetJoypadDirectInputState(DX_INPUT_PAD1, &game.inputState);	// キー入力取得
		// DebugCamera(game);
		Move(game);
		Draw(game);


		ScreenFlip();	// 裏画面の内容を表画面に反映させる
		WaitTimer(1000 / FPS);	// 一定時間待つ
		if (ProcessMessage() == -1) break;	// Windowsから情報を受取エラーが起きたら終了
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) break;	// ESCキーが押された終了
	}


	DxLib_End();	// DXライブラリ試用の終了処理
	return 0;	// ソフトの終了
}

VEC2 NewVEC2(int x, int y)
{
	VEC2 ret;
	ret.x = x;
	ret.y = y;
	return ret;
}

// ゲームを初期状態にする
void Init(Game& game)
{
	// 初期パラメータ設定
	Init((game.camera), NewVEC2(0, 0), NewVEC2(WIDTH, HEIGHT), 0xFF0000);		// カメラに対して、色は無意味とする
	Init((game.body), NewVEC2(0, 0), NewVEC2(100, 75), 0xFF0000);
	Init((game.legL), NewVEC2(-75, 0), NewVEC2(100, 100), 0x00FF00);
	Init((game.legR), NewVEC2( 75, 0), NewVEC2(100, 100), 0x00FF00);
}

// 各Objectにパラメータ設定
void Init(Object& object, VEC2 pos, VEC2 size, int c)
{
	object.pos = pos;
	object.size = size;
	object.color = c;
}


void Move(Game& game)	// 1f分の挙動
{

}

// ゲーム中のオブジェクトを全描画
void Draw(Game& game)
{
	Draw(game.camera, game.legL);
	Draw(game.camera, game.legR);
	Draw(game.camera, game.body);
}

// オブジェクトを描画(楕円版)
void Draw(Object& obj)
{
	int x1 = obj.pos.x - obj.size.x / 2;
	int y1 = obj.pos.y - obj.size.y / 2;
	int x2 = obj.pos.x + obj.size.x / 2;
	int y2 = obj.pos.y + obj.size.y / 2;
	DrawOval_Rect(x1, y1, x2, y2, obj.color, true);
}

// オブジェクトをカメラで描画(楕円版)
void Draw(Object& cam, Object& obj)
{
	// キャラクター描画用のワールド座標を決める
	int x1 = obj.pos.x - obj.size.x / 2;
	int y1 = obj.pos.y - obj.size.y / 2;
	int x2 = obj.pos.x + obj.size.x / 2;
	int y2 = obj.pos.y + obj.size.y / 2;

	// カメラの撮影範囲の右上からの相対座標に変換
	x1 = x1 - cam.pos.x + cam.size.x / 2;
	y1 = y1 - cam.pos.y + cam.size.y / 2;
	x2 = x2 - cam.pos.x + cam.size.x / 2;
	y2 = y2 - cam.pos.y + cam.size.y / 2;

	// カメラの描画範囲を考慮したズーム
	// 縦横の拡大率
	float zoomX = static_cast <float>(WIDTH) / static_cast <float>(cam.size.x);
	float zoomY = static_cast <float>(HEIGHT) / static_cast <float>(cam.size.y);
	x1 = static_cast <int>(x1 * zoomX);
	y1 = static_cast <int>(y1 * zoomY);
	x2 = static_cast <int>(x2 * zoomX);
	y2 = static_cast <int>(y2 * zoomY);
	

	DrawOval_Rect(x1, y1, x2, y2, obj.color, true);
}

// カメラテスト用
void DebugCamera(Game& game)
{
	// カメラテスト
	if (game.inputState.X > 0) game.camera.pos.x+=5;
	if (game.inputState.X < 0) game.camera.pos.x-=5;
	if (game.inputState.Y > 0) game.camera.pos.y+=5;
	if (game.inputState.Y < 0) game.camera.pos.y-=5;
	if (game.inputState.Rx > 0) game.camera.size.x+=5;
	if (game.inputState.Rx < 0) game.camera.size.x-=5;
	if (game.inputState.Ry > 0) game.camera.size.y+=5;
	if (game.inputState.Ry < 0) game.camera.size.y-=5;

	// テスト用表示
	DrawFormatString(10, 10, 0x000000, "%5d", game.inputState.X);
	DrawFormatString(10, 25, 0x000000, "%5d", game.inputState.Y);
	DrawFormatString(10, 40, 0x000000, "%5d", game.inputState.Z);
	DrawFormatString(70, 10, 0x000000, "%5d", game.inputState.Rx);
	DrawFormatString(70, 25, 0x000000, "%5d", game.inputState.Ry);
	DrawFormatString(70, 40, 0x000000, "%5d", game.inputState.Rz);

	DrawFormatString(200, 10, 0x000000, "Camera", 0);
	DrawFormatString(200, 25, 0x000000, " posX: %5d", game.camera.pos.x);
	DrawFormatString(200, 40, 0x000000, " posY: %5d", game.camera.pos.y);
	DrawFormatString(200, 55, 0x000000, "sizeX: %5d", game.camera.size.x);
	DrawFormatString(200, 70, 0x000000, "sizeY: %5d", game.camera.size.y);
}