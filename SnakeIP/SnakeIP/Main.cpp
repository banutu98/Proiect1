#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
using namespace sf;
using namespace std;

//declarari
RenderWindow window(VideoMode(640, 512), "Snake v5.5");
Texture t1, t2, t3, t4, t5, t6, t7, t8, t9;
float timer = 0, timer_AI = 0;
int aparut = 0;

struct Snake
{
	int x = 640 / 32, y = 480 / 32;
}snake[100];

struct SnakeAI
{
	int x = 640 / 32, y = 480 / 32;
}snakeAI[100];

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

struct Scor
{
	char nume[33], nr[33];
	int numeric;
}scores[10];

int directie = 2, lungime, lungime_init, OK = 1, nr_mutari = 0, este_mancat = 1, is_rand = 1, r, specialX, specialY, counter = 0, directie_aleasa = 0, nr_mancate, nr_mancate_AI, verif_lab;
float delay = 0.3, delay_AI = 0.3;
int labirint[30][40], labirintAI[30][40], directie_AI = 0;
int Scor = 0, Scor_AI = 0, scoruri_n[11], lungime_AI, nrp = 0;

struct
{
	int x, y;
}pozitii[1000];

void initializare_AI()
{
	snakeAI[0].x = 640 / 32 + 14;
	snakeAI[0].y = 480 / 32 - 12;
	snakeAI[1].x = -4;
	snakeAI[1].y = -4;
	snakeAI[2].x = -4;
	snakeAI[2].y = -4;
	snakeAI[3].x = -4;
	snakeAI[3].y = -4;
	lungime_AI = 4;
}

void miscare_AI()
{
	for (int index = lungime_AI; index > 0; index--)
	{
		snakeAI[index].x = snakeAI[index - 1].x;
		snakeAI[index].y = snakeAI[index - 1].y;
	}
	if (directie_AI == 0)
		snakeAI[0].y += 1;
	if (directie_AI == 1)
		snakeAI[0].x -= 1;
	if (directie_AI == 2)
		snakeAI[0].x += 1;
	if (directie_AI == 3)
		snakeAI[0].y -= 1;
	if (snakeAI[0].x == food.x && snakeAI[0].y == food.y)
	{
		verif_lab = 1;
		lungime_AI++;
		food.x = rand() % 640 / 16;
		food.y = rand() % 480 / 16;
		int sigur = 1;
		do
		{
			sigur = 1;
			for (int index = 0; index < lungime_AI; index++)
				if (snakeAI[index].x == food.x && snakeAI[index].y == food.y)
					sigur = 0;
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
		nr_mancate_AI++;
		Scor_AI += 5;
		if (nr_mancate_AI % 2 == 0 && delay_AI > 0.1)
			delay_AI -= 0.02;
		if (nr_mancate % 5 == 0)
		{
			nr_mutari = 0;
			este_mancat = 1;
		}

	}

	//steaua
	if (snakeAI[0].x == star.x && snakeAI[0].y == star.y)
	{
		este_mancat = 0;
		counter++;
		is_rand = 1;
		star.x = -2;
		star.y = -2;
		Scor_AI += 10 * (30 - nr_mutari);
	}

	//slow
	if (snakeAI[0].x == slow.x && snakeAI[0].y == slow.y)
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
	if (snakeAI[0].x == cut.x && snakeAI[0].y == cut.y)
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
	if (snakeAI[0].x == multiplier.x && snakeAI[0].y == multiplier.y)
	{
		if (Scor_AI > 0)
			Scor_AI *= 2;
		else Scor_AI += 10;
		este_mancat = 0;
		counter++;
		is_rand = 1;
		multiplier.x = -2;
		multiplier.y = -2;
	}

	//trecerea prin pereti
	if (snakeAI[0].x >= 640 / 16) snakeAI[0].x = 0;
	else if (snakeAI[0].x < 0) snakeAI[0].x = 624 / 16;

	if (snakeAI[0].y >= 480 / 16) snakeAI[0].y = 0;
	else if (snakeAI[0].y < 0) snakeAI[0].y = 464 / 16;

}

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
		verif_lab = 1;
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
			for (int index = 0; index < lungime_AI; index++)
				if (snakeAI[index].x == food.x && snakeAI[index].y == food.y)
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

bool coliziune_cu_AI()
{
	for (int index = 0; index < lungime_AI; index++)
		if (snake[0].x == snakeAI[index].x && snake[0].y == snakeAI[index].y)
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
				for (int index = 0; index < lungime_AI; index++)
					if (snakeAI[index].x == food.x && snakeAI[index].y == food.y)
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
			
		}
	}
}

