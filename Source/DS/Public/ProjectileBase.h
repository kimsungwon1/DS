

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TurnSwitcher.h"
#include "ProjectileBase.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UCharacterInstanceComponent;
class UDSSpellData;

UCLASS(Abstract)
class DS_API AProjectileBase : public AActor, public ITurnSwitcher
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileBase();

	// 스폰 직후 호출 — 시전자를 콜리전 무시 목록에 등록하고, 명중 시 데미지 적용에 사용할 정보를 저장
	void Initialize(UCharacterInstanceComponent* InCaster, UDSSpellData* InSpellData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ReceiveTurn_Implementation(ADSGameMode* mode, UObject* passer) {}

	virtual void TurnFinished_Implementation(ADSGameMode* mode, float fDelay) {}

	virtual void PassTurn_Implementation(ADSGameMode* mode, UObject* switcher) {}

protected:
	// 캐릭터(적/아군)에 겹쳤을 때 — 적이면 데미지 적용 후 소멸, 아군/시전자면 무시하고 통과
	UFUNCTION()
	void OnProjectileOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 캐릭터가 아닌 것(벽 등)에 물리적으로 막혔을 때 — 소멸
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> CollisionComp;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UCharacterInstanceComponent> Caster;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UDSSpellData> SpellData;
};
