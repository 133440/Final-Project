#include "stdafx.h" //remove before submitting
#include <iostream>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/graphics.hpp>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

class Balls
{
public:
	Balls();
	sf::CircleShape main;
	sf::CircleShape mark;
	sf::RectangleShape stripe;
	sf::Text num;
	sf::Vector2f velocity;
	bool isPocketed;
};

Balls::Balls()
{
};

//Dimensions
const float pi = 3.14159f;
int numBalls = 10;
int WIDTH = 800;
int HEIGHT = WIDTH / 2;
float FPS = 30;
float gameSpeed = 10;
float SPF = 1 / FPS;
const sf::Vector2f zero(0, 0);

float borderDepth = WIDTH / 25;
float pocketRad = borderDepth * .5f;
sf::Vector2f pocket2Size(pocketRad * 2, pocketRad);
float ballRad = pocketRad * .65f;
float aimLength = 0;
float aimMax = 100;
float aimAngle = 0;
float aimStandardForce = .8 * .001f * gameSpeed;
float frictionForce = .5 * .001f * gameSpeed;
int firstHit;
int ballToHit;
int mover;
int BS;

float borderTop = borderDepth;
float borderBot = HEIGHT - borderDepth;
float borderLeft = borderDepth;
float borderRight = WIDTH - borderDepth;


//Booleans
bool menuShow = true;
bool onePlayerGame = false;
bool twoPlayerGame = false;
bool gameStart = false;
bool playerIsAiming = false;
bool playerMovesBall = false;
bool gameIsMoving = false;
bool programExit = false;
bool gameQuit = false;
bool justGotPocketed = false;
bool pauseGame = false;
bool pause1P = false;
bool pause2P = false;
bool noBallMoving = false;
bool endofGame = false;
bool end1P = false;
bool end2P = false;

//Objects
sf::RenderWindow BILLIARDS(sf::VideoMode(WIDTH, HEIGHT, 64), "Billiards", sf::Style::Titlebar | sf::Style::Close);
sf::Font myFont;
sf::RectangleShape mat;
vector<sf::CircleShape> pockets(6);
vector<sf::RectangleShape> pockets2(6);
vector<sf::RectangleShape> borders(4);
sf::Vertex aim[];
Balls a, b, c, d, e, f, g, h, i, j;
Balls ball[10] = { a, b, c, d, e, f, g, h, i, j };

sf::Text moves;
sf::Text bestScore;

//TitleMenu
sf::Text title;
vector<sf::RectangleShape> menuRect(3);
vector<sf::Text> menuText(3);
string menuCommands[3] = { "One Player", "Two Player", "Exit" };

//PauseMenu
sf::Text pause;
vector<sf::RectangleShape> pauseMenuRect(2);
vector<sf::Text> pauseMenuText(2);
string pauseMenuCommands[2] = { "Resume", "Quit" };

//gameEnd
sf::Text endTitle;
vector<sf::RectangleShape> endMenuRect(1);
vector<sf::Text> endMenuText(1);
string endMenuCommands[1] = { "Main Menu"};


//Colors
sf::Color brown(54, 18, 8);
sf::Color darkGreen(0, 50, 0);
sf::Color green(0, 80, 0);
sf::Color indigo(75, 0, 130);
sf::Color orange(255, 128, 0);
sf::Color maroon(128, 0, 0);
sf::Color grey(125, 125, 125);

float dot(sf::Vector2f a, sf::Vector2f b)
{
	float c = a.x * b.x + a.y * b.y;
	return c;
}

sf::Vector2f proj(sf::Vector2f a, sf::Vector2f b)
{
	sf::Vector2f c = (dot(b, a) / dot(a, a)) * a;
	return c;
}

float mag(sf::Vector2f a)
{
	float b = sqrt(pow(a.x, 2) + pow(a.y, 2));
	return b;
}

sf::Vector2f normal(sf::Vector2f a)
{
	sf::Vector2f b = a / mag(a);
	return b;
}

