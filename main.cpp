/*--+----1----+----2----+----3----+----4----+----5-----+----6----+----7----+----8----+----9----+---*/
//初期：基本のプログラムのサンプル

//エラー抑制
#define _CRT_SECURE_NO_WARNINGS

//########## ヘッダーファイル読み込み ##########
#include "DxLib.h"
#include "math.h"

//########## マクロ定義 ##########
#define GAME_WIDTH			800	//画面の横の大きさ
#define GAME_HEIGHT			600	//画面の縦の大きさ
#define GAME_COLOR			32	//画面のカラービット

#define GAME_WINDOW_BAR		0				//タイトルバーはデフォルトにする
#define GAME_WINDOW_NAME	"GAME TITLE"	//ウィンドウのタイトル
#define GAME_FPS			60				//FPSの数値	

#define GAME_ICON_ID		333				//ゲームのアイコンID

//パスの長さ
#define PATH_MAX			255	//255文字まで
#define NAME_MAX			255	//255文字まで

//キーボードの種類
#define KEY_MAX				256

//エラーメッセージ
#define IMAGE_LOAD_ERR_TITLE	TEXT("画像読み込みエラー")

//終了ダイアログ用
#define MOUSE_R_CLICK_TITLE		TEXT("ゲーム中断")
#define MOUSE_R_CLICK_CAPTION	TEXT("ゲームを中断し、タイトル画面に戻りますか？")

//フォント
#define FONT_TANU_PATH			TEXT(".\\TanukiMagic.ttf")			//フォントの場所
#define FONT_TANU_NAME			TEXT("たぬき油性マジック")			//フォントの名前

//エラーメッセージ
#define FONT_INSTALL_ERR_TITLE	TEXT("フォントインストールエラー")
#define FONT_CREATE_ERR_TITLE	TEXT("フォント作成エラー")

//文字描画用
#define STR_DRAW_SIZE	24		//フォントのサイズ
#define STR_DRAW_ROW_MAX	4	//文字列の最大行
#define STR_ROW_MAX		30		//文字行の最大値
#define STR_COL_MAX		255		//文字列の最大値
#define STR_DRAW_SPEED	1		//文字列の描画速度(何フレームごとに、１文字描画するか？)

//########## 列挙型 ##########
enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};	//ゲームのシーン

//########## 構造体の宣言 ##########
typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];		//パス
	int handle;					//ハンドル
	int x;						//X位置
	int y;						//Y位置
	int width;					//幅
	int height;					//高さ
}IMAGE;	//画像構造体

//########## グローバル変数 ##########
//FPS関連
int StartTimeFps;				//測定開始時刻
int CountFps;					//カウンタ
float CalcFps;					//計算結果

//キーボードの入力を取得
char AllKeyState[KEY_MAX];		//すべてのキーの状態が入る
char OldAllKeyState[KEY_MAX];	//すべてのキーの状態(直前)が入る

//ゲーム関連
GAME_SCENE GameScene;			//ゲームのシーンを管理

//文字描画関連（空白は全角の空白で入力すること！）
//選択肢は１～９まで
//１質問１文章
char Bunsho1[STR_ROW_MAX][STR_COL_MAX] =
{
	"　ゲームプログラムとは、いやプログラムとは" ,
	"ある事柄を実現するプログラムの方法を説明されても理解できないことがある。B" ,
	"@　なぜならそのプログラム技法も何かの基本的な技法の組み合わせで出来ているからだ。B",
	"@　これはその他の学問も基本がわからないと応用が利かないということと同じ現象で、",
	"別に特に珍しいことでもない。B" ,
	"C　しかしゲームプログラムとなると覚えなくてはならない基礎が沢山あり、" ,
	"さらにある程度クオリティの高いソフトを作ろうとすると色々なプログラム技法を",
	"習得しなくてはならない。B" ,
	"C　しかもある程度レベルが高くなると自分で技法を編み出すか、技術レベルの高い",
	"プログラマーに聞くなどするしか方法がなく大変厄介である。B"
	"というかそのせいでゲームプログラムの敷居は高くなっているといえる。B",
	"E"
};

