// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACM_Collectable.generated.h"

class UCapsuleComponent;
class UParticleSystemComponent;
class UGameplayEffect;
class UAbilitySystemComponent;
class AACM_Spawner;

UCLASS()
class ARKDECM_API AACM_Collectable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACM_Collectable();	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCapsuleComponent* CollisionCapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UParticleSystemComponent* CollectableParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay Ability System")
		UAbilitySystemComponent* CharacterAbilitySystemComponent;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> CollectableEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		AACM_Spawner* CollectableSpawner;
public:

	UFUNCTION()
		void CapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
