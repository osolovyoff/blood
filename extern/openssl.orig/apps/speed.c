/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 *
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 *
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 *
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */
/* ====================================================================
 * Copyright 2002 Sun Microsystems, Inc. ALL RIGHTS RESERVED.
 *
 * Portions of the attached software ("Contribution") are developed by
 * SUN MICROSYSTEMS, INC., and are contributed to the OpenSSL project.
 *
 * The Contribution is licensed pursuant to the OpenSSL open source
 * license provided above.
 *
 * The ECDH and ECDSA speed test software is originally written by
 * Sumit Gupta of Sun Microsystems Laboratories.
 *
 */

#undef SECONDS
#define SECONDS                 3
#define PRIME_SECONDS   10
#define RSA_SECONDS             10
#define DSA_SECONDS             10
#define ECDSA_SECONDS   10
#define ECDH_SECONDS    10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "apps.h"
#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/objects.h>
#if !defined(OPENSSL_SYS_MSDOS)
# include OPENSSL_UNISTD
#endif

#ifndef OPENSSL_SYS_NETWARE
# include <signal.h>
#endif

#if defined(_WIN32) || defined(__CYGWIN__)
# include <windows.h>
# if defined(__CYGWIN__) && !defined(_WIN32)
  /*
   * <windows.h> should define _WIN32, which normally is mutually exclusive
   * with __CYGWIN__, but if it didn't...
   */
#  define _WIN32
  /* this is done because Cygwin alarm() fails sometimes. */
# endif
#endif

#include <openssl/bn.h>
#ifndef OPENSSL_NO_DES
# include <openssl/des.h>
#endif
#ifndef OPENSSL_NO_AES
# include <openssl/aes.h>
#endif
#ifndef OPENSSL_NO_CAMELLIA
# include <openssl/camellia.h>
#endif
#ifndef OPENSSL_NO_MD2
# include <openssl/md2.h>
#endif
#ifndef OPENSSL_NO_MDC2
# include <openssl/mdc2.h>
#endif
#ifndef OPENSSL_NO_MD4
# include <openssl/md4.h>
#endif
#ifndef OPENSSL_NO_MD5
# include <openssl/md5.h>
#endif
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#ifndef OPENSSL_NO_RMD160
# include <openssl/ripemd.h>
#endif
#ifndef OPENSSL_NO_WHIRLPOOL
# include <openssl/whrlpool.h>
#endif
#ifndef OPENSSL_NO_RC4
# include <openssl/rc4.h>
#endif
#ifndef OPENSSL_NO_RC5
# include <openssl/rc5.h>
#endif
#ifndef OPENSSL_NO_RC2
# include <openssl/rc2.h>
#endif
#ifndef OPENSSL_NO_IDEA
# include <openssl/idea.h>
#endif
#ifndef OPENSSL_NO_SEED
# include <openssl/seed.h>
#endif
#ifndef OPENSSL_NO_BF
# include <openssl/blowfish.h>
#endif
#ifndef OPENSSL_NO_CAST
# include <openssl/cast.h>
#endif
#ifndef OPENSSL_NO_RSA
# include <openssl/rsa.h>
# include "./testrsa.h"
#endif
#include <openssl/x509.h>
#ifndef OPENSSL_NO_DSA
# include <openssl/dsa.h>
# include "./testdsa.h"
#endif
#ifndef OPENSSL_NO_EC
# include <openssl/ecdsa.h>
# include <openssl/ecdh.h>
#endif
#include <openssl/modes.h>

#include <openssl/bn.h>

#ifndef HAVE_FORK
# if defined(OPENSSL_SYS_VMS) || defined(OPENSSL_SYS_WINDOWS) || defined(OPENSSL_SYS_OS2) || defined(OPENSSL_SYS_NETWARE)
#  define HAVE_FORK 0
# else
#  define HAVE_FORK 1
# endif
#endif

#if HAVE_FORK
# undef NO_FORK
#else
# define NO_FORK
#endif

#undef BUFSIZE
#define BUFSIZE (1024*8+1)
#define MAX_MISALIGNMENT 63

static volatile int run = 0;

static int mr = 0;
static int usertime = 1;

static double Time_F(int s);
static void print_message(const char *s, long num, int length);
static void pkey_print_message(const char *str, const char *str2,
                               long num, int bits, int sec);
static void print_result(int alg, int run_no, int count, double time_used);
#ifndef NO_FORK
static int do_multi(int multi);
#endif

#define ALGOR_NUM       30
#define SIZE_NUM        5
#define PRIME_NUM       3
#define RSA_NUM         7
#define DSA_NUM         3

#define EC_NUM       16
#define MAX_ECDH_SIZE 256
#define MISALIGN        64

static const char *names[ALGOR_NUM] = {
    "md2", "mdc2", "md4", "md5", "hmac(md5)", "sha1", "rmd160", "rc4",
    "des cbc", "des ede3", "idea cbc", "seed cbc",
    "rc2 cbc", "rc5-32/12 cbc", "blowfish cbc", "cast cbc",
    "aes-128 cbc", "aes-192 cbc", "aes-256 cbc",
    "camellia-128 cbc", "camellia-192 cbc", "camellia-256 cbc",
    "evp", "sha256", "sha512", "whirlpool",
    "aes-128 ige", "aes-192 ige", "aes-256 ige", "ghash"
};

static double results[ALGOR_NUM][SIZE_NUM];
static int lengths[SIZE_NUM] = {
    16, 64, 256, 1024, 8 * 1024
};

#ifndef OPENSSL_NO_RSA
static double rsa_results[RSA_NUM][2];
#endif
#ifndef OPENSSL_NO_DSA
static double dsa_results[DSA_NUM][2];
#endif
#ifndef OPENSSL_NO_EC
static double ecdsa_results[EC_NUM][2];
static double ecdh_results[EC_NUM][1];
#endif

#if defined(OPENSSL_NO_DSA) && !defined(OPENSSL_NO_EC)
static const char rnd_seed[] =
    "string to make the random number generator think it has entropy";
static int rnd_fake = 0;
#endif

#ifdef SIGALRM
# if defined(__STDC__) || defined(sgi) || defined(_AIX)
#  define SIGRETTYPE void
# else
#  define SIGRETTYPE int
# endif

static SIGRETTYPE sig_done(int sig);
static SIGRETTYPE sig_done(int sig)
{
    signal(SIGALRM, sig_done);
    run = 0;
}
#endif

#define START   0
#define STOP    1

#if defined(_WIN32)

# if !defined(SIGALRM)
#  define SIGALRM
# endif
static unsigned int lapse, schlock;
static void alarm_win32(unsigned int secs)
{
    lapse = secs * 1000;
}

# define alarm alarm_win32

static DWORD WINAPI sleepy(VOID * arg)
{
    schlock = 1;
    Sleep(lapse);
    run = 0;
    return 0;
}

static double Time_F(int s)
{
    double ret;
    static HANDLE thr;

    if (s == START) {
        schlock = 0;
        thr = CreateThread(NULL, 4096, sleepy, NULL, 0, NULL);
        if (thr == NULL) {
            DWORD ret = GetLastError();
            BIO_printf(bio_err, "unable to CreateThread (%d)", ret);
            ExitProcess(ret);
        }
        while (!schlock)
            Sleep(0);           /* scheduler spinlock */
        ret = app_tminterval(s, usertime);
    } else {
        ret = app_tminterval(s, usertime);
        if (run)
            TerminateThread(thr, 0);
        CloseHandle(thr);
    }

    return ret;
}
#else

static double Time_F(int s)
{
    double ret = app_tminterval(s, usertime);
    if (s == STOP)
        alarm(0);
    return ret;
}
#endif

#ifndef OPENSSL_NO_EC
static const int KDF1_SHA1_len = 20;
static void *KDF1_SHA1(const void *in, size_t inlen, void *out,
                       size_t *outlen)
{
    if (*outlen < SHA_DIGEST_LENGTH)
        return NULL;
    *outlen = SHA_DIGEST_LENGTH;
    return SHA1(in, inlen, out);
}
#endif                         /* OPENSSL_NO_EC */

static void multiblock_speed(const EVP_CIPHER *evp_cipher);

static int found(const char *name, const OPT_PAIR * pairs, int *result)
{
    for (; pairs->name; pairs++)
        if (strcmp(name, pairs->name) == 0) {
            *result = pairs->retval;
            return 1;
        }
    return 0;
}

typedef enum OPTION_choice {
    OPT_ERR = -1, OPT_EOF = 0, OPT_HELP,
    OPT_ELAPSED, OPT_EVP, OPT_DECRYPT, OPT_ENGINE, OPT_MULTI,
    OPT_MR, OPT_MB, OPT_MISALIGN
} OPTION_CHOICE;

OPTIONS speed_options[] = {
    {OPT_HELP_STR, 1, '-', "Usage: %s [options] ciphers...\n"},
    {OPT_HELP_STR, 1, '-', "Valid options are:\n"},
    {"help", OPT_HELP, '-', "Display this summary"},
    {"evp", OPT_EVP, 's', "Use specified EVP cipher"},
    {"decrypt", OPT_DECRYPT, '-',
     "Time decryption instead of encryption (only EVP)"},
    {"mr", OPT_MR, '-', "Produce machine readable output"},
    {"mb", OPT_MB, '-'},
    {"misalign", OPT_MISALIGN, 'n', "Amount to mis-align buffers"},
    {"elapsed", OPT_ELAPSED, '-',
     "Measure time in real time instead of CPU user time"},
#ifndef NO_FORK
    {"multi", OPT_MULTI, 'p', "Run benchmarks in parallel"},
#endif
#ifndef OPENSSL_NO_ENGINE
    {"engine", OPT_ENGINE, 's', "Use engine, possibly a hardware device"},
#endif
    {NULL},
};

