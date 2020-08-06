#include "GameManager.h"

void CGameManager::InitialiseWindow()
{
	window = new RenderWindow(VideoMode(800, 600, 32), "Grumpy Birds");
	window->setFramerateLimit(60);
}


void CGameManager::Update()
{
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);

	CreateGround(World, 400.f, 600.0f);

	Texture GroundTexture;
	GroundTexture.loadFromFile("Resources/Textures/ground.png");

	Texture GrumpyBird;
	GrumpyBird.loadFromFile("Resources/Textures/grumpybird.png");

	float boxSize = 41.0f;
	int MousePX;
	int MousePY;
	int MouseRX;
	int MouseRY;
	bool isFired = false;
	bool reset = true;

	CreateObject(World, 50, 50, 40, 460, "Resources/Textures/grumpybird.png", 0.5f, 0.5f);
	Bodies[0]->SetEnabled(false);
	CreateObject(World, boxSize, boxSize, 400, 580, "Resources/Textures/box.png", 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 400, 580, "Resources/Textures/box.png", 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 480, 560, "Resources/Textures/box.png", 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 480, 560, "Resources/Textures/box.png", 0.5f, 0.5f);

	//plank #5
	CreateObject(World, 100, 10, 440, 520, "Resources/Textures/plank.png");

	CreateObject(World, boxSize, boxSize, 400, 500, "Resources/Textures/box.png", 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 400, 500, "Resources/Textures/box.png", 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 480, 480, "Resources/Textures/box.png", 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 480, 480, "Resources/Textures/box.png", 0.5f, 0.5f);

	//plank #10
	CreateObject(World, 100, 10, 440, 460, "Resources/Textures/plank.png");

	//Arrow sprite
	Texture aBox;
	aBox.loadFromFile("Resources/Textures/arrow.png");
	Sprite arrow;
	arrow.setTexture(aBox);
	arrow.setOrigin(10.0f, 457.0 / 2);
	arrow.setPosition(40.0f, 460.0f);
	arrow.scale(0.0f, 0.0f);
	

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
			if (!isPressed)
			{
				isPressed = true;
				MousePX = Mouse::getPosition(*window).x;
				MousePY = Mouse::getPosition(*window).y;
			}

			if (reset)
			{
				vec2 forceVec = vec2(Mouse::getPosition(*window).x - MousePX, Mouse::getPosition(*window).y - MousePY);
				forceVec = normalize(forceVec);
				arrow.setScale(0.1f, 0.1f);
				arrow.setRotation(atan2f(forceVec.y, forceVec.x) * (180.0f / float(PI)) - 180.0f);
				arrow.setScale(Distancev2(vec2(MousePX, MousePY), vec2(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)) / 1000.0f, 0.1f);
			}
		}
		else if (isPressed)
		{
			isPressed = false;
			MouseRX = Mouse::getPosition(*window).x;
			MouseRY = Mouse::getPosition(*window).y;

			arrow.scale(0.0f, 0.0f);

			if (reset)
				isFired = true;
		}

		/*if (Bodies[0]->GetFixtureList() == Bodies[6]->GetFixtureList() || Bodies[0]->GetFixtureList() == Bodies[6]->GetFixtureList())
		{
			isFired = true;
		}*/
		
			
		

		if (isFired)
		{
			vec2 forceVec = vec2(MouseRX - MousePX, MouseRY - MousePY);
			forceVec = normalize(forceVec);

			float distance = Distancev2(vec2(MousePX, MousePY), vec2(MouseRX, MouseRY));

			forceVec *= -distance * 2;

			Bodies[0]->SetEnabled(true);
			Bodies[0]->ApplyForceToCenter(b2Vec2(forceVec.x, forceVec.y), true);

			isFired = false;
			reset = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Q))
		{
			Bodies[0]->SetTransform(b2Vec2(40/SCALE, 460/SCALE), 0.0f);
			Bodies[0]->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			Bodies[0]->SetAngularVelocity(0.0f);
			Bodies[0]->SetEnabled(false);
			reset = true;
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

	
		window->draw(arrow);

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
