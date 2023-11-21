#include "pne_fonctions.h"
#include "mps_define.h"

int PNEsetcbmessage(PROBLEME_PNE *problem_pne, callback_function the_callback_function, void *something_from_the_caller, SIRIUS_LOGLEVEL log_level)
{
  problem_pne->callback = the_callback_function;
  problem_pne->something_from_the_caller = something_from_the_caller;
  // TODO
  return 0;
}

/*int PROBLEME_MPSsetcbmessage(PROBLEME_MPS *problem_mps, callback_function the_callback_function, void *something_from_the_caller, SIRIUS_LOGLEVEL log_level)
{
  problem_mps->callback = the_callback_function;
  problem_mps->something_from_the_caller = something_from_the_caller;
  // TODO
  return 0;
}*/

int PROBLEME_A_RESOUDREsetcbmessage(PROBLEME_A_RESOUDRE *probleme_a_resoudre, callback_function the_callback_function, void *something_from_the_caller, SIRIUS_LOGLEVEL log_level)
{
  probleme_a_resoudre->callback = the_callback_function;
  probleme_a_resoudre->something_from_the_caller = something_from_the_caller;
  // TODO
  return 0;
}