#define D_MD2           0
#define D_MDC2          1
#define D_MD4           2
#define D_MD5           3
#define D_HMAC          4
#define D_SHA1          5
#define D_RMD160        6
#define D_RC4           7
#define D_CBC_DES       8
#define D_EDE3_DES      9
#define D_CBC_IDEA      10
#define D_CBC_SEED      11
#define D_CBC_RC2       12
#define D_CBC_RC5       13
#define D_CBC_BF        14
#define D_CBC_CAST      15
#define D_CBC_128_AES   16
#define D_CBC_192_AES   17
#define D_CBC_256_AES   18
#define D_CBC_128_CML   19
#define D_CBC_192_CML   20
#define D_CBC_256_CML   21
#define D_EVP           22
#define D_SHA256        23
#define D_SHA512        24
#define D_WHIRLPOOL     25
#define D_IGE_128_AES   26
#define D_IGE_192_AES   27
#define D_IGE_256_AES   28
#define D_GHASH         29
OPT_PAIR doit_choices[] = {
#ifndef OPENSSL_NO_MD2
    {"md2", D_MD2},
#endif
#ifndef OPENSSL_NO_MDC2
    {"mdc2", D_MDC2},
#endif
#ifndef OPENSSL_NO_MD4
    {"md4", D_MD4},
#endif
#ifndef OPENSSL_NO_MD5
    {"md5", D_MD5},
#endif
#ifndef OPENSSL_NO_MD5
    {"hmac", D_HMAC},
#endif
    {"sha1", D_SHA1},
    {"sha256", D_SHA256},
    {"sha512", D_SHA512},
#ifndef OPENSSL_NO_WHIRLPOOL
    {"whirlpool", D_WHIRLPOOL},
#endif
#ifndef OPENSSL_NO_RMD160
    {"ripemd", D_RMD160},
    {"rmd160", D_RMD160},
    {"ripemd160", D_RMD160},
#endif
#ifndef OPENSSL_NO_RC4
    {"rc4", D_RC4},
#endif
#ifndef OPENSSL_NO_DES
    {"des-cbc", D_CBC_DES},
    {"des-ede3", D_EDE3_DES},
#endif
#ifndef OPENSSL_NO_AES
    {"aes-128-cbc", D_CBC_128_AES},
    {"aes-192-cbc", D_CBC_192_AES},
    {"aes-256-cbc", D_CBC_256_AES},
    {"aes-128-ige", D_IGE_128_AES},
    {"aes-192-ige", D_IGE_192_AES},
    {"aes-256-ige", D_IGE_256_AES},
#endif
#ifndef OPENSSL_NO_RC2
    {"rc2-cbc", D_CBC_RC2},
    {"rc2", D_CBC_RC2},
#endif
#ifndef OPENSSL_NO_RC5
    {"rc5-cbc", D_CBC_RC5},
    {"rc5", D_CBC_RC5},
#endif
#ifndef OPENSSL_NO_IDEA
    {"idea-cbc", D_CBC_IDEA},
    {"idea", D_CBC_IDEA},
#endif
#ifndef OPENSSL_NO_SEED
    {"seed-cbc", D_CBC_SEED},
    {"seed", D_CBC_SEED},
#endif
#ifndef OPENSSL_NO_BF
    {"bf-cbc", D_CBC_BF},
    {"blowfish", D_CBC_BF},
    {"bf", D_CBC_BF},
#endif
#ifndef OPENSSL_NO_CAST
    {"cast-cbc", D_CBC_CAST},
    {"cast", D_CBC_CAST},
    {"cast5", D_CBC_CAST},
#endif
    {"ghash", D_GHASH},
    {NULL}
};

#define R_DSA_512       0
#define R_DSA_1024      1
#define R_DSA_2048      2
static OPT_PAIR dsa_choices[] = {
    {"dsa512", R_DSA_512},
    {"dsa1024", R_DSA_1024},
    {"dsa2048", R_DSA_2048},
    {NULL},
};

#define R_RSA_512       0
#define R_RSA_1024      1
#define R_RSA_2048      2
#define R_RSA_3072      3
#define R_RSA_4096      4
#define R_RSA_7680      5
#define R_RSA_15360     6
static OPT_PAIR rsa_choices[] = {
    {"rsa512", R_RSA_512},
    {"rsa1024", R_RSA_1024},
    {"rsa2048", R_RSA_2048},
    {"rsa3072", R_RSA_3072},
    {"rsa4096", R_RSA_4096},
    {"rsa7680", R_RSA_7680},
    {"rsa15360", R_RSA_15360},
    {NULL}
};

#define R_EC_P160    0
#define R_EC_P192    1
#define R_EC_P224    2
#define R_EC_P256    3
#define R_EC_P384    4
#define R_EC_P521    5
#define R_EC_K163    6
#define R_EC_K233    7
#define R_EC_K283    8
#define R_EC_K409    9
#define R_EC_K571    10
#define R_EC_B163    11
#define R_EC_B233    12
#define R_EC_B283    13
#define R_EC_B409    14
#define R_EC_B571    15
#ifndef OPENSSL_NO_ECA
static OPT_PAIR ecdsa_choices[] = {
    {"ecdsap160", R_EC_P160},
    {"ecdsap192", R_EC_P192},
    {"ecdsap224", R_EC_P224},
    {"ecdsap256", R_EC_P256},
    {"ecdsap384", R_EC_P384},
    {"ecdsap521", R_EC_P521},
    {"ecdsak163", R_EC_K163},
    {"ecdsak233", R_EC_K233},
    {"ecdsak283", R_EC_K283},
    {"ecdsak409", R_EC_K409},
    {"ecdsak571", R_EC_K571},
    {"ecdsab163", R_EC_B163},
    {"ecdsab233", R_EC_B233},
    {"ecdsab283", R_EC_B283},
    {"ecdsab409", R_EC_B409},
    {"ecdsab571", R_EC_B571},
    {NULL}
};
static OPT_PAIR ecdh_choices[] = {
    {"ecdhp160", R_EC_P160},
    {"ecdhp192", R_EC_P192},
    {"ecdhp224", R_EC_P224},
    {"ecdhp256", R_EC_P256},
    {"ecdhp384", R_EC_P384},
    {"ecdhp521", R_EC_P521},
    {"ecdhk163", R_EC_K163},
    {"ecdhk233", R_EC_K233},
    {"ecdhk283", R_EC_K283},
    {"ecdhk409", R_EC_K409},
    {"ecdhk571", R_EC_K571},
    {"ecdhb163", R_EC_B163},
    {"ecdhb233", R_EC_B233},
    {"ecdhb283", R_EC_B283},
    {"ecdhb409", R_EC_B409},
    {"ecdhb571", R_EC_B571},
    {NULL}
};
#endif

int speed_main(int argc, char **argv)
{
    char *prog;
    const EVP_CIPHER *evp_cipher = NULL;
    const EVP_MD *evp_md = NULL;
    double d = 0.0;
    OPTION_CHOICE o;
    int decrypt = 0, multiblock = 0, doit[ALGOR_NUM], pr_header = 0;
    int dsa_doit[DSA_NUM], rsa_doit[RSA_NUM];
    int ret = 1, i, j, k, misalign = MAX_MISALIGNMENT + 1;
    long c[ALGOR_NUM][SIZE_NUM], count = 0, save_count = 0;
    unsigned char *buf_malloc = NULL, *buf2_malloc = NULL;
    unsigned char *buf = NULL, *buf2 = NULL;
    unsigned char md[EVP_MAX_MD_SIZE];
#ifndef NO_FORK
    int multi = 0;
#endif
    /* What follows are the buffers and key material. */
#if !defined(OPENSSL_NO_RSA) || !defined(OPENSSL_NO_DSA)
    long rsa_count;
#endif
#ifndef OPENSSL_NO_MD2
    unsigned char md2[MD2_DIGEST_LENGTH];
#endif
#ifndef OPENSSL_NO_MDC2
    unsigned char mdc2[MDC2_DIGEST_LENGTH];
#endif
#ifndef OPENSSL_NO_MD4
    unsigned char md4[MD4_DIGEST_LENGTH];
#endif
#ifndef OPENSSL_NO_MD5
    unsigned char md5[MD5_DIGEST_LENGTH];
    unsigned char hmac[MD5_DIGEST_LENGTH];
#endif
    unsigned char sha[SHA_DIGEST_LENGTH];
    unsigned char sha256[SHA256_DIGEST_LENGTH];
    unsigned char sha512[SHA512_DIGEST_LENGTH];
#ifndef OPENSSL_NO_WHIRLPOOL
    unsigned char whirlpool[WHIRLPOOL_DIGEST_LENGTH];
#endif
#ifndef OPENSSL_NO_RMD160
    unsigned char rmd160[RIPEMD160_DIGEST_LENGTH];
#endif
#ifndef OPENSSL_NO_RC4
    RC4_KEY rc4_ks;
#endif
#ifndef OPENSSL_NO_RC5
    RC5_32_KEY rc5_ks;
#endif
#ifndef OPENSSL_NO_RC2
    RC2_KEY rc2_ks;
#endif
#ifndef OPENSSL_NO_IDEA
    IDEA_KEY_SCHEDULE idea_ks;
#endif
#ifndef OPENSSL_NO_SEED
    SEED_KEY_SCHEDULE seed_ks;
#endif
#ifndef OPENSSL_NO_BF
    BF_KEY bf_ks;
#endif
#ifndef OPENSSL_NO_CAST
    CAST_KEY cast_ks;
#endif
    static const unsigned char key16[16] = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12
    };
#ifndef OPENSSL_NO_AES
    static const unsigned char key24[24] = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12,
        0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34
    };
    static const unsigned char key32[32] = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12,
        0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34,
        0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34, 0x56
    };
#endif
#ifndef OPENSSL_NO_CAMELLIA
    static const unsigned char ckey24[24] = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12,
        0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34
    };
    static const unsigned char ckey32[32] = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0,
        0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12,
        0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34,
        0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34, 0x56
    };
    CAMELLIA_KEY camellia_ks1, camellia_ks2, camellia_ks3;
#endif
#ifndef OPENSSL_NO_AES
# define MAX_BLOCK_SIZE 128
#else
# define MAX_BLOCK_SIZE 64
#endif
    unsigned char DES_iv[8];
    unsigned char iv[2 * MAX_BLOCK_SIZE / 8];
#ifndef OPENSSL_NO_DES
    static DES_cblock key = {
        0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0
    };
    static DES_cblock key2 = {
        0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12
    };
    static DES_cblock key3 = {
        0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0, 0x12, 0x34
    };
    DES_key_schedule sch;
    DES_key_schedule sch2;
    DES_key_schedule sch3;
#endif
#ifndef OPENSSL_NO_AES
    AES_KEY aes_ks1, aes_ks2, aes_ks3;
#endif
#ifndef OPENSSL_NO_RSA
    unsigned rsa_num;
    RSA *rsa_key[RSA_NUM];
    long rsa_c[RSA_NUM][2];
    static unsigned int rsa_bits[RSA_NUM] = {
        512, 1024, 2048, 3072, 4096, 7680, 15360
    };
    static unsigned char *rsa_data[RSA_NUM] = {
        test512, test1024, test2048, test3072, test4096, test7680, test15360
    };
    static int rsa_data_length[RSA_NUM] = {
        sizeof(test512), sizeof(test1024),
        sizeof(test2048), sizeof(test3072),
        sizeof(test4096), sizeof(test7680),
        sizeof(test15360)
    };
#endif
#ifndef OPENSSL_NO_DSA
    DSA *dsa_key[DSA_NUM];
    long dsa_c[DSA_NUM][2];
    static unsigned int dsa_bits[DSA_NUM] = { 512, 1024, 2048 };
