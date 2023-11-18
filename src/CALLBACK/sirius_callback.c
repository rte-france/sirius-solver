#include "sirius_callback.h"


int Siriussetcbmessage(void *problem, callback_function the_callback_function, void *caller, SIRIUS_LOGLEVEL log_level)
{
  problem_spx->callback = the_callback_function;
  problem_spx->caller = caller;
  // TODO
  return 0;
}