
/* ABDUL REHMAN
	23i-2082
	AI_B
	Project
*/	



#include <GL/glut.h>
#include <iostream>
// #include "util.h"
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <string>
#include<fstream>

using namespace std;

const int BRICK_WIDTH = 80;
const int BRICK_HEIGHT = 20;
const int PADDLE_WIDTH = 140;
const int PADDLE_HEIGHT = 20;
const int BALL_RADIUS = 6;

const int speed = 2;
const int MAX_BALLS = 50;

struct GameState
{
	// colors for game
	const float WHITE[3] = {1.0, 1.0, 1.0};
	const float BLACK[3] = {0.0, 0.0, 0.0};
	const float RED[3] = {1.0, 0.0, 0.0};
	const float GREEN[3] = {0.0, 1.0, 0.0};
	const float BLUE[3] = {0.0, 0.0, 1.0};
	const float YELLOW[3] = {1.0, 1.0, 0.0};
	const float PINK[3] = {1.0, 0.0, 1.0};

	int SCREEN_WIDTH = 1240;
	int SCREEN_HEIGHT = 1100;

	class GameObject // game object class
	{
	protected:
		float x, y;
		int width, height;
		const float *color;

	public:
		GameObject(float x, float y, int width, int height, const float *color) : x(x), y(y), width(width), height(height), color(color) {}

		float getX() const { return x; }
		float getY() const { return y; }
		int getWidth() const { return width; }
		int getHeight() const { return height; }

		virtual void draw() = 0;
		virtual ~GameObject() {}
	};

	class Ball : public GameObject // Ball class
	{
	private:
		int velocityX, velocityY;

	public:
		Ball(float x, float y, int radius, const float *color, int velocityX, int velocityY) : GameObject(x, y, radius * 2, radius * 2, color), velocityX(velocityX), velocityY(velocityY) {}

		void move()
		{
			x += velocityX;
			y += velocityY;
		}

		int getVelocityX() const { return velocityX; }
		int getVelocityY() const { return velocityY; }

		void setVelocityX(int vx) { velocityX = vx; }
		void setVelocityY(int vy) { velocityY = vy; }

		// Add setX and setY methods
		void setX(float newX) { x = newX; }
		void setY(float newY) { y = newY; }
		float GetX() { return x; }
		float GetY() { return y; }