#endif
#ifndef OPENSSL_NO_EC
    /*
     * We only test over the following curves as they are representative, To
     * add tests over more curves, simply add the curve NID and curve name to
     * the following arrays and increase the EC_NUM value accordingly.
     */
    static unsigned int test_curves[EC_NUM] = {
        /* Prime Curves */
        NID_secp160r1, NID_X9_62_prime192v1, NID_secp224r1,
        NID_X9_62_prime256v1, NID_secp384r1, NID_secp521r1,
        /* Binary Curves */
        NID_sect163k1, NID_sect233k1, NID_sect283k1,
        NID_sect409k1, NID_sect571k1, NID_sect163r2,
        NID_sect233r1, NID_sect283r1, NID_sect409r1,
        NID_sect571r1
    };
    static const char *test_curves_names[EC_NUM] = {
        /* Prime Curves */
        "secp160r1", "nistp192", "nistp224",
        "nistp256", "nistp384", "nistp521",
        /* Binary Curves */
        "nistk163", "nistk233", "nistk283",
        "nistk409", "nistk571", "nistb163",
        "nistb233", "nistb283", "nistb409",
        "nistb571"
    };
    static int test_curves_bits[EC_NUM] = {
        160, 192, 224,
        256, 384, 521,
        163, 233, 283,
        409, 571, 163,
        233, 283, 409,
        571
    };
#endif
#ifndef OPENSSL_NO_EC
    unsigned char ecdsasig[256];
    unsigned int ecdsasiglen;
    EC_KEY *ecdsa[EC_NUM];
    long ecdsa_c[EC_NUM][2];
    int ecdsa_doit[EC_NUM];
    EC_KEY *ecdh_a[EC_NUM], *ecdh_b[EC_NUM];
    unsigned char secret_a[MAX_ECDH_SIZE], secret_b[MAX_ECDH_SIZE];
    int secret_size_a, secret_size_b;
    int ecdh_checks = 0;
    int secret_idx = 0;
    long ecdh_c[EC_NUM][2];
    int ecdh_doit[EC_NUM];
#endif

    memset(results, 0, sizeof(results));
#ifndef OPENSSL_NO_DSA
    memset(dsa_key, 0, sizeof(dsa_key));
#endif
#ifndef OPENSSL_NO_EC
    for (i = 0; i < EC_NUM; i++)
        ecdsa[i] = NULL;
    for (i = 0; i < EC_NUM; i++)
        ecdh_a[i] = ecdh_b[i] = NULL;
#endif
#ifndef OPENSSL_NO_RSA
    memset(rsa_key, 0, sizeof(rsa_key));
    for (i = 0; i < RSA_NUM; i++)
        rsa_key[i] = NULL;
#endif

    memset(c, 0, sizeof(c));
    memset(DES_iv, 0, sizeof(DES_iv));
    memset(iv, 0, sizeof(iv));

    for (i = 0; i < ALGOR_NUM; i++)
        doit[i] = 0;
    for (i = 0; i < RSA_NUM; i++)
        rsa_doit[i] = 0;
    for (i = 0; i < DSA_NUM; i++)
        dsa_doit[i] = 0;
#ifndef OPENSSL_NO_EC
    for (i = 0; i < EC_NUM; i++)
        ecdsa_doit[i] = 0;
    for (i = 0; i < EC_NUM; i++)
        ecdh_doit[i] = 0;
#endif

    buf = buf_malloc = app_malloc((int)BUFSIZE + misalign, "input buffer");
    buf2 = buf2_malloc = app_malloc((int)BUFSIZE + misalign, "output buffer");
    misalign = 0;

    prog = opt_init(argc, argv, speed_options);
    while ((o = opt_next()) != OPT_EOF) {
        switch (o) {
        case OPT_EOF:
        case OPT_ERR:
 opterr:
            BIO_printf(bio_err, "%s: Use -help for summary.\n", prog);
            goto end;
        case OPT_HELP:
            opt_help(speed_options);
            ret = 0;
            goto end;
        case OPT_ELAPSED:
            usertime = 0;
            break;
        case OPT_EVP:
            evp_cipher = EVP_get_cipherbyname(opt_arg());
            if (evp_cipher == NULL)
                evp_md = EVP_get_digestbyname(opt_arg());
            if (evp_cipher == NULL && evp_md == NULL) {
                BIO_printf(bio_err,
                           "%s: %s  an unknown cipher or digest\n",
                           prog, opt_arg());
                goto end;
            }
            doit[D_EVP] = 1;
            break;
        case OPT_DECRYPT:
            decrypt = 1;
            break;
        case OPT_ENGINE:
            (void)setup_engine(opt_arg(), 0);
            break;
        case OPT_MULTI:
#ifndef NO_FORK
            multi = atoi(opt_arg());
#endif
            break;
        case OPT_MISALIGN:
            if (!opt_int(opt_arg(), &misalign))
                goto end;
            if (misalign > MISALIGN) {
                BIO_printf(bio_err,
                           "%s: Maximum offset is %d\n", prog, MISALIGN);
                goto opterr;
            }
            buf = buf_malloc + misalign;
            buf2 = buf2_malloc + misalign;
            break;
        case OPT_MR:
            mr = 1;
            break;
        case OPT_MB:
            multiblock = 1;
            break;
        }
    }
    argc = opt_num_rest();
    argv = opt_rest();

    if (!app_load_modules(NULL))
        goto end;

    /* Remaining arguments are algorithms. */
    for ( ; *argv; argv++) {
        if (found(*argv, doit_choices, &i)) {
            doit[i] = 1;
            continue;
        }
#ifndef OPENSSL_NO_DES
        if (strcmp(*argv, "des") == 0) {
            doit[D_CBC_DES] = doit[D_EDE3_DES] = 1;
            continue;
        }
#endif
        if (strcmp(*argv, "sha") == 0) {
            doit[D_SHA1] = doit[D_SHA256] = doit[D_SHA512] = 1;
            continue;
        }
#ifndef OPENSSL_NO_RSA
# ifndef RSA_NULL
        if (strcmp(*argv, "openssl") == 0) {
            RSA_set_default_method(RSA_PKCS1_SSLeay());
            continue;
        }
# endif
        if (strcmp(*argv, "rsa") == 0) {
            rsa_doit[R_RSA_512] = rsa_doit[R_RSA_1024] =
                rsa_doit[R_RSA_2048] = rsa_doit[R_RSA_3072] =
                rsa_doit[R_RSA_4096] = rsa_doit[R_RSA_7680] =
                rsa_doit[R_RSA_15360] = 1;
            continue;
        }
        if (found(*argv, rsa_choices, &i)) {
            rsa_doit[i] = 1;
            continue;
        }
#endif
#ifndef OPENSSL_NO_DSA
        if (strcmp(*argv, "dsa") == 0) {
            dsa_doit[R_DSA_512] = dsa_doit[R_DSA_1024] =
                dsa_doit[R_DSA_2048] = 1;
            continue;
        }
        if (found(*argv, dsa_choices, &i)) {
            dsa_doit[i] = 2;
            continue;
        }
#endif
#ifndef OPENSSL_NO_AES
        if (strcmp(*argv, "aes") == 0) {
            doit[D_CBC_128_AES] = doit[D_CBC_192_AES] =
                doit[D_CBC_256_AES] = 1;
            continue;
        }
#endif
#ifndef OPENSSL_NO_CAMELLIA
        if (strcmp(*argv, "camellia") == 0) {
            doit[D_CBC_128_CML] = doit[D_CBC_192_CML] =
                doit[D_CBC_256_CML] = 1;
            continue;
        }
#endif
#ifndef OPENSSL_NO_EC
        if (strcmp(*argv, "ecdsa") == 0) {
            for (i = 0; i < EC_NUM; i++)
                ecdsa_doit[i] = 1;
            continue;
        }
        if (found(*argv, ecdsa_choices, &i)) {
            ecdsa_doit[i] = 2;
            continue;
        }
        if (strcmp(*argv, "ecdh") == 0) {
            for (i = 0; i < EC_NUM; i++)
                ecdh_doit[i] = 1;
            continue;
        }
        if (found(*argv, ecdh_choices, &i)) {
            ecdh_doit[i] = 2;
            continue;
        }
#endif
        BIO_printf(bio_err, "%s: Unknown algorithm %s\n", prog, *argv);
        goto end;
    }

#ifndef NO_FORK
    if (multi && do_multi(multi))
        goto show_res;
#endif

    /* No parameters; turn on everything. */
    if ((argc == 0) && !doit[D_EVP]) {
        for (i = 0; i < ALGOR_NUM; i++)
            if (i != D_EVP)
                doit[i] = 1;
        for (i = 0; i < RSA_NUM; i++)
            rsa_doit[i] = 1;
        for (i = 0; i < DSA_NUM; i++)
            dsa_doit[i] = 1;
#ifndef OPENSSL_NO_EC
        for (i = 0; i < EC_NUM; i++)
            ecdsa_doit[i] = 1;
        for (i = 0; i < EC_NUM; i++)
            ecdh_doit[i] = 1;
#endif
    }
    for (i = 0; i < ALGOR_NUM; i++)
        if (doit[i])
            pr_header++;

    if (usertime == 0 && !mr)
        BIO_printf(bio_err,
                   "You have chosen to measure elapsed time "
                   "instead of user CPU time.\n");

#ifndef OPENSSL_NO_RSA
    for (i = 0; i < RSA_NUM; i++) {
        const unsigned char *p;

        p = rsa_data[i];
        rsa_key[i] = d2i_RSAPrivateKey(NULL, &p, rsa_data_length[i]);
        if (rsa_key[i] == NULL) {
            BIO_printf(bio_err, "internal error loading RSA key number %d\n",
                       i);
            goto end;
        }
    }
#endif

#ifndef OPENSSL_NO_DSA
    dsa_key[0] = get_dsa512();
    dsa_key[1] = get_dsa1024();
    dsa_key[2] = get_dsa2048();
#endif

#ifndef OPENSSL_NO_DES
    DES_set_key_unchecked(&key, &sch);
    DES_set_key_unchecked(&key2, &sch2);
    DES_set_key_unchecked(&key3, &sch3);
#endif
#ifndef OPENSSL_NO_AES
    AES_set_encrypt_key(key16, 128, &aes_ks1);
    AES_set_encrypt_key(key24, 192, &aes_ks2);
    AES_set_encrypt_key(key32, 256, &aes_ks3);
#endif
#ifndef OPENSSL_NO_CAMELLIA
    Camellia_set_key(key16, 128, &camellia_ks1);
    Camellia_set_key(ckey24, 192, &camellia_ks2);
    Camellia_set_key(ckey32, 256, &camellia_ks3);
