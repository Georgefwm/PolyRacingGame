// Fill out your copyright notice in the Description page of Project Settings.

#include "PolyRacingStaticUtils.h"
#include "CoreMinimal.h"


FString FPolyRacingStaticUtils::GetFloatAsStringWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
	// Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	
	if(FMath::Abs(TheFloat - Rounded) < FMath::Pow(10.f,-1.f * Precision))
	{       
		TheFloat = Rounded;
	}
	
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	
	return FText::AsNumber(TheFloat, &NumberFormat).ToString(); 
}

FText FPolyRacingStaticUtils::GetFloatAsTextWithPrecision(float TheFloat, int32 Precision, bool IncludeLeadingZero)
{
	// Round to integral if have something like 1.9999 within precision
	float Rounded = roundf(TheFloat);
	
	if(FMath::Abs(TheFloat - Rounded) < FMath::Pow(10.f,-1.f * Precision))
	{       
		TheFloat = Rounded;
	}
	
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = (IncludeLeadingZero) ? 1 : 0;
	NumberFormat.MaximumIntegralDigits = 10000;
	NumberFormat.MinimumFractionalDigits = Precision;
	NumberFormat.MaximumFractionalDigits = Precision;
	
	return FText::AsNumber(TheFloat, &NumberFormat); 
}

FText FPolyRacingStaticUtils::GetFormatTimeText(float TimeStampSeconds)
{
	FNumberFormattingOptions BaseFormat;
	BaseFormat.MinimumIntegralDigits = 2;
	BaseFormat.MaximumFractionalDigits = 2;

	FNumberFormattingOptions MillisFormat;
	MillisFormat.MinimumIntegralDigits = 3;
	MillisFormat.MaximumFractionalDigits = 3;
	
	return FText::Format(FText::FromString(TEXT("{0}:{1}:{2}")),
		FText::AsNumber(FMath::FloorToInt(TimeStampSeconds / 60.0f), &BaseFormat),
		FText::AsNumber(FMath::FloorToInt(FMath::Fmod(TimeStampSeconds, 60.0f)), &BaseFormat),
		FText::AsNumber(FMath::FloorToInt(FMath::Fmod(TimeStampSeconds * 1000.0f, 1000.0f)), &MillisFormat));
}
