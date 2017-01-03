#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
using namespace sf;

//declarari
RenderWindow window(VideoMode(800, 640), "Snake v2.1");
Texture t1, t2, t3;
float timer = 0;
int aparut = 0;

struct Snake
{
	int x, y;
}snake[100];

struct Food
{
	int x=8, y=10;
}food;

struct Star
{
	int x=12, y=4;
}star;


int directie = 2, lungime, lungime_init, nr_mancate = 0, OK = 1, nr_mutari = 0;
float delay = 0.3;

void miscareSarpe()
{
	//miscare sarpe
	for (int index = lungime; index>0; index--)
	{
		snake[index].x = snake[index - 1].x;
		snake[index].y = snake[index - 1].y;
	}
	if (directie == 0)
		snake[0].y += 1;
	if (directie == 1)
		snake[0].x -= 1;
	if (directie == 2)
		snake[0].x += 1;
	if (directie == 3)
		snake[0].y -= 1;

	//fruct
	if (snake[0].x == food.x && snake[0].y == food.y)
	{
		lungime++;
		food.x = rand() % 800/16;
		food.y = rand() % 640/16;
		if (nr_mancate % 2 == 0) 
		{
			if (delay > 0.1)
				delay -= 0.04; 
			nr_mutari = 0;
			OK = 1;
		}
	}

	//steaua
	if (snake[0].x == star.x && snake[0].y == star.y)
	{
		lungime++;
		star.x = rand() % 800 / 16;
		star.y = rand() % 640 / 16;
		if (nr_mancate % 2 == 0 && delay > 0.1) delay -= 0.04;
	}
}


void snakeOnGoing()
{
	Clock clock;
	srand(0);
	window.setFramerateLimit(60);
	t1.loadFromFile("green.png");
	t2.loadFromFile("food.png");
	t3.loadFromFile("star.png");
	Sprite sarpe(t1), mancare(t2), stea(t3);

	while (window.isOpen())
	{
		float timp = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += timp;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left) && directie != 2)
			directie = 1;
		if (Keyboard::isKeyPressed(Keyboard::Right) && directie != 1)
			directie = 2;
		if (Keyboard::isKeyPressed(Keyboard::Up) && directie != 0)
			directie = 3;
		if (Keyboard::isKeyPressed(Keyboard::Down) && directie != 3)
			directie = 0;

		if (timer > delay)
		{
			timer = 0;
			miscareSarpe();
			if (aparut == 1 && OK == 1)
				nr_mutari++;
		}
		window.clear();

		if((lungime_init - lungime) % 5 == 0 && lungime != lungime_init)
		{
			aparut = 1;
			if (OK == 1 && nr_mutari <= 30)
			{
				stea.setPosition(star.x * 16, star.y * 16);
				window.draw(stea);
			}
			else
			{
				window.clear();
				OK = 0;
			}
		}
		for (int index = 0; index < lungime; index++)
		{
			sarpe.setPosition(snake[index].x * 16, snake[index].y * 16);
			window.draw(sarpe);
		}
		mancare.setPosition(food.x * 16, food.y * 16);
		window.draw(mancare);
		window.display();
	}
}

void initLabirint0()
{
	lungime = 4;
	lungime_init = lungime;
	snake[0].x = 800 / 32;
	snake[0].y = 640 / 32;
	
	for (int index = 1; index < lungime; index++)
	{
		snake[index].x = snake[index - 1].x - 16;
		snake[index].y = snake[index - 1].y;
	}

	if (Keyboard::isKeyPressed(Keyboard::Space))
		snakeOnGoing();
}

int main()
{
	while (window.isOpen())
		initLabirint0();
	return 0;
}