#include "App/App.h"

int main()
{
	try 
	{
		App* application = new App();
		application->Init();
		application->Run();
		application->Clear();
	}
	catch (const std::runtime_error& error)
	{
		std::cerr << error.what() << std::endl;
	}

	return 0;
}