// Fill out your copyright notice in the Description page of Project Settings.


#include "StartPositionActor.h"

AStartPositionActor::AStartPositionActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	
}

void AStartPositionActor::BeginPlay()
{
	Super::BeginPlay();
}