void desenareElemente()
{
	t1.loadFromFile("green.png");
	t2.loadFromFile("food.png");
	t6.loadFromFile("wall.png");
	t8.loadFromFile("sand.png");
	Sprite sarpe(t1), mancare(t2), zid(t6), nisip(t8);

	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			if (labirint[i][j] == 1)
			{
				zid.setPosition(j * 16, i * 16);
				window.draw(zid);
			}
			else
			{
				nisip.setPosition(j * 16, i * 16);
				window.draw(nisip);
			}
		}

	for (int index = 0; index < lungime; index++)
	{
		sarpe.setPosition(snake[index].x * 16, snake[index].y * 16);
		window.draw(sarpe);
	}
	
	mancare.setPosition(food.x * 16, food.y * 16);
	window.draw(mancare);
}

void desenare_AI()
{
	t9.loadFromFile("magenta.png");
	Sprite sarpe_ai(t9);
	
	for (int index = 0; index < lungime_AI; index++)
	{
		sarpe_ai.setPosition(snakeAI[index].x * 16, snakeAI[index].y * 16);
		window.draw(sarpe_ai);
	}
}

void directieAI()
{
	if (pozitii[nrp].x > snakeAI[0].y)
		directie_AI = 0;
	if (pozitii[nrp].x < snakeAI[0].y)
		directie_AI = 3;
	if (pozitii[nrp].y > snakeAI[0].x)
		directie_AI = 2;
	if (pozitii[nrp].y < snakeAI[0].x)
		directie_AI = 1;	
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

//initializare labirinturi

void initLabirint0()
{
	window.clear();
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
			labirint[i][j] = 0;
	lungime = 4;
	lungime_init = lungime;
	snake[0].x = 640 / 32;
	snake[0].y = 480 / 32;
	snake[1].x = -3;
	snake[1].y = -3;
	snake[2].x = -3;
	snake[2].y = -3;
	snake[3].x = -3;
	snake[3].y = -3;
	directie = 2;

	int sigur = 1;
	if (labirint[food.y][food.x] == 1)
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
}

void initLabirint1()
{
	window.clear();
	ifstream fin1("labirint1.txt");
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin1 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
	snake[0].x = 640 / 32;
	snake[0].y = 480 / 32;
	snake[1].x = -3;
	snake[1].y = -3;
	snake[2].x = -3;
	snake[2].y = -3;
	snake[3].x = -3;
	snake[3].y = -3;
	directie = 2;
	fin1.close();

	int sigur = 1;
	if (labirint[food.y][food.x] == 1)
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
}

void initLabirint2()
{
	window.clear();
	ifstream fin2("labirint2.txt");
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin2 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
	snake[0].x = 640 / 32;
	snake[0].y = 480 / 32;
	snake[1].x = -3;
	snake[1].y = -3;
	snake[2].x = -3;
	snake[2].y = -3;
	snake[3].x = -3;
	snake[3].y = -3;
	directie = 2;
	fin2.close();

	int sigur = 1;
	if (labirint[food.y][food.x] == 1)
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
}

void initLabirint3()
{
	window.clear();
	ifstream fin3("labirint3.txt");
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin3 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
	snake[0].x = 640 / 32;
	snake[0].y = 480 / 32;
	snake[1].x = -3;
	snake[1].y = -3;
	snake[2].x = -3;
	snake[2].y = -3;
	snake[3].x = -3;
	snake[3].y = -3;
	directie = 2;
	fin3.close();

	int sigur = 1;
	if (labirint[food.y][food.x] == 1)
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
}

void initLabirint4()
{
	window.clear();
	ifstream fin4("labirint4.txt");
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin4 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
	snake[0].x = 640 / 32;
	snake[0].y = 480 / 32;
	snake[1].x = -3;
	snake[1].y = -3;
	snake[2].x = -3;
	snake[2].y = -3;
	snake[3].x = -3;
	snake[3].y = -3;
	directie = 2;
	fin4.close();

	int sigur = 1;
	if (labirint[food.y][food.x] == 1)
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
}

void initLabirint5()
{
	window.clear();
	ifstream fin5("labirint5.txt");
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin5 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
	snake[0].x = 640 / 32;
	snake[0].y = 480 / 32;
	snake[1].x = -3;
	snake[1].y = -3;
	snake[2].x = -3;
	snake[2].y = -3;
	snake[3].x = -3;
	snake[3].y = -3;
	directie = 2;
	fin5.close();

	int sigur = 1;
	if (labirint[food.y][food.x] == 1)
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
}

void initLabirint6()
{
	window.clear();
	ifstream fin6("labirint6.txt");
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
		{
			fin6 >> labirint[i][j];
		}
	lungime = 4;
	lungime_init = lungime;
	snake[0].x = 640 / 32;
	snake[0].y = 480 / 32;
	snake[1].x = -3;
	snake[1].y = -3;
	snake[2].x = -3;
	snake[2].y = -3;
	snake[3].x = -3;
	snake[3].y = -3;
	directie = 2;
	fin6.close();

	int sigur = 1;
	if (labirint[food.y][food.x] == 1)
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
}

//declarari subprograme

void meniu();
void submeniu_classic();
void submeniu_versus();
void submeniu_scor();
void scoruri(int index);

//moduri joc

void snakeClassic()
{
	Clock clock;
	srand(time(0));
	window.setFramerateLimit(30);
	nr_mancate = 0;
	Scor = 0;
	int selected_menu = 0;
	char scor_char[10];
	Font font;
	font.loadFromFile("arial.ttf");
	Text text[3];
	text[1].setColor(Color::White);
	text[1].setString("Classic Mode");
	text[1].setPosition(200, 480);
	text[1].setFont(font);
	text[1].setCharacterSize(25);

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

		if (Event::KeyReleased && event.key.code == Keyboard::Escape)
		{
			selected_menu = 1;
			break;
		}

		directieSarpe();

		if (timer > delay)
		{
			timer = 0;
			miscareSarpe();
			if (coliziune() == true)
			{
				if (coliziune() == true)
				{
					char scor_curent[33];
					ifstream fin("scores_classic.txt");
					for (int i = 0; i < 10; i++)
					{
						fin >> scor_curent;
						scores[i].nume[0] = '\0';
						strcat_s(scores[i].nume, scor_curent);

						fin >> scor_curent;
						scores[i].nr[0] = '\0';
						strcat_s(scores[i].nr, scor_curent);
						scores[i].numeric = stoi(scor_curent);
					}
					fin.clear();
					fin.close();
					ofstream fout("scores_classic.txt");
					for (int i = 0; i < 10; i++)
						if (Scor >= scores[i].numeric)
						{
							for (int j = 9; j > i; j--)
								scores[j] = scores[j - 1];
							scores[i].numeric = Scor;
							scores[i].nume[0] = '\0';
							strcat_s(scores[i].nume, "scor_nou");
							scores[i].nr[0] = '\0';
							_itoa_s(Scor, scores[i].nr, 10);
							break;
						}
					for (int i = 0; i < 10; i++)
						fout << scores[i].nume << " " << scores[i].nr << " ";
					fout.close();
					scoruri(1);
				}
			}
			if (aparut == 1)
				nr_mutari++;
		}
		window.clear();

		_itoa_s(Scor, scor_char, 10);
		
		text[0].setColor(Color::White);
		text[0].setFont(font);
		text[0].setPosition(10, 480);
		text[0].setString(scor_char);
		text[0].setCharacterSize(25);

		for (int i = 0; i <= 1; i++)
			window.draw(text[i]);

		desenareElemente();

		powerUp();
		
		window.display();
	}

	if (selected_menu == 1)
		submeniu_classic();
}

