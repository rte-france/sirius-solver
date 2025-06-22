
#include "spx_fonctions.h"

callback_function SPXgetcbmessage(PROBLEME_SPX *problem_spx){
  callback_function call_back = NULL;
  call_back = (callback_function)problem_spx->callback;
// TODO else default
if (!call_back)
{
  call_back = SiriusDefaultCallback;
}
return call_back;
}
