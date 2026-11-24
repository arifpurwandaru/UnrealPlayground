// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Srigala.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimSequence.h"
#include "Engine/SkeletalMesh.h"
#include "InputCoreTypes.h"
#include "UObject/ConstructorHelpers.h"
#include "UnrealPlayground/DebugMacros.h"

// Sets default values
ASrigala::ASrigala()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCapsuleHalfHeight(75.f);
	GetCapsuleComponent()->SetCapsuleRadius(40.f);

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm, USpringArmComponent::SocketName);
	ConfigureFollowCamera();

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;

	static ConstructorHelpers::FObjectFinder<UAnimSequence> idleAsset(TEXT("/Game/AnimalVarietyPack/Wolf/Animations/ANIM_Wolf_IdleBreathe.ANIM_Wolf_IdleBreathe"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> walkAsset(TEXT("/Game/AnimalVarietyPack/Wolf/Animations/ANIM_Wolf_Walk.ANIM_Wolf_Walk"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> runAsset(TEXT("/Game/AnimalVarietyPack/Wolf/Animations/ANIM_Wolf_Run.ANIM_Wolf_Run"));
	idleAnimation = idleAsset.Object;
	walkAnimation = walkAsset.Object;
	runAnimation = runAsset.Object;

	runAction = CreateDefaultSubobject<UInputAction>(TEXT("RunAction"));
	runAction->ValueType = EInputActionValueType::Boolean;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASrigala::ConfigureFollowCamera()
{
	// Old Blueprint offsets must not be added on top of the spring-arm length.
	springArm->SetAbsolute(false, false, false);
	springArm->SetRelativeTransform(FTransform::Identity);
	springArm->TargetArmLength = cameraDistance;
	springArm->TargetOffset = FVector(0.f, 0.f, 40.f);
	springArm->SocketOffset = FVector::ZeroVector;
	springArm->bUsePawnControlRotation = true;
	springArm->bInheritPitch = true;
	springArm->bInheritYaw = true;
	springArm->bInheritRoll = false;
	springArm->bEnableCameraLag = false;
	springArm->bEnableCameraRotationLag = false;
	springArm->bDoCollisionTest = true;

	camera->SetAbsolute(false, false, false);
	camera->SetRelativeTransform(FTransform::Identity);
	camera->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void ASrigala::BeginPlay()
{
	Super::BeginPlay();

	springArm->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	camera->AttachToComponent(springArm, FAttachmentTransformRules::SnapToTargetIncludingScale, USpringArmComponent::SocketName);
	ConfigureFollowCamera();
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	camera->Activate();

	StopRunning();
	if (bUseSimpleLocomotionAnimation)
	{
		const USkeletalMesh* meshAsset = GetMesh()->GetSkeletalMeshAsset();
		if (!meshAsset || !idleAnimation || !walkAnimation || !runAnimation)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Assign the wolf to the inherited Mesh and assign all three locomotion animations."), *GetName());
			bUseSimpleLocomotionAnimation = false;
		}
		else if (idleAnimation->GetSkeleton() != meshAsset->GetSkeleton() ||
			walkAnimation->GetSkeleton() != meshAsset->GetSkeleton() ||
			runAnimation->GetSkeleton() != meshAsset->GetSkeleton())
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Locomotion animations must use the inherited Mesh's skeleton."), *GetName());
			bUseSimpleLocomotionAnimation = false;
		}
		else
		{
			UpdateLocomotionAnimation();
		}
	}

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(srigalaMappingContext, 0);
		}
	}
	
}

void ASrigala::Move(const FInputActionValue& Value)
{
	const float directionValue = Value.Get<float>();
	if (Controller && (directionValue != 0.0f))
	{
		const FRotator yawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector direction = yawRotation.Vector();
		AddMovementInput(direction, directionValue);

	}
}

void ASrigala::Look(const FInputActionValue& Value)
{
	const FVector2D lookValue = Value.Get<FVector2D>();
	if (Controller && (lookValue != FVector2D::ZeroVector))
	{
		AddControllerYawInput(lookValue.X * lookSensitivity);
		AddControllerPitchInput(-lookValue.Y * lookSensitivity);
	}
}

void ASrigala::RemoveRunMappingContext()
{
	if (UEnhancedInputLocalPlayerSubsystem* subsystem = runInputSubsystem.Get())
	{
		subsystem->RemoveMappingContext(runMappingContext);
	}
	runInputSubsystem.Reset();
	StopRunning();
}

void ASrigala::UnPossessed()
{
	RemoveRunMappingContext();
	Super::UnPossessed();
}

void ASrigala::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveRunMappingContext();
	Super::EndPlay(EndPlayReason);
}

void ASrigala::StartRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = runSpeed;
}

void ASrigala::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void ASrigala::UpdateLocomotionAnimation()
{
	const float speed = GetVelocity().Size2D();
	UAnimSequence* animation = idleAnimation;
	if (speed > 3.f)
	{
		const float runThreshold = (walkSpeed + runSpeed) * 0.5f;
		animation = speed >= runThreshold ? runAnimation : walkAnimation;
	}

	if (animation != currentAnimation)
	{
		GetMesh()->PlayAnimation(animation, true);
		currentAnimation = animation;
	}
}

// Called every frame
void ASrigala::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bUseSimpleLocomotionAnimation)
	{
		UpdateLocomotionAnimation();
	}
}

// Called to bind functionality to input
void ASrigala::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &ASrigala::Move);
		enhancedInputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &ASrigala::Look);

		RemoveRunMappingContext();
		if (!runAction || runAction->ValueType != EInputActionValueType::Boolean)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Run Action must be assigned and use the Boolean value type."), *GetName());
			return;
		}
		if (runSpeed <= walkSpeed)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Run Speed (%f) must exceed Walk Speed (%f)."), *GetName(), runSpeed, walkSpeed);
			return;
		}

		// Map the final Blueprint-selected action, not the constructor's original action.
		runMappingContext = NewObject<UInputMappingContext>(this);
		runMappingContext->MapKey(runAction, EKeys::LeftShift);
		enhancedInputComponent->BindAction(runAction, ETriggerEvent::Triggered, this, &ASrigala::StartRunning);
		enhancedInputComponent->BindAction(runAction, ETriggerEvent::Completed, this, &ASrigala::StopRunning);
		enhancedInputComponent->BindAction(runAction, ETriggerEvent::Canceled, this, &ASrigala::StopRunning);
	}

	if (APlayerController* playerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(runMappingContext, 1);
			runInputSubsystem = subsystem;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s: Enhanced Input local player subsystem is unavailable for running."), *GetName());
		}
	}

}

