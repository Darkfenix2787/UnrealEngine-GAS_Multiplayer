// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects\ACM_Spawner.h"
#include "Objects\ACM_Collectable.h"

//===========================================================================================================================================================//
// Sets default values
AACM_Spawner::AACM_Spawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RespawnTime = 10.f;
}

//===========================================================================================================================================================//
// Called when the game starts or when spawned
void AACM_Spawner::BeginPlay()
{
	Super::BeginPlay();
	
}

//===========================================================================================================================================================//
void AACM_Spawner::SetupRespawnTimer()
{
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &AACM_Spawner::RespawnCollectable, RespawnTime, false);
}

//===========================================================================================================================================================//
void AACM_Spawner::RespawnCollectable()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	FTransform SpawnTransform = GetActorTransform();
	FVector SpawnLocation = SpawnTransform.GetLocation() + FVector(0.f, 0.f, 100.f);
	SpawnTransform.SetLocation(SpawnLocation);

	GetWorld()->SpawnActor<AACM_Collectable>(CollectableSubClass, SpawnTransform, SpawnParams);

}

//===========================================================================================================================================================//
void AACM_Spawner::SetCollectableSubClass(TSubclassOf<AACM_Collectable> SelectedSubClass)
{
	CollectableSubClass = SelectedSubClass;
}


