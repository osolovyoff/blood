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
 * Copyright (c) 1998-2006 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/bio.h>
#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <openssl/lhash.h>
#include <openssl/conf.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#ifndef OPENSSL_NO_ENGINE
# include <openssl/engine.h>
#endif
#include <openssl/err.h>
#ifdef OPENSSL_FIPS
# include <openssl/fips.h>
#endif
#define USE_SOCKETS /* needed for the _O_BINARY defs in the MS world */
#include "s_apps.h"
/* Needed to get the other O_xxx flags. */
#ifdef OPENSSL_SYS_VMS
# include <unixio.h>
#endif
#define INCLUDE_FUNCTION_TABLE
#include "apps.h"


#ifdef OPENSSL_NO_CAMELLIA
# define FORMAT "%-15s"
# define COLUMNS 5
#else
# define FORMAT "%-18s"
# define COLUMNS 4
#endif

/* Special sentinel to exit the program. */
#define EXIT_THE_PROGRAM (-1)

/*
 * The LHASH callbacks ("hash" & "cmp") have been replaced by functions with
 * the base prototypes (we cast each variable inside the function to the
 * required type of "FUNCTION*"). This removes the necessity for
 * macro-generated wrapper functions.
 */
DECLARE_LHASH_OF(FUNCTION);
static LHASH_OF(FUNCTION) *prog_init(void);
static int do_cmd(LHASH_OF(FUNCTION) *prog, int argc, char *argv[]);
static void list_pkey(void);
static void list_type(FUNC_TYPE ft);
char *default_config_file = NULL;

CONF *config = NULL;
BIO *bio_in = NULL;
BIO *bio_out = NULL;
BIO *bio_err = NULL;

static void apps_startup()
{
#ifdef SIGPIPE
    signal(SIGPIPE, SIG_IGN);
#endif
    CRYPTO_malloc_init();
    ERR_load_crypto_strings();
    ERR_load_SSL_strings();
    OpenSSL_add_all_algorithms();
    OpenSSL_add_ssl_algorithms();
    OPENSSL_load_builtin_modules();
    setup_ui_method();
    /*SSL_library_init();*/
#ifndef OPENSSL_NO_ENGINE
    ENGINE_load_builtin_engines();
#endif
}

static void apps_shutdown()
{
#ifndef OPENSSL_NO_ENGINE
    ENGINE_cleanup();
#endif
    destroy_ui_method();
    CONF_modules_unload(1);
#ifndef OPENSSL_NO_COMP
    COMP_zlib_cleanup();
    SSL_COMP_free_compression_methods();
#endif
    OBJ_cleanup();
    EVP_cleanup();
    CRYPTO_cleanup_all_ex_data();
    ERR_remove_thread_state(NULL);
    RAND_cleanup();
    ERR_free_strings();
}

static char *make_config_name()
{
    const char *t;
    size_t len;
    char *p;

    if ((t = getenv("OPENSSL_CONF")) != NULL
        || (t = getenv("SSLEAY_CONF")) != NULL)
        return BUF_strdup(t);

    t = X509_get_default_cert_area();
    len = strlen(t) + 1 + strlen(OPENSSL_CONF) + 1;
    p = app_malloc(len, "config filename buffer");
    strcpy(p, t);
#ifndef OPENSSL_SYS_VMS
    strcat(p, "/");
#endif
    strcat(p, OPENSSL_CONF);

    return p;
}

static void lock_dbg_cb(int mode, int type, const char *file, int line)
{
    static int modes[CRYPTO_NUM_LOCKS];
    const char *errstr = NULL;
    int rw = mode & (CRYPTO_READ | CRYPTO_WRITE);

    if (rw != CRYPTO_READ && rw != CRYPTO_WRITE) {
        errstr = "invalid mode";
        goto err;
    }

    if (type < 0 || type >= CRYPTO_NUM_LOCKS) {
        errstr = "type out of bounds";
        goto err;
    }

    if (mode & CRYPTO_LOCK) {
        if (modes[type]) {
            errstr = "already locked";
            /* must not happen in a single-threaded program --> deadlock! */
            goto err;
        }
        modes[type] = rw;
    } else if (mode & CRYPTO_UNLOCK) {
        if (!modes[type]) {
            errstr = "not locked";
            goto err;
        }

        if (modes[type] != rw) {
            errstr = (rw == CRYPTO_READ) ?
                "CRYPTO_r_unlock on write lock" :
                "CRYPTO_w_unlock on read lock";
        }

        modes[type] = 0;
    } else {
        errstr = "invalid mode";
        goto err;
    }

 err:
    if (errstr) {
        BIO_printf(bio_err,
                   "openssl (lock_dbg_cb): %s (mode=%d, type=%d) at %s:%d\n",
                   errstr, mode, type, file, line);
    }
}

