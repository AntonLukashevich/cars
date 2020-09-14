#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Box2D/Box2D.h>
#include "Pinball.h"

static const float SCALE = 30.f;
b2RevoluteJoint* m_leftJoint;
b2RevoluteJoint* m_rightJoint;

void CreateGround(b2World& World, float X, float Y, float Angle, float width, float length);

void CreateBall(b2World& World, float X, float Y);
void CreateFlipperLeft(b2World& World, b2Body* pinball);
void CreateFlipperRight(b2World& World, b2Body* pinball);
void CreateBonus_1(b2World& World, float X, float Y);

void CreateBox(b2World& World, int MouseX, int MouseY);

bool startGame(RenderWindow & window)
{
	bool exitGame = true;

	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);
	b2BodyDef BodyDef;
	b2Body* pinball = World.CreateBody(&BodyDef);

	////////////—тенки ограничивающие поле дл€ игры
	CreateGround(World, 300.f, .0f, .0f, 300.f, 1.f);
	CreateGround(World, 300.f, 600.f, .0f, 300.f, 1.f);
	CreateGround(World, 0.f, 300.0f, 1.57f, 300.f, 1.f);
	CreateGround(World, 379.f, 330.f, 1.57f, 300.f, 1.f);
	CreateGround(World, 400.f, 300.f, 1.57f, 300.f, 1.f);
	CreateGround(World, 65.5f, 465.5f, 1.57f / 2.0f, 95.f, 1.f); // 1.57 радиан - 90 градусов
	CreateGround(World, 334.5f, 465.5f, -1.57f / 2.0f, 95.f, 1.f);

	CreateGround(World, 392.f, 7.f, 1.57f / 2.0, 10.f, 1.f);
	CreateGround(World, 390.f, 400.f, 0, 10.f, 1.f);

	//CreateGround(World, 135.f, 300.f, 1.57f, 300.f, 1.f);

	CreateBall(World, 390.f, 390.f);
	CreateFlipperRight(World, pinball);
	CreateFlipperLeft(World, pinball);
	CreateBonus_1(World, 100.f, 100.f);
	CreateBonus_1(World, 280.f, 100.f);
	CreateBonus_1(World, 190.f, 200.f);

	Texture GroundTexture, GroundTexture2, GroundTexture3;
	Texture BonusTexture;
	Texture BoxTexture;
	Texture BallTexture, FlipperTexture;
	Texture Ground;
	GroundTexture.loadFromFile("images/ground.png");
	GroundTexture2.loadFromFile("images/ground2.png");
	GroundTexture3.loadFromFile("images/ground3.png");
	BonusTexture.loadFromFile("images/Bonus_1.png");
	BoxTexture.loadFromFile("images/box.png");
	BallTexture.loadFromFile("images/Ball.png");
	FlipperTexture.loadFromFile("images/fl.png");
	Ground.loadFromFile("images/11.png");

	Sprite ground(Ground);
	ground.setPosition(0, 0);

	bool buttonRight = false, buttonLeft = false;

	while (exitGame  && window.isOpen())
	{
		World.Step(1 / 60.f, 100, 50);
		Event event; // ѕровер€ем все событи€ окна, которые были вызваны с последней итерации цикла
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) // «акрываем окно, в случае получени€ управл€ющего событи€
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
			if (event.type == sf::Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Space))
			{
				Clock clock; // часы запускаютс€
				clock.restart();
				Time elapsed = clock.getElapsedTime();
				cout << elapsed.asSeconds() << endl;
				bool start = false;
				for (b2Body* BodyBall = World.GetBodyList(); BodyBall != 0; BodyBall = BodyBall->GetNext())
				{
					if (BodyBall->GetUserData() == "Ball" && 380 / SCALE < BodyBall->GetPosition().x && BodyBall->GetPosition().x < 400 / SCALE && 380 / SCALE < BodyBall->GetPosition().y && BodyBall->GetPosition().y < 400 / SCALE)
					{
						start = true;
					}
				}
				while (start)
				{
					window.pollEvent(event);
					cout << elapsed.asSeconds() << endl;
					if (event.type == Event::KeyReleased)
					{
						//int Pusk = elapsed;
						elapsed = clock.getElapsedTime();
						cout << elapsed.asSeconds() << endl;
						start = false;
						for (b2Body* BodyBall = World.GetBodyList(); BodyBall != 0; BodyBall = BodyBall->GetNext())
						{
							if (BodyBall->GetUserData() == "Ball")
							{
								float pusk = 0;
								for (Time t = seconds(0.f);t < elapsed;t += seconds(0.05f)) pusk += 0.5f;
								BodyBall->ApplyLinearImpulseToCenter(b2Vec2(0, -pusk), 1);
							}
						}
					}
					window.pollEvent(event);
				}
			}

		}



		if (Mouse::isButtonPressed(Mouse::Left))
		{
			int MouseX = Mouse::getPosition(window).x;
			int MouseY = Mouse::getPosition(window).y;
			CreateBox(World, MouseX, MouseY);
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			exitGame = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Z))
		{
			buttonLeft = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::X))
		{
			buttonRight = true;
		}

		if (buttonLeft)
		{
			m_leftJoint->SetMotorSpeed(-15.0f);
			sleep(seconds(0.001f));
		}
		else
		{
			m_leftJoint->SetMotorSpeed(25.0f);
			sleep(seconds(0.001f));
		}
		buttonLeft = false;

		if (buttonRight)
		{
			m_rightJoint->SetMotorSpeed(15.0f);
			sleep(seconds(0.001f));
		}
		else
		{
			m_rightJoint->SetMotorSpeed(-25.0f);
			sleep(seconds(0.001f));
		}
		buttonRight = false;


		window.clear(Color::White);
		window.draw(ground);
		int BodyCount = 0;
		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{

			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				if (BodyIterator->GetUserData() == "Ball")
				{
					if (BodyIterator->GetPosition().y > 580 / SCALE)
					{
						BodyIterator->SetTransform(b2Vec2(390.f / SCALE, 390.f / SCALE), 0.f);
					}
					Sprite Sprite;
					Sprite.setTexture(BallTexture);
					Sprite.setOrigin(9.f, 9.f);
					Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
					Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
					window.draw(Sprite);
				}
				else if (BodyIterator->GetUserData() == "RightFlipper")
				{
					Sprite Sprite;
					Sprite.setTexture(FlipperTexture);
					Sprite.setOrigin(55.f, 2.5f);
					Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
					Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
					window.draw(Sprite);
				}
				else if (BodyIterator->GetUserData() == "LeftFlipper")
				{
					Sprite Sprite;
					Sprite.setTexture(FlipperTexture);
					Sprite.setOrigin(55.f, 2.5f);
					Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
					Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
					window.draw(Sprite);
				}
				else
				{
					Sprite Sprite;
					Sprite.setTexture(BoxTexture);
					Sprite.setOrigin(16.f, 16.f);
					Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
					Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
					window.draw(Sprite);
				}
				++BodyCount;
			}
			else
			{
				Sprite GroundSprite;
				if (BodyIterator->GetPosition().x == 390 / SCALE || BodyIterator->GetPosition().x == 392 / SCALE)
				{
					GroundSprite.setTexture(GroundTexture2);
					GroundSprite.setOrigin(10.f, 1.f);
				}
				else if (BodyIterator->GetPosition().y == 465.5f / SCALE)
				{
					GroundSprite.setTexture(GroundTexture3);
					GroundSprite.setOrigin(95.f, 1.f);
				}
				else if (BodyIterator->GetUserData() == "BonusCircle")
				{
					GroundSprite.setTexture(BonusTexture);
					GroundSprite.setOrigin(25.f, 25.f);
				}
				else
				{
					GroundSprite.setTexture(GroundTexture);
					GroundSprite.setOrigin(300.f, 1.f);
				}
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				window.draw(GroundSprite);
			}
		}
		window.display();
	}

	return false;
}