		void draw() override
		{
			glColor3fv(color);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++)
			{
				float theta = i * 3.14159 / 180;
				glVertex2f(x + BALL_RADIUS * cos(theta), y + BALL_RADIUS * sin(theta));
			}
			glEnd();
			cout << velocityX << " " << velocityY << endl;
		}
	};

	class Paddle : public GameObject // Paddle class
	{
	public:
		Paddle(float x, float y, int width, int height, const float *color) : GameObject(x, y, width, height, color) {}

		void move(int dx)
		{
			x += dx;
		}
		void setWidth(int newWidth)
		{
			width = newWidth;
		}

		int getWidth() const
		{
			return width;
		}

		void draw() override
		{
			glColor3fv(color);
			glBegin(GL_POLYGON);
			glVertex2f(x, y);
			glVertex2f(x + width, y);
			glVertex2f(x + width, y + height);
			glVertex2f(x, y + height);
			glEnd();
		}
		void draw1(float xPos, float yPos, float color[])
		{
			glColor3fv(color);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++)
			{
				float theta = i * 3.14159 / 180;
				glVertex2f(xPos + BALL_RADIUS * cos(theta), yPos + BALL_RADIUS * sin(theta));
			}
			glEnd();
		}
	};

	class Brick : public GameObject // Brick class
	{
	private:
		int collisions;
		int maxCollisions;
		const float *color;
		const float *color1;

	public:
		Brick(float x, float y, int width, int height, const float *color, const float *color1, int maxCollisions)
			: GameObject(x, y, width, height, color), collisions(0), maxCollisions(maxCollisions), color(color), color1(color1) {}
		void incrementCollisions()
		{
			if (collisions < maxCollisions)
			{
				collisions++;
			}
		}

		int getCollisions() const { return collisions; }
		int getMaxCollisions() const { return maxCollisions; }
		const float *getColor() const { return color; } // Added getColor() function
		const float *getColor1() const { return color1; }
		void setColor(const float *newColor) { color = newColor; }

		void draw() override
		{
			// Draw black border
			glColor3f(0.0, 0.0, 0.0);
			glLineWidth(2.0);
			glBegin(GL_LINE_LOOP);
			glVertex2f(x, y);
			glVertex2f(x + width, y);
			glVertex2f(x + width, y + height);
			glVertex2f(x, y + height);
			glEnd();

			// Fill with brick color
			glColor3fv(color);
			glBegin(GL_QUADS);
			glVertex2f(x + 2, y + 2);
			glVertex2f(x + width - 2, y + 2);
			glVertex2f(x + width - 2, y + height - 2);
			glVertex2f(x + 2, y + height - 2);
			glEnd();
		}
	};

	class PowerUp : public GameObject // Powers class
	{
	private:
		int speed;
		const float *color2;

	public:
		PowerUp(float x, float y, int width, int height, const float *color, const float *color2, int speed)
			: GameObject(x, y, width, height, color), speed(speed), color2(color2) {}

		void move()
		{
			y += speed;
		}
		const float *getColor2() const { return color2; }

		void draw() override
		{
			glColor3fv(color);
			glBegin(GL_QUADS);
			glVertex2f(x, y);
			glVertex2f(x + width, y);
			glVertex2f(x + width, y + height);
			glVertex2f(x, y + height);
			glEnd();
		}
		void draw1()
		{

			glBegin(GL_QUADS);
			glColor3f(1.0, 1.0, 0.0);
			glVertex2f(x, y);
			glVertex2f(x + width, y);
			glVertex2f(x + width, y + height);
			glVertex2f(x, y + height);
			glEnd();
		}
		void drawTriangle()
		{
			glBegin(GL_TRIANGLES);			// Begin drawing triangles
			glColor3f(0.0, 1.0, 0.0);		// Set color to blue
			glVertex2f(x, y);				// Vertex 1 (bottom-left)
			glVertex2f(x - 10.0, y + 20.0); // Vertex 2 (top-left)
			glVertex2f(x + 10.0, y + 20.0); // Vertex 3 (top-right)
			glEnd();						// End drawing triangles
		}
		void drawCircle()
		{
			glColor3fv(color);
			glBegin(GL_POLYGON);
			for (int i = 0; i < 360; i++)
			{
				float theta = i * 3.14159 / 180;
				glVertex2f(x + BALL_RADIUS * cos(theta), y + BALL_RADIUS * sin(theta));
			}
			glEnd();
		}

		void drawSquare()
		{
			glBegin(GL_QUADS);
			glColor3f(1.0, 0.0, 0.0);  // Begin drawing quads
			glVertex2f(x, y);		   // Bottom-left vertex
			glVertex2f(x + 40, y);	   // Bottom-right vertex
			glVertex2f(x + 40, y + 8); // Top-right vertex
			glVertex2f(x, y + 8);	   // Top-left vertex
			glEnd();				   // End drawing quads
		}
	};
	class SomeVar // class for some imporant variables
	{
		int BRICK_ROWS;
		int BRICK_COLS;
		int level;
		int lives;
		int score;

	public:
		SomeVar(int rows, int cols, int lvl, int live, int sco) : BRICK_ROWS(rows), BRICK_COLS(cols), level(lvl), lives(live), score(sco) {}
		void setBRICK_ROWS(int r)
		{
			BRICK_ROWS = r;
		}
		void setBRICK_COLS(int b)
		{
			BRICK_COLS = b;
		}
		void setLevel(int l)
		{
			level = l;
		}
		int getBRICK_ROWS()
		{
			return BRICK_ROWS;
		}
		int getBRICK_COLS()
		{
			return BRICK_COLS;
		}
		int getlevel()
		{
			return level;
		}
		int getlives()
		{
			return lives;
		}
		void setlive(int livr)
		{
			lives = livr;
		}
		int getscore()
		{
			return score;
		}
		void setscore(int sc)
		{
			score = sc;
		}
	};
	class Timevar // class for timer
	{
		int var1;
		int var2;
		int var3;
		int var4;
		int var5;
		int flag1;
		int flag2;
		int flag3;
		int flag4;
		int flag5;

	public:
		Timevar();
		Timevar(int num1, int num2, int num3, int num4, int num5, int num6, int num7, int num8, int num9, int num10) : var1(num1), var2(num2), var3(num3), var4(num4), var5(num5), flag1(num6), flag2(num7), flag3(num8), flag4(num9), flag5(num10) {}
		void setvar1(int num)
		{
			var1 = num;
		}
		void setvar2(int num)
		{
			var2 = num;
		}
		void setvar3(int num)
		{
			var3 = num;
		}
		void setvar4(int num)
		{
			var4 = num;
		}
		void setvar5(int num)
		{
			var5 = num;
		}
		int getvar1()
		{
			return var1;
		}
		int getvar2()
		{
			return var2;
		}
		int getvar3()
		{
			return var3;
		}
		int getvar4()
		{
			return var4;
		}
		int getvar5()
		{
			return var5;
		}
		void setflag1(int num)
		{
			flag1 = num;
		}
		void setflag2(int num)
		{
			flag2 = num;
		}
		void setflag3(int num)
		{
			flag3 = num;
		}
		void setflag4(int num)
		{
			flag4 = num;
		}
		void setflag5(int num)
		{
			flag5 = num;
		}
		int getflag1()
		{
			return flag1;
		}
		int getflag2()
		{
			return flag2;
		}
		int getflag3()
		{
			return flag3;
		}
		int getflag4()
		{
			return flag4;
		}
		int getflag5()
		{
			return flag5;
		}
	};
	Timevar *time;								   // for time calculation
	SomeVar *var;								   // important variables
	GameObject *gameObjects[500][500] = {nullptr}; // for brick
	Paddle *upperPaddle;						   // upper paddle
	Paddle *lowerPaddle;						   // lower paddle
	Ball *ball[MAX_BALLS] = {nullptr};			   // array of bals
	PowerUp *powerUp = nullptr;
	static const int MAX_POWERUPS = 1000; // Maximum number of power-ups
	PowerUp *powerUps[MAX_POWERUPS];	  // Array to store power-ups
	int numPowerUps = 0;				  // Number of active power-ups
	void checkBallBrickCollision()		  // this function check all side of ball brick collision
	{
		for (int i = 0; i < MAX_BALLS; i++)
		{
			Ball *ball = this->ball[i];
			if (ball != nullptr)
			{

				for (int row = 0; row < var->getBRICK_ROWS(); row++)
				{
					for (int col = 0; col < var->getBRICK_COLS(); col++)
					{
						Brick *brick = dynamic_cast<Brick *>(GameState::gameObjects[row][col]);

						if (brick != nullptr)
						{
							float ballLeft = ball->getX() - BALL_RADIUS;
							float ballRight = ball->getX() + BALL_RADIUS;
							float ballTop = ball->getY() - BALL_RADIUS;
							float ballBottom = ball->getY() + BALL_RADIUS;

							float brickLeft = brick->getX();
							float brickRight = brick->getX() + brick->getWidth();
							float brickTop = brick->getY();
							float brickBottom = brick->getY() + brick->getHeight();

							// Check if the ball's bounding box intersects with the brick's bounding box
							if (ballRight >= brickLeft && ballLeft <= brickRight && ballBottom >= brickTop && ballTop <= brickBottom)
							{
								// Determine the side of collision
								float overlapLeft = ballRight - brickLeft;
								float overlapRight = brickRight - ballLeft;
								float overlapTop = ballBottom - brickTop;
								float overlapBottom = brickBottom - ballTop;

								float minOverlapX = std::min(overlapLeft, overlapRight);
								float minOverlapY = std::min(overlapTop, overlapBottom);

								// Handle collision based on the minimum overlap
								if (minOverlapX < minOverlapY)
								{
									// Collision is horizontal
									ball->setVelocityX(-ball->getVelocityX());
								}
								else
								{
									// Collision is vertical
									ball->setVelocityY(-ball->getVelocityY());
								}

								brick->incrementCollisions();

								// Update brick color and count based on collision
								int currentCount = brick->getCollisions();
								const float *currentColor = brick->getColor();

								// Decrement count and update color based on rules
								if (currentColor == GameState::BLUE)
								{

									if (currentCount > 0)
									{
										brick->setColor(GameState::RED);
									}
								}
								else if (currentColor == GameState::RED)
								{
									if (currentCount > 0)
									{
										brick->setColor(GameState::YELLOW);
									}
								}
								else if (currentColor == GameState::YELLOW)
								{
									if (currentCount > 0)
									{
										brick->setColor(GameState::PINK);
									}
								}
								else if (currentColor == GameState::PINK)
								{
									if (currentCount > 0)
									{
										brick->setColor(GameState::GREEN);
									}
								}

								// Handle brick destruction if count exceeds maximum
								if (brick->getCollisions() >= brick->getMaxCollisions())
								{
									if (numPowerUps < MAX_POWERUPS)
									{ // Check if there's room for another power-up
										// Create a power-up
										PowerUp *powerUp = new PowerUp(brick->getX(), brick->getY(), 20, 20, brick->getColor1(), brick->getColor1(), speed);
										var->setscore(var->getscore() + 3);
										// Add the power-up to the array
										powerUps[numPowerUps++] = powerUp;
									}

									delete brick;
									GameState::gameObjects[row][col] = nullptr;
								}

								// No need for brickIsDestroyed condition
								// Exit the loop since we've handled the collision for this ball
								return;
							}
						}
					}
				}
			}
		}
	}

	bool checkBallLowerPaddleCollision() // this function check collision with lower paddle
	{
		for (int i = 0; i < MAX_BALLS; i++)
		{
			Ball *ball = this->ball[i];
			if (ball != nullptr)
			{
				GameState::Paddle *paddle = this->lowerPaddle;
				bool collisionDetected = false;

				// Check collision with the lower paddle (top and bottom)
				if ((ball->getY() + BALL_RADIUS >= paddle->getY() && ball->getY() - BALL_RADIUS <= paddle->getY() + paddle->getHeight()) ||
					(ball->getY() - BALL_RADIUS <= paddle->getY() + paddle->getHeight() && ball->getY() + BALL_RADIUS >= paddle->getY()))
				{
					if (ball->getX() + BALL_RADIUS >= paddle->getX() && ball->getX() - BALL_RADIUS <= paddle->getX() + paddle->getWidth())
					{
						// Adjust ball position to prevent overlap
						if (ball->getY() <= paddle->getY() + paddle->getHeight() / 2)
						{
							ball->setY(paddle->getY() - BALL_RADIUS);
							ball->setVelocityY(-abs(ball->getVelocityY())); // Reflect ball's vertical velocity
						}
						else
						{
							ball->setY(paddle->getY() + paddle->getHeight() + BALL_RADIUS);
							ball->setVelocityY(abs(ball->getVelocityY())); // Reflect ball's vertical velocity
						}
						// Detect if the ball hits the sides of the paddle
						if (ball->getX() < paddle->getX() || ball->getX() > paddle->getX() + paddle->getWidth())
						{
							ball->setVelocityX(-ball->getVelocityX()); // Reflect ball's horizontal velocity
						}
						collisionDetected = true;
					}
				}

				// Check collision with the left or right side of the paddle
				if (ball->getX() - BALL_RADIUS <= paddle->getX() + paddle->getWidth() &&
					ball->getX() + BALL_RADIUS >= paddle->getX() &&
					ball->getY() >= paddle->getY() && ball->getY() <= paddle->getY() + paddle->getHeight())
				{
					// Adjust ball position to prevent overlap
					ball->setX(paddle->getX() + paddle->getWidth() + BALL_RADIUS);
					ball->setVelocityX(abs(ball->getVelocityX())); // Reflect ball's horizontal velocity
					collisionDetected = true;
				}

				if (ball->getX() + BALL_RADIUS >= paddle->getX() &&
					ball->getX() - BALL_RADIUS <= paddle->getX() + paddle->getWidth() &&
					ball->getY() >= paddle->getY() && ball->getY() <= paddle->getY() + paddle->getHeight())
				{
					// Adjust ball position to prevent overlap
					ball->setX(paddle->getX() - BALL_RADIUS);
					ball->setVelocityX(-abs(ball->getVelocityX())); // Reflect ball's horizontal velocity
					collisionDetected = true;
				}

				if (collisionDetected)
				{
					return true; // Return true if collision detected for any ball
				}
			}
		}
		return false; // Return false if no collision detected for any ball
	}

	bool checkBallUpperPaddleCollision() // this is for upper
	{
		for (int i = 0; i < MAX_BALLS; i++)
		{
			Ball *ball = this->ball[i];
			if (ball != nullptr)
			{
				GameState::Paddle *paddle = this->upperPaddle;
				bool collisionDetected = false;

				// Check collision with the upper paddle (top and bottom)
				if ((ball->getY() - BALL_RADIUS <= paddle->getY() + paddle->getHeight() && ball->getY() + BALL_RADIUS >= paddle->getY()) ||
					(ball->getY() + BALL_RADIUS >= paddle->getY() && ball->getY() - BALL_RADIUS <= paddle->getY() + paddle->getHeight()))
				{
					if (ball->getX() + BALL_RADIUS >= paddle->getX() && ball->getX() - BALL_RADIUS <= paddle->getX() + paddle->getWidth())
					{
						// Adjust ball position to prevent overlap
						if (ball->getY() <= paddle->getY() + paddle->getHeight() / 2)
						{
							ball->setY(paddle->getY() - BALL_RADIUS);
							ball->setVelocityY(-abs(ball->getVelocityY())); // Reflect ball's vertical velocity
						}
						else
						{
							ball->setY(paddle->getY() + paddle->getHeight() + BALL_RADIUS);
							ball->setVelocityY(abs(ball->getVelocityY())); // Reflect ball's vertical velocity
						}
						// Detect if the ball hits the sides of the paddle
						if (ball->getX() < paddle->getX() || ball->getX() > paddle->getX() + paddle->getWidth())
						{
							ball->setVelocityX(-ball->getVelocityX()); // Reflect ball's horizontal velocity
						}
						collisionDetected = true;
					}
				}

				// Check collision with the left or right side of the paddle
				if (ball->getX() - BALL_RADIUS <= paddle->getX() + paddle->getWidth() &&
					ball->getX() + BALL_RADIUS >= paddle->getX() &&
					ball->getY() >= paddle->getY() && ball->getY() <= paddle->getY() + paddle->getHeight())
				{
					// Adjust ball position to prevent overlap
					ball->setX(paddle->getX() + paddle->getWidth() + BALL_RADIUS);
					ball->setVelocityX(abs(ball->getVelocityX())); // Reflect ball's horizontal velocity
					collisionDetected = true;
				}

				if (ball->getX() + BALL_RADIUS >= paddle->getX() &&
					ball->getX() - BALL_RADIUS <= paddle->getX() + paddle->getWidth() &&
					ball->getY() >= paddle->getY() && ball->getY() <= paddle->getY() + paddle->getHeight())
				{
					// Adjust ball position to prevent overlap
					ball->setX(paddle->getX() - BALL_RADIUS);
					ball->setVelocityX(-abs(ball->getVelocityX())); // Reflect ball's horizontal velocity
					collisionDetected = true;
				}

				if (collisionDetected)
				{
					return true; // Return true if collision detected for any ball
				}
			}
		}
		return false; // Return false if no collision detected for any ball
	}
};

