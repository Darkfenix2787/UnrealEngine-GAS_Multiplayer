// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/WindSwirl/ACM_WindSphere.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArkdeCMCharacter.h"
#include "AbilitySystemComponent.h"
#include "Sound/SoundCue.h"

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
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AACM_WindSphere::SphereComponentBeginOverlap);
}


void AACM_WindSphere::SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (IsValid(OtherActor) && OtherActor != this->GetOwner())
	{
		AArkdeCMCharacter* Character = Cast<AArkdeCMCharacter>(OtherActor);

		if (!IsValid(Character))
		{
			return;
		}

		CharacterAbilitySystemComponent = Character->AbilitySystemComponent;
		if (GetLocalRole() != ROLE_Authority && !IsValid(CharacterAbilitySystemComponent))
		{
			return;
		}

		FGameplayEffectContextHandle EffectContext = CharacterAbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		if (IsValid(DamageEffect))
		{
			FGameplayEffectSpecHandle newDamageHandle = CharacterAbilitySystemComponent->MakeOutgoingSpec(DamageEffect, 1.f, EffectContext);
			if (newDamageHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveDamageGEHandle = CharacterAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*newDamageHandle.Data.Get());
				if (IsValid(WindSwirlSound))
				{
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), WindSwirlSound, this->GetActorLocation());
				}
			}
		}
	}
}