void CreateBall(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2CircleShape Circle;
	Circle.m_radius = 9.f / SCALE;
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.restitution = 0.4f;
	//FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Circle;
	Body->CreateFixture(&FixtureDef);
	Body->SetUserData("Ball");
}

void CreateFlipperLeft(b2World& World, b2Body* pinball)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(132.f / SCALE, 538.f / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* leftFlipper = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(55.f / SCALE, 3.f / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.0f;
	FixtureDef.shape = &Shape;
	leftFlipper->CreateFixture(&FixtureDef);
	leftFlipper->SetUserData("LeftFlipper");

	b2RevoluteJointDef jd;
	jd.bodyA = pinball;
	jd.localAnchorB.SetZero();
	jd.enableMotor = true;
	jd.maxMotorTorque = 1000.0f;
	jd.enableLimit = true;

	jd.motorSpeed = 0.0f;
	jd.localAnchorA = b2Vec2(132.f / SCALE, 538.f / SCALE);
	jd.bodyB = leftFlipper;
	jd.lowerAngle = -15.0f * b2_pi / 180.0f;
	jd.upperAngle = 30.0f * b2_pi / 180.0f;
	m_leftJoint = (b2RevoluteJoint*)World.CreateJoint(&jd);
}

void CreateFlipperRight(b2World& World, b2Body* pinball)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(268.f / SCALE, 538.f / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* rightFlipper = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(55.f / SCALE, 3.f / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.0f;
	FixtureDef.shape = &Shape;
	rightFlipper->CreateFixture(&FixtureDef);
	rightFlipper->SetUserData("RightFlipper");

	b2RevoluteJointDef jd;
	jd.bodyA = pinball;
	jd.localAnchorB.SetZero();
	jd.enableMotor = true;
	jd.maxMotorTorque = 1000.0f;
	jd.enableLimit = true;

	jd.motorSpeed = 0.0f;
	jd.localAnchorA = b2Vec2(268.f / SCALE, 538.f / SCALE);
	jd.bodyB = rightFlipper;
	jd.lowerAngle = -30.0f * b2_pi / 180.0f;
	jd.upperAngle = 15.0f * b2_pi / 180.0f;
	m_rightJoint = (b2RevoluteJoint*)World.CreateJoint(&jd);
}

void CreateBox(b2World& World, int MouseX, int MouseY)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(MouseX / SCALE, MouseY / SCALE);
	BodyDef.type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.restitution = 0.f;
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

//////////ѕредмет/координата х/координата у/угол/ширина/длина
void CreateGround(b2World& World, float X, float Y, float Angle, float width, float length)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.angle = Angle;
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox(width / SCALE, length / SCALE);
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;
	FixtureDef.restitution = 0.f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}

void CreateBonus_1(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2CircleShape Circle;
	Circle.m_radius = 25.f / SCALE;
	b2FixtureDef FixtureDef;
	FixtureDef.density = 1.f;
	FixtureDef.restitution = 10.f;
	//FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Circle;
	Body->CreateFixture(&FixtureDef);
	Body->SetUserData("BonusCircle");
}