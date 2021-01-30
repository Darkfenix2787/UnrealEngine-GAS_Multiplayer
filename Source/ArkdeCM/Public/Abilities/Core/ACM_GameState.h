// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ACM_GameState.generated.h"

/**
 * 
 */
UCLASS()
class ARKDECM_API AACM_GameState : public AGameStateBase
{
	GENERATED_BODY()

public:

	void CheckWinCondition();
	
};