char Bunsho2[STR_ROW_MAX][STR_COL_MAX] =
{
	"あなたはどちらがお好きかしら？",
	"@　１．イヌ派だワン！",
	"@　２．ネコ派だニャン！Q",
	"1Cワンちゃんはかわいいですわ！",
	"@私は家で飼っているマロンちゃんのお散歩が大好きですの。X",
	"2Cネコちゃんはカワイイですわ！",
	"@家で買っているクロは、スマートでカッコよくもありますの。X",
	"X@そうですわ、今、お時間ありますかしら？",
	"@よろしかったら、お家でお茶でもいかがかしら？",
	"@私のペットをご覧なっていただきたいの！B",
	"E"
};

int DrawPointX, DrawPointY;	// 文字列描画の位置
int RowPos;					// 文字列の行数
int ColPos;					// 文字列の列数
BOOL EndFlag = FALSE;		// 文字列終了フラグ
char OneMojiBuf[3];			// １文字分一時記憶配列(１文字分＋\0)
char DrawStringBuf[STR_DRAW_ROW_MAX][STR_COL_MAX];		// 描画する文字列を入れる
int DefaultFontSize;								// デフォルトのフォントサイズを取得
RECT rectString = { 0,GAME_HEIGHT - STR_DRAW_SIZE * STR_DRAW_ROW_MAX,GAME_WIDTH,GAME_HEIGHT };	// 文字列描画用エリア
int DrawSpeedCnt = 0;					//文字の描画速度カウンタ
int DrawSpeedCntMax = STR_DRAW_SPEED;	//文字の描画速度カウンタMAX

//########## 外部のプロトタイプ宣言 ##########
VOID MY_FPS_UPDATE(VOID);	//FPS値を計測、更新する
VOID MY_FPS_DRAW(VOID);		//FPS値を描画する
VOID MY_FPS_WAIT(VOID);		//FPS値を計測し、待つ

VOID MY_START(VOID);		//スタート画面
VOID MY_START_PROC(VOID);	//スタート画面
VOID MY_START_DRAW(VOID);	//スタート画面
VOID MY_PLAY_INIT(VOID);	//プレイ画面(初期化)
VOID MY_PLAY(VOID);			//プレイ画面
VOID MY_PLAY_PROC(VOID);	//プレイ画面
VOID MY_PLAY_DRAW(VOID);	//プレイ画面
VOID MY_END(VOID);			//エンド画面
VOID MY_END_PROC(VOID);		//エンド画面
VOID MY_END_DRAW(VOID);		//エンド画面

VOID MY_ALL_KEYDOWN_UPDATE(VOID);				//キーの入力状態を更新する
BOOL MY_KEY_DOWN(int KEY_INPUT_);				//キーを押しているか、キーコードで判断する
BOOL MY_KEY_UP(int KEY_INPUT_);					//キーを押し上げたか、キーコードで判断する
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int msec);	//キーを押し続けているか、キーコードで判断する
BOOL MY_KEY_PUSH(int KEY_INPUT_);				//キーをプッシュしたか、キーコードで判断する

BOOL MY_FONT_INSTALL_ONCE(VOID);	//フォントをこのソフト用に、一時的にインストール
VOID MY_FONT_UNINSTALL_ONCE(VOID);	//フォントをこのソフト用に、一時的にアンインストール

int Kaigyou(VOID);							//改行関数
VOID NovelProc(char str[][STR_COL_MAX]);	//小説文字列処理関数（引数で描画する文字列をもらう）
VOID NovelDraw(VOID);						//小説文字列描画関数

