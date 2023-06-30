// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Element/ResultTimeWidget.h"

#include "PolyRacingStaticUtils.h"
#include "Components/TextBlock.h"
#include "Framework/PolyRacingPlayerState.h"


void UResultTimeWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();
	
	PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APolyRacingPlayerState>();

	if (PlayerState)
		FinalTimeTextBlock->SetText(FPolyRacingStaticUtils::GetFormatTimeText(PlayerState->EventEndTime - PlayerState->EventStartTime));
}



