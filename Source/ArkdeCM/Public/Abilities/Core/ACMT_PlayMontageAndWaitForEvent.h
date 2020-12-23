// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ACMT_PlayMontageAndWaitForEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FACMPlayMontageAndWaitForEventDelegate, FGameplayTag, EventTag, FGameplayEventData, EventData);

/**
 *
 */
UCLASS()
class ARKDECM_API UACMT_PlayMontageAndWaitForEvent : public UAbilityTask
{
	GENERATED_BODY()

public:

	UACMT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer);

	virtual void Activate();
	virtual void ExternalCancel();
	virtual FString GetDebugString() const;
	virtual void OnDestroy(bool bInOwnerFinished);

	UFUNCTION(BlueprintCallable, Category = "Ability|Task", meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "True"))
		static UACMT_PlayMontageAndWaitForEvent* PlayMontageAndWaitForEvent
		(
			UGameplayAbility* OwningAbility,
			FName TaskInstanceName,
			UAnimMontage* MontageToPlay,
			FGameplayTagContainer EventTags,
			float Rate = 1.f,
			FName StartSection = NAME_None,
			bool bStopWhenAbilityEnds = true,
			float AnimRootMotionTranslationScale = 1.f
		);

public:

	UPROPERTY(BlueprintAssignable)
		FACMPlayMontageAndWaitForEventDelegate OnComplete;

	UPROPERTY(BlueprintAssignable)
		FACMPlayMontageAndWaitForEventDelegate OnBlendOut;

	UPROPERTY(BlueprintAssignable)
		FACMPlayMontageAndWaitForEventDelegate OnInterrupted;

	UPROPERTY(BlueprintAssignable)
		FACMPlayMontageAndWaitForEventDelegate OnCancelled;

	UPROPERTY(BlueprintAssignable)
		FACMPlayMontageAndWaitForEventDelegate OnEventReceived;	

private:

	UPROPERTY()
		float Rate;

	UPROPERTY()
		float AnimRootMotionTranslationScale;

	UPROPERTY()
		bool bStopWhenAbilityEnds;

	UPROPERTY()
		UAnimMontage* MontageToPlay;

	UPROPERTY()
		FGameplayTagContainer EventTags;

	UPROPERTY()
		FName StartSection;

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageEnded MontageEndedDelegate;
	FDelegateHandle CancelledHandle;
	FDelegateHandle EventHandle;

private:

	bool StopPlayingMontage();
	void OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted);
	void OnAbilityCancelled();
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* PlayLoad);

};