void snakeCampaign()
{
	Clock clock;
	srand(time(0));
	window.setFramerateLimit(30);

	Scor = 0;
	int nr_nivel = 1, nr_labirint = 0;
	int selected_menu;
	nr_mancate = 0;
	initLabirint0();

	Font font;
	font.loadFromFile("arial.ttf");
	Text text[3];
	text[1].setColor(Color::White);
	text[1].setString("Campaign Mode");
	text[1].setPosition(200, 480);
	text[1].setFont(font);
	text[1].setCharacterSize(25);

	char scor_char[10], remained_char[4];

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

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			selected_menu = 1;
			break;
		}

		directieSarpe();
		
		if (nr_mancate > 0 && nr_mancate % (9 * nr_nivel) == 0 && verif_lab == 1)
		{
			nr_labirint++;

			if (nr_labirint == 7)
			{
				nr_labirint = 0;
				nr_nivel++;
			}

			verif_lab = 0;

			switch (nr_labirint)
			{
				case 0: initLabirint0();
						nr_mancate = 0;
						break;

				case 1: initLabirint1();
						nr_mancate = 0;
						break;

				case 2: initLabirint2();
						nr_mancate = 0;
						break;

				case 3: initLabirint3();
						nr_mancate = 0;
						break;

				case 4: initLabirint4();
						nr_mancate = 0;
						break;

				case 5: initLabirint5();
						nr_mancate = 0;
						break;

				case 6: initLabirint6();
						nr_mancate = 0;
						break;
			}
		}

		

		if (timer > delay)
		{
			timer = 0;
			miscareSarpe();
			if (coliziune() == true)
			{
				char scor_curent[33];
				ifstream fin("scores_campaign.txt");
				for (int i = 0; i < 10; i++)
				{
					fin >> scor_curent;
					scores[i].nume[0] = '\0';
					strcat_s(scores[i].nume, scor_curent);

					fin >> scor_curent;
					scores[i].nr[0] = '\0';
					strcat_s(scores[i].nr, scor_curent);
					scores[i].numeric = stoi(scor_curent);
				}
				fin.clear();
				fin.close();
				ofstream fout("scores_campaign.txt");
				for (int i = 0; i < 10; i++)
					if (Scor >= scores[i].numeric)
					{
						for (int j = 9; j > i; j--)
							scores[j] = scores[j - 1];
						scores[i].numeric = Scor;
						scores[i].nume[0] = '\0';
						strcat_s(scores[i].nume, "scor_nou");
						scores[i].nr[0] = '\0';
						_itoa_s(Scor, scores[i].nr, 10);
						break;
					}
				for (int i = 0; i < 10; i++)
					fout << scores[i].nume << " " << scores[i].nr << " ";
				fout.close();
				scoruri(2);
			}
			if (aparut == 1)
				nr_mutari++;
		}
		window.clear();

		_itoa_s(Scor, scor_char, 10);
		text[0].setColor(Color::White);
		text[0].setFont(font);
		text[0].setPosition(10, 480);
		text[0].setString(scor_char);
		text[0].setCharacterSize(25);

		
		_itoa_s(9 * nr_nivel - nr_mancate, remained_char, 10);

		text[2].setColor(Color::White);
		text[2].setFont(font);
		text[2].setPosition(600, 480);
		text[2].setString(remained_char);
		text[2].setCharacterSize(25);

		for (int i = 0; i <= 2; i++)
			window.draw(text[i]);

		desenareElemente();

		powerUp();
		
		window.display();
	}

	if (selected_menu == 1)
		meniu();
		
}