//########## プログラムで最初に実行される関数 ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txtを出力しない
	ChangeWindowMode(TRUE);								//ウィンドウモードに設定
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//指定の数値でウィンドウを表示する
	SetWindowStyleMode(GAME_WINDOW_BAR);				//タイトルバーはデフォルトにする
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));			//ウィンドウのタイトルの文字
	SetAlwaysRunFlag(TRUE);								//非アクティブでも実行する
	SetWindowIconID(GAME_ICON_ID);						//アイコンファイルを読込

	if (DxLib_Init() == -1) { return -1; }	//ＤＸライブラリ初期化処理

	//Draw系関数は裏画面に描画
	SetDrawScreen(DX_SCREEN_BACK);

	//フォントを一時的にインストール
	if (MY_FONT_INSTALL_ONCE() == FALSE) { return -1; }

	//ゲームシーンはスタート画面から
	GameScene = GAME_SCENE_START;

	//文字にアンチエイリアスをかける
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_8X8);

	//無限ループ
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//メッセージ処理の結果がエラーのとき、強制終了
		if (ClearDrawScreen() != 0) { break; }	//画面を消去できなかったとき、強制終了

		MY_ALL_KEYDOWN_UPDATE();				//押しているキー状態を取得

		MY_FPS_UPDATE();	//FPSの処理[更新]

		//シーンごとに処理を行う
		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//スタート画面
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	//プレイ画面
			break;
		case GAME_SCENE_END:
			MY_END();	//エンド画面
			break;
		}

		MY_FPS_DRAW();		//FPSの処理[描画]

		ScreenFlip();		//モニタのリフレッシュレートの速さで裏画面を再描画

		MY_FPS_WAIT();		//FPSの処理[待つ]
	}

	//一時的にインストールしたフォントをアンインストール
	MY_FONT_UNINSTALL_ONCE();

	DxLib_End();	//ＤＸライブラリ使用の終了処理

	return 0;
}

//改行関数
int Kaigyou(VOID)
{
	// 描画列を最初に戻す
	DrawPointX = 0;

	DrawPointY++;	// 描画行位置を一つ下げる

	// もし画面からはみ出るなら文字列をスクロールさせる
	if (rectString.top + DrawPointY * STR_DRAW_SIZE + STR_DRAW_SIZE > rectString.bottom)
	{
		//一番上の行を空にする
		for (int j = 0; j < STR_COL_MAX; j++) { DrawStringBuf[0][j] = '\0'; }

		//上の行に順番に移動させる
		for (int i = 1; i < STR_DRAW_ROW_MAX; i++)
		{
			strcpy(DrawStringBuf[i - 1], DrawStringBuf[i]);
		}

		//最後の行を空にする
		for (int j = 0; j < STR_COL_MAX; j++) { DrawStringBuf[STR_DRAW_ROW_MAX - 1][j] = '\0'; }

		// 最終行から描画を開始する
		DrawPointY = STR_DRAW_ROW_MAX - 1;
	}

	return 0;
}

//小説文字列処理関数
//引数で、描画させたい文字列をもらってくる
VOID NovelProc(char str[][STR_COL_MAX])
{
	switch (str[RowPos][ColPos])	//文字を１つ取得
	{
	case '@':	// 改行文字

		// 改行処理および参照文字位置を一つ進める
		Kaigyou();
		ColPos++;	//次の文字を読み取る

		break;

	case 'B':	// ボタン押し待ち文字

		//ボタンを押されたら、次の文章へ
		if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE) { ColPos++; }

		break;

	case 'Q':	// 質問文字

		//１ボタンを押されたとき
		if (MY_KEY_DOWN(KEY_INPUT_1) == TRUE)
		{
			//最初が1から始まる文章を探す
			for (int i = 0; i < STR_ROW_MAX; i++)
			{
				if (str[i][0] == '1')
				{
					RowPos = i;
					ColPos = 1;
					break;
				}
			}
		}

		//２ボタンを押されたとき
		if (MY_KEY_DOWN(KEY_INPUT_2) == TRUE)
		{
			//最初が2から始まる文章を探す
			for (int i = 0; i < STR_ROW_MAX; i++)
			{
				if (str[i][0] == '2')
				{
					RowPos = i;
					ColPos = 1;
					break;
				}
			}
		}

		break;

	case 'X':	// 質問から戻ってくる文字

		//ボタンを押されたら、次の文章へ
		if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
		{
			//最初がXから始まる文章を探す
			for (int i = 0; i < STR_ROW_MAX; i++)
			{
				if (str[i][0] == 'X')
				{
					RowPos = i;
					ColPos = 1;
					break;
				}
			}
		}

		break;

	case 'E':	// 終了文字

		// 終了フラグを立てるおよび参照文字位置を一つ進める
		EndFlag = TRUE;
		ColPos++;	//次の文字を読み取る

		break;

	case 'C':	// クリア文字

		//描画している文字列を空にする
		for (int i = 0; i < STR_DRAW_ROW_MAX; i++)
		{
			for (int j = 0; j < STR_COL_MAX; j++)
			{
				DrawStringBuf[i][j] = '\0';
			}
		}

		//文字の位置を初期化
		DrawPointY = 0;
		DrawPointX = 0;
		ColPos++;

		break;

	default:	// その他の文字

		//描画タイミングを計算
		if (DrawSpeedCnt < DrawSpeedCntMax)
		{
			DrawSpeedCnt++;
		}
		else
		{
			// １文字分抜き出す
			OneMojiBuf[0] = str[RowPos][ColPos];
			OneMojiBuf[1] = str[RowPos][ColPos + 1];
			OneMojiBuf[2] = '\0';

			// 文字列を結合する
			strcat(DrawStringBuf[DrawPointY], OneMojiBuf);

			// 参照文字位置を２バイト進める
			ColPos += 2;

			// カーソルを一文字分進める
			DrawPointX++;

			// 画面からはみ出たら改行する
			if (DrawPointX * STR_DRAW_SIZE + STR_DRAW_SIZE > rectString.right)
			{
				Kaigyou();
			}

			DrawSpeedCnt = 0;
		}

		break;
	}

	// 参照文字列の終端まで行っていたら参照文字列を進める
	if (str[RowPos][ColPos] == '\0')
	{
		RowPos++;
		ColPos = 0;
	}

	return;
}