#if defined( OPENSSL_SYS_VMS)
extern char **copy_argv(int *argc, char **argv);
#endif

int main(int argc, char *argv[])
{
    FUNCTION f, *fp;
    LHASH_OF(FUNCTION) *prog = NULL;
    char **copied_argv = NULL;
    char *p, *pname;
    char buf[1024];
    const char *prompt;
    ARGS arg;
    int first, n, i, ret = 0;

    arg.argv = NULL;
    arg.size = 0;

    /* Set up some of the environment. */
    default_config_file = make_config_name();
    bio_in = dup_bio_in(FORMAT_TEXT);
    bio_out = dup_bio_out(FORMAT_TEXT);
    bio_err = BIO_new_fp(stderr, BIO_NOCLOSE | BIO_FP_TEXT);

#if defined( OPENSSL_SYS_VMS)
    copied_argv = argv = copy_argv(&argc, argv);
#endif

    p = getenv("OPENSSL_DEBUG_MEMORY");
    if (p == NULL)
        /* if not set, use compiled-in default */
        ;
    else if (strcmp(p, "off") != 0) {
        CRYPTO_malloc_debug_init();
        CRYPTO_set_mem_debug_options(V_CRYPTO_MDEBUG_ALL);
    } else {
        CRYPTO_set_mem_debug_functions(0, 0, 0, 0, 0);
    }
    CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
    CRYPTO_set_locking_callback(lock_dbg_cb);

    if (getenv("OPENSSL_FIPS")) {
#ifdef OPENSSL_FIPS
        if (!FIPS_mode_set(1)) {
            ERR_load_crypto_strings();
            ERR_print_errors(bio_err);
            return 1;
        }
#else
        BIO_printf(bio_err, "FIPS mode not supported.\n");
        return 1;
#endif
    }

    apps_startup();

    /*
     * If first argument is a colon, skip it.  Because in "interactive"
     * mode our prompt is a colon and we can cut/paste whole lines
     * by doing this hack.
     */
    if (argv[1] && strcmp(argv[1], ":") == 0) {
        argv[1] = argv[0];
        argc--;
        argv++;
    }
    prog = prog_init();
    pname = opt_progname(argv[0]);

    /* first check the program name */
    f.name = pname;
    fp = lh_FUNCTION_retrieve(prog, &f);
    if (fp != NULL) {
        argv[0] = pname;
        ret = fp->func(argc, argv);
        goto end;
    }

    /* If there is stuff on the command line, run with that. */
    if (argc != 1) {
        argc--;
        argv++;
        ret = do_cmd(prog, argc, argv);
        if (ret < 0)
            ret = 0;
        goto end;
    }

    /* ok, lets enter interactive mode */
    for (;;) {
        ret = 0;
        /* Read a line, continue reading if line ends with \ */
        for (p = buf, n = sizeof buf, i = 0, first = 1; n > 0; first = 0) {
            prompt = first ? "openssl : " : "> ";
            p[0] = '\0';
#ifndef READLINE
            fputs(prompt, stdout);
            fflush(stdout);
            if (!fgets(p, n, stdin))
                goto end;
            if (p[0] == '\0')
                goto end;
            i = strlen(p);
            if (i <= 1)
                break;
            if (p[i - 2] != '\\')
                break;
            i -= 2;
            p += i;
            n -= i;
#else
            {
                extern char *readline(const char *);
                extern void add_history(const char *cp);
                char *text;

                char *text = readline(prompt);
                if (text == NULL)
                    goto end;
                i = strlen(text);
                if (i == 0 || i > n)
                    break;
                if (text[i - 1] != '\\') {
                    p += strlen(strcpy(p, text));
                    free(text);
                    add_history(buf);
                    break;
                }

                text[i - 1] = '\0';
                p += strlen(strcpy(p, text));
                free(text);
                n -= i;
            }
#endif
        }

        if (!chopup_args(&arg, buf)) {
            BIO_printf(bio_err, "Can't parse (no memory?)\n");
            break;
        }

        ret = do_cmd(prog, arg.argc, arg.argv);
        if (ret == EXIT_THE_PROGRAM) {
            ret = 0;
            goto end;
        }
        if (ret != 0)
            BIO_printf(bio_err, "error in %s\n", arg.argv[0]);
        (void)BIO_flush(bio_out);
        (void)BIO_flush(bio_err);
    }
    ret = 1;
 end:
    OPENSSL_free(copied_argv);
    OPENSSL_free(default_config_file);
    NCONF_free(config);
    config = NULL;
    lh_FUNCTION_free(prog);
    OPENSSL_free(arg.argv);

    BIO_free(bio_in);
    BIO_free_all(bio_out);
    apps_shutdown();
    CRYPTO_mem_leaks(bio_err);
    BIO_free(bio_err);
    return (ret);
}