GameState gameState; // golbal object of Gamestate

class GameFunctions // class of game function
{
public:
	static void checkPowerUpCollisionWithPaddle() // power collision with paddle
	{
		for (int i = 0; i < gameState.numPowerUps; ++i)
		{
			if (gameState.powerUps[i] != nullptr)
			{
				if (gameState.powerUps[i]->getX() + gameState.powerUps[i]->getWidth() >= gameState.lowerPaddle->getX() &&
					gameState.powerUps[i]->getX() <= gameState.lowerPaddle->getX() + gameState.lowerPaddle->getWidth() &&
					gameState.powerUps[i]->getY() + gameState.powerUps[i]->getHeight() >= gameState.lowerPaddle->getY() &&
					gameState.powerUps[i]->getY() <= gameState.lowerPaddle->getY() + gameState.lowerPaddle->getHeight())
				{

					// Collision detected with lower paddle
					if (gameState.powerUps[i]->getColor2() == gameState.GREEN)
					{
						if (gameState.var->getlevel() == 3)
						{
							gameState.upperPaddle->setWidth(gameState.lowerPaddle->getWidth() * 2);
						}
						gameState.lowerPaddle->setWidth(gameState.lowerPaddle->getWidth() * 2);

						gameState.time->setvar1(313);
						gameState.time->setflag1(1);
					}
					if (gameState.powerUps[i]->getColor2() == gameState.PINK)
					{
						if (gameState.var->getlevel() == 3)
						{
							gameState.upperPaddle->setWidth(gameState.lowerPaddle->getWidth() / 2);
						}
						gameState.lowerPaddle->setWidth(gameState.lowerPaddle->getWidth() / 2);

						gameState.time->setvar2(313);
						gameState.time->setflag2(1);
					}
					if (gameState.powerUps[i]->getColor2() == gameState.BLUE)
					{
						for (int j = 0; j < MAX_BALLS; j++)
						{
							if (gameState.ball[j] != nullptr)
							{
								int velX = gameState.ball[j]->getVelocityX();
								int velY = gameState.ball[j]->getVelocityY();
								if ((velX > 2 || velX < -2) && (velY > 2 || velY < -2))
								{
									gameState.ball[j]->setVelocityX(velX / 2);
									gameState.ball[j]->setVelocityY(velY / 2);
								}
								else
								{
									gameState.ball[j]->setVelocityX(1);
									gameState.ball[j]->setVelocityY(1);
								}
							}
						}
						gameState.time->setvar3(313);
						gameState.time->setflag3(1);
					}

					if (gameState.powerUps[i]->getColor2() == gameState.RED)
					{
						for (int j = 0; j < MAX_BALLS; j++)
						{
							if (gameState.ball[j] != nullptr)
							{
								int velX = gameState.ball[j]->getVelocityX();
								int velY = gameState.ball[j]->getVelocityY();
								if ((velX < 7 && velX > -7) && (velY < 7 && velY > -7))
								{
									gameState.ball[j]->setVelocityX(velX * 2);
									gameState.ball[j]->setVelocityY(velY * 2);
								}
								else
								{
									gameState.ball[j]->setVelocityX(10);
									gameState.ball[j]->setVelocityY(10);
								}
							}
						}
						gameState.time->setvar4(313);
						gameState.time->setflag4(1);
					}
					if (gameState.powerUps[i]->getColor2() == gameState.YELLOW)
					{
						int arr1[2];
						int arr2[2];

						// Determine velocities for the first new ball
						if (gameState.ball[0]->getVelocityX() > 0)
						{
							arr1[0] = -1;
						}
						else
						{
							arr1[0] = 1;
						}
						if (gameState.ball[0]->getVelocityY() > 0)
						{
							arr2[0] = 1;
						}
						else
						{
							arr2[0] = -1;
						}

						// Determine velocities for the second new ball
						if (gameState.ball[0]->getVelocityX() > 0)
						{
							arr1[1] = 1;
						}
						else
						{
							arr1[1] = -1;
						}
						if (gameState.ball[0]->getVelocityY() > 0)
						{
							arr2[1] = -1;
						}
						else
						{
							arr2[1] = 1;
						}

						// Add two additional balls with different velocities
						for (int k = 0; k < 2; ++k)
						{
							for (int j = 0; j < MAX_BALLS; ++j)
							{
								if (gameState.ball[j] == nullptr)
								{
									// Create a new ball object
									gameState.ball[j] = new GameState::Ball(gameState.ball[0]->GetX(), gameState.ball[0]->GetY(), BALL_RADIUS, gameState.WHITE, arr1[k] * gameState.ball[0]->getVelocityX(), arr2[k] * gameState.ball[0]->getVelocityY());
									// Set initial velocity or position of the new ball if needed
									break;
								}
							}
						}
						gameState.time->setvar5(313);
						gameState.time->setflag5(1);
					}

					// Remove the power-up from the array
					delete gameState.powerUps[i];
					gameState.powerUps[i] = nullptr;
				}
			}
		}

		// Decrement variables and flags
		if (gameState.time->getflag1())
		{
			gameState.time->setvar1(gameState.time->getvar1() - 1);
		}
		if (gameState.time->getflag2())
		{
			gameState.time->setvar2(gameState.time->getvar2() - 1);
		}
		if (gameState.time->getflag3())
		{
			gameState.time->setvar3(gameState.time->getvar3() - 1);
		}
		if (gameState.time->getflag4())
		{
			gameState.time->setvar4(gameState.time->getvar4() - 1);
		}
		if (gameState.time->getflag5())
		{
			gameState.time->setvar5(gameState.time->getvar5() - 1);
		}

		// Reset power-up effects if their duration has expired
		if (gameState.time->getvar1() == 0)
		{
			gameState.lowerPaddle->setWidth(140);
			gameState.upperPaddle->setWidth(140);
			gameState.time->setvar1(1);
			gameState.time->setflag1(0);
		}
		if (gameState.time->getvar2() == 0)
		{
			gameState.lowerPaddle->setWidth(140);
			gameState.upperPaddle->setWidth(140);
			gameState.time->setvar2(1);
			gameState.time->setflag2(0);
		}
		if (gameState.time->getvar3() == 0)
		{
			int num1 = 0, num2 = 0;
			for (int j = 0; j < MAX_BALLS; j++)
			{
				if (gameState.ball[j] != nullptr)
				{
					// Determine the new x-velocity
					if (gameState.ball[j]->getVelocityX() < 0)
					{
						num1 = -4;
					}
					else
					{
						num1 = 4;
					}
					// Determine the new y-velocity
					if (gameState.ball[j]->getVelocityY() < 0)
					{
						num2 = -4;
					}
					else
					{
						num2 = 4;
					}
					// Set the new velocities
					gameState.ball[j]->setVelocityX(num1);
					gameState.ball[j]->setVelocityY(num2);
				}
			}
			gameState.time->setvar3(1);
			gameState.time->setflag3(0);
		}
		if (gameState.time->getvar4() == 0)
		{
			int num1 = 0, num2 = 0;
			for (int j = 0; j < MAX_BALLS; j++)
			{
				if (gameState.ball[j] != nullptr)
				{
					// Determine the new x-velocity
					if (gameState.ball[j]->getVelocityX() < 0)
					{
						num1 = -4;
					}
					else
					{
						num1 = 4;
					}
					// Determine the new y-velocity
					if (gameState.ball[j]->getVelocityY() < 0)
					{
						num2 = -4;
					}
					else
					{
						num2 = 4;
					}
					// Set the new velocities
					gameState.ball[j]->setVelocityX(num1);
					gameState.ball[j]->setVelocityY(num2);
				}
			}
			gameState.time->setvar4(1);
			gameState.time->setflag4(0);
		}
		if (gameState.time->getvar5() == 0)
		{
			for (int j = 1; j < MAX_BALLS; ++j)
			{
				if (gameState.ball[j] != nullptr)
				{
					// Remove the ball from the array
					delete gameState.ball[j];
					gameState.ball[j] = nullptr;
					// Reset ballInfo
				}
			}

			gameState.time->setvar5(1);
			gameState.time->setflag5(0);
		}
	}
	static void checkVelocity() // some all balls velocity is zero to tackle this i made this funtion
	{

		for (int j = 0; j < MAX_BALLS; ++j)
		{
			if (gameState.ball[j] != nullptr && gameState.ball[j]->getVelocityX() == 0 && gameState.ball[j]->getVelocityY() == 0)
			{
				gameState.ball[j]->setVelocityX((rand() % 2 == 0 ? 4 : -4));
				gameState.ball[j]->setVelocityY(-4);
			}
		}
	}

