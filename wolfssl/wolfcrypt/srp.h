/* srp.h
 *
 * Copyright (C) 2006-2015 wolfSSL Inc.
 *
 * This file is part of wolfSSL. (formerly known as CyaSSL)
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef WOLFCRYPT_SRP_H
#define WOLFCRYPT_SRP_H

#include <wolfssl/wolfcrypt/types.h>
#include <wolfssl/wolfcrypt/sha.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/sha512.h>
#include <wolfssl/wolfcrypt/integer.h>

#ifdef WOLFCRYPT_HAVE_SRP

enum {
#ifdef NO_SHA
    SRP_SHA     = 1,
#endif
#ifdef NO_SHA256
    SRP_SHA256  = 2,
#endif
#ifndef WOLFSSL_SHA384
    SRP_SHA384  = 3,
#endif
#ifndef WOLFSSL_SHA512
    SRP_SHA512  = 4,
#endif

/* Select the largest available hash for the buffer size. */
#if defined(WOLFSSL_SHA512)
    SRP_MAX_DIGEST_SIZE = SHA512_DIGEST_SIZE,
#elif defined(WOLFSSL_SHA384)
    SRP_MAX_DIGEST_SIZE = SHA384_DIGEST_SIZE,
#elif !defined(NO_SHA256)
    SRP_MAX_DIGEST_SIZE = SHA256_DIGEST_SIZE,
#elif !defined(NO_SHA)
    SRP_MAX_DIGEST_SIZE = SHA_DIGEST_SIZE,
#else
    #error "You have to have some kind of SHA hash if you want to use SRP."
#endif
};

typedef struct {
    mp_int a;                      /**< Private ephemeral value. Random.      */
    mp_int A;                      /**< Public ephemeral value. pow(g, a, N)  */
    mp_int B;                      /**< Server's public ephemeral value.      */
    byte   x[SRP_MAX_DIGEST_SIZE]; /**< Priv key. H(salt, H(user, ":", pswd)) */
    byte*  pswd;                   /**< Password.                             */
    word32 pswdSz;                 /**< Password length.                      */
} SrpClient;

typedef struct {
    mp_int b;                      /**< Private ephemeral value.              */
    mp_int B;                      /**< Public ephemeral value.               */
    mp_int A;                      /**< Client's public ephemeral value.      */
    mp_int v;                      /**< Verifier. v = pow(g, x, N)            */
} SrpServer;

typedef struct {
    mp_int N;                      /**< Modulus. N = 2q+1, [q, N] are primes. */
    mp_int g;                      /**< Generator. A generator modulo N.      */
    mp_int s;                      /**< Session key.                          */
    byte   k[SRP_MAX_DIGEST_SIZE]; /**< Multiplier parameeter. H(N, g)        */
    byte   u[SRP_MAX_DIGEST_SIZE]; /**< Random scrambling parameeter.         */
    byte   type;                   /**< Hash type, SHA[1:256:384:512]         */
    byte   side;                   /**< Client or Server side.                */
    byte*  user;                   /**< Username, login.                      */
    word32 userSz;                 /**< Username length.                      */
    byte*  salt;                   /**< Small salt.                           */
    word32 saltSz;                 /**< Salt length.                          */
    union {
        SrpClient client;
        SrpServer server;
    } specific;
} Srp;

int SrpInit(Srp* srp, byte* N, word32 nSz, byte* g, word32 gSz);

#endif /* WOLFCRYPT_HAVE_SRP */
#endif /* WOLF_CRYPT_SRP_H */