void intializare_labAI()
{
	for (int i = 0; i < 30; i++)
		for (int j = 0; j < 40; j++)
			labirintAI[i][j] = labirint[i][j];
	for (int index = 0; index < lungime; index++)
		labirintAI[snake[index].y][snake[index].x] = 1;
	for (int index = 1; index < lungime_AI; index++)
		labirintAI[snakeAI[index].y][snakeAI[index].x] = 1;
}

bool interior(int x, int y)
{
	if (x >= 0 && x < 40  && y >= 0 && y < 30) return true;
	return false;
}

void drum_minim()
{
	int xv, yv, i, prim, ultim, l, c, contor;
	int dx[] = { 1,0,0,-1 };
	int dy[] = { 0,-1,1,0 };
	struct
	{
		int x, y;
	}coada[1000];
	
	nrp = 0;
	prim = ultim = 1;
	l = snakeAI[0].x;
	c = snakeAI[0].y;
	coada[ultim].x = l;
	coada[ultim].y = c;
	labirintAI[c][l] = 2;
	while (prim <= ultim)
	{
		if (star.x != -2 && star.y != -2)
		{
			if (labirintAI[star.y][star.x] != 0)
				break;
		}
		else if (multiplier.x != -2 && multiplier.y != -2)
		{
			if (labirintAI[multiplier.y][multiplier.x] != 0)
				break;
		}
		else if (labirintAI[food.y][food.x] != 0)
			break;
		l = coada[prim].x;
		c = coada[prim++].y;
		for (i = 0; i < 4; i++)
		{
			xv = l + dx[i];
			yv = c + dy[i];
			if (interior(xv, yv) == true && labirintAI[yv][xv] == 0)
			{
				labirintAI[yv][xv] = labirintAI[c][l] + 1;
				coada[++ultim].x = xv;
				coada[ultim].y = yv;
			}
		}
	}
	pozitii[++nrp].x = food.y;
	pozitii[nrp].y = food.x;
	contor = labirintAI[food.y][food.x];
	while (contor != 3)
	{
		if (labirintAI[pozitii[nrp].x - 1][pozitii[nrp].y] == contor - 1)
		{
			nrp++;
			pozitii[nrp].x = pozitii[nrp-1].x - 1;
			pozitii[nrp].y = pozitii[nrp-1].y;
			contor--;
		}
		else if (labirintAI[pozitii[nrp].x + 1][pozitii[nrp].y] == contor - 1)
		{
			nrp++;
			pozitii[nrp].x = pozitii[nrp - 1].x + 1;
			pozitii[nrp].y = pozitii[nrp - 1].y;
			contor--;
		}
		else if (labirintAI[pozitii[nrp].x][pozitii[nrp].y - 1] == contor - 1)
		{
			nrp++;
			pozitii[nrp].x = pozitii[nrp - 1].x;
			pozitii[nrp].y = pozitii[nrp - 1].y - 1;
			contor--;
		}
		else if (labirintAI[pozitii[nrp].x][pozitii[nrp].y + 1] == contor - 1)
		{
			nrp++;
			pozitii[nrp].x = pozitii[nrp - 1].x;
			pozitii[nrp].y = pozitii[nrp - 1].y + 1;
			contor--;
		}
	}
	/*for (int i = nrp; i >= 1; i--)
		cout << labirintAI[pozitii[i].x][pozitii[i].y] << " ";*/
}

