// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACM_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UParticleSystemComponent;
class AArkdeCMCharacter;

UCLASS()
class AACM_Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AACM_Projectile();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UParticleSystemComponent* ParticleSystemComponent;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Range;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetProjectileInitialSpeed(float initialSpeed);

	UFUNCTION(NetMulticast, WithValidation, Reliable)
		void Multicast_IgnoreActor(AArkdeCMCharacter* ActorToIgnore);



};
