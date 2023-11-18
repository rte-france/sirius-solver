#include "bb_fonctions.h"


int bbsetcbmessage(BB *Bb, callback_function the_callback_function, void *caller, SIRIUS_LOGLEVEL log_level)
{
	Bb->callback = the_callback_function;
	Bb->caller = caller;
}