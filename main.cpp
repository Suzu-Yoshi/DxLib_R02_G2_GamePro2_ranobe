/*--+----1----+----2----+----3----+----4----+----5-----+----6----+----7----+----8----+----9----+---*/
//�����F��{�̃v���O�����̃T���v��

//�G���[�}��
#define _CRT_SECURE_NO_WARNINGS

//########## �w�b�_�[�t�@�C���ǂݍ��� ##########
#include "DxLib.h"
#include "math.h"

//########## �}�N����` ##########
#define GAME_WIDTH			800	//��ʂ̉��̑傫��
#define GAME_HEIGHT			600	//��ʂ̏c�̑傫��
#define GAME_COLOR			32	//��ʂ̃J���[�r�b�g

#define GAME_WINDOW_BAR		0				//�^�C�g���o�[�̓f�t�H���g�ɂ���
#define GAME_WINDOW_NAME	"GAME TITLE"	//�E�B���h�E�̃^�C�g��
#define GAME_FPS			60				//FPS�̐��l	

#define GAME_ICON_ID		333				//�Q�[���̃A�C�R��ID

//�p�X�̒���
#define PATH_MAX			255	//255�����܂�
#define NAME_MAX			255	//255�����܂�

//�L�[�{�[�h�̎��
#define KEY_MAX				256

//�G���[���b�Z�[�W
#define IMAGE_LOAD_ERR_TITLE	TEXT("�摜�ǂݍ��݃G���[")

//�I���_�C�A���O�p
#define MOUSE_R_CLICK_TITLE		TEXT("�Q�[�����f")
#define MOUSE_R_CLICK_CAPTION	TEXT("�Q�[���𒆒f���A�^�C�g����ʂɖ߂�܂����H")

//�t�H���g
#define FONT_TANU_PATH			TEXT(".\\TanukiMagic.ttf")			//�t�H���g�̏ꏊ
#define FONT_TANU_NAME			TEXT("���ʂ������}�W�b�N")			//�t�H���g�̖��O

//�G���[���b�Z�[�W
#define FONT_INSTALL_ERR_TITLE	TEXT("�t�H���g�C���X�g�[���G���[")
#define FONT_CREATE_ERR_TITLE	TEXT("�t�H���g�쐬�G���[")

//�����`��p
#define STR_DRAW_SIZE	24		//�t�H���g�̃T�C�Y
#define STR_DRAW_ROW_MAX	4	//������̍ő�s
#define STR_ROW_MAX		30		//�����s�̍ő�l
#define STR_COL_MAX		255		//������̍ő�l
#define STR_DRAW_SPEED	1		//������̕`�摬�x(���t���[�����ƂɁA�P�����`�悷�邩�H)

//########## �񋓌^ ##########
enum GAME_SCENE {
	GAME_SCENE_START,
	GAME_SCENE_PLAY,
	GAME_SCENE_END,
};	//�Q�[���̃V�[��

//########## �\���̂̐錾 ##########
typedef struct STRUCT_IMAGE
{
	char path[PATH_MAX];		//�p�X
	int handle;					//�n���h��
	int x;						//X�ʒu
	int y;						//Y�ʒu
	int width;					//��
	int height;					//����
}IMAGE;	//�摜�\����

//########## �O���[�o���ϐ� ##########
//FPS�֘A
int StartTimeFps;				//����J�n����
int CountFps;					//�J�E���^
float CalcFps;					//�v�Z����

//�L�[�{�[�h�̓��͂��擾
char AllKeyState[KEY_MAX];		//���ׂẴL�[�̏�Ԃ�����
char OldAllKeyState[KEY_MAX];	//���ׂẴL�[�̏��(���O)������

//�Q�[���֘A
GAME_SCENE GameScene;			//�Q�[���̃V�[�����Ǘ�

