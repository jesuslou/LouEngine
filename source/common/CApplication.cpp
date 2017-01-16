#include <CApplication.h>

#include <stdio.h>


#include <SFML/Graphics/View.hpp>
//#include <entityx/entityx.h>

//namespace ex = entityx;
//
//class Application : public ex::EntityX
//{
//public:
//};

//-------------------------
CApplicationBase::CApplicationBase( ) 
: m_elapsed( 1.f / 60.f )
, m_quit( false )
{ 
	sf::View View;
	View.setViewport(sf::FloatRect(0, (float)10, (float)10, 0));
}

//-------------------------
bool CApplicationBase::init( const char *app_title, unsigned x_res, unsigned y_res ) {

  return initProject( );
}

//-------------------------
void CApplicationBase::update( ) {

}

//-------------------------
void CApplicationBase::destroy( ) {

  // Project specific destruction
  destroyProject( );

}

//-------------------------
void CApplicationBase::exitApplication( ) {
  m_quit = true;
}
