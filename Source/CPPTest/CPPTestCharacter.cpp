// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPTestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "CPPTestGameMode.h"
#include "PhysXInterfaceWrapperCore.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACPPTestCharacter

ACPPTestCharacter::ACPPTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACPPTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACPPTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPPTestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACPPTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACPPTestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACPPTestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACPPTestCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACPPTestCharacter::OnResetVR);

	
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &ACPPTestCharacter::Pickup);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ACPPTestCharacter::ShootProjectile);
}


void ACPPTestCharacter::OnResetVR()
{
	// If CPPTest is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in CPPTest.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACPPTestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACPPTestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ACPPTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACPPTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACPPTestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACPPTestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

AActor* ACPPTestCharacter::Raycast(int RaycastDistance)
{
	FHitResult OutHit;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector Forward = FollowCamera->GetForwardVector();

	Start = Start + (Forward * CameraBoom->TargetArmLength);

	FVector End = Start + (Forward * 500.f);
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this -> GetOwner());
	DrawDebugLine(GetWorld(), Start, End, FColor::Green,false, 1,0,1);

	bool IsHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionQueryParams);

	if(IsHit)
	{
		if(OutHit.GetActor())
		{
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, (FString(OutHit.GetActor()->GetName())));
			return OutHit.GetActor();
		}
	}
	return nullptr;
}

void ACPPTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	ACPPTestGameMode* GameMode = (ACPPTestGameMode*)GetWorld()->GetAuthGameMode();
	if (GameMode == nullptr)
	{
		GLog->Log("I can't find GameMode");
		return;
	}
	GameMode->SetSpawnPosition(GetTransform());
}


void ACPPTestCharacter::AddLife(int lifeToAdd)
{
	if (health == 0)
		return;

	ACPPTestCharacter::health += lifeToAdd;

	if (health > 100)
	{
		health = 100;
	}
	else if (health <= 0)
	{
		health = 0;
		Die();
	}

	if (GEngine)
	{
		if(lifeToAdd >= 0)
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, (FString(FString::FromInt(health))));
		else
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, (FString(FString::FromInt(health))));
	}
}
void ACPPTestCharacter::Die()
{
	GetMesh()->SetSimulatePhysics(true);
	GetCharacterMovement()->DisableMovement();
	GetMesh()->SetCollisionProfileName("Ragdoll");
	ACPPTestGameMode* GameMode = (ACPPTestGameMode*)GetWorld()->GetAuthGameMode();
	if (GameMode == nullptr)
	{
		GLog->Log("I can't find GameMode");
		return;
	}
	GameMode->RespawnNewPlayer();
}

void ACPPTestCharacter::Pickup()
{
	if(actorInHand == nullptr)
	{
		actorInHand = Raycast(raycastCheckDistance);
		if(actorInHand == nullptr)
		{
			return;
		}
		else
		{
			actorInHand->SetActorLocation(FollowCamera->GetComponentLocation() + (offsetPickup *FollowCamera->GetForwardVector()));
			
			actorInHand->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			//SUR LE MESH
			UPrimitiveComponent* charaInHand = Cast<UPrimitiveComponent>(actorInHand);
			if(charaInHand==nullptr)
			{
				return;
			}
			charaInHand->SetSimulatePhysics(false);
		}
	}
	else
	{
		UPrimitiveComponent* charaInHand = Cast<UPrimitiveComponent>(actorInHand);
		if(charaInHand==nullptr)
		{
			GLog->Log("NoCapsule");
			return;
		}
		charaInHand->SetSimulatePhysics(true);
		if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, (FString(FString::FromInt(health))));
		actorInHand->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		actorInHand = nullptr;
	}
}

void ACPPTestCharacter::ShootProjectile()
{
	//GetWorld()->SpawnActor<AProjectile>(arrowProj->GetComponentLocation(), arrowProj->GetComponentRotation());
}

