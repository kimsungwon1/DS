#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DSVoiceSet.generated.h"

/**
 * 캐릭터 음성 세트. 여러 캐릭터가 같은 에셋을 공유할 수 있습니다.
 * 각 항목은 SoundCue(랜덤 재생)나 SoundWave 모두 사용 가능합니다.
 */
UCLASS(BlueprintType)
class DS_API UDSVoiceSet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// 공격 기합
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<USoundBase> AttackCry;

	// 피격 반응
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<USoundBase> HitReaction;

	// 사망
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<USoundBase> Death;

	// 전투 승리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<USoundBase> Victory;

	// 레벨업
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice")
	TObjectPtr<USoundBase> LevelUp;
};
