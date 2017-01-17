#include <LouEnginePrecompile.h>

#include <core/application/CApplication.h>

CApplication::CApplication( ) 
{ 
}

bool CApplication::Init(const char *appTitle, unsigned xRes, unsigned yRes) {
	printf("Project '%s' initialized\n", appTitle);
  return InitProject( );
}

void CApplication::Update( ) {
	printf("Updating...\n");
	UpdateProject();
	getchar();
}

void CApplication::Destroy( ) {
	printf("Destroying...\n");
	DestroyProject( );
	getchar();
}
