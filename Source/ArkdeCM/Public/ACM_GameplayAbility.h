// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArkdeCM/ArkdeCM.h"
#include "ACM_GameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class ARKDECM_API UACM_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UACM_GameplayAbility();

public:

	/* ---- Ability Input IDs Start ----*/

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability")
		EACM_AbilityInputID AbilityInputID;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Gameplay Ability")
		EACM_AbilityInputID AbilityID;

	/* ---- Ability Input IDs End ----*/

	
};