#endif
#ifndef OPENSSL_NO_IDEA
    idea_set_encrypt_key(key16, &idea_ks);
#endif
#ifndef OPENSSL_NO_SEED
    SEED_set_key(key16, &seed_ks);
#endif
#ifndef OPENSSL_NO_RC4
    RC4_set_key(&rc4_ks, 16, key16);
#endif
#ifndef OPENSSL_NO_RC2
    RC2_set_key(&rc2_ks, 16, key16, 128);
#endif
#ifndef OPENSSL_NO_RC5
    RC5_32_set_key(&rc5_ks, 16, key16, 12);
#endif
#ifndef OPENSSL_NO_BF
    BF_set_key(&bf_ks, 16, key16);
#endif
#ifndef OPENSSL_NO_CAST
    CAST_set_key(&cast_ks, 16, key16);
#endif
#ifndef OPENSSL_NO_RSA
    memset(rsa_c, 0, sizeof(rsa_c));
#endif
#ifndef SIGALRM
# ifndef OPENSSL_NO_DES
    BIO_printf(bio_err, "First we calculate the approximate speed ...\n");
    count = 10;
    do {
        long it;
        count *= 2;
        Time_F(START);
        for (it = count; it; it--)
            DES_ecb_encrypt((DES_cblock *)buf,
                            (DES_cblock *)buf, &sch, DES_ENCRYPT);
        d = Time_F(STOP);
    } while (d < 3);
    save_count = count;
    c[D_MD2][0] = count / 10;
    c[D_MDC2][0] = count / 10;
    c[D_MD4][0] = count;
    c[D_MD5][0] = count;
    c[D_HMAC][0] = count;
    c[D_SHA1][0] = count;
    c[D_RMD160][0] = count;
    c[D_RC4][0] = count * 5;
    c[D_CBC_DES][0] = count;
    c[D_EDE3_DES][0] = count / 3;
    c[D_CBC_IDEA][0] = count;
    c[D_CBC_SEED][0] = count;
    c[D_CBC_RC2][0] = count;
    c[D_CBC_RC5][0] = count;
    c[D_CBC_BF][0] = count;
    c[D_CBC_CAST][0] = count;
    c[D_CBC_128_AES][0] = count;
    c[D_CBC_192_AES][0] = count;
    c[D_CBC_256_AES][0] = count;
    c[D_CBC_128_CML][0] = count;
    c[D_CBC_192_CML][0] = count;
    c[D_CBC_256_CML][0] = count;
    c[D_SHA256][0] = count;
    c[D_SHA512][0] = count;
    c[D_WHIRLPOOL][0] = count;
    c[D_IGE_128_AES][0] = count;
    c[D_IGE_192_AES][0] = count;
    c[D_IGE_256_AES][0] = count;
    c[D_GHASH][0] = count;

    for (i = 1; i < SIZE_NUM; i++) {
        long l0, l1;

        l0 = (long)lengths[0];
        l1 = (long)lengths[i];

        c[D_MD2][i] = c[D_MD2][0] * 4 * l0 / l1;
        c[D_MDC2][i] = c[D_MDC2][0] * 4 * l0 / l1;
        c[D_MD4][i] = c[D_MD4][0] * 4 * l0 / l1;
        c[D_MD5][i] = c[D_MD5][0] * 4 * l0 / l1;
        c[D_HMAC][i] = c[D_HMAC][0] * 4 * l0 / l1;
        c[D_SHA1][i] = c[D_SHA1][0] * 4 * l0 / l1;
        c[D_RMD160][i] = c[D_RMD160][0] * 4 * l0 / l1;
        c[D_SHA256][i] = c[D_SHA256][0] * 4 * l0 / l1;
        c[D_SHA512][i] = c[D_SHA512][0] * 4 * l0 / l1;
        c[D_WHIRLPOOL][i] = c[D_WHIRLPOOL][0] * 4 * l0 / l1;

        l0 = (long)lengths[i - 1];

        c[D_RC4][i] = c[D_RC4][i - 1] * l0 / l1;
        c[D_CBC_DES][i] = c[D_CBC_DES][i - 1] * l0 / l1;
        c[D_EDE3_DES][i] = c[D_EDE3_DES][i - 1] * l0 / l1;
        c[D_CBC_IDEA][i] = c[D_CBC_IDEA][i - 1] * l0 / l1;
        c[D_CBC_SEED][i] = c[D_CBC_SEED][i - 1] * l0 / l1;
        c[D_CBC_RC2][i] = c[D_CBC_RC2][i - 1] * l0 / l1;
        c[D_CBC_RC5][i] = c[D_CBC_RC5][i - 1] * l0 / l1;
        c[D_CBC_BF][i] = c[D_CBC_BF][i - 1] * l0 / l1;
        c[D_CBC_CAST][i] = c[D_CBC_CAST][i - 1] * l0 / l1;
        c[D_CBC_128_AES][i] = c[D_CBC_128_AES][i - 1] * l0 / l1;
        c[D_CBC_192_AES][i] = c[D_CBC_192_AES][i - 1] * l0 / l1;
        c[D_CBC_256_AES][i] = c[D_CBC_256_AES][i - 1] * l0 / l1;
        c[D_CBC_128_CML][i] = c[D_CBC_128_CML][i - 1] * l0 / l1;
        c[D_CBC_192_CML][i] = c[D_CBC_192_CML][i - 1] * l0 / l1;
        c[D_CBC_256_CML][i] = c[D_CBC_256_CML][i - 1] * l0 / l1;
        c[D_IGE_128_AES][i] = c[D_IGE_128_AES][i - 1] * l0 / l1;
        c[D_IGE_192_AES][i] = c[D_IGE_192_AES][i - 1] * l0 / l1;
        c[D_IGE_256_AES][i] = c[D_IGE_256_AES][i - 1] * l0 / l1;
    }

#  ifndef OPENSSL_NO_RSA
    rsa_c[R_RSA_512][0] = count / 2000;
    rsa_c[R_RSA_512][1] = count / 400;
    for (i = 1; i < RSA_NUM; i++) {
        rsa_c[i][0] = rsa_c[i - 1][0] / 8;
        rsa_c[i][1] = rsa_c[i - 1][1] / 4;
        if ((rsa_doit[i] <= 1) && (rsa_c[i][0] == 0))
            rsa_doit[i] = 0;
        else {
            if (rsa_c[i][0] == 0) {
                rsa_c[i][0] = 1;
                rsa_c[i][1] = 20;
            }
        }
    }
#  endif

#  ifndef OPENSSL_NO_DSA
    dsa_c[R_DSA_512][0] = count / 1000;
    dsa_c[R_DSA_512][1] = count / 1000 / 2;
    for (i = 1; i < DSA_NUM; i++) {
        dsa_c[i][0] = dsa_c[i - 1][0] / 4;
        dsa_c[i][1] = dsa_c[i - 1][1] / 4;
        if ((dsa_doit[i] <= 1) && (dsa_c[i][0] == 0))
            dsa_doit[i] = 0;
        else {
            if (dsa_c[i] == 0) {
                dsa_c[i][0] = 1;
                dsa_c[i][1] = 1;
            }
        }
    }
#  endif

#  ifndef OPENSSL_NO_EC
    ecdsa_c[R_EC_P160][0] = count / 1000;
    ecdsa_c[R_EC_P160][1] = count / 1000 / 2;
    for (i = R_EC_P192; i <= R_EC_P521; i++) {
        ecdsa_c[i][0] = ecdsa_c[i - 1][0] / 2;
        ecdsa_c[i][1] = ecdsa_c[i - 1][1] / 2;
        if ((ecdsa_doit[i] <= 1) && (ecdsa_c[i][0] == 0))
            ecdsa_doit[i] = 0;
        else {
            if (ecdsa_c[i] == 0) {
                ecdsa_c[i][0] = 1;
                ecdsa_c[i][1] = 1;
            }
        }
    }
    ecdsa_c[R_EC_K163][0] = count / 1000;
    ecdsa_c[R_EC_K163][1] = count / 1000 / 2;
    for (i = R_EC_K233; i <= R_EC_K571; i++) {
        ecdsa_c[i][0] = ecdsa_c[i - 1][0] / 2;
        ecdsa_c[i][1] = ecdsa_c[i - 1][1] / 2;
        if ((ecdsa_doit[i] <= 1) && (ecdsa_c[i][0] == 0))
            ecdsa_doit[i] = 0;
        else {
            if (ecdsa_c[i] == 0) {
                ecdsa_c[i][0] = 1;
                ecdsa_c[i][1] = 1;
            }
        }
    }
    ecdsa_c[R_EC_B163][0] = count / 1000;
    ecdsa_c[R_EC_B163][1] = count / 1000 / 2;
    for (i = R_EC_B233; i <= R_EC_B571; i++) {
        ecdsa_c[i][0] = ecdsa_c[i - 1][0] / 2;
        ecdsa_c[i][1] = ecdsa_c[i - 1][1] / 2;
        if ((ecdsa_doit[i] <= 1) && (ecdsa_c[i][0] == 0))
            ecdsa_doit[i] = 0;
        else {
            if (ecdsa_c[i] == 0) {
                ecdsa_c[i][0] = 1;
                ecdsa_c[i][1] = 1;
            }
        }
    }

    ecdh_c[R_EC_P160][0] = count / 1000;
    ecdh_c[R_EC_P160][1] = count / 1000;
    for (i = R_EC_P192; i <= R_EC_P521; i++) {
        ecdh_c[i][0] = ecdh_c[i - 1][0] / 2;
        ecdh_c[i][1] = ecdh_c[i - 1][1] / 2;
        if ((ecdh_doit[i] <= 1) && (ecdh_c[i][0] == 0))
            ecdh_doit[i] = 0;
        else {
            if (ecdh_c[i] == 0) {
                ecdh_c[i][0] = 1;
                ecdh_c[i][1] = 1;
            }
        }
    }
    ecdh_c[R_EC_K163][0] = count / 1000;
    ecdh_c[R_EC_K163][1] = count / 1000;
    for (i = R_EC_K233; i <= R_EC_K571; i++) {
        ecdh_c[i][0] = ecdh_c[i - 1][0] / 2;
        ecdh_c[i][1] = ecdh_c[i - 1][1] / 2;
        if ((ecdh_doit[i] <= 1) && (ecdh_c[i][0] == 0))
            ecdh_doit[i] = 0;
        else {
            if (ecdh_c[i] == 0) {
                ecdh_c[i][0] = 1;
                ecdh_c[i][1] = 1;
            }
        }
    }
    ecdh_c[R_EC_B163][0] = count / 1000;
    ecdh_c[R_EC_B163][1] = count / 1000;
    for (i = R_EC_B233; i <= R_EC_B571; i++) {
        ecdh_c[i][0] = ecdh_c[i - 1][0] / 2;
        ecdh_c[i][1] = ecdh_c[i - 1][1] / 2;
        if ((ecdh_doit[i] <= 1) && (ecdh_c[i][0] == 0))
            ecdh_doit[i] = 0;
        else {
            if (ecdh_c[i] == 0) {
                ecdh_c[i][0] = 1;
                ecdh_c[i][1] = 1;
            }
        }
    }
