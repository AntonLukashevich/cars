#pragma once
// Здесь начинается header guard.  ADD_H – любое уникальное имя. Обычно используют имя заголовочного файла в качестве этого идентификатора.
#ifndef PINBALL_H
#define PINBALL_H

using namespace std;
using namespace sf;

// А это уже контент файла .h, где и находятся все предварительные объявления
bool startGame(RenderWindow & window); // прототип функции

#endif // Конец header guard