#include "stdafx.h" //remove before submitting
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/graphics.hpp>
#include <cstdlib>
#include <cmath>

using namespace std;

class Balls
{
public:
	Balls();
	sf::CircleShape main;
	sf::CircleShape mark;
	sf::RectangleShape stripe;
	sf::Text num;
	float speed;
	float vx;
	float vy;
	float angle;
	bool isPocketed;
	int collidedWith[9];
};

Balls::Balls()
{
};

//Dimensions
const float pi = 3.14159f;
int numBalls = 2;
int WIDTH = 800;
int HEIGHT = WIDTH / 2;
float FPS = 30;
float gameSpeed = 10;
float SPF = 1 / FPS;
float borderDepth = WIDTH / 25;
float pocketRad = borderDepth * .5f;
float ballRad = pocketRad * .65f;
float aimLength = 0;
float aimAngle = 0;
float aimStandardForce = .8 * .001f * gameSpeed;
float frictionForce = 1 * .001f * gameSpeed;

float borderTop = borderDepth;
float borderBot = HEIGHT - borderDepth;
float borderLeft = borderDepth;
float borderRight = WIDTH - borderDepth;


//Booleans
bool menuShow = true;
bool onePlayerGame = false;
bool gameStart = false;
bool twoPlayerGame = false;
bool playerIsAiming = false;
bool playerMovesBall = false;
bool gameIsMoving = false;
bool programExit = false;
bool gameQuit = false;

//Objects
sf::RenderWindow BILLIARDS(sf::VideoMode(WIDTH, HEIGHT, 64), "Billiards", sf::Style::Titlebar | sf::Style::Close);
sf::Font myFont;
sf::RectangleShape mat;
vector<sf::CircleShape> pockets(6);
vector<sf::RectangleShape> borders(4);
sf::Vertex aim[];
Balls a, b, c, d, e, f, g, h, i, j;
Balls ball[10] = { a, b, c, d, e, f, g, h, i, j };


//TitleMenu
sf::Text title;
vector<sf::RectangleShape> menuRect(2);
vector<sf::Text> menuText(2);

//Colors
sf::Color brown(54, 18, 8);
sf::Color darkGreen(0, 50, 0);
sf::Color green(0, 80, 0);
sf::Color indigo(75, 0, 130);
sf::Color orange(255, 128, 0);
sf::Color maroon(128, 0, 0);
sf::Color grey(125, 125, 125);

void ballsDraw()
{
	for (int i = 0; i < numBalls; i++)
	{
		ball[i].main.setRadius(ballRad);
		ball[i].main.setOrigin(ballRad, ballRad);
		ball[i].main.setOutlineThickness(1);
		ball[i].main.setOutlineColor(sf::Color::Black);
		if (i != 0)
		{
			ball[i].mark.setRadius(ballRad * .5f);
			ball[i].mark.setOrigin(ballRad * .5f, ballRad * .5f);
			string num = to_string(i);
			ball[i].num.setFont(myFont);
			ball[i].num.setCharacterSize(6);
			ball[i].num.setStyle(sf::Text::Regular);
			ball[i].num.setColor(sf::Color::Black);
			ball[i].num.setString(num);
			ball[i].num.setOrigin(ball[i].num.getCharacterSize() / 2.f, ball[i].num.getCharacterSize() / 2.f);

			switch (i)
			{
			case 1:
				ball[i].main.setFillColor(sf::Color::Yellow);
				break;
			case 2:
				ball[i].main.setFillColor(sf::Color::Blue);
				break;
			case 3:
				ball[i].main.setFillColor(sf::Color::Red);
				break;
			case 4:
				ball[i].main.setFillColor(indigo);
				break;
			case 5:
				ball[i].main.setFillColor(orange);
				break;
			case 6:
				ball[i].main.setFillColor(green);
				break;
			case 7:
				ball[i].main.setFillColor(maroon);
				break;
			case 8:
				ball[i].main.setFillColor(sf::Color::Black);
				break;
			case 9:
				sf::Vector2f stripeSize(1.8f * ballRad, ballRad);
				ball[i].stripe.setFillColor(sf::Color::Yellow);
				ball[i].stripe.setSize(stripeSize);
				ball[i].stripe.setOrigin(stripeSize.x / 2, stripeSize.y / 2);
				break;
			}
		}
	}

	for (int i = 0; i < numBalls; i++)
	{
		if (!ball[i].isPocketed)
		{
			BILLIARDS.draw(ball[i].main);
			if (i == 9)
			{
				BILLIARDS.draw(ball[i].stripe);
			}
			BILLIARDS.draw(ball[i].mark);
			BILLIARDS.draw(ball[i].num);
		}
	}
}

