#include <iostream>
#include <dos.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Pinball.h"

bool SoundP = true;//переменная для произведение звука
int volume = 0;//регулеровка музыки
Music musicGround;//создаем объект музыки

void menuTablicaRecords(RenderWindow & window, Sound tap);
void menuSettings(RenderWindow & window, Sound tap);
void playSound(Sound tap);
void playMusic();

bool menu(RenderWindow & window)
{
	SoundBuffer tapBuffer; //создаём буфер для звука
	tapBuffer.loadFromFile("tik.ogg"); //загружаем в него звук
	Sound tap(tapBuffer); //создаем звук и загружаем в него звук из буфера

	musicGround.openFromFile("musicGround.ogg");//загружаем файл
	musicGround.play();//воспроизводим музыку
	musicGround.setLoop(true);//зацикливаем музыку
	playMusic();

	setlocale(LC_ALL, "rus"); //Для проверки (Отключить НАДО!!!!!!!!!)
	Texture menuTextureGround, menuTextureStart, menuTextureSettings, menuTextureTablicaRecords, menuTextureExit;
	menuTextureGround.loadFromFile("images/Menu/Ground.png"); //Фон
	menuTextureStart.loadFromFile("images/Menu/NewGame.png"); //Кнопка - "Новая игра"
	menuTextureSettings.loadFromFile("images/Menu/Settings.png"); //Кнопка - "Настройки"
	menuTextureTablicaRecords.loadFromFile("images/Menu/TablicaRecords.png"); //Кнопка - "Таблица рекордов"
	menuTextureExit.loadFromFile("images/Menu/Exit.png"); //Кнопка - "Выход"
	Sprite menuSta(menuTextureStart), menuTab(menuTextureTablicaRecords), menuSet(menuTextureSettings), menuEx(menuTextureExit), ground(menuTextureGround);
	menuSta.setPosition(150, 125);//Установка начальной позиции кнопки "Новая Игра"
	menuTab.setPosition(125, 200);//Установка начальной позиции кнопки "Таблица рекордов"
	menuSet.setPosition(150, 275);//Установка начальной позиции кнопки "Настройки"
	menuEx.setPosition(150, 350);//Установка начальной позиции кнопки "Выход"
	ground.setPosition(0, 0);

	bool exitMenu = true;//Переменная для выхода из цикла меню - выход из игры
	int numberMenu = -1;//Переменная для прослеживания выбранного пункта

	while (exitMenu && window.isOpen())//Цикл отрисовки и взаимодействия с меню
	{
		Event event; // Проверяем все события окна, которые были вызваны с последней итерации цикла
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // Закрываем окно, в случае получения управляющего события
				window.close();
			if (event.type == sf::Event::LostFocus)
			{
				bool pause = true;
				while (pause)
				{
					window.pollEvent(event);
					if (event.type == sf::Event::GainedFocus) pause = false;
				}
			}
		}

		menuSta.setColor(Color::White);//Установка  цвета кнопки "Новая игра"
		menuTab.setColor(Color::White);//Установка цвета кнопки "Таблица рекордов"
		menuSet.setColor(Color::White);//Установка цвета кнопки "Настройки"
		menuEx.setColor(Color::White);//Установка цвета кнопки "Выход"

		//Проверка на нахождения мыши на поле. При нахождении мышки на одной из кнопок, кнопка перекрашивается в красный
		if (IntRect(0, 0, 600, 600).contains(Mouse::getPosition(window))) { numberMenu = -1; }
		if (IntRect(190, 125, 220, 50).contains(Mouse::getPosition(window))) { menuSta.setColor(Color::Red); numberMenu = 1; }
		if (IntRect(130, 200, 340, 50).contains(Mouse::getPosition(window))) { menuTab.setColor(Color::Red); numberMenu = 2; }
		if (IntRect(190, 275, 220, 50).contains(Mouse::getPosition(window))) { menuSet.setColor(Color::Red); numberMenu = 3; }
		if (IntRect(240, 350, 120, 50).contains(Mouse::getPosition(window))) { menuEx.setColor(Color::Red); numberMenu = 0; }

		if (Mouse::isButtonPressed(Mouse::Left)) //проверка на нажатие одной из кнопок
		{
			if (numberMenu != -1) playSound(tap);
			if (numberMenu == 1) { cout << "Обломись"; startGame(window); }
			if (numberMenu == 2) { cout << "Таблица хуйня"; menuTablicaRecords(window, tap); }
			if (numberMenu == 3) { menuSettings(window, tap); }
			if (numberMenu == 0) { sleep(seconds(0.1f)); exitMenu = false; }
			numberMenu = -1;
		}

		//Отрисовка кнопок и фона
		window.draw(ground);
		window.draw(menuSta);
		window.draw(menuTab);
		window.draw(menuSet);
		window.draw(menuEx);
		window.display();
	}
	return false;
}

void menuTablicaRecords(RenderWindow &window, Sound tap)
{
	Texture menuTextureGround, menuTextureBack;
	menuTextureGround.loadFromFile("images/MenuTablica/Ground.png");
	menuTextureBack.loadFromFile("images/MenuTablica/Back.png");
	Sprite ground(menuTextureGround), menuBack(menuTextureBack);
	ground.setPosition(0, 0);
	menuBack.setPosition(15, 540);

	bool exitMenuTablicaRecords = true;
	int numberMenuSound = -1;

	while (exitMenuTablicaRecords  && window.isOpen())
	{
		Event event; // Проверяем все события окна, которые были вызваны с последней итерации цикла
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // Закрываем окно, в случае получения управляющего события
				window.close();
			if (event.type == sf::Event::LostFocus)
			{
				bool pause = true;
				while (pause)
				{
					window.pollEvent(event);
					if (event.type == sf::Event::GainedFocus) pause = false;
				}
			}
		}

		ground.setColor(Color::White);
		menuBack.setColor(Color::White);

		if (IntRect(0, 0, 600, 600).contains(Mouse::getPosition(window))) { numberMenuSound = -1; }
		if (IntRect(15, 540, 150, 50).contains(Mouse::getPosition(window))) { menuBack.setColor(Color::Red); numberMenuSound = 0; }
		
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (numberMenuSound != -1) playSound(tap);
			if (numberMenuSound == 0) { exitMenuTablicaRecords = false; }
			numberMenuSound = -1;
		}

		window.draw(ground);
		window.draw(menuBack);
		window.display();
	}
}

