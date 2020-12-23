// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/MagicShield/ACM_GA_MagicShield.h"
#include "Character/ArkdeCMCharacter.h"
#include "Abilities/Core/ACMT_PlayMontageAndWaitForEvent.h"
#include "Abilities/MagicShield/ACM_Shield.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"

//==================================================================================================================//
void UACM_GA_MagicShield::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	montageTask->OnBlendOut.AddDynamic(this, &UACM_GA_MagicShield::OnMontageCompleted);
	montageTask->OnComplete.AddDynamic(this, &UACM_GA_MagicShield::OnMontageCompleted);
	montageTask->OnInterrupted.AddDynamic(this, &UACM_GA_MagicShield::OnMontageCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UACM_GA_MagicShield::OnMontageCancelled);
	montageTask->OnEventReceived.AddDynamic(this, &UACM_GA_MagicShield::EventReceived);

	montageTask->ReadyForActivation();

}

//==================================================================================================================//
void UACM_GA_MagicShield::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

//==================================================================================================================//
void UACM_GA_MagicShield::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//==================================================================================================================//
void UACM_GA_MagicShield::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == ShieldSpawnTag)
	{
		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());
		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FTransform spawnTransform = character->GetMesh()->GetSocketTransform(AbilitySocketName);		

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AACM_Shield* magicShield = GetWorld()->SpawnActor<AACM_Shield>(ShieldClass, spawnTransform, spawnParams);	
	}
}



