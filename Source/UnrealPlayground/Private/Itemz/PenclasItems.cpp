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
	UE_LOG(LogTemp, Warning, TEXT("PenclasItems BeginPlay() called! hAPPY?"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("PenclasItems Loging on Screen Messsage yay"));
	}

	UWorld* World = GetWorld();

	FVector Location = GetActorLocation();
	DRAW_SPHERE(Location, FColor::Red);

	FVector ForwardVector = GetActorForwardVector();
	DRAW_VECTOR(Location, Location + ForwardVector * 100.f, FColor::Red);

	DRAW_CAPSULE(Location + FVector(0.f, 0.f, 100.f), FColor::Green);
	
}

// Called every frame
void APenclasItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

