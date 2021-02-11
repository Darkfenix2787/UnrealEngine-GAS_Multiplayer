// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACM_AbsorptionSphere.generated.h"


class UGameplayEffect;
class USphereComponent;
class UParticleSystemComponent;
class USoundCue;
class UAbilitySystemComponent;

UCLASS()
class ARKDECM_API AACM_AbsorptionSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACM_AbsorptionSphere();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* MeeleSphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
		UParticleSystemComponent* MeeleParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
		UAbilitySystemComponent* CharacterAbilitySystemComponent;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Audio")
		USoundCue* ShockingGraspSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
		TSubclassOf<UGameplayEffect> AddHealthEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay Ability System")
		TSubclassOf<UGameplayEffect> DamageEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void SphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	

	bool ApplyEffect(AActor* OtherActor, TSubclassOf<UGameplayEffect> EffectApplied);	

	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void Multicast_ParticleActivation(bool bIsActive);

};
