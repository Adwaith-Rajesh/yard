#ifndef _YARD_H
#define _YARD_H

#include <stddef.h>

#include "core/ds/dc.h"
#include "core/ds/list.h"
#include "core/ds/map.h"
#include "core/ds/str.h"

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

// stores all the maps, sets and list created dusting a session
typedef struct {
    void *(*allocator)(size_t);
    void (*deallocator)(void *);
    void *(*reallocator)(void *, size_t);

    // map used to direct KV pairs
    // set key val
    // get key
    Map *_default_map;

    // map user to set user created maps
    // setz mapname key val
    // getz mapname key
    Map *_user_maps;

    // list used to directly add value to
    // pushl val
    // popr
    List *_default_list;

    // named lists created by users
    // pushlz list_name val
    // popr list_name
    Map *_user_list;  // Map[str, List]

} YardMasterCtx;

// macros that uses the allocators in mctx to create the DS's

void _container_print_list(ListNode *node);
void _container_print_map(MapEntry *entry);
void _container_free(void *data);

#define LIST_FROM_MCTX(MCTX) list_create((MCTX)->allocator, (MCTX)->deallocator, _container_free, _container_print_list)
#define MAP_FROM_MCTX(MCTX) map_create(0, (MCTX)->allocator, (MCTX)->deallocator, _container_free, _container_print_map)
#define STR_FROM_MCTX(MCTX) str_create((MCTX)->allocator, (MCTX)->deallocator, (MCTX)->reallocator)

// container section
#define INT_DC_FROM_MCTX(MCTX, I) create_int_container(I, (MCTX)->allocator, (MCTX)->deallocator)
#define FLOAT_DC_FROM_MCTX(MCTX, I) create_float_container(I, (MCTX)->allocator, (MCTX)->deallocator)
#define STR_DC_FROM_MCTX(MCTX, I) create_str_container(I, (MCTX)->allocator, (MCTX)->deallocator)

// create the master context
YardMasterCtx *mctx_create(void *(*allocator)(size_t), void (*deallocator)(void *), void *(*reallocator)(void *, size_t));

// free the master context
void mctx_free(YardMasterCtx *mctx);

#endif