void snakeVersus()
{
	initializare_AI();
	Clock clock;
	srand(time(0));
	window.setFramerateLimit(30);
	nr_mancate = 0;
	Scor = 0, Scor_AI = 0;
	int selected_menu = 0;
	char scor_char[10], scorAI_char[10];
	Font font;
	font.loadFromFile("arial.ttf");
	Text text[3];
	text[1].setColor(Color::White);
	text[1].setString("VS Mode");
	text[1].setPosition(260, 480);
	text[1].setFont(font);
	text[1].setCharacterSize(25);

	while (window.isOpen())
	{
		float timp = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += timp;
		timer_AI += timp;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

		}

		if (Event::KeyReleased && event.key.code == Keyboard::Escape)
		{
			selected_menu = 1;
			break;
		}

		directieSarpe();

		if (timer > delay)
		{
			timer = 0;
			miscareSarpe();
			if (coliziune() == true || coliziune_cu_AI() == true)
			{
				char scor_curent[33];
				ifstream fin("scores_versus.txt");
				for (int i = 0; i < 10; i++)
				{
					fin >> scor_curent;
					scores[i].nume[0] = '\0';
					strcat_s(scores[i].nume, scor_curent);

					fin >> scor_curent;
					scores[i].nr[0] = '\0';
					strcat_s(scores[i].nr, scor_curent);
					scores[i].numeric = stoi(scor_curent);
				}
				fin.clear();
				fin.close();
				ofstream fout("scores_versus.txt");
				for (int i = 0; i < 10; i++)
					if (Scor >= scores[i].numeric)
					{
						for (int j = 9; j > i; j--)
							scores[j] = scores[j - 1];
						scores[i].numeric = Scor;
						scores[i].nume[0] = '\0';
						strcat_s(scores[i].nume, "scor_nou");
						scores[i].nr[0] = '\0';
						_itoa_s(Scor, scores[i].nr, 10);
						break;
					}
				for (int i = 0; i < 10; i++)
					fout << scores[i].nume << " " << scores[i].nr << " ";
				fout.close();
				scoruri(3);
			}
			if (aparut == 1)
				nr_mutari++;
			intializare_labAI();
			drum_minim();
		}

		directieAI();

		if (timer_AI > delay_AI)
		{
			timer_AI = 0;
			miscare_AI();
		}
		window.clear();

		_itoa_s(Scor, scor_char, 10);

		text[0].setColor(Color::Green);
		text[0].setFont(font);
		text[0].setPosition(10, 480);
		text[0].setString(scor_char);
		text[0].setCharacterSize(25);

		_itoa_s(Scor_AI, scorAI_char, 10);

		text[2].setColor(Color::Magenta);
		text[2].setFont(font);
		text[2].setPosition(600, 480);
		text[2].setString(scorAI_char);
		text[2].setCharacterSize(25);

		for (int i = 0; i <= 2; i++)
			window.draw(text[i]);

		desenareElemente();

		desenare_AI();

		powerUp();

		window.display();
	}

	if (selected_menu == 1)
		submeniu_versus();
}

