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

	double const CurrentTime = GetWorld()->GetTimeSeconds();
	
	// TODO: Format text in style: MIN:SEC:MILLIS
	TotalTimeValueTextBlock->SetText(FPolyRacingStaticUtils::GetFormatTimeText(CurrentTime - PlayerState->EventStartTime));
	CurrentLapTimeValueTextBlock->SetText(FPolyRacingStaticUtils::GetFormatTimeText(CurrentTime - PlayerState->LastLapStartTime));
}