void menu()
{
	title.setFont(myFont);
	title.setCharacterSize(WIDTH / 16);
	title.setStyle(sf::Text::Bold);
	title.setColor(sf::Color::Black);
	title.setString("BILLIARDS");
	title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);	
	title.setPosition(WIDTH / 2, 3 * HEIGHT / 16);

	sf::Vector2f menuRectSize(3 * WIDTH / 16, HEIGHT / 8);
	string menuCommands[2] = { "Play", "Exit" };

	for (int i = 0; i < menuRect.size(); i++)
	{
		menuRect[i].setSize(menuRectSize);
		menuRect[i].setFillColor(grey);
		menuRect[i].setOrigin(menuRectSize.x / 2, menuRectSize.y / 2);
		menuRect[i].setPosition(WIDTH / 2, 6 * HEIGHT / 16 + i * (5 * HEIGHT / 32));

		menuText[i].setFont(myFont);
		menuText[i].setCharacterSize(2 * title.getCharacterSize() / 5);
		menuText[i].setStyle(sf::Text::Bold);
		menuText[i].setColor(sf::Color::Black);
		menuText[i].setString(menuCommands[i]);
		menuText[i].setOrigin(menuText[i].getLocalBounds().width / 2, menuText[i].getLocalBounds().height / 2);
		menuText[i].setPosition(menuRect[i].getPosition().x, menuRect[i].getPosition().y);
	}

	BILLIARDS.draw(title);
	for (int i = 0; i < menuRect.size(); i++)
	{
		BILLIARDS.draw(menuRect[i]);
		BILLIARDS.draw(menuText[i]);
	}
}

void table()
{
	sf::Vector2f tbBorder(WIDTH, borderDepth);
	sf::Vector2f lrBorder(borderDepth, HEIGHT);
	sf::Vector2f matSize(WIDTH - 2 * borderDepth, HEIGHT - 2 * borderDepth);
	float borderPosX[4] = {0.f, 0.f, 0.f, WIDTH - borderDepth};
	float borderPosY[4] = {0.f, HEIGHT - borderDepth, 0.f, 0.f};
	float pocketPosX[3] = {borderLeft, WIDTH / 2, borderRight};
	float pocketPosY[2] = {borderTop, borderBot};
	
	mat.setSize(matSize);
	mat.setOrigin(matSize.x / 2, matSize.y / 2);
	mat.setPosition(WIDTH/2, HEIGHT/2);
	mat.setFillColor(darkGreen);

	for (int i = 0; i < 4; i++)
	{
		if (i < 2) { borders[i].setSize(tbBorder); }
		else { borders[i].setSize(lrBorder); }
		borders[i].setPosition(borderPosX[i], borderPosY[i]);
		borders[i].setFillColor(brown);
		borders[i].setOutlineColor(sf::Color::Black);
		borders[i].setOutlineThickness(1);
	}

	for (int i = 0; i < 6; i++)
	{
		pockets[i].setRadius(pocketRad);
		pockets[i].setOrigin(pocketRad, pocketRad);
		pockets[i].setFillColor(sf::Color::Black);
		pockets[i].setOutlineColor(sf::Color::Black);
		pockets[i].setOutlineThickness(1);
	}

	int k = 0;
	for (int i = 0; i < 3; i++)
	{ 
		for (int j = 0; j < 2; j++)
		{
			pockets[k].setPosition(pocketPosX[i], pocketPosY[j]);
			k = k + 1;
		}
	}

	BILLIARDS.draw(mat);
	for (int i = 0; i < borders.size(); i++) { BILLIARDS.draw(borders[i]); }
	for (int i = 0; i < pockets.size(); i++) { BILLIARDS.draw(pockets[i]); }
}

void setup()
{
	ballsDraw();
	sf::Vector2f ballBlockPos(3 * WIDTH / 4.f, HEIGHT / 2.f);
	float x = ballBlockPos.x;
	float y = ballBlockPos.y;

	float a = x - 4 * ballRad;
	float b = x - 2 * ballRad;
	float c = x + 2 * ballRad;
	float d = x + 4 * ballRad;

	float ballPosX[9] = { a, b, b, x, x, x, c, c, d };
	a = y - 2 * ballRad;
	b = y - 1 * ballRad;
	c = y + 1 * ballRad;
	d = y + 2 * ballRad;
	float ballPosY[9] = { y, b, c, a, y, d, b, c, y };
	
	for (int i = 0; i < numBalls; i++)
	{
		if (i != 0)
		{
			ball[i].main.setPosition(ballPosX[i - 1], ballPosY[i - 1]);
			ball[i].mark.setPosition(ball[i].main.getPosition());
			ball[i].num.setPosition(ball[i].main.getPosition());
			ball[i].stripe.setPosition(ball[i].main.getPosition());
		}
	}

	ball[0].main.setPosition(WIDTH / 4.f, HEIGHT / 2.f);
}

