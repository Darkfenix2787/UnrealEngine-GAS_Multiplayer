// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/ArkdeCMCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "Abilities/Core/ACM_AttributeSet.h"
#include "Abilities/Core/ACM_GameplayAbility.h"
#include "ArkdeCM/ArkdeCM.h"
#include "Abilities/Core/ACM_PlayerState.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

//////////////////////////////////////////////////////////////////////////
// AArkdeCMCharacter

AArkdeCMCharacter::AArkdeCMCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	MeleeSocketName = "AbilityLeftSocket";

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//GAS
	IsInputBound = false;
	IsAbilitiesGiven = false;
	IsEffectsGiven = false;

}

//===========================================================================================================================================================//
void AArkdeCMCharacter::BeginPlay()
{
	Super::BeginPlay();		
}

//===========================================================================================================================================================//
void AArkdeCMCharacter::SetUpGasInputs()
{
	if (!IsInputBound && IsValid(AbilitySystemComponent) && IsValid(InputComponent))
	{
		// Setup ASC Input bindings
		AbilitySystemComponent->BindAbilityActivationToInputComponent
		(
			InputComponent,
			FGameplayAbilityInputBinds(
				"Confirm",
				"Cancel",
				"EACM_AbilityInputID",
				static_cast<int32>(EACM_AbilityInputID::Confirm),
				static_cast<int32>(EACM_AbilityInputID::Cancel)
			)
		);

		IsInputBound = true;
	}
}

//===========================================================================================================================================================//
void AArkdeCMCharacter::SetUpAbilities()
{
	if (GetLocalRole() != ROLE_Authority && !IsValid(AbilitySystemComponent) && IsAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UACM_GameplayAbility>& currentAbility : StartingAbilities)
	{
		if (IsValid(currentAbility))
		{
			UACM_GameplayAbility* defaultObj = currentAbility->GetDefaultObject<UACM_GameplayAbility>();
			FGameplayAbilitySpec abilitySpec(defaultObj, 1, static_cast<int32>(defaultObj->AbilityInputID), this);
			AbilitySystemComponent->GiveAbility(abilitySpec);
		}
	}

	IsAbilitiesGiven = true;
}

//===========================================================================================================================================================//
void AArkdeCMCharacter::SetUpEffects()
{
	if (GetLocalRole() != ROLE_Authority && !IsValid(AbilitySystemComponent) && IsEffectsGiven)
	{
		return;
	}

	FGameplayEffectContextHandle effectContext = AbilitySystemComponent->MakeEffectContext();
	effectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect>& currentEffect : StartingEffect)
	{
		if (IsValid(currentEffect))
		{
			FGameplayEffectSpecHandle newHandle = AbilitySystemComponent->MakeOutgoingSpec(currentEffect, 1.f, effectContext);
			if (newHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*newHandle.Data.Get(), GetAbilitySystemComponent());
			}		
		}
	}
	IsEffectsGiven = true;
}

//===========================================================================================================================================================
void AArkdeCMCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AACM_PlayerState* playerState = GetPlayerState<AACM_PlayerState>();
	if (IsValid(playerState))
	{
		AbilitySystemComponent = playerState->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(playerState, this);
		AttributeSet = playerState->GetAttributeSet();

		SetUpAbilities();
		SetUpEffects();
	}
}

//===========================================================================================================================================================//
void AArkdeCMCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AACM_PlayerState* playerState = GetPlayerState<AACM_PlayerState>();
	if (IsValid(playerState))
	{
		AbilitySystemComponent = playerState->GetAbilitySystemComponent();
		AbilitySystemComponent->InitAbilityActorInfo(playerState, this);
		AttributeSet = playerState->GetAttributeSet();

		SetUpGasInputs();
	}
}

//===========================================================================================================================================================
//////////////////////////////////////////////////////////////////////////
// Input

void AArkdeCMCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AArkdeCMCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArkdeCMCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AArkdeCMCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AArkdeCMCharacter::LookUpAtRate);

	SetUpGasInputs();
}

//===========================================================================================================================================================
UAbilitySystemComponent* AArkdeCMCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//===========================================================================================================================================================// 
void AArkdeCMCharacter::Die()
{
}

//===========================================================================================================================================================
void AArkdeCMCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

//===========================================================================================================================================================
void AArkdeCMCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

//===========================================================================================================================================================
void AArkdeCMCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

//===========================================================================================================================================================
void AArkdeCMCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//===========================================================================================================================================================
void AArkdeCMCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//===========================================================================================================================================================
void AArkdeCMCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

//===========================================================================================================================================================
void AArkdeCMCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
