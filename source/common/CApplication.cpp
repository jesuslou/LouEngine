#include "common.h"
#include "application_base.h"
#include "render/renderer.h"
#include <SDL.h>
#include <stdio.h>
#include <time.h>
#include <SDL_image.h>
#include "audio/audio_manager.h"
#include "input/input_manager.h"
#include "fonts/font_manager.h"
#include "utils/microtimer.h"
#include "game_states/game_state_manager.h"

//-------------------------
CApplicationBase::CApplicationBase( ) 
: m_elapsed( 1.f / 60.f )
, m_quit( false )
{ }

//-------------------------
bool CApplicationBase::init( const char *app_title, unsigned x_res, unsigned y_res ) {

  srand( static_cast<unsigned int>( time( nullptr ) ) );

  bool is_ok = CRenderer::get( ).init( app_title, x_res, y_res );
  if( !is_ok ) {
    printf( "CApplicationBase::CRenderer initialization FAILURE!\n" );
    return false;
  }

  is_ok = CAudioManager::get( ).init( );
  if( !is_ok ) {
    printf( "CApplicationBase::CAudioManager initialization FAILURE!\n" );
    return false;
  } 

  is_ok = CFontManager::get( ).init( );
  if( !is_ok ) {
    printf( "CApplicationBase::CFontManager initialization FAILURE!\n" );
    return false;
  }

  printf( "CApplicationBase::Loading game vars\n" );
  loadGameVars( );

  printf( "CApplicationBase::Loading specific project game states\n" );
  registerGameStates( );

  printf( "CApplicationBase::Loading specific project content\n" );
  return initProject( );
}

//-------------------------
void CApplicationBase::update( ) {

  //Event handler
  SDL_Event e;

  //Set text color as black
  SDL_Color textColor = { 0, 0, 0, 255 };

  //While application is running
  while( !m_quit ) {

    CMicroTimer timer;
    timer.start( );

    CInputManager::get( ).update( );

    //Handle events on queue
    while( SDL_PollEvent( &e ) != 0 ) {
      //User requests quit
      if( e.type == SDL_QUIT ) {
        m_quit = true;
      }

      CInputManager::get( ).updateMouse( e );
    }

    CGameStateManager::get( ).update( m_elapsed );

    CRenderer::get( ).beginRender( );
    CGameStateManager::get( ).render( );
    CFontManager::get( ).render( );
    CRenderer::get( ).endRender( );

    m_elapsed = timer.elapsed( );
  }
}

//-------------------------
void CApplicationBase::destroy( ) {

  // Project specific destruction
  destroyProject( );

  CInputManager::get( ).destroy( );
  CFontManager::get( ).destroy( );
  CRenderer::get( ).destroy( );

}

//-------------------------
void CApplicationBase::exitApplication( ) {
  m_quit = true;
}
