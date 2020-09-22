// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArkdeCMGameMode.h"
#include "ArkdeCMCharacter.h"
#include "UObject/ConstructorHelpers.h"

AArkdeCMGameMode::AArkdeCMGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
