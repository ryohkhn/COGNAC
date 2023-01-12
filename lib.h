/**
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, Outscale SAS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

 /*
  * This code is autogenerated, don't edit it directely
  */

#ifndef __SDK_C__
#define __SDK_C__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <curl/curl.h>

#ifdef __GNUC__
/*
 * note that thoses attribute work, on the struct, not the pointer
 * so, use it with "auto_osc_env struct osc_env e;"
 * note the absence of '*' before e; (and same with osc_str)
 */
#define auto_osc_str __attribute__((cleanup(osc_deinit_str)))
#define auto_osc_env __attribute__((cleanup(osc_deinit_sdk)))
#endif

struct osc_str {
	int len;
	char *buf;
};

#define OSC_ENV_FREE_AK_SK 1
#define OSC_ENV_FREE_REGION 2
#define OSC_VERBOSE_MODE 4
#define OSC_INSECURE_MODE 8
#define OSC_ENV_FREE_CERT 16
#define OSC_ENV_FREE_SSLKEY 32

#define OSC_API_VERSION "____api_version____"
#define OSC_SDK_VERSION ____sdk_version____

struct osc_env {
	char *ak;
	char *sk;
	char *region;
	char *cert;
	char *sslkey;
	int flag;
	struct curl_slist *headers;
	struct osc_str endpoint;
	CURL *c;
};

#define OSC_SDK_VERSON_L (sizeof "00.11.22" - 1)

static const char *osc_sdk_version_str(void)
{
	static char ret[OSC_SDK_VERSON_L];

	if (OSC_SDK_VERSION == 0xC061AC)
		return "unstable";
	ret[1] = (OSC_SDK_VERSION & 0x00000F) + '0';
	ret[0] = ((OSC_SDK_VERSION & 0x0000F0) >> 4) + '0';
	ret[2] = '.';
	ret[4] = ((OSC_SDK_VERSION & 0x000F00) >> 8) + '0';
	ret[3] = ((OSC_SDK_VERSION & 0x00F000) >> 12) + '0';
	ret[5] = '.';
	ret[7] = ((OSC_SDK_VERSION & 0x0F0000) >> 16) + '0';
	ret[6] = ((OSC_SDK_VERSION & 0xF00000) >> 20) + '0';
	return ret;
}

____args____

int osc_load_ak_sk_from_conf(const char *profile, char **ak, char **sk);
int osc_load_region_from_conf(const char *profile, char **region);

/**
 * @brief parse osc config file, and store cred/key.
 *
 * @return if < 0, an error, otherwise a flag contain OSC_ENV_FREE_CERT,
 *	OSC_ENV_FREE_SSLKEY, both or 0
 */
int osc_load_cert_from_conf(const char *profile, char **cert, char **key);

void osc_init_str(struct osc_str *r);
void osc_deinit_str(struct osc_str *r);
int osc_init_sdk(struct osc_env *e, const char *profile, unsigned int flag);
void osc_deinit_sdk(struct osc_env *e);

/*
 * osc_new_sdk/str and osc_destroy_sdk/str where made so we can use
 * C++'s std::unique_ptr with the lib.
 * use it like
 * const std::unique_ptr<osc_env, decltype(&osc_destroy_sdk)>
 *	e(osc_new_sdk(NULL, 0), &osc_destroy_sdk);
 */
static struct osc_env *osc_new_sdk(const char *profile, unsigned int flag)
{
	struct osc_env *e = (struct osc_env *)malloc(sizeof *e);

	if (osc_init_sdk(e, profile, flag) < 0) {
		free(e);
		return NULL;
	}
	return e;
}

static void osc_destroy_sdk(struct osc_env *e)
{
	osc_deinit_sdk(e);
	free(e);
}

static struct osc_str *osc_new_str(void)
{
	struct osc_str *e = (struct osc_str *)malloc(sizeof *e);

	osc_init_str(e);
	return e;
}

static void osc_destroy_str(struct osc_str *e)
{
	osc_deinit_str(e);
	free(e);
}

int osc_sdk_set_useragent(struct osc_env *e, const char *str);

#ifdef WITH_DESCRIPTION

const char *osc_find_description(const char *call_name);
const char *osc_find_args_description(const char *call_name);

#endif /* WITH_DESCRIPTION */

____functions_proto____

#ifdef __cplusplus
}
#endif

#endif /* __SDK_C__ */