//meniu

void meniu()
{
	window.clear();
	int selectedIndex = 0;
	Font font;
	font.loadFromFile("Beef'd.ttf");
	Text text[6];

	text[0].setFont(font);
	text[0].setColor(Color::Red);
	text[0].setString("Classic mode");
	text[0].setPosition(100, 100);
	text[0].setCharacterSize(20);

	text[1].setFont(font);
	text[1].setColor(Color::White);
	text[1].setString("Campaign mode");
	text[1].setPosition(100, 160);
	text[1].setCharacterSize(20);

	text[2].setFont(font);
	text[2].setColor(Color::White);
	text[2].setString("Versus mode");
	text[2].setPosition(100, 220);
	text[2].setCharacterSize(20);

	text[3].setFont(font);
	text[3].setColor(Color::White);
	text[3].setString("High Scores");
	text[3].setPosition(100, 280);
	text[3].setCharacterSize(20);

	text[4].setFont(font);
	text[4].setColor(Color::White);
	text[4].setString("Exit");
	text[4].setPosition(100, 340);
	text[4].setCharacterSize(20);

	text[5].setFont(font);
	text[5].setColor(Color::Magenta);
	text[5].setString("Snake 5.5");
	text[5].setPosition(200, 10);
	text[5].setCharacterSize(20);

	int selected_menu;

	while (window.isOpen())
	{
		for (int i = 0; i <= 5; i++)
			window.draw(text[i]);
		window.display();
		window.clear();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			//meniu jos
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				if (selectedIndex < 4)
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex++;
					text[selectedIndex].setColor(Color::Red);
				}
				else
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex = 0;
					text[selectedIndex].setColor(Color::Red);
				}
			}

			//meniu sus
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (selectedIndex > 0)
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex--;
					text[selectedIndex].setColor(Color::Red);
				}
				else
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex = 4;
					text[selectedIndex].setColor(Color::Red);
				}
			}


		}

		//meniu select

		if (Event::KeyReleased && event.key.code == Keyboard::Space)
		{
			break;
		}
	}

	switch (selectedIndex)
	{
		case 0:
			submeniu_classic();
			break;

		case 1:
			snakeCampaign();
			break;

		case 2:
			submeniu_versus();
			break;

		case 3:
			submeniu_scor();
			break;

		case 4:
			window.close();
			break;
	}
}

//submeniu mod clasic

void submeniu_classic()
{
	window.clear();
	int selectedIndex = 1;
	Font font;
	font.loadFromFile("Beef'd.ttf");
	Text text[8];

	text[0].setFont(font);
	text[0].setColor(Color::Magenta);
	text[0].setString("Select a map");
	text[0].setPosition(180, 10);
	text[0].setCharacterSize(20);

	text[1].setFont(font);
	text[1].setColor(Color::Red);
	text[1].setString("Map 0");
	text[1].setPosition(100, 100);
	text[1].setCharacterSize(20);

	text[2].setFont(font);
	text[2].setColor(Color::White);
	text[2].setString("Map 1");
	text[2].setPosition(100, 150);
	text[2].setCharacterSize(20);

	text[3].setFont(font);
	text[3].setColor(Color::White);
	text[3].setString("Map 2");
	text[3].setPosition(100, 200);
	text[3].setCharacterSize(20);

	text[4].setFont(font);
	text[4].setColor(Color::White);
	text[4].setString("Map 3");
	text[4].setPosition(100, 250);
	text[4].setCharacterSize(20);

	text[5].setFont(font);
	text[5].setColor(Color::White);
	text[5].setString("Map 4");
	text[5].setPosition(100, 300);
	text[5].setCharacterSize(20);

	text[6].setFont(font);
	text[6].setColor(Color::White);
	text[6].setString("Map 5");
	text[6].setPosition(100, 350);
	text[6].setCharacterSize(20);

	text[7].setFont(font);
	text[7].setColor(Color::White);
	text[7].setString("Map 6");
	text[7].setPosition(100, 400);
	text[7].setCharacterSize(20);

	int selected_menu = 0;

	while (window.isOpen())
	{
		for (int i = 0; i <= 7; i++)
			window.draw(text[i]);
		window.display();
		window.clear();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			//meniu jos
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				if (selectedIndex < 7)
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex++;
					text[selectedIndex].setColor(Color::Red);
				}
				else
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex = 1;
					text[selectedIndex].setColor(Color::Red);
				}
			}

			//meniu sus
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (selectedIndex > 1)
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex--;
					text[selectedIndex].setColor(Color::Red);
				}
				else
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex = 7;
					text[selectedIndex].setColor(Color::Red);
				}
			}


		}

		//meniu select
		
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			selected_menu = 1;
			break;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			selected_menu = 2;
			break;
		}
	}

	if (selected_menu == 1)
	{
		switch (selectedIndex)
		{
			case 1:
				initLabirint0();
				snakeClassic();
				break;

			case 2:
				initLabirint1();
				snakeClassic();
				break;

			case 3:
				initLabirint2();
				snakeClassic();
				break;

			case 4:
				initLabirint3();
				snakeClassic();
				break;

			case 5:
				initLabirint4();
				snakeClassic();
				break;

			case 6:
				initLabirint5();
				snakeClassic();
				break;

			case 7:
				initLabirint6();
				snakeClassic();
				break;
		}
	}
	else if (selected_menu == 2)
		meniu();
}

