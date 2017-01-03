#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
using namespace std;

struct snake 
{
	int x=800/32, y=640/32;
}s[100];

struct food
{
	int x=8, y=10;
}f;
int directie = 2, lungime = 4;
void Rotire()
{
	for (int index = lungime; index>0; index--)
	{
		s[index].x = s[index - 1].x;
		s[index].y = s[index - 1].y;
	}
	if (directie == 0)
		s[0].y += 1;
	if (directie == 1)
		s[0].x -= 1;
	if (directie == 2)
		s[0].x += 1;
	if (directie == 3)
		s[0].y -= 1;
	if (s[0].x == f.x&&s[0].y == f.y)
	{
		lungime++;
		f.x = rand() % 800/16;
		f.y = rand() % 640/16;
	}
}
int main()
{
	srand(0);
	sf::RenderWindow window(sf::VideoMode(800, 640), "Snake v1.0");
	window.setFramerateLimit(60);
	sf::Texture t1, t2, t3;
	t1.loadFromFile("green.png");
	t2.loadFromFile("food.png");
	t3.loadFromFile("star.png");
	sf::Sprite sarpe(t1);
	sf::Sprite mancare(t2);
	sf::Sprite stea(t3);
	sf::Clock clock;
	float timer = 0, delay = 0.1;

	while (window.isOpen())
	{
		float timp = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += timp;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && directie != 2)
			directie = 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && directie != 1)
			directie = 2;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && directie != 0)
			directie = 3;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && directie != 3)
			directie = 0;

		if (timer > delay)
		{
			timer = 0;
			Rotire();
		}
		window.clear();

		for (int index = 0; index < lungime; index++)
		{
			sarpe.setPosition(s[index].x * 16, s[index].y * 16);
			window.draw(sarpe);
		}
		mancare.setPosition(f.x*16, f.y*16);
		window.draw(mancare);
		window.display();
	}

	return 0;
}