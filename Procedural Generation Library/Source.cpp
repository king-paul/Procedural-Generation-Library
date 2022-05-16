#include <iostream>
#include <wtypes.h>
#include <comutil.h>

extern "C" __declspec(dllexport) int AddNumbers(int a, int b);

int AddNumbers(int a, int b) {
	return a + b;
}