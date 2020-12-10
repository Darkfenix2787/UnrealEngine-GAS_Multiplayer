// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMGA_FireBolt.h"
#include "ArkdeCM/ArkdeCMCharacter.h"
#include "ACMT_PlayMontageAndWaitForEvent.h"
#include "ACM_Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"



//==================================================================================================================//
void UACMGA_FireBolt::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
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

	montageTask->OnBlendOut.AddDynamic(this, &UACMGA_FireBolt::OnMontageCompleted);
	montageTask->OnComplete.AddDynamic(this, &UACMGA_FireBolt::OnMontageCompleted);
	montageTask->OnInterrupted.AddDynamic(this, &UACMGA_FireBolt::OnMontageCancelled);
	montageTask->OnCancelled.AddDynamic(this, &UACMGA_FireBolt::OnMontageCancelled);
	montageTask->OnEventReceived.AddDynamic(this, &UACMGA_FireBolt::EventReceived);

	montageTask->ReadyForActivation();

}

//==================================================================================================================//
void UACMGA_FireBolt::OnMontageCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

//==================================================================================================================//
void UACMGA_FireBolt::OnMontageCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

//==================================================================================================================//
void UACMGA_FireBolt::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ENetRole Role = GetOwningActorFromActorInfo()->GetLocalRole();
	if (GetAvatarActorFromActorInfo()->GetLocalRole() == ROLE_Authority && EventTag == ProjectileSpawnTag)
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
		fireBolt->FinishSpawning(spawnTransform);
	}
}