	static void originalBallFailsToCollideWithPaddle() // function check if ball fall down and other balls have
	{
		if (gameState.ball[0]->GetY() > 1050)
		{
			// Initialize the index of the ball to swap with as -1
			int swapIndex = -1;

			// Find the best ball from the array to swap with the original ball
			for (int j = 1; j < MAX_BALLS; ++j)
			{
				if (gameState.ball[j] != nullptr && gameState.ball[j]->GetY() <= 1050)
				{
					// Check if there's no ball to swap with yet or if this ball is closer to the paddle
					if (swapIndex == -1 || gameState.ball[j]->GetY() > gameState.ball[swapIndex]->GetY())
					{
						swapIndex = j;
					}
				}
			}

			// If a valid ball to swap with is found, perform the swap
			if (swapIndex != -1)
			{
				GameState::Ball *temp = gameState.ball[0];
				gameState.ball[0] = gameState.ball[swapIndex];
				gameState.ball[swapIndex] = temp;
			}
		}
		if (gameState.var->getlevel() == 3)
		{
			if (gameState.ball[0]->GetY() <= 70)
			{
				// Initialize the index of the ball to swap with as -1
				int swapIndex = -1;

				// Find the best ball from the array to swap with the original ball
				for (int j = 1; j < MAX_BALLS; ++j)
				{
					if (gameState.ball[j] != nullptr && gameState.ball[j]->GetY() <= 1050)
					{
						// Check if there's no ball to swap with yet or if this ball is closer to the paddle
						if (swapIndex == -1 || gameState.ball[j]->GetY() > gameState.ball[swapIndex]->GetY())
						{
							swapIndex = j;
						}
					}
				}

				// If a valid ball to swap with is found, perform the swap
				if (swapIndex != -1)
				{
					GameState::Ball *temp = gameState.ball[0];
					gameState.ball[0] = gameState.ball[swapIndex];
					gameState.ball[swapIndex] = temp;
				}
			}
		}
	}
	static void createBricks1() // level1 brick
	{
		srand(time(NULL));
		// Calculate the total width of all bricks in a row
		int totalBrickWidth = gameState.var->getBRICK_COLS() * BRICK_WIDTH;
		// Calculate the starting x-coordinate to center the bricks horizontally
		int startX = 20;

		// Calculate the total height of all bricks in a column
		int totalBrickHeight = gameState.var->getBRICK_ROWS() * BRICK_HEIGHT;
		// Calculate the starting y-coordinate to center the bricks vertically
		int startY = 0;

		for (int row = 0; row < gameState.var->getBRICK_ROWS(); row++)
		{
			for (int col = 0; col < gameState.var->getBRICK_COLS(); col++)
			{
				float brick_x = startX + col * BRICK_WIDTH;	 // Adjusted x-coordinate
				float brick_y = startY + row * BRICK_HEIGHT; // Adjusted y-coordinate
				const float *brick_color;
				const float *brick_color1;
				int maxCollisions;

				// Randomly select a color for each brick
				switch (rand() % 5)
				{
				case 0:
					brick_color = gameState.RED;
					brick_color1 = gameState.RED;
					maxCollisions = 4;
					break;
				case 1:
					brick_color = gameState.GREEN;
					brick_color1 = gameState.GREEN;
					maxCollisions = 1;
					break;
				case 2:
					brick_color = gameState.BLUE;
					brick_color1 = gameState.BLUE;
					maxCollisions = 5;
					break;
				case 3:
					brick_color = gameState.YELLOW;
					brick_color1 = gameState.YELLOW;
					maxCollisions = 3;
					break;
				case 4:
					brick_color = gameState.PINK;
					brick_color1 = gameState.PINK;
					maxCollisions = 2;
					break;
				}

				gameState.gameObjects[row][col] = new GameState::Brick(brick_x, brick_y, BRICK_WIDTH, BRICK_HEIGHT, brick_color, brick_color1, maxCollisions);
			}
		}
	}

