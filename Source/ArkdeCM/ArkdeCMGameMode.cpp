// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArkdeCMGameMode.h"
#include "Character/ArkdeCMCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Abilities/Core/ACM_PlayerController.h"
#include "Abilities/Core/ACM_GameState.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "Objects/ACM_Spawner.h"

//==================================================================================================================//
AArkdeCMGameMode::AArkdeCMGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

//==================================================================================================================//
void AArkdeCMGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitPlayerStarts();
	SetupWorldSpawners();
}

//==================================================================================================================//
void AArkdeCMGameMode::PlayerKilled(AController* VictimController)
{
	AACM_PlayerController* ArkdeVictimController = Cast<AACM_PlayerController>(VictimController);
	if (IsValid(ArkdeVictimController))
	{
		ArkdeVictimController->GameConclussion(false);
	}

	AACM_GameState* WorldGameState = GetWorld()->GetGameState<AACM_GameState>();

	if (IsValid(WorldGameState))
	{
		WorldGameState->CheckWinCondition();
	}
}

//==================================================================================================================//
void AArkdeCMGameMode::InitPlayerStarts()
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* CurrentPlayerStart = *It;

		if (IsValid(CurrentPlayerStart))
		{
			WorldPlayerStarts.Add(CurrentPlayerStart);
		}
	}
}

//==================================================================================================================//
void AArkdeCMGameMode::SetupWorldSpawners()
{
	int CollectableIndex = 0;
	for (TActorIterator<AACM_Spawner> It(GetWorld()); It; ++It)
	{
		AACM_Spawner* CurrentSpawner = *It;
		CurrentSpawner->SetCollectableSubClass(SpawnableCollectables[CollectableIndex % SpawnableCollectables.Num()]);
		CurrentSpawner->RespawnCollectable();
		CollectableIndex++;
	}
}

//==================================================================================================================//
AActor* AArkdeCMGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	APlayerStart* BestStart = nullptr;
	int32 RandomIndex = FMath::RandHelper(WorldPlayerStarts.Num() - 1);

	if (RandomIndex >= 0 && WorldPlayerStarts.Num() > 0)
	{
		BestStart = WorldPlayerStarts[RandomIndex];
		WorldPlayerStarts.RemoveAt(RandomIndex);
	}

	if (IsValid(BestStart))
	{
		return BestStart;
	}
	else
	{
		return Super::ChoosePlayerStart_Implementation(Player);
	}
}
