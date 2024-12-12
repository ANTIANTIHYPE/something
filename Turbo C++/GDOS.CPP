/**
 * geometry dash in MS-DOS !!!!!!! yay
 */



#include <GRAPHICS.H>
#include <CONIO.H>
#include <DOS.H>
#include <STDLIB.H>
#include <TIME.H>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define PLAYER_SIZE 30
#define GRAVITY 1
#define JUMP_STRENGTH 15

struct Obstacle
{
    int x, y, width, height;
    int isHazard; // 1 if it's a hazard, 0 if it's a block
};

Obstacle obstacles[10];
int obstacleCount = 0;
int playerY = SCREEN_HEIGHT - PLAYER_SIZE;
int playerVelocityY = 0;
int isJumping = 0;
int gameOver = 0;

void initObstacles()
{
    for (int i = 0; i < 10; i++)
    {
	obstacles[i].x = SCREEN_WIDTH + i * 200;
	obstacles[i].y = SCREEN_HEIGHT - 40;
	obstacles[i].width = 40;
	obstacles[i].height = 40;
	obstacles[i].isHazard = rand() % 2;
    }
    obstacleCount = 10;
}

void drawPlayer(int x, int y)
{
    setfillstyle(SOLID_FILL, YELLOW);
    bar(x, y, x + PLAYER_SIZE, y + PLAYER_SIZE);
}

void drawObstacles()
{
    for (int i = 0; i < obstacleCount; i++)
    {
	if (obstacles[i].isHazard)
	{
	    setfillstyle(SOLID_FILL, RED);
	}
	else
	{
	    setfillstyle(SOLID_FILL, WHITE);
	}
	bar(obstacles[i].x, obstacles[i].y, obstacles[i].x + obstacles[i].width, obstacles[i].y + obstacles[i].height); // Draw obstacle
    }
}

void updateObstacles()
{
    for (int i = 0; i < obstacleCount; i++)
    {
	obstacles[i].x -= 5;
	if (obstacles[i].x < -obstacles[i].width)
	{
	    obstacles[i].x = SCREEN_WIDTH + rand() % 200;
	    obstacles[i].isHazard = rand() % 2;
	}
    }
}

int checkCollision(int playerX, int playerY)
{
    for (int i = 0; i < obstacleCount; i++)
    {
	if (playerX + PLAYER_SIZE > obstacles[i].x && playerX < obstacles[i].x + obstacles[i].width &&
	    playerY + PLAYER_SIZE > obstacles[i].y && playerY < obstacles[i].y + obstacles[i].height)
	{
	    return i;
	}
    }
    return -1; // No collision
}

void handleCollision(int playerX, int &playerY)
{
    int collisionIndex = checkCollision(playerX, playerY);
    if (collisionIndex != -1)
    {
	if (obstacles[collisionIndex].isHazard)
	{
	    gameOver = 1;
	}
	else
	{
	    if (playerY + PLAYER_SIZE <= obstacles[collisionIndex].y + obstacles[collisionIndex].height &&
		playerY + PLAYER_SIZE + playerVelocityY >= obstacles[collisionIndex].y)
	    {
		playerY = obstacles[collisionIndex].y - PLAYER_SIZE - 2;
		playerVelocityY = 0;
		isJumping = 0;
	    }
	}
    }
}

void gameLoop()
{
    int playerX = 100;
    while (!gameOver)
    {
	cleardevice();

	if (isJumping)
	{
	    playerVelocityY -= GRAVITY;
	    playerY -= playerVelocityY;
	    if (playerY >= SCREEN_HEIGHT - PLAYER_SIZE)
	    {
		playerY = SCREEN_HEIGHT - PLAYER_SIZE;
		isJumping = 0;
		playerVelocityY = 0;
	    }
	}
	else
	{
	    if (playerY < SCREEN_HEIGHT - PLAYER_SIZE)
	    {
		playerY += GRAVITY;
	    }
	}

	updateObstacles();
	drawObstacles();

	drawPlayer(playerX, playerY);

	handleCollision(playerX, playerY);

	delay(25); // game speed
	if (kbhit())
	{
	    char ch = getch();
	    if (ch == ' ' && !isJumping)
	    {
		isJumping = 1;
		playerVelocityY = JUMP_STRENGTH;
	    }
	}
    }
}

void main()
{
    while ( 1 )
    {
	srand(time(NULL));
	int gd = DETECT, gm;
	initgraph(&gd, &gm, "C:\\Turboc3\\BGI");

	initObstacles();
	gameLoop();

	// game over screen
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
	outtextxy(SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, "YOU DIE LOL");
	getch();

    }

    // closegraph();
    // return 0;
}