//小説文字列描画関数
VOID NovelDraw(VOID)
{
	//文字を描画するときは
	if (EndFlag == FALSE)
	{
		//文字を描画する領域を描画(半透明の四角を描画)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);	//半透明にして
		DrawBox(
			rectString.left,
			rectString.top,
			rectString.right,
			rectString.bottom,
			GetColor(255, 255, 255),
			TRUE
		);	//四角を描いて
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//元に戻す

		SetFontSize(STR_DRAW_SIZE);	//ノベルのフォントのサイズセット
		//文字列を描画
		for (int cnt = 0; cnt <= DrawPointY; cnt++)
		{
			DrawString(rectString.left, rectString.top + cnt * STR_DRAW_SIZE, DrawStringBuf[cnt], GetColor(0, 0, 0));
		}
		SetFontSize(DefaultFontSize);	//ノベルのフォントをもとに戻す
	}
}

//スタート画面
VOID MY_START(VOID)
{
	MY_START_PROC();	//スタート画面の処理
	MY_START_DRAW();	//スタート画面の描画

	return;
}

//スタート画面の処理
VOID MY_START_PROC(VOID)
{
	//スペースキーを押したら、プレイシーンへ移動する
	if (MY_KEY_PUSH(KEY_INPUT_SPACE) == TRUE)
	{
		//プレイ画面を初期化
		MY_PLAY_INIT();

		//プレイ画面へ
		GameScene = GAME_SCENE_PLAY;
	}

	return;
}