//�����`��֘A�i�󔒂͑S�p�̋󔒂œ��͂��邱�ƁI�j
//�I�����͂P�`�X�܂�
//�P����P����
char Bunsho1[STR_ROW_MAX][STR_COL_MAX] =
{
	"�@�Q�[���v���O�����Ƃ́A����v���O�����Ƃ�" ,
	"���鎖������������v���O�����̕��@���������Ă������ł��Ȃ����Ƃ�����BB" ,
	"@�@�Ȃ��Ȃ炻�̃v���O�����Z�@�������̊�{�I�ȋZ�@�̑g�ݍ��킹�ŏo���Ă��邩�炾�BB",
	"@�@����͂��̑��̊w�����{���킩��Ȃ��Ɖ��p�������Ȃ��Ƃ������ƂƓ������ۂŁA",
	"�ʂɓ��ɒ��������Ƃł��Ȃ��BB" ,
	"C�@�������Q�[���v���O�����ƂȂ�Ɗo���Ȃ��Ă͂Ȃ�Ȃ���b����R����A" ,
	"����ɂ�����x�N�I���e�B�̍����\�t�g����낤�Ƃ���ƐF�X�ȃv���O�����Z�@��",
	"�K�����Ȃ��Ă͂Ȃ�Ȃ��BB" ,
	"C�@������������x���x���������Ȃ�Ǝ����ŋZ�@��҂ݏo�����A�Z�p���x���̍���",
	"�v���O���}�[�ɕ����Ȃǂ��邵�����@���Ȃ���ϖ��ł���BB"
	"�Ƃ��������̂����ŃQ�[���v���O�����̕~���͍����Ȃ��Ă���Ƃ�����BB",
	"E"
};

char Bunsho2[STR_ROW_MAX][STR_COL_MAX] =
{
	"���Ȃ��͂ǂ��炪���D��������H",
	"@�@�P�D�C�k�h�������I",
	"@�@�Q�D�l�R�h���j�����IQ",
	"1C���������͂��킢���ł���I",
	"@���͉ƂŎ����Ă���}���������̂��U������D���ł��́BX",
	"2C�l�R�����̓J���C�C�ł���I",
	"@�ƂŔ����Ă���N���́A�X�}�[�g�ŃJ�b�R�悭������܂��́BX",
	"X@�����ł���A���A�����Ԃ���܂�������H",
	"@��낵��������A���Ƃł����ł�������������H",
	"@���̃y�b�g�������Ȃ��Ă������������́IB",
	"E"
};

int DrawPointX, DrawPointY;	// ������`��̈ʒu
int RowPos;					// ������̍s��
int ColPos;					// ������̗�
BOOL EndFlag = FALSE;		// ������I���t���O
char OneMojiBuf[3];			// �P�������ꎞ�L���z��(�P�������{\0)
char DrawStringBuf[STR_DRAW_ROW_MAX][STR_COL_MAX];		// �`�悷�镶���������
int DefaultFontSize;								// �f�t�H���g�̃t�H���g�T�C�Y���擾
RECT rectString = { 0,GAME_HEIGHT - STR_DRAW_SIZE * STR_DRAW_ROW_MAX,GAME_WIDTH,GAME_HEIGHT };	// ������`��p�G���A
int DrawSpeedCnt = 0;					//�����̕`�摬�x�J�E���^
int DrawSpeedCntMax = STR_DRAW_SPEED;	//�����̕`�摬�x�J�E���^MAX

//########## �O���̃v���g�^�C�v�錾 ##########
VOID MY_FPS_UPDATE(VOID);	//FPS�l���v���A�X�V����
VOID MY_FPS_DRAW(VOID);		//FPS�l��`�悷��
VOID MY_FPS_WAIT(VOID);		//FPS�l���v�����A�҂�

VOID MY_START(VOID);		//�X�^�[�g���
VOID MY_START_PROC(VOID);	//�X�^�[�g���
VOID MY_START_DRAW(VOID);	//�X�^�[�g���
VOID MY_PLAY_INIT(VOID);	//�v���C���(������)
VOID MY_PLAY(VOID);			//�v���C���
VOID MY_PLAY_PROC(VOID);	//�v���C���
VOID MY_PLAY_DRAW(VOID);	//�v���C���
VOID MY_END(VOID);			//�G���h���
VOID MY_END_PROC(VOID);		//�G���h���
VOID MY_END_DRAW(VOID);		//�G���h���

VOID MY_ALL_KEYDOWN_UPDATE(VOID);				//�L�[�̓��͏�Ԃ��X�V����
BOOL MY_KEY_DOWN(int KEY_INPUT_);				//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_UP(int KEY_INPUT_);					//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int msec);	//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����
BOOL MY_KEY_PUSH(int KEY_INPUT_);				//�L�[���v�b�V���������A�L�[�R�[�h�Ŕ��f����

BOOL MY_FONT_INSTALL_ONCE(VOID);	//�t�H���g�����̃\�t�g�p�ɁA�ꎞ�I�ɃC���X�g�[��
VOID MY_FONT_UNINSTALL_ONCE(VOID);	//�t�H���g�����̃\�t�g�p�ɁA�ꎞ�I�ɃA���C���X�g�[��

