//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name   : GameManager.cpp
// Description : GameManager implementation file
// Author      : alexander jenkins
// Mail        : alexander.jenkins@mds.ac.nz
//

#include "GameManager.h"

CGameManager::CGameManager()
{
	Window = new RenderWindow(VideoMode(utils::ScreenWidth, utils::ScreenHeight, 32), "Grumpy Birds");
	Window->setFramerateLimit(60);
	Gravity = new b2Vec2(0.f, 9.8f);
	World = new b2World(*Gravity);
	Texture* MouseTex = new Texture;
	MouseTex->loadFromFile("Resources/Textures/mouse.png");
	MouseSprite = new Sprite;
	MouseSprite->setTexture(*MouseTex);
}

CGameManager::~CGameManager()
{
	ClearLevel1();
	ClearGameOver();

	while (Sprites.size() > 0)
	{
		Sprites.pop_back();
	}
	while (Bodies.size() > 0)
	{
		Bodies.pop_back();
	}
	while (DestSprites.size() > 0)
	{
		DestSprites.pop_back();
	}
	while (DestBodies.size() > 0)
	{
		DestBodies.pop_back();
	}
	
}

//Load level 1
void CGameManager::InitiliaseLevel1()
{
	//initialise variables
	IsFired = false;
	Reset = true;
	InGame = GameState::GAME;
	EnemySize = 0;
	//Initialise pointers
	Arrow = new Sprite;
	SlingSprite = new Sprite;
	BackgroundSprite = new Sprite;
	//Initialise instructions text
	InstructionsText = new Text;
	Font* font = new Font;
	font->loadFromFile("Resources/Fonts/BRLNSR.TTF");
	InstructionsText->setFont(*font);
	InstructionsText->setCharacterSize(20);
	InstructionsText->setOutlineColor(Color::Black);
	InstructionsText->setOutlineThickness(2.0f);
	InstructionsText->setFillColor(Color::White);
	InstructionsText->setString("Press <Q> to Reset Level - Press <E> to Reset Bird - <Left Click> And Drag Bird To Aim Then Release To Fire");
	InstructionsText->setPosition(40.0f, 0.0f);

	//create ceiling and joint objects
	CreateObject(World, utils::ScreenWidth, 0.0f, utils::HSWidth, 0.0f, "Resources/Textures/ground.png", BShape::BOX, 1.0f, 1.0f, b2BodyType::b2_staticBody);
	CreateObject(World, 100, 10, 100, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[0], Bodies[1], b2Vec2(Bodies[0]->GetPosition().x, Bodies[0]->GetPosition().y + 0.3f), b2Vec2(Bodies[1]->GetPosition().x - 50.0f / SCALE, Bodies[1]->GetPosition().y), BJoint::DIST);
	CreateObject(World, 100, 10, 100, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[1], Bodies[2], b2Vec2(Bodies[1]->GetPosition().x + 50.0f / SCALE, Bodies[1]->GetPosition().y), b2Vec2(Bodies[2]->GetPosition().x - 25.0f / SCALE, Bodies[2]->GetPosition().y), BJoint::REVO);
	CreateObject(World, 100, 10, 100, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[2], Bodies[3], b2Vec2(Bodies[2]->GetPosition().x - 50.0f / SCALE, Bodies[2]->GetPosition().y), b2Vec2(), BJoint::REVO);

	//create ground
	CreateObject(World, utils::ScreenWidth, 25.0f, utils::HSWidth, utils::ScreenHeight, "Resources/Textures/ground.png", BShape::BOX, 1.0f, 1.0f, b2BodyType::b2_staticBody);
	
	//create background sprite
	Texture* Background = new Texture;
	Background->loadFromFile("Resources/Textures/background.png");
	BackgroundSprite->setTexture(*Background);

	//create slingshot sprite
	Texture* Sling = new Texture;
	Sling->loadFromFile("Resources/Textures/slingshot.png");
	SlingSprite->setTexture(*Sling);
	SlingSprite->setPosition(150.0f, 430.0f);

	//Create Grumpy Bird
	CreateBird();
	BirdBody->SetEnabled(false);
	BirdBody->SetSleepingAllowed(false);

	//Create All platforms and obstacles
	float boxSize = 50.0f;
	CreateObject(World, boxSize, boxSize, 700, 560, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 560, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 540, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 540, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 480, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 480, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 460, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 460, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);

	//Destructable planks
	CreateDestructable(World, 100, 10, 740, 500, "Resources/Textures/plank.png");
	CreateDestructable(World, 100, 10, 740, 440, "Resources/Textures/plank.png");

	//Destructable enemy
	CreateEnemy(World, 50, 50, 740, 500, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
	CreateEnemy(World, 50, 50, 740, 440, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
	CreateEnemy(World, 50, 50, 740, 540, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);

	//Arrow sprite
	Texture* aBox = new Texture;
	aBox->loadFromFile("Resources/Textures/arrow.png");
	Arrow->setTexture(*aBox);
	Arrow->setOrigin(0.0f, 20.0f);
	Arrow->setPosition(200.0f, 460.0f);
	Arrow->scale(0.0f, 0.0f);
}
//Clear Level 1 pointers and vectors
void CGameManager::ClearLevel1()
{
	EnemySize = 0;
	Arrow = 0;
	SlingSprite = 0;
	BackgroundSprite = 0;

	while (Sprites.size() > 0)
	{
		Sprites.pop_back();
	}
	while (Bodies.size() > 0)
	{
		Bodies.pop_back();
	}

	while (DestSprites.size() > 0)
	{
		DestSprites.pop_back();
	}
	while (DestBodies.size() > 0)
	{
		DestBodies.pop_back();
	}
	
	while (EnemyBodies.size() > 0)
	{
		EnemyBodies.pop_back();
	}	
	while (EnemySprites.size() > 0)
	{
		EnemySprites.pop_back();
	}
	//clear world body list
	while (World->GetBodyCount() > 0)
	{
		World->DestroyBody(World->GetBodyList());
	}
}
//Initialise GameOver
void CGameManager::InitialiseGameOver()
{
	//initialise background image
	Texture* WinTex = new Texture;
	WinTex->loadFromFile("Resources/Textures/win.png");
	WinSprite = new Sprite;
	WinSprite->setTexture(*WinTex);
	//initialise retry font
	Font* RetryTex = new Font;
	RetryTex->loadFromFile("Resources/Fonts/vindicatorital.ttf");
	RetryButton = new Text;
	RetryButton->setFont(*RetryTex);
	RetryButton->setCharacterSize(60);
	RetryButton->setFillColor(Color::Green);
}
//clear game over pointers
void CGameManager::ClearGameOver()
{
	delete WinSprite;
	WinSprite = 0;
	delete RetryButton;
	RetryButton = 0;
}
//Update Function
void CGameManager::Update()
{
	//Initialise level 1
	InitiliaseLevel1();
	
	//Game update
	while (Window->isOpen())
	{
		//set mouse sprite to follow mouse for bird collision
		MouseSprite->setPosition(Mouse::getPosition(*Window).x - 25.0f, Mouse::getPosition(*Window).y - 25.0f);

		Event event;
		while (Window->pollEvent(event))
		{
			if (event.type == Event::Closed)
				Window->close();
		}

		//switch game state
		switch (InGame)
		{
		case GameState::MENU:
		{
			//menu code here

			break;
		}
		case GameState::GAME:
		{
			//Reset the level by pressing 'Q'
			if (Keyboard::isKeyPressed(Keyboard::Q))
			{
				//Clear and reset entire level
				ClearLevel1();
				InitiliaseLevel1();
				Reset = true;
			}

			//Reset the bird by pressing 'E'
			if (Keyboard::isKeyPressed(Keyboard::E))
			{
				//set to disabled and reset to origin position
				BirdBody->SetEnabled(false);
				BirdBody->SetTransform(b2Vec2(200 / SCALE, 460 / SCALE), 0.0f);
				BirdBody->SetAngularVelocity(0.0f);
				Reset = true;
			}

			//Show direction vector when mouse is held and dragged for firing
			if (Mouse::isButtonPressed(Mouse::Left) && BirdSprite->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//Get initial mouse position
				if (!IsPressed)
				{
					IsPressed = true;
					MouseInitialX = Mouse::getPosition(*Window).x;
					MouseInitialY = Mouse::getPosition(*Window).y;
				}


				if (Reset)
				{
					//Set bird body position to follow mouse
					BirdBody->SetLinearVelocity(b2Vec2(0, 0));
					BirdBody->SetTransform(b2Vec2(Mouse::getPosition(*Window).x / SCALE, Mouse::getPosition(*Window).y / SCALE), BirdBody->GetAngle());
					//create a force vector between initial position and mouse position for transforming the arrow sprite
					vec2 forceVec = vec2(Mouse::getPosition(*Window).x - MouseInitialX, Mouse::getPosition(*Window).y - MouseInitialY);
					forceVec = normalize(forceVec);
					//Scale and rotate the arrow sprite to show trajectory
					Arrow->setRotation(atan2f(forceVec.y, forceVec.x) * (180.0f / float(PI)) - 180.0f);
					Arrow->setScale(Distancev2(vec2(MouseInitialX, MouseInitialY), vec2(Mouse::getPosition(*Window).x, Mouse::getPosition(*Window).y)) / 200.0f, 1.0f);
				}
			}
			else if (IsPressed)
			{
				//when released get new position of mouse for the forc vector
				IsPressed = false;
				MouseReleaseX = Mouse::getPosition(*Window).x;
				MouseReleaseY = Mouse::getPosition(*Window).y;
				//hide arrow sprite
				Arrow->scale(0.0f, 0.0f);

				if (Reset)
					IsFired = true;
			}

			//When bird is fired
			if (IsFired)
			{
				//create a force vector between the mouse and initial bird position
				vec2 forceVec = vec2(MouseReleaseX - MouseInitialX, MouseReleaseY - MouseInitialY);
				forceVec = normalize(forceVec);

				//Get the distance between the two postions
				float distance = Distancev2(vec2(MouseInitialX, MouseInitialY), vec2(MouseReleaseX, MouseReleaseY));
				//Reverse the distance direction and multiple by factor
				forceVec *= -distance * 4;
				//apply force to bird body
				BirdBody->SetEnabled(true);
				BirdBody->ApplyForceToCenter(b2Vec2(forceVec.x, forceVec.y), true);

				IsFired = false;
				Reset = false;
			}


			/** Simulate the world */
			World->Step(1 / 60.f, 8, 3);
			//Clear screen with white
			Window->clear(Color::White);

			//Drawing spites (order matters)
			Window->draw(*BackgroundSprite);

			//Set sprites to follow b2Body position and rotation
			for (size_t i = 0; i < Sprites.size(); i++)
			{
				Sprites[i]->setPosition(SCALE * Bodies[i]->GetPosition().x, Bodies[i]->GetPosition().y * SCALE);
				Sprites[i]->setRotation(Bodies[i]->GetAngle() * 180 / b2_pi);
				Window->draw(*Sprites[i]);
			}
			//Set sprites to follow b2Body position and rotation
			for (size_t i = 0; i < DestSprites.size(); i++)
			{
				DestSprites[i]->setPosition(SCALE * DestBodies[i]->GetPosition().x, DestBodies[i]->GetPosition().y * SCALE);
				DestSprites[i]->setRotation(DestBodies[i]->GetAngle() * 180 / b2_pi);
				Window->draw(*DestSprites[i]);
			}
			//Set sprites to follow b2Body position and rotation
			for (size_t i = 0; i < EnemySprites.size(); i++)
			{
				
				EnemySprites[i]->setPosition(SCALE * EnemyBodies[i]->GetPosition().x, EnemyBodies[i]->GetPosition().y * SCALE);
				EnemySprites[i]->setRotation(EnemyBodies[i]->GetAngle() * 180 / b2_pi);
				Window->draw(*EnemySprites[i]);

				if (Reset)
				{
					EnemyBodies[i]->SetAngularVelocity(0.0f);
				}
			}

			//Set bird body max velocity
			if (BirdBody->GetLinearVelocity().x > 20.0f)
			{
				BirdBody->SetLinearVelocity(b2Vec2(20.0f, BirdBody->GetLinearVelocity().y));
			}

			//Set bird sprite following bird b2Body
			BirdSprite->setPosition(SCALE * BirdBody->GetPosition().x, BirdBody->GetPosition().y * SCALE);
			BirdSprite->setRotation(BirdBody->GetAngle() * 180 / b2_pi);

			//Draw sprites
			Window->draw(*BirdSprite);
			Window->draw(*SlingSprite);
			Window->draw(*Arrow);
			Window->draw(*InstructionsText);
			//display window
			Window->display();

			//Check for player and destructable object collision
			for (size_t i = 0; i < DestSprites.size(); i++)
			{
				if (BirdSprite->getGlobalBounds().intersects(DestSprites[i]->getGlobalBounds()))
				{
					World->DestroyBody(DestBodies[i]);
					DestBodies.erase(DestBodies.begin() + i);
					DestSprites.erase(DestSprites.begin() + i);
				}
			}

			//Check for player and enemy object collision and object ground collision
			for (size_t i = 0; i < EnemySprites.size(); i++)
			{
				if (BirdSprite->getGlobalBounds().intersects(EnemySprites[i]->getGlobalBounds()) || EnemySprites[i]->getGlobalBounds().intersects(Sprites[4]->getGlobalBounds()))
				{
					World->DestroyBody(EnemyBodies[i]);
					EnemyBodies.erase(EnemyBodies.begin() + i);
					EnemySprites.erase(EnemySprites.begin() + i);
					EnemySize--;
				}
			}
			//when all enemies are dead (or debug pres 'A' - initialise game over
			if (EnemySize == 0 || Keyboard::isKeyPressed(Keyboard::A))
			{
				//clear level objects
				ClearLevel1();
				//Initialise game over screen
				InitialiseGameOver();
				//change gamestate
				InGame = GameState::END;
			}
			break;
		}	
		case GameState::END:
		{
			//When hovering over retry button
			if (RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//change text and move position for consistancy
				RetryButton->setString("[Retry]");
				RetryButton->setOutlineThickness(3.0f);
				RetryButton->setPosition(utils::HSWidth - 93.0f, utils::HSHeight + 100.0f);
				
			}
			else
			{
				//reset text when not hovering
				RetryButton->setOutlineThickness(1.0f);
				RetryButton->setString("Retry");
				RetryButton->setPosition(utils::HSWidth - 75.0f, utils::HSHeight + 100.0f);
				
			}
			//Clear screen with white
			Window->clear(Color::White);
			//Draw win screen
			Window->draw(*WinSprite);
	
			Window->draw(*RetryButton);
			//display window
			Window->display();

			//initialise game when clicking on "Retry"
			if (Mouse::isButtonPressed(Mouse::Left) && RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				ClearGameOver();
				InitiliaseLevel1();
				InGame = GameState::GAME;
			}
		}
			break;
		default:
			break;
		}

	}
}

//Create an object in the game world setting the object size, position, texture path, shape, scale and body type
void CGameManager::CreateObject(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, BShape _shape, float _scaleX, float _scaleY, b2BodyType _bodyType)
{
	//Create a texture
	Texture* texture = new Texture;
	texture->loadFromFile(texPath);
	//Create a b2Body definition
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(PosX / SCALE, PosY / SCALE);
	BodyDef->type = _bodyType;
	//Set a b2Body pointer
	b2Body* Body = World->CreateBody(BodyDef);
	//Create a fixture pointer
	b2FixtureDef* FixtureDef = new b2FixtureDef;
	FixtureDef->density = 1.f;
	FixtureDef->friction = 0.7f;

	//Define the fixture based on shape input
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
	//attach the fixture to the bosy
	Body->CreateFixture(FixtureDef);
	//Create the sprite
	sf::Sprite* sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(SizeX / 2, SizeY / 2);
	sprite->setScale(_scaleX, _scaleY);
	//Push both body and sprite in to vectors
	Bodies.push_back(Body);
	Sprites.push_back(sprite);
}

//creat the player bird object
void CGameManager::CreateBird()
{
	Texture* texture = new Texture;
	texture->loadFromFile("Resources/Textures/grumpybird.png");
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(200 / SCALE, 460 / SCALE);
	BodyDef->type = b2BodyType::b2_dynamicBody;
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
//create a destructable object in the game world setting the object size, position, texture path and shape
void CGameManager::CreateDestructable(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, float _scaleX, float _scaleY, BShape _shape)
{
	Texture* texture = new Texture;
	texture->loadFromFile(texPath);
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(PosX / SCALE, PosY / SCALE);
	BodyDef->type = b2BodyType::b2_dynamicBody;
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

//create a destructable enemy in the game world setting the object size, position, texture path and shape
void CGameManager::CreateEnemy(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, float _scaleX, float _scaleY, BShape _shape)
{
	Texture* texture = new Texture;
	texture->loadFromFile(texPath);
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(PosX / SCALE, PosY / SCALE);
	BodyDef->type = b2BodyType::b2_dynamicBody;
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

	//enemy size for win condition
	EnemySize++;

	EnemyBodies.push_back(Body);
	EnemySprites.push_back(sprite);
	
}
//Creat e a joint between two objects defined by two anchor points and a joint type
void CGameManager::JoinObjects(b2Body* _body1, b2Body* _body2, b2Vec2 _anchorB1, b2Vec2 _anchorB2, BJoint _joint)
{
	switch (_joint)
	{
	case BJoint::DIST:
	{
		b2DistanceJointDef* newJoint = new b2DistanceJointDef();
		newJoint->Initialize(_body1, _body2, _anchorB1, _anchorB2);
		newJoint->length = 0.5;
		newJoint->collideConnected = true;

		World->CreateJoint(newJoint);
		break;
	}
	case BJoint::REVO:
	{
		b2RevoluteJointDef* newJoint2 = new b2RevoluteJointDef();
		newJoint2->Initialize(_body1, _body2, _anchorB1);


		World->CreateJoint(newJoint2);
		break;
	}
	default:
	{
		break;
	}
	}
	

}