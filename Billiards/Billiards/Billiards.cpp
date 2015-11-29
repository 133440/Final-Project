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
};

Balls::Balls()
{
};

//Dimensions
const float pi = 3.14159f;
const int numBalls = 10;
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
float frictionForce = .001f * gameSpeed;

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
Balls ball[numBalls] = { a, b, c, d, e, f, g, h, i, j };


//TitleMenu
sf::Text title;
vector<sf::RectangleShape> menuRect(3);
vector<sf::Text> menuText(3);

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
	string menuCommands[3] = { "1 Player", "2 Player", "Exit" };

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

void startup()
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

			BILLIARDS.draw(ball[i].main);
			if (i == 9)
			{
				BILLIARDS.draw(ball[i].stripe);
			}
			BILLIARDS.draw(ball[i].mark);
			BILLIARDS.draw(ball[i].num);
		}
	}
	ball[0].main.setPosition(WIDTH / 4.f, HEIGHT / 2.f);
	BILLIARDS.draw(ball[0].main);
}

void ballCollision(int a, int b)
{
	
	float xd = ball[a].main.getPosition().x - ball[b].main.getPosition().x;
	float yd = ball[a].main.getPosition().y - ball[b].main.getPosition().y;
	float sqrRad = (2 * ballRad) * (2 * ballRad);
	float sqrD = (xd*xd) + (yd*yd);
	if (sqrD < sqrRad)
	{
		
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


///////////////////////////////////////////////////////Main//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{

	BILLIARDS.setVerticalSyncEnabled(true);
	if (!myFont.loadFromFile("orbitron-black.otf")) { cout << "Font can't be loaded." << std::endl; }

	while (!programExit)
	{
		table();
		startup();
		menu();
		BILLIARDS.display();

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
					}
					if (menuCommandActivate[2])
						programExit = true;
				}
			}

			while (onePlayerGame)
			{
				int gameTime = clock.restart().asMilliseconds();
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
							ball[0].speed = aimLength * .001f * 1.5f * gameSpeed;
							playerIsAiming = false;
							gameIsMoving = true;
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

				BILLIARDS.draw(mat);
				for (int i = 0; i < borders.size(); i++) { BILLIARDS.draw(borders[i]); }
				for (int i = 0; i < pockets.size(); i++) { BILLIARDS.draw(pockets[i]); }
				for (int i = 0; i < numBalls; i++)
				{
					BILLIARDS.draw(ball[i].main);
					if (i == 9)
					{
						BILLIARDS.draw(ball[i].stripe);
					}
					BILLIARDS.draw(ball[i].mark);
					BILLIARDS.draw(ball[i].num);
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
					ball[0].vx = ball[0].speed * cos(ball[0].angle);
					ball[0].vy = ball[0].speed * sin(ball[0].angle);
					ball[0].main.setPosition(ball[0].main.getPosition().x + ball[0].vx * gameTime, ball[0].main.getPosition().y + ball[0].vy * gameTime);
					ball[0].speed = ball[0].speed - frictionForce;
					
					for (int i = 0; i < numBalls; i++)
					{
						if (ball[i].speed < 0)
							ball[i].speed = 0;
						for (int j = 0; j < 11; j++)
						{
							if (i != j)
								ballCollision(i, j);
						}
						wallCollision(i);
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
						playerIsAiming = true;
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