OPTIONS exit_options[] = {
    {NULL}
};

static void list_cipher_fn(const EVP_CIPHER *c,
                           const char *from, const char *to, void *arg)
{
    if (c)
        BIO_printf(arg, "%s\n", EVP_CIPHER_name(c));
    else {
        if (!from)
            from = "<undefined>";
        if (!to)
            to = "<undefined>";
        BIO_printf(arg, "%s => %s\n", from, to);
    }
}

static void list_md_fn(const EVP_MD *m,
                       const char *from, const char *to, void *arg)
{
    if (m)
        BIO_printf(arg, "%s\n", EVP_MD_name(m));
    else {
        if (!from)
            from = "<undefined>";
        if (!to)
            to = "<undefined>";
        BIO_printf((BIO *)arg, "%s => %s\n", from, to);
    }
}

/* Unified enum for help and list commands. */
typedef enum HELPLIST_CHOICE {
    OPT_ERR = -1, OPT_EOF = 0, OPT_HELP,
    OPT_COMMANDS, OPT_DIGEST_COMMANDS,
    OPT_DIGEST_ALGORITHMS, OPT_CIPHER_COMMANDS, OPT_CIPHER_ALGORITHMS,
    OPT_PK_ALGORITHMS
} HELPLIST_CHOICE;

OPTIONS list_options[] = {
    {"help", OPT_HELP, '-', "Display this summary"},
    {"commands", OPT_COMMANDS, '-', "List of standard commands"},
    {"digest-commands", OPT_DIGEST_COMMANDS, '-',
     "List of message digest commands"},
    {"digest-algorithms", OPT_DIGEST_ALGORITHMS, '-',
     "List of message digest algorithms"},
    {"cipher-commands", OPT_CIPHER_COMMANDS, '-', "List of cipher commands"},
    {"cipher-algorithms", OPT_CIPHER_ALGORITHMS, '-',
     "List of cipher algorithms"},
    {"public-key-algorithms", OPT_PK_ALGORITHMS, '-',
     "List of public key algorithms"},
    {NULL}
};

int list_main(int argc, char **argv)
{
    char *prog;
    HELPLIST_CHOICE o;

    prog = opt_init(argc, argv, list_options);
    while ((o = opt_next()) != OPT_EOF) {
        switch (o) {
        case OPT_EOF:
        case OPT_ERR:
            BIO_printf(bio_err, "%s: Use -help for summary.\n", prog);
            return 1;
        case OPT_HELP:
            opt_help(list_options);
            break;
        case OPT_COMMANDS:
            list_type(FT_general);
            break;
        case OPT_DIGEST_COMMANDS:
            list_type(FT_md);
            break;
        case OPT_DIGEST_ALGORITHMS:
            EVP_MD_do_all_sorted(list_md_fn, bio_out);
            break;
        case OPT_CIPHER_COMMANDS:
            list_type(FT_cipher);
            break;
        case OPT_CIPHER_ALGORITHMS:
            EVP_CIPHER_do_all_sorted(list_cipher_fn, bio_out);
            break;
        case OPT_PK_ALGORITHMS:
            list_pkey();
            break;
        }
    }

    return 0;
}

OPTIONS help_options[] = {
    {"help", OPT_HELP, '-', "Display this summary"},
    {NULL}
};

int help_main(int argc, char **argv)
{
    FUNCTION *fp;
    int i, nl;
    FUNC_TYPE tp;
    char *prog;
    HELPLIST_CHOICE o;

    prog = opt_init(argc, argv, help_options);
    while ((o = opt_next()) != OPT_EOF) {
        switch (o) {
        default:
            BIO_printf(bio_err, "%s: Use -help for summary.\n", prog);
            return 1;
        case OPT_HELP:
            opt_help(help_options);
            return 0;
        }
    }
    argc = opt_num_rest();
    argv = opt_rest();

    if (argc != 0) {
        BIO_printf(bio_err, "Usage: %s\n", prog);
        return 1;
    }

    BIO_printf(bio_err, "\nStandard commands");
    i = 0;
    tp = FT_none;
    for (fp = functions; fp->name != NULL; fp++) {
        nl = 0;
        if (((i++) % COLUMNS) == 0) {
            BIO_printf(bio_err, "\n");
            nl = 1;
        }
        if (fp->type != tp) {
            tp = fp->type;
            if (!nl)
                BIO_printf(bio_err, "\n");
            if (tp == FT_md) {
                i = 1;
                BIO_printf(bio_err,
                           "\nMessage Digest commands (see the `dgst' command for more details)\n");
            } else if (tp == FT_cipher) {
                i = 1;
                BIO_printf(bio_err,
                           "\nCipher commands (see the `enc' command for more details)\n");
            }
        }
        BIO_printf(bio_err, FORMAT, fp->name);
    }
    BIO_printf(bio_err, "\n\n");
    return 0;
}

