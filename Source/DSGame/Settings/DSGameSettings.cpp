#include "DSGameSettings.h"

UDSGameSettings::UDSGameSettings()
{
	InputActionConfig = nullptr;
}

UDSGameSettings* UDSGameSettings::Get()
{
	static UDSGameSettings* Instance;
	if (!Instance)
	{
		for (const TObjectIterator<UDSGameSettings> Itr(RF_NoFlags); Itr;)
		{
			Instance = *Itr;
			break;
		}
	}

	return Instance;
}
