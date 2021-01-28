// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/WindSwirl/ACM_GA_WindSwirl.h"
#include "Character/ArkdeCMCharacter.h"
#include "Abilities/Core/ACMT_PlayMontageAndWaitForEvent.h"
#include "Abilities/WindSwirl/ACM_WindSphere.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"

//==================================================================================================================//
void UACM_GA_WindSwirl::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	UACMT_PlayMontageAndWaitForEvent* montageTask = UACMT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent
	(
		this,
		NAME_None,
		CastMontage,
		FGameplayTagContainer(),
		1.f,
		NAME_None,
		false,
		1.f
	);

	montageTask->OnBlendOut.AddDynamic(this, &UACM_GA_WindSwirl::OnMontageCompleted);
	montageTask->OnComplete.AddDynamic(this, &UACM_GA_WindSwirl::OnMontageCompleted);
	montageTask->OnInterrupted.AddDynamic(this, &UACM_GA_WindSwirl::OnMontageCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UACM_GA_WindSwirl::OnMontageCancelled);
	montageTask->OnEventReceived.AddDynamic(this, &UACM_GA_WindSwirl::EventReceived);

	montageTask->ReadyForActivation();

}

//==================================================================================================================//
void UACM_GA_WindSwirl::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

//==================================================================================================================//
void UACM_GA_WindSwirl::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//==================================================================================================================//
void UACM_GA_WindSwirl::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == WindSphereSpawnTag)
	{
		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());
		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}	

		FTransform spawnTransform = character->GetActorTransform();

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AACM_WindSphere* windSphere = GetWorld()->SpawnActorDeferred<AACM_WindSphere>(WindSphereClass, spawnTransform, character, character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);		
		windSphere->FinishSpawning(spawnTransform);		
	}
}

