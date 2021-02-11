// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/ShockingGrasp/ACM_AbsorptionSphere.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Character/ArkdeCMCharacter.h"
#include "AbilitySystemComponent.h"

//===========================================================================================================================================================// 
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

//===========================================================================================================================================================// 
// Called when the game starts or when spawned
void AACM_AbsorptionSphere::BeginPlay()
{
	Super::BeginPlay();		
	MeeleSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AACM_AbsorptionSphere::SphereComponentBeginOverlap);
	MeeleSphereComponent->OnComponentEndOverlap.AddDynamic(this, &AACM_AbsorptionSphere::SphereComponentEndOverlap);
}

//===========================================================================================================================================================// 
void AACM_AbsorptionSphere::SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* OwnerActor = this->GetOwner();

	if (IsValid(OtherActor) && IsValid(OwnerActor) && OtherActor != OwnerActor)
	{
		if (ApplyEffect(OtherActor, DamageEffect) && ApplyEffect(OwnerActor, AddHealthEffect))
		{
			Multicast_ParticleActivation(true);
		}
	}
}

//===========================================================================================================================================================// 
bool AACM_AbsorptionSphere::ApplyEffect(AActor* OtherActor, TSubclassOf<UGameplayEffect> EffectApplied)
{
	AArkdeCMCharacter* Character = Cast<AArkdeCMCharacter>(OtherActor);

	if (!IsValid(Character) && IsValid(EffectApplied))
	{
		return false;
	}

	CharacterAbilitySystemComponent = Character->AbilitySystemComponent;
	if (GetLocalRole() != ROLE_Authority && !IsValid(CharacterAbilitySystemComponent))
	{
		return false;
	}

	FGameplayEffectContextHandle EffectContext = CharacterAbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	if (IsValid(EffectApplied))
	{
		FGameplayEffectSpecHandle newHandle = CharacterAbilitySystemComponent->MakeOutgoingSpec(EffectApplied, 1.f, EffectContext);
		if (newHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveDamageGEHandle = CharacterAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*newHandle.Data.Get());
		}
	}

	return true;
}

//===========================================================================================================================================================// 
void AACM_AbsorptionSphere::SphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Multicast_ParticleActivation(false);
}

//===========================================================================================================================================================// 
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



