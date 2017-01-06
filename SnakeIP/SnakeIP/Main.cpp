#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace sf;
using namespace std;

//fisiere
ifstream fin1("labirint1.txt");
ifstream fin2("labirint2.txt");
ifstream fin3("labirint3.txt");
ifstream fin4("labirint4.txt");
ifstream fin5("labirint5.txt");
ifstream fin6("labirint6.txt");

//declarari
RenderWindow window(VideoMode(640, 480), "Snake v4.2");
Texture t1, t2, t3, t4, t5, t6, t7;
float timer = 0;
int aparut = 0;

struct Snake
{
	int x = 640 / 32, y = 480 / 32;
}snake[100];

struct Food
{
	int x = 10, y = 8;
}food;

struct Star
{
	int x = -2, y = -2;
}star;

struct Slow
{
	int x = -2, y = -2;
}slow;

struct Cut
{
	int x = -2, y = -2;
}cut;

struct Multiplier
{
	int x = -2, y = -2;
}multiplier;
int directie = 2, lungime, lungime_init, nr_mancate = 0, OK = 1, nr_mutari = 0, este_mancat = 1, is_rand = 1, r, specialX, specialY, counter = 0, directie_aleasa = 0;
float delay = 0.3;
int labirint[30][40];
int Scor = 0;

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
	directie_aleasa = 0;

	//fruct
	if (snake[0].x == food.x && snake[0].y == food.y)
	{
		lungime++;
		food.x = rand() % 640/16;
		food.y = rand() % 480/16;
		int sigur = 1;
		do
		{
			sigur = 1;
			for (int index = 0; index < lungime; index++)
				if (snake[index].x == food.x && snake[index].y == food.y)
					sigur = 0;
			if (food.x >= (640 / 16) - 1 || food.x <= 1 || food.y <= 1 || food.y >= (480 / 16) - 1)
				sigur = 0;
			if (labirint[food.y][food.x] == 1)
				sigur = 0;
			if (sigur == 0)
			{
				food.x = rand() % 640 / 16;
				food.y = rand() % 480 / 16;
			}
		} while (sigur == 0);
		nr_mancate++;
		Scor += 5;
		cout << Scor << " ";
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
		star.x = -2;
		star.y = -2;
		Scor += 10 * (30 - nr_mutari);
		cout << Scor << " ";
	}
	
	//slow
	if (snake[0].x == slow.x && snake[0].y == slow.y)
	{
		if (delay <= 0.3)
			delay += 0.1;
		este_mancat = 0;
		counter++;
		is_rand = 1;
		slow.x = -2;
		slow.y = -2;
	}

	//scadere lungime
	if (snake[0].x == cut.x && snake[0].y == cut.y)
	{
		if (lungime > 3)
			lungime /= 2;
		este_mancat = 0;
		counter++;
		is_rand = 1;
		cut.x = -2;
		cut.y = -2;
	}

	//dublare scor
	if (snake[0].x == multiplier.x && snake[0].y == multiplier.y)
	{
		if (Scor > 0)
			Scor *= 2;
		else Scor += 10;
		este_mancat = 0;
		counter++;
		is_rand = 1;
		multiplier.x = -2;
		multiplier.y = -2;
	}

	//trecerea prin pereti
	if (snake[0].x >= 640/16) snake[0].x = 0;
	else if(snake[0].x < 0) snake[0].x = 624/16;

	if (snake[0].y >= 480/16) snake[0].y = 0;
	else if (snake[0].y < 0) snake[0].y = 464/16;

}

bool coliziune()
{
	if (labirint[snake[0].y][snake[0].x] == 1)
		return true;
	for (int index = 1; index < lungime; index++)
		if (snake[index].x == snake[0].x && snake[index].y == snake[0].y)
			return true;
	return false;
}

