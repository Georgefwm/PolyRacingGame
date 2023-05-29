﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "WheeledVehiclePawn.h"
#include "Customisation/VehicleCustomisationComponent.h"
#include "PolyRacingWheeledVehiclePawn.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class POLYRACINGGAME_API APolyRacingWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APolyRacingWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	UVehicleCustomisationComponent* VehicleCustomisationComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input Actions")
	UInputAction* ThrottleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input Actions")
	UInputAction* BrakeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input Actions")
	UInputAction* SteeringAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input Actions")
	UInputAction* HandbrakeAction;
	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplyThrottle(const FInputActionValue& Value);

	void ApplyBrake(const FInputActionValue& Value);
	
	void ApplySteering(const FInputActionValue& Value);

	void OnHandBrakePressed();
	void OnHandBrakeReleased();

	void UpdateInAirControl(float DeltaTime);
};