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
    setcaption("五子棋 Made By Frnks");
    setbkcolor(GREEN);
    startInterface();
    cleardevice();
    refreshBackground();
    player(1);
//    printf("最终分数：\n");
//    printf("黑子 %d : 白子 %d\n");
//    printf("按任意键结束\n");
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

    setfont(60, 0, "微软雅黑");
    xyprintf(50, 550, "游戏规则：");
    setfont(25, 0, "微软雅黑");
    xyprintf(50, 650, "按左右上下方向键移动棋子，按空格键确定落棋子，禁止在棋盘外按空格");
    xyprintf(50, 700, "你是否接受上述的游戏规则?（Y/N）");

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
    setfont(30,0,"微软雅黑");
    xyprintf(700, 100, "简易五子棋");
    xyprintf(700, 150, "上下左右键移动光标");
    xyprintf(700, 200, "空格键放置棋子");
    xyprintf(700, 300, "黄边棋子代表光标");
    xyprintf(700, 350, "红色棋子代表该位置已有棋子");
    xyprintf(700, 450, "黑子 : 白子");
    xyprintf(700, 500, "   %d  :  %d",blackScore,whiteScore);
    xyprintf(700, 600, "Enjoy~");
}


void player(int playerNum)
{
    bool isPressed;
    bool isEnd = false;
    // 初始化棋子位置
    int xCursor = 7;
    int yCursor = 7;
    color_t borderColor;
    color_t fillColor;

    for (; is_run(); delay_fps(60)) {
        bool keyDown = false;        //按键按下标志位
        int key;                    //记录按下的按键
        while (kbmsg()) {
            key_msg kMsg = getkey();        //读取按键消息
            //判断是否是按键按下
            if (kMsg.msg == key_msg_down) {
                //是则标记按键按下，并记录下按键
                keyDown = true;
                key = kMsg.key;
            }
        }
        int dx = 0, dy = 0;
        //按键按下时
        if (keyDown) {
            playerControl(key, xCursor, yCursor, dx, dy, playerNum, isEnd);
            //计算下一个位置在哪
            int xNext = xCursor + dx, yNext = yCursor + dy;
            //对下一个位置做边界检测，在边界内的才移动,
            if (0 <= xNext && xNext < COL
                && 0 <= yNext && yNext < ROW) {
                //如果里面有围墙，那么就需要再次判断要走到的位置是否是围墙
                xCursor += dx;
                yCursor += dy;
            }
        }

        cleardevice();
        refreshBackground();
        drawChessBoard();
        tips();
//        xyprintf(5, 5, "当前位置：(%d, %d)", xCursor, yCursor);
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
        case 'A': case key_left:	//左
            dx = -1; dy = 0;
            break;
        case 'W': case key_up:		//上
            dx = 0; dy = -1;
            break;
        case 'S': case key_down:	//下
            dx = 0; dy = 1;
            break;
        case 'D': case key_right:	//右
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
        default:					//其他键不移动
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
        setfont(60, 0, "微软雅黑");
        setcolor(BLACK);
        setbkcolor(WHITE);
        xyprintf(100,300,"黑子胜利！");
    }
    else
    {
        setfont(60, 0, "微软雅黑");
        setcolor(WHITE);
        setbkcolor(BLACK);
        xyprintf(100,300,"白子胜利！");
    }
    setfont(40, 0, "微软雅黑");
    xyprintf(100,400,"是否重新开始？(Y/N)");

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
        else if (confirm == 2) closegraph(); // 这里有点小问题
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
