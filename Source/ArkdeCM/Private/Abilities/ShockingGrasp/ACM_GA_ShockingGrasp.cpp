// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ShockingGrasp/ACM_GA_ShockingGrasp.h"
#include "Abilities/Core/ACMT_PlayMontageAndWaitForEvent.h"
#include "Character/ArkdeCMCharacter.h"
#include "Components/SphereComponent.h"
#include "Abilities/ShockingGrasp/ACM_AbsorptionSphere.h"
#include "Components/SphereComponent.h"


//==================================================================================================================//
void UACM_GA_ShockingGrasp::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	montageTask->OnBlendOut.AddDynamic(this, &UACM_GA_ShockingGrasp::OnMontageCompleted);
	montageTask->OnComplete.AddDynamic(this, &UACM_GA_ShockingGrasp::OnMontageCompleted);
	montageTask->OnInterrupted.AddDynamic(this, &UACM_GA_ShockingGrasp::OnMontageCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UACM_GA_ShockingGrasp::OnMontageCancelled);
	montageTask->OnEventReceived.AddDynamic(this, &UACM_GA_ShockingGrasp::EventReceived);

	montageTask->ReadyForActivation();
}

//==================================================================================================================//
void UACM_GA_ShockingGrasp::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

//==================================================================================================================//
void UACM_GA_ShockingGrasp::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//==================================================================================================================//
void UACM_GA_ShockingGrasp::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());
	
	if (!IsValid(character))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == EnableCollisionTag)
	{		
		FTransform spawnTransform = character->GetMesh()->GetSocketTransform(AbilitySocketName);

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		AbsorptionSphere = GetWorld()->SpawnActorDeferred<AACM_AbsorptionSphere>(AbsorptionSphereClass, spawnTransform, character, character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);		
		AbsorptionSphere->AttachToComponent(character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AbilitySocketName);			
		AbsorptionSphere->FinishSpawning(spawnTransform);
				
		AbsorptionSphere->MeeleSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		
	}

	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == DisableCollisionTag)
	{	
		if (IsValid(AbsorptionSphere))
		{
			AbsorptionSphere->MeeleSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			AbsorptionSphere->Destroy();
		}
	}
}
