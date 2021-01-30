// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "ACM_PlayerState.generated.h"

class UAbilitySystemComponent;
class UACM_AttributeSet;

/**
 * 
 */
UCLASS()
class ARKDECM_API AACM_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AACM_PlayerState();

	bool IsAlive() const;

	float GetHealth() const;

	float GetMaxHealth() const;

	float GetHealthRegen() const;

	UAbilitySystemComponent* GetAbilitySystemComponent() const;

	UACM_AttributeSet* GetAttributeSet() const;
	
	virtual void BeginPlay() override;	

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);	

	void ScoreKill();

public:

	UPROPERTY()
		UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		UACM_AttributeSet* StateAttributeSet;	

	FDelegateHandle HealthChangeDelegateHandle;

public:

	int KillCount;

	

	
};
