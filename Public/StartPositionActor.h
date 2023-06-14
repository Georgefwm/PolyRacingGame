// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Editor Preview")
	TArray<UStaticMeshComponent*> PreviewMeshes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Editor Preview")
	UStaticMesh* PreviewMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Start Parameters")
	bool UseVehicleForPreview = false;
	
	UFUNCTION(CallInEditor, Category="Start Parameters")
	void UpdateEditorPreview();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Start Parameters")
	int MaxStartLocations = 8;

	// Z will not be used, just feels cleaner to use a vector than two fields
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Start Parameters")
	FIntVector Shape = FIntVector(2, 4, 0);

	// Again, Z will not be used.
	// Distance between each starting position
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Start Parameters")
	FVector Size = FVector(50.f, 50.f, 0.f);
};



