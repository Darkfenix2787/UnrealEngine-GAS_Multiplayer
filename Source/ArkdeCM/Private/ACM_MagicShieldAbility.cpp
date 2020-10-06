// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_MagicShieldAbility.h"
#include "ArkdeCM/ArkdeCMCharacter.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "ArkdeCM/Public/ACM_Shield.h"



UACM_MagicShieldAbility::UACM_MagicShieldAbility()
{
	SocketName = "MagicShieldSocket";
	MagicShieldTime = 5.0f;
}

void UACM_MagicShieldAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{	
	OwnerCharacter = Cast<AArkdeCMCharacter>(GetActorInfo().OwnerActor);

	if (IsValid(OwnerCharacter))
	{
		bool bCommit = CommitAbility(Handle, ActorInfo, ActivationInfo);

		if (bCommit)
		{
			MyAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
			if (IsValid(MyAnimInstance) && IsValid(CastShieldMontage))
			{
				MyAnimInstance->Montage_Play(CastShieldMontage);
				NewShield = Cast<AACM_Shield>(GetWorld()->SpawnActor<AACM_Shield>(MagicShieldClass));
				if (IsValid(NewShield))
				{
					FAttachmentTransformRules AttachTransformRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
					NewShield->AttachToComponent(OwnerCharacter->GetMesh(), AttachTransformRules, SocketName);
					GetWorld()->GetTimerManager().SetTimer(TimerHandle_DestroyMagicShield, &UACM_MagicShieldAbility::MagicShieldDestroy, MagicShieldTime,false);					
				}
				
				EndAbility(Handle, ActorInfo, ActivationInfo, true, true);				
			}
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	
}

void UACM_MagicShieldAbility::MagicShieldDestroy()
{
	if (IsValid(NewShield))
	{
		NewShield->Destroy();
	}
}

