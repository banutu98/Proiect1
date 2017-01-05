#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
using namespace sf;

//declarari
RenderWindow window(VideoMode(800, 640), "Snake v3.0");
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
	int x=-2, y=-2;
}star;

struct Slow
{
	int x=-2, y=-2;
}slow;

struct Less
{
	int x=-2, y=-2;
}less;

int directie = 2, lungime, lungime_init, nr_mancate = 0, OK = 1, nr_mutari = 0, este_mancat = 1, is_rand = 1, r, specialX, specialY, counter = 0;
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
		int sigur = 1;
		do
		{
			sigur = 1;
			for (int index = 0; index < lungime; index++)
				if (snake[index].x == food.x && snake[index].y == food.y)
					sigur = 0;
			if (food.x >= (800 / 16) - 1 || food.x <= 1 || food.y <= 1 || food.y >= (640 / 16) - 1)
				sigur = 0;
			if (sigur == 0)
			{
				food.x = rand() % 800 / 16;
				food.y = rand() % 640 / 16;
			}
		} while (sigur == 0);
		nr_mancate++;
		if (nr_mancate % 2 == 0 && delay > 0.1) 
			delay -= 0.02;
		if (nr_mancate % 5 == 0)
		{
			nr_mutari = 0;
			este_mancat = 1;
		}
		
	}

	//steaua
	if (snake[0].x == star.x && snake[0].y == star.y)
	{
		este_mancat = 0;
		counter++;
		is_rand = 1;
	}
	
	//slow
	if (snake[0].x == slow.x && snake[0].y == slow.y)
	{
		if (delay <= 0.3)
			delay += 0.1;
		este_mancat = 0;
		counter++;
		is_rand = 1;
	}

	//scadere lungime
	if (snake[0].x == less.x && snake[0].y == less.y)
	{
		if (lungime > 3)
			lungime /= 2;
		este_mancat = 0;
		counter++;
		is_rand = 1;
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
	srand(time(0));
	window.setFramerateLimit(60);
	t1.loadFromFile("green.png");
	t2.loadFromFile("food.png");
	t3.loadFromFile("star.png");
	t4.loadFromFile("clock.png");
	t5.loadFromFile("minus2.png");
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
			if (aparut == 1)
				nr_mutari++;
		}
		window.clear();

		if (nr_mancate % 6 == 0)
			counter = nr_mancate;
		if (counter % 6 == 0 && counter != 0 && este_mancat == 1)
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
					if (specialX >= (800 / 16) - 1 || specialX <= 1 || specialY <= 1 || specialY >= (640 / 16) - 1)
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
		if (este_mancat == 1 && nr_mutari <= 40)
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
			is_rand = 1;
			este_mancat = 0;
			less.x = -2;
			less.y = less.x;
			star.x = less.x;
			star.y = less.x;
			slow.x = less.x;
			slow.y = less.x;
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
	snakeOnGoing();
}

int main()
{
	while (window.isOpen())
		initLabirint0();
	return 0;
}