// Fill out your copyright notice in the Description page of Project Settings.


#include "SAdvancedTransformInputBox.h"
#include "StartPositionActor.h"
#include "Kismet/KismetMathLibrary.h"

AStartPositionActor::AStartPositionActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");

	UStaticMeshComponent* Position1 = CreateDefaultSubobject<UStaticMeshComponent>("Position1");
	UStaticMeshComponent* Position2 = CreateDefaultSubobject<UStaticMeshComponent>("Position2");
	UStaticMeshComponent* Position3 = CreateDefaultSubobject<UStaticMeshComponent>("Position3");
	UStaticMeshComponent* Position4 = CreateDefaultSubobject<UStaticMeshComponent>("Position4");
	UStaticMeshComponent* Position5 = CreateDefaultSubobject<UStaticMeshComponent>("Position5");
	UStaticMeshComponent* Position6 = CreateDefaultSubobject<UStaticMeshComponent>("Position6");
	UStaticMeshComponent* Position7 = CreateDefaultSubobject<UStaticMeshComponent>("Position7");
	UStaticMeshComponent* Position8 = CreateDefaultSubobject<UStaticMeshComponent>("Position8");

	Position1->SetupAttachment(RootComponent);
	Position2->SetupAttachment(RootComponent);
	Position3->SetupAttachment(RootComponent);
	Position4->SetupAttachment(RootComponent);
	Position5->SetupAttachment(RootComponent);
	Position6->SetupAttachment(RootComponent);
	Position7->SetupAttachment(RootComponent);
	Position8->SetupAttachment(RootComponent);
	
	PreviewMeshes.Add(Position1);
	PreviewMeshes.Add(Position2);
	PreviewMeshes.Add(Position3);
	PreviewMeshes.Add(Position4);
	PreviewMeshes.Add(Position5);
	PreviewMeshes.Add(Position6);
	PreviewMeshes.Add(Position7);
	PreviewMeshes.Add(Position8);
}

void AStartPositionActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorHiddenInGame(true);
}

FTransform AStartPositionActor::GetSpawnTransformFromIndex(int PlayerIndex)
{
	return FTransform();
}

void AStartPositionActor::UpdateEditorPreview()
{
	int Iterations = 0;

	

	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + (GetActorUpVector() * -1000);

	FHitResult HitResult;
	
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	QueryParams.AddIgnoredActor(this);

	// Perform the line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);

	FVector GroundHeight = FVector::Zero();
	if (bHit)
		GroundHeight = HitResult.ImpactPoint;

	float YOffset = 0.f;
	if (Shape.X % 2 == 0)
		YOffset = Size.X * 0.5;
	
	for (int y = 0; y < Shape.Y; y++)
	{
		for (int x = 0; x < Shape.X; x++)
		{
			if (Iterations >= MaxStartLocations)
			{
				UE_LOG(LogTemp, Warning, TEXT("Exhausted iterations"))
				return;
			}
			
			if (!PreviewMeshes.IsValidIndex(Iterations))
			{
				UE_LOG(LogTemp, Warning, TEXT("Preview mesh out of bounds"))
				return;
			}

			if (!PreviewMesh)
			{
				UE_LOG(LogTemp, Warning, TEXT("Preview mesh not set"))
				return;
			}
			
			PreviewMeshes.GetData()[Iterations]->SetStaticMesh(PreviewMesh);

			UE_LOG(LogTemp, Warning, TEXT("RESULT: %i"), Shape.X % 2)

			FVector NewLocation = FVector(
				y * -Size.Y,
				x * Size.X - Size.X * Shape.X / 2 + YOffset,
				GroundHeight.Z);
			
			PreviewMeshes.GetData()[Iterations]->SetRelativeLocation(NewLocation);

			Iterations++;
		}
	}
}
