// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACM_GameplayAbility.h"
#include "ACM_MagicShieldAbility.generated.h"


class AArkdeCMCharacter;
class UAnimMontage;
class UAnimInstance;
class AACM_Shield;
class AArkdeCMCharacter;

/**
 * 
 */
UCLASS()
class ARKDECM_API UACM_MagicShieldAbility : public UACM_GameplayAbility
{
	GENERATED_BODY()


public:

	UACM_MagicShieldAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void MagicShieldDestroy();
	

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Owner")
		AArkdeCMCharacter* OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Owner")
		AACM_Shield* NewShield;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
		UAnimMontage* CastShieldMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawner")
		TSubclassOf<AACM_Shield> MagicShieldClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
		FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ShieldProperty")
		float MagicShieldTime;

	UAnimInstance* MyAnimInstance;
	FTimerHandle TimerHandle_DestroyMagicShield;

	
};
