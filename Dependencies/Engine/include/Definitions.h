#pragma once

#ifdef AGPT_EXPORTS
#define AGPT_API __declspec(dllexport)
#else
#define AGPT_API __declspec(dllimport)
#endif
