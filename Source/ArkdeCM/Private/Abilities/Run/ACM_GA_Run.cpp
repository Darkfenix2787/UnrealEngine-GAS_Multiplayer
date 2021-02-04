// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Run/ACM_GA_Run.h"
#include "Character/ArkdeCMCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"


//===========================================================================================================================================================//
UACM_GA_Run::UACM_GA_Run()
{
	AbilityDuration = 5.f;
	CurrentWalkSpeed = 0;
	AbilityWalkSpeed = 2000;
}


//===========================================================================================================================================================//
void UACM_GA_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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


		DelayDelegate = UAbilityTask_WaitDelay::WaitDelay(this, AbilityDuration);
		DelayDelegate->OnFinish.AddDynamic(this, &UACM_GA_Run::NormalizeSpeed);
		DelayDelegate->Activate();
		
		//GetWorld()->GetTimerManager().SetTimer(TimerHandle_Ability, this, &UACM_GA_Run::NormalizeSpeed, AbilityDuration, false);
	}
}
	
	//===========================================================================================================================================================//
void UACM_GA_Run::NormalizeSpeed()
{	
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority)
	{
		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());

		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		character->GetCharacterMovement()->MaxWalkSpeed = CurrentWalkSpeed;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Ability);		
		DelayDelegate->EndTask();
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