int exit_main(int argc, char **argv)
{
    return EXIT_THE_PROGRAM;
}

static void list_type(FUNC_TYPE ft)
{
    FUNCTION *fp;
    int i = 0;

    for (fp = functions; fp->name != NULL; fp++)
        if (fp->type == ft) {
            if ((i++ % COLUMNS) == 0)
                BIO_printf(bio_out, "\n");
            BIO_printf(bio_out, FORMAT, fp->name);
        }
    BIO_printf(bio_out, "\n");
}

static int do_cmd(LHASH_OF(FUNCTION) *prog, int argc, char *argv[])
{
    FUNCTION f, *fp;

    if (argc <= 0 || argv[0] == NULL)
        return (0);
    f.name = argv[0];
    fp = lh_FUNCTION_retrieve(prog, &f);
    if (fp == NULL) {
        if (EVP_get_digestbyname(argv[0])) {
            f.type = FT_md;
            f.func = dgst_main;
            fp = &f;
        } else if (EVP_get_cipherbyname(argv[0])) {
            f.type = FT_cipher;
            f.func = enc_main;
            fp = &f;
        }
    }
    if (fp != NULL) {
        return (fp->func(argc, argv));
    }
    if ((strncmp(argv[0], "no-", 3)) == 0) {
        /*
         * User is asking if foo is unsupported, by trying to "run" the
         * no-foo command.  Strange.
         */
        f.name = argv[0] + 3;
        if (lh_FUNCTION_retrieve(prog, &f) == NULL) {
            BIO_printf(bio_out, "%s\n", argv[0]);
            return (0);
        }
        BIO_printf(bio_out, "%s\n", argv[0] + 3);
        return 1;
    }
    if (strcmp(argv[0], "quit") == 0 || strcmp(argv[0], "q") == 0 ||
        strcmp(argv[0], "exit") == 0 || strcmp(argv[0], "bye") == 0)
        /* Special value to mean "exit the program. */
        return EXIT_THE_PROGRAM;

    BIO_printf(bio_err, "Invalid command '%s'; type \"help\" for a list.\n",
               argv[0]);
    return (1);
}

static void list_pkey(void)
{
    int i;

    for (i = 0; i < EVP_PKEY_asn1_get_count(); i++) {
        const EVP_PKEY_ASN1_METHOD *ameth;
        int pkey_id, pkey_base_id, pkey_flags;
        const char *pinfo, *pem_str;
        ameth = EVP_PKEY_asn1_get0(i);
        EVP_PKEY_asn1_get0_info(&pkey_id, &pkey_base_id, &pkey_flags,
                                &pinfo, &pem_str, ameth);
        if (pkey_flags & ASN1_PKEY_ALIAS) {
            BIO_printf(bio_out, "Name: %s\n", OBJ_nid2ln(pkey_id));
            BIO_printf(bio_out, "\tAlias for: %s\n",
                       OBJ_nid2ln(pkey_base_id));
        } else {
            BIO_printf(bio_out, "Name: %s\n", pinfo);
            BIO_printf(bio_out, "\tType: %s Algorithm\n",
                       pkey_flags & ASN1_PKEY_DYNAMIC ?
                       "External" : "Builtin");
            BIO_printf(bio_out, "\tOID: %s\n", OBJ_nid2ln(pkey_id));
            if (pem_str == NULL)
                pem_str = "(none)";
            BIO_printf(bio_out, "\tPEM string: %s\n", pem_str);
        }

    }
}

static int function_cmp(const FUNCTION * a, const FUNCTION * b)
{
    return strncmp(a->name, b->name, 8);
}

static IMPLEMENT_LHASH_COMP_FN(function, FUNCTION)

static unsigned long function_hash(const FUNCTION * a)
{
    return lh_strhash(a->name);
}

static IMPLEMENT_LHASH_HASH_FN(function, FUNCTION)

static int SortFnByName(const void *_f1, const void *_f2)
{
    const FUNCTION *f1 = _f1;
    const FUNCTION *f2 = _f2;

    if (f1->type != f2->type)
        return f1->type - f2->type;
    return strcmp(f1->name, f2->name);
}

static LHASH_OF(FUNCTION) *prog_init(void)
{
    LHASH_OF(FUNCTION) *ret;
    FUNCTION *f;
    size_t i;

    /* Sort alphabetically within category. For nicer help displays. */
    for (i = 0, f = functions; f->name != NULL; ++f, ++i) ;
    qsort(functions, i, sizeof(*functions), SortFnByName);

    if ((ret = lh_FUNCTION_new()) == NULL)
        return (NULL);

    for (f = functions; f->name != NULL; f++)
        (void)lh_FUNCTION_insert(ret, f);
    return (ret);
}
