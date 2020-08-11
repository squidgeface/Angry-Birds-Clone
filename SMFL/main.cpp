#include "utils.h"
#include "GameManager.h"

int main()
{
	CGameManager* GameManager = new CGameManager();
	
	GameManager->InitialiseWindow();
	GameManager->InitialiseWorld();
	GameManager->Update();

	return 0;
}