#  endif

#  define COND(d) (count < (d))
#  define COUNT(d) (d)
# else
/* not worth fixing */
#  error "You cannot disable DES on systems without SIGALRM."
# endif                        /* OPENSSL_NO_DES */
#else
# define COND(c) (run && count<0x7fffffff)
# define COUNT(d) (count)
# ifndef _WIN32
    signal(SIGALRM, sig_done);
# endif
#endif                         /* SIGALRM */

#ifndef OPENSSL_NO_MD2
    if (doit[D_MD2]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_MD2], c[D_MD2][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_MD2][j]); count++)
                EVP_Digest(buf, (unsigned long)lengths[j], &(md2[0]), NULL,
                           EVP_md2(), NULL);
            d = Time_F(STOP);
            print_result(D_MD2, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_MDC2
    if (doit[D_MDC2]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_MDC2], c[D_MDC2][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_MDC2][j]); count++)
                EVP_Digest(buf, (unsigned long)lengths[j], &(mdc2[0]), NULL,
                           EVP_mdc2(), NULL);
            d = Time_F(STOP);
            print_result(D_MDC2, j, count, d);
        }
    }
#endif

#ifndef OPENSSL_NO_MD4
    if (doit[D_MD4]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_MD4], c[D_MD4][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_MD4][j]); count++)
                EVP_Digest(&(buf[0]), (unsigned long)lengths[j], &(md4[0]),
                           NULL, EVP_md4(), NULL);
            d = Time_F(STOP);
            print_result(D_MD4, j, count, d);
        }
    }
#endif

#ifndef OPENSSL_NO_MD5
    if (doit[D_MD5]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_MD5], c[D_MD5][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_MD5][j]); count++)
                MD5(buf, lengths[j], md5);
            d = Time_F(STOP);
            print_result(D_MD5, j, count, d);
        }
    }
#endif

#if !defined(OPENSSL_NO_MD5)
    if (doit[D_HMAC]) {
        HMAC_CTX hctx;

        HMAC_CTX_init(&hctx);
        HMAC_Init_ex(&hctx, (unsigned char *)"This is a key...",
                     16, EVP_md5(), NULL);

        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_HMAC], c[D_HMAC][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_HMAC][j]); count++) {
                HMAC_Init_ex(&hctx, NULL, 0, NULL, NULL);
                HMAC_Update(&hctx, buf, lengths[j]);
                HMAC_Final(&hctx, &(hmac[0]), NULL);
            }
            d = Time_F(STOP);
            print_result(D_HMAC, j, count, d);
        }
        HMAC_CTX_cleanup(&hctx);
    }
#endif
    if (doit[D_SHA1]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_SHA1], c[D_SHA1][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_SHA1][j]); count++)
                SHA1(buf, lengths[j], sha);
            d = Time_F(STOP);
            print_result(D_SHA1, j, count, d);
        }
    }
    if (doit[D_SHA256]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_SHA256], c[D_SHA256][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_SHA256][j]); count++)
                SHA256(buf, lengths[j], sha256);
            d = Time_F(STOP);
            print_result(D_SHA256, j, count, d);
        }
    }
    if (doit[D_SHA512]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_SHA512], c[D_SHA512][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_SHA512][j]); count++)
                SHA512(buf, lengths[j], sha512);
            d = Time_F(STOP);
            print_result(D_SHA512, j, count, d);
        }
    }

#ifndef OPENSSL_NO_WHIRLPOOL
    if (doit[D_WHIRLPOOL]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_WHIRLPOOL], c[D_WHIRLPOOL][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_WHIRLPOOL][j]); count++)
                WHIRLPOOL(buf, lengths[j], whirlpool);
            d = Time_F(STOP);
            print_result(D_WHIRLPOOL, j, count, d);
        }
    }
#endif

#ifndef OPENSSL_NO_RMD160
    if (doit[D_RMD160]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_RMD160], c[D_RMD160][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_RMD160][j]); count++)
                EVP_Digest(buf, (unsigned long)lengths[j], &(rmd160[0]), NULL,
                           EVP_ripemd160(), NULL);
            d = Time_F(STOP);
            print_result(D_RMD160, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_RC4
    if (doit[D_RC4]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_RC4], c[D_RC4][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_RC4][j]); count++)
                RC4(&rc4_ks, (unsigned int)lengths[j], buf, buf);
            d = Time_F(STOP);
            print_result(D_RC4, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_DES
    if (doit[D_CBC_DES]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_DES], c[D_CBC_DES][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_DES][j]); count++)
                DES_ncbc_encrypt(buf, buf, lengths[j], &sch,
                                 &DES_iv, DES_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_DES, j, count, d);
        }
    }

    if (doit[D_EDE3_DES]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_EDE3_DES], c[D_EDE3_DES][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_EDE3_DES][j]); count++)
                DES_ede3_cbc_encrypt(buf, buf, lengths[j],
                                     &sch, &sch2, &sch3,
                                     &DES_iv, DES_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_EDE3_DES, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_AES
    if (doit[D_CBC_128_AES]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_128_AES], c[D_CBC_128_AES][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_128_AES][j]); count++)
                AES_cbc_encrypt(buf, buf,
                                (unsigned long)lengths[j], &aes_ks1,
                                iv, AES_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_128_AES, j, count, d);
        }
    }
    if (doit[D_CBC_192_AES]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_192_AES], c[D_CBC_192_AES][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_192_AES][j]); count++)
                AES_cbc_encrypt(buf, buf,
                                (unsigned long)lengths[j], &aes_ks2,
                                iv, AES_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_192_AES, j, count, d);
        }
    }
    if (doit[D_CBC_256_AES]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_256_AES], c[D_CBC_256_AES][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_256_AES][j]); count++)
                AES_cbc_encrypt(buf, buf,
                                (unsigned long)lengths[j], &aes_ks3,
                                iv, AES_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_256_AES, j, count, d);
        }
    }

    if (doit[D_IGE_128_AES]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_IGE_128_AES], c[D_IGE_128_AES][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_IGE_128_AES][j]); count++)
                AES_ige_encrypt(buf, buf2,
                                (unsigned long)lengths[j], &aes_ks1,
                                iv, AES_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_IGE_128_AES, j, count, d);
        }
    }
    if (doit[D_IGE_192_AES]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_IGE_192_AES], c[D_IGE_192_AES][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_IGE_192_AES][j]); count++)
                AES_ige_encrypt(buf, buf2,
                                (unsigned long)lengths[j], &aes_ks2,
                                iv, AES_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_IGE_192_AES, j, count, d);
        }
    }
    if (doit[D_IGE_256_AES]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_IGE_256_AES], c[D_IGE_256_AES][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_IGE_256_AES][j]); count++)
                AES_ige_encrypt(buf, buf2,
                                (unsigned long)lengths[j], &aes_ks3,
                                iv, AES_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_IGE_256_AES, j, count, d);
        }
    }
    if (doit[D_GHASH]) {
        GCM128_CONTEXT *ctx =
            CRYPTO_gcm128_new(&aes_ks1, (block128_f) AES_encrypt);
        CRYPTO_gcm128_setiv(ctx, (unsigned char *)"0123456789ab", 12);

        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_GHASH], c[D_GHASH][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_GHASH][j]); count++)
                CRYPTO_gcm128_aad(ctx, buf, lengths[j]);
            d = Time_F(STOP);
            print_result(D_GHASH, j, count, d);
        }
        CRYPTO_gcm128_release(ctx);
    }
#endif
#ifndef OPENSSL_NO_CAMELLIA
    if (doit[D_CBC_128_CML]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_128_CML], c[D_CBC_128_CML][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_128_CML][j]); count++)
                Camellia_cbc_encrypt(buf, buf,
                                     (unsigned long)lengths[j], &camellia_ks1,
                                     iv, CAMELLIA_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_128_CML, j, count, d);
        }
    }
    if (doit[D_CBC_192_CML]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_192_CML], c[D_CBC_192_CML][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_192_CML][j]); count++)
                Camellia_cbc_encrypt(buf, buf,
                                     (unsigned long)lengths[j], &camellia_ks2,
                                     iv, CAMELLIA_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_192_CML, j, count, d);
        }
    }
    if (doit[D_CBC_256_CML]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_256_CML], c[D_CBC_256_CML][j],
                          lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_256_CML][j]); count++)
                Camellia_cbc_encrypt(buf, buf,
                                     (unsigned long)lengths[j], &camellia_ks3,
                                     iv, CAMELLIA_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_256_CML, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_IDEA
    if (doit[D_CBC_IDEA]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_IDEA], c[D_CBC_IDEA][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_IDEA][j]); count++)
                idea_cbc_encrypt(buf, buf,
                                 (unsigned long)lengths[j], &idea_ks,
                                 iv, IDEA_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_IDEA, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_SEED
    if (doit[D_CBC_SEED]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_SEED], c[D_CBC_SEED][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_SEED][j]); count++)
                SEED_cbc_encrypt(buf, buf,
                                 (unsigned long)lengths[j], &seed_ks, iv, 1);
            d = Time_F(STOP);
            print_result(D_CBC_SEED, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_RC2
    if (doit[D_CBC_RC2]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_RC2], c[D_CBC_RC2][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_RC2][j]); count++)
                RC2_cbc_encrypt(buf, buf,
                                (unsigned long)lengths[j], &rc2_ks,
                                iv, RC2_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_RC2, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_RC5
    if (doit[D_CBC_RC5]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_RC5], c[D_CBC_RC5][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_RC5][j]); count++)
                RC5_32_cbc_encrypt(buf, buf,
                                   (unsigned long)lengths[j], &rc5_ks,
                                   iv, RC5_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_RC5, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_BF
    if (doit[D_CBC_BF]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_BF], c[D_CBC_BF][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_BF][j]); count++)
                BF_cbc_encrypt(buf, buf,
                               (unsigned long)lengths[j], &bf_ks,
                               iv, BF_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_BF, j, count, d);
        }
    }
