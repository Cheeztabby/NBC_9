// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "NBC_PlayerController.generated.h"

class UNBC_ChatInput;
/**
 * 
 */
UCLASS()
class NBC_CH9_API ANBC_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	
	void SetChatMessageString(const FString& InChatMessageString);
	void PrintChatMessageString(const FString& InChatMessageString) const;
protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UNBC_ChatInput> ChatInputWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UNBC_ChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;
};