int Kaigyou(VOID);							//���s�֐�
VOID NovelProc(char str[][STR_COL_MAX]);	//���������񏈗��֐��i�����ŕ`�悷�镶��������炤�j
VOID NovelDraw(VOID);						//����������`��֐�

//########## �v���O�����ōŏ��Ɏ��s�����֐� ##########
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);				//Log.txt���o�͂��Ȃ�
	ChangeWindowMode(TRUE);								//�E�B���h�E���[�h�ɐݒ�
	SetGraphMode(GAME_WIDTH, GAME_HEIGHT, GAME_COLOR);	//�w��̐��l�ŃE�B���h�E��\������
	SetWindowStyleMode(GAME_WINDOW_BAR);				//�^�C�g���o�[�̓f�t�H���g�ɂ���
	SetMainWindowText(TEXT(GAME_WINDOW_NAME));			//�E�B���h�E�̃^�C�g���̕���
	SetAlwaysRunFlag(TRUE);								//��A�N�e�B�u�ł����s����
	SetWindowIconID(GAME_ICON_ID);						//�A�C�R���t�@�C����Ǎ�

	if (DxLib_Init() == -1) { return -1; }	//�c�w���C�u��������������

	//Draw�n�֐��͗���ʂɕ`��
	SetDrawScreen(DX_SCREEN_BACK);

	//�t�H���g���ꎞ�I�ɃC���X�g�[��
	if (MY_FONT_INSTALL_ONCE() == FALSE) { return -1; }

	//�Q�[���V�[���̓X�^�[�g��ʂ���
	GameScene = GAME_SCENE_START;

	//�����ɃA���`�G�C���A�X��������
	ChangeFontType(DX_FONTTYPE_ANTIALIASING_8X8);

	//�������[�v
	while (TRUE)
	{
		if (ProcessMessage() != 0) { break; }	//���b�Z�[�W�����̌��ʂ��G���[�̂Ƃ��A�����I��
		if (ClearDrawScreen() != 0) { break; }	//��ʂ������ł��Ȃ������Ƃ��A�����I��

		MY_ALL_KEYDOWN_UPDATE();				//�����Ă���L�[��Ԃ��擾

		MY_FPS_UPDATE();	//FPS�̏���[�X�V]

		//�V�[�����Ƃɏ������s��
		switch (GameScene)
		{
		case GAME_SCENE_START:
			MY_START();	//�X�^�[�g���
			break;
		case GAME_SCENE_PLAY:
			MY_PLAY();	//�v���C���
			break;
		case GAME_SCENE_END:
			MY_END();	//�G���h���
			break;
		}

		MY_FPS_DRAW();		//FPS�̏���[�`��]

		ScreenFlip();		//���j�^�̃��t���b�V�����[�g�̑����ŗ���ʂ��ĕ`��

		MY_FPS_WAIT();		//FPS�̏���[�҂�]
	}

	//�ꎞ�I�ɃC���X�g�[�������t�H���g���A���C���X�g�[��
	MY_FONT_UNINSTALL_ONCE();

	DxLib_End();	//�c�w���C�u�����g�p�̏I������

	return 0;
}

//���s�֐�
int Kaigyou(VOID)
{
	// �`�����ŏ��ɖ߂�
	DrawPointX = 0;

	DrawPointY++;	// �`��s�ʒu���������

	// ������ʂ���͂ݏo��Ȃ當������X�N���[��������
	if (rectString.top + DrawPointY * STR_DRAW_SIZE + STR_DRAW_SIZE > rectString.bottom)
	{
		//��ԏ�̍s����ɂ���
		for (int j = 0; j < STR_COL_MAX; j++) { DrawStringBuf[0][j] = '\0'; }

		//��̍s�ɏ��ԂɈړ�������
		for (int i = 1; i < STR_DRAW_ROW_MAX; i++)
		{
			strcpy(DrawStringBuf[i - 1], DrawStringBuf[i]);
		}

		//�Ō�̍s����ɂ���
		for (int j = 0; j < STR_COL_MAX; j++) { DrawStringBuf[STR_DRAW_ROW_MAX - 1][j] = '\0'; }

		// �ŏI�s����`����J�n����
		DrawPointY = STR_DRAW_ROW_MAX - 1;
	}

	return 0;
}

