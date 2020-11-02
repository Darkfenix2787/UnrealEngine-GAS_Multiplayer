// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_Shield.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AACM_Shield::AACM_Shield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComponent;

	ParticleSystemComponent_P01 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component P01"));
	ParticleSystemComponent_P01->SetupAttachment(RootComponent);

	ParticleSystemComponent_P02 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component P02"));
	ParticleSystemComponent_P02->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AACM_Shield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACM_Shield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

