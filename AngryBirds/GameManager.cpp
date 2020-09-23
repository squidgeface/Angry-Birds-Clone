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
// Author      : alexander jenkins, raven clancey-peetz
// Mail        : alexander.jenkins@mds.ac.nz, raven.clancey-peetz@mds.ac.nz
//

#include "GameManager.h"
//GameManager initalisation
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
//GameManager destructor
CGameManager::~CGameManager()
{
	ClearLevel1();
	ClearWin();
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
	delete Arrow;
	Arrow = 0;
	delete SlingSprite;
	SlingSprite = 0;
	delete BackgroundSprite;
	BackgroundSprite = 0;
	BirdBody = 0;
	delete BirdSprite;
	BirdSprite = 0;

	Sprites.clear();
	Bodies.clear();
	DestSprites.clear();
	DestBodies.clear();
	EnemySprites.clear();
	EnemyBodies.clear();
	CloneSprites.clear();
	CloneBodies.clear();

	//clear world body list
	while (World->GetBodyCount() > 0)
	{
		World->DestroyBody(World->GetBodyList());
	}
	//clear world joint list
	while (World->GetJointCount() > 0)
	{
		World->DestroyJoint(World->GetJointList());
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
	CreateDestructable(World, 100, 5.0f, utils::HSWidth, 70.0f, "Resources/Textures/plank.png", 1.0f, 1.0f, BShape::BOX, b2BodyType::b2_staticBody);
	DestBodies[0]->SetTransform(DestBodies[0]->GetPosition(),  1.57f);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(DestBodies[0], Bodies[0], b2Vec2(DestBodies[0]->GetPosition().x, DestBodies[0]->GetPosition().y + 50/SCALE), b2Vec2(Bodies[0]->GetPosition().x - 50.0f / SCALE, Bodies[0]->GetPosition().y), BJoint::DIST);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[0], Bodies[1], b2Vec2(Bodies[0]->GetPosition().x + 50.0f / SCALE, Bodies[0]->GetPosition().y), b2Vec2(), BJoint::REVO);
	CreateObject(World, 100, 10, 750, 100, "Resources/Textures/plank.png", BShape::BOX);
	JoinObjects(Bodies[1], Bodies[2], b2Vec2(Bodies[1]->GetPosition().x - 50.0f / SCALE, Bodies[1]->GetPosition().y), b2Vec2(), BJoint::REVO);
	
	CreateObject(World, utils::ScreenWidth, 0.0f, utils::HSWidth, 0.0f, "Resources/Textures/ground.png", BShape::BOX, 1.0f, 1.0f, b2BodyType::b2_staticBody);
	JoinObjects(Bodies[2], Bodies[3], b2Vec2(Bodies[2]->GetPosition().x + 50.0f / SCALE, Bodies[2]->GetPosition().y), b2Vec2(Bodies[3]->GetPosition().x + 250.0f / SCALE, Bodies[3]->GetPosition().y + 0.3f), BJoint::DIST);


	//create ground and ceiling //body[4]
	CreateObject(World, utils::ScreenWidth, 25.0f, utils::HSWidth, utils::ScreenHeight, "Resources/Textures/ground.png", BShape::BOX, 1.0f, 1.0f, b2BodyType::b2_staticBody);
	
	//create background sprite
	Texture* Background = new Texture;
	Background->loadFromFile("Resources/Textures/background2.png");
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
	CreateDestructable(World, 100, 10, 740, 500, "Resources/Textures/plank.png", 2.0, 1.0);
	CreateObject(World, boxSize, boxSize, 725, 540, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 725, 560, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 750, 540, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 750, 560, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	
	CreateDestructable(World, 100, 10, 740, 430, "Resources/Textures/plank.png", 2.0, 1.0);
	CreateObject(World, boxSize, boxSize, 660, 460, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 660, 480, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 820, 460, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	CreateObject(World, boxSize, boxSize, 820, 480, "Resources/Textures/box.png", BShape::BOX, 0.5f, 0.5f);
	//CreateDestructable(World, 100, 10, 740, 570, "Resources/Textures/plank.png");
	//Destructable planks
	
	


	//Destructable enemy
	CreateEnemy(World, 50, 50, 630, 10, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);

	CreateEnemy(World, 50, 50, 725, 470, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);
	CreateEnemy(World, 50, 50, 750, 470, "Resources/Textures/enemy.png", 0.5f, 0.5f, BShape::CIRCLE);

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
	delete Arrow;
	Arrow = 0;
	delete SlingSprite;
	SlingSprite = 0;
	delete BackgroundSprite;
	BackgroundSprite = 0;
	BirdBody = 0;
	delete BirdSprite;
	BirdSprite = 0;

	Sprites.clear();
	Bodies.clear();
	DestSprites.clear();
	DestBodies.clear();
	EnemySprites.clear();
	EnemyBodies.clear();
	CloneSprites.clear();
	CloneBodies.clear();

	//clear world body list
	while (World->GetBodyCount() > 0)
	{
		World->DestroyBody(World->GetBodyList());
	}
	//clear world joint list
	while (World->GetJointCount() > 0)
	{
		World->DestroyJoint(World->GetJointList());
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
			//Draw sprites and buttons
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
				break;
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
				if (Level == 1)
				{
					ClearLevel1();
					InitiliaseLevel1();
				}
				else if (Level == 2)
				{
					ClearLevel2();
					InitiliaseLevel2();
				}
				Reset = true;
			}
			
			//if birds are available to be fired
			if (Keyboard::isKeyPressed(Keyboard::Num1) && BirdsUsed[0] == 0)
			{
				//Create bird 1
				CreateBird();
				BirdBody->SetEnabled(false);
				BirdBody->SetSleepingAllowed(false);
				Reset = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Num2) && BirdsUsed[1] == 0)
			{
				//Create bird 2
				CreateBird2(75.0f);
				BirdBody->SetEnabled(false);
				BirdBody->SetSleepingAllowed(false);
				Reset = true;
			}
			if (Keyboard::isKeyPressed(Keyboard::Num3) && BirdsUsed[2] == 0)
			{
				//Create bird 3
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
					//bird 2 splits into three
					CreateClones();
					BirdBody->SetEnabled(false);
					BirdBody->SetTransform(b2Vec2(-100, 0), 0);
					Reset = true;
				}
				else if (BirdCount == 3)
				{
					//Bird 3 stops and becomes heavy
					Reset = true;
					BirdBody->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
					BirdBody->SetGravityScale(3.0f);
				}
			}

			//Show direction vector when mouse is held and dragged for firing
			if (Mouse::isButtonPressed(Mouse::Left) && BirdSprite->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()) || IsPressed && Mouse::isButtonPressed(Mouse::Left))
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
					//Set bounds for bird sprite movement when firing
					//y bounds
					if (BirdBody->GetTransform().p.y > (utils::ScreenHeight - 15.0f)/SCALE)
					{
						BirdBody->SetTransform(b2Vec2(BirdBody->GetTransform().p.x, (utils::ScreenHeight - 15.0f) / SCALE), 0.0f);
					}
					else if (BirdBody->GetTransform().p.y < (utils::HSHeight) / SCALE)
					{
						BirdBody->SetTransform(b2Vec2(BirdBody->GetTransform().p.x, (utils::HSHeight) / SCALE), 0.0f);
					}
					//x bounds
					if (BirdBody->GetTransform().p.x > 200 / SCALE)
					{
						BirdBody->SetTransform(b2Vec2(200 / SCALE, BirdBody->GetTransform().p.y), 0.0f);
					}
					else if (BirdBody->GetTransform().p.x <= 0)
					{
						BirdBody->SetTransform(b2Vec2(0, BirdBody->GetTransform().p.y), 0.0f);
					}

					//create a force vector between initial position and mouse position for transforming the arrow sprite
					vec2 forceVec = vec2(Mouse::getPosition(*Window).x - MouseInitialX, Mouse::getPosition(*Window).y - MouseInitialY);
					forceVec = normalize(forceVec);
					//Scale and rotate the arrow sprite to show trajectory and power of firing
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
				//distance of the initial and final position
				float distance = Distancev2(vec2(MouseInitialX, MouseInitialY), vec2(MouseReleaseX, MouseReleaseY));
				//only fire if the distance is greater than 0
				if (distance > 0)
				{
					//hide arrow sprite
					Arrow->scale(0.0f, 0.0f);
					//run fire code
					if (Reset)
						IsFired = true;
				}
				else
				{
					//reset bird to starting position
						BirdBody->SetEnabled(false);
						BirdBody->SetTransform(b2Vec2(200 / SCALE, 460 / SCALE), 0);
				}
			}

			//When bird is fired
			if (IsFired)
			{
				//create a force vector between the mouse and initial bird position
				vec2 forceVec = vec2(MouseReleaseX - MouseInitialX, MouseReleaseY - MouseInitialY);
				forceVec = normalize(forceVec);
				//set this bird as being fired
				BirdsUsed[BirdCount - 1] = 1;
				//Get the distance between the two postions
				float distance = Distancev2(vec2(MouseInitialX, MouseInitialY), vec2(MouseReleaseX, MouseReleaseY));
				//Reverse the distance direction and multiple by factor
				forceVec *= -distance * 4;
				//apply force to bird body
				BirdBody->SetEnabled(true);
				BirdBody->ApplyForceToCenter(b2Vec2(forceVec.x, forceVec.y), true);
				//reset for next bird
				IsFired = false;
				Reset = false;
			}


			/** Simulate the world **/
			World->Step(1 / 60.f, 8, 3);
			//Clear screen with white
			Window->clear(Color::White);

			//Drawing spites (order matters)
			Window->draw(*BackgroundSprite);

			//Set static sprites to follow b2Body position and rotation
			for (size_t i = 0; i < Sprites.size(); i++)
			{
				Sprites[i]->setPosition(SCALE * Bodies[i]->GetPosition().x, Bodies[i]->GetPosition().y * SCALE);
				Sprites[i]->setRotation(Bodies[i]->GetAngle() * 180 / b2_pi);
				Window->draw(*Sprites[i]);
			}
			//Set destructable sprites to follow b2Body position and rotation
			for (size_t i = 0; i < DestSprites.size(); i++)
			{
				DestSprites[i]->setPosition(SCALE * DestBodies[i]->GetPosition().x, DestBodies[i]->GetPosition().y * SCALE);
				DestSprites[i]->setRotation(DestBodies[i]->GetAngle() * 180 / b2_pi);
				Window->draw(*DestSprites[i]);
			}
			//Set enemy sprites to follow b2Body position and rotation
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
			//Set clone sprites to follow b2Body position and rotation
			for (size_t i = 0; i < CloneSprites.size(); i++)
			{
				CloneSprites[i]->setPosition(SCALE * CloneBodies[i]->GetPosition().x, CloneBodies[i]->GetPosition().y * SCALE);
				CloneSprites[i]->setRotation(CloneBodies[i]->GetAngle() * 180 / b2_pi);
				Window->draw(*CloneSprites[i]);
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
			for (size_t j = 0; j < CloneSprites.size(); j++)
			{
				for (size_t i = 0; i < DestSprites.size(); i++)
				{
					if (CloneSprites[j]->getGlobalBounds().intersects(DestSprites[i]->getGlobalBounds()))
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
				//level 1 and level 2 have different floor sprites for enemy collision
				if (Level == 1)
				{
					if (BirdSprite->getGlobalBounds().intersects(EnemySprites[i]->getGlobalBounds()) || EnemySprites[i]->getGlobalBounds().intersects(Sprites[6]->getGlobalBounds()))
					{
						World->DestroyBody(EnemyBodies[i]);
						EnemyBodies.erase(EnemyBodies.begin() + i);
						EnemySprites.erase(EnemySprites.begin() + i);
						EnemySize--;
					}
				}
				else
				{
					if (BirdSprite->getGlobalBounds().intersects(EnemySprites[i]->getGlobalBounds()) || EnemySprites[i]->getGlobalBounds().intersects(Sprites[4]->getGlobalBounds()))
					{
						World->DestroyBody(EnemyBodies[i]);
						EnemyBodies.erase(EnemyBodies.begin() + i);
						EnemySprites.erase(EnemySprites.begin() + i);
						EnemySize--;
					}
				}
			}

			//check for clone sprite enemy collision
			for (size_t j = 0; j < CloneSprites.size(); j++)
			{
				for (size_t i = 0; i < EnemySprites.size(); i++)
				{
					if (CloneSprites[j]->getGlobalBounds().intersects(EnemySprites[i]->getGlobalBounds()))
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

			//when all enemies are dead (or debug pres 'A') - initialise win level
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


			//get current elapsed time of frame

			deltaTime = deltaClock.restart();
			//check for time passed after all birds are fired for lose condition
			if (BirdsUsed[0] == 1 && BirdsUsed[1] == 1 && BirdsUsed[2] == 1 && (Level == 1 || Level == 2))
			{
				Timer += deltaTime.asSeconds();
				//When last bird fired and 5 seconds has passed
				if (Timer >= 5)
				{
					if (Level == 1)
					{
						ClearLevel1();
					}
					else if (Level == 2)
					{
						ClearLevel2();
					}
					//initialise lose screen
					InitialiseLose();
					InGame = GameState::LOSE;
				}
			}

			break;
		}
		case GameState::WIN:
		{
			//When hovering over continue button
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
				break;
			}
			//initialise game when clicking on "Quit"
			if (QuitButton != NULL)
			{
				if (Mouse::isButtonPressed(Mouse::Left) && QuitButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
				{
					ClearWin();
					InitialiseMenu();
					InGame = GameState::MENU;
				}
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

			Window->draw(*RetryButton);
			Window->draw(*QuitButton);
			//display window
			Window->display();

			//initialise game when clicking on "Retry"
			if (Mouse::isButtonPressed(Mouse::Left) && RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				
				if (Level == 1)
				{
					InitiliaseLevel1();
				}
				else if (Level == 2)
				{
					InitiliaseLevel2();
				}
				Timer = 0;
				ClearWin();
				InGame = GameState::GAME;
				break;
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
			Window->draw(*RetryButton);
			Window->draw(*QuitButton);
			//display window
			Window->display();

			//initialise game when clicking on "Retry"
			if (Mouse::isButtonPressed(Mouse::Left) && RetryButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				ClearGameOver();
				InitiliaseLevel1();
				InGame = GameState::GAME;
				break;
			}

			//initialise game when clicking on "Quit"
			if (Mouse::isButtonPressed(Mouse::Left) && QuitButton->getGlobalBounds().intersects(MouseSprite->getGlobalBounds()))
			{
				exit(0);
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

//creat the player bird1 object
void CGameManager::CreateBird()
{
	//check if birdbody is not null before destroying it
	if (BirdBody != NULL)
	{
		World->DestroyBody(BirdBody);
	}
	//destroy clone bodies if found
	for (size_t i = 0; i < CloneBodies.size(); i++)
	{
		World->DestroyBody(CloneBodies[i]);
	}
	//clear clone vectors
	while (CloneSprites.size() > 0)
	{
		CloneSprites.pop_back();
		CloneBodies.pop_back();
	}
	//set to bird 1 as active bird
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

//creat the player bird2 object that Splits
void CGameManager::CreateBird2(float _size)
{
	//set to bird 2 as active bird
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
	Shape->m_radius = ((_size - 10) / 4) / SCALE;
	b2FixtureDef* FixtureDef = new b2FixtureDef;
	FixtureDef->density = 0.5f;
	FixtureDef->friction = 0.7f;
	FixtureDef->shape = Shape;
	Body->CreateFixture(FixtureDef);
	sf::Sprite* sprite = new Sprite;
	sprite->setTexture(*texture);
	sprite->setOrigin(_size/2 - 10, _size / 2 - 10);
	sprite->setScale(_size / 100, _size / 100);
	BirdSprite = sprite;
	BirdBody = Body;
}

void CGameManager::CreateClones()
{
	//create three small bird clones as the same location as bird 2
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
		//set velocity of this bird as the same as bird2 plus an offset ased on which of the three birds it is
		Body->SetLinearVelocity(b2Vec2(BirdBody->GetLinearVelocity().x, BirdBody->GetLinearVelocity().y - i * 1));
		CloneSprites.push_back(sprite);
		CloneBodies.push_back(Body);
	}
}

//creat the player bird 3 object - the divebomb bird
void CGameManager::CreateBird3()
{
	for (size_t i = 0; i < CloneBodies.size(); i++)
	{
		World->DestroyBody(CloneBodies[i]);
	}

	while (CloneSprites.size() > 0)
	{
		CloneSprites.pop_back();
		CloneBodies.pop_back();
	}
	//set to bird 3 as active bird
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
void CGameManager::CreateDestructable(b2World* World, float SizeX, float SizeY, float PosX, float PosY, String texPath, float _scaleX, float _scaleY, BShape _shape, b2BodyType _type)
{
	Texture* texture = new Texture;
	texture->loadFromFile(texPath);
	b2BodyDef* BodyDef = new b2BodyDef;
	BodyDef->position = b2Vec2(PosX / SCALE, PosY / SCALE);
	BodyDef->type = _type;
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
		//set anchor positions of the pully at 100 above the bodies
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