void ballCollision(int a)
{
	for (int b = 0; b < numBalls; b++)
	{
		if ((a != b) && (!ball[b].isPocketed))
		{
			sf::Vector2f col = ball[a].main.getPosition() - ball[b].main.getPosition();
			float d = sqrt(col.x * col.x + col.y * col.y);
			if (d < 2 * ballRad)
			{
				if (ball[a].collidedWith[0] == NULL)
				{
					ball[a].collidedWith[0] = b;
					ball[b].collidedWith[0] = a;

					col.x = col.x / d;
					col.y = col.y / d;

					float ballavel = ball[a].vx * col.x + ball[a].vy * col.y;
					float ballbvel = ball[b].vx * col.x + ball[b].vy * col.y;
					
					ball[a].vx = (ballavel - (ballavel - ballbvel) * col.x);
					ball[a].vy = (ballavel - (ballavel - ballbvel) * col.y);
					ball[b].vx = (ballbvel - (ballbvel - ballavel) * col.x);
					ball[b].vy = (ballbvel - (ballbvel - ballavel) * col.y);

					ball[a].speed = sqrt(ball[a].vx * ball[a].vx + ball[a].vy * ball[a].vy);
					ball[b].speed = sqrt(ball[b].vx * ball[b].vx + ball[b].vy * ball[b].vy);
					ball[a].angle = atan2f(ball[a].vy, ball[a].vx);
					ball[b].angle = atan2f(ball[b].vy, ball[b].vx);
				}
			}
			else
			{
				ball[a].collidedWith[0] = NULL;
				ball[b].collidedWith[0] = NULL;
			}

		}
	}
}

void wallCollision(int a)
{
	if (ball[a].main.getPosition().x >= borderRight - ballRad)
	{
		ball[a].main.setPosition(borderRight - ballRad, ball[a].main.getPosition().y);
		ball[a].angle = -ball[a].angle + pi;
	}
	if (ball[a].main.getPosition().x <= borderLeft + ballRad)
	{
		ball[a].main.setPosition(borderLeft + ballRad, ball[a].main.getPosition().y);
		ball[a].angle = -ball[a].angle + pi;
	}
	if (ball[a].main.getPosition().y <= borderTop + ballRad)
	{
		ball[a].main.setPosition(ball[a].main.getPosition().x, borderTop + ballRad);
		ball[a].angle = -ball[a].angle;
	}
	if (ball[a].main.getPosition().y >= borderBot - ballRad)
	{
		ball[a].main.setPosition(ball[a].main.getPosition().x, borderBot - ballRad);
		ball[a].angle = -ball[a].angle;
	}
}

void pocketCheck(int i)
{
	for (int j = 0; j < pockets.size(); j++)
	{
		if (sqrt(pow(ball[i].main.getPosition().x - pockets[j].getPosition().x, 2) + pow(ball[i].main.getPosition().y - pockets[j].getPosition().y, 2)) < pocketRad)
		{
			ball[i].isPocketed = true;
			ball[i].speed = 0;
		}
	}
}


