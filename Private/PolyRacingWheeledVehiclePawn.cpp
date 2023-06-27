﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "PolyRacingWheeledVehiclePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicles/Public/ChaosVehicleMovementComponent.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Customisation/VehicleCustomisationComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "UI/InGameHUD.h"


// Sets default values
APolyRacingWheeledVehiclePawn::APolyRacingWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;

	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetIsReplicated(true);
	MeshComponent->SetSimulatePhysics(true);
	
	SetupInputMappingContext();

	VehicleCustomisationComponent = CreateDefaultSubobject<UVehicleCustomisationComponent>("Customisation Component");
	VehicleCustomisationComponent->EditingMesh = GetMesh();
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	// Spring arm default settings
	SpringArmComponent->TargetArmLength = 700.f;
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));
	SpringArmComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.f, -10.f, 0.f)));
	SpringArmComponent->bEnableCameraRotationLag = true;
	SpringArmComponent->CameraRotationLagSpeed = 2.f;  // Lower = slower
	SpringArmComponent->bInheritRoll = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Camera default settings
	CameraComponent->FieldOfView = 90.f;

	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultVehicleWidgetFinder(TEXT("/Game/UI/WidgetBlueprints/WPB_VehiclePawn"));
	VehicleHUD = DefaultVehicleWidgetFinder.Class;
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

void APolyRacingWheeledVehiclePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetupInputMappingContext();
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
	
	if (APolyRacingPlayerController* PlayerController = Cast<APolyRacingPlayerController>(GetController()))
	{
		if (AInGameHUD* HUD = Cast<AInGameHUD>(PlayerController->GetHUD()))
		{
			// TODO: Move this so it isn't a binding between the vehicle and controller
			EnhancedInputComponent->BindAction(ToggleInGameMenuAction, ETriggerEvent::Started, HUD, &AInGameHUD::TogglePauseMenu);
		}
	}
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

