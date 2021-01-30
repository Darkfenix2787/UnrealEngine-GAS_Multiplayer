// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Core/ACM_PlayerController.h"

//==================================================================================================================//
AACM_PlayerController::AACM_PlayerController()
{
	CameraSpeed = 0.5;
}

//==================================================================================================================//
void AACM_PlayerController::GameConclussion(bool bWasSuccessfull)
{
	BP_GameConclussion(bWasSuccessfull);

	SetViewTargetWithBlend(nullptr, CameraSpeed, EViewTargetBlendFunction::VTBlend_Cubic);
}