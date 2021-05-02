// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "D:/Programs/Epic Games/UE_4.25/Engine/Plugins/Online/OnlineSubsystem/Source/Public/OnlineSubsystem.h"
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
	FName SessionDefaultName = TEXT("SessionName");

public:

	virtual void Init() override;

	void OnSessionCreated(FName SessionName, bool Success);

	void OnSessionDestroyed(FName SessionName, bool Success);

	UFUNCTION(BlueprintCallable)
		void Host();

	UFUNCTION(BlueprintCallable)
		void Join();


	
};
