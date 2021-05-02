// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ACM_GameInstance.h"
#include "D:/Programs/Epic Games/UE_4.25/Engine/Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "D:/Programs/Epic Games/UE_4.25/Engine/Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h"
#include "D:/Programs/Epic Games/UE_4.25/Engine/Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"

void UACM_GameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::Init Subsystem Found %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface)
		{
			UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::Init SessionInterface Found"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UACM_GameInstance::OnSessionCreated);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UACM_GameInstance::OnSessionDestroyed);
		}
	}	
}

void UACM_GameInstance::Host()
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::Host Create session start"));
	auto CreateSession = SessionInterface->GetNamedSession(SessionDefaultName);

	if (CreateSession)
	{
		SessionInterface->DestroySession(SessionDefaultName);
	}
	else
	{
		FOnlineSessionSettings SessionSettings;
		SessionInterface->CreateSession(0, SessionDefaultName, SessionSettings);
	}		
}

void UACM_GameInstance::Join()
{
}


void UACM_GameInstance::OnSessionCreated(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionCreated Create session end"));
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionCreated Create session failed"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionCreated Create session success"));
	UWorld* GameWorld = GetWorld();
	GameWorld->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap");
}

void UACM_GameInstance::OnSessionDestroyed(FName SessionName, bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionDestroyed Destroy session end"));
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionDestroyed Destroy session failed"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionDestroyed Destroy session success"));
	FOnlineSessionSettings SessionSettings;
	SessionInterface->CreateSession(0, SessionName, SessionSettings);
}
