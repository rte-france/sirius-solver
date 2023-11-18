#ifdef __cplusplus
extern "C"
{
#endif
#pragma once
    typedef enum SIRIUS_LOGLEVEL
    {
        SIRIUS_TRACE,
        SIRIUS_DEBUG,
        SIRIUS_INFO,
        SIRIUS_WARN,
        SIRIUS_ERROR,
        SIRIUS_FATAL
    } SIRIUS_LOGLEVEL;
    typedef int (*callback_function)(void *caller, const char *sMsg, int nLen, SIRIUS_LOGLEVEL log_level);
  
int Siriussetcbmessage(void *problem, callback_function the_callback_function, void *caller, SIRIUS_LOGLEVEL log_level);
#ifdef __cplusplus
}
#endif