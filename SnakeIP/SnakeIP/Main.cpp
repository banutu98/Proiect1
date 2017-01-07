#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdio.h>
using namespace sf;
using namespace std;

//declarari
RenderWindow window(VideoMode(640, 512), "Snake v5.0");
Texture t1, t2, t3, t4, t5, t6, t7, t8;
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

int directie = 2, lungime, lungime_init, OK = 1, nr_mutari = 0, este_mancat = 1, is_rand = 1, r, specialX, specialY, counter = 0, directie_aleasa = 0, nr_mancate, verif_lab;
float delay = 0.3;
int labirint[30][40];
int Scor = 0, scoruri_n[11];

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
void scoruri();

//moduri joc

void snakeClassic()
{
	Clock clock;
	srand(time(0));
	window.setFramerateLimit(30);
	nr_mancate = 0;
	Scor = 0;
	int selected_menu = 0;
	char scor_char[4];
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
					int scor_curent;
					ifstream fin("scores.txt");
					for (int i = 0; i < 10; i++)
					{
						fin >> scor_curent;
						scoruri_n[i] = scor_curent;
					}
					fin.clear();
					fin.close();
					ofstream fout("scores.txt");
					for (int i = 0; i < 10; i++)
						if (Scor >= scoruri_n[i])
						{
							for (int j = 9; j > i; j--)
								scoruri_n[j] = scoruri_n[j - 1];
							scoruri_n[i] = Scor;
							break;
						}
					for (int i = 0; i < 10; i++)
						fout << scoruri_n[i] << " ";
					fout.close();
					scoruri();
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
				int scor_curent;
				ifstream fin("scores.txt");
				for (int i = 0; i < 10; i++)
				{
					fin >> scor_curent;
					scoruri_n[i] = scor_curent;
				}
				fin.clear();
				fin.close();
				ofstream fout("scores.txt");
				for (int i = 0; i < 10; i++)
					if (Scor >= scoruri_n[i])
					{
						for (int j = 9; j > i; j--)
							scoruri_n[j] = scoruri_n[j - 1];
						scoruri_n[i] = Scor;
						break;
					}
				for (int i = 0; i < 10; i++)
					fout << scoruri_n[i] << " ";
				fout.close();
				scoruri();
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

void snakeVersus()
{

}

//meniu

void meniu()
{
	window.clear();
	int selectedIndex = 0;
	Font font;
	font.loadFromFile("arial.ttf");
	Text text[6];

	text[0].setFont(font);
	text[0].setColor(Color::Red);
	text[0].setString("Classic mode");
	text[0].setPosition(100, 100);

	text[1].setFont(font);
	text[1].setColor(Color::White);
	text[1].setString("Campaign mode");
	text[1].setPosition(100, 150);

	text[2].setFont(font);
	text[2].setColor(Color::White);
	text[2].setString("Versus mode");
	text[2].setPosition(100, 200);

	text[3].setFont(font);
	text[3].setColor(Color::White);
	text[3].setString("High Scores");
	text[3].setPosition(100, 250);

	text[4].setFont(font);
	text[4].setColor(Color::White);
	text[4].setString("Exit");
	text[4].setPosition(100, 300);

	text[5].setFont(font);
	text[5].setColor(Color::Magenta);
	text[5].setString("Snake 5.0");
	text[5].setPosition(250, 10);

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
			scoruri();
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
	font.loadFromFile("arial.ttf");
	Text text[8];

	text[0].setFont(font);
	text[0].setColor(Color::Magenta);
	text[0].setString("Select a map");
	text[0].setPosition(250, 10);

	text[1].setFont(font);
	text[1].setColor(Color::Red);
	text[1].setString("Map 0");
	text[1].setPosition(100, 50);

	text[2].setFont(font);
	text[2].setColor(Color::White);
	text[2].setString("Map 1");
	text[2].setPosition(100, 110);

	text[3].setFont(font);
	text[3].setColor(Color::White);
	text[3].setString("Map 2");
	text[3].setPosition(100, 170);

	text[4].setFont(font);
	text[4].setColor(Color::White);
	text[4].setString("Map 3");
	text[4].setPosition(100, 230);

	text[5].setFont(font);
	text[5].setColor(Color::White);
	text[5].setString("Map 4");
	text[5].setPosition(100, 290);

	text[6].setFont(font);
	text[6].setColor(Color::White);
	text[6].setString("Map 5");
	text[6].setPosition(100, 350);

	text[7].setFont(font);
	text[7].setColor(Color::White);
	text[7].setString("Map 6");
	text[7].setPosition(100, 410);

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
	font.loadFromFile("arial.ttf");
	Text text[8];

	text[0].setFont(font);
	text[0].setColor(Color::Magenta);
	text[0].setString("Select a map");
	text[0].setPosition(250, 10);

	text[1].setFont(font);
	text[1].setColor(Color::Red);
	text[1].setString("Map 0");
	text[1].setPosition(100, 50);

	text[2].setFont(font);
	text[2].setColor(Color::White);
	text[2].setString("Map 1");
	text[2].setPosition(100, 110);

	text[3].setFont(font);
	text[3].setColor(Color::White);
	text[3].setString("Map 2");
	text[3].setPosition(100, 170);

	text[4].setFont(font);
	text[4].setColor(Color::White);
	text[4].setString("Map 3");
	text[4].setPosition(100, 230);

	text[5].setFont(font);
	text[5].setColor(Color::White);
	text[5].setString("Map 4");
	text[5].setPosition(100, 290);

	text[6].setFont(font);
	text[6].setColor(Color::White);
	text[6].setString("Map 5");
	text[6].setPosition(100, 350);

	text[7].setFont(font);
	text[7].setColor(Color::White);
	text[7].setString("Map 6");
	text[7].setPosition(100, 410);

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

//scoruri

void scoruri()
{
	window.clear();
	Font font;
	font.loadFromFile("arial.ttf");
	Text scores[11];

	ifstream fin_scor("scores.txt");
	scores[0].setColor(Color::Magenta);
	scores[0].setFont(font);
	scores[0].setString("High Scores");
	scores[0].setPosition(250, 10);

	int scor;
	char nr[33], scor_char[33];

	for (int i = 1; i <= 10; i++)
	{
		fin_scor >> scor;
		_itoa_s(i, nr, 10);
		_itoa_s(scor, scor_char, 10);
		strcat_s(nr, ". ");
		strcat_s(nr, scor_char);
		scores[i].setColor(Color::White);
		scores[i].setFont(font);
		scores[i].setCharacterSize(25);
		scores[i].setString(nr);
		scores[i].setPosition(100, 35*i + 40);
	}

	int selected_menu = 0;

	while (window.isOpen())
	{
		for (int i = 0; i <= 10; i++)
			window.draw(scores[i]);
		window.display();
		window.clear();

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
	}

	if (selected_menu == 1)
		meniu();
	
}


int main()
{
	//initLabirint6();
	//snakeClassic();
	//snakeCampaign();
	meniu();
	return 0;
}