// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/FireBolt/ACM_GA_FireBolt.h"
#include "Character/ArkdeCMCharacter.h"
#include "Abilities/Core/ACMT_PlayMontageAndWaitForEvent.h"
#include "Abilities/FireBolt/ACM_Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"



//==================================================================================================================//
void UACM_GA_FireBolt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	montageTask->OnBlendOut.AddDynamic(this, &UACM_GA_FireBolt::OnMontageCompleted);
	montageTask->OnComplete.AddDynamic(this, &UACM_GA_FireBolt::OnMontageCompleted);
	montageTask->OnInterrupted.AddDynamic(this, &UACM_GA_FireBolt::OnMontageCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UACM_GA_FireBolt::OnMontageCancelled);
	montageTask->OnEventReceived.AddDynamic(this, &UACM_GA_FireBolt::EventReceived);

	montageTask->ReadyForActivation();

}

//==================================================================================================================//
void UACM_GA_FireBolt::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

//==================================================================================================================//
void UACM_GA_FireBolt::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//==================================================================================================================//
void UACM_GA_FireBolt::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (GetOwningActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == ProjectileSpawnTag)
	{
		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(GetAvatarActorFromActorInfo());
		if (!IsValid(character))
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}

		FVector startLoc = character->GetMesh()->GetSocketLocation(AbilitySocketName);
		FVector endLoc = character->GetCameraBoom()->GetComponentLocation() + (character->GetFollowCamera()->GetForwardVector() * ProjectileRange);
		FRotator projectileRotation = UKismetMathLibrary::FindLookAtRotation(startLoc, endLoc);

		FTransform spawnTransform = character->GetMesh()->GetSocketTransform(AbilitySocketName);
		spawnTransform.SetRotation(projectileRotation.Quaternion());

		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AACM_Projectile* fireBolt = GetWorld()->SpawnActorDeferred<AACM_Projectile>(ProjectileClass, spawnTransform, GetOwningActorFromActorInfo(), character, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		fireBolt->Multicast_IgnoreActor(character);
		fireBolt->Range = ProjectileRange;
		fireBolt->SetProjectileInitialSpeed(ProjectileSpeed);
		fireBolt->FinishSpawning(spawnTransform);
	}
}


