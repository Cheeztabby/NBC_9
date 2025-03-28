// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "NBC_Pawn.generated.h"

UCLASS()
class NBC_CH9_API ANBC_Pawn : public APawn
{
	GENERATED_BODY()

public:
	ANBC_Pawn();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

};
