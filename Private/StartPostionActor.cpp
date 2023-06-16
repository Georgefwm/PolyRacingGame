// Fill out your copyright notice in the Description page of Project Settings.


#include "StartPositionActor.h"
#include "Net/UnrealNetwork.h"

AStartPositionActor::AStartPositionActor()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");

	ArrowMarker = CreateDefaultSubobject<UArrowComponent>("MarkerMesh");
	ArrowMarker->SetupAttachment(RootComponent);

	// There is probably a better way to do this... Does the job but isn't very flexible
	// TODO: Allow the positions/amount to be dynamically changed
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

void AStartPositionActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	DOREPLIFETIME(AStartPositionActor, Owner);
	DOREPLIFETIME(AStartPositionActor, CurrentPlayerIndex);
}

void AStartPositionActor::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(true);

	for (UStaticMeshComponent* Mesh : PreviewMeshes)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

FTransform AStartPositionActor::GetSpawnTransformFromIndex(int PlayerIndex)
{
	if (!PreviewMeshes.IsValidIndex(PlayerIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Player index not valid"))
		return FTransform();
	}

	return PreviewMeshes.GetData()[PlayerIndex]->GetComponentTransform();
}

/*
 * @return FTransform describing the start position
 *
 * @note only call once, or there will be non-uniform positions.
 * Used to quickly setup a start location for a new player.
 */
FTransform AStartPositionActor::GetNextSpawnTransform()
{
	const FTransform Transform = GetSpawnTransformFromIndex(CurrentPlayerIndex);

	UE_LOG(LogTemp, Warning, TEXT("Position %i: %s"), CurrentPlayerIndex, *Transform.GetLocation().ToString())

	CurrentPlayerIndex++;
	
	return Transform;
}

void AStartPositionActor::UpdateEditorPreview()
{
	int Iterations = 0;
	
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation + GetActorUpVector() * -5000;

	FHitResult HitResult;
	
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	
	// Try find ground level
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams);
	
	float GroundHeight = 0.f;
	if (bHit)
		GroundHeight = HitResult.ImpactPoint.Z;

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
				0.f);
			
			PreviewMeshes.GetData()[Iterations]->SetRelativeLocation(NewLocation);

			if (bHit)
			{
				Iterations++;
				continue;
			}

			// Apply world position for Z axis if ground height was found
			FVector CurrentLocation = PreviewMeshes.GetData()[Iterations]->GetComponentLocation();
			FVector ZWorldPosition(CurrentLocation.X, CurrentLocation.Y, GroundHeight + StartHeight);
			
			PreviewMeshes.GetData()[Iterations]->SetWorldLocation(ZWorldPosition);
			
			Iterations++;
		}
	}
}
