// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Core/ACM_GameplayAbility.h"
#include "ACM_GA_MagicShield.generated.h"

class UAnimMontage;
class AACM_Shield;
/**
 * 
 */
UCLASS()
class ARKDECM_API UACM_GA_MagicShield : public UACM_GameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* CastMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<AACM_Shield> ShieldClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag ShieldSpawnTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName AbilitySocketName;


public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:

	UFUNCTION()
		void OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);
	
};