void menuSettings(RenderWindow &window, Sound tap)
{
	Texture menuTextureGround, menuTextureSoundText, menuTextureSoundOn, menuTextureSoundOff, menuTextureMusicText, menuTextureMusicUp, menuTextureMusicOn, menuTextureMusicOff, menuTextureMusicDown, menuTextureBack;
	menuTextureGround.loadFromFile("images/MenuSound/Ground.png");
	menuTextureSoundOn.loadFromFile("images/MenuSound/SoundOn.png");
	menuTextureSoundOff.loadFromFile("images/MenuSound/SoundOff.png");
	menuTextureMusicUp.loadFromFile("images/MenuSound/Up.png");
	menuTextureMusicOn.loadFromFile("images/MenuSound/MusicOn.png");
	menuTextureMusicOff.loadFromFile("images/MenuSound/MusicOff.png");
	menuTextureMusicDown.loadFromFile("images/MenuSound/Down.png");
	menuTextureBack.loadFromFile("images/MenuSound/Back.png");
	menuTextureMusicText.loadFromFile("images/MenuSound/MusicText.png");
	menuTextureSoundText.loadFromFile("images/MenuSound/SoundText.png");
	Sprite ground(menuTextureGround), menuSOn(menuTextureSoundOn), menuSOff(menuTextureSoundOff), menuMUp(menuTextureMusicUp), menuMDown(menuTextureMusicDown), menuST(menuTextureSoundText), menuMT(menuTextureMusicText), menuBack(menuTextureBack);
	ground.setPosition(0, 0);
	menuST.setPosition(75, 200);
	menuMT.setPosition(75, 250);
	menuSOn.setPosition(340, 200);
	menuSOff.setPosition(340, 200);
	menuMDown.setPosition(255, 250);
	menuMUp.setPosition(425, 250);
	menuBack.setPosition(15, 540);

	bool exitMenuSettings = true;
	int numberMenuSettings = -1;

	while (exitMenuSettings  && window.isOpen())
	{
		Event event; // Проверяем все события окна, которые были вызваны с последней итерации цикла
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // Закрываем окно, в случае получения управляющего события
				window.close();
			if (event.type == sf::Event::LostFocus)
			{
				bool pause = true;
				while (pause)
				{
					window.pollEvent(event);
					if (event.type == sf::Event::GainedFocus) pause = false;
				}
			}
		}

		ground.setColor(Color::White);
		menuST.setColor(Color::White);
		menuMT.setColor(Color::White);
		menuSOn.setColor(Color::White);
		menuSOff.setColor(Color::White);
		menuMUp.setColor(Color::White);
		menuMDown.setColor(Color::White);
		menuBack.setColor(Color::White);

		if (IntRect(0, 0, 600, 600).contains(Mouse::getPosition(window))) { numberMenuSettings = -1; }
		if (IntRect(340, 200, 50, 50).contains(Mouse::getPosition(window))) { menuSOn.setColor(Color::Red); menuSOff.setColor(Color::Red); numberMenuSettings = 1; }
		if (IntRect(15, 540, 150, 50).contains(Mouse::getPosition(window))) { menuBack.setColor(Color::Red); numberMenuSettings = 0; }
		if (IntRect(265, 250, 25, 50).contains(Mouse::getPosition(window))) { menuMDown.setColor(Color::Red); numberMenuSettings = 2; }
		if (IntRect(435, 250, 25, 50).contains(Mouse::getPosition(window))) { menuMUp.setColor(Color::Red); numberMenuSettings = 3; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			
			if (numberMenuSettings != -1) playSound(tap);
			if (numberMenuSettings == 0) { exitMenuSettings = false; }
			if (numberMenuSettings == 1) {
				if (SoundP == true) { SoundP = false; }
				else { SoundP = true; }
			}
			if (numberMenuSettings == 2 && volume != 0) { volume--; playMusic(); }
			if (numberMenuSettings == 3 && volume != 5) { volume++; playMusic(); }
			numberMenuSettings = -1;
		}

		window.draw(ground);
		window.draw(menuST);
		window.draw(menuMT);
		window.draw(menuMUp);
		window.draw(menuMDown);
		window.draw(menuBack);

		if (SoundP == true) window.draw(menuSOn);
		else window.draw(menuSOff);
		float start = 300;
		for (int i = 0; i < volume;i++, start += 25.0)
		{
			Sprite  menuMOn(menuTextureMusicOn);
			menuMOn.setPosition(start, 250);
			window.draw(menuMOn);
		}
		for (int i = volume; i < 5;i++, start += 25.0)
		{
			Sprite  menuMOff(menuTextureMusicOff);
			menuMOff.setPosition(start, 250);
			window.draw(menuMOff);
		}

		window.display();
	}
}

void playSound(Sound tap)
{
	if (SoundP == true) tap.play();
	sleep(seconds(0.2f));
}

void playMusic()
{
	if (volume == 0) musicGround.pause();
	if (volume != 0 && musicGround.getStatus() == 1) musicGround.play();
	if (volume != 0) musicGround.setVolume(volume * 20.0f);
}