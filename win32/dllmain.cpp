// dllmain.cpp : Defines the entry point for the DLL application.

#include "../factory.h"

using namespace config;
struct factory_starter {
    factory_starter() {
        factory::start();
    }
    ~factory_starter() {
        factory::stop();
    }
};
namespace {
    factory_starter factory_starter_value;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 ) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

