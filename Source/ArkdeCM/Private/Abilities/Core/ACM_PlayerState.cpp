// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Core/ACM_PlayerState.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Core/ACM_AttributeSet.h"
#include "Character/ArkdeCMCharacter.h"
#include "GameplayEffectExtension.h"


//==================================================================================================================//
AACM_PlayerState::AACM_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System Component"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	StateAttributeSet = CreateDefaultSubobject<UACM_AttributeSet>(TEXT("Attribute Set"));

	NetUpdateFrequency = 100.0f;

	KillCount = 0;
}

//==================================================================================================================//
bool AACM_PlayerState::IsAlive() const
{	
	return GetHealth() > 0.f;
}

//==================================================================================================================//
float AACM_PlayerState::GetHealth() const
{
	return GetAttributeSet()->GetHealth();
}

//==================================================================================================================//
float AACM_PlayerState::GetMaxHealth() const
{
	return GetAttributeSet()->GetMaxHealth();
}

//==================================================================================================================//
float AACM_PlayerState::GetHealthRegen() const
{
	return GetAttributeSet()->GetHealthRegen();
}

//==================================================================================================================//
UAbilitySystemComponent* AACM_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//==================================================================================================================//
UACM_AttributeSet* AACM_PlayerState::GetAttributeSet() const
{
	return StateAttributeSet;
}

//==================================================================================================================//
void AACM_PlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{		
		//CallBack Attribute Change
		HealthChangeDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetHealthAttribute()).AddUObject(this, &AACM_PlayerState::OnHealthChanged);
	}
}

//==================================================================================================================//
void AACM_PlayerState::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (!IsAlive() && IsValid(AbilitySystemComponent) && GetLocalRole() == ROLE_Authority)
	{		
		AArkdeCMCharacter* characterRef = Cast<AArkdeCMCharacter>(GetPawn());
		if (IsValid(characterRef))
		{
			AActor* KillerActor = Data.GEModData->EffectSpec.GetEffectContext().GetEffectCauser();
			if (IsValid(KillerActor))
			{
				AArkdeCMCharacter* KillerCharacter = Cast<AArkdeCMCharacter>(KillerActor);
				if (IsValid(KillerCharacter))
				{
					characterRef->Server_Die(KillerCharacter);
				}
			}			
		}
	}
}

//==================================================================================================================//
void AACM_PlayerState::ScoreKill()
{
	KillCount++;
}
