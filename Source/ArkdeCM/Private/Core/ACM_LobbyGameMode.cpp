// Fill out your copyright notice in the Description page of Project Settings.


#include "Core\ACM_LobbyGameMode.h"
//===========================================================================================================================================================//
AACM_LobbyGameMode::AACM_LobbyGameMode()
{
	PlayersToTravelMaxAmount = 3;
	LobbyCurrentPlayers = 0;
}

//===========================================================================================================================================================//
void AACM_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (IsValid(NewPlayer))
	{
		LobbyCurrentPlayers++;

		if (LobbyCurrentPlayers >= PlayersToTravelMaxAmount)
		{
			UWorld* GameWorld = GetWorld();

			if (IsValid(GameWorld))
			{
				bUseSeamlessTravel = true;
				GameWorld->ServerTravel("/Game/Maps/BattleRoyalMap?listen");
			}
		}
	}
}
