// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "StartPositionActor.generated.h"

/**
 * 
 */
UCLASS()
class POLYRACINGGAME_API AStartPositionActor : public AActor
{
	GENERATED_BODY()

public:
	
	AStartPositionActor();

	virtual void BeginPlay() override;

	FTransform GetSpawnTransformFromIndex(int PlayerIndex);

	FTransform GetNextSpawnTransform();

	UPROPERTY(Replicated)
	int CurrentPlayerIndex;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UArrowComponent* ArrowMarker;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Editor Preview")
	TArray<UStaticMeshComponent*> PreviewMeshes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Editor Preview")
	UStaticMesh* PreviewMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Start Parameters")
	bool UseVehicleForPreview = false;

	// Update the transform of the start position editor previews
	UFUNCTION(CallInEditor, Category="Start Parameters")
	void UpdateEditorPreview();

	// How high above the ground the start positions should be
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Start Parameters")
	float StartHeight = 20.f;

	// Amount of start locations, locked for now
	UPROPERTY(BlueprintReadOnly, Category="Start Parameters")
	int MaxStartLocations = 8;

	// Z will not be used, just feels cleaner to use a vector than two fields
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Start Parameters")
	FIntVector Shape = FIntVector(2, 4, 0);

	// Again, Z will not be used.
	// Distance between each starting position
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Start Parameters")
	FVector Size = FVector(50.f, 50.f, 0.f);
};



