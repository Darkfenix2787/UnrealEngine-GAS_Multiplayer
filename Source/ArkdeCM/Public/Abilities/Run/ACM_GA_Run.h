// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Core/ACM_GameplayAbility.h"
#include "ACM_GA_Run.generated.h"

UCLASS()
class ARKDECM_API UACM_GA_Run : public UACM_GameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
		float AbilityDuration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Variable")
		int AbilityWalkSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Gameplay Ability System")
		FGameplayTag StaminaRegenEffectTag;

	int CurrentWalkSpeed;

	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
		TSubclassOf<UGameplayEffect> StaminaCostEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
		UAbilitySystemComponent* CharacterAbilitySystemComponent;	

public:

	UACM_GA_Run();


public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;		
};
