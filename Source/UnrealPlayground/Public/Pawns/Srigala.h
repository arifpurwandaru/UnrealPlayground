// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Srigala.generated.h"

class UAnimSequence;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class UNREALPLAYGROUND_API ASrigala : public ACharacter
{
	GENERATED_BODY()

public:
	ASrigala();
	virtual void Tick(float DeltaTime) override;
	virtual void UnPossessed() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* srigalaMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* moveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* runAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (ClampMin = "100.0", ClampMax = "600.0"))
	float cameraDistance = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (ClampMin = "0.01", ClampMax = "2.0"))
	float lookSensitivity = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (ClampMin = "1.0"))
	float walkSpeed = 180.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement, meta = (ClampMin = "1.0"))
	float runSpeed = 450.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
	bool bUseSimpleLocomotionAnimation = true;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
	UAnimSequence* idleAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
	UAnimSequence* walkAnimation;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Animation)
	UAnimSequence* runAnimation;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartRunning();
	void StopRunning();
	void UpdateLocomotionAnimation();
	void ConfigureFollowCamera();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;

	UPROPERTY()
	UInputMappingContext* runMappingContext;
	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> runInputSubsystem;
	void RemoveRunMappingContext();

	UPROPERTY(Transient)
	UAnimSequence* currentAnimation = nullptr;

};
