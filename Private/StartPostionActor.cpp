// Fill out your copyright notice in the Description page of Project Settings.


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
	for (int i = 0; i < UKismetMathLibrary::Min(MaxStartLocations, (Shape.X * Shape.Y)); i++)
	{
		if (!PreviewMeshes.IsValidIndex(i))
			return;

		if (!PreviewMesh)
		{
			UE_LOG(LogTemp, Warning, TEXT("Preview mesh not set"))
			return;
		}
		
		UStaticMeshComponent* EditingMeshComponent = PreviewMeshes.GetData()[i];
		EditingMeshComponent->SetStaticMesh(PreviewMesh);

		FVector NewLocation = FVector(
			1.f,
			1.f,
			1.f
			);
		
		EditingMeshComponent->SetRelativeLocation(NewLocation);
	}
}
