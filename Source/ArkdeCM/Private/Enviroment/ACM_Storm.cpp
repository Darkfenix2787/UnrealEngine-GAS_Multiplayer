// Fill out your copyright notice in the Description page of Project Settings.


#include "Enviroment/ACM_Storm.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/ArkdeCMCharacter.h"
#include "AbilitySystemComponent.h"

// Sets default values
AACM_Storm::AACM_Storm()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	StormStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StormStaticMeshComponent"));
	StormStaticMeshComponent->SetupAttachment(RootComponent);
	StormStaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StormStaticMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	StormStaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);


	//StormSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereDetectorComponent"));
	//StormSphereComponent->SetupAttachment(StormStaticMeshComponent);
	//StormSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	//StormSphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//StormSphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

// Called when the game starts or when spawned
void AACM_Storm::BeginPlay()
{
	Super::BeginPlay();

	StormStaticMeshComponent->OnComponentBeginOverlap.AddDynamic(this, &AACM_Storm::SphereComponentBeginOverlap);
	StormStaticMeshComponent->OnComponentEndOverlap.AddDynamic(this, &AACM_Storm::SphereComponentEndOverlap);
}

//===========================================================================================================================================================// 
void AACM_Storm::SphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	AArkdeCMCharacter* Character = Cast<AArkdeCMCharacter>(OtherActor);

	if (IsValid(Character))
	{
		CharacterAbilitySystemComponent = Character->AbilitySystemComponent;

		if (IsValid(CharacterAbilitySystemComponent) && IsValid(DamageEffect))
		{
			CharacterAbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(DamageEffect, CharacterAbilitySystemComponent);			
		}
	}
}

//===========================================================================================================================================================// 
void AACM_Storm::SphereComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	AArkdeCMCharacter* Character = Cast<AArkdeCMCharacter>(OtherActor);

	if (IsValid(Character))
	{
		CharacterAbilitySystemComponent = Character->AbilitySystemComponent;

		if (IsValid(CharacterAbilitySystemComponent) && IsValid(DamageEffect))
		{
			FGameplayEffectContextHandle EffectContext = CharacterAbilitySystemComponent->MakeEffectContext();
			EffectContext.AddSourceObject(this);

			FGameplayEffectSpecHandle newHandle = CharacterAbilitySystemComponent->MakeOutgoingSpec(DamageEffect, 1.f, EffectContext);
			if (newHandle.IsValid())
			{
				FActiveGameplayEffectHandle ActiveDamageGEHandle = CharacterAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*newHandle.Data.Get());
			}
		}
	}
}



