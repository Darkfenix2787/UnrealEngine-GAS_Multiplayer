// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/Run/ACM_GA_Run.h"
#include "Character/ArkdeCMCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "AbilitySystemComponent.h"


//===========================================================================================================================================================//
UACM_GA_Run::UACM_GA_Run()
{
	AbilityDuration = 5.f;	
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

		int CurrentWalkSpeed = character->GetCharacterMovement()->MaxWalkSpeed;
		CharacterAbilitySystemComponent = character->AbilitySystemComponent;

		if (IsValid(CharacterAbilitySystemComponent))
		{
			if (CurrentWalkSpeed == AbilityWalkSpeed)
			{
				character->NormalizeSpeed();
			}
			else
			{
				character->GetCharacterMovement()->MaxWalkSpeed = AbilityWalkSpeed;

				FGameplayEffectContextHandle EffectContext = CharacterAbilitySystemComponent->MakeEffectContext();
				EffectContext.AddSourceObject(this);

				if (IsValid(StaminaCostEffect) && StaminaRegenEffectTag.IsValid())
				{
					FGameplayEffectSpecHandle newHandle = CharacterAbilitySystemComponent->MakeOutgoingSpec(StaminaCostEffect, 1.f, EffectContext);
					if (newHandle.IsValid())
					{
						FActiveGameplayEffectHandle ActiveGEHandle = CharacterAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*newHandle.Data.Get());	

						FGameplayTagContainer EffectTagsToRemove;
						EffectTagsToRemove.AddTag(StaminaRegenEffectTag);
						CharacterAbilitySystemComponent->RemoveActiveEffectsWithAppliedTags(EffectTagsToRemove);
					}
				}
			}
		}

		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
	}
}