// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_CH9/Public/Player/NBC_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/NBC_GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NBC_CH9/NBC_CH9.h"
#include "Net/UnrealNetwork.h"
#include "Player/NBC_PlayerState.h"
#include "UI/NBC_ChatInput.h"

ANBC_PlayerController::ANBC_PlayerController()
{
	bReplicates = true;
}

void ANBC_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	bShowMouseCursor = true;
	if (IsLocalController() == false)
	{
		return;
	}
	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UNBC_ChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}
	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void ANBC_PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NotificationText);
}

void ANBC_PlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void ANBC_PlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		ANBC_GameMode* NBC_GM = Cast<ANBC_GameMode>(GM);
		if (IsValid(NBC_GM) == true)
		{
			NBC_GM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void ANBC_PlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;
	//PrintChatMessageString(ChatMessageString);

	if (IsLocalController()==true)
	{
		ANBC_PlayerState* NBC_PlayerState = GetPlayerState<ANBC_PlayerState>();
		if (IsValid(NBC_PlayerState) == true)
		{
			FString CombinedMessageString = NBC_PlayerState->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void ANBC_PlayerController::PrintChatMessageString(const FString& InChatMessageString) const
{
	// FString NetModeString = NBC_CH9FunctionLibrary::GetNetModeString(this);
	// FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	//NBC_CH9FunctionLibrary::MyPrintString(this, CombinedMessageString, 10.f);
	NBC_CH9FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}
