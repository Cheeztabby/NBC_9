// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/NBC_GameState.h"

#include "Kismet/GameplayStatics.h"
#include "Player/NBC_PlayerController.h"

void ANBC_GameState::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			ANBC_PlayerController* NBC_PlayerController = Cast<ANBC_PlayerController>(PC);
			if (IsValid(NBC_PlayerController) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				NBC_PlayerController->PrintChatMessageString(NotificationString);
			}
		}
	}
}