	static void createBricks2() // level2 bricks
	{
		srand(time(NULL));
		// Calculate the total width of all bricks in a row
		int totalBrickWidth = gameState.var->getBRICK_COLS() * BRICK_WIDTH;
		// Calculate the starting x-coordinate to center the bricks horizontally
		int startX = 20;

		// Calculate the total height of all bricks in a column
		int totalBrickHeight = gameState.var->getBRICK_ROWS() * BRICK_HEIGHT;
		// Calculate the starting y-coordinate to center the bricks vertically
		int startY = 0;

		for (int row = 0; row < gameState.var->getBRICK_ROWS(); row++)
		{
			for (int col = 0; col < gameState.var->getBRICK_COLS(); col++)
			{
				float brick_x = startX + col * BRICK_WIDTH;	 // Adjusted x-coordinate
				float brick_y = startY + row * BRICK_HEIGHT; // Adjusted y-coordinate
				const float *brick_color;
				const float *brick_color1;
				int maxCollisions;

				// Randomly select a color for each brick
				switch (rand() % 5)
				{
				case 0:
					brick_color = gameState.RED;
					brick_color1 = gameState.RED;
					maxCollisions = 4;
					break;
				case 1:
					brick_color = gameState.GREEN;
					brick_color1 = gameState.GREEN;
					maxCollisions = 1;
					break;
				case 2:
					brick_color = gameState.BLUE;
					brick_color1 = gameState.BLUE;
					maxCollisions = 5;
					break;
				case 3:
					brick_color = gameState.YELLOW;
					brick_color1 = gameState.YELLOW;
					maxCollisions = 3;
					break;
				case 4:
					brick_color = gameState.PINK;
					brick_color1 = gameState.PINK;
					maxCollisions = 2;
					break;
				}
				if ((col % 15 < 5 && row % 30 < 6) ||
					(col % 15 >= 5 && col % 15 < 10 && row % 30 >= 6 && row % 30 < 12) ||
					(col % 15 >= 10 && col % 15 < 15 && row % 30 >= 12 && row % 30 < 18) ||
					(col % 15 < 5 && row % 30 < 30 && row % 30 >= 24) ||
					(col % 15 >= 5 && col % 15 < 10 && row % 30 >= 18 && row % 30 < 24))
				{
					gameState.gameObjects[row][col] = new GameState::Brick(brick_x, brick_y, BRICK_WIDTH, BRICK_HEIGHT, brick_color, brick_color1, maxCollisions);
				}
			}
		}
	}

