// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Core/ACM_PlayerController.h"
#include "Kismet/GamePlayStatics.h"

//==================================================================================================================//
AACM_PlayerController::AACM_PlayerController()
{
	CameraSpeed = 0.5;
}

//==================================================================================================================//
void AACM_PlayerController::GameConclussion(bool bWasSuccessful)
{
	BP_GameConclussion(bWasSuccessful);

	TArray<AActor*> ViewPointsActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ViewActorClass, ViewPointsActors);

	SetViewTargetWithBlend(ViewPointsActors[0], CameraSpeed, EViewTargetBlendFunction::VTBlend_Cubic);
}

//==================================================================================================================//
void AACM_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	SetViewTarget(aPawn);
}
