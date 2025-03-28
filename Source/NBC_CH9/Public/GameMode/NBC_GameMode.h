// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NBC_GameMode.generated.h"

class ANBC_PlayerController;
/**
 * 
 */
UCLASS()
class NBC_CH9_API ANBC_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ANBC_GameMode();

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	void PrintChatMessageString(ANBC_PlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(ANBC_PlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(ANBC_PlayerController* InChattingPlayerController, int InStrikeCount);

	ANBC_PlayerController* GetCurrentTurnPlayerController() const;
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;
	
	virtual void BeginPlay() override;
	
	FString SecretNumberString;
	
	TArray<TObjectPtr<ANBC_PlayerController>> AllPlayerControllers;

	FTimerHandle MainTimerHandle;

	int32 CurrentTurnIndex = 0;
private:
	UFUNCTION()
	void OnMainTimerElapsed();
};
