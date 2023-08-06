#include "AlchemistCharacter.h"

#include "AbilityComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ThirdPersonCamera.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AAlchemistCharacter::AAlchemistCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	CharacterCamera = CreateDefaultSubobject<UThirdPersonCamera>(TEXT("Character Camera"));
	CharacterCamera->SetupAttachment(RootComponent);

	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("Character Abilities"));
}

void AAlchemistCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AAlchemistCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AAlchemistCharacter::GetGroundLocation() const
{
	return GetActorLocation() - GetCapsuleComponent()->GetScaledCapsuleHalfHeight()*GetActorUpVector();
}

void AAlchemistCharacter::JumpAction(const bool bForce /*=false*/)
{
	if (!bForce && IsIgnoreInputActions()) return;
	
	OnJumpAction();
}

void AAlchemistCharacter::Move(const FVector2D Value)
{
	if (IsIgnoreInputActions()) return;
	
	if (Controller != nullptr)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, Value.Y);
		AddMovementInput(RightDirection, Value.X);
		OnMove();
	}
}

void AAlchemistCharacter::Look(const FVector2D Value)
{
	if (Controller != nullptr && CharacterCamera != nullptr)
	{
		CharacterCamera->TurnCamera(Value.X, Value.Y);
		OnLook();
	}
}

void AAlchemistCharacter::Die()
{
	AbilityComponent->ClearBombQueue();
	OnDying();
}

void AAlchemistCharacter::ResetExtraJump()
{
	OnResetExtraJump();	
}

void AAlchemistCharacter::ResetGravity()
{
	OnResetGravity();
}

void AAlchemistCharacter::Boost()
{
	OnBoost();
}

void AAlchemistCharacter::IgnoreInputActions(const bool bIgnore)
{
	bIgnoreActions = bIgnore;
}

bool AAlchemistCharacter::IsIgnoreInputActions() const
{
	return bIgnoreActions;
}
