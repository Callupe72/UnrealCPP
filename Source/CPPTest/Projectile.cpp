// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"

#include "Engine/DecalActor.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//StaticMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);


	//MESH
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/StarterContent/Props/MaterialSphere.MaterialSphere'"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MonMesh"));
	if (CubeMesh.Succeeded())
	{
		Mesh->SetStaticMesh(CubeMesh.Object);
	}

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereCollision->InitSphereRadius(50.f);

	SphereCollision->BodyInstance.SetCollisionProfileName(TEXT("CollisionProfile"));
	Mesh->BodyInstance.SetCollisionProfileName(TEXT("CollisionProfile"));
	SphereCollision->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	Mesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	SetRootComponent(SphereCollision);

	//PROJECTILE
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjMove"));
	ProjectileMovement->SetUpdatedComponent(Mesh);
	ProjectileMovement->InitialSpeed = speed;
	ProjectileMovement->MaxSpeed = speed;
	ProjectileMovement->ProjectileGravityScale = 0.005f;
	ProjectileMovement->bShouldBounce = true;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	//this->SetActorScale3D(FVector(0.1f, 0.1, 0.1));
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GLog->Log("Hit");
	GetWorld()->SpawnActor<ADecalActor>(this->GetActorLocation(), this->GetActorRotation());
	if(destroyOnHit)
	{
		//Destroy(this);	
	}
}

