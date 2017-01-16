#ifndef _INC_SDL_APPLICATION_H_
#define _INC_SDL_APPLICATION_H_


class CApplicationBase {
public:
  CApplicationBase( );
  bool init( const char *app_title, unsigned x_res, unsigned y_res );
  void update( );
  void destroy( );

  void exitApplication( );

protected:
  virtual bool initProject( ) { return true; }
  virtual void loadGameVars( ) { }
  virtual void registerGameStates( ) { }
  virtual void destroyProject( ) { }
  
  float m_elapsed;

  bool m_quit;
};

#endif