#endif
#ifndef OPENSSL_NO_CAST
    if (doit[D_CBC_CAST]) {
        for (j = 0; j < SIZE_NUM; j++) {
            print_message(names[D_CBC_CAST], c[D_CBC_CAST][j], lengths[j]);
            Time_F(START);
            for (count = 0, run = 1; COND(c[D_CBC_CAST][j]); count++)
                CAST_cbc_encrypt(buf, buf,
                                 (unsigned long)lengths[j], &cast_ks,
                                 iv, CAST_ENCRYPT);
            d = Time_F(STOP);
            print_result(D_CBC_CAST, j, count, d);
        }
    }
#endif

    if (doit[D_EVP]) {
#ifdef EVP_CIPH_FLAG_TLS1_1_MULTIBLOCK
        if (multiblock && evp_cipher) {
            if (!
                (EVP_CIPHER_flags(evp_cipher) &
                 EVP_CIPH_FLAG_TLS1_1_MULTIBLOCK)) {
                BIO_printf(bio_err, "%s is not multi-block capable\n",
                        OBJ_nid2ln(evp_cipher->nid));
                goto end;
            }
            multiblock_speed(evp_cipher);
            ret = 0;
            goto end;
        }
#endif
        for (j = 0; j < SIZE_NUM; j++) {
            if (evp_cipher) {
                EVP_CIPHER_CTX ctx;
                int outl;

                names[D_EVP] = OBJ_nid2ln(evp_cipher->nid);
                /*
                 * -O3 -fschedule-insns messes up an optimization here!
                 * names[D_EVP] somehow becomes NULL
                 */
                print_message(names[D_EVP], save_count, lengths[j]);

                EVP_CIPHER_CTX_init(&ctx);
                if (decrypt)
                    EVP_DecryptInit_ex(&ctx, evp_cipher, NULL, key16, iv);
                else
                    EVP_EncryptInit_ex(&ctx, evp_cipher, NULL, key16, iv);
                EVP_CIPHER_CTX_set_padding(&ctx, 0);

                Time_F(START);
                if (decrypt)
                    for (count = 0, run = 1;
                         COND(save_count * 4 * lengths[0] / lengths[j]);
                         count++)
                        EVP_DecryptUpdate(&ctx, buf, &outl, buf, lengths[j]);
                else
                    for (count = 0, run = 1;
                         COND(save_count * 4 * lengths[0] / lengths[j]);
                         count++)
                        EVP_EncryptUpdate(&ctx, buf, &outl, buf, lengths[j]);
                if (decrypt)
                    EVP_DecryptFinal_ex(&ctx, buf, &outl);
                else
                    EVP_EncryptFinal_ex(&ctx, buf, &outl);
                d = Time_F(STOP);
                EVP_CIPHER_CTX_cleanup(&ctx);
            }
            if (evp_md) {
                names[D_EVP] = OBJ_nid2ln(evp_md->type);
                print_message(names[D_EVP], save_count, lengths[j]);

                Time_F(START);
                for (count = 0, run = 1;
                     COND(save_count * 4 * lengths[0] / lengths[j]); count++)
                    EVP_Digest(buf, lengths[j], &(md[0]), NULL, evp_md, NULL);

                d = Time_F(STOP);
            }
            print_result(D_EVP, j, count, d);
        }
    }

    RAND_bytes(buf, 36);
#ifndef OPENSSL_NO_RSA
    for (j = 0; j < RSA_NUM; j++) {
        int st;
        if (!rsa_doit[j])
            continue;
        st = RSA_sign(NID_md5_sha1, buf, 36, buf2, &rsa_num, rsa_key[j]);
        if (st == 0) {
            BIO_printf(bio_err,
                       "RSA sign failure.  No RSA sign will be done.\n");
            ERR_print_errors(bio_err);
            rsa_count = 1;
        } else {
            pkey_print_message("private", "rsa",
                               rsa_c[j][0], rsa_bits[j], RSA_SECONDS);
            /* RSA_blinding_on(rsa_key[j],NULL); */
            Time_F(START);
            for (count = 0, run = 1; COND(rsa_c[j][0]); count++) {
                st = RSA_sign(NID_md5_sha1, buf, 36, buf2,
                              &rsa_num, rsa_key[j]);
                if (st == 0) {
                    BIO_printf(bio_err, "RSA sign failure\n");
                    ERR_print_errors(bio_err);
                    count = 1;
                    break;
                }
            }
            d = Time_F(STOP);
            BIO_printf(bio_err,
                       mr ? "+R1:%ld:%d:%.2f\n"
                       : "%ld %d bit private RSA's in %.2fs\n",
                       count, rsa_bits[j], d);
            rsa_results[j][0] = d / (double)count;
            rsa_count = count;
        }

        st = RSA_verify(NID_md5_sha1, buf, 36, buf2, rsa_num, rsa_key[j]);
        if (st <= 0) {
            BIO_printf(bio_err,
                       "RSA verify failure.  No RSA verify will be done.\n");
            ERR_print_errors(bio_err);
            rsa_doit[j] = 0;
        } else {
            pkey_print_message("public", "rsa",
                               rsa_c[j][1], rsa_bits[j], RSA_SECONDS);
            Time_F(START);
            for (count = 0, run = 1; COND(rsa_c[j][1]); count++) {
                st = RSA_verify(NID_md5_sha1, buf, 36, buf2,
                                rsa_num, rsa_key[j]);
                if (st <= 0) {
                    BIO_printf(bio_err, "RSA verify failure\n");
                    ERR_print_errors(bio_err);
                    count = 1;
                    break;
                }
            }
            d = Time_F(STOP);
            BIO_printf(bio_err,
                       mr ? "+R2:%ld:%d:%.2f\n"
                       : "%ld %d bit public RSA's in %.2fs\n",
                       count, rsa_bits[j], d);
            rsa_results[j][1] = d / (double)count;
        }

        if (rsa_count <= 1) {
            /* if longer than 10s, don't do any more */
            for (j++; j < RSA_NUM; j++)
                rsa_doit[j] = 0;
        }
    }
#endif

    RAND_bytes(buf, 20);
#ifndef OPENSSL_NO_DSA
    if (RAND_status() != 1) {
        RAND_seed(rnd_seed, sizeof rnd_seed);
        rnd_fake = 1;
    }
    for (j = 0; j < DSA_NUM; j++) {
        unsigned int kk;
        int st;

        if (!dsa_doit[j])
            continue;

        /* DSA_generate_key(dsa_key[j]); */
        /* DSA_sign_setup(dsa_key[j],NULL); */
        st = DSA_sign(EVP_PKEY_DSA, buf, 20, buf2, &kk, dsa_key[j]);
        if (st == 0) {
            BIO_printf(bio_err,
                       "DSA sign failure.  No DSA sign will be done.\n");
            ERR_print_errors(bio_err);
            rsa_count = 1;
        } else {
            pkey_print_message("sign", "dsa",
                               dsa_c[j][0], dsa_bits[j], DSA_SECONDS);
            Time_F(START);
            for (count = 0, run = 1; COND(dsa_c[j][0]); count++) {
                st = DSA_sign(EVP_PKEY_DSA, buf, 20, buf2, &kk, dsa_key[j]);
                if (st == 0) {
                    BIO_printf(bio_err, "DSA sign failure\n");
                    ERR_print_errors(bio_err);
                    count = 1;
                    break;
                }
            }
            d = Time_F(STOP);
            BIO_printf(bio_err,
                       mr ? "+R3:%ld:%d:%.2f\n"
                       : "%ld %d bit DSA signs in %.2fs\n",
                       count, dsa_bits[j], d);
            dsa_results[j][0] = d / (double)count;
            rsa_count = count;
        }

        st = DSA_verify(EVP_PKEY_DSA, buf, 20, buf2, kk, dsa_key[j]);
        if (st <= 0) {
            BIO_printf(bio_err,
                       "DSA verify failure.  No DSA verify will be done.\n");
            ERR_print_errors(bio_err);
            dsa_doit[j] = 0;
        } else {
            pkey_print_message("verify", "dsa",
                               dsa_c[j][1], dsa_bits[j], DSA_SECONDS);
            Time_F(START);
            for (count = 0, run = 1; COND(dsa_c[j][1]); count++) {
                st = DSA_verify(EVP_PKEY_DSA, buf, 20, buf2, kk, dsa_key[j]);
                if (st <= 0) {
                    BIO_printf(bio_err, "DSA verify failure\n");
                    ERR_print_errors(bio_err);
                    count = 1;
                    break;
                }
            }
            d = Time_F(STOP);
            BIO_printf(bio_err,
                       mr ? "+R4:%ld:%d:%.2f\n"
                       : "%ld %d bit DSA verify in %.2fs\n",
                       count, dsa_bits[j], d);
            dsa_results[j][1] = d / (double)count;
        }

        if (rsa_count <= 1) {
            /* if longer than 10s, don't do any more */
            for (j++; j < DSA_NUM; j++)
                dsa_doit[j] = 0;
        }
    }
    if (rnd_fake)
        RAND_cleanup();
#endif

#ifndef OPENSSL_NO_EC
    if (RAND_status() != 1) {
        RAND_seed(rnd_seed, sizeof rnd_seed);
        rnd_fake = 1;
    }
    for (j = 0; j < EC_NUM; j++) {
        int st;

        if (!ecdsa_doit[j])
            continue;           /* Ignore Curve */
        ecdsa[j] = EC_KEY_new_by_curve_name(test_curves[j]);
        if (ecdsa[j] == NULL) {
            BIO_printf(bio_err, "ECDSA failure.\n");
            ERR_print_errors(bio_err);
            rsa_count = 1;
        } else {
            EC_KEY_precompute_mult(ecdsa[j], NULL);
            /* Perform ECDSA signature test */
            EC_KEY_generate_key(ecdsa[j]);
            st = ECDSA_sign(0, buf, 20, ecdsasig, &ecdsasiglen, ecdsa[j]);
            if (st == 0) {
                BIO_printf(bio_err,
                           "ECDSA sign failure.  No ECDSA sign will be done.\n");
                ERR_print_errors(bio_err);
                rsa_count = 1;
            } else {
                pkey_print_message("sign", "ecdsa",
                                   ecdsa_c[j][0],
                                   test_curves_bits[j], ECDSA_SECONDS);

                Time_F(START);
                for (count = 0, run = 1; COND(ecdsa_c[j][0]); count++) {
                    st = ECDSA_sign(0, buf, 20,
                                    ecdsasig, &ecdsasiglen, ecdsa[j]);
                    if (st == 0) {
                        BIO_printf(bio_err, "ECDSA sign failure\n");
                        ERR_print_errors(bio_err);
                        count = 1;
                        break;
                    }
                }
                d = Time_F(STOP);

                BIO_printf(bio_err,
                           mr ? "+R5:%ld:%d:%.2f\n" :
                           "%ld %d bit ECDSA signs in %.2fs \n",
                           count, test_curves_bits[j], d);
                ecdsa_results[j][0] = d / (double)count;
                rsa_count = count;
            }

            /* Perform ECDSA verification test */
            st = ECDSA_verify(0, buf, 20, ecdsasig, ecdsasiglen, ecdsa[j]);
            if (st != 1) {
                BIO_printf(bio_err,
                           "ECDSA verify failure.  No ECDSA verify will be done.\n");
                ERR_print_errors(bio_err);
                ecdsa_doit[j] = 0;
            } else {
                pkey_print_message("verify", "ecdsa",
                                   ecdsa_c[j][1],
                                   test_curves_bits[j], ECDSA_SECONDS);
                Time_F(START);
                for (count = 0, run = 1; COND(ecdsa_c[j][1]); count++) {
                    st = ECDSA_verify(0, buf, 20, ecdsasig, ecdsasiglen,
                                      ecdsa[j]);
                    if (st != 1) {
                        BIO_printf(bio_err, "ECDSA verify failure\n");
                        ERR_print_errors(bio_err);
                        count = 1;
                        break;
                    }
                }
                d = Time_F(STOP);
                BIO_printf(bio_err,
                           mr ? "+R6:%ld:%d:%.2f\n"
                           : "%ld %d bit ECDSA verify in %.2fs\n",
                           count, test_curves_bits[j], d);
                ecdsa_results[j][1] = d / (double)count;
            }

            if (rsa_count <= 1) {
                /* if longer than 10s, don't do any more */
                for (j++; j < EC_NUM; j++)
                    ecdsa_doit[j] = 0;
            }
        }
    }
    if (rnd_fake)
        RAND_cleanup();
#endif

#ifndef OPENSSL_NO_EC
    if (RAND_status() != 1) {
        RAND_seed(rnd_seed, sizeof rnd_seed);
        rnd_fake = 1;
    }
    for (j = 0; j < EC_NUM; j++) {
        if (!ecdh_doit[j])
            continue;
        ecdh_a[j] = EC_KEY_new_by_curve_name(test_curves[j]);
        ecdh_b[j] = EC_KEY_new_by_curve_name(test_curves[j]);
        if ((ecdh_a[j] == NULL) || (ecdh_b[j] == NULL)) {
            BIO_printf(bio_err, "ECDH failure.\n");
            ERR_print_errors(bio_err);
            rsa_count = 1;
        } else {
            /* generate two ECDH key pairs */
            if (!EC_KEY_generate_key(ecdh_a[j]) ||
                !EC_KEY_generate_key(ecdh_b[j])) {
                BIO_printf(bio_err, "ECDH key generation failure.\n");
                ERR_print_errors(bio_err);
                rsa_count = 1;
            } else {
                /*
                 * If field size is not more than 24 octets, then use SHA-1
                 * hash of result; otherwise, use result (see section 4.8 of
                 * draft-ietf-tls-ecc-03.txt).
                 */
                int field_size, outlen;
                void *(*kdf) (const void *in, size_t inlen, void *out,
                              size_t *xoutlen);
                field_size =
                    EC_GROUP_get_degree(EC_KEY_get0_group(ecdh_a[j]));
                if (field_size <= 24 * 8) {
                    outlen = KDF1_SHA1_len;
                    kdf = KDF1_SHA1;
                } else {
                    outlen = (field_size + 7) / 8;
                    kdf = NULL;
                }
                secret_size_a =
                    ECDH_compute_key(secret_a, outlen,
                                     EC_KEY_get0_public_key(ecdh_b[j]),
                                     ecdh_a[j], kdf);
                secret_size_b =
                    ECDH_compute_key(secret_b, outlen,
                                     EC_KEY_get0_public_key(ecdh_a[j]),
                                     ecdh_b[j], kdf);
                if (secret_size_a != secret_size_b)
                    ecdh_checks = 0;
                else
                    ecdh_checks = 1;

                for (secret_idx = 0; (secret_idx < secret_size_a)
                     && (ecdh_checks == 1); secret_idx++) {
                    if (secret_a[secret_idx] != secret_b[secret_idx])
                        ecdh_checks = 0;
                }

                if (ecdh_checks == 0) {
                    BIO_printf(bio_err, "ECDH computations don't match.\n");
                    ERR_print_errors(bio_err);
                    rsa_count = 1;
                }

                pkey_print_message("", "ecdh",
                                   ecdh_c[j][0],
                                   test_curves_bits[j], ECDH_SECONDS);
                Time_F(START);
                for (count = 0, run = 1; COND(ecdh_c[j][0]); count++) {
                    ECDH_compute_key(secret_a, outlen,
                                     EC_KEY_get0_public_key(ecdh_b[j]),
                                     ecdh_a[j], kdf);
                }
                d = Time_F(STOP);
                BIO_printf(bio_err,
                           mr ? "+R7:%ld:%d:%.2f\n" :
                           "%ld %d-bit ECDH ops in %.2fs\n", count,
                           test_curves_bits[j], d);
                ecdh_results[j][0] = d / (double)count;
                rsa_count = count;
            }
        }

        if (rsa_count <= 1) {
            /* if longer than 10s, don't do any more */
            for (j++; j < EC_NUM; j++)
                ecdh_doit[j] = 0;
        }
    }
    if (rnd_fake)
        RAND_cleanup();
#endif
#ifndef NO_FORK
 show_res:
#endif
    if (!mr) {
        printf("%s\n", SSLeay_version(SSLEAY_VERSION));
        printf("%s\n", SSLeay_version(SSLEAY_BUILT_ON));
        printf("options:");
        printf("%s ", BN_options());
#ifndef OPENSSL_NO_MD2
        printf("%s ", MD2_options());
#endif
#ifndef OPENSSL_NO_RC4
        printf("%s ", RC4_options());
#endif
#ifndef OPENSSL_NO_DES
        printf("%s ", DES_options());
#endif
#ifndef OPENSSL_NO_AES
        printf("%s ", AES_options());
#endif
#ifndef OPENSSL_NO_IDEA
        printf("%s ", idea_options());
#endif
#ifndef OPENSSL_NO_BF
        printf("%s ", BF_options());
#endif
        printf("\n%s\n", SSLeay_version(SSLEAY_CFLAGS));
    }

    if (pr_header) {
        if (mr)
            printf("+H");
        else {
            printf
                ("The 'numbers' are in 1000s of bytes per second processed.\n");
            printf("type        ");
        }
        for (j = 0; j < SIZE_NUM; j++)
            printf(mr ? ":%d" : "%7d bytes", lengths[j]);
        printf("\n");
    }

    for (k = 0; k < ALGOR_NUM; k++) {
        if (!doit[k])
            continue;
        if (mr)
            printf("+F:%d:%s", k, names[k]);
        else
            printf("%-13s", names[k]);
        for (j = 0; j < SIZE_NUM; j++) {
            if (results[k][j] > 10000 && !mr)
                printf(" %11.2fk", results[k][j] / 1e3);
            else
                printf(mr ? ":%.2f" : " %11.2f ", results[k][j]);
        }
        printf("\n");
    }
#ifndef OPENSSL_NO_RSA
    j = 1;
    for (k = 0; k < RSA_NUM; k++) {
        if (!rsa_doit[k])
            continue;
        if (j && !mr) {
            printf("%18ssign    verify    sign/s verify/s\n", " ");
            j = 0;
        }
        if (mr)
            printf("+F2:%u:%u:%f:%f\n",
                   k, rsa_bits[k], rsa_results[k][0], rsa_results[k][1]);
        else
            printf("rsa %4u bits %8.6fs %8.6fs %8.1f %8.1f\n",
                   rsa_bits[k], rsa_results[k][0], rsa_results[k][1],
                   1.0 / rsa_results[k][0], 1.0 / rsa_results[k][1]);
    }
#endif
#ifndef OPENSSL_NO_DSA
    j = 1;
    for (k = 0; k < DSA_NUM; k++) {
        if (!dsa_doit[k])
            continue;
        if (j && !mr) {
            printf("%18ssign    verify    sign/s verify/s\n", " ");
            j = 0;
        }
        if (mr)
            printf("+F3:%u:%u:%f:%f\n",
                   k, dsa_bits[k], dsa_results[k][0], dsa_results[k][1]);
        else
            printf("dsa %4u bits %8.6fs %8.6fs %8.1f %8.1f\n",
                   dsa_bits[k], dsa_results[k][0], dsa_results[k][1],
                   1.0 / dsa_results[k][0], 1.0 / dsa_results[k][1]);
    }
#endif
#ifndef OPENSSL_NO_EC
    j = 1;
    for (k = 0; k < EC_NUM; k++) {
        if (!ecdsa_doit[k])
            continue;
        if (j && !mr) {
            printf("%30ssign    verify    sign/s verify/s\n", " ");
            j = 0;
        }

        if (mr)
            printf("+F4:%u:%u:%f:%f\n",
                   k, test_curves_bits[k],
                   ecdsa_results[k][0], ecdsa_results[k][1]);
        else
            printf("%4u bit ecdsa (%s) %8.4fs %8.4fs %8.1f %8.1f\n",
                   test_curves_bits[k],
                   test_curves_names[k],
                   ecdsa_results[k][0], ecdsa_results[k][1],
                   1.0 / ecdsa_results[k][0], 1.0 / ecdsa_results[k][1]);
    }
#endif

#ifndef OPENSSL_NO_EC
    j = 1;
    for (k = 0; k < EC_NUM; k++) {
        if (!ecdh_doit[k])
            continue;
        if (j && !mr) {
            printf("%30sop      op/s\n", " ");
            j = 0;
        }
        if (mr)
            printf("+F5:%u:%u:%f:%f\n",
                   k, test_curves_bits[k],
                   ecdh_results[k][0], 1.0 / ecdh_results[k][0]);

        else
            printf("%4u bit ecdh (%s) %8.4fs %8.1f\n",
                   test_curves_bits[k],
                   test_curves_names[k],
                   ecdh_results[k][0], 1.0 / ecdh_results[k][0]);
    }
#endif

    ret = 0;

 end:
    ERR_print_errors(bio_err);
    OPENSSL_free(buf_malloc);
    OPENSSL_free(buf2_malloc);
#ifndef OPENSSL_NO_RSA
    for (i = 0; i < RSA_NUM; i++)
        RSA_free(rsa_key[i]);
#endif
#ifndef OPENSSL_NO_DSA
    for (i = 0; i < DSA_NUM; i++)
        DSA_free(dsa_key[i]);
#endif

#ifndef OPENSSL_NO_EC
    for (i = 0; i < EC_NUM; i++) {
        EC_KEY_free(ecdsa[i]);
        EC_KEY_free(ecdh_a[i]);
        EC_KEY_free(ecdh_b[i]);
    }
