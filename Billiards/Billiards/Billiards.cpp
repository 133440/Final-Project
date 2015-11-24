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
float borderDepth = WIDTH / 25;
float pocketRad = borderDepth * .5f;
float ballRad = pocketRad * .65f;
float aimLength = 0;
float aimAngle = 0;
float wBallSpeed = 0;

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
vector<sf::CircleShape> balls(9);
vector<sf::CircleShape> ballMarks(9);
vector<sf::Text> ballNums(9);
sf::CircleShape whiteBall;
sf::RectangleShape stripe9;
sf::Vertex aim[];


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

		ballMarks[i].setRadius(ballRad * .5);
		ballMarks[i].setOrigin(ballRad *.5, ballRad*.5);

		string ballNum = to_string(i + 1);
		ballNums[i].setFont(myFont);
		ballNums[i].setCharacterSize(6);
		ballNums[i].setStyle(sf::Text::Regular);
		ballNums[i].setColor(sf::Color::Black);
		ballNums[i].setString(ballNum);
		ballNums[i].setOrigin(ballNums[i].getCharacterSize() / 2, ballNums[i].getCharacterSize() / 2);
	}

	balls[0].setFillColor(sf::Color::Yellow);
	balls[1].setFillColor(sf::Color::Blue);
	balls[2].setFillColor(sf::Color::Red);
	balls[3].setFillColor(indigo);
	balls[4].setFillColor(orange);
	balls[5].setFillColor(green);
	balls[6].setFillColor(maroon);
	balls[7].setFillColor(sf::Color::Black);

	sf::Vector2f stripeSize(1.8 * ballRad, ballRad);
	stripe9.setFillColor(sf::Color::Yellow);
	stripe9.setSize(stripeSize);
	stripe9.setOrigin(stripeSize.x/2, stripeSize.y / 2);

	whiteBall.setRadius(ballRad);
	whiteBall.setOrigin(ballRad, ballRad);
	whiteBall.setOutlineThickness(1);
	whiteBall.setOutlineColor(sf::Color::Black);
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
	for (int i = 0; i < balls.size(); i++)
	{
		balls[i].setPosition(ballPosX[i], ballPosY[i]);
		ballMarks[i].setPosition(balls[i].getPosition());
		ballNums[i].setPosition(balls[i].getPosition());
	}
	stripe9.setPosition(balls[8].getPosition());
	whiteBall.setPosition(WIDTH / 4.f, HEIGHT / 2.f);

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
			if (i == 8)
			{
				BILLIARDS.draw(stripe9);
				BILLIARDS.draw(whiteBall);
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
			int gameTime = clock.restart().asMilliseconds();

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
							wBallSpeed = aimLength / 5;
							playerIsAiming = false;
							ballIsMoving = true;
						}
					}
				}
				if (gameStart)
				{
					whiteBall.setPosition(sf::Mouse::getPosition(BILLIARDS).x, sf::Mouse::getPosition(BILLIARDS).y);
					if (whiteBall.getPosition().x > mat.getPosition().x - mat.getSize().x / 4 - ballRad)
						whiteBall.setPosition(mat.getPosition().x - mat.getSize().x / 4 - ballRad, whiteBall.getPosition().y);
					if (whiteBall.getPosition().x < borderLeft + ballRad)
						whiteBall.setPosition(borderLeft + ballRad, whiteBall.getPosition().y);
					if (whiteBall.getPosition().y < borderTop + ballRad)
						whiteBall.setPosition(whiteBall.getPosition().x, borderTop + ballRad);
					if (whiteBall.getPosition().y > borderBot - ballRad)
						whiteBall.setPosition(whiteBall.getPosition().x, borderBot - ballRad);

					for (int i = 0; i < 2; i++)
					{
						if ((abs(whiteBall.getPosition().x - pockets[i].getPosition().x) < ballRad + pocketRad) && (abs(whiteBall.getPosition().y - pockets[i].getPosition().y) < ballRad + pocketRad))
						{
							whiteBall.setPosition(pockets[i].getPosition().x + (pocketRad + ballRad) * cos(atan2f(whiteBall.getPosition().y - pockets[i].getPosition().y, whiteBall.getPosition().x - pockets[i].getPosition().x)), pockets[i].getPosition().y + (pocketRad + ballRad) * sin(atan2f(whiteBall.getPosition().y - pockets[i].getPosition().y, whiteBall.getPosition().x - pockets[i].getPosition().x)));
						}
					}
				}

				BILLIARDS.draw(mat);
				for (int i = 0; i < borders.size(); i++) { BILLIARDS.draw(borders[i]); }
				for (int i = 0; i < pockets.size(); i++) { BILLIARDS.draw(pockets[i]); }
				for (int i = 0; i < balls.size(); i++) {
					BILLIARDS.draw(balls[i]);
					if (i == 8)
					{
						BILLIARDS.draw(stripe9);
						BILLIARDS.draw(whiteBall);
					}
					BILLIARDS.draw(ballMarks[i]);
					BILLIARDS.draw(ballNums[i]);
				}


				if (playerIsAiming)
				{
					aimAngle = atan2f(whiteBall.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y, whiteBall.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x);
					aimLength = sqrt((whiteBall.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x) *(whiteBall.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x) + (whiteBall.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y) * (whiteBall.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y));

					if (aimLength > 100) { aimLength = 100; }

					sf::Vector2f aimOrigin(whiteBall.getPosition());
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
					float wBallVelocityX = wBallSpeed * cos(aimAngle);
					float wBallVelocityY = wBallSpeed * sin(aimAngle);
					whiteBall.move(wBallVelocityX, wBallVelocityY);
					wBallSpeed = wBallSpeed - 0.1;
					if (wBallSpeed < 0)
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