	static void createBricksRecursive(int row, int col, int startX, int startY) // level3 bricks
	{
		if (row >= gameState.var->getBRICK_ROWS()) // Base case: stop when all rows are filled
			return;

		// Calculate adjusted coordinates for the brick
		float brick_x = startX + col * BRICK_WIDTH;
		float brick_y = startY + row * BRICK_HEIGHT;

		// Randomly select a color for the brick
		const float *brick_color;
		const float *brick_color1;
		int maxCollisions;

		switch (rand() % 5)
		{
		case 0:
			brick_color = gameState.RED;
			brick_color1 = gameState.RED;
			maxCollisions = 4;
			break;
		case 1:
			brick_color = gameState.GREEN;
			brick_color1 = gameState.GREEN;
			maxCollisions = 1;
			break;
		case 2:
			brick_color = gameState.BLUE;
			brick_color1 = gameState.BLUE;
			maxCollisions = 5;
			break;
		case 3:
			brick_color = gameState.YELLOW;
			brick_color1 = gameState.YELLOW;
			maxCollisions = 3;
			break;
		case 4:
			brick_color = gameState.PINK;
			brick_color1 = gameState.PINK;
			maxCollisions = 2;
			break;
		}

		// Add brick to gameObjects if it meets the condition
		if ((row == 0 && col >= 0 && col < 15) ||
			(row == 1 && ((col >= 0 && col < 7) || (col >= 8 && col < 15))) ||
			(row == 2 && ((col >= 0 && col < 6) || (col >= 9 && col < 15))) ||
			(row == 3 && ((col >= 0 && col < 5) || (col >= 10 && col < 15))) ||
			(row == 4 && ((col >= 0 && col < 4) || (col >= 11 && col < 15))) ||
			(row == 5 && ((col >= 0 && col < 3) || (col >= 12 && col < 15))) ||
			(row == 6 && ((col >= 0 && col < 1) || (col >= 14 && col < 15))) ||
			(row == 7 && ((col >= 0 && col < 1) || (col >= 14 && col < 15))) ||
			(row == 8 && ((col >= 0 && col < 3) || (col >= 12 && col < 15))) ||
			(row == 9 && ((col >= 0 && col < 4) || (col >= 11 && col < 15))) ||
			(row == 10 && ((col >= 0 && col < 5) || (col >= 10 && col < 15))) ||
			(row == 11 && ((col >= 0 && col < 6) || (col >= 9 && col < 15))) ||
			(row == 12 && ((col >= 0 && col < 7) || (col >= 8 && col < 15))) ||
			(row == 13 && col >= 0 && col < 15))
		{
			gameState.gameObjects[row][col] = new GameState::Brick(brick_x, brick_y, BRICK_WIDTH, BRICK_HEIGHT, brick_color, brick_color1, maxCollisions);
		}
		// Move to the next column or row
		if (col < gameState.var->getBRICK_COLS() - 1)			 // If there are more columns in this row
			createBricksRecursive(row, col + 1, startX, startY); // Move to the next column
		else													 // If this is the last column in the row
			createBricksRecursive(row + 1, 0, startX, startY);	 // Move to the next row
	}

	static void createBricks3()
	{
		srand(time(NULL));
		int totalBrickWidth = gameState.var->getBRICK_COLS() * BRICK_WIDTH;
		int startX = 20;
		int totalBrickHeight = gameState.var->getBRICK_ROWS() * BRICK_HEIGHT;
		int startY = 350;
		createBricksRecursive(0, 0, startX, startY);
	}

	static void initGame() // initialize some imporant things
	{
		gameState.time = new GameState::Timevar(1, 1, 1, 1, 1, 0, 0, 0, 0, 0);
		gameState.var = new GameState::SomeVar(0, 0, 0, 3, 0);
		gameState.upperPaddle = new GameState::Paddle(580, 50, PADDLE_WIDTH, PADDLE_HEIGHT, gameState.WHITE);
		gameState.lowerPaddle = new GameState::Paddle(580, 1020, PADDLE_WIDTH, PADDLE_HEIGHT, gameState.WHITE);
	}

	static void NonPrintableKeys(int key, int x, int y) // non printable key
	{
		if (key == GLUT_KEY_LEFT)
		{
			gameState.upperPaddle->move(-10);
			if (gameState.upperPaddle->getX() < 0)
			{
				gameState.upperPaddle->move(10); // Move back to previous position
			}
		}
		else if (key == GLUT_KEY_RIGHT)
		{
			gameState.upperPaddle->move(10);
			if (gameState.upperPaddle->getX() + gameState.upperPaddle->getWidth() > gameState.SCREEN_WIDTH)
			{
				gameState.upperPaddle->move(-10); // Move back to previous position
			}
		}

		glutPostRedisplay();
	}

	static void MousePressedAndMoved(int x, int y) // mouse pressed funtion
	{
		// Calculate the new x-coordinate for the paddle based on the mouse position
		int newPaddleX = x - gameState.lowerPaddle->getWidth() / 2;

		// Ensure that /the paddle stays within the screen boundaries
		if (newPaddleX >= 0 && newPaddleX + gameState.lowerPaddle->getWidth() <= gameState.SCREEN_WIDTH)
		{
			// Move the paddle to the new x-coordinate
			gameState.lowerPaddle->move(newPaddleX - gameState.lowerPaddle->getX());
		}

		glutPostRedisplay();
	}