//スタート画面の描画
VOID MY_START_DRAW(VOID)
{
	DrawString(0, 0, "スタート画面(スペースキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//プレイ画面(初期化)
VOID MY_PLAY_INIT(VOID)
{
	// 描画位置の初期位置セット
	DrawPointX = 0;
	DrawPointY = 0;

	// 参照文字位置をセット
	RowPos = 0;	// １行目の
	ColPos = 0;	// ０文字

	// 終了フラグを倒す
	EndFlag = FALSE;

	//デフォルトのフォントサイズを取得
	DefaultFontSize = GetFontSize();

	//文字列を空にする
	OneMojiBuf[0] = '\0';
	OneMojiBuf[1] = '\0';
	OneMojiBuf[2] = '\0';

	for (int i = 0; i < STR_DRAW_ROW_MAX; i++)
	{
		for (int j = 0; j < STR_COL_MAX; j++)
		{
			DrawStringBuf[i][j] = '\0';
		}
	}

	return;
}

//プレイ画面
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//プレイ画面の処理
	MY_PLAY_DRAW();	//プレイ画面の描画

	return;
}

//プレイ画面の処理
VOID MY_PLAY_PROC(VOID)
{
	//スペースキーを押したら、スタートシーンへ移動する
	if (MY_KEY_PUSH(KEY_INPUT_SPACE) == TRUE)
	{
		GameScene = GAME_SCENE_END;

		return;
	}

	if (EndFlag == FALSE)
	{
		//小説文字列描画
		NovelProc(Bunsho2);
	}

	return;
}

//プレイ画面の描画
VOID MY_PLAY_DRAW(VOID)
{
	//文字列を描画
	NovelDraw();

	DrawString(0, 0, "プレイ画面(スペースキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//エンド画面
VOID MY_END(VOID)
{
	MY_END_PROC();	//エンド画面の処理
	MY_END_DRAW();	//エンド画面の描画
	return;
}

//エンド画面の処理
VOID MY_END_PROC(VOID)
{
	//スペースキーを押したら、スタートシーンへ移動する
	if (MY_KEY_PUSH(KEY_INPUT_SPACE) == TRUE)
	{
		GameScene = GAME_SCENE_START;
		return;
	}

	return;
}

//エンド画面の描画
VOID MY_END_DRAW(VOID)
{

	DrawString(0, 0, "エンド画面(スペースキーを押して下さい)", GetColor(255, 255, 255));
	return;
}

//########## FPS値を計測、更新する関数 ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1フレーム目なら時刻を記憶
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == GAME_FPS) //60フレーム目なら平均を計算
	{
		int now = GetNowCount();
		//現在の時間から、0フレーム目の時間を引き、FPSの数値で割る＝1FPS辺りの平均時間が計算される
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)GAME_FPS);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS値を描画する関数 ##########
VOID MY_FPS_DRAW(VOID)
{
	//文字列を描画
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS値を計測し、待つ関数 ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//かかった時間
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//待つべき時間

	if (waitTime > 0)		//指定のFPS値よりも早い場合
	{
		WaitTimer(waitTime);	//待つ
	}
	return;
}

//########## キーの入力状態を更新する関数 ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	//参考：https://dxlib.xsrv.jp/function/dxfunc_input.html

	char TempKey[KEY_MAX];	//一時的に、現在のキーの入力状態を格納する

	//直前のキー入力をとっておく
	for (int i = 0; i < KEY_MAX; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey); // 全てのキーの入力状態を得る

	for (int i = 0; i < KEY_MAX; i++)
	{
		if (TempKey[i] != 0)	//押されているキーのキーコードを押しているとき
		{
			AllKeyState[i]++;	//押されている
		}
		else
		{
			AllKeyState[i] = 0;	//押されていない
		}
	}
	return;
}

//キーを押しているか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//キーコードのキーを押している時
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//キーを押している
	}
	else
	{
		return FALSE;	//キーを押していない
	}
}

//キーを押し上げたか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//直前は押していて
		&& AllKeyState[KEY_INPUT_] == 0)	//今は押していないとき
	{
		return TRUE;	//キーを押し上げている
	}
	else
	{
		return FALSE;	//キーを押し上げていない
	}
}

//キーをプッシュしたか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
BOOL MY_KEY_PUSH(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] == 0		//直前は押していなくて
		&& AllKeyState[KEY_INPUT_] >= 1)	//今は押しているとき
	{
		return TRUE;	//キーをプッシュした（押し続けても、１回のみ発生）
	}
	else
	{
		return FALSE;	//キーをプッシュしていないか、押し続けている
	}
}

//キーを押し続けているか、キーコードで判断する
//引　数：int：キーコード：KEY_INPUT_???
//引　数：int：キーを押し続ける時間(ミリ秒)
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int milliTime)
{
	int MilliSec = 1000;	//１秒は1000ミリ秒

	//押し続ける時間=秒数×FPS値
	//例）60FPSのゲームで、1秒間押し続けるなら、1秒×60FPS
	int UpdateTime = (milliTime / MilliSec) * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//押し続けている
	}
	else
	{
		return FALSE;	//押し続けていない
	}
}

//領域の当たり判定をする関数
BOOL MY_CHECK_RECT_COLL(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;	//当たっている
	}

	return FALSE;		//当たっていない
}

//フォントをこのソフト用に、一時的にインストール
BOOL MY_FONT_INSTALL_ONCE(VOID)
{
	//フォントを一時的に読み込み(WinAPI)
	if (AddFontResourceEx(FONT_TANU_PATH, FR_PRIVATE, NULL) == 0)
	{
		//エラーメッセージ表示
		MessageBox(GetMainWindowHandle(), FONT_TANU_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

//フォントをこのソフト用に、一時的にアンインストール
VOID MY_FONT_UNINSTALL_ONCE(VOID)
{
	//一時的に読み込んだフォントを削除(WinAPI)
	RemoveFontResourceEx(FONT_TANU_PATH, FR_PRIVATE, NULL);

	return;
}