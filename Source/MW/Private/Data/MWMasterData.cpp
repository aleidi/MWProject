#include "Data/MWMasterData.h"

FPrimaryAssetId UMWMasterData::MasterDataID = FPrimaryAssetId(FPrimaryAssetType(TEXT("MasterData")), FName(TEXT("MWMasterData")));

FPrimaryAssetId UMWMasterData::GetPrimaryAssetId() const
{
	return MasterDataID;
}
