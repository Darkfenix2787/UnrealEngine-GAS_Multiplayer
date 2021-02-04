// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Core/ACM_GameplayAbility.h"
#include "ACM_GA_Speed.generated.h"

/**
 * 
 */
UCLASS()
class ARKDECM_API UACM_GA_Speed : public UACM_GameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
		float AbilityDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
		int AbilityWalkSpeed;

	int CurrentWalkSpeed;

	FTimerHandle TimerHandle_Ability;

public:

	UACM_GA_Speed();

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION()
		void AbilityDurationTimer();
	
};
