
#include "sirius_callback.h"
#include <stdio.h>

int SiriusDefaultCallback(void *something_from_the_caller, const char *sMsg, int nLen, SIRIUS_LOGLEVEL log_level)
{
	if (nLen <= 0)
	{
		return 0;
	}
	if (log_level == SIRIUS_ERROR)
	{
		fprintf(stderr, sMsg);
	}
	else
	{
		printf("%s", sMsg);
	}
}