//���������񏈗��֐�
//�����ŁA�`�悳�������������������Ă���
VOID NovelProc(char str[][STR_COL_MAX])
{
	switch (str[RowPos][ColPos])	//�������P�擾
	{
	case '@':	// ���s����

		// ���s��������юQ�ƕ����ʒu����i�߂�
		Kaigyou();
		ColPos++;	//���̕�����ǂݎ��

		break;

	case 'B':	// �{�^�������҂�����

		//�{�^���������ꂽ��A���̕��͂�
		if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE) { ColPos++; }

		break;

	case 'Q':	// ���╶��

		//�P�{�^���������ꂽ�Ƃ�
		if (MY_KEY_DOWN(KEY_INPUT_1) == TRUE)
		{
			//�ŏ���1����n�܂镶�͂�T��
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

		//�Q�{�^���������ꂽ�Ƃ�
		if (MY_KEY_DOWN(KEY_INPUT_2) == TRUE)
		{
			//�ŏ���2����n�܂镶�͂�T��
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

	case 'X':	// ���₩��߂��Ă��镶��

		//�{�^���������ꂽ��A���̕��͂�
		if (MY_KEY_DOWN(KEY_INPUT_RETURN) == TRUE)
		{
			//�ŏ���X����n�܂镶�͂�T��
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

	case 'E':	// �I������

		// �I���t���O�𗧂Ă邨��юQ�ƕ����ʒu����i�߂�
		EndFlag = TRUE;
		ColPos++;	//���̕�����ǂݎ��

		break;

	case 'C':	// �N���A����

		//�`�悵�Ă��镶�������ɂ���
		for (int i = 0; i < STR_DRAW_ROW_MAX; i++)
		{
			for (int j = 0; j < STR_COL_MAX; j++)
			{
				DrawStringBuf[i][j] = '\0';
			}
		}

		//�����̈ʒu��������
		DrawPointY = 0;
		DrawPointX = 0;
		ColPos++;

		break;

	default:	// ���̑��̕���

		//�`��^�C�~���O���v�Z
		if (DrawSpeedCnt < DrawSpeedCntMax)
		{
			DrawSpeedCnt++;
		}
		else
		{
			// �P�����������o��
			OneMojiBuf[0] = str[RowPos][ColPos];
			OneMojiBuf[1] = str[RowPos][ColPos + 1];
			OneMojiBuf[2] = '\0';

			// ���������������
			strcat(DrawStringBuf[DrawPointY], OneMojiBuf);

			// �Q�ƕ����ʒu���Q�o�C�g�i�߂�
			ColPos += 2;

			// �J�[�\�����ꕶ�����i�߂�
			DrawPointX++;

			// ��ʂ���͂ݏo������s����
			if (DrawPointX * STR_DRAW_SIZE + STR_DRAW_SIZE > rectString.right)
			{
				Kaigyou();
			}

			DrawSpeedCnt = 0;
		}

		break;
	}

	// �Q�ƕ�����̏I�[�܂ōs���Ă�����Q�ƕ������i�߂�
	if (str[RowPos][ColPos] == '\0')
	{
		RowPos++;
		ColPos = 0;
	}

	return;
}

//����������`��֐�
VOID NovelDraw(VOID)
{
	//������`�悷��Ƃ���
	if (EndFlag == FALSE)
	{
		//������`�悷��̈��`��(�������̎l�p��`��)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);	//�������ɂ���
		DrawBox(
			rectString.left,
			rectString.top,
			rectString.right,
			rectString.bottom,
			GetColor(255, 255, 255),
			TRUE
		);	//�l�p��`����
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	//���ɖ߂�

		SetFontSize(STR_DRAW_SIZE);	//�m�x���̃t�H���g�̃T�C�Y�Z�b�g
		//�������`��
		for (int cnt = 0; cnt <= DrawPointY; cnt++)
		{
			DrawString(rectString.left, rectString.top + cnt * STR_DRAW_SIZE, DrawStringBuf[cnt], GetColor(0, 0, 0));
		}
		SetFontSize(DefaultFontSize);	//�m�x���̃t�H���g�����Ƃɖ߂�
	}
}

//�X�^�[�g���
VOID MY_START(VOID)
{
	MY_START_PROC();	//�X�^�[�g��ʂ̏���
	MY_START_DRAW();	//�X�^�[�g��ʂ̕`��

	return;
}

//�X�^�[�g��ʂ̏���
VOID MY_START_PROC(VOID)
{
	//�X�y�[�X�L�[����������A�v���C�V�[���ֈړ�����
	if (MY_KEY_PUSH(KEY_INPUT_SPACE) == TRUE)
	{
		//�v���C��ʂ�������
		MY_PLAY_INIT();

		//�v���C��ʂ�
		GameScene = GAME_SCENE_PLAY;
	}

	return;
}

//�X�^�[�g��ʂ̕`��
VOID MY_START_DRAW(VOID)
{
	DrawString(0, 0, "�X�^�[�g���(�X�y�[�X�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

//�v���C���(������)
VOID MY_PLAY_INIT(VOID)
{
	// �`��ʒu�̏����ʒu�Z�b�g
	DrawPointX = 0;
	DrawPointY = 0;

	// �Q�ƕ����ʒu���Z�b�g
	RowPos = 0;	// �P�s�ڂ�
	ColPos = 0;	// �O����

	// �I���t���O��|��
	EndFlag = FALSE;

	//�f�t�H���g�̃t�H���g�T�C�Y���擾
	DefaultFontSize = GetFontSize();

	//���������ɂ���
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

//�v���C���
VOID MY_PLAY(VOID)
{
	MY_PLAY_PROC();	//�v���C��ʂ̏���
	MY_PLAY_DRAW();	//�v���C��ʂ̕`��

	return;
}

//�v���C��ʂ̏���
VOID MY_PLAY_PROC(VOID)
{
	//�X�y�[�X�L�[����������A�X�^�[�g�V�[���ֈړ�����
	if (MY_KEY_PUSH(KEY_INPUT_SPACE) == TRUE)
	{
		GameScene = GAME_SCENE_END;

		return;
	}

	if (EndFlag == FALSE)
	{
		//����������`��
		NovelProc(Bunsho2);
	}

	return;
}

//�v���C��ʂ̕`��
VOID MY_PLAY_DRAW(VOID)
{
	//�������`��
	NovelDraw();

	DrawString(0, 0, "�v���C���(�X�y�[�X�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

//�G���h���
VOID MY_END(VOID)
{
	MY_END_PROC();	//�G���h��ʂ̏���
	MY_END_DRAW();	//�G���h��ʂ̕`��
	return;
}

//�G���h��ʂ̏���
VOID MY_END_PROC(VOID)
{
	//�X�y�[�X�L�[����������A�X�^�[�g�V�[���ֈړ�����
	if (MY_KEY_PUSH(KEY_INPUT_SPACE) == TRUE)
	{
		GameScene = GAME_SCENE_START;
		return;
	}

	return;
}

//�G���h��ʂ̕`��
VOID MY_END_DRAW(VOID)
{

	DrawString(0, 0, "�G���h���(�X�y�[�X�L�[�������ĉ�����)", GetColor(255, 255, 255));
	return;
}

//########## FPS�l���v���A�X�V����֐� ##########
VOID MY_FPS_UPDATE(VOID)
{
	if (CountFps == 0) //1�t���[���ڂȂ玞�����L��
	{
		StartTimeFps = GetNowCount();
	}

	if (CountFps == GAME_FPS) //60�t���[���ڂȂ畽�ς��v�Z
	{
		int now = GetNowCount();
		//���݂̎��Ԃ���A0�t���[���ڂ̎��Ԃ������AFPS�̐��l�Ŋ��遁1FPS�ӂ�̕��ώ��Ԃ��v�Z�����
		CalcFps = 1000.f / ((now - StartTimeFps) / (float)GAME_FPS);
		CountFps = 0;
		StartTimeFps = now;
	}
	CountFps++;
	return;
}

//########## FPS�l��`�悷��֐� ##########
VOID MY_FPS_DRAW(VOID)
{
	//�������`��
	DrawFormatString(0, GAME_HEIGHT - 20, GetColor(255, 255, 255), "FPS:%.1f", CalcFps);
	return;
}

//########## FPS�l���v�����A�҂֐� ##########
VOID MY_FPS_WAIT(VOID)
{
	int resultTime = GetNowCount() - StartTimeFps;					//������������
	int waitTime = CountFps * 1000 / GAME_FPS - resultTime;	//�҂ׂ�����

	if (waitTime > 0)		//�w���FPS�l���������ꍇ
	{
		WaitTimer(waitTime);	//�҂�
	}
	return;
}

//########## �L�[�̓��͏�Ԃ��X�V����֐� ##########
VOID MY_ALL_KEYDOWN_UPDATE(VOID)
{
	//�Q�l�Fhttps://dxlib.xsrv.jp/function/dxfunc_input.html

	char TempKey[KEY_MAX];	//�ꎞ�I�ɁA���݂̃L�[�̓��͏�Ԃ��i�[����

	//���O�̃L�[���͂��Ƃ��Ă���
	for (int i = 0; i < KEY_MAX; i++)
	{
		OldAllKeyState[i] = AllKeyState[i];
	}

	GetHitKeyStateAll(TempKey); // �S�ẴL�[�̓��͏�Ԃ𓾂�

	for (int i = 0; i < KEY_MAX; i++)
	{
		if (TempKey[i] != 0)	//������Ă���L�[�̃L�[�R�[�h�������Ă���Ƃ�
		{
			AllKeyState[i]++;	//������Ă���
		}
		else
		{
			AllKeyState[i] = 0;	//������Ă��Ȃ�
		}
	}
	return;
}

//�L�[�������Ă��邩�A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
BOOL MY_KEY_DOWN(int KEY_INPUT_)
{
	//�L�[�R�[�h�̃L�[�������Ă��鎞
	if (AllKeyState[KEY_INPUT_] != 0)
	{
		return TRUE;	//�L�[�������Ă���
	}
	else
	{
		return FALSE;	//�L�[�������Ă��Ȃ�
	}
}

//�L�[�������グ�����A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
BOOL MY_KEY_UP(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] >= 1	//���O�͉����Ă���
		&& AllKeyState[KEY_INPUT_] == 0)	//���͉����Ă��Ȃ��Ƃ�
	{
		return TRUE;	//�L�[�������グ�Ă���
	}
	else
	{
		return FALSE;	//�L�[�������グ�Ă��Ȃ�
	}
}

//�L�[���v�b�V���������A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
BOOL MY_KEY_PUSH(int KEY_INPUT_)
{
	if (OldAllKeyState[KEY_INPUT_] == 0		//���O�͉����Ă��Ȃ���
		&& AllKeyState[KEY_INPUT_] >= 1)	//���͉����Ă���Ƃ�
	{
		return TRUE;	//�L�[���v�b�V�������i���������Ă��A�P��̂ݔ����j
	}
	else
	{
		return FALSE;	//�L�[���v�b�V�����Ă��Ȃ����A���������Ă���
	}
}

//�L�[�����������Ă��邩�A�L�[�R�[�h�Ŕ��f����
//���@���Fint�F�L�[�R�[�h�FKEY_INPUT_???
//���@���Fint�F�L�[�����������鎞��(�~���b)
BOOL MY_KEYDOWN_KEEP(int KEY_INPUT_, int milliTime)
{
	int MilliSec = 1000;	//�P�b��1000�~���b

	//���������鎞��=�b���~FPS�l
	//��j60FPS�̃Q�[���ŁA1�b�ԉ���������Ȃ�A1�b�~60FPS
	int UpdateTime = (milliTime / MilliSec) * GAME_FPS;

	if (AllKeyState[KEY_INPUT_] > UpdateTime)
	{
		return TRUE;	//���������Ă���
	}
	else
	{
		return FALSE;	//���������Ă��Ȃ�
	}
}

//�̈�̓����蔻�������֐�
BOOL MY_CHECK_RECT_COLL(RECT a, RECT b)
{
	if (a.left < b.right &&
		a.top < b.bottom &&
		a.right > b.left &&
		a.bottom > b.top
		)
	{
		return TRUE;	//�������Ă���
	}

	return FALSE;		//�������Ă��Ȃ�
}

//�t�H���g�����̃\�t�g�p�ɁA�ꎞ�I�ɃC���X�g�[��
BOOL MY_FONT_INSTALL_ONCE(VOID)
{
	//�t�H���g���ꎞ�I�ɓǂݍ���(WinAPI)
	if (AddFontResourceEx(FONT_TANU_PATH, FR_PRIVATE, NULL) == 0)
	{
		//�G���[���b�Z�[�W�\��
		MessageBox(GetMainWindowHandle(), FONT_TANU_NAME, FONT_INSTALL_ERR_TITLE, MB_OK);
		return FALSE;
	}

	return TRUE;
}

//�t�H���g�����̃\�t�g�p�ɁA�ꎞ�I�ɃA���C���X�g�[��
VOID MY_FONT_UNINSTALL_ONCE(VOID)
{
	//�ꎞ�I�ɓǂݍ��񂾃t�H���g���폜(WinAPI)
	RemoveFontResourceEx(FONT_TANU_PATH, FR_PRIVATE, NULL);

	return;
}