// Fill out your copyright notice in the Description page of Project Settings.


#include "ACM_WindSphere.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AACM_WindSphere::AACM_WindSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	RootComponent = SphereComponent;

	ParticleSystemComponent_P01 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component P01"));
	ParticleSystemComponent_P01->SetupAttachment(RootComponent);

	ParticleSystemComponent_P02 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component P02"));
	ParticleSystemComponent_P02->SetupAttachment(RootComponent);

	ParticleSystemComponent_P03 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component P03"));
	ParticleSystemComponent_P03->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AACM_WindSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AACM_WindSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

