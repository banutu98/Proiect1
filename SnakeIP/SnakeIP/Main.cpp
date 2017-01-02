#include <SFML/Graphics.hpp>
#include <iostream>
#include <time.h>
using namespace std;
struct snake {
	int x, y;
}s[100];
int directie = 0, lungime = 4;
void Rotire()
{
	for (int index = lungime; index>0; index--)
	{
		s[index].x = s[index - 1].x;
		s[index].y = s[index - 1].y;
	}
	if (directie == 0)
		s[0].y += 1;
}
int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 640), "Snake v1.0");
	sf::Texture t;
	t.loadFromFile("white.png");
	sf::Sprite sprite(t);
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

		if (timer > delay)
		{
			timer = 0;
			Rotire();
		}
		window.clear();

		for (int index = 0; index < lungime; index++)
		{
			sprite.setPosition(s[index].x * 16, s[index].y * 16);
			window.draw(sprite);
		}
		window.display();
	}

	return 0;
}