// Fill out your copyright notice in the Description page of Project Settings.

#include "PolyRacingWheeledVehiclePawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "ChaosVehicles/Public/ChaosVehicleMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Controller/PolyRacingPlayerController.h"
#include "Customisation/VehicleCustomisationComponent.h"
#include "Framework/PolyRacingPlayerState.h"
#include "GameFramework/SpringArmComponent.h"


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
	SpringArmComponent->CameraRotationLagSpeed = 1.f;  // Lower = slower
	SpringArmComponent->bInheritRoll = false;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);

	// Camera default settings
	CameraComponent->FieldOfView = 90.f;

	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultVehicleWidgetFinder(TEXT("/Game/UI/WidgetBlueprints/WPB_VehiclePawn"));
	VehicleHUD = DefaultVehicleWidgetFinder.Class;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> WheelNiagaraSystemFinder(TEXT("/Game/FX/NS_SkidMark"));
	WheelNiagaraSystem = WheelNiagaraSystemFinder.Object;

	Fl_WheelAttachment = CreateDefaultSubobject<USceneComponent>("Fl_WheelAttachment");
	Fr_WheelAttachment = CreateDefaultSubobject<USceneComponent>("Fr_WheelAttachment");
	Rl_WheelAttachment = CreateDefaultSubobject<USceneComponent>("Rl_WheelAttachment");
	Rr_WheelAttachment = CreateDefaultSubobject<USceneComponent>("Rr_WheelAttachment");

	Fl_WheelAttachment->SetupAttachment(MeshComponent);
	Fr_WheelAttachment->SetupAttachment(MeshComponent);
	Rl_WheelAttachment->SetupAttachment(MeshComponent);
	Rr_WheelAttachment->SetupAttachment(MeshComponent);

	// These have to be in order based on WheelSetup in VehicleMovementComponent : ( FL -> FR -> RL -> RR )
	WheelAttachments.Add(Fl_WheelAttachment);
	WheelAttachments.Add(Fr_WheelAttachment);
	WheelAttachments.Add(Rl_WheelAttachment);
	WheelAttachments.Add(Rr_WheelAttachment);

	TyreSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	TyreSoundComponent->bAutoActivate = false;
	TyreSoundComponent->SetupAttachment(GetRootComponent());
	TyreSoundComponent->VolumeMultiplier = 0.1;
	BaseTyreSoundMultiplier = TyreSoundComponent->VolumeMultiplier;

	// Position a bit behind the rear axel
	TyreSoundComponent->SetRelativeLocation(FVector(Rl_WheelAttachment->GetRelativeLocation().X - 20.0f, 0.f, 20.f));

	static ConstructorHelpers::FObjectFinder<USoundBase> TyreSoundFinder(TEXT("/Game/Sounds/FX/C_TyreSqueelLoop"));
	TyreSoundComponent->Sound = TyreSoundFinder.Object;
}

// Called when the game starts or when spawned
void APolyRacingWheeledVehiclePawn::BeginPlay()
{
	Super::BeginPlay();

	BaseTyreSoundMultiplier = TyreSoundComponent->VolumeMultiplier;

	SetupInputMappingContext();

	if (WheelNiagaraSystem)
	{
		for (int WheelIndex = 0; WheelIndex < GetChaosVehicleMovementComponent()->GetNumWheels(); WheelIndex++)
		{
			WheelNiagaraComponents.Add(UNiagaraFunctionLibrary::SpawnSystemAttached(WheelNiagaraSystem,
				WheelAttachments.GetData()[WheelIndex],
				NAME_None,
				FVector(0.f),
				FRotator(0.f),
				EAttachLocation::Type::SnapToTarget,
				false));
			
			WheelNiagaraComponents.GetData()[WheelIndex]->SetPaused(true);
		}
	}
}

// Called every frame
void APolyRacingWheeledVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!WheelNiagaraSystem)
		return;

	int TyreSlippingCount = 0;
	int NoContactCount = 0;

	for (int WheelIndex = 0; WheelIndex < GetChaosVehicleMovementComponent()->GetNumWheels(); WheelIndex++)
	{
		FWheelStatus const WheelStatus = GetChaosVehicleMovementComponent()->GetWheelState(WheelIndex);
		UNiagaraComponent* WheelNiagaraComponent = WheelNiagaraComponents.GetData()[WheelIndex];

		// Apparently FWheelStatus.bInContact doesnt work
		bool WheelHasContact = !GetChaosVehicleMovementComponent()->Wheels[WheelIndex]->IsInAir();

		if (!WheelHasContact)
			NoContactCount++;
		
		// If we arent skidding then stop the effect
		if (!WheelStatus.bIsSkidding && !WheelStatus.bIsSlipping || !WheelHasContact) 
		{
			if (WheelNiagaraComponent)
			{
				FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
				WheelNiagaraComponent->DetachFromComponent(DetachmentTransformRules);
				
				WheelNiagaraComponent->SetActive(false);
				
				WheelNiagaraComponents[WheelIndex] = nullptr;
			}
			
			continue;
		}

		TyreSlippingCount++;
		
		// If we arent skidding already, reset and start the effect
		if (!WheelNiagaraComponent)
		{
			UNiagaraComponent* NewComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(WheelNiagaraSystem,
				WheelAttachments.GetData()[WheelIndex],
				NAME_None,
				FVector(0.f),
				FRotator(0.f),
				EAttachLocation::Type::SnapToTarget,
				true);

			WheelNiagaraComponents[WheelIndex] = NewComponent;
		}
	}
	
	if (NoContactCount >= 3)
	{
		RollTimer += DeltaTime;

		if (RollTimer > 5)
		{
			// Prompt vehicle reset
			// HandleVehicleReset();
		}
	}
	else
		RollTimer = 0;

	if (TyreSlippingCount > 0)
	{
		if (!TyreSoundComponent->GetSound())
			return;
		
		TyreSoundComponent->SetVolumeMultiplier(BaseTyreSoundMultiplier * (TyreSlippingCount / 3));
		
		if (!TyreSoundComponent->IsPlaying())
			TyreSoundComponent->FadeIn(0.3);
	}
	else
	{
		if (TyreSoundComponent->IsPlaying())
			TyreSoundComponent->FadeOut(0.15, 0.0f); // TODO: Fade out
	}
}

void APolyRacingWheeledVehiclePawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	SetupInputMappingContext();
}

void APolyRacingWheeledVehiclePawn::SetupInputMappingContext()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
		return;
	
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	if (!InputSubsystem)
		return;

	InputSubsystem->AddMappingContext(InputMappingContext, 0);
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

	EnhancedInputComponent->BindAction(ResetAction, ETriggerEvent::Completed, this, &APolyRacingWheeledVehiclePawn::HandleVehicleReset);
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

void APolyRacingWheeledVehiclePawn::HandleVehicleReset()
{
	APolyRacingPlayerController* PlayerController = GetController<APolyRacingPlayerController>();
	if (!PlayerController)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Requested vehicle reset"))
	
	PlayerController->Server_RequestCheckpointRestart(PlayerController);
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

UChaosWheeledVehicleMovementComponent* APolyRacingWheeledVehiclePawn::GetChaosVehicleMovementComponent()
{
	return Cast<UChaosWheeledVehicleMovementComponent>(GetMovementComponent());
}

