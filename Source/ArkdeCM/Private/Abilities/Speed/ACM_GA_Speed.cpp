// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Speed/ACM_GA_Speed.h"
#include "Character/ArkdeCMCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include <GameplayAbilities/Private/Abilities/GameplayAbility.cpp>


UACM_GA_Speed::UACM_GA_Speed()
{
	AbilityDuration = 5.f;
	CurrentWalkSpeed = 0;
	AbilityWalkSpeed = 2000;
}

void UACM_GA_Speed::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());

		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
		
		CurrentWalkSpeed = character->GetCharacterMovement()->MaxWalkSpeed;
		character->GetCharacterMovement()->MaxWalkSpeed = AbilityWalkSpeed;

		UAbilityTask_WaitDelay* DelayDelegate = UAbilityTask_WaitDelay::WaitDelay(this, AbilityDuration);
		DelayDelegate->OnFinish.AddDynamic(this, &UACM_GA_Speed::AbilityDurationTimer);
		DelayDelegate->Activate();

		//GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ability, this, &UACM_GA_Speed::AbilityDurationTimer, AbilityDuration, false);		
	}
}

void UACM_GA_Speed::AbilityDurationTimer()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint ability out"));
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sprint ability in"));
		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());

		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		character->GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Ability);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}	
}
