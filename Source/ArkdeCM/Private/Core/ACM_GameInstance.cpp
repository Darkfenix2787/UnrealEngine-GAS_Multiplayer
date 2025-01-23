// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/ACM_GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

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
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UACM_GameInstance::OnSessionFound);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UACM_GameInstance::OnSessionJoined);
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
		SessionSettings.bIsLANMatch = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.NumPublicConnections = 2;
		SessionInterface->CreateSession(0, SessionDefaultName, SessionSettings);
	}		
}

void UACM_GameInstance::Join()
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::Join Find Sessions Start"));
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->MaxSearchResults = 100;
	SessionSearch->bIsLanQuery = true;
	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
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
	GameWorld->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
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
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionInterface->CreateSession(0, SessionName, SessionSettings);
}

void UACM_GameInstance::OnSessionFound(bool Success)
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound Find Sessions end"));
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound Find Sessions failed"));
		return;
	}

	if (SessionSearch->SearchResults.Num() > 0)
	{
		for (FOnlineSessionSearchResult CurrentResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound Session Found %s"), *CurrentResult.GetSessionIdStr());
		}
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound Join Session Start"));
		SessionInterface->JoinSession(0, SessionDefaultName, SessionSearch->SearchResults[0]);

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionFound No Sessions found"));
	}
}

void UACM_GameInstance::OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult)
{
	UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionJoined Join Session End"));
	FString Address;

	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionJoined Join Session Failed"));
		return;
	}

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Warning, TEXT("UACM_GameInstance::OnSessionJoined No Player Controller Found"));
		return;
	}
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}