	static void MouseMoved(int x, int y)
	{
		// cout << x << " " << y << endl;
		glutPostRedisplay();
	}
	static void PrintableKeys(unsigned char key, int x, int y) // print able keys
	{

		if (key == 27) // quit game
		{

			exit(1);
		}
		if (key == '1') // for level 1
		{
			gameState.var->setLevel(1);
			gameState.var->setBRICK_ROWS(15);
			gameState.var->setBRICK_COLS(15);
			for (int i = 0; i < 500; i++)
			{
				for (int j = 0; j < 500; j++)
				{
					gameState.gameObjects[i][j] = nullptr;
				}
			}
			gameState.ball[0] = new GameState::Ball(650, 1020, BALL_RADIUS, gameState.WHITE, (rand() % 2 == 0 ? 4 : -4), 4);

			createBricks1();
		}
		if (key == '2') // for level 2
		{
			gameState.var->setLevel(2);
			for (int i = 0; i < 500; i++)
			{
				for (int j = 0; j < 500; j++)
				{
					gameState.gameObjects[i][j] = nullptr;
				}
			}
			gameState.var->setBRICK_COLS(15);
			gameState.var->setBRICK_ROWS(30);
			gameState.ball[0] = new GameState::Ball(650, 1020, BALL_RADIUS, gameState.WHITE, (rand() % 2 == 0 ? 4 : -4), 4);

			createBricks2();
		}
		if (key == '3') // for level 3
		{
			gameState.var->setLevel(3);
			for (int i = 0; i < 500; i++)
			{
				for (int j = 0; j < 500; j++)
				{
					gameState.gameObjects[i][j] = nullptr;
				}
			}
			gameState.var->setBRICK_COLS(15);
			gameState.var->setBRICK_ROWS(14);
			gameState.ball[0] = new GameState::Ball(670, 1020, BALL_RADIUS, gameState.WHITE, (rand() % 2 == 0 ? 4 : -4), 4);
			createBricks3();
		}

		glutPostRedisplay();
	}

	static void MouseClicked(int button, int state, int x, int y)
	{

		if (button == GLUT_LEFT_BUTTON) // dealing only with left button
		{
			cout << GLUT_DOWN << " " << GLUT_UP << endl;
		}
		else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
		{
			cout << "Right Button Pressed" << endl;
		}
		glutPostRedisplay();
	}
	static void handleAllBallsCrossedThreshold() // function check ball fall down and player have remaing live
	{
		// Decrement lives if lives are available
		if (gameState.var->getlives() > 1)
		{
			cout << gameState.var->getlives();
			int num = gameState.var->getlives() - 1;
			cout << num;
			gameState.var->setlive(num);
			gameState.upperPaddle = new GameState::Paddle(600, 90, PADDLE_WIDTH, PADDLE_HEIGHT, gameState.WHITE);
			gameState.lowerPaddle = new GameState::Paddle(600, 1020, PADDLE_WIDTH, PADDLE_HEIGHT, gameState.WHITE);
			gameState.ball[0] = new GameState::Ball(660, 1000, BALL_RADIUS, gameState.WHITE, (rand() % 2 == 0 ? 4 : -4), -4);
		}
		else
		{
			gameState.var->setlive(-1);
		}
	}

	static void allBallsCrossedThreshold() // check all ball croos the thershold
	{									   // for lower
		cout << gameState.var->getlives();
		bool allCrossed = true;
		for (int j = 0; j < MAX_BALLS; ++j)
		{
			if (gameState.ball[j] != nullptr && gameState.ball[j]->GetY() < 1070)
			{
				allCrossed = false;
				break;
			}
		}
		if (allCrossed)
		{
			GameFunctions::handleAllBallsCrossedThreshold();
		}
	}
	static void allBallsCrossedThresholForUpper() // same above functionality but for level3
	{
		cout << gameState.var->getlives();
		bool allCrossed = true;
		for (int j = 0; j < MAX_BALLS; ++j)
		{
			if (gameState.ball[j] != nullptr && gameState.ball[j]->GetY() > 40)
			{
				allCrossed = false;
				break;
			}
		}
		if (allCrossed)
		{
			GameFunctions::handleAllBallsCrossedThreshold();
		}
	}
	static void drawString(float x, float y, const char *string, float r, float g, float b)
	{
		glColor3f(r, g, b);	 // Set text color
		glRasterPos2f(x, y); // Set position for text rendering
		for (const char *c = string; *c != '\0'; ++c)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); // Render each character
		}
	}
	
