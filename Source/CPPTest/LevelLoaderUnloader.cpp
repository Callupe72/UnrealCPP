// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelLoaderUnloader.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALevelLoaderUnloader::ALevelLoaderUnloader()
{
	PrimaryActorTick.bCanEverTick = true;


	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Material(TEXT("Material'/Game/Material/Terrain/LoadUnloadMat.LoadUnloadMat'"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	StaticMesh->SetCollisionProfileName(TEXT("Trigger"));
	if (CubeMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(CubeMesh.Object);
	}

	if(Material.Succeeded())
	{
		StaticMesh->SetMaterial(0,Material.Object);
	}
	StaticMesh->SetSimulatePhysics(false);
	
	this->SetHidden(true);
	
	OnActorBeginOverlap.AddDynamic(this, &ALevelLoaderUnloader::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ALevelLoaderUnloader::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ALevelLoaderUnloader::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void ALevelLoaderUnloader::Tick(float DeltaTime)
{

}


void ALevelLoaderUnloader::LoadUnload(bool Load)
{
	FLatentActionInfo LatentInfo;
	if(Load)
	{
		UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, false, LatentInfo);
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, ("LOAD " + LevelToLoad.ToString() ));
		}
	}
	else
	{
		UGameplayStatics::UnloadStreamLevel(this, LevelToLoad, LatentInfo, false);
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, ("UNLOAD " + LevelToLoad.ToString() ));
		}
	}
}
void ALevelLoaderUnloader::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	ACPPTestCharacter* player = Cast<ACPPTestCharacter>(OtherActor);
	if (player == nullptr)
	{
		return;
	}

	if(ActionToDo == ActionToDoEnum::LoadWhileIn || ActionToDo == ActionToDoEnum::LoadOnEnter)
	{
		LoadUnload(true);
	}
	else
	{
		LoadUnload(false);
	}
	
}

void ALevelLoaderUnloader::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	ACPPTestCharacter* player = Cast<ACPPTestCharacter>(OtherActor);
	if (player == nullptr)
	{
		return;
	}

	if(ActionToDo == ActionToDoEnum::LoadWhileIn)
	{
		LoadUnload(false);
	}
}