///////////////////////////////////////////////////////Main//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{

	BILLIARDS.setVerticalSyncEnabled(true);
	if (!myFont.loadFromFile("orbitron-black.otf")) { cout << "Font can't be loaded." << std::endl; }

	while (!programExit)
	{
		sf::Clock clock;

		while ((BILLIARDS.isOpen()) && (!programExit))
		{
			sf::Event event;

			while (BILLIARDS.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					BILLIARDS.close();
				if (event.mouseButton.button == sf::Mouse::Left && event.type == sf::Event::MouseButtonReleased)
				{
					bool menuCommandActivate[] = { false, false, false };
					for (int i = 0; i < menuRect.size(); i++)
					{
						if (abs(event.mouseButton.x - menuRect[i].getPosition().x) < menuRect[i].getSize().x / 2)
						{
							if (abs(event.mouseButton.y - menuRect[i].getPosition().y) < menuRect[i].getSize().y / 2)
								menuCommandActivate[i] = true;
						}
					}
					if (menuCommandActivate[0])
					{
						onePlayerGame = true;
						gameStart = true;
						setup();
					}
					if (menuCommandActivate[1])
						programExit = true;
				}
			}

			table();
			menu();
			BILLIARDS.display();
			BILLIARDS.clear();

			while (onePlayerGame)
			{
				float gameTime = clock.restart().asMilliseconds();
				while (BILLIARDS.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						BILLIARDS.close();
					if (event.key.code == sf::Keyboard::Space && event.type == event.KeyReleased)
					{
						if (gameStart)
						{
							gameStart = false;
							playerIsAiming = true;
						}
						else if (playerIsAiming)
						{
							ball[0].angle = aimAngle;
							ball[0].speed = aimLength * aimStandardForce;
							playerIsAiming = false;
							gameIsMoving = true;
						}
						else if (playerMovesBall)
						{
							playerMovesBall = false;
							playerIsAiming = true;
						}
						
					}
				}
				if (gameStart)
				{
					ball[0].main.setPosition(sf::Mouse::getPosition(BILLIARDS).x, sf::Mouse::getPosition(BILLIARDS).y);
					if (ball[0].main.getPosition().x > mat.getPosition().x - mat.getSize().x / 4 - ballRad)
						ball[0].main.setPosition(mat.getPosition().x - mat.getSize().x / 4 - ballRad, ball[0].main.getPosition().y);
					if (ball[0].main.getPosition().x < borderLeft + ballRad)
						ball[0].main.setPosition(borderLeft + ballRad, ball[0].main.getPosition().y);
					if (ball[0].main.getPosition().y < borderTop + ballRad)
						ball[0].main.setPosition(ball[0].main.getPosition().x, borderTop + ballRad);
					if (ball[0].main.getPosition().y > borderBot - ballRad)
						ball[0].main.setPosition(ball[0].main.getPosition().x, borderBot - ballRad);

					for (int i = 0; i < 2; i++)
					{
						if ((abs(ball[0].main.getPosition().x - pockets[i].getPosition().x) < ballRad + pocketRad) && (abs(ball[0].main.getPosition().y - pockets[i].getPosition().y) < ballRad + pocketRad))
						{
							ball[0].main.setPosition(pockets[i].getPosition().x + (pocketRad + ballRad) * cos(atan2f(ball[0].main.getPosition().y - pockets[i].getPosition().y, ball[0].main.getPosition().x - pockets[i].getPosition().x)), pockets[i].getPosition().y + (pocketRad + ballRad) * sin(atan2f(ball[0].main.getPosition().y - pockets[i].getPosition().y, ball[0].main.getPosition().x - pockets[i].getPosition().x)));
						}
					}
				}
				
				table();
				ballsDraw();

				if (playerMovesBall)
				{
					ball[0].isPocketed = false;

					ball[0].main.setPosition(sf::Mouse::getPosition(BILLIARDS).x, sf::Mouse::getPosition(BILLIARDS).y);
					if (ball[0].main.getPosition().x > borderRight - ballRad)
						ball[0].main.setPosition(borderRight - ballRad, ball[0].main.getPosition().y);
					if (ball[0].main.getPosition().x < borderLeft + ballRad)
						ball[0].main.setPosition(borderLeft + ballRad, ball[0].main.getPosition().y);
					if (ball[0].main.getPosition().y < borderTop + ballRad)
						ball[0].main.setPosition(ball[0].main.getPosition().x, borderTop + ballRad);
					if (ball[0].main.getPosition().y > borderBot - ballRad)
						ball[0].main.setPosition(ball[0].main.getPosition().x, borderBot - ballRad);

					for (int i = 0; i < pockets.size(); i++)
					{
						if ((abs(ball[0].main.getPosition().x - pockets[i].getPosition().x) < ballRad + pocketRad) && (abs(ball[0].main.getPosition().y - pockets[i].getPosition().y) < ballRad + pocketRad))
						{
							ball[0].main.setPosition(pockets[i].getPosition().x + (pocketRad + ballRad) * cos(atan2f(ball[0].main.getPosition().y - pockets[i].getPosition().y, ball[0].main.getPosition().x - pockets[i].getPosition().x)), pockets[i].getPosition().y + (pocketRad + ballRad) * sin(atan2f(ball[0].main.getPosition().y - pockets[i].getPosition().y, ball[0].main.getPosition().x - pockets[i].getPosition().x)));
						}
					}

					for (int i = 1; i < numBalls; i++)
					{
						if ((abs(ball[0].main.getPosition().x - ball[i].main.getPosition().x) < ballRad * 2) && (abs(ball[0].main.getPosition().y - ball[i].main.getPosition().y) < ballRad * 2))
						{
							ball[0].main.setPosition(ball[i].main.getPosition().x + (2 * ballRad) * cos(atan2f(ball[0].main.getPosition().y - ball[i].main.getPosition().y, ball[0].main.getPosition().x - ball[i].main.getPosition().x)), ball[i].main.getPosition().y + (2 * ballRad) * sin(atan2f(ball[0].main.getPosition().y - ball[i].main.getPosition().y, ball[0].main.getPosition().x - ball[i].main.getPosition().x)));
						}
					}
				}

				if (playerIsAiming)
				{
					aimAngle = atan2f(ball[0].main.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y, ball[0].main.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x);
					aimLength = sqrt((ball[0].main.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x) * (ball[0].main.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x) + (ball[0].main.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y) * (ball[0].main.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y));

					if (aimLength > 100) { aimLength = 100; }

					sf::Vector2f aimOrigin(ball[0].main.getPosition());
					sf::Vector2f aimXY(aimLength * cos(aimAngle), aimLength  * sin(aimAngle));
					sf::Vector2f arrwXY1(3 * aimLength / 10 * cos(aimAngle - 5 * pi / 6), 3 * aimLength / 10 * sin(aimAngle - 5 * pi / 6));
					sf::Vector2f arrwXY2(3 * aimLength / 10 * cos(aimAngle - 7 * pi / 6), 3 * aimLength / 10 * sin(aimAngle - 7 * pi / 6));


					sf::Vertex aim[] =
					{
						sf::Vertex(aimOrigin),
						sf::Vertex(aimOrigin + aimXY)
					};

					sf::Vertex arrw1[] =
					{
						sf::Vertex(aimOrigin + aimXY),
						sf::Vertex(aimOrigin + aimXY + arrwXY1)
					};

					sf::Vertex arrw2[] =
					{
						sf::Vertex(aimOrigin + aimXY),
						sf::Vertex(aimOrigin + aimXY + arrwXY2)
					};

					BILLIARDS.draw(aim, 2, sf::Lines);
					BILLIARDS.draw(arrw1, 2, sf::Lines);
					BILLIARDS.draw(arrw2, 2, sf::Lines);

				}

				if (gameIsMoving)
				{	
					for (int i = 0; i < numBalls; i++)
					{
						if (!ball[i].isPocketed)
						{
							if (ball[i].speed <= 0)
								ball[i].speed = 0;
							else
							{
								ballCollision(i);
							}
							wallCollision(i);
							pocketCheck(i);
						}
					}

					for (int i = 0; i < numBalls; i++)
					{
						ball[i].vx = ball[i].speed * cos(ball[i].angle);
						ball[i].vy = ball[i].speed * sin(ball[i].angle);
						ball[i].main.setPosition(ball[i].main.getPosition().x + ball[i].vx * gameTime, ball[i].main.getPosition().y + ball[i].vy * gameTime);
						ball[i].speed = ball[i].speed - frictionForce;

						if (i != 0)
						{
							ball[i].mark.setPosition(ball[i].main.getPosition());
							ball[i].num.setPosition(ball[i].main.getPosition());
							ball[i].stripe.setPosition(ball[i].main.getPosition());
						}
					}
					bool noBallMoving = true;

					for (int i = 0; i < numBalls; i++)
					{
						if (ball[i].speed > 0)
						{
							noBallMoving = false;
						}
					}

					if (noBallMoving)
					{
						gameIsMoving = false;
						bool allIsPocketed = true;
						for (int i = 1; i < numBalls; i++)
						{
							if (!ball[i].isPocketed)
								allIsPocketed = false;
						}
						if (allIsPocketed)
						{
							if (ball[0].isPocketed)
							{
								ball[numBalls - 1].isPocketed = false;
								ball[numBalls - 1].main.setPosition(3 * WIDTH / 4.f, HEIGHT / 2.f);
								if (numBalls == 10)
									ball[numBalls - 1].stripe.setPosition(3 * WIDTH / 4.f, HEIGHT / 2.f);
								ball[numBalls - 1].mark.setPosition(3 * WIDTH / 4.f, HEIGHT / 2.f);
								ball[numBalls - 1].num.setPosition(3 * WIDTH / 4.f, HEIGHT / 2.f);
								playerMovesBall = true;
							}
							else
							{
								onePlayerGame = false;
								for (int i = 0; i < numBalls; i++)
								{
									ball[i].isPocketed = false;
									ball[i].speed = 0;
								}
							}
						}
						else
						{
							if (ball[0].isPocketed)
								playerMovesBall = true;
							else
								playerIsAiming = true;
						}
					}
				}
				BILLIARDS.display();
				BILLIARDS.clear();
			}
		}
		if (programExit)
		{
			return 0;
		}
	}

}