#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <windows.h> 
#include "Menu.h"
#include "Pinball.h"

using namespace sf;

/** We need this to easily convert between pixel and real-world coordinates*/
static const float SCALE = 30.f;

int main()
{
	//ShowWindow(GetConsoleWindow(), SW_HIDE); //�������� ���������� ����
	RenderWindow window(VideoMode(600, 600), "Mega-Gaming"); 
	window.setFramerateLimit(100);
	
	startGame(window);
	menu(window);

	//while (window.isOpen())
	//{
	//	Event event; // ��������� ��� ������� ����, ������� ���� ������� � ��������� �������� �����
	//	while (window.pollEvent(event))
	//	{
	//		if (event.type == Event::Closed) // ��������� ����, � ������ ��������� ������������ �������
	//			window.close();
	//	}
	//	window.clear();
	//	window.display();
	//}

	return 0;
}