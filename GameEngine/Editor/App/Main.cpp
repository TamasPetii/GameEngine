#include <App/App.h>

int main()
{
	try 
	{
		App::Instance()->Run();
		App::Destroy();
	}
	catch (const std::exception& error)
	{
		std::cerr << error.what() << std::endl;
		exit(1);
	}
}