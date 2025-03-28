// Fill out your copyright notice in the Description page of Project Settings.


#include "NBC_CH9/Public/UI/NBC_ChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/NBC_PlayerController.h"

void UNBC_ChatInput::NativeConstruct()
{
	Super::NativeConstruct();
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);		
	}	
}

void UNBC_ChatInput::NativeDestruct()
{
	Super::NativeDestruct();
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UNBC_ChatInput::OnChatInputTextCommitted(const FText& Text, const ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			ANBC_PlayerController* NBC_PlayerController = Cast<ANBC_PlayerController>(OwningPlayerController);
			if (IsValid(NBC_PlayerController) == true)
			{
				NBC_PlayerController->SetChatMessageString(Text.ToString());
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
