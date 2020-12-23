// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Core/ACM_GameplayAbility.h"
#include "ACM_GA_FireBolt.generated.h"

class UAnimMontage;
class AACM_Projectile;

/**
 *
 */
UCLASS()
class ARKDECM_API UACM_GA_FireBolt : public UACM_GameplayAbility
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ProjectileSpeed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		float ProjectileRange;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* CastMontage;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<AACM_Projectile> ProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		FGameplayTag ProjectileSpawnTag;

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