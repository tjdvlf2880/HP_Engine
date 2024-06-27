module;
#ifdef HP_INPUT_EXPORTS
#define HP_INPUT_API __declspec(dllexport)
#else
#define HP_INPUT_API __declspec(dllimport)
#endif
module HP_Input;


int x = 3;

HP_INPUT_API void myFunction()
{
	
}