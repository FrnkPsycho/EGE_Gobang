#include <graphics.h>
#include <cstdio>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800

const int COL=15 , ROW=15;
int ChessBoard[COL*2+1][ROW*2+1] = {0};
int radius = 10;
int blackScore=0;
int whiteScore=0;

void startInterface();
void tips();
void player(int playerNum);
int judgeWinner(int playerNum);
void getImage(PIMAGE pimg, const char *fileName, int width, int height);
void playerControl(int key, int x, int y, int& dx, int& dy, int& playerNum, bool& isEnd);
void refreshBackground();
void putChess(int playerNum, int x, int y);
void drawChessBoard();
void clearChessBoard();
bool checkIfPut(int x,int y);
//int dfsJudge(int chess);
int Judge(int playerNum);
void endRound(int playerNum);
bool endGame = false;

int main() {
    initgraph(SCREEN_WIDTH,SCREEN_HEIGHT,0);
    ege_enable_aa(true);
    setcaption("������ Made By Frnks");
    setbkcolor(GREEN);
    startInterface();
    cleardevice();
    refreshBackground();
    player(1);
//    printf("���շ�����\n");
//    printf("���� %d : ���� %d\n");
//    printf("�����������\n");
//    getchar();
    closegraph();
    return 0;
}

void refreshBackground()
{
    PIMAGE pimg = newimage();
    getImage(pimg,"/bg.jpg",SCREEN_WIDTH,SCREEN_HEIGHT);
    putimage(0, 0, pimg);
    getImage(pimg,"/qipan.jpg",610,610); //TODO Add image
    putimage(75,75,pimg);
    delimage(pimg);
}

void startInterface() {
    PIMAGE pimg = newimage();
    getImage(pimg, "/bg.jpg", SCREEN_WIDTH, SCREEN_HEIGHT);
    putimage(0, 0, pimg);
    delimage(pimg);

    setcolor(BLACK);
    setbkmode(TRANSPARENT);

    setfont(60, 0, "΢���ź�");
    xyprintf(50, 550, "��Ϸ����");
    setfont(25, 0, "΢���ź�");
    xyprintf(50, 650, "���������·�����ƶ����ӣ����ո��ȷ�������ӣ���ֹ�������ⰴ�ո�");
    xyprintf(50, 700, "���Ƿ������������Ϸ����?��Y/N��");

    int confirm = 0; // 1 yes 2 no
    for ( ; is_run() ; delay_fps(60))
    {
        bool keyDown = false;
        int key;
        while ( kbmsg() )
        {
            key_msg kMsg = getkey();
            if ( kMsg.msg == key_msg_down)
            {
                keyDown = true;
                key = kMsg.key;
            }
        }
        if ( keyDown )
        {
            switch (key) {
                case 'Y':
                    confirm = 1;
                    break;
                case 'N':
                    confirm = 2;
                    break;
                default:
                    confirm = 0;
                    break;
            }
        }
        if ( confirm == 0 ) continue;
        else if ( confirm == 2 ) break; //TODO quit
        else if ( confirm == 1 ) break;
    }
    if ( confirm == 2 ) closegraph();
}

void getImage(PIMAGE pimg, const char *fileName, int width, int height)
{
    PIMAGE temp = newimage();
    getimage(temp,fileName);
    if (getwidth(pimg) != width || getheight(pimg) != height)
    {
        resize(pimg,width,height);
    }
    putimage(pimg,0,0,width,height,temp,0,0, getwidth(temp), getheight(temp));
    delimage(temp);
}

void tips()
{
    setcolor(BLACK);
    setfont(30,0,"΢���ź�");
    xyprintf(700, 100, "����������");
    xyprintf(700, 150, "�������Ҽ��ƶ����");
    xyprintf(700, 200, "�ո����������");
    xyprintf(700, 300, "�Ʊ����Ӵ�����");
    xyprintf(700, 350, "��ɫ���Ӵ����λ����������");
    xyprintf(700, 450, "���� : ����");
    xyprintf(700, 500, "   %d  :  %d",blackScore,whiteScore);
    xyprintf(700, 600, "Enjoy~");
}


