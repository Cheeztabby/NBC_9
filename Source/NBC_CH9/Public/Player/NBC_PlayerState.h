// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "NBC_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class NBC_CH9_API ANBC_PlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ANBC_PlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPlayerInfoString() const;
	
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount = 0;

	UPROPERTY(Replicated)
	int32 MaxGuessCount = 3;
};

