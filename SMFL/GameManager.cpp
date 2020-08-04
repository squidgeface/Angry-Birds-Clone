#include "GameManager.h"

void CGameManager::InitialiseWindow()
{
	window = new RenderWindow(VideoMode(800, 600, 32), "SFML works!");
	window->setFramerateLimit(60);
}


void CGameManager::Update()
{
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);

	CreateGround(World, 400.f, 600.0f);

	Texture GroundTexture;
	Texture BoxTexture;
	Texture PlankTexture;
	GroundTexture.loadFromFile("Resources/Textures/ground.png");
	BoxTexture.loadFromFile("Resources/Textures/box.png");
	PlankTexture.loadFromFile("Resources/Textures/plank.png");

	float boxSize = 41.0f;


	CreateObject(World, boxSize, boxSize, 400, 450, "Resources/Textures/box.png");
	CreateObject(World, boxSize, boxSize, 400, 420, "Resources/Textures/box.png");
	//CreateObject(World, boxSize, boxSize, 430, 450, "Resources/Textures/box.png");
	//CreateObject(World, boxSize, boxSize, 430, 420, "Resources/Textures/box.png");
	CreateObject(World, 100, 10, 450, 380, "Resources/Textures/plank.png", 0.1, 10);

	while (window->isOpen())
	{
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			
			int MouseX = Mouse::getPosition(*window).x;
			int MouseY = Mouse::getPosition(*window).y;
			
		}
		else
		{

		}

		/** Simulate the world */
		World.Step(1 / 60.f, 8, 3);

		window->clear(Color::White);

		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			
			if (BodyIterator->GetType() == b2_staticBody)
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(993.0 / 2, 44.0f /2);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				window->draw(GroundSprite);
			}
			
		}

		for (size_t i = 0; i < Sprites.size(); i++)
		{
			Sprites[i]->setPosition(SCALE * Bodies[i]->GetPosition().x, Bodies[i]->GetPosition().y * SCALE);
			Sprites[i]->setRotation(Bodies[i]->GetAngle() * 180 / b2_pi);
			window->draw(*Sprites[i]);
		}

		window->display();
	}
}

void CGameManager::CreateGround(b2World& World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((993.0f / 2) / SCALE, (44.0f / 2) / SCALE); // Creates a box shape. Divide your desired width and height by 2.
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;  // Sets the density of the body
	FixtureDef.shape = &Shape; // Sets the shape
	Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}

void CGameManager::CreateObject(b2World& World, float SizeX, float SizeY, float PosX, float PosY, String texPath, float _scaleX, float _scaleY)
{
	Texture* texture = new Texture;

	texture->loadFromFile(texPath);
	
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(PosX / SCALE, PosY / SCALE);
	BodyDef->type = b2_dynamicBody;
	b2Body* Body = World.CreateBody(BodyDef);

	b2PolygonShape* Shape = new b2PolygonShape;
	Shape->SetAsBox(((SizeX /2) * _scaleX) / SCALE, ((SizeY/2) * _scaleY) / SCALE);
	b2FixtureDef* FixtureDef = new b2FixtureDef;
	FixtureDef->density = 1.f;
	FixtureDef->friction = 0.7f;
	FixtureDef->shape = Shape;

	Body->CreateFixture(FixtureDef);

	sf::Sprite* sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(SizeX / 2, SizeY / 2);
	sprite->setScale(_scaleX, _scaleY);

	Bodies.push_back(Body);
	Sprites.push_back(sprite);
}

void CGameManager::UpdatePhysicsShapes()
{

}

bool CGameManager::GetWindowOpen()
{
	return window->isOpen();
}
