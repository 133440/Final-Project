#include "stdafx.h" //remove before submitting
#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/graphics.hpp>
#include <cstdlib>
#include <cmath>

using namespace std;

//Dimensions
const float pi = 3.14159f;
float WIDTH = 800.f;
float HEIGHT = WIDTH / 2;
float FPS = 30;
float gameSpeed = 10;
float SPF = 1 / FPS;
float borderDepth = WIDTH / 25;
float pocketRad = borderDepth * .5f;
float ballRad = pocketRad * .65f;
float aimLength = 0;
float aimAngle = 0;
float frictionForce = .001 * gameSpeed;

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
bool ballIsMoving = false;
bool programExit = false;
bool gameQuit = false;

//Objects
sf::RenderWindow BILLIARDS(sf::VideoMode(WIDTH, HEIGHT, 64), "Billiards", sf::Style::Titlebar | sf::Style::Close);
sf::Font myFont;
sf::RectangleShape mat;
vector<sf::CircleShape> pockets(6);
vector<sf::RectangleShape> borders(4);
sf::Vertex aim[];


vector<sf::CircleShape> balls(10);
vector<sf::CircleShape> ballMarks(10);
vector<sf::Text> ballNums(10);
sf::RectangleShape stripe9;
float ballSpeed[10];
float ballAngle[10];



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

void titleMenuDraw()
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
}

void tableDraw()
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
}

void ballsDraw()
{
	for (int i = 0; i < balls.size(); i++)
	{
		balls[i].setRadius(ballRad);
		balls[i].setOrigin(ballRad, ballRad);
		balls[i].setOutlineThickness(1);
		balls[i].setOutlineColor(sf::Color::Black);

		if (i != 0)
		{
			ballMarks[i].setRadius(ballRad * .5);
			ballMarks[i].setOrigin(ballRad *.5, ballRad*.5);
			string ballNum = to_string(i);
			ballNums[i].setFont(myFont);
			ballNums[i].setCharacterSize(6);
			ballNums[i].setStyle(sf::Text::Regular);
			ballNums[i].setColor(sf::Color::Black);
			ballNums[i].setString(ballNum);
			ballNums[i].setOrigin(ballNums[i].getCharacterSize() / 2, ballNums[i].getCharacterSize() / 2);
		}
	}

	balls[1].setFillColor(sf::Color::Yellow);
	balls[2].setFillColor(sf::Color::Blue);
	balls[3].setFillColor(sf::Color::Red);
	balls[4].setFillColor(indigo);
	balls[5].setFillColor(orange);
	balls[6].setFillColor(green);
	balls[7].setFillColor(maroon);
	balls[8].setFillColor(sf::Color::Black);

	sf::Vector2f stripeSize(1.8 * ballRad, ballRad);
	stripe9.setFillColor(sf::Color::Yellow);
	stripe9.setSize(stripeSize);
	stripe9.setOrigin(stripeSize.x/2, stripeSize.y / 2);
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
	for (int i = 1; i < balls.size(); i++)
	{
		balls[i].setPosition(ballPosX[i - 1], ballPosY[i - 1]);
		ballMarks[i].setPosition(balls[i].getPosition());
		ballNums[i].setPosition(balls[i].getPosition());
	}
	stripe9.setPosition(balls[9].getPosition());
	balls[0].setPosition(WIDTH / 4.f, HEIGHT / 2.f);

}