void player(int playerNum)
{
    bool isPressed;
    bool isEnd = false;
    // ��ʼ������λ��
    int xCursor = 7;
    int yCursor = 7;
    color_t borderColor;
    color_t fillColor;

    for (; is_run(); delay_fps(60)) {
        bool keyDown = false;        //�������±�־λ
        int key;                    //��¼���µİ���
        while (kbmsg()) {
            key_msg kMsg = getkey();        //��ȡ������Ϣ
            //�ж��Ƿ��ǰ�������
            if (kMsg.msg == key_msg_down) {
                //�����ǰ������£�����¼�°���
                keyDown = true;
                key = kMsg.key;
            }
        }
        int dx = 0, dy = 0;
        //��������ʱ
        if (keyDown) {
            playerControl(key, xCursor, yCursor, dx, dy, playerNum, isEnd);
            //������һ��λ������
            int xNext = xCursor + dx, yNext = yCursor + dy;
            //����һ��λ�����߽��⣬�ڱ߽��ڵĲ��ƶ�,
            if (0 <= xNext && xNext < COL
                && 0 <= yNext && yNext < ROW) {
                //���������Χǽ����ô����Ҫ�ٴ��ж�Ҫ�ߵ���λ���Ƿ���Χǽ
                xCursor += dx;
                yCursor += dy;
            }
        }

        cleardevice();
        refreshBackground();
        drawChessBoard();
        tips();
//        xyprintf(5, 5, "��ǰλ�ã�(%d, %d)", xCursor, yCursor);
        if ( playerNum == 1 )
        {
            borderColor = YELLOW;
            fillColor = BLACK;
        }
        else if ( playerNum == -1 )
        {
            borderColor = YELLOW;
            fillColor = WHITE;
        }
        if (!checkIfPut(xCursor,yCursor))
        {
            fillColor = RED;
        }
        setcolor(borderColor);
        setfillcolor(fillColor);
        fillellipse(xCursor * 4 * radius + 100 , yCursor * 4 * radius + 100, 2 * radius, 2 * radius);
        if ( isEnd ) break;
    }
    endRound(playerNum);
}

void playerControl(int key, int x, int y, int& dx, int& dy, int& playerNum, bool& isEnd)
{
    switch (key) {
        case 'A': case key_left:	//��
            dx = -1; dy = 0;
            break;
        case 'W': case key_up:		//��
            dx = 0; dy = -1;
            break;
        case 'S': case key_down:	//��
            dx = 0; dy = 1;
            break;
        case 'D': case key_right:	//��
            dx = 1; dy = 0;
            break;
        case key_space:
            if (checkIfPut(x,y))
            {
                putChess(playerNum, x, y);
                if (judgeWinner(playerNum) == playerNum )
                {
                    isEnd = true;
                    if ( playerNum == 1 ) blackScore++;
                    else if ( playerNum == -1 ) whiteScore++;
                }
                else
                {
                    if (playerNum == 1) playerNum = -1;
                    else playerNum = 1;
                }
            }
            break;
        default:					//���������ƶ�
            dx = dy = 0;
            break;
    }
}

void putChess(int playerNum, int x, int y)
{
    int chess=0;
    if ( playerNum == 1 ) chess = 1;
    else if ( playerNum == -1 ) chess = -1;
    ChessBoard[x][y] = chess;
}

bool checkIfPut(int x,int y)
{
    if ( ChessBoard[x][y] != 0 )
    {
        return false;
    }
    else return true;
}

void drawChessBoard()
{
    for ( int i=0 ; i<16 ; i++ )
    {
        for ( int j=0 ; j<16 ; j++ )
        {
            if ( ChessBoard[i][j] == 1 )
            {
                setcolor(BLACK);
                setfillcolor(BLACK);
                fillellipse(i * 4 * radius + 100 , j * 4 * radius + 100, 2 * radius, 2 * radius);
            }
            else if ( ChessBoard[i][j] == -1 )
            {
                setcolor(WHITE);
                setfillcolor(WHITE);
                fillellipse(i * 4 * radius + 100 , j * 4 * radius + 100, 2 * radius, 2 * radius);
            }
        }
    }
}

int judgeWinner(int playerNum)
{
    int res;
    if ( playerNum == 1 ) res = Judge(1);
    else res = Judge(-1);
    return res;
}

