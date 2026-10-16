#pragma once
#include "tut-greeter.h"

/* Public implementation module interface */
typedef GTypeModule * (*tut_module_greeter_fn) (void);
typedef TutGreeter * (*tut_module_greeter_fabricate_fn) (void);
