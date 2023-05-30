// Fill out your copyright notice in the Description page of Project Settings.


#include "PolyRacingWheeledVehiclePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ChaosVehicles/Public/ChaosVehicleMovementComponent.h"
#include "Tests/AutomationTestSettings.h"


// Sets default values
APolyRacingWheeledVehiclePawn::APolyRacingWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetSimulatePhysics(true);
	
	SetupInputMappingContext();

	VehicleCustomisationComponent = CreateDefaultSubobject<UVehicleCustomisationComponent>("Customisation Component");
	VehicleCustomisationComponent->EditingMesh = GetMesh();
}

// Called when the game starts or when spawned
void APolyRacingWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	SetupInputMappingContext();
}

// Called every frame
void APolyRacingWheeledVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APolyRacingWheeledVehiclePawn::SetupInputMappingContext()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		InputSubsystem->ClearAllMappings();
		InputSubsystem->AddMappingContext(InputMappingContext, 0);
	}
}

// Called to bind functionality to input
void APolyRacingWheeledVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// CastChecked assert checks the cast succeeded
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Triggered, this, &APolyRacingWheeledVehiclePawn::ApplyThrottle);
	EnhancedInputComponent->BindAction(ThrottleAction, ETriggerEvent::Completed, this, &APolyRacingWheeledVehiclePawn::ApplyThrottle);
	
	EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Triggered, this, &APolyRacingWheeledVehiclePawn::ApplyBrake);
	EnhancedInputComponent->BindAction(BrakeAction, ETriggerEvent::Completed, this, &APolyRacingWheeledVehiclePawn::ApplyBrake);
	
	EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Triggered, this, &APolyRacingWheeledVehiclePawn::ApplySteering);
	EnhancedInputComponent->BindAction(SteeringAction, ETriggerEvent::Completed, this, &APolyRacingWheeledVehiclePawn::ApplySteering);
	
	EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Triggered, this, &APolyRacingWheeledVehiclePawn::OnHandBrakePressed);
	EnhancedInputComponent->BindAction(HandbrakeAction, ETriggerEvent::Completed, this, &APolyRacingWheeledVehiclePawn::OnHandBrakeReleased);
}	

void APolyRacingWheeledVehiclePawn::ApplyThrottle(const FInputActionValue& Value)
{
	GetVehicleMovementComponent()->SetThrottleInput(Value.Get<float>());
}

void APolyRacingWheeledVehiclePawn::ApplyBrake(const FInputActionValue& Value)
{
	GetVehicleMovementComponent()->SetBrakeInput(Value.Get<float>());
}

void APolyRacingWheeledVehiclePawn::ApplySteering(const FInputActionValue& Value)
{
	GetVehicleMovementComponent()->SetSteeringInput(Value.Get<float>());
}

void APolyRacingWheeledVehiclePawn::OnHandBrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void APolyRacingWheeledVehiclePawn::OnHandBrakeReleased()
{
GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

void APolyRacingWheeledVehiclePawn::UpdateInAirControl(float DeltaTime)
{
}

