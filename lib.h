#ifndef __SDK_C__
#define __SDK_C__

#include <curl/curl.h>

struct osc_env {
	char *ak;
	char *sk;
	struct curl_slist *headers;
	CURL *c;
};

struct osc_str {
	int len;
	char *buf;
};

#define OSC_VERBOSE_MODE 1

struct osc_arg {
	____args____
};

void osc_init_str(struct osc_str *r);
void osc_deinit_str(struct osc_str *r);
int osc_init_sdk(struct osc_env *e, unsigned int flag);
void osc_deinit_sdk(struct osc_env *e);

____functions_proto____

#endif /* __SDK_C__ */
