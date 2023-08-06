#include "ProtoMovementCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MathUtil.h"
#include "Kismet/KismetMathLibrary.h"
#include "CharacterUtil.h"

#define SELF AProtoMovementCharacter

AProtoMovementCharacter::AProtoMovementCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate
	
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AProtoMovementCharacter::BeginPlay()
{
	Super::BeginPlay();

	MaxDashCharges = 1;
	CanAirJump = true;

	//Load ability information

	ResetDashCharge();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AProtoMovementCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &SELF::JumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &SELF::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &SELF::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &SELF::Look);

		EnhancedInputComponent->BindAction(HDashAction, ETriggerEvent::Started, this, &SELF::HorizontalDashStart);
		EnhancedInputComponent->BindAction(VDashAction, ETriggerEvent::Started, this, &SELF::VerticalDashStart);
	}
}

void AProtoMovementCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(InHDash) HDashLoop(DeltaTime);

	if(InVDash) VDashLoop(DeltaTime);
}

void AProtoMovementCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProtoMovementCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AProtoMovementCharacter::JumpInput()
{
	if(GetCharacterMovement()->IsFalling() && CanAirJump)
	{
		GetCharacterMovement()->Velocity *= AirJumpCancelVelocity;
		GetCharacterMovement()->AddImpulse(FVector::UpVector * AirJumpForce, true);
		CanAirJump = false;
		OnAirJump();
		return;
	}

	Jump();
}

void AProtoMovementCharacter::HorizontalDashStart()
{
	if(!HasHorizontalDash && !DebugAbilityBypass) return;
	if(InHDash || InVDash || CurrentDashCharges <= 0) return;
	OnHorizontalDashStart();
	GetWorldTimerManager().ClearTimer(DashTimerHandle);
	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &SELF::HorizontalDashAction, HDashActionDelay, false);
}

void AProtoMovementCharacter::HorizontalDashAction()
{
	ExitDash();
	InHDash = true;
	OnHorizontalDashAction();
	CurrentDashCharges--;

	GetCharacterMovement()->Velocity *= HDashCancelVelocity;
	DashDirection = GetActorQuat().RotateVector(HDashDirection.GetSafeNormal());
	GetCharacterMovement()->Velocity += DashDirection * HDashForce/HDashDuration;

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetController()->SetIgnoreMoveInput(true);

	DashStartLocation = GetActorLocation();
}

void AProtoMovementCharacter::HDashLoop(float DeltaTime)
{
	const float Decel = FMathf::Pow(DashLoopTimer, HDashBrakingSlope) * (HDashForce/(HDashDuration*HDashDuration)) * HDashBrakingScale;

	FVector Velocity = GetCharacterMovement()->Velocity;

	const FVector VelocitySign = FVector(FMath::Sign(Velocity.X), FMath::Sign(Velocity.Y), Velocity.Z > 0);

	Velocity -= VelocitySign * Decel*DeltaTime;
	const float GravityZ = (GetMovementComponent()->GetGravityZ()*HDashGravityScale);
	Velocity = GetCharacterMovement()->NewFallVelocity(Velocity, FVector::UpVector*GravityZ, DeltaTime);

	GetCharacterMovement()->Velocity = Velocity;

	if(DashLoopTimer >= HDashDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("Output: %f"), (GetActorLocation() - DashStartLocation).Length());
		OnHorizontalDashFinished();
		ExitDash();
		return;
	}

	DashLoopTimer += DeltaTime;
	DashLoopTimer = FMathf::Clamp(DashLoopTimer, 0, HDashDuration);
}

void AProtoMovementCharacter::VerticalDashStart()
{
	if(!HasVerticalDash && !DebugAbilityBypass) return;
	if(InHDash || InVDash|| CurrentDashCharges <= 0) return;
	OnVerticalDashStart();
	GetWorldTimerManager().ClearTimer(DashTimerHandle);
	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &SELF::VerticalDashAction, VDashActionDelay, false);
}

void AProtoMovementCharacter::VerticalDashAction()
{
	ExitDash();
	InVDash = true;
	OnVerticalDashAction();
	CurrentDashCharges--;

	GetCharacterMovement()->Velocity *= VDashCancelVelocity;
	DashDirection = GetActorQuat().RotateVector(VDashDirection.GetSafeNormal());
	GetCharacterMovement()->Velocity += DashDirection * VDashForce/VDashDuration;

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetController()->SetIgnoreMoveInput(true);

	DashStartLocation = GetActorLocation();
}

void AProtoMovementCharacter::VDashLoop(float DeltaTime)
{
	const float Decel = FMathf::Pow(DashLoopTimer, VDashBrakingSlope) * (VDashForce/(VDashDuration*VDashDuration)) * VDashBrakingScale;

	FVector Velocity = GetCharacterMovement()->Velocity;

	const FVector VelocitySign = FVector(FMath::Sign(Velocity.X), FMath::Sign(Velocity.Y), Velocity.Z > 0);

	Velocity -= VelocitySign * Decel*DeltaTime;
	const float GravityZ = (GetMovementComponent()->GetGravityZ()*VDashGravityScale);
	Velocity = GetCharacterMovement()->NewFallVelocity(Velocity, FVector::UpVector*GravityZ, DeltaTime);

	GetCharacterMovement()->Velocity = Velocity;

	if(DashLoopTimer >= VDashDuration)
	{
		UE_LOG(LogTemp, Warning, TEXT("Output: %f"), (GetActorLocation() - DashStartLocation).Length());
		OnVerticalDashFinished();
		ExitDash();
		return;
	}

	DashLoopTimer += DeltaTime;
	DashLoopTimer = FMathf::Clamp(DashLoopTimer, 0, VDashDuration);
}

void AProtoMovementCharacter::ResetDashCharge()
{
	CurrentDashCharges = MaxDashCharges;
}

void AProtoMovementCharacter::ExitDash()
{
	InHDash = false;
	InVDash = false;
	DashLoopTimer = 0;
	CanAirJump = true;
	
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	GetController()->SetIgnoreMoveInput(false);
}

void AProtoMovementCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	ResetDashCharge();
	CanAirJump = true;
}

bool AProtoMovementCharacter::IsInHorizontalDash() { return InHDash; }

bool AProtoMovementCharacter::IsInVerticalDash() { return InVDash; }

bool AProtoMovementCharacter::IsInDash() { return InHDash || InHDash; }

void AProtoMovementCharacter::GiveAbility(const EAbility Ability)
{
	switch (Ability)
	{
	case EAbility::Ability_HorizontalDash: HasHorizontalDash = true; break;
	case EAbility::Ability_VerticalDash: HasVerticalDash = true; break;
	case EAbility::Ability_DashCharges: GainDashCharge(); break;
	}

	//Save to file
}

void AProtoMovementCharacter::GainDashCharge(const int Amount)
{
	MaxDashCharges += Amount;

	//Save to file
}

void AProtoMovementCharacter::SetDashCharges(const int Amount)
{
	MaxDashCharges = Amount;

	//Save to file
}

bool AProtoMovementCharacter::HasAbility(EAbility Ability)
{
	switch (Ability)
	{
		case EAbility::Ability_HorizontalDash: return HasHorizontalDash;
		case EAbility::Ability_VerticalDash: return HasVerticalDash;	
	}

	return false;
}

int AProtoMovementCharacter::GetMaxDashCharges() const { return MaxDashCharges; }

int AProtoMovementCharacter::GetDashCharges() { return CurrentDashCharges; }

bool AProtoMovementCharacter::HasDashCharge() { return CurrentDashCharges > 0; }
