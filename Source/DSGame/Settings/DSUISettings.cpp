#include "DSUISettings.h"

UDSUISettings::UDSUISettings()
{
	
}

UDSUISettings* UDSUISettings::Get()
{
	static UDSUISettings* Instance;
	if (!Instance)
	{
		for (const TObjectIterator<UDSUISettings> Itr(RF_NoFlags); Itr;)
		{
			Instance = *Itr;
			break;
		}
	}

	return Instance;
}
