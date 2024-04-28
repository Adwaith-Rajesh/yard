#ifndef _YARD_H
#define _YARD_H

#include <stddef.h>

#include "core/ds/dc.h"
#include "core/ds/list.h"
#include "core/ds/map.h"
#include "core/ds/str.h"
#include "mctx.h"

#define LICENSE                                                                        \
    "BSD 3-Clause License\n"                                                           \
    "\n"                                                                               \
    "Copyright (c) 2024, Adwaith Rajesh <adwaithrajesh3180[at]gmail[dot]com>\n"        \
    "\n"                                                                               \
    "THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\"\n"  \
    "AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE\n"      \
    "IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n" \
    "DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE\n"   \
    "FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL\n"     \
    "DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR\n"     \
    "SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER\n"     \
    "CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,\n"  \
    "OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE\n"  \
    "OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\n\n"

// macros that uses the allocators in mctx to create the DS's

void _container_print_list(ListNode *node);
void _container_print_map(MapEntry *entry);
void _container_free(void *data);

// create the master context
YardMasterCtx *mctx_create(void *(*allocator)(size_t), void (*deallocator)(void *), void *(*reallocator)(void *, size_t));

// free the master context
void mctx_free(YardMasterCtx *mctx);

// create result
CmdResult *result_create(YardMasterCtx *mctx);

// free result
void result_free(CmdResult *res, YardMasterCtx *mctx);

#endif
