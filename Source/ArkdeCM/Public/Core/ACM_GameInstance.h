// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "D:/Programs/Epic Games/UE_4.25/Engine/Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
#include "D:/Programs/Epic Games/UE_4.25/Engine/Plugins/Online/OnlineSubsystem/Source/Public/OnlineSessionSettings.h"
#include "D:/Programs/Epic Games/UE_4.25/Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"
#include "D:/Programs/Epic Games/UE_4.25/Engine/Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "ACM_GameInstance.generated.h"


/**
 * 
 */
UCLASS()
class ARKDECM_API UACM_GameInstance : public UGameInstance
{
	GENERATED_BODY()


public:

	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

	FName SessionDefaultName = TEXT("SessionName");

public:

	virtual void Init() override;

	void OnSessionCreated(FName SessionName, bool Success);

	void OnSessionDestroyed(FName SessionName, bool Success);

	void OnSessionFound(bool Success);

	void OnSessionJoined(FName SessionName, EOnJoinSessionCompleteResult::Type JoinResult);

	void CreateSessionSettings();

	UFUNCTION(BlueprintCallable)
		void Host();	

	UFUNCTION(BlueprintCallable)
		void Join();


	
};
