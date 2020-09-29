// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_Shield.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AACM_Shield::AACM_Shield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

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

