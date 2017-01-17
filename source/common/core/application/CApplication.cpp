#include <LouEnginePrecompile.h>

#include <core/application/CApplication.h>

CApplication::CApplication( ) 
{ 
}

bool CApplication::Init(const char *appTitle, unsigned xRes, unsigned yRes) {
  return InitProject( );
}

void CApplication::Update( ) {
}

void CApplication::Destroy( ) {
  DestroyProject( );
}
