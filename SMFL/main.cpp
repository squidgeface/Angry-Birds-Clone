#include "utils.h"

void CreateGround(b2World& World, float X, float Y);
void CreateBox(b2World& World, int MouseX, int MouseY); // Spawns a box at MouseX, MouseY
const float SCALE = 30.0f;

int main()
{

	RenderWindow window(VideoMode(800, 600, 32), "SFML works!");
	window.setFramerateLimit(60);
		
	b2Vec2 Gravity(0.f, 9.8f);
	b2World World(Gravity);

	CreateGround(World, 400.f, 500.f);

	sf::Texture GroundTexture;
	sf::Texture BoxTexture;
	GroundTexture.loadFromFile("Resources/Textures/ground.png");
	BoxTexture.loadFromFile("Resources/Textures/box.png");



	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			int MouseX = sf::Mouse::getPosition(window).x;
			int MouseY = sf::Mouse::getPosition(window).y;
			CreateBox(World, MouseX, MouseY);
		}

		/** Simulate the world */
		World.Step(1 / 60.f, 8, 3);
		
		window.clear(Color::White);

		for (b2Body* BodyIterator = World.GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
		{
			if (BodyIterator->GetType() == b2_dynamicBody)
			{
				sf::Sprite Sprite;
				Sprite.setTexture(BoxTexture);
				Sprite.setOrigin(16.f, 16.f);
				Sprite.setPosition(SCALE * BodyIterator->GetPosition().x, SCALE * BodyIterator->GetPosition().y);
				Sprite.setRotation(BodyIterator->GetAngle() * 180 / b2_pi);
				window.draw(Sprite);
			}
			else
			{
				sf::Sprite GroundSprite;
				GroundSprite.setTexture(GroundTexture);
				GroundSprite.setOrigin(400.f, 8.f);
				GroundSprite.setPosition(BodyIterator->GetPosition().x * SCALE, BodyIterator->GetPosition().y * SCALE);
				GroundSprite.setRotation(180 / b2_pi * BodyIterator->GetAngle());
				window.draw(GroundSprite);
			}
		}
		
	
		window.display();
	}

	return 0;
}

void CreateGround(b2World& World, float X, float Y)
{
	

	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / 30.f, Y / 30.f);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World.CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE); // Creates a box shape. Divide your desired width and height by 2.
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;  // Sets the density of the body
	FixtureDef.shape = &Shape; // Sets the shape
	Body->CreateFixture(&FixtureDef); // Apply the fixture definition
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
	FixtureDef.friction = 0.7f;
	FixtureDef.shape = &Shape;
	Body->CreateFixture(&FixtureDef);
}