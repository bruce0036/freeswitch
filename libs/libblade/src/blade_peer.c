/*
 * Copyright (c) 2007-2014, Anthony Minessale II
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * * Neither the name of the original author; nor the names of any contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 * 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "blade.h"

typedef enum {
	BP_NONE = 0,
	BP_MYPOOL = (1 << 0)
} bppvt_flag_t;

struct blade_peer_s {
	bppvt_flag_t flags;
	ks_pool_t *pool;
};


KS_DECLARE(ks_status_t) blade_peer_destroy(blade_peer_t **bpP)
{
	blade_peer_t *bp = NULL;
	bppvt_flag_t flags;
	ks_pool_t *pool;

	ks_assert(bpP);

	bp = *bpP;
	*bpP = NULL;

	ks_assert(bp);

	flags = bp->flags;
	pool = bp->pool;

	ks_pool_free(bp->pool, bp);

	if (pool && (flags & BP_MYPOOL)) {
		ks_pool_close(&pool);
	}

	return KS_STATUS_SUCCESS;
}

KS_DECLARE(ks_status_t) blade_peer_create(blade_peer_t **bpP, ks_pool_t *pool)
{
	bppvt_flag_t newflags = BP_NONE;
	blade_peer_t *bp = NULL;

	if (!pool) {
		newflags |= BP_MYPOOL;
		ks_pool_open(&pool);
	}

	bp = ks_pool_alloc(pool, sizeof(*bp));
	bp->pool = pool;
	bp->flags = newflags;
	*bpP = bp;

	return KS_STATUS_SUCCESS;
}


/* For Emacs:
 * Local Variables:
 * mode:c
 * indent-tabs-mode:t
 * tab-width:4
 * c-basic-offset:4
 * End:
 * For VIM:
 * vim:set softtabstop=4 shiftwidth=4 tabstop=4 noet:
 */