void MoveDraw()
{
	moves.setFont(myFont);
	moves.setCharacterSize(12);
	moves.setStyle(sf::Text::Bold);
	moves.setColor(sf::Color::Red);
	moves.setOrigin(moves.getLocalBounds().width / 2, moves.getLocalBounds().height / 2);
	moves.setPosition(WIDTH * 3 / 4, borderDepth / 2);
	bestScore.setFont(myFont);
	bestScore.setCharacterSize(12);
	bestScore.setStyle(sf::Text::Bold);
	bestScore.setColor(sf::Color::Red);
	bestScore.setOrigin(bestScore.getLocalBounds().width / 2, bestScore.getLocalBounds().height / 2);
	bestScore.setPosition(WIDTH * 1 / 4, borderDepth / 2);

	stringstream mess2;
	if (BS != 0)
		mess2 << "Best Score: " << BS;
	else
		mess2 << "Best Score: --";
	string message2 = mess2.str();
	bestScore.setString(message2);

	if ((twoPlayerGame) || (pause2P))
	{
		if (mover > 2)
			mover = 1;
		stringstream mess;
		mess << "Player " << mover << "'s turn";
		string message = mess.str();
		moves.setString(message);
		BILLIARDS.draw(moves);
	}
	else
	{
		stringstream mess;
		mess << "Moves: " << mover;
		string message = mess.str();
		moves.setString(message);
		BILLIARDS.draw(bestScore);
		if ((onePlayerGame)||(pause1P))
			BILLIARDS.draw(moves);
	}
}

