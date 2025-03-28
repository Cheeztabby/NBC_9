// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "NBC_GameState.generated.h"

/**
 * 
 */
UCLASS()
class NBC_CH9_API ANBC_GameState : public AGameStateBase
{
	GENERATED_BODY()
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
	
};
