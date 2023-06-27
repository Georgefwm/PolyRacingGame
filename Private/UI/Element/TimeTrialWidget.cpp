// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Element/TimeTrialWidget.h"
#include "PolyRacingStaticUtils.h"
#include "Components/TextBlock.h"
#include "Framework/PolyRacingPlayerState.h"


void UTimeTrialWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APolyRacingPlayerState>();
}

void UTimeTrialWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!PlayerState)
		return;

	
	if (PlayerState->EventEndTime > 0.01)  // TODO: Make less 'hacky'
	{
		TotalTimeValueTextBlock->SetText(FPolyRacingStaticUtils::GetFormatTimeText(PlayerState->EventEndTime - PlayerState->EventStartTime));
		CurrentLapTimeValueTextBlock->SetText(FPolyRacingStaticUtils::GetFormatTimeText(PlayerState->EventEndTime - PlayerState->LastLapStartTime));
		return;
	}
	
	if (PlayerState->EventStartTime < 0.01)  // TODO: Make less 'hacky'
	{
		TotalTimeValueTextBlock->SetText(FText::FromString(TEXT("00:00:00")));
		CurrentLapTimeValueTextBlock->SetText(FText::FromString(TEXT("00:00:00")));
		return;
	}

	double const CurrentTime = GetWorld()->GetTimeSeconds();
	
	TotalTimeValueTextBlock->SetText(FPolyRacingStaticUtils::GetFormatTimeText(CurrentTime - PlayerState->EventStartTime));
	CurrentLapTimeValueTextBlock->SetText(FPolyRacingStaticUtils::GetFormatTimeText(CurrentTime - PlayerState->LastLapStartTime));
}
