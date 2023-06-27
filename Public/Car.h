// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Car.generated.h"


UCLASS()
class POLYRACINGGAME_API AACar : public APawn
{
	GENERATED_BODY()

public:
	
	AACar();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