#endif
    return (ret);
}

static void print_message(const char *s, long num, int length)
{
#ifdef SIGALRM
    BIO_printf(bio_err,
               mr ? "+DT:%s:%d:%d\n"
               : "Doing %s for %ds on %d size blocks: ", s, SECONDS, length);
    (void)BIO_flush(bio_err);
    alarm(SECONDS);
#else
    BIO_printf(bio_err,
               mr ? "+DN:%s:%ld:%d\n"
               : "Doing %s %ld times on %d size blocks: ", s, num, length);
    (void)BIO_flush(bio_err);
#endif
}

static void pkey_print_message(const char *str, const char *str2, long num,
                               int bits, int tm)
{
#ifdef SIGALRM
    BIO_printf(bio_err,
               mr ? "+DTP:%d:%s:%s:%d\n"
               : "Doing %d bit %s %s's for %ds: ", bits, str, str2, tm);
    (void)BIO_flush(bio_err);
    alarm(tm);
#else
    BIO_printf(bio_err,
               mr ? "+DNP:%ld:%d:%s:%s\n"
               : "Doing %ld %d bit %s %s's: ", num, bits, str, str2);
    (void)BIO_flush(bio_err);
#endif
}

static void print_result(int alg, int run_no, int count, double time_used)
{
    BIO_printf(bio_err,
               mr ? "+R:%d:%s:%f\n"
               : "%d %s's in %.2fs\n", count, names[alg], time_used);
    results[alg][run_no] = ((double)count) / time_used * lengths[run_no];
}

#ifndef NO_FORK
static char *sstrsep(char **string, const char *delim)
{
    char isdelim[256];
    char *token = *string;

    if (**string == 0)
        return NULL;

    memset(isdelim, 0, sizeof isdelim);
    isdelim[0] = 1;

    while (*delim) {
        isdelim[(unsigned char)(*delim)] = 1;
        delim++;
    }

    while (!isdelim[(unsigned char)(**string)]) {
        (*string)++;
    }

    if (**string) {
        **string = 0;
        (*string)++;
    }

    return token;
}

static int do_multi(int multi)
{
    int n;
    int fd[2];
    int *fds;
    static char sep[] = ":";

    fds = malloc(sizeof(*fds) * multi);
    for (n = 0; n < multi; ++n) {
        if (pipe(fd) == -1) {
            BIO_printf(bio_err, "pipe failure\n");
            exit(1);
        }
        fflush(stdout);
        (void)BIO_flush(bio_err);
        if (fork()) {
            close(fd[1]);
            fds[n] = fd[0];
        } else {
            close(fd[0]);
            close(1);
            if (dup(fd[1]) == -1) {
                BIO_printf(bio_err, "dup failed\n");
                exit(1);
            }
            close(fd[1]);
            mr = 1;
            usertime = 0;
            free(fds);
            return 0;
        }
        printf("Forked child %d\n", n);
    }

    /* for now, assume the pipe is long enough to take all the output */
    for (n = 0; n < multi; ++n) {
        FILE *f;
        char buf[1024];
        char *p;

        f = fdopen(fds[n], "r");
        while (fgets(buf, sizeof buf, f)) {
            p = strchr(buf, '\n');
            if (p)
                *p = '\0';
            if (buf[0] != '+') {
                BIO_printf(bio_err, "Don't understand line '%s' from child %d\n",
                        buf, n);
                continue;
            }
            printf("Got: %s from %d\n", buf, n);
            if (strncmp(buf, "+F:", 3) == 0) {
                int alg;
                int j;

                p = buf + 3;
                alg = atoi(sstrsep(&p, sep));
                sstrsep(&p, sep);
                for (j = 0; j < SIZE_NUM; ++j)
                    results[alg][j] += atof(sstrsep(&p, sep));
            } else if (strncmp(buf, "+F2:", 4) == 0) {
                int k;
                double d;

                p = buf + 4;
                k = atoi(sstrsep(&p, sep));
                sstrsep(&p, sep);

                d = atof(sstrsep(&p, sep));
                if (n)
                    rsa_results[k][0] = 1 / (1 / rsa_results[k][0] + 1 / d);
                else
                    rsa_results[k][0] = d;

                d = atof(sstrsep(&p, sep));
                if (n)
                    rsa_results[k][1] = 1 / (1 / rsa_results[k][1] + 1 / d);
                else
                    rsa_results[k][1] = d;
            }
# ifndef OPENSSL_NO_DSA
            else if (strncmp(buf, "+F3:", 4) == 0) {
                int k;
                double d;

                p = buf + 4;
                k = atoi(sstrsep(&p, sep));
                sstrsep(&p, sep);

                d = atof(sstrsep(&p, sep));
                if (n)
                    dsa_results[k][0] = 1 / (1 / dsa_results[k][0] + 1 / d);
                else
                    dsa_results[k][0] = d;

                d = atof(sstrsep(&p, sep));
                if (n)
                    dsa_results[k][1] = 1 / (1 / dsa_results[k][1] + 1 / d);
                else
                    dsa_results[k][1] = d;
            }
# endif
# ifndef OPENSSL_NO_EC
            else if (strncmp(buf, "+F4:", 4) == 0) {
                int k;
                double d;

                p = buf + 4;
                k = atoi(sstrsep(&p, sep));
                sstrsep(&p, sep);

                d = atof(sstrsep(&p, sep));
                if (n)
                    ecdsa_results[k][0] =
                        1 / (1 / ecdsa_results[k][0] + 1 / d);
                else
                    ecdsa_results[k][0] = d;

                d = atof(sstrsep(&p, sep));
                if (n)
                    ecdsa_results[k][1] =
                        1 / (1 / ecdsa_results[k][1] + 1 / d);
                else
                    ecdsa_results[k][1] = d;
            }
# endif

# ifndef OPENSSL_NO_EC
            else if (strncmp(buf, "+F5:", 4) == 0) {
                int k;
                double d;

                p = buf + 4;
                k = atoi(sstrsep(&p, sep));
                sstrsep(&p, sep);

                d = atof(sstrsep(&p, sep));
                if (n)
                    ecdh_results[k][0] = 1 / (1 / ecdh_results[k][0] + 1 / d);
                else
                    ecdh_results[k][0] = d;

            }
# endif

            else if (strncmp(buf, "+H:", 3) == 0) {
                ;
            } else
                BIO_printf(bio_err, "Unknown type '%s' from child %d\n", buf, n);
        }

        fclose(f);
    }
    free(fds);
    return 1;
}
#endif

static void multiblock_speed(const EVP_CIPHER *evp_cipher)
{
    static int mblengths[] =
        { 8 * 1024, 2 * 8 * 1024, 4 * 8 * 1024, 8 * 8 * 1024, 8 * 16 * 1024 };
    int j, count, num = OSSL_NELEM(lengths);
    const char *alg_name;
    unsigned char *inp, *out, no_key[32], no_iv[16];
    EVP_CIPHER_CTX ctx;
    double d = 0.0;

    inp = app_malloc(mblengths[num - 1], "multiblock input buffer");
    out = app_malloc(mblengths[num - 1] + 1024, "multiblock output buffer");
    EVP_CIPHER_CTX_init(&ctx);
    EVP_EncryptInit_ex(&ctx, evp_cipher, NULL, no_key, no_iv);
    EVP_CIPHER_CTX_ctrl(&ctx, EVP_CTRL_AEAD_SET_MAC_KEY, sizeof(no_key),
                        no_key);
    alg_name = OBJ_nid2ln(evp_cipher->nid);

    for (j = 0; j < num; j++) {
        print_message(alg_name, 0, mblengths[j]);
        Time_F(START);
        for (count = 0, run = 1; run && count < 0x7fffffff; count++) {
            unsigned char aad[EVP_AEAD_TLS1_AAD_LEN];
            EVP_CTRL_TLS1_1_MULTIBLOCK_PARAM mb_param;
            size_t len = mblengths[j];
            int packlen;

            memset(aad, 0, 8);  /* avoid uninitialized values */
            aad[8] = 23;        /* SSL3_RT_APPLICATION_DATA */
            aad[9] = 3;         /* version */
            aad[10] = 2;
            aad[11] = 0;        /* length */
            aad[12] = 0;
            mb_param.out = NULL;
            mb_param.inp = aad;
            mb_param.len = len;
            mb_param.interleave = 8;

            packlen = EVP_CIPHER_CTX_ctrl(&ctx,
                                          EVP_CTRL_TLS1_1_MULTIBLOCK_AAD,
                                          sizeof(mb_param), &mb_param);

            if (packlen > 0) {
                mb_param.out = out;
                mb_param.inp = inp;
                mb_param.len = len;
                EVP_CIPHER_CTX_ctrl(&ctx,
                                    EVP_CTRL_TLS1_1_MULTIBLOCK_ENCRYPT,
                                    sizeof(mb_param), &mb_param);
            } else {
                int pad;

                RAND_bytes(out, 16);
                len += 16;
                aad[11] = len >> 8;
                aad[12] = len;
                pad = EVP_CIPHER_CTX_ctrl(&ctx,
                                          EVP_CTRL_AEAD_TLS1_AAD,
                                          EVP_AEAD_TLS1_AAD_LEN, aad);
                EVP_Cipher(&ctx, out, inp, len + pad);
            }
        }
        d = Time_F(STOP);
        BIO_printf(bio_err, mr ? "+R:%d:%s:%f\n"
                   : "%d %s's in %.2fs\n", count, "evp", d);
        results[D_EVP][j] = ((double)count) / d * mblengths[j];
    }

    if (mr) {
        fprintf(stdout, "+H");
        for (j = 0; j < num; j++)
            fprintf(stdout, ":%d", mblengths[j]);
        fprintf(stdout, "\n");
        fprintf(stdout, "+F:%d:%s", D_EVP, alg_name);
        for (j = 0; j < num; j++)
            fprintf(stdout, ":%.2f", results[D_EVP][j]);
        fprintf(stdout, "\n");
    } else {
        fprintf(stdout,
                "The 'numbers' are in 1000s of bytes per second processed.\n");
        fprintf(stdout, "type                    ");
        for (j = 0; j < num; j++)
            fprintf(stdout, "%7d bytes", mblengths[j]);
        fprintf(stdout, "\n");
        fprintf(stdout, "%-24s", alg_name);

        for (j = 0; j < num; j++) {
            if (results[D_EVP][j] > 10000)
                fprintf(stdout, " %11.2fk", results[D_EVP][j] / 1e3);
            else
                fprintf(stdout, " %11.2f ", results[D_EVP][j]);
        }
        fprintf(stdout, "\n");
    }

    OPENSSL_free(inp);
    OPENSSL_free(out);
}