void endRound(int playerNum)
{
    if ( playerNum == 1 )
    {
        setfont(60, 0, "΢���ź�");
        setcolor(BLACK);
        setbkcolor(WHITE);
        xyprintf(100,300,"����ʤ����");
    }
    else
    {
        setfont(60, 0, "΢���ź�");
        setcolor(WHITE);
        setbkcolor(BLACK);
        xyprintf(100,300,"����ʤ����");
    }
    setfont(40, 0, "΢���ź�");
    xyprintf(100,400,"�Ƿ����¿�ʼ��(Y/N)");

    int confirm=0;
    for ( ; is_run() ; delay_fps(60)) {
        bool keyDown = false;
        int key;
        while (kbmsg()) {
            key_msg kMsg = getkey();
            if (kMsg.msg == key_msg_down) {
                keyDown = true;
                key = kMsg.key;
            }
        }
        if (keyDown) {
            switch (key) {
                case 'Y':
                    confirm = 1;
                    break;
                case 'N':
                    confirm = 2;
                    break;
                default:
                    confirm = 0;
                    break;
            }
        }
        if (confirm == 0) continue;
        else if (confirm == 2) closegraph(); // �����е�С����
        else if (confirm == 1)
        {
            clearChessBoard();
            player(1);
        }
    }
}

void clearChessBoard()
{
    for ( int i=0 ; i<COL ; i++ )
    {
        for ( int j=0 ; j<ROW ; j++ )
        {
            ChessBoard[i][j] = 0;
        }
    }
}

int Judge(int playerNum)
{
    int i,j;
    if (playerNum == 1) {

        for (i = 0; i < COL; i++) {

            for (j = 0; j < ROW; j++) {

                if (ChessBoard[i][j] == 1 && ChessBoard[i + 1][j] == 1 && ChessBoard[i + 2][j] == 1 &&
                    ChessBoard[i + 3][j] == 1 && ChessBoard[i + 4][j] == 1)
                    return 1;
                else if (ChessBoard[i][j] == 1 && ChessBoard[i][j + 1] == 1 && ChessBoard[i][j + 2] == 1 &&
                         ChessBoard[i][j + 3] == 1 && ChessBoard[i][j + 4] == 1)
                    return 1;
                else if (ChessBoard[i][j] == 1 && ChessBoard[i + 1][j + 1] == 1 && ChessBoard[i + 2][j + 2] == 1 &&
                         ChessBoard[i + 3][j + 3] == 1 && ChessBoard[i + 4][j + 4] == 1)
                    return 1;
                else if (j >= 4 && ChessBoard[i][j] == 1 && ChessBoard[i + 1][j - 1] == 1 &&
                         ChessBoard[i + 2][j - 2] == 1 && ChessBoard[i + 3][j - 3] == 1 &&
                         ChessBoard[i + 4][j - 4] == 1)
                    return 1;
            }
        }
    } else if (playerNum == -1) {
        for (i = 0; i < COL; i++) {
            for (j = 0; j < ROW; j++) {
                if (ChessBoard[i][j] == -1 && ChessBoard[i + 1][j] == -1 && ChessBoard[i + 2][j] == -1 &&
                    ChessBoard[i + 3][j] == -1 && ChessBoard[i + 4][j] == -1)
                    return -1;
                else if (ChessBoard[i][j] == -1 && ChessBoard[i][j + 1] == -1 && ChessBoard[i][j + 2] == -1 &&
                         ChessBoard[i][j + 3] == -1 && ChessBoard[i][j + 4] == -1)
                    return -1;
                else if (ChessBoard[i][j] == -1 && ChessBoard[i + 1][j + 1] == -1 && ChessBoard[i + 2][j + 2] == -1 &&
                         ChessBoard[i + 3][j + 3] == -1 && ChessBoard[i + 4][j + 4] == -1)
                    return -1;
                else if (j >= 4 && ChessBoard[i][j] == -1 && ChessBoard[i + 1][j - 1] == -1 &&
                         ChessBoard[i + 2][j - 2] == -1 && ChessBoard[i + 3][j - 3] == -1 &&
                         ChessBoard[i + 4][j - 4] == -1)
                    return -1;
            }
        }
    }
    return 0;
}


//int v[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
//
//int dfsJudge(int chess)
//{
//    int cnt=0;
//    for ( int i=0 ; i<COL ; i++ )
//    {
//        for ( int j=0 ; j<ROW ; j++ )
//        {
//            if ( ChessBoard[i][j] == chess )
//            {
//                for ( int k=0 ; k<8 ; j++ )
//                {
//                    int ni = i;
//                    int nj = j;
//                    cnt = 1;
//                    for ( int t=0 ; t<ROW ; t++ )
//                    {
//                        ni+=v[k][0];
//                        nj+=v[k][1];
//                        if (ni<0 || nj<0 || ni>=COL || nj>=ROW || ChessBoard[ni][nj] != chess ) break;
//                        cnt++;
//                    }
//                    if ( cnt >= 5 )
//                    {
//                        return chess;
//                    }
//                }
//            }
//        }
//    }
//    return 0;
//}
