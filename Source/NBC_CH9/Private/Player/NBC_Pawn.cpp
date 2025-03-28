// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/NBC_Pawn.h"
#include "NBC_CH9/NBC_CH9.h"

ANBC_Pawn::ANBC_Pawn()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANBC_Pawn::BeginPlay()
{
	Super::BeginPlay();
	FString NetModeString = NBC_CH9FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("NBC_Pawn::BeginPlay() %s [%s]"), *NBC_CH9FunctionLibrary::GetNetModeString(this), *NetModeString);
	NBC_CH9FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void ANBC_Pawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetModeString = NBC_CH9FunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("NBC_Pawn::PossessedBy() %s [%s]"), *NBC_CH9FunctionLibrary::GetNetModeString(this), *NetModeString);
	NBC_CH9FunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}