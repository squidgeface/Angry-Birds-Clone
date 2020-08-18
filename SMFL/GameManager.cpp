#include "GameManager.h"

void CGameManager::InitialiseWindow()
{
	window = new RenderWindow(VideoMode(utils::ScreenWidth, utils::ScreenHeight, 32), "Grumpy Birds");
	window->setFramerateLimit(60);
}

void CGameManager::InitialiseWorld()
{
	Gravity = new b2Vec2(0.f, 9.8f);
	World = new b2World(*Gravity);
	m_pContactListener = new b2ContactListener;
	World->SetContactListener(m_pContactListener);
}


void CGameManager::Update()
{
	//create ground
	CreateGround(World, 400.f, 600.0f);

	//create ceiling and joint objects
	CreateObject(World, utils::ScreenWidth, 25.0f, 400, 20, "Resources/Textures/ground.png", BShape::BOX, 1.0f, 1.0f, b2_staticBody);
	CreateObject(World, 100, 10, 100, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[0], Bodies[1], b2Vec2(Bodies[0]->GetPosition().x, Bodies[0]->GetPosition().y + 1.0f), b2Vec2(Bodies[1]->GetPosition().x - 50.0f/SCALE, Bodies[1]->GetPosition().y), BJoint::DIST);
	CreateObject(World, 100, 10, 100, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[1], Bodies[2], b2Vec2(Bodies[1]->GetPosition().x + 50.0f / SCALE, Bodies[1]->GetPosition().y), b2Vec2(Bodies[2]->GetPosition().x - 25.0f / SCALE, Bodies[2]->GetPosition().y), BJoint::REVO);
	CreateObject(World, 100, 10, 100, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[2], Bodies[3], b2Vec2(Bodies[2]->GetPosition().x - 50.0f / SCALE, Bodies[2]->GetPosition().y), b2Vec2(), BJoint::REVO);

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
	bool overlap = false;

	//Create Grumpy Bird
	CreateBird();
	BirdBody->SetEnabled(false);
	BirdBody->SetSleepingAllowed(false);

	//Create All platforms and obstacles
	CreateObject(World, boxSize, boxSize, 400, 580, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 400, 580, "Resources/Textures/box.png",BShape::BOX,0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 480, 560, "Resources/Textures/box.png",BShape::BOX,0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 480, 560, "Resources/Textures/box.png",BShape::BOX,0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 400, 500, "Resources/Textures/box.png",BShape::BOX,0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 400, 500, "Resources/Textures/box.png",BShape::BOX,0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 480, 480, "Resources/Textures/box.png",BShape::BOX,0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 480, 480, "Resources/Textures/box.png",BShape::BOX,0.5f, 0.5f);
	
	//Destructable planks
	CreateDestructable(World, 100, 10, 440, 520, "Resources/Textures/plank.png");
	CreateDestructable(World, 100, 10, 440, 460, "Resources/Textures/plank.png");

	//Destructable enemy
	CreateDestructable(World, 50, 50, 440, 500, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
	CreateDestructable(World, 50, 50, 440, 440, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
	CreateDestructable(World, 50, 50, 440, 540, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);

	//Arrow sprite
	Texture aBox;
	aBox.loadFromFile("Resources/Textures/arrow.png");
	Sprite arrow;
	arrow.setTexture(aBox);
	arrow.setOrigin(10.0f, 457.0 / 2);
	arrow.setPosition(40.0f, 460.0f);
	arrow.scale(0.0f, 0.0f);

	//Game update
	while (window->isOpen())
	{
		//Check for player and destructable object collision
		for (size_t i = 0; i < DestBodies.size(); i++)
		{
			if (World->GetContactList() != NULL && BirdBody->GetContactList() != NULL && DestBodies[i]->GetContactList() != NULL)
			{
				if (BirdBody->GetContactList()->contact->GetFixtureA()->GetBody() == DestBodies[i] && BirdBody->GetContactList()->contact->IsTouching() || DestBodies[i]->GetContactList()->contact->GetFixtureA()->GetBody() == BirdBody && DestBodies[i]->GetContactList()->contact->IsTouching())
				{
					World->DestroyBody(DestBodies[i]);
					DestBodies.erase(DestBodies.begin() + i);
					DestSprites.erase(DestSprites.begin() + i);
					break;
				}
				//World->GetContactList()->GetFixtureA()->GetBody()->GetContactList()
				if (BeginContact(World->GetContactList(), DestBodies[i]))
				{
					World->DestroyBody(DestBodies[i]);
					DestBodies.erase(DestBodies.begin() + i);
					DestSprites.erase(DestSprites.begin() + i);
					break;
				}
			}
		}

		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				window->close();
		}

		//Show direction vector when mouse is held and dragged
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
		

		if (isFired)
		{
			vec2 forceVec = vec2(MouseRX - MousePX, MouseRY - MousePY);
			forceVec = normalize(forceVec);

			float distance = Distancev2(vec2(MousePX, MousePY), vec2(MouseRX, MouseRY));

			forceVec *= -distance * 2;

			BirdBody->SetEnabled(true);
			BirdBody->ApplyForceToCenter(b2Vec2(forceVec.x, forceVec.y), true);

			isFired = false;
			reset = false;
		}

		if (Keyboard::isKeyPressed(Keyboard::Q))
		{
			BirdBody->SetTransform(b2Vec2(40/SCALE, 460/SCALE), 0.0f);
			BirdBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			BirdBody->SetAngularVelocity(0.0f);
			BirdBody->SetEnabled(false);
			reset = true;
		}
	

		/** Simulate the world */
		World->Step(1 / 60.f, 8, 3);

		window->clear(Color::White);

		for (b2Body* BodyIterator = World->GetBodyList(); BodyIterator != 0; BodyIterator = BodyIterator->GetNext())
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
		
		for (size_t i = 0; i < DestSprites.size(); i++)
		{
			DestSprites[i]->setPosition(SCALE * DestBodies[i]->GetPosition().x, DestBodies[i]->GetPosition().y * SCALE);
			DestSprites[i]->setRotation(DestBodies[i]->GetAngle() * 180 / b2_pi);
			window->draw(*DestSprites[i]);
		}

		BirdSprite->setPosition(SCALE* BirdBody->GetPosition().x, BirdBody->GetPosition().y* SCALE);
		BirdSprite->setRotation(BirdBody->GetAngle() * 180 / b2_pi);
		window->draw(*BirdSprite);
		if (BirdBody->GetLinearVelocity().x > 20.0f)
		{
			BirdBody->SetLinearVelocity(b2Vec2(20.0f, BirdBody->GetLinearVelocity().y));
		}
	
		window->draw(arrow);

		window->display();

		//Check for player and destructable object collision
		for (size_t i = 0; i < DestBodies.size(); i++)
		{
			if (World->GetContactList() != NULL && BirdBody->GetContactList() != NULL && DestBodies[i]->GetContactList() != NULL)
			{
				if (BirdBody->GetContactList()->contact->GetFixtureA()->GetBody() == DestBodies[i] && BirdBody->GetContactList()->contact->IsTouching() || DestBodies[i]->GetContactList()->contact->GetFixtureA()->GetBody() == BirdBody && DestBodies[i]->GetContactList()->contact->IsTouching())
				{
					World->DestroyBody(DestBodies[i]);
					DestBodies.erase(DestBodies.begin() + i);
					DestSprites.erase(DestSprites.begin() + i);
					break;
				}
				//World->GetContactList()->GetFixtureA()->GetBody()->GetContactList()
				if (BeginContact(World->GetContactList(), DestBodies[i]))
				{
					World->DestroyBody(DestBodies[i]);
					DestBodies.erase(DestBodies.begin() + i);
					DestSprites.erase(DestSprites.begin() + i);
					break;
				}
			}
		}
	}
}

void CGameManager::CreateGround(b2World* World, float X, float Y)
{
	b2BodyDef BodyDef;
	BodyDef.position = b2Vec2(X / SCALE, Y / SCALE);
	BodyDef.type = b2_staticBody;
	b2Body* Body = World->CreateBody(&BodyDef);

	b2PolygonShape Shape;
	Shape.SetAsBox((utils::HSWidth) / SCALE, (25.0f) / SCALE); // Creates a box shape. Divide your desired width and height by 2.
	b2FixtureDef FixtureDef;
	FixtureDef.density = 0.f;  // Sets the density of the body
	FixtureDef.shape = &Shape; // Sets the shape
	Body->CreateFixture(&FixtureDef); // Apply the fixture definition
}

void CGameManager::CreateObject(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, BShape _shape, float _scaleX, float _scaleY, b2BodyType _bodyType)
{
	Texture* texture = new Texture;

	texture->loadFromFile(texPath);
	
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(PosX / SCALE, PosY / SCALE);
	BodyDef->type = _bodyType;
	b2Body* Body = World->CreateBody(BodyDef);

	b2FixtureDef* FixtureDef = new b2FixtureDef;
	FixtureDef->density = 1.f;
	FixtureDef->friction = 0.7f;

	if (_shape == BShape::BOX)
	{
		b2PolygonShape* Shape = new b2PolygonShape;
		Shape->SetAsBox(((SizeX / 2) * _scaleX) / SCALE, ((SizeY / 2) * _scaleY) / SCALE);
		FixtureDef->shape = Shape;
	}
	else 	if (_shape == BShape::CIRCLE)
	{
		b2CircleShape* Shape = new b2CircleShape;
		Shape->m_radius = SizeX / 2 * _scaleX;
		FixtureDef->shape = Shape;
	}

	Body->CreateFixture(FixtureDef);

	sf::Sprite* sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(SizeX / 2, SizeY / 2);
	sprite->setScale(_scaleX, _scaleY);

	Bodies.push_back(Body);
	Sprites.push_back(sprite);
}

void CGameManager::CreateBird()
{
	Texture* texture = new Texture;

	texture->loadFromFile("Resources/Textures/grumpybird.png");

	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(40 / SCALE, 460 / SCALE);
	BodyDef->type = b2_dynamicBody;
	b2Body* Body = World->CreateBody(BodyDef);

	b2CircleShape* Shape = new b2CircleShape;
	Shape->m_radius = (50 / 4) / SCALE;
	b2FixtureDef* FixtureDef = new b2FixtureDef;
	FixtureDef->density = 1.f;
	FixtureDef->friction = 0.7f;
	FixtureDef->shape = Shape;

	Body->CreateFixture(FixtureDef);

	sf::Sprite* sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(25, 25);
	sprite->setScale(0.5, 0.5);

	BirdSprite = sprite;
	BirdBody = Body;
}

void CGameManager::CreateDestructable(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, float _scaleX, float _scaleY, BShape _shape)
{
	Texture* texture = new Texture;

	texture->loadFromFile(texPath);

	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(PosX / SCALE, PosY / SCALE);
	BodyDef->type = b2_dynamicBody;
	b2Body* Body = World->CreateBody(BodyDef);
	b2FixtureDef* FixtureDef = new b2FixtureDef;

	if (_shape == BShape::BOX)
	{
		b2PolygonShape* Shape = new b2PolygonShape;
		Shape->SetAsBox(((SizeX / 2) * _scaleX) / SCALE, ((SizeY / 2) * _scaleY) / SCALE);
		FixtureDef->shape = Shape;
	}
	else 	if (_shape == BShape::CIRCLE)
	{
		b2CircleShape* Shape = new b2CircleShape;
		Shape->m_radius = SizeX / 2 * _scaleX / SCALE;
		FixtureDef->shape = Shape;
	}
	
	
	FixtureDef->density = 1.f;
	FixtureDef->friction = 0.7f;
	

	string* name = new string("Platform");

	Body->CreateFixture(FixtureDef)->SetUserData(name);

	sf::Sprite* sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(SizeX / 2, SizeY / 2);
	sprite->setScale(_scaleX, _scaleY);

	DestBodies.push_back(Body);
	DestSprites.push_back(sprite);
	
}


bool CGameManager::GetWindowOpen()
{
	return window->isOpen();
}

bool CGameManager::BeginContact(b2Contact* contact, b2Body* platform)
{
		if ((contact->GetFixtureA()->GetBody() == BirdBody && contact->GetFixtureB()->GetBody() == platform && contact->IsTouching())
			|| (contact->GetFixtureA()->GetBody() == platform && contact->GetFixtureB()->GetBody() == BirdBody && contact->IsTouching()))
		{
			return true;
		}
		else
		{
			return false;
		}
}

void CGameManager::JoinObjects(b2Body* _body1, b2Body* _body2, b2Vec2 _anchorB1, b2Vec2 _anchorB2, BJoint _joint)
{
	switch (_joint)
	{
	case BJoint::DIST:
	{
		b2DistanceJointDef* newJoint = new b2DistanceJointDef();
		newJoint->Initialize(_body1, _body2, _anchorB1, _anchorB2);
		newJoint->length = 0.5;
		//newJoint->collideConnected = true;

		World->CreateJoint(newJoint);
		break;
	}
	case BJoint::REVO:
	{
		b2RevoluteJointDef* newJoint2 = new b2RevoluteJointDef();
		newJoint2->Initialize(_body1, _body2, _anchorB1);
		//newJoint->collideConnected = true;
	/*	newJoint2->lowerAngle = -0.5f * b2_pi;		newJoint2->upperAngle = 0.25f * b2_pi;		newJoint2->enableLimit = true;*/

		World->CreateJoint(newJoint2);
		break;
	}
	default:
	{
		break;
	}
	}
	

}