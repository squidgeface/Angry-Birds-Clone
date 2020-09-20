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
//Initialise Menu
void CGameManager::InitialiseMenu()
{
	Level = 0;
	//initialise background image
	Texture* WinTex = new Texture;
	WinTex->loadFromFile("Resources/Textures/menu.png");
	WinSprite = new Sprite;
	WinSprite->setTexture(*WinTex);
	//initialise retry font
	Font* RetryTex = new Font;
	RetryTex->loadFromFile("Resources/Fonts/vindicatorital.ttf");
	PlayButton = new Text;
	PlayButton->setFont(*RetryTex);
	PlayButton->setCharacterSize(60);
	PlayButton->setFillColor(Color::Green);
	QuitButton = new Text;
	QuitButton->setFont(*RetryTex);
	QuitButton->setCharacterSize(60);
	QuitButton->setFillColor(Color::Green);
}
//clear game over pointers
void CGameManager::ClearMenu()
{
	delete WinSprite;
	WinSprite = 0;
	delete PlayButton;
	PlayButton = 0;
	delete QuitButton;
	QuitButton = 0;
}
//Load level 1
void CGameManager::InitiliaseLevel1()
{
	//initialise variables
	Level = 1;
	IsFired = false;
	Reset = true;
	InGame = GameState::GAME;
	EnemySize = 0;
	BirdsUsed[0] = 0;
	BirdsUsed[1] = 0;
	BirdsUsed[2] = 0;
	//Initialise pointers
	Arrow = new Sprite;
	SlingSprite = new Sprite;
	BackgroundSprite = new Sprite;
	//Initialise instructions text
	InstructionsText = new Text;
	Font* font = new Font;
	font->loadFromFile("Resources/Fonts/BRLNSR.TTF");
	InstructionsText->setFont(*font);
	InstructionsText->setCharacterSize(18);
	InstructionsText->setOutlineColor(Color::Black);
	InstructionsText->setOutlineThickness(1.0f);
	InstructionsText->setFillColor(Color::White);
	InstructionsText->setString("Press <Q> to Reset Level - Press <E> to use abilities - <Left Click> And Drag Bird To Aim Then Release To Fire - <1-3> change Birds");
	InstructionsText->setPosition(5.0f, 0.0f);

	//create ceiling and joint objects
	CreateObject(World, utils::ScreenWidth, 0.0f, utils::HSWidth, 0.0f, "Resources/Textures/ground.png", BShape::BOX, 1.0f, 1.0f, b2BodyType::b2_staticBody);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[0], Bodies[1], b2Vec2(Bodies[0]->GetPosition().x + 250 / SCALE, Bodies[0]->GetPosition().y + 0.3f), b2Vec2(Bodies[1]->GetPosition().x - 50.0f / SCALE, Bodies[1]->GetPosition().y), BJoint::DIST);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[1], Bodies[2], b2Vec2(Bodies[1]->GetPosition().x + 50.0f / SCALE, Bodies[1]->GetPosition().y), b2Vec2(Bodies[2]->GetPosition().x - 25.0f / SCALE, Bodies[2]->GetPosition().y), BJoint::REVO);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[2], Bodies[3], b2Vec2(Bodies[2]->GetPosition().x - 50.0f / SCALE, Bodies[2]->GetPosition().y), b2Vec2(), BJoint::REVO);

	CreateObject(World, 100, 10, 400, 400, "Resources/Textures/plank.png", BShape::BOX);
	CreateObject(World, 1, 250, 600, 500, "Resources/Textures/gate.png", BShape::BOX);
	JoinObjects(Bodies[4], Bodies[5], b2Vec2(Bodies[4]->GetPosition().x, Bodies[4]->GetPosition().y), b2Vec2(Bodies[5]->GetPosition().x, Bodies[4]->GetPosition().y), BJoint::PULLY, b2Vec2(utils::HSWidth - 200, 0.0f), b2Vec2(utils::HSWidth + 200, 0.0f));
	Bodies[4]->SetFixedRotation(true);
	Bodies[5]->SetFixedRotation(true);
	Bodies[4]->SetGravityScale(0.5f);


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

	//Create Grumpy Birds
	CreateBird();
	BirdBody->SetEnabled(false);
	BirdBody->SetSleepingAllowed(false);

	//Create All platforms and obstacles
	float boxSize = 50.0f;
	CreateObject(World, boxSize, boxSize, 700, 540, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 560, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 540, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 560, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 460, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 480, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 460, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 480, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);

	//Destructable planks
	CreateDestructable(World, 100, 10, 740, 500, "Resources/Textures/plank.png");
	CreateDestructable(World, 100, 10, 740, 430, "Resources/Textures/plank.png");
	CreateDestructable(World, 100, 10, 740, 570, "Resources/Textures/plank.png");

	//Destructable enemy
	CreateEnemy(World, 50, 50, 740, 500, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
	CreateEnemy(World, 50, 50, 740, 400, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
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
	BirdBody = 0;
	BirdSprite = 0;

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
	while (cloneSprites.size() > 0)
	{
		cloneSprites.pop_back();
		cloneBodies.pop_back();
	}
	//clear world body list
	while (World->GetBodyCount() > 0)
	{
		World->DestroyBody(World->GetBodyList());
	}
}
//Load level 2
void CGameManager::InitiliaseLevel2()
{
	//initialise variables
	Level = 2;
	IsFired = false;
	Reset = true;
	InGame = GameState::GAME;
	EnemySize = 0;
	BirdsUsed[0] = 0;
	BirdsUsed[1] = 0;
	BirdsUsed[2] = 0;
	//Initialise pointers
	Arrow = new Sprite;
	SlingSprite = new Sprite;
	BackgroundSprite = new Sprite;
	//Initialise instructions text
	InstructionsText = new Text;
	Font* font = new Font;
	font->loadFromFile("Resources/Fonts/BRLNSR.TTF");
	InstructionsText->setFont(*font);
	InstructionsText->setCharacterSize(18);
	InstructionsText->setOutlineColor(Color::Black);
	InstructionsText->setOutlineThickness(1.0f);
	InstructionsText->setFillColor(Color::White);
	InstructionsText->setString("Press <Q> to Reset Level - Press <E> to use abilities - <Left Click> And Drag Bird To Aim Then Release To Fire - <1-3> change Birds");
	InstructionsText->setPosition(5.0f, 0.0f);

	//create ceiling and joint objects
	CreateObject(World, utils::ScreenWidth, 0.0f, utils::HSWidth, 0.0f, "Resources/Textures/ground.png", BShape::BOX, 1.0f, 1.0f, b2BodyType::b2_staticBody);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[0], Bodies[1], b2Vec2(Bodies[0]->GetPosition().x + 250 / SCALE, Bodies[0]->GetPosition().y + 0.3f), b2Vec2(Bodies[1]->GetPosition().x - 50.0f / SCALE, Bodies[1]->GetPosition().y), BJoint::DIST);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[1], Bodies[2], b2Vec2(Bodies[1]->GetPosition().x + 50.0f / SCALE, Bodies[1]->GetPosition().y), b2Vec2(Bodies[2]->GetPosition().x - 25.0f / SCALE, Bodies[2]->GetPosition().y), BJoint::REVO);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[2], Bodies[3], b2Vec2(Bodies[2]->GetPosition().x - 50.0f / SCALE, Bodies[2]->GetPosition().y), b2Vec2(), BJoint::REVO);

	CreateObject(World, 100, 10, 400, 400, "Resources/Textures/plank.png", BShape::BOX);
	CreateObject(World, 1, 250, 600, 500, "Resources/Textures/gate.png", BShape::BOX);
	JoinObjects(Bodies[4], Bodies[5], b2Vec2(Bodies[4]->GetPosition().x, Bodies[4]->GetPosition().y), b2Vec2(Bodies[5]->GetPosition().x, Bodies[4]->GetPosition().y), BJoint::PULLY, b2Vec2(utils::HSWidth - 200, 0.0f), b2Vec2(utils::HSWidth + 200, 0.0f));
	Bodies[4]->SetFixedRotation(true);
	Bodies[5]->SetFixedRotation(true);
	Bodies[4]->SetGravityScale(0.5f);


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

	//Create Grumpy Birds
	CreateBird();
	BirdBody->SetEnabled(false);
	BirdBody->SetSleepingAllowed(false);

	//Create All platforms and obstacles
	float boxSize = 50.0f;
	CreateObject(World, boxSize, boxSize, 700, 540, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 560, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 540, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 560, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 460, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 700, 480, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 460, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 780, 480, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);

	//Destructable planks
	CreateDestructable(World, 100, 10, 740, 500, "Resources/Textures/plank.png");
	CreateDestructable(World, 100, 10, 740, 430, "Resources/Textures/plank.png");
	CreateDestructable(World, 100, 10, 740, 570, "Resources/Textures/plank.png");

	//Destructable enemy
	CreateEnemy(World, 50, 50, 740, 500, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
	CreateEnemy(World, 50, 50, 740, 400, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
	CreateEnemy(World, 50, 50, 740, 540, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);

	//Arrow sprite
	Texture* aBox = new Texture;
	aBox->loadFromFile("Resources/Textures/arrow.png");
	Arrow->setTexture(*aBox);
	Arrow->setOrigin(0.0f, 20.0f);
	Arrow->setPosition(200.0f, 460.0f);
	Arrow->scale(0.0f, 0.0f);
}
//Clear Level 2 pointers and vectors
void CGameManager::ClearLevel2()
{
	EnemySize = 0;
	Arrow = 0;
	SlingSprite = 0;
	BackgroundSprite = 0;
	BirdBody = 0;
	BirdSprite = 0;

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
	while (cloneSprites.size() > 0)
	{
		cloneSprites.pop_back();
		cloneBodies.pop_back();
	}
	//clear world body list
	while (World->GetBodyCount() > 0)
	{
		World->DestroyBody(World->GetBodyList());
	}
}
//Initialise win screen
void CGameManager::InitialiseWin()
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
	QuitButton = new Text;
	QuitButton->setFont(*RetryTex);
	QuitButton->setCharacterSize(60);
	QuitButton->setFillColor(Color::Green);
}
//clear game win pointers
void CGameManager::ClearWin()
{
	delete WinSprite;
	WinSprite = 0;
	delete RetryButton;
	RetryButton = 0;
	delete QuitButton;
	QuitButton = 0;
}
//Initialise Lose
void CGameManager::InitialiseLose()
{
	Level = 0;
	//initialise background image
	Texture* WinTex = new Texture;
	WinTex->loadFromFile("Resources/Textures/lose.png");
	WinSprite = new Sprite;
	WinSprite->setTexture(*WinTex);
	//initialise retry font
	Font* RetryTex = new Font;
	RetryTex->loadFromFile("Resources/Fonts/vindicatorital.ttf");
	RetryButton = new Text;
	RetryButton->setFont(*RetryTex);
	RetryButton->setCharacterSize(60);
	RetryButton->setFillColor(Color::Green);
	QuitButton = new Text;
	QuitButton->setFont(*RetryTex);
	QuitButton->setCharacterSize(60);
	QuitButton->setFillColor(Color::Green);
}
//clear game over pointers
void CGameManager::ClearLose()
{
	delete WinSprite;
	WinSprite = 0;
	delete RetryButton;
	RetryButton = 0;
	delete QuitButton;
	QuitButton = 0;
}
//Initialise GameOver
void CGameManager::InitialiseGameOver()
{
	Level = 0;
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
	InitialiseMenu();
	
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
			//When hovering over play button
			if (PlayButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//change text and move position for consistancy
				PlayButton->setString("[Play]");
				PlayButton->setOutlineThickness(3.0f);
				PlayButton->setPosition(utils::HSWidth - 93.0f, utils::HSHeight);

			}
			else
			{
				//reset text when not hovering
				PlayButton->setOutlineThickness(1.0f);
				PlayButton->setString("Play");
				PlayButton->setPosition(utils::HSWidth - 75.0f, utils::HSHeight);

			}
			//When hovering over quit button
			if (QuitButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//change text and move position for consistancy
				QuitButton->setString("[Quit]");
				QuitButton->setOutlineThickness(3.0f);
				QuitButton->setPosition(utils::HSWidth - 93.0f, utils::HSHeight + 100.0f);

			}
			else
			{
				//reset text when not hovering
				QuitButton->setOutlineThickness(1.0f);
				QuitButton->setString("Quit");
				QuitButton->setPosition(utils::HSWidth - 75.0f, utils::HSHeight + 100.0f);

			}
			//Clear screen with white
			Window->clear(Color::White);
			//Draw win screen
			Window->draw(*WinSprite);

			Window->draw(*PlayButton);
			Window->draw(*QuitButton);
			//display window
			Window->display();

			//initialise game when clicking on "Play"
			if (Mouse::isButtonPressed(Mouse::Left) && PlayButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				ClearMenu();
				InitiliaseLevel1();
				InGame = GameState::GAME;
			}
			//initialise game when clicking on "Quit"
			if (Mouse::isButtonPressed(Mouse::Left) && QuitButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				Window->close();
			}

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
			
			if (Keyboard::isKeyPressed(Keyboard::Num1) && BirdsUsed[0] == 0)
			{
				//Clear and reset entire level
				CreateBird();
				BirdBody->SetEnabled(false);
				BirdBody->SetSleepingAllowed(false);
				Reset = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Num2) && BirdsUsed[1] == 0)
			{
				//Clear and reset entire level
				CreateBird2(75.0f);
				BirdBody->SetEnabled(false);
				BirdBody->SetSleepingAllowed(false);
				Reset = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Num3) && BirdsUsed[2] == 0)
			{
				//Clear and reset entire level
				CreateBird3();
				BirdBody->SetEnabled(false);
				BirdBody->SetSleepingAllowed(false);
				Reset = true;
			}

			//Press E to activate bird power
			if (Keyboard::isKeyPressed(Keyboard::E) && Reset == false)
			{
				if (BirdCount == 2)
				{
					//bird splits into three
					CreateClones();
					BirdBody->SetEnabled(false);
					BirdBody->SetTransform(b2Vec2(-100, 0), 0);
					Reset = true;
				}
				else if (BirdCount == 3)
				{
					Reset = true;
					BirdBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					BirdBody->SetGravityScale(3.0f);
				}
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
					
					BirdsUsed[BirdCount - 1] = 1;

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

			for (size_t i = 0; i < cloneSprites.size(); i++)
			{
				cloneSprites[i]->setPosition(SCALE * cloneBodies[i]->GetPosition().x, cloneBodies[i]->GetPosition().y * SCALE);
				cloneSprites[i]->setRotation(cloneBodies[i]->GetAngle() * 180 / b2_pi);
				Window->draw(*cloneSprites[i]);
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

			//check for clone sprite destructable collision
			for (size_t j = 0; j < cloneSprites.size(); j++)
			{
				for (size_t i = 0; i < DestSprites.size(); i++)
				{
					if (cloneSprites[j]->getGlobalBounds().intersects(DestSprites[i]->getGlobalBounds()))
					{
						World->DestroyBody(DestBodies[i]);
						DestBodies.erase(DestBodies.begin() + i);
						DestSprites.erase(DestSprites.begin() + i);
					}
				}
			}

		

			//Check for player and enemy object collision and object ground collision
			for (size_t i = 0; i < EnemySprites.size(); i++)
			{
				if (BirdSprite->getGlobalBounds().intersects(EnemySprites[i]->getGlobalBounds()) || EnemySprites[i]->getGlobalBounds().intersects(Sprites[6]->getGlobalBounds()))
				{
					World->DestroyBody(EnemyBodies[i]);
					EnemyBodies.erase(EnemyBodies.begin() + i);
					EnemySprites.erase(EnemySprites.begin() + i);
					EnemySize--;
				}
			}

			//check for clone sprite enemy collision
			for (size_t j = 0; j < cloneSprites.size(); j++)
			{
				for (size_t i = 0; i < EnemySprites.size(); i++)
				{
					if (cloneSprites[j]->getGlobalBounds().intersects(EnemySprites[i]->getGlobalBounds()))
					{
						World->DestroyBody(EnemyBodies[i]);
						EnemyBodies.erase(EnemyBodies.begin() + i);
						EnemySprites.erase(EnemySprites.begin() + i);
						EnemySize--;
					}
				}
			}

			//keep pully still
			Bodies[4]->SetLinearVelocity(b2Vec2(0.0f, Bodies[4]->GetLinearVelocity().y));
			Bodies[5]->SetLinearVelocity(b2Vec2(0.0f, Bodies[5]->GetLinearVelocity().y));

			
			//get current elapsed time of frame
			currentTime = deltaClock.getElapsedTime();

			deltaTime = (currentTime - prevDeltaTime);

			prevDeltaTime = currentTime;
			deltaClock.restart();

			//when all enemies are dead (or debug pres 'A' - initialise game over
			if (EnemySize == 0 || Keyboard::isKeyPressed(Keyboard::A))
			{
				if (Level == 1)
				{
					//clear level objects
					ClearLevel1();
					//Initialise game over screen
					InitialiseWin();
					//change gamestate
					InGame = GameState::WIN;
				}
				else if (Level == 2)
				{
					//clear level objects
					ClearLevel2();
					//Initialise game over screen
					InitialiseWin();
					//change gamestate
					InGame = GameState::END;
				}
			}

			if (BirdsUsed[0] == 1 && BirdsUsed[1] == 1 && BirdsUsed[2] == 1 && (Level == 1 || Level == 2))
			{
				Timer += abs(deltaTime.asMicroseconds());

				if (Timer >= 200000)
				{
					if (Level == 1)
					{
						ClearLevel1();
					}
					else if (Level == 2)
					{
						ClearLevel2();
					}

					InitialiseLose();
					InGame = GameState::LOSE;
				}

			}

			break;
		}
		case GameState::WIN:
		{
			//When hovering over retry button
			if (RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//change text and move position for consistancy
				RetryButton->setString("[Continue]");
				RetryButton->setOutlineThickness(3.0f);
				RetryButton->setPosition(utils::HSWidth - 118.0f, utils::HSHeight);

			}
			else
			{
				//reset text when not hovering
				RetryButton->setOutlineThickness(1.0f);
				RetryButton->setString("Continue");
				RetryButton->setPosition(utils::HSWidth - 100.0f, utils::HSHeight);

			}
			//When hovering over retry button
			if (QuitButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//change text and move position for consistancy
				QuitButton->setString("[Quit]");
				QuitButton->setOutlineThickness(3.0f);
				QuitButton->setPosition(utils::HSWidth - 93.0f, utils::HSHeight + 100.0f);

			}
			else
			{
				//reset text when not hovering
				QuitButton->setOutlineThickness(1.0f);
				QuitButton->setString("Quit");
				QuitButton->setPosition(utils::HSWidth - 75.0f, utils::HSHeight + 100.0f);

			}
			//Clear screen with white
			Window->clear(Color::White);
			//Draw win screen
			Window->draw(*WinSprite);

			Window->draw(*RetryButton);
			Window->draw(*QuitButton);
			//display window
			Window->display();

			//initialise game when clicking on "Retry"
			if (Mouse::isButtonPressed(Mouse::Left) && RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				ClearWin();
				InitiliaseLevel2();
				InGame = GameState::GAME;
			}
			//initialise game when clicking on "Quit"
			if (Mouse::isButtonPressed(Mouse::Left) && QuitButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				ClearWin();
				InitialiseMenu();
				InGame = GameState::MENU;
			}
			break;
		}
		case GameState::LOSE:
		{
			//When hovering over retry button
			if (RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//change text and move position for consistancy
				RetryButton->setString("[Retry]");
				RetryButton->setOutlineThickness(3.0f);
				RetryButton->setPosition(utils::HSWidth - 93.0f, utils::HSHeight);

			}
			else
			{
				//reset text when not hovering
				RetryButton->setOutlineThickness(1.0f);
				RetryButton->setString("Retry");
				RetryButton->setPosition(utils::HSWidth - 75.0f, utils::HSHeight);

			}
			//When hovering over retry button
			if (QuitButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//change text and move position for consistancy
				QuitButton->setString("[Quit]");
				QuitButton->setOutlineThickness(3.0f);
				QuitButton->setPosition(utils::HSWidth - 93.0f, utils::HSHeight + 100.0f);

			}
			else
			{
				//reset text when not hovering
				QuitButton->setOutlineThickness(1.0f);
				QuitButton->setString("Quit");
				QuitButton->setPosition(utils::HSWidth - 75.0f, utils::HSHeight + 100.0f);

			}
			//Clear screen with white
			Window->clear(Color::White);
			//Draw win screen
			Window->draw(*WinSprite);

			Window->draw(*RetryButton);
			Window->draw(*QuitButton);
			//display window
			Window->display();

			//initialise game when clicking on "Retry"
			if (Mouse::isButtonPressed(Mouse::Left) && RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				ClearWin();
				if (Level == 1)
				{
					InitiliaseLevel1();
				}
				else if (Level == 2)
				{
					InitiliaseLevel2();
				}
				
				InGame = GameState::GAME;
			}
			//initialise game when clicking on "Quit"
			if (Mouse::isButtonPressed(Mouse::Left) && QuitButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				ClearWin();
				InitialiseMenu();
				InGame = GameState::MENU;
			}
			break;
		}
		case GameState::END:
		{
			//When hovering over retry button
			if (RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				//change text and move position for consistancy
				RetryButton->setString("[Quit]");
				RetryButton->setOutlineThickness(3.0f);
				RetryButton->setPosition(utils::HSWidth - 93.0f, utils::HSHeight + 100.0f);
				
			}
			else
			{
				//reset text when not hovering
				RetryButton->setOutlineThickness(1.0f);
				RetryButton->setString("Quit");
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
				InitialiseMenu();
				InGame = GameState::MENU;
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
	if (BirdBody != NULL)
	{
		World->DestroyBody(BirdBody);
	}

	for (size_t i = 0; i < cloneBodies.size(); i++)
	{
		World->DestroyBody(cloneBodies[i]);
	}

	while (cloneSprites.size() > 0)
	{
		cloneSprites.pop_back();
		cloneBodies.pop_back();
	}
	BirdCount = 1;
	Texture* texture = new Texture;
	texture->loadFromFile("Resources/Textures/grumpybird.png");
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(200 / SCALE, 460 / SCALE);
	BodyDef->type = b2BodyType::b2_dynamicBody;
	b2Body* Body = World->CreateBody(BodyDef);
	b2CircleShape* Shape = new b2CircleShape;
	Shape->m_radius = (60 / 4) / SCALE;
	b2FixtureDef* FixtureDef = new b2FixtureDef;
	FixtureDef->density = 0.8f;
	FixtureDef->friction = 0.7f;
	FixtureDef->shape = Shape;
	Body->CreateFixture(FixtureDef);
	sf::Sprite* sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(60 / 2, 60 / 2);
	sprite->setScale(0.6, 0.6);
	BirdSprite = sprite;
	BirdBody = Body;
}

//creat the player bird object Splits
void CGameManager::CreateBird2(float _size)
{
	BirdCount = 2;
	if (BirdBody != NULL)
	{
		World->DestroyBody(BirdBody);
	}
	Texture* texture = new Texture;
	texture->loadFromFile("Resources/Textures/grumpybird2.png");
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(200 / SCALE, 460 / SCALE);
	BodyDef->type = b2BodyType::b2_dynamicBody;
	b2Body* Body = World->CreateBody(BodyDef);
	b2CircleShape* Shape = new b2CircleShape;
	Shape->m_radius = (_size / 4) / SCALE;
	b2FixtureDef* FixtureDef = new b2FixtureDef;
	FixtureDef->density = 0.5f;
	FixtureDef->friction = 0.7f;
	FixtureDef->shape = Shape;
	Body->CreateFixture(FixtureDef);
	sf::Sprite* sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(_size/2 - 5, _size / 2 - 5);
	sprite->setScale(_size / 100, _size / 100);
	BirdSprite = sprite;
	BirdBody = Body;
}

void CGameManager::CreateClones()
{
	BirdCount = 2;
	for (size_t i = 0; i < 3; i++)
	{
		Texture* texture = new Texture;
		texture->loadFromFile("Resources/Textures/grumpybird2.png");
		b2BodyDef* BodyDef = new b2BodyDef;
		BodyDef->position = BirdBody->GetPosition();
		BodyDef->type = b2BodyType::b2_dynamicBody;
		b2Body* Body = World->CreateBody(BodyDef);
		b2CircleShape* Shape = new b2CircleShape;
		Shape->m_radius = (30 / 4) / SCALE;
		b2FixtureDef* FixtureDef = new b2FixtureDef;
		FixtureDef->density = 0.8f;
		FixtureDef->friction = 0.7f;
		FixtureDef->shape = Shape;
		Body->CreateFixture(FixtureDef);
		sf::Sprite* sprite = new Sprite;
		sprite->setTexture(*texture);
		sprite->setOrigin(25, 25);
		sprite->setScale(0.4f, 0.4f);
		Body->SetLinearVelocity(b2Vec2(BirdBody->GetLinearVelocity().x, BirdBody->GetLinearVelocity().y - i * 1));
		cloneSprites.push_back(sprite);
		cloneBodies.push_back(Body);
	}
}

//creat the player bird object divebomb
void CGameManager::CreateBird3()
{
	for (size_t i = 0; i < cloneBodies.size(); i++)
	{
		World->DestroyBody(cloneBodies[i]);
	}

	while (cloneSprites.size() > 0)
	{
		cloneSprites.pop_back();
		cloneBodies.pop_back();
	}

	BirdCount = 3;
	if (BirdBody != NULL)
	{
		World->DestroyBody(BirdBody);
	}
	Texture* texture = new Texture;
	texture->loadFromFile("Resources/Textures/grumpybird3.png");
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(200 / SCALE, 460 / SCALE);
	BodyDef->type = b2BodyType::b2_dynamicBody;
	b2Body* Body = World->CreateBody(BodyDef);
	b2CircleShape* Shape = new b2CircleShape;
	Shape->m_radius = (50 / 4) / SCALE;
	b2FixtureDef* FixtureDef = new b2FixtureDef;
	FixtureDef->density = 1.0f;
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
void CGameManager::JoinObjects(b2Body* _body1, b2Body* _body2, b2Vec2 _anchorB1, b2Vec2 _anchorB2, BJoint _joint, b2Vec2 _wAnchor1, b2Vec2 _wAnchor2)
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
	case BJoint::PULLY:
	{

		b2PulleyJointDef* newJoint3 = new b2PulleyJointDef();
		newJoint3->Initialize(_body1, _body2, b2Vec2(_body1->GetPosition().x, _body1->GetPosition().y + 100), b2Vec2(_body2->GetPosition().x, _body2->GetPosition().y + 100), _body1->GetWorldCenter(), _body2->GetWorldCenter(), 0.5f);
		World->CreateJoint(newJoint3);
		break;
	}
	default:
	{
		break;
	}
	}
	

}