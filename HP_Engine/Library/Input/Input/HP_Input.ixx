module;
#ifdef HP_INPUT_EXPORTS
#define HP_INPUT_API __declspec(dllexport)
#else
#define HP_INPUT_API __declspec(dllimport)
#endif
export module HP_Input;


extern "C" HP_INPUT_API int x;
extern "C" HP_INPUT_API void myFunction();