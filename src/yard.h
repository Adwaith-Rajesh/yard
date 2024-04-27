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

typedef enum {
    R_ERROR = 0,
    R_INT,
    R_FLOAT,
    R_STR,
    R_HELP
} result_t;

// store the result of a command
typedef struct {
    // 1 on success, 0 on error
    int ok;

    // type
    result_t result_type;

    // used to store error messages or help messages
    // or anything like that
    Str *emsg;

    // all the types that we support rn
    Str *_str;     // store the string result
    int _int;      // store the int result
    float _float;  // store the float result
} CmdResult;

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
#define CMD_DC_FROM_MCTX(MCTX, I) create_cmd_fn_container(I, (MCTX)->allocator, (MCTX)->deallocator)

// create the master context
YardMasterCtx *mctx_create(void *(*allocator)(size_t), void (*deallocator)(void *), void *(*reallocator)(void *, size_t));

// free the master context
void mctx_free(YardMasterCtx *mctx);

#endif
