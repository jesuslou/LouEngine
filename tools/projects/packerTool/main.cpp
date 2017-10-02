#include <packer/CPackBuilder.h>

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
		builder.GeneratePacksFromConfigurationFile(packDefsFile);
		printf("\n-- PACKERTOOL - PACKS GENERATED SUCCESSFULLY!\n");
		return 0;
	}
	else
	{
		printf("\n-- PACKERTOOL - INCORRECT NUMBER OF PARAMETERS!\n");
		return 1;
	}
}
