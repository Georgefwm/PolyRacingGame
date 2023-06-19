// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTables.h"
#include "Components/ActorComponent.h"
#include "VehicleCustomisationComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class POLYRACINGGAME_API UVehicleCustomisationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVehicleCustomisationComponent();

	UPROPERTY(Replicated)
	USkeletalMeshComponent* EditingMesh;

	UPROPERTY(ReplicatedUsing = OnRep_PrimaryColorChanged)
	int CurrentPrimaryColor = 0;

	UPROPERTY(ReplicatedUsing = OnRep_AccentColorChanged)
	int CurrentAccentColor = 0;

	UFUNCTION()
	void OnRep_PrimaryColorChanged();

	UFUNCTION()
	void OnRep_AccentColorChanged();

	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

	void SetPrimaryColor(UMaterialInstance* DesiredMaterialInstance);
	void SetAccentColor(UMaterialInstance* DesiredMaterialInstance);
};
