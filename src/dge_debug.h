#ifndef DGE_DEBUG_H
#define DGE_DEBUG_H

#include <stdio.h>

#define debug(MSG, ...) fprintf(stderr, "(%s:%d) "MSG"\n", __FILE__, __LINE__, __VA_ARGS__)

// NOTE: We need the else-block here so that if somebody says "if(...) assert(0) else ..."
//       then it doesn't match the else with the wrong if (namely the one inside the assert)
// TODO: The fact that we use this like a function call and put a semicolon at the end causes unbracketed if-blocks to break (because you say if(...) assert(0); else and then the else complains about not havin a corresponding if
//#define assert(condition, MSG, ...) if(!(condition)) { debug("Assert Failed ("#condition"): "MSG, __VA_ARGS__); *(int*)0 = 0; } else {}

#endif
