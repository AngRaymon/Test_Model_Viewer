#include "Renderer_GL.h"
#include <stdio.h>
#include <string>
#include "ColladaLoader.h"
#include "Model.h"

bool init();
void handleKeys( unsigned char key, int x, int y );
//Per frame update
void update();
//Renders quad to the screen
void render();
//Frees media and shuts down SDL
void close();
//Render flag
bool gRenderTriangles = true;
Renderer_GL* s_renderer;
NModel* s_model;
bool init()
{
	s_renderer = Renderer_GL::GetInstance();
	return s_renderer->Init();
}

void handleKeys( unsigned char key, int x, int y )
{
	//Toggle quad
	if( key == 'q' )
	{
		gRenderTriangles = !gRenderTriangles;
	}
}

void update()
{
}

void render()
{
	s_renderer->DrawModel(s_model, !gRenderTriangles);
}

void close()
{

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;
		
		//Enable text input
		SDL_StartTextInput();

		s_model = new NModel();
		ColladaLoader loader;
		loader.LoadData("duck_tri.dae", 3);
		s_model->LoadData(loader.GetVerts(), loader.GetIndices());

		//While application is running
		while( !quit )
		{
			//Handle events on queue
			while( SDL_PollEvent( &e ) != 0 )
			{
				//User requests quit
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
				//Handle keypress with current mouse position
				else if( e.type == SDL_TEXTINPUT )
				{
					int x = 0, y = 0;
					SDL_GetMouseState( &x, &y );
					handleKeys( e.text.text[ 0 ], x, y );
				}
			}
			s_renderer->StartRenderFrame(0);
			//Render quad
			render();
			s_renderer->EndRenderFrame(0);
		}
		
		//Disable text input
		SDL_StopTextInput();
	}

	//Free resources and close SDL
	close();

	return 0;
}