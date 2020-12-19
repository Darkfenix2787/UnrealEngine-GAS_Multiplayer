// Fill out your copyright notice in the Description page of Project Settings.


#include "ACMT_PlayMontageAndWaitForEvent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "AbilitySystemGlobals.h"

//==================================================================================================================//
UACMT_PlayMontageAndWaitForEvent::UACMT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Rate = 1.f;
	bStopWhenAbilityEnds = true;
}

//==================================================================================================================//
void UACMT_PlayMontageAndWaitForEvent::Activate()
{
	if (Ability == nullptr)
	{
		return;
	}

	bool bPlayerMontage = false;

	if (AbilitySystemComponent)
	{
		const FGameplayAbilityActorInfo* actorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* animInstance = actorInfo->GetAnimInstance();

		if (animInstance != nullptr)
		{
			EventHandle = AbilitySystemComponent->AddGameplayEventTagContainerDelegate(EventTags, FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &UACMT_PlayMontageAndWaitForEvent::OnGameplayEvent));

			if (AbilitySystemComponent->PlayMontage(Ability, Ability->GetCurrentActivationInfo(), MontageToPlay, Rate, StartSection) > 0.f)
			{
				if (ShouldBroadcastAbilityTaskDelegates()==false)
				{
					return;
				}

				CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &UACMT_PlayMontageAndWaitForEvent::OnAbilityCancelled);

				BlendingOutDelegate.BindUObject(this, &UACMT_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
				animInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate, MontageToPlay);

				MontageEndedDelegate.BindUObject(this, &UACMT_PlayMontageAndWaitForEvent::OnMontageEnded);
				animInstance->Montage_SetEndDelegate(MontageEndedDelegate, MontageToPlay);

				ACharacter* character = Cast<ACharacter>(GetAvatarActor());
				if (character && (character->GetLocalRole() == ROLE_Authority || (character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
				{
					character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
				}

				bPlayerMontage = true;
			}
		}
	}

	if (!bPlayerMontage)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

//==================================================================================================================//
void UACMT_PlayMontageAndWaitForEvent::ExternalCancel()
{
	check(AbilitySystemComponent);
	OnAbilityCancelled();
	Super::ExternalCancel();
}

//==================================================================================================================//
FString UACMT_PlayMontageAndWaitForEvent::GetDebugString() const
{
	UAnimMontage* playingMontage = nullptr;
	if (Ability)
	{
		const FGameplayAbilityActorInfo* actorInfo = Ability->GetCurrentActorInfo();
		UAnimInstance* animInstance = actorInfo->GetAnimInstance();

		if (animInstance != nullptr)
		{
			playingMontage = animInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : animInstance->GetCurrentActiveMontage();
		}
	}

	return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s (CurrentPlaying): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(playingMontage));
}

//==================================================================================================================//
void UACMT_PlayMontageAndWaitForEvent::OnDestroy(bool bInOwnerFinished)
{
	if (Ability)
	{
		Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
		if (bInOwnerFinished && bStopWhenAbilityEnds)
		{
			StopPlayingMontage();
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveGameplayEventTagContainerDelegate(EventTags, EventHandle);
	}

	Super::OnDestroy(bInOwnerFinished);

}

//==================================================================================================================//
UACMT_PlayMontageAndWaitForEvent* UACMT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(UGameplayAbility* OwningAbility, FName TaskInstanceName, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags, float Rate, FName StartSection, bool bStopWhenAbilityEnds, float AnimRootMotionTranslationScale)
{
	UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

	UACMT_PlayMontageAndWaitForEvent* myObj = NewAbilityTask<UACMT_PlayMontageAndWaitForEvent>(OwningAbility, TaskInstanceName);
	myObj->MontageToPlay = MontageToPlay;
	myObj->EventTags = EventTags;
	myObj->Rate = Rate;
	myObj->StartSection = StartSection;
	myObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
	myObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

	return myObj;
}

//==================================================================================================================//
bool UACMT_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
	const FGameplayAbilityActorInfo* actorInfo = Ability->GetCurrentActorInfo();
	if (!actorInfo)
	{
		return false;
	}

	UAnimInstance* animInstance = actorInfo->GetAnimInstance();
	if (animInstance == nullptr)
	{
		return false;
	}

	if (AbilitySystemComponent && Ability)
	{
		if (AbilitySystemComponent->GetAnimatingAbility() == Ability && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
		{
			FAnimMontageInstance* montageInstance = animInstance->GetActiveInstanceForMontage(MontageToPlay);
			if (montageInstance)
			{
				montageInstance->OnMontageBlendingOutStarted.Unbind();
				montageInstance->OnMontageEnded.Unbind();
			}

			AbilitySystemComponent->CurrentMontageStop();
			return true;
		}
	}

	return false;
}

//==================================================================================================================//
void UACMT_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
	if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
	{
		if (Montage == MontageToPlay)
		{
			AbilitySystemComponent->ClearAnimatingAbility(Ability);

			ACharacter* character = Cast<ACharacter>(GetAvatarActor());
			if (character && (character->GetLocalRole() == ROLE_Authority || (character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
			{
				character->SetAnimRootMotionTranslationScale(1.f);
			}

		}
	}

	if (bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnInterrupted.Broadcast(FGameplayTag(), FGameplayEventData());
		}
		else
		{
			if (ShouldBroadcastAbilityTaskDelegates())
			{
				OnBlendOut.Broadcast(FGameplayTag(), FGameplayEventData());
			}
		}
	}


}

//==================================================================================================================//
void UACMT_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
	if (StopPlayingMontage())
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnCancelled.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}
}

//==================================================================================================================//
void UACMT_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (!bInterrupted)
	{
		if (ShouldBroadcastAbilityTaskDelegates())
		{
			OnComplete.Broadcast(FGameplayTag(), FGameplayEventData());
		}
	}

	EndTask();
}

//==================================================================================================================//
void UACMT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* PlayLoad)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		FGameplayEventData tempData = *PlayLoad;
		tempData.EventTag = EventTag;

		OnEventReceived.Broadcast(EventTag, tempData);
	}
}