void ballsDraw()
{
	for (int i = 0; i < numBalls; i++)
	{
		ball[i].main.setRadius(ballRad - ballRad /10.4);
		ball[i].main.setOrigin(ballRad - ballRad /10.4, ballRad - ballRad / 10.4);
		ball[i].main.setOutlineThickness(ballRad / 10.4);
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
				sf::Vector2f stripeSize(1.5f * ballRad, ballRad);
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

void titleMenu()
{
	title.setFont(myFont);
	title.setCharacterSize(WIDTH / 16);
	title.setStyle(sf::Text::Bold);
	title.setColor(sf::Color::Black);
	title.setString("BILLIARDS");
	title.setOrigin(title.getLocalBounds().width / 2, title.getLocalBounds().height / 2);	
	title.setPosition(WIDTH / 2, 3 * HEIGHT / 16);

	sf::Vector2f menuRectSize(3 * WIDTH / 16, HEIGHT / 8);

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

void pauseMenu()
{
	pause.setFont(myFont);
	pause.setCharacterSize(WIDTH / 16);
	pause.setStyle(sf::Text::Bold);
	pause.setColor(sf::Color::Black);
	pause.setString("PAUSE");
	pause.setOrigin(pause.getLocalBounds().width / 2, pause.getLocalBounds().height / 2);
	pause.setPosition(WIDTH / 2, 3 * HEIGHT / 16);

	sf::Vector2f menuRectSize(3 * WIDTH / 16, HEIGHT / 8);

	for (int i = 0; i < pauseMenuRect.size(); i++)
	{
		pauseMenuRect[i].setSize(menuRectSize);
		pauseMenuRect[i].setFillColor(grey);
		pauseMenuRect[i].setOrigin(menuRectSize.x / 2, menuRectSize.y / 2);
		pauseMenuRect[i].setPosition(WIDTH / 2, 6 * HEIGHT / 16 + i * (5 * HEIGHT / 32));

		pauseMenuText[i].setFont(myFont);
		pauseMenuText[i].setCharacterSize(2 * pause.getCharacterSize() / 5);
		pauseMenuText[i].setStyle(sf::Text::Bold);
		pauseMenuText[i].setColor(sf::Color::Black);
		pauseMenuText[i].setString(pauseMenuCommands[i]);
		pauseMenuText[i].setOrigin(pauseMenuText[i].getLocalBounds().width / 2, pauseMenuText[i].getLocalBounds().height / 2);
		pauseMenuText[i].setPosition(pauseMenuRect[i].getPosition().x, pauseMenuRect[i].getPosition().y);
	}

	BILLIARDS.draw(pause);
	for (int i = 0; i < pauseMenuRect.size(); i++)
	{
		BILLIARDS.draw(pauseMenuRect[i]);
		BILLIARDS.draw(pauseMenuText[i]);
	}
}

void endGame()
{
	endTitle.setFont(myFont);
	endTitle.setCharacterSize(WIDTH / 16);
	endTitle.setStyle(sf::Text::Bold);
	endTitle.setColor(sf::Color::Black);
	string str;
	if (end1P)
	{
		stringstream message;
		message << "Final score: " << mover;
		str = message.str();
		
	}
	else if (end2P)
	{
		stringstream message;
		message << "Player " << mover << " wins!";
		str = message.str();
	}
	endTitle.setString(str);
	endTitle.setOrigin(endTitle.getLocalBounds().width / 2, endTitle.getLocalBounds().height / 2);
	endTitle.setPosition(WIDTH / 2, 2 * HEIGHT / 5);

	sf::Vector2f menuRectSize(3 * WIDTH / 16, HEIGHT / 8);

	for (int i = 0; i < endMenuRect.size(); i++)
	{
		endMenuRect[i].setSize(menuRectSize);
		endMenuRect[i].setFillColor(grey);
		endMenuRect[i].setOrigin(menuRectSize.x / 2, menuRectSize.y / 2);
		endMenuRect[i].setPosition(WIDTH / 2, 4 * HEIGHT / 5);

		endMenuText[i].setFont(myFont);
		endMenuText[i].setCharacterSize(2 * pause.getCharacterSize() / 5);
		endMenuText[i].setStyle(sf::Text::Bold);
		endMenuText[i].setColor(sf::Color::Black);
		endMenuText[i].setString(endMenuCommands[i]);
		endMenuText[i].setOrigin(endMenuText[i].getLocalBounds().width / 2, endMenuText[i].getLocalBounds().height / 2);
		endMenuText[i].setPosition(endMenuRect[i].getPosition().x, endMenuRect[i].getPosition().y);
	}

	BILLIARDS.draw(endTitle);
	for (int i = 0; i < endMenuRect.size(); i++)
	{
		BILLIARDS.draw(endMenuRect[i]);
		BILLIARDS.draw(endMenuText[i]);
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
	float pocketPosY[2] = {borderTop - 5, borderBot + 5};
	float pocket2OrigX[6] = { pocketRad, pocketRad, pocketRad, pocketRad, pocketRad, pocketRad};
	float pocket2OrigY[6] = { 0, pocketRad, 0, pocketRad, 0, pocketRad };
	float pocket2Angle[6] = { -45, 45, 0, 0, 45, -45 };
	
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
		if ((i != 2) && (i != 3))
		{
			pockets2[i].setSize(pocket2Size);
			pockets2[i].setOrigin(pocket2OrigX[i], pocket2OrigY[i]);
			pockets2[i].setFillColor(sf::Color::Black);
			pockets2[i].setRotation(pocket2Angle[i]);
		}
	}

	pockets[0].setPosition(borderLeft, borderTop);
	pockets[1].setPosition(borderLeft, borderBot);
	pockets[2].setPosition(WIDTH / 2, borderTop);
	pockets[3].setPosition(WIDTH / 2, borderBot);
	pockets[4].setPosition(borderRight, borderTop);
	pockets[5].setPosition(borderRight, borderBot);

	for (int i = 0; i < pockets2.size(); i++)
		pockets2[i].setPosition(pockets[i].getPosition());

	BILLIARDS.draw(mat);
	for (int i = 0; i < borders.size(); i++) { BILLIARDS.draw(borders[i]); }
	for (int i = 0; i < pockets.size(); i++) 
	{ 
		BILLIARDS.draw(pockets[i]);
		BILLIARDS.draw(pockets2[i]);
	}

	MoveDraw();
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
			sf::Vector2f col = ball[b].main.getPosition() - ball[a].main.getPosition();
			if (mag(col) < 2 * ballRad)
			{
				if ((firstHit == -1) && (a == 0))
					firstHit = b;

				sf::Vector2f ballavel(ball[a].velocity);
				sf::Vector2f ballbvel(ball[b].velocity);

				sf::Vector2f initializeDistance((2 * ballRad) * normal(col).x , (2 * ballRad ) * normal(col).y);
				ball[a].main.setPosition(ball[b].main.getPosition() - (initializeDistance));
				ball[a].velocity = zero;
				ball[b].velocity = zero;

				col = ball[b].main.getPosition() - ball[a].main.getPosition();

				sf::Vector2f aPar = proj(col, ballavel);
				sf::Vector2f bPar = proj(col, ballbvel);
				sf::Vector2f aPerp = ballavel - aPar;
				sf::Vector2f bPerp = ballbvel - bPar;

				ball[a].velocity = bPar + aPerp;
				ball[b].velocity = aPar + bPerp;
			}
		}
	}
}

void wallCollision(int a)
{
	if (ball[a].main.getPosition().x >= borderRight - ballRad)
	{
		ball[a].main.setPosition(borderRight - ballRad, ball[a].main.getPosition().y);
		ball[a].velocity.x = -ball[a].velocity.x;
	}
	if (ball[a].main.getPosition().x <= borderLeft + ballRad)
	{
		ball[a].main.setPosition(borderLeft + ballRad, ball[a].main.getPosition().y);
		ball[a].velocity.x = -ball[a].velocity.x;
	}
	if (ball[a].main.getPosition().y <= borderTop + ballRad)
	{
		ball[a].main.setPosition(ball[a].main.getPosition().x, borderTop + ballRad);
		ball[a].velocity.y = -ball[a].velocity.y;
	}
	if (ball[a].main.getPosition().y >= borderBot - ballRad)
	{
		ball[a].main.setPosition(ball[a].main.getPosition().x, borderBot - ballRad);
		ball[a].velocity.y = -ball[a].velocity.y;
	}
}

void pocketCheck(int i)
{
	for (int j = 0; j < pockets.size(); j++)
	{
		if ((!ball[i].isPocketed) && (sqrt(pow(ball[i].main.getPosition().x - pockets[j].getPosition().x, 2) + pow(ball[i].main.getPosition().y - pockets[j].getPosition().y, 2)) < 
			pocketRad * 1))
		{
			justGotPocketed = true;
			ball[i].isPocketed = true;
			ball[i].velocity = zero;
		}
	}
}


///////////////////////////////////////////////////////Main//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	string s;
	ifstream fs;
	fs.open("BS.txt");
	if (fs.is_open())
	{
		fs >> s;
	}
	fs.close();
	BS = stoi(s);

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
				{
					stringstream s;
					ofstream fs;
					fs.open("BS.txt");
					fs << BS;
					fs.close();

					BILLIARDS.close();
					return 0;
				}
				if (event.mouseButton.button == sf::Mouse::Left && event.type == sf::Event::MouseButtonReleased)
				{
					bool menuCommandActivate[] = { false, false, false };
					if (pauseGame)
					{
						bool menuCommandActivate[] = { false, false, false };
						for (int i = 0; i < pauseMenuRect.size(); i++)
						{
							if (abs(event.mouseButton.x - pauseMenuRect[i].getPosition().x) < pauseMenuRect[i].getSize().x / 2)
							{
								if (abs(event.mouseButton.y - pauseMenuRect[i].getPosition().y) < pauseMenuRect[i].getSize().y / 2)
									menuCommandActivate[i] = true;
							}
						}
						if (menuCommandActivate[0])
						{
							if (pause1P)
								onePlayerGame = true;
							else if (pause2P)
								twoPlayerGame = true;
							pause1P = false;
							pause2P = false;
							pauseGame = false;
						}
						if (menuCommandActivate[1])
						{
							onePlayerGame = false;
							twoPlayerGame = false;
							for (int i = 0; i < numBalls; i++)
							{
								ball[i].isPocketed = false;
								ball[i].velocity = zero;
							}
							playerIsAiming = false;
							playerMovesBall = false;
							gameStart = false;
							gameIsMoving = false;
							pauseGame = false;
							pause1P = false;
							pause2P = false;
						}
					}

					else if (endofGame)
					{
						bool menuCommandActivate[] = { false, false, false };
						for (int i = 0; i < endMenuRect.size(); i++)
						{
							if (abs(event.mouseButton.x - endMenuRect[i].getPosition().x) < endMenuRect[i].getSize().x / 2)
							{
								if (abs(event.mouseButton.y - endMenuRect[i].getPosition().y) < endMenuRect[i].getSize().y / 2)
									menuCommandActivate[i] = true;
							}
						}
						if (menuCommandActivate[0])
						{
							endofGame = false;
							end1P = false;
							end2P = false;
						}
					}

					else
					{
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
							mover = 0;
							setup();
						}
						if (menuCommandActivate[1])
						{
							twoPlayerGame = true;
							gameStart = true;
							mover = 1;
							setup();
						}
						if (menuCommandActivate[2])
							programExit = true;
					}
					
				}
			}

			table();
			if (pauseGame)
			{
				clock.restart().asMilliseconds();
				ballsDraw();
				BILLIARDS.draw(ball[0].main);
				pauseMenu();
			}
			else if (endofGame)
			{
				clock.restart().asMilliseconds();
				ballsDraw();
				BILLIARDS.draw(ball[0].main);
				endGame();
			}
			else
				titleMenu();
			BILLIARDS.display();
			BILLIARDS.clear();
