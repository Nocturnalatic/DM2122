
#include <time.h>
#include "Application.h"

int main( void )
{
	srand(time(NULL));
	Application app;
	app.Init();
	app.Run();
	app.Exit();
}