// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NBC_PlayerState.h"

#include "Net/UnrealNetwork.h"

ANBC_PlayerState::ANBC_PlayerState()
{
	bReplicates = true;
}

void ANBC_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);
}


FString ANBC_PlayerState::GetPlayerInfoString() const
{
	FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(CurrentGuessCount) + TEXT("/") + FString::FromInt(MaxGuessCount) + TEXT(")");
	return PlayerInfoString;
}
