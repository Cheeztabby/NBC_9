// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/NBC_GameMode.h"

#include "EngineUtils.h"
#include "GameState/NBC_GameState.h"
#include "Player/NBC_PlayerController.h"
#include "Player/NBC_PlayerState.h"


ANBC_GameMode::ANBC_GameMode()
{
}

FString ANBC_GameMode::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool ANBC_GameMode::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do
	{
		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;
	}
	while (false);

	return bCanPlay;
}

FString ANBC_GameMode::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}
	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void ANBC_GameMode::PrintChatMessageString(ANBC_PlayerController* InChattingPlayerController,
                                           const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true && InChattingPlayerController == GetCurrentTurnPlayerController())
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);
		IncreaseGuessCount(InChattingPlayerController);
		for (TActorIterator<ANBC_PlayerController> It(GetWorld()); It; ++It)
		{
			ANBC_PlayerController* NBC_PlayerController = *It;
			if (IsValid(NBC_PlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				NBC_PlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<ANBC_PlayerController> It(GetWorld()); It; ++It)
		{
			ANBC_PlayerController* NBC_PlayerController = *It;
			if (IsValid(NBC_PlayerController) == true)
			{
				NBC_PlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void ANBC_GameMode::IncreaseGuessCount(ANBC_PlayerController* InChattingPlayerController)
{
	ANBC_PlayerState* NBC_PlayerState = InChattingPlayerController->GetPlayerState<ANBC_PlayerState>();
	if (IsValid(NBC_PlayerState) == true)
	{
		NBC_PlayerState->CurrentGuessCount++;
	}
}

void ANBC_GameMode::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& NBC_PlayerController : AllPlayerControllers)
	{
		ANBC_PlayerState* NBC_PlayerState = NBC_PlayerController->GetPlayerState<ANBC_PlayerState>();
		if (IsValid(NBC_PlayerState) == true)
		{
			NBC_PlayerState->CurrentGuessCount = 0;
		}
	}
	CurrentTurnIndex = 0;
}

void ANBC_GameMode::JudgeGame(ANBC_PlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		ANBC_PlayerState* NBC_PlayerState = InChattingPlayerController->GetPlayerState<ANBC_PlayerState>();
		for (const auto& NBC_PlayerController : AllPlayerControllers)
		{
			if (IsValid(NBC_PlayerState) == true)
			{
				FString CombinedMessageString = NBC_PlayerState->PlayerNameString + TEXT(" has won the game.");
				NBC_PlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& NBC_PlayerController : AllPlayerControllers)
		{
			ANBC_PlayerState* NBC_PlayerState = NBC_PlayerController->GetPlayerState<ANBC_PlayerState>();
			if (IsValid(NBC_PlayerState) == true)
			{
				if (NBC_PlayerState->CurrentGuessCount < NBC_PlayerState->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& NBC_PlayerController : AllPlayerControllers)
			{
				NBC_PlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}

ANBC_PlayerController* ANBC_GameMode::GetCurrentTurnPlayerController() const
{
	if (AllPlayerControllers.IsEmpty() == true)
	{
		return nullptr;
	}
	
	if (AllPlayerControllers.IsValidIndex(CurrentTurnIndex))
	{
		return AllPlayerControllers[CurrentTurnIndex];
	}
	
	return nullptr;
}

void ANBC_GameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	ANBC_PlayerController* NBC_PlayerController = Cast<ANBC_PlayerController>(NewPlayer);
	if (IsValid(NBC_PlayerController) == true)
	{
		NBC_PlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		AllPlayerControllers.Add(NBC_PlayerController);

		ANBC_PlayerState* NBC_PlayerState = NBC_PlayerController->GetPlayerState<ANBC_PlayerState>();
		if (IsValid(NBC_PlayerState) == true)
		{
			NBC_PlayerState->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		ANBC_GameState* NBC_GameStateBase =  GetGameState<ANBC_GameState>();
		if (IsValid(NBC_GameStateBase) == true)
		{
			NBC_GameStateBase->MulticastRPCBroadcastLoginMessage(NBC_PlayerState->PlayerNameString);
		}

		if (CurrentTurnIndex == AllPlayerControllers.Num() - 1)
		{
			NBC_PlayerController->NotificationText = FText::FromString(TEXT("It's your turn!"));
		}
		else
		{
			NBC_PlayerController->NotificationText = FText::FromString(TEXT("Waiting for other player..."));			
		}
	}
}

void ANBC_GameMode::BeginPlay()
{
	Super::BeginPlay();
	SecretNumberString = GenerateSecretNumber();
	GetWorldTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 15.f, true);
}

void ANBC_GameMode::OnMainTimerElapsed()
{
	if (AllPlayerControllers.Num() == 0)
	{
		return;
	}

	CurrentTurnIndex = (CurrentTurnIndex + 1) % AllPlayerControllers.Num();

	for (int32 i = 0; i < AllPlayerControllers.Num(); ++i)
	{
		if (IsValid(AllPlayerControllers[i]) == true)
		{
			if (i == CurrentTurnIndex)
			{
				AllPlayerControllers[i]->NotificationText = FText::FromString(TEXT("It's your turn!"));
			}
			else
			{
				AllPlayerControllers[i]->NotificationText = FText::FromString(TEXT("Waiting for other player..."));
			}
		}
	}
}
