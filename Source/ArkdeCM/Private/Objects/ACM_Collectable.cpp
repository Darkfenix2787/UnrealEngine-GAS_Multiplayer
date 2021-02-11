// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects\ACM_Collectable.h"
#include "Components\CapsuleComponent.h"
#include "Particles\ParticleSystemComponent.h"
#include "Character\ArkdeCMCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "Objects\ACM_Spawner.h"

//===========================================================================================================================================================//
// Sets default values
AACM_Collectable::AACM_Collectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CollisionCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collectable Capsule Component"));
	RootComponent = CollisionCapsuleComponent;

	CollectableParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Collectable Particle System Component"));
	CollectableParticleSystem->SetupAttachment(RootComponent);

	bReplicates = true;

}

//===========================================================================================================================================================//
// Called when the game starts or when spawned
void AACM_Collectable::BeginPlay()
{
	Super::BeginPlay();
	CollisionCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AACM_Collectable::CapsuleComponentBeginOverlap);
}

//===========================================================================================================================================================//
void AACM_Collectable::CapsuleComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		AArkdeCMCharacter* CharacterRef = Cast<AArkdeCMCharacter>(OtherActor);

		if (IsValid(CharacterRef))
		{
			CollisionCapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			CharacterAbilitySystemComponent = CharacterRef->AbilitySystemComponent;

			if (!IsValid(CharacterAbilitySystemComponent))
			{
				return;
			}

			FGameplayEffectContextHandle EffectContext = CharacterAbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			if (IsValid(CollectableEffect))
			{
				FGameplayEffectSpecHandle newEffectHandle = CharacterAbilitySystemComponent->MakeOutgoingSpec(CollectableEffect, 1.f, EffectContext);
				if (newEffectHandle.IsValid())
				{
					FActiveGameplayEffectHandle ActiveGEHandle = CharacterAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*newEffectHandle.Data.Get());

					CollectableSpawner = Cast<AACM_Spawner>(GetOwner());
					if (IsValid(CollectableSpawner))
					{
						CollectableSpawner->SetupRespawnTimer();
					}
				}
			}
		}
	}

	Destroy();
}

