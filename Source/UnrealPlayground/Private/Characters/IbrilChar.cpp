#include "Characters/IbrilChar.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AIbrilChar::AIbrilChar()
{
	PrimaryActorTick.bCanEverTick = true;

	// tiga line below is to prevent the character from rotating when moving the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->bUsePawnControlRotation = true;
	springArm->TargetArmLength = 300.f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);

	// this line below is to make the character rotate when moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AIbrilChar::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(ibrilIMC, 0);
		}
	}

	
}

void AIbrilChar::Move(const FInputActionValue& value)
{
	const FVector2D movementVector = value.Get<FVector2D>();
	
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, movementVector.Y);
	AddMovementInput(Right, movementVector.X);
}

void AIbrilChar::Look(const FInputActionValue& value)
{
	const FVector2D lookVector = value.Get<FVector2D>();

	AddControllerPitchInput(lookVector.Y);
	AddControllerYawInput(lookVector.X);
}

void AIbrilChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIbrilChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(ibrilMovement, ETriggerEvent::Triggered, this, &AIbrilChar::Move);
		enhancedInputComponent->BindAction(ibrilLook, ETriggerEvent::Triggered, this, &AIbrilChar::Look);
	}
}

