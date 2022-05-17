#include <stdlib.h>
#include <string.h>
#include "curl/curl.h"
#include "osc_sdk.h"

#define AK_SIZE 20
#define SK_SIZE 40

/* We don't use _Bool as we try to be C89 compatible */
int osc_str_append_bool(struct osc_str *osc_str, int bool)
{
	int len = osc_str->len;

	osc_str->len = len + (bool ? 4 : 5);
	osc_str->buf = realloc(osc_str->buf, osc_str->len + 1);
	if (!osc_str->buf)
		return -1;
	strcpy(osc_str->buf + len, (bool ? "true" : "false"));
	return 0;
}

int osc_str_append_int(struct osc_str *osc_str, int i)
{
	int len = osc_str->len;

	osc_str->buf = realloc(osc_str->buf, len + 64);
	if (!osc_str->buf)
		return -1;
	osc_str->len = len + snprintf(osc_str->buf + len, 64, "%d", i);
	osc_str->buf[osc_str->len] = 0;
	return 0;
}

int osc_str_append_string(struct osc_str *osc_str, const char *str)
{
	if (!str)
		return 0;

	int len = osc_str->len;
	int dlen = strlen(str);

	osc_str->len = osc_str->len + dlen;
	osc_str->buf = realloc(osc_str->buf, osc_str->len + 1);
	if (!osc_str->buf)
		return -1;
	memcpy(osc_str->buf + len, str, dlen + 1);
	return 0;
}

/* Function that will write the data inside a variable */
static size_t write_data(void *data, size_t size, size_t nmemb, void *userp)
{
	size_t bufsize = size * nmemb;
	struct osc_str *response = userp;
	int olen = response->len;

	response->len = response->len + bufsize;
	response->buf = realloc(response->buf, response->len + 1);
	memcpy(response->buf + olen, data, bufsize);
	response->buf[response->len] = 0;
	return bufsize;
}

void osc_init_str(struct osc_str *r)
{
	r->len = 0;
	r->buf = NULL;
}

void osc_deinit_str(struct osc_str *r)
{
	free(r->buf);
	osc_init_str(r);
}

____func_code____

int osc_init_sdk(struct osc_env *e)
{
	char ak_sk[AK_SIZE + SK_SIZE + 2];

	e->ak = getenv("OSC_ACCESS_KEY");
	e->sk = getenv("OSC_SECRET_KEY");

	if (strlen(e->ak) != AK_SIZE || strlen(e->sk) != SK_SIZE) {
		fprintf(stderr, "Wrong size OSC_ACCESS_KEY or OSC_SECRET_KEY\n");
		return(1);
	}

	e->headers = NULL;
	stpcpy(stpcpy(stpcpy(ak_sk, e->ak), ":"), e->sk);
	e->c = curl_easy_init();
	e->headers = curl_slist_append(e->headers, "Content-Type: application/json");

	/* Setting HEADERS */
	curl_easy_setopt(e->c, CURLOPT_HTTPHEADER, e->headers);
	curl_easy_setopt(e->c, CURLOPT_WRITEFUNCTION, write_data);

	/* For authentification we specify the method and our acces key / secret key */
	curl_easy_setopt(e->c, CURLOPT_AWS_SIGV4, "osc");
	curl_easy_setopt(e->c, CURLOPT_USERPWD, ak_sk);

	return 0;
}

void osc_deinit_sdk(struct osc_env *e)
{
	curl_slist_free_all(e->headers);
	curl_easy_cleanup(e->c);
	e->c = NULL;
}