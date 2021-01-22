// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ShockingGrasp/ACM_AbsorptionSphere.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArkdeCMCharacter.h"
#include "AbilitySystemComponent.h"


// Sets default values
AACM_AbsorptionSphere::AACM_AbsorptionSphere()
{
	MeeleSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("MeleeDetectorComponent"));
	MeeleSphereComponent->SetupAttachment(RootComponent);		
	MeeleSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeeleSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	MeeleSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	MeeleParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	MeeleParticle->bAutoActivate = false;
	MeeleParticle->SetupAttachment(MeeleSphereComponent);
}

// Called when the game starts or when spawned
void AACM_AbsorptionSphere::BeginPlay()
{
	Super::BeginPlay();		
	MeeleSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AACM_AbsorptionSphere::SphereComponentBeginOverlap);
	MeeleSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AACM_AbsorptionSphere::SphereComponentEndOverlap);
}

void AACM_AbsorptionSphere::SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OtherActor))
	{		
		if (OtherActor == this->GetOwner())
		{
			return;
		}

		AArkdeCMCharacter* character = Cast<AArkdeCMCharacter>(OtherActor);
		if (IsValid(character))
		{
			CharacterAbilitySystemComponent = character->AbilitySystemComponent;
			if (GetLocalRole() != ROLE_Authority && !IsValid(CharacterAbilitySystemComponent))
			{
				return;
			}

			FGameplayEffectContextHandle effectContext = CharacterAbilitySystemComponent->MakeEffectContext();
			effectContext.AddSourceObject(this);

			if (IsValid(AddHealthEffect))
			{
				FGameplayEffectSpecHandle newHandle = CharacterAbilitySystemComponent->MakeOutgoingSpec(AddHealthEffect, 1.f, effectContext);
				if (newHandle.IsValid() && IsValid(ShockingGraspSound))
				{
					FActiveGameplayEffectHandle ActiveGEHandle = CharacterAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*newHandle.Data.Get());
					Multicast_ParticleActivation(true);
				}
			}
		}
	}
}

void AACM_AbsorptionSphere::SphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (IsValid(OtherActor))
	{
		if (OtherActor == this->GetOwner())
		{
			return;
		}

		Multicast_ParticleActivation(false);
	}
}


void AACM_AbsorptionSphere::Multicast_ParticleActivation_Implementation(bool bIsActive)
{
	if (IsValid(MeeleParticle) && IsValid(ShockingGraspSound))
	{
		MeeleParticle->SetActive(bIsActive);
		if (bIsActive)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShockingGraspSound, this->GetActorLocation());
		}
	}
}

//===========================================================================================================================================================// 
bool AACM_AbsorptionSphere::Multicast_ParticleActivation_Validate(bool bIsActive)
{
	return true;
}



