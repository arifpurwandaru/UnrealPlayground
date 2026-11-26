// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "IbrilChar.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

class UGroomComponent;

UCLASS()
class UNREALPLAYGROUND_API AIbrilChar : public ACharacter
{
	GENERATED_BODY()

public:
	AIbrilChar();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category=Input)
	UInputMappingContext* ibrilIMC;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ibrilMovement;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ibrilLook;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);
	void Jump();

private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere, Category="Hair")
	UGroomComponent* hair;
	UPROPERTY(VisibleAnywhere, Category="Hair")
	UGroomComponent* eyebrows;


};
