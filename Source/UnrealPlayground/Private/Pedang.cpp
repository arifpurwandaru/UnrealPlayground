// Fill out your copyright notice in the Description page of Project Settings.


#include "Pedang.h"

// Sets default values
APedang::APedang()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	SetRootComponent(ItemMesh);

}

// Called when the game starts or when spawned
void APedang::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APedang::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	float deltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);

	AddActorWorldOffset(FVector(0.f, 0.f, deltaZ));
	AddActorWorldRotation(FRotator(0.f, DeltaTime * RotationSpeed, 0.f));

}