///////////////////////////////////////////////////////Game/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			while ((onePlayerGame) || (twoPlayerGame))
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
							sf::Vector2f newAngle(cos(aimAngle), sin(aimAngle));
							ball[0].velocity = aimLength * aimStandardForce * newAngle;
							playerIsAiming = false;
							gameIsMoving = true;
						}
						else if (playerMovesBall)
						{
							playerMovesBall = false;
							playerIsAiming = true;
						}

					}

					if (event.key.code == sf::Keyboard::P && event.type == event.KeyReleased)
					{
						pauseGame = true;
						if (onePlayerGame)
						{
							onePlayerGame = false;
							pause1P = true;
						}
						else if (twoPlayerGame)
						{
							twoPlayerGame = false;
							pause2P = true;
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
					firstHit = -1;
					ballToHit = 1;
					for (int i = 1; i < numBalls; i++)
					{
						if (ball[ballToHit].isPocketed)
							ballToHit += 1;
					}

					aimAngle = atan2f(ball[0].main.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y, ball[0].main.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x);
					aimLength = sqrt((ball[0].main.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x) * (ball[0].main.getPosition().x - sf::Mouse::getPosition(BILLIARDS).x) + (ball[0].main.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y) * (ball[0].main.getPosition().y - sf::Mouse::getPosition(BILLIARDS).y));

					if (aimLength > aimMax) { aimLength = aimMax; }

					sf::Vector2f aimOrigin(ball[0].main.getPosition());
					sf::Vector2f aimXY(aimLength * cos(aimAngle), aimLength  * sin(aimAngle));
					sf::Vector2f arrwXY1(3 * aimLength / 10 * cos(aimAngle - 11 * pi / 12), 3 * aimLength / 10 * sin(aimAngle - 11 * pi / 12));
					sf::Vector2f arrwXY2(3 * aimLength / 10 * cos(aimAngle - 13 * pi / 12), 3 * aimLength / 10 * sin(aimAngle - 13 * pi / 12));


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
							ballCollision(i);
							wallCollision(i);
							pocketCheck(i);
						}
					}

					for (int i = 0; i < numBalls; i++)
					{
						ball[i].main.setPosition(ball[i].main.getPosition() + ball[i].velocity * gameTime);
						sf::Vector2f ballVelocityAngle(cos(atan2f(ball[i].velocity.y, ball[i].velocity.x)), sin(atan2f(ball[i].velocity.y, ball[i].velocity.x)));
						float speed = sqrt(pow(ball[i].velocity.x, 2) + pow(ball[i].velocity.y, 2));
						speed = speed - frictionForce;
						if (speed <= 0)
							speed = 0;
						ball[i].velocity = speed * ballVelocityAngle;

						if (i != 0)
						{
							ball[i].mark.setPosition(ball[i].main.getPosition());
							ball[i].num.setPosition(ball[i].main.getPosition());
							ball[i].stripe.setPosition(ball[i].main.getPosition());
						}
					}
					noBallMoving = true;

					for (int i = 0; i < numBalls; i++)
					{
						if (ball[i].velocity != zero)
							noBallMoving = false;
					}

					if (noBallMoving)
					{
						gameIsMoving = false;
						if (ball[numBalls - 1].isPocketed)
						{
							if ((firstHit != ballToHit) || (ball[0].isPocketed))
							{
								ball[numBalls - 1].isPocketed = false;
								ball[numBalls - 1].main.setPosition(3 * WIDTH / 4.f, HEIGHT / 2.f);
								ballCollision(numBalls - 1);
								if (numBalls == 10)
									ball[numBalls - 1].stripe.setPosition(ball[numBalls - 1].main.getPosition());
								ball[numBalls - 1].mark.setPosition(ball[numBalls - 1].main.getPosition());
								ball[numBalls - 1].num.setPosition(ball[numBalls - 1].main.getPosition());
								playerMovesBall = true;

								if (onePlayerGame)
									mover += 2;
								else
									mover += 1;
							}
							else
							{
								if (onePlayerGame)
								{
									mover += 1;
									if (mover < BS)
										BS = mover;									
									onePlayerGame = false;
									end1P = true;
									endofGame = true;
								}

								if (twoPlayerGame)
								{
									twoPlayerGame = false;
									end2P = true;
									endofGame = true;
								}
								for (int i = 0; i < numBalls; i++)
								{
									ball[i].isPocketed = false;
									ball[i].velocity = zero;
								}
							}
						}
						else
						{
							if ((ball[0].isPocketed) || (ballToHit != firstHit))
							{
								playerMovesBall = true;
								if (onePlayerGame)
									mover += 2;
								else
									mover += 1;
							}
							else
							{
								if (twoPlayerGame)
								{
									if (justGotPocketed)
										justGotPocketed = false;
									else
										mover += 1;
								}
								else
									mover += 1;
								playerIsAiming = true;
							}
						}
					}
				}
				BILLIARDS.display();
				BILLIARDS.clear();
			}
			if (programExit)
			{
				stringstream s;
				ofstream fs;
				fs.open("BS.txt");
				fs << BS;
				fs.close();
				return 0;
			}
		}
	}
}