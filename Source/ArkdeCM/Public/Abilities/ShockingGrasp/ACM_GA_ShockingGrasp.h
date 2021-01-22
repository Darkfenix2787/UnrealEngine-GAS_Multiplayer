// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Core/ACM_GameplayAbility.h"
#include "ACM_GA_ShockingGrasp.generated.h"

class AArkdeCMCharacter;
class AACM_AbsorptionSphere;

/**
 * 
 */
UCLASS()
class ARKDECM_API UACM_GA_ShockingGrasp : public UACM_GameplayAbility
{
	GENERATED_BODY()

public:		

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* CastMontage;		
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag EnableCollisionTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag DisableCollisionTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<AACM_AbsorptionSphere> AbsorptionSphereClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FName AbilitySocketName;

	UPROPERTY(BlueprintReadOnly)
		AACM_AbsorptionSphere* AbsorptionSphere;


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