int main()
{

	BILLIARDS.setVerticalSyncEnabled(true);
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	if (!myFont.loadFromFile("orbitron-black.otf")) { cout << "Font can't be loaded." << std::endl; }

	while (!programExit)
	{
		//DRAW TABLE
		tableDraw();
		BILLIARDS.draw(mat);
		for (int i = 0; i < borders.size(); i++) { BILLIARDS.draw(borders[i]); }
		for (int i = 0; i < pockets.size(); i++) { BILLIARDS.draw(pockets[i]); }

		//STARTUP
		startup();
		for (int i = 0; i < balls.size(); i++) {
			BILLIARDS.draw(balls[i]);
			if (i == 9)
			{
				BILLIARDS.draw(stripe9);
			}
			BILLIARDS.draw(ballMarks[i]);
			BILLIARDS.draw(ballNums[i]);
		}

		//TITLE MENU
		titleMenuDraw();
		BILLIARDS.draw(title);
		for (int i = 0; i < menuRect.size(); i++)
		{
			BILLIARDS.draw(menuRect[i]);
			BILLIARDS.draw(menuText[i]);
		}
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
							ballAngle[0] = aimAngle;
							ballSpeed[0] = aimLength * .001 * 1.5 * gameSpeed;
							playerIsAiming = false;
							ballIsMoving = true;
						}
					}
				}
				if (gameStart)
				{
					balls[0].setPosition(sf::Mouse::getPosition(BILLIARDS).x, sf::Mouse::getPosition(BILLIARDS).y);
					if (balls[0].getPosition().x > mat.getPosition().x - mat.getSize().x / 4 - ballRad)
						balls[0].setPosition(mat.getPosition().x - mat.getSize().x / 4 - ballRad, balls[0].getPosition().y);
					if (balls[0].getPosition().x < borderLeft + ballRad)
						balls[0].setPosition(borderLeft + ballRad, balls[0].getPosition().y);
					if (balls[0].getPosition().y < borderTop + ballRad)
						balls[0].setPosition(balls[0].getPosition().x, borderTop + ballRad);
					if (balls[0].getPosition().y > borderBot - ballRad)
						balls[0].setPosition(balls[0].getPosition().x, borderBot - ballRad);

					for (int i = 0; i < 2; i++)
					{
						if ((abs(balls[0].getPosition().x - pockets[i].getPosition().x) < ballRad + pocketRad) && (abs(balls[0].getPosition().y - pockets[i].getPosition().y) < ballRad + pocketRad))
						{
							balls[0].setPosition(pockets[i].getPosition().x + (pocketRad + ballRad) * cos(atan2f(balls[0].getPosition().y - pockets[i].getPosition().y, balls[0].getPosition().x - pockets[i].getPosition().x)), pockets[i].getPosition().y + (pocketRad + ballRad) * sin(atan2f(balls[0].getPosition().y - pockets[i].getPosition().y, balls[0].getPosition().x - pockets[i].getPosition().x)));
						}
					}
				}

				BILLIARDS.draw(mat);
				for (int i = 0; i < borders.size(); i++) { BILLIARDS.draw(borders[i]); }
				for (int i = 0; i < pockets.size(); i++) { BILLIARDS.draw(pockets[i]); }
				for (int i = 0; i < balls.size(); i++) {
					BILLIARDS.draw(balls[i]);
					if (i == 9)
					{
						BILLIARDS.draw(stripe9);
					}
					BILLIARDS.draw(ballMarks[i]);
					BILLIARDS.draw(ballNums[i]);
				}


				if (playerIsAiming)
				{
					aimAngle = atan2f(balls[0].getPosition().y - sf::Mouse::getPosition(BILLIARDS).y, balls[0].getPosition().x - sf::Mouse::getPosition(BILLIARDS).x);
					aimLength = sqrt((balls[0].getPosition().x - sf::Mouse::getPosition(BILLIARDS).x) *(balls[0].getPosition().x - sf::Mouse::getPosition(BILLIARDS).x) + (balls[0].getPosition().y - sf::Mouse::getPosition(BILLIARDS).y) * (balls[0].getPosition().y - sf::Mouse::getPosition(BILLIARDS).y));

					if (aimLength > 100) { aimLength = 100; }

					sf::Vector2f aimOrigin(balls[0].getPosition());
					sf::Vector2f aimXY(aimLength * cos(aimAngle), aimLength  * sin(aimAngle));
					sf::Vector2f arrwXY1(3 * aimLength / 10  * cos(aimAngle - 5 * pi /6), 3 * aimLength / 10 * sin(aimAngle - 5 * pi /6));
					sf::Vector2f arrwXY2 (3 * aimLength / 10  * cos(aimAngle - 7 * pi / 6), 3 * aimLength / 10 * sin(aimAngle - 7 * pi / 6));


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

				if (ballIsMoving)
				{
					float wBallVelocityX = ballSpeed[0] * cos(ballAngle[0]);
					float wBallVelocityY = ballSpeed[0] * sin(ballAngle[0]);
					balls[0].setPosition(balls[0].getPosition().x + wBallVelocityX * gameTime, balls[0].getPosition().y + wBallVelocityY * gameTime);
					ballSpeed[0] = ballSpeed[0] - frictionForce;
					if (ballSpeed[0] < 0)
					{
						ballIsMoving = false;
						playerIsAiming = true;
					}
				}
				BILLIARDS.display();
				BILLIARDS.clear();
			}

		}
	}
	if (programExit)
	{
		return 0;
	}
}

