// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACM_Spawner.generated.h"

class AACM_Collectable;

UCLASS()
class ARKDECM_API AACM_Spawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AACM_Spawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	FTimerHandle RespawnTimerHandle;

public:

	TSubclassOf<AACM_Collectable> CollectableSubClass;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float RespawnTime;

public:

	UFUNCTION(BlueprintCallable)
		void SetupRespawnTimer();

	UFUNCTION()
		void RespawnCollectable();

	void SetCollectableSubClass(TSubclassOf<AACM_Collectable> SelectedSubClass);

};
