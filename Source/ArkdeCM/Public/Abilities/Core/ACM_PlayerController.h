// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ACM_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ARKDECM_API AACM_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	AACM_PlayerController();


public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		float CameraSpeed;
	
public:

	UFUNCTION(BlueprintImplementableEvent)
		void BP_GameConclussion(bool bWasSuccessfull);

	void GameConclussion(bool bWasSuccessfull);

	
};
