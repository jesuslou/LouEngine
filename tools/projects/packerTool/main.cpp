#include <packer/CPackBuilder.h>
#include <utils/StringUtils.h>

#include <cstdlib>

int main(int argc, char** argv)
{
	if (argc == 3)
	{
		const char* const globalConfigFile = argv[1];
		const char* const packDefsFile = argv[2];
		printf("-- PACKERTOOL - Global Config File: %s\n", globalConfigFile);
		printf("-- PACKERTOOL - Packs Definition File: %s\n", packDefsFile);

		CPackBuilder builder(globalConfigFile);
		bool success = builder.GeneratePacksFromConfigurationFile(packDefsFile);
		if (success)
		{
			printf("\n-- PACKERTOOL - PACKS GENERATED SUCCESSFULLY!\n");
			return 0;
		}
	}
	else if (argc == 4)
	{
		const char* const globalConfigFile = argv[1];
		const char* const packDefsFile = argv[2];
		const char* const specificPacks = argv[3];
		printf("-- PACKERTOOL - Global Config File: %s\n", globalConfigFile);
		printf("-- PACKERTOOL - Packs Definition File: %s\n", packDefsFile);

		CPackBuilder builder(globalConfigFile);
		bool success = builder.GeneratePacksFromConfigurationFile(packDefsFile, StringUtils::SplitString(specificPacks, ","));
		if (success)
		{
			printf("\n-- PACKERTOOL - PACKS GENERATED SUCCESSFULLY!\n");
			return 0;
		}
	}

	printf("\n-- PACKERTOOL - SOMETHING WENT WRONG!\n");
	return 1;
}