//submeniu mod versus

void submeniu_versus()
{
	window.clear();
	int selectedIndex = 1;
	Font font;
	font.loadFromFile("Beef'd.ttf");
	Text text[8];

	text[0].setFont(font);
	text[0].setColor(Color::Magenta);
	text[0].setString("Select a map");
	text[0].setPosition(180, 10);
	text[0].setCharacterSize(20);

	text[1].setFont(font);
	text[1].setColor(Color::Red);
	text[1].setString("Map 0");
	text[1].setPosition(100, 100);
	text[1].setCharacterSize(20);

	text[2].setFont(font);
	text[2].setColor(Color::White);
	text[2].setString("Map 1");
	text[2].setPosition(100, 150);
	text[2].setCharacterSize(20);

	text[3].setFont(font);
	text[3].setColor(Color::White);
	text[3].setString("Map 2");
	text[3].setPosition(100, 200);
	text[3].setCharacterSize(20);

	text[4].setFont(font);
	text[4].setColor(Color::White);
	text[4].setString("Map 3");
	text[4].setPosition(100, 250);
	text[4].setCharacterSize(20);

	text[5].setFont(font);
	text[5].setColor(Color::White);
	text[5].setString("Map 4");
	text[5].setPosition(100, 300);
	text[5].setCharacterSize(20);

	text[6].setFont(font);
	text[6].setColor(Color::White);
	text[6].setString("Map 5");
	text[6].setPosition(100, 350);
	text[6].setCharacterSize(20);

	text[7].setFont(font);
	text[7].setColor(Color::White);
	text[7].setString("Map 6");
	text[7].setPosition(100, 400);
	text[7].setCharacterSize(20);

	int selected_menu = 0;

	while (window.isOpen())
	{
		for (int i = 0; i <= 7; i++)
			window.draw(text[i]);
		window.display();
		window.clear();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			//meniu jos
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				if (selectedIndex < 7)
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex++;
					text[selectedIndex].setColor(Color::Red);
				}
				else
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex = 1;
					text[selectedIndex].setColor(Color::Red);
				}
			}

			//meniu sus
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (selectedIndex > 1)
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex--;
					text[selectedIndex].setColor(Color::Red);
				}
				else
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex = 7;
					text[selectedIndex].setColor(Color::Red);
				}
			}


		}

		//meniu select

		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			selected_menu = 1;
			break;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			selected_menu = 2;
			break;
		}
	}

	if (selected_menu == 1)
	{
		switch (selectedIndex)
		{
		case 1:
			initLabirint0();
			snakeVersus();
			break;

		case 2:
			initLabirint1();
			snakeVersus();
			break;

		case 3:
			initLabirint2();
			snakeVersus();
			break;

		case 4:
			initLabirint3();
			snakeVersus();
			break;

		case 5:
			initLabirint4();
			snakeVersus();
			break;

		case 6:
			initLabirint5();
			snakeVersus();
			break;

		case 7:
			initLabirint6();
			snakeVersus();
			break;
		}
	}
	else if (selected_menu == 2)
		meniu();
}

//submeniu scoruri