void powerUp()
{
	t3.loadFromFile("star.png");
	t4.loadFromFile("clock.png");
	t5.loadFromFile("minus2.png");
	t7.loadFromFile("2x.png");
	Sprite stea(t3), incetinire(t4), scurtare(t5), dublare(t7);

	if (nr_mancate % 6 == 0)
		counter = nr_mancate;
	if (counter % 6 == 0 && counter != 0 && este_mancat == 1)
	{
		if (is_rand == 1)
		{
			specialX = rand() % 640 / 16;
			specialY = rand() % 480 / 16;
			int sigur = 1;
			do
			{
				sigur = 1;
				for (int index = 0; index < lungime; index++)
					if (snake[index].x == specialX && snake[index].y == specialY)
						sigur = 0;
				if (specialX == food.x && specialY == food.y)
					sigur = 0;
				if (specialX >= (640 / 16) - 1 || specialX <= 1 || specialY <= 1 || specialY >= (480 / 16) - 1)
					sigur = 0;
				if (labirint[specialX][specialY] == 1)
					sigur = 0;
				if (sigur == 0)
				{
					specialX = rand() % 640 / 16;
					specialY = rand() % 480 / 16;
				}
			} while (sigur == 0);
			r = rand() % 4 + 1;
			is_rand = 0;
		}
		aparut = 1;
		if (este_mancat == 1 && nr_mutari <= 30)
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
			else if (r == 3)
			{
				cut.x = specialX;
				cut.y = specialY;
				scurtare.setPosition(cut.x * 16, cut.y * 16);
				window.draw(scurtare);
			}
			else
			{
				multiplier.x = specialX;
				multiplier.y = specialY;
				dublare.setPosition(multiplier.x * 16, multiplier.y * 16);
				window.draw(dublare);
			}
		}
		else
		{
			is_rand = 1;
			este_mancat = 0;
			cut.x = -2;
			cut.y = cut.x;
			star.x = cut.x;
			star.y = cut.x;
			slow.x = cut.x;
			slow.y = cut.x;
			window.clear();
		}
	}
}

void desenareElemente()
{
	t1.loadFromFile("green.png");
	t2.loadFromFile("food.png");
	t6.loadFromFile("wall.png");
	Sprite sarpe(t1), mancare(t2), zid(t6);

	for (int index = 0; index < lungime; index++)
	{
		sarpe.setPosition(snake[index].x * 16, snake[index].y * 16);
		window.draw(sarpe);
	}

	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			if (labirint[i][j] == 1)
			{
				zid.setPosition(j * 16, i * 16);
				window.draw(zid);
			}
		}

	
	mancare.setPosition(food.x * 16, food.y * 16);
	window.draw(mancare);
}

void directieSarpe()
{

	if (Keyboard::isKeyPressed(Keyboard::Left) && directie != 2 && directie_aleasa == 0)
	{
		directie = 1;
		directie_aleasa = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right) && directie != 1 && directie_aleasa == 0)
	{
		directie = 2;
		directie_aleasa = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Up) && directie != 0 && directie_aleasa == 0)
	{
		directie = 3;
		directie_aleasa = 1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Down) && directie != 3 && directie_aleasa == 0)
	{
		directie = 0;
		directie_aleasa = 1;
	}
}

void snakeClassic()
{
	Clock clock;
	srand(time(0));
	window.setFramerateLimit(30);

	Scor = 0;
	
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

		directieSarpe();

		if (timer > delay)
		{
			timer = 0;
			miscareSarpe();
			if (coliziune() == true)
			{
				cout << "crash";
			}
			if (aparut == 1)
				nr_mutari++;
		}
		window.clear();

		powerUp();
		
		desenareElemente();
		
		window.display();
	}
}

//initializare labirinturi

void initLabirint0()
{
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
			labirint[i][j] = 0;
	lungime = 4;
	lungime_init = lungime;
	Scor = 0;
}

void initLabirint1()
{
	
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin1 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
}

void initLabirint2()
{

	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin2 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
}

void initLabirint3()
{

	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin3 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
}

void initLabirint4()
{

	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin4 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
}

void initLabirint5()
{

	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin5 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
}

void initLabirint6()
{

	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin6 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
}

//story mode

void storyMode()
{
	int nr_nivel = 1;
	nr_mancate = 0;
	while (nr_mancate < nr_nivel * 10)
	{
		initLabirint0();
	}
}

int main()
{
	initLabirint1();
	snakeClassic();
	return 0;
}