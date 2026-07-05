#include "DxLib.h"
#include "Run.h"

const int WIDTH = 1200;
const int HEIGHT = 720;
const int FPS = 60;

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	SetWindowText("title");	// ウィンドウのタイトル
	SetGraphMode(WIDTH, HEIGHT, 32);// ウィンドウサイズ、カラー設定
	ChangeWindowMode(true);		// ウィンドウモード
	if (DxLib_Init() == -1) return -1;	// ライブラリ初期化、エラーが出たら終了
	SetBackgroundColor(255, 255, 255);	// 背景色の指定
	SetDrawScreen(DX_SCREEN_BACK);	// 裏画面に描画

	DINPUT_JOYSTATE inputState;
	Game game;
	Init(game);

	while (1)
	{
		ClearDrawScreen();

		//// ゲームの処理
		GetJoypadDirectInputState(DX_INPUT_PAD1, &inputState);

		DrawFormatString(10, 10,	0x000000, "%5d", inputState.X);
		DrawFormatString(10, 25,	0x000000, "%5d", inputState.Y);
		DrawFormatString(10, 40,	0x000000, "%5d", inputState.Z);
		DrawFormatString(70, 10,	0x000000, "%5d", inputState.Rx);
		DrawFormatString(70, 25,	0x000000, "%5d", inputState.Ry);
		DrawFormatString(70, 40,	0x000000, "%5d", inputState.Rz);
		
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
	Init((game.body), NewVEC2(WIDTH/2, HEIGHT/2), NewVEC2(150, 75), 0xFF0000);
	Init((game.legL), NewVEC2(0, 0), NewVEC2(0, 0), 0);
	Init((game.legR), NewVEC2(0, 0), NewVEC2(0, 0), 0);
}

// 各Objectにパラメータ設定
void Init(Object& object, VEC2 pos, VEC2 size, int c)
{
	object.pos = pos;
	object.size = size;
	object.color = c;
}

// ゲーム中のオブジェクトを全描画
void Draw(Game& game)
{
	Draw(game.legL);
	Draw(game.legR);
	Draw(game.body);
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
	int x1 = obj.pos.x - obj.size.x / 2;
	int y1 = obj.pos.y - obj.size.y / 2;
	int x2 = obj.pos.x + obj.size.x / 2;
	int y2 = obj.pos.y + obj.size.y / 2;
	DrawOval_Rect(x1, y1, x2, y2, obj.color, true);
}