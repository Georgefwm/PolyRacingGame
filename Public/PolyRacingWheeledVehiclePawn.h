// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WheeledVehiclePawn.h"
#include "PolyRacingWheeledVehiclePawn.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UChaosWheeledVehicleMovementComponent;
class UVehicleCustomisationComponent;
class USpringArmComponent;
class UCameraComponent;
class UUserWidget;
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;


UCLASS()
class POLYRACINGGAME_API APolyRacingWheeledVehiclePawn : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:

	APolyRacingWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer);

	UPROPERTY()
	UVehicleCustomisationComponent* VehicleCustomisationComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> VehicleHUD;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FX")
	UNiagaraSystem* WheelNiagaraSystem;

	UPROPERTY()
	TArray<UNiagaraComponent*> WheelNiagaraComponents;

	UPROPERTY()
	TArray<USceneComponent*> WheelAttachments;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* Fl_WheelAttachment;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* Fr_WheelAttachment;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* Rl_WheelAttachment;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* Rr_WheelAttachment;
	

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input Actions")
	UInputAction* ResetAction;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input Actions")
	// UInputAction* ToggleInGameMenuAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FX")
	UMaterialInterface* SkidMarkMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FX")
	UAudioComponent* TyreAudioComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FX")
	UAudioComponent* EngineAudioComponent;

	UPROPERTY()
	float BaseTyreSoundMultiplier;

	UPROPERTY()
	float RollTimer = 0.0f;

	virtual void PossessedBy(AController* NewController) override;
	
	void SetupInputMappingContext();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ApplyThrottle(const FInputActionValue& Value);

	void ApplyBrake(const FInputActionValue& Value);
	
	void ApplySteering(const FInputActionValue& Value);

	void HandleVehicleReset();

	void OnHandBrakePressed();
	void OnHandBrakeReleased();

	void UpdateInAirControl(float DeltaTime);

	// Use this to avoid casting to UChaosWheeledVehicleMovementComponent* every time
	UChaosWheeledVehicleMovementComponent* GetChaosVehicleMovementComponent();
};
