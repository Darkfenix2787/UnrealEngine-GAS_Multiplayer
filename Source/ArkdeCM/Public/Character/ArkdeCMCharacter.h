// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ArkdeCMCharacter.generated.h"

class UAbilitySystemComponent;
class UACM_AttributeSet;
class UACM_GameplayAbility;
class UGameplayEffect;
class USphereComponent;
class UParticleSystemComponent;
class USoundCue;
class UAnimMontage;

UCLASS(config=Game)
class AArkdeCMCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	AArkdeCMCharacter();

	virtual void BeginPlay() override;	

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


#pragma region Gameplay Ability System

	/* ---- Gameplay Ability System Start ---- */

public:

	bool bIsInputBound;

	bool bIsAbilitiesGiven;

	bool bIsEffectsGiven;

	bool bIsDying;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Melee")
		FName MeleeSocketName;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
		UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
		UACM_AttributeSet* AttributeSet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
		TArray<TSubclassOf<UACM_GameplayAbility>> StartingAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
		TArray<TSubclassOf<UGameplayEffect>> StartingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
		TSubclassOf<UGameplayEffect> DeathEffectClass;

	UPROPERTY(EditAnywhere, Category = "Animation")
		UAnimMontage* DeathMontage;

	

public:	

	void SetUpGasInputs();

	void SetUpAbilities();

	void SetUpEffects();	

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;

	UFUNCTION(BlueprintCallable, Category = "Gameplay Ability System")
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(Server, Reliable)
		virtual void Server_Die(AArkdeCMCharacter* KillerCharacter);

	UFUNCTION(NetMulticast, Reliable)
		void MulticastOnDeath();
	

#pragma endregion



};