void submeniu_scor()
{
	window.clear();
	int selectedIndex = 1;
	Font font;
	font.loadFromFile("Beef'd.ttf");
	Text text[6];

	text[0].setFont(font);
	text[0].setColor(Color::Magenta);
	text[0].setString("Select a mode");
	text[0].setPosition(150, 50);
	text[0].setCharacterSize(20);

	text[1].setFont(font);
	text[1].setColor(Color::Red);
	text[1].setString("Classic");
	text[1].setPosition(100, 160);
	text[1].setCharacterSize(20);

	text[2].setFont(font);
	text[2].setColor(Color::White);
	text[2].setString("Campaign");
	text[2].setPosition(100, 220);
	text[2].setCharacterSize(20);

	text[3].setFont(font);
	text[3].setColor(Color::White);
	text[3].setString("Versus");
	text[3].setPosition(100, 280);
	text[3].setCharacterSize(20);

	text[4].setFont(font);
	text[4].setColor(Color::White);
	text[4].setString("Reset scores");
	text[4].setPosition(100, 400);
	text[4].setCharacterSize(20);

	int selected_menu;

	while (window.isOpen())
	{
		for (int i = 0; i <= 4; i++)
			window.draw(text[i]);
		window.display();
		window.clear();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			//meniu jos
			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				if (selectedIndex < 4)
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex++;
					text[selectedIndex].setColor(Color::Red);
				}
				else
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex = 1;
					text[selectedIndex].setColor(Color::Red);
				}
			}

			//meniu sus
			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				if (selectedIndex > 1)
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex--;
					text[selectedIndex].setColor(Color::Red);
				}
				else
				{
					text[selectedIndex].setColor(Color::White);
					selectedIndex = 4;
					text[selectedIndex].setColor(Color::Red);
				}
			}


		}

		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			selected_menu = 1;
			break;
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			selected_menu = 2;
			break;
		}
	}

	if (selected_menu == 2)
		meniu();
	else if (selected_menu == 1)
		scoruri(selectedIndex);
}

//scoruri

void resetare_scoruri()
{
	ofstream fout1("scores_classic.txt");
	ofstream fout2("scores_campaign.txt");
	ofstream fout3("scores_versus.txt");

	for (int i = 0; i <= 9; i++)
	{
		fout1 << "no_name " << "0 ";
		fout2 << "no_name " << "0 ";
		fout3 << "no_name " << "0 ";
	}

	fout1.close();
	fout2.close();
	fout3.close();
}

void scoruri(int index)
{
	window.clear();
	Font font;
	font.loadFromFile("Beef'd.ttf");
	Text scores_text[11];
	char nume_fisier[33];
	nume_fisier[0] = '\0';
	switch (index)
	{
		case 1:
			strcat_s(nume_fisier, "scores_classic.txt");
			break;
		
		case 2:
			strcat_s(nume_fisier, "scores_campaign.txt");
			break;

		case 3:
			strcat_s(nume_fisier, "scores_versus.txt");
			break;

		case 4:
			resetare_scoruri();
			break;
	}

	ifstream fin_scor(nume_fisier);

	scores_text[0].setColor(Color::Magenta);
	scores_text[0].setFont(font);
	scores_text[0].setString("High Scores");
	scores_text[0].setPosition(200, 10);
	scores_text[0].setCharacterSize(20);

	char scor_de_afisat[33];

	for (int i = 1; i <= 10; i++)
	{
		fin_scor >> scores[i-1].nume >> scores[i-1].nr;
		scor_de_afisat[0] = '\0';
		strcat_s(scor_de_afisat, scores[i-1].nume);
		strcat_s(scor_de_afisat, " - ");
		strcat_s(scor_de_afisat, scores[i-1].nr);
		scores_text[i].setColor(Color::White);
		scores_text[i].setFont(font);
		scores_text[i].setCharacterSize(20);
		scores_text[i].setString(scor_de_afisat);
		scores_text[i].setPosition(100, 35*i + 40);
	}

	int selected_menu = 0;

	while (window.isOpen())
	{
		for (int i = 0; i <= 10; i++)
			window.draw(scores_text[i]);
		window.display();
		window.clear();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Event::KeyReleased && event.key.code == Keyboard::Escape)
		{
			selected_menu = 1;
			break;
		}
	}

	if (selected_menu == 1)
		submeniu_scor();
	
}


int main()
{
	meniu();
	return 0;
}