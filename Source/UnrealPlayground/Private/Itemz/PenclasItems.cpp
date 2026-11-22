// Fill out your copyright notice in the Description page of Project Settings.


#include "Itemz/PenclasItems.h"
#include "UnrealPlayground/DebugMacros.h"

// Sets default values
APenclasItems::APenclasItems()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APenclasItems::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APenclasItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;
	float deltaZ = Amplitude * FMath::Sin(RunningTime * TimeConstant);
	
	AddActorWorldOffset(FVector(0.f, 0.f, deltaZ));
	AddActorWorldRotation(FRotator(0.f, DeltaTime * RotationSpeed, 0.f));

	DRAW_SPHERE_SingleFrame(GetActorLocation(), FColor::Red);
	DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f, FColor::Red);


}

