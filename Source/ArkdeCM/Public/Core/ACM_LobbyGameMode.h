// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "..\..\ArkdeCMGameMode.h"
#include "ACM_LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ARKDECM_API AACM_LobbyGameMode : public AArkdeCMGameMode
{
	GENERATED_BODY()

	AACM_LobbyGameMode();

public:

	/** Called after a successful login.  This is the first place it is safe to call replicated functions on the PlayerController. */
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
		int PlayersToTravelMaxAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
		int LobbyCurrentPlayers;

	
};
