// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/IbrilAniminstanceCeplusplus.h"
#include "Characters/IbrilChar.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UIbrilAniminstanceCeplusplus::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ibrilCharacter = Cast<AIbrilChar>(TryGetPawnOwner());
	if (ibrilCharacter)
	{
		ibrilCharMovement = ibrilCharacter->GetCharacterMovement();
	}
}

void UIbrilAniminstanceCeplusplus::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (ibrilCharMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(ibrilCharMovement->Velocity);
		
	}
}
