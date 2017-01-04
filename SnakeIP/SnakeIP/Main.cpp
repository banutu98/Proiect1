#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
using namespace sf;

//declarari
RenderWindow window(VideoMode(800, 640), "Snake v2.1");
Texture t1, t2, t3, t4, t5;
float timer = 0;
int aparut = 0;

struct Snake
{
	int x=800/32, y=640/32;
}snake[100];

struct Food
{
	int x=8, y=10;
}food;

struct Star
{
	int x, y;
}star;

struct Slow
{
	int x, y;
}slow;

struct Less
{
	int x, y;
}less;

int directie = 2, lungime, lungime_init, nr_mancate = 0, OK = 1, nr_mutari = 0, verif = 0, counter = 1, is_rand = 0, r, specialX, specialY;
float delay = 0.1;

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
		counter++;
		is_rand = 1;
		food.x = rand() % 800/16;
		food.y = rand() % 640/16;
		int sigur = 1;
		do
		{
			sigur = 1;
			for (int index = 0; index < lungime; index++)
				if (snake[index].x == food.x && snake[index].y == food.y)
					sigur = 0;
			if (sigur == 0)
			{
				food.x = rand() % 800 / 16;
				food.y = rand() % 640 / 16;
			}
		} while (sigur == 0);
		nr_mancate++;
		if (nr_mancate % 2 == 0 && delay > 0.1) 
			delay -= 0.04;
		if (nr_mancate % 3 == 0)
		{
			nr_mutari = 0;
			OK = 1;
		}
		
	}

	//steaua
	if (snake[0].x == star.x && snake[0].y == star.y)
	{
		verif = 0;
		nr_mancate++;
		if (nr_mancate % 2 == 0 && delay > 0.1) delay -= 0.04;
	}
	
	//slow
	if (snake[0].x == slow.x && snake[0].y == slow.y)
	{
		delay += 0.14;
		verif = 0;
		nr_mancate++;
		if (nr_mancate % 2 == 0 && delay > 0.1) delay -= 0.04;
	}

	//scadere lungime
	if (snake[0].x == less.x && snake[0].y == less.y)
	{
		lungime /= 2;
		verif = 0;
		nr_mancate++;
		if (nr_mancate % 2 == 0 && delay > 0.1) delay -= 0.04;
	}
	//trecerea prin pereti
	if (snake[0].x>=800/16) snake[0].x = 0;
	else if(snake[0].x<0) snake[0].x = 784/16;
	if (snake[0].y>=640/16) snake[0].y = 0;
	else if (snake[0].y<0) snake[0].y = 624/16;

}


void snakeOnGoing()
{
	Clock clock;
	srand(0);
	window.setFramerateLimit(60);
	t1.loadFromFile("green.png");
	t2.loadFromFile("food.png");
	t3.loadFromFile("star.png");
	t4.loadFromFile("clock.png");
	t5.loadFromFile("minus.png");
	Sprite sarpe(t1), mancare(t2), stea(t3), incetinire(t4), scurtare(t5);

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

		if (counter % 5 == 0)
			verif = 1;
		if (verif == 1)
		{
			if (is_rand == 1)
			{
				specialX = rand() % 800 / 16;
				specialY = rand() % 640 / 16;
				int sigur = 1;
				do
				{
					sigur = 1;
					for (int index = 0; index < lungime; index++)
						if (snake[index].x == specialX && snake[index].y == specialY)
							sigur = 0;
					if (specialX == food.x && specialY == food.y)
						sigur = 0;
					if (sigur == 0)
					{
						specialX = rand() % 800 / 16;
						specialY = rand() % 640 / 16;
					}
				} while (sigur == 0);
				r = rand() % 3 + 1;
				is_rand = 0;
			}
			aparut = 1;
			if (OK == 1 && nr_mutari <= 50)
			{
				if (r == 1)
				{
					star.x = specialX;
					star.y = specialY;
					stea.setPosition(star.x * 16, star.y * 16);
					window.draw(stea);
				}
				else if (r == 2)
				{
					slow.x = specialX;
					slow.y = specialY;
					incetinire.setPosition(slow.x * 16, slow.y * 16);
					window.draw(incetinire);
				}
				else 
				{
					less.x = specialX;
					less.y = specialY;
					scurtare.setPosition(less.x * 16, less.y * 16);
					window.draw(scurtare);
				}
			}
			else
			{
				OK = 0;
				window.clear();
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
	if (Keyboard::isKeyPressed(Keyboard::Space))
		snakeOnGoing();
}

int main()
{
	while (window.isOpen())
		initLabirint0();
	return 0;
}