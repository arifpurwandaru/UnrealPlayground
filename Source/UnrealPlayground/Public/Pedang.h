// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pedang.generated.h"

UCLASS()
class UNREALPLAYGROUND_API APedang : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APedang();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;
	float RunningTime;
	UPROPERTY(EditAnywhere)
	float Amplitude = 7.f;
	UPROPERTY(EditAnywhere)
	float TimeConstant = 5.f;
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 45.f;

};
