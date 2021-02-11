// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Controller.h"
#include "ArkdeCMGameMode.generated.h"

class APlayerStart;
class AACM_Collectable;

UCLASS(minimalapi)
class AArkdeCMGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArkdeCMGameMode();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerKilled(AController* VictimController);

	void InitPlayerStarts();

	void SetupWorldSpawners();

protected:

	virtual void BeginPlay() override;	

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<TSubclassOf<AACM_Collectable>> SpawnableCollectables;

	TArray<APlayerStart*> WorldPlayerStarts;
};