static	int getHighScore(const std::string& filename) {
    int highScore = 0;  // Initialize to 0 or any other appropriate default value

    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return highScore;  // Return default value if file cannot be opened
    }

    if (inFile >> highScore) {
        // Read the first score successfully
        int score;
        while (inFile >> score) {
            // Loop through the rest of the scores and update highScore if needed
            if (score > highScore) {
                highScore = score;
            }
        }
        inFile.close();
    } else {
        std::cerr << "Error: Could not read from file " << filename << std::endl;
    }

    return highScore;
}

	static void display() // dispaly function
	{
		glClear(GL_COLOR_BUFFER_BIT);

		if (gameState.var->getlevel() == 0) // for start
		{	
			GameFunctions::drawString(500, 100, "ABDUL REHMAN", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 500, "Start Game", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 530, "Press 1 for Level 1 ", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 560, "Press 2 for Level 2 ", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 590, "Press 3 for Level 3 ", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 620, "Exit (Press ESC) ", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 650, "23 2082", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 680, "High Score ::", 1.0, 1.0, 1.0);
			int score = GameFunctions::getHighScore("scores.txt");

        // Convert the high score to a string
        std::stringstream ss;
        ss << score;
        std::string scoreString = ss.str();
        const char* scoreCString = scoreString.c_str();

        // Draw the high score
        GameFunctions::drawString(630, 680, scoreCString, 1.0, 1.0, 1.0);
			glutSwapBuffers();
			return;
		}
	if (gameState.var->getlives() == -1) // Assuming getLives() returns the player's remaining lives
{
    GameFunctions::drawString(500, 500, "Game End !", 1.0, 1.0, 1.0);
    int score = gameState.var->getscore(); // Assuming getScore() returns the player's score
    stringstream ss;
    ss << "Score: " << score; // Concatenate "Score: " with the score value
    std::string scoreString = ss.str(); // Convert stringstream to string
    const char* scoreCString = scoreString.c_str(); // Convert string to C-string
    GameFunctions::drawString(500, 571, scoreCString, 1.0, 1.0, 1.0); // Draw the score
    ofstream outFile("scores.txt", ios::app); // Open the file in append mode
    if (outFile.is_open()) {
        outFile << score << endl; // Write the score to the file
        outFile.close(); // Close the file
        GameFunctions::drawString(470, 540, "Exit (Press ESC) ", 1.0, 1.0, 1.0);
        glutSwapBuffers(); // Update the screen
        return; // Return from the function or exit the game loop
    } else {
        // Failed to open the file
        cerr << "Error: Could not open highscores.txt for writing." << endl;
        // Handle the error, possibly by displaying an error message to the player
        // and allowing them to retry or exit the game
    }
}
		if (gameState.var->getlevel() == -2)
		{
			GameFunctions::drawString(500, 530, "Press 1 for Level 1 ", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 560, "Press 2 for Level 2 ", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 590, "Press 3 for Level 3 ", 1.0, 1.0, 1.0);
			GameFunctions::drawString(500, 590, "Exit (Press ESC) ", 1.0, 1.0, 1.0);
			glutSwapBuffers();
			return;
		}
		std::string livesStr = std::to_string(gameState.var->getlives());
		std::string scoreStr = std::to_string(gameState.var->getscore());
		GameFunctions::drawString(35, 1070, "Score :: ", 0.0, 0.0, 1.0);
		GameFunctions::drawString(1100, 1070, "Lives ::", 0.0, 0.0, 1.0);
		GameFunctions::drawString(1180, 1070, livesStr.c_str(), 0.0, 0.0, 1.0);
		GameFunctions::drawString(110, 1070, scoreStr.c_str(), 0.0, 0.0, 1.0);
		// draw briks
		for (int row = 0; row < gameState.var->getBRICK_ROWS(); row++)
		{
			for (int col = 0; col < gameState.var->getBRICK_COLS(); col++)
			{
				if (gameState.gameObjects[row][col] != nullptr)
				{
					gameState.gameObjects[row][col]->draw();
				}
			}
		}

		// drae paddle
		gameState.lowerPaddle->draw();
		if (gameState.var->getlevel() == 3)
		{
			gameState.upperPaddle->draw();
		}
		// draw balls
		for (int i = 0; i < MAX_BALLS; ++i)
		{
			if (gameState.ball[i] != nullptr)
			{
				gameState.ball[i]->draw();
			}
		}
		// draw powerups
		for (int i = 0; i < gameState.numPowerUps; i++)
		{
			if (gameState.powerUps[i] != nullptr)
			{
				if (gameState.powerUps[i]->getColor2() == gameState.GREEN)
				{
					gameState.powerUps[i]->drawTriangle();
				}
				else if (gameState.powerUps[i]->getColor2() == gameState.PINK)
				{
					gameState.powerUps[i]->draw();
				}
				else if (gameState.powerUps[i]->getColor2() == gameState.BLUE)
				{
					gameState.powerUps[i]->drawCircle();
				}
				else if (gameState.powerUps[i]->getColor2() == gameState.RED)
				{
					gameState.powerUps[i]->drawSquare();
				}
				else if (gameState.powerUps[i]->getColor2() == gameState.YELLOW)
				{
					gameState.powerUps[i]->draw1();
				}
			}
		}
	}

	static void update(int value) // update funtion
	{
		// move balls
		if (gameState.var->getlevel() > 0)
		{
			for (int i = 0; i < MAX_BALLS; ++i)
			{
				if (gameState.ball[i] != nullptr)
				{
					gameState.ball[i]->move();
				}
			}
			// move power ups
			for (int i = 0; i < gameState.numPowerUps; i++)
			{

				if (gameState.powerUps[i] != nullptr)
				{
					gameState.powerUps[i]->move(); // Assuming the PowerUp class has a move() method
				}
			}
			for (int i = 0; i < MAX_BALLS; ++i)
			{
				if (gameState.ball[i] != nullptr)
				{
					if (gameState.ball[i]->getX() - BALL_RADIUS <= 0 || gameState.ball[i]->getX() + BALL_RADIUS >= gameState.SCREEN_WIDTH)
					{
						gameState.ball[i]->setVelocityX(-gameState.ball[i]->getVelocityX());
					}
				}
			}
			// Check for collisions with walls and adjust ball velocity

			for (int i = 0; i < MAX_BALLS; ++i)
			{
				if (gameState.ball[i] != nullptr)
				{
					if (gameState.ball[i]->getY() - BALL_RADIUS <= 0)
					{
						gameState.ball[i]->setVelocityY(-gameState.ball[i]->getVelocityY());
					}
				}
			}
			// Check for collision with ceiling and adjust ball velocity

			// Check for collision between power-up and paddle
			GameFunctions::checkPowerUpCollisionWithPaddle();

			// Check for collision with upper and lower paddles
			gameState.checkBallUpperPaddleCollision();
			gameState.checkBallLowerPaddleCollision();
			GameFunctions::originalBallFailsToCollideWithPaddle();
			GameFunctions::checkVelocity();
			// Check for collision with bricks and handle brick destruction
			gameState.checkBallBrickCollision();
			GameFunctions::allBallsCrossedThreshold();
			GameFunctions::allBallsCrossedThresholForUpper();
			glBegin(GL_QUADS);
			glColor3f(0.6, 0.3, 0.0); // Begin drawing quads
			glVertex2f(150, 1090);	  // Bottom-left vertex
			glVertex2f(1090, 1090);	  // Bottom-right vertex
			glVertex2f(1090, 1040);	  // Top-right vertex
			glVertex2f(150, 1040);	  // Top-left vertex
			glEnd();
			if (gameState.var->getlevel() == 3)
			{
				glBegin(GL_QUADS);
				glColor3f(0.6, 0.3, 0.0); // Begin drawing quads
				glVertex2f(0, 30);		  // Bottom-left vertex
				glVertex2f(1240, 30);	  // Bottom-right vertex
				glVertex2f(1240, 0);	  // Top-right vertex
				glVertex2f(0, 0);		  // Top-left vertex
				glEnd();
			}

			glutSwapBuffers();
		}

		glutPostRedisplay();
		// Call the update function again after a delay
		glutTimerFunc(16, update, 0);
	}
};
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(gameState.SCREEN_WIDTH, gameState.SCREEN_HEIGHT);
	glutCreateWindow("Brick Breaker");

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, gameState.SCREEN_WIDTH, gameState.SCREEN_HEIGHT, 0);

	GameFunctions::initGame();

	glutDisplayFunc(GameFunctions::display);
	glutSpecialFunc(GameFunctions::NonPrintableKeys);
	glutMouseFunc(GameFunctions::MouseClicked);
	glutPassiveMotionFunc(GameFunctions::MouseMoved);
	glutMotionFunc(GameFunctions::MousePressedAndMoved);
	glutKeyboardFunc(GameFunctions::PrintableKeys);
	glutTimerFunc(16, GameFunctions::update, 0);

	glutMainLoop();

	return 0;
}
