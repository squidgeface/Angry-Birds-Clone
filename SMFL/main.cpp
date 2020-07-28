#include "utils.h"
#include "GameManager.h"

int main()
{
	CGameManager* GameManager = new CGameManager();
	
	GameManager->InitialiseWindow();
	GameManager->Update();

	return 0;
}
