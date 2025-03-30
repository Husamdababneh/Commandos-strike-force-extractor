/* ========================================================================
   $File: base_arena.cpp
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
   ========================================================================*/


#include "base.h"
#include "base_arena.h"

#include "base_platform.h"


constexpr
ArenaParams createArenaParams(ArenaFlags flags   = ArenaFlag_None,
                              size64 resSize     = arena_default_reserve_size,
                              size64 cmtSize     = arena_default_commit_size,
                              vptr backingBuffer = nullptr) {
    ArenaParams result;
    result.flags                   = flags;
    result.reserve_size            = resSize;
    result.commit_size             = cmtSize;
    result.optional_backing_buffer = backingBuffer;
    return result;
}

constexpr global ArenaParams defaultArenaParams = createArenaParams();

proc
Arena * arena_alloc_(const ArenaParams* params)
{
    SystemInfo systemInfo = getSystemInfo();
    
    u64 reserve_size = params->reserve_size;
    u64 commit_size = params->commit_size;
    if(params->flags & ArenaFlag_LargePages)
    {
        // TODO: Large page size
        // reserve_size = AlignPow2(reserve_size, systemInfo.pageSize);
        // commit_size  = AlignPow2(commit_size,  systemInfo.pageSize);
    }
    else
    {
        reserve_size = alignPow2(reserve_size, (u64)systemInfo.pageSize);
        commit_size  = alignPow2(commit_size,  (u64)systemInfo.pageSize);
    }
  
    // rjf: reserve/commit initial block
    void *base = params->optional_backing_buffer;
    if(base == 0)
    {
        if(params->flags & ArenaFlag_LargePages)
        {
            // TODO: Panic here
            // base = os_reserve_large(reserve_size);
            // os_commit_large(base, commit_size);
        }
        else
        {
            base = reserve_memory(reserve_size);
            commit_memory(base, commit_size);
        }
    }
  
    // TODO: Handle on arena creation failure

  
    // rjf: extract arena header & fill
    Arena *arena = (Arena *)base;
    arena->current = arena;
    arena->flags = params->flags;
    arena->cmt_size = params->commit_size;
    arena->res_size = params->reserve_size;
    arena->base_pos = 0;
    arena->pos = ARENA_HEADER_SIZE;
    arena->cmt = commit_size;
    arena->res = reserve_size;
#if ARENA_FREE_LIST
    arena->free_size = 0;
    arena->free_last = 0;
#endif
    // TODO: Make this possible 
    // AsanPoisonMemoryRegion(base, commit_size);
    // AsanUnpoisonMemoryRegion(base, ARENA_HEADER_SIZE);
    return arena;
}

proc inline
void arena_release(Arena *arena)
{
    for(Arena *n = arena->current, *prev = 0; n != 0; n = prev)
    {
        prev = n->prev;
        free_memory(n, n->res);
    }
}

#pragma warning(push)
#pragma warning(disable: 5045)
proc inline
vptr arena_push(Arena *arena, u64 size, u64 align)
{
    Arena *current = arena->current;
    u64 pos_pre = alignPow2(current->pos, align);
    u64 pos_pst = pos_pre + size;
  
    // rjf: chain, if needed
    if(current->res < pos_pst && !(arena->flags & ArenaFlag_NoChain))
    {
        Arena* new_block = 0;
    
#if ARENA_FREE_LIST
        Arena *prev_block;
        for(new_block = arena->free_last,
                prev_block = 0;
            new_block != 0;
            prev_block = new_block,
                new_block = new_block->prev)
        {
            if(new_block->res >= AlignPow2(size, align))
            {
                if(prev_block)
                {
                    prev_block->prev = new_block->prev;
                }
                else
                {
                    arena->free_last = new_block->prev;
                }
                arena->free_size -= new_block->res_size;
                AsanUnpoisonMemoryRegion((U8*)new_block + ARENA_HEADER_SIZE,
                                         new_block->res_size - ARENA_HEADER_SIZE);
                break;
            }
        }
#endif
    
        if(new_block == 0)
        {
            u64 res_size = current->res_size;
            u64 cmt_size = current->cmt_size;
            if(size + ARENA_HEADER_SIZE > res_size)
            {
                res_size = alignPow2(size + ARENA_HEADER_SIZE, align);
                cmt_size = alignPow2(size + ARENA_HEADER_SIZE, align);
            }

            ArenaParams params = {
                .flags        = current->flags,
                .reserve_size = res_size,
                .commit_size  = cmt_size,
            };
            new_block = arena_alloc_(&params);
        }
    
        new_block->base_pos = current->base_pos + current->res;
        SLLStackPush_N(arena->current, new_block, prev);
    
        current = new_block;
        pos_pre = alignPow2(current->pos, align);
        pos_pst = pos_pre + size;
    }
  
    // rjf: commit new pages, if needed
    if(current->cmt < pos_pst)
    {
        u64 cmt_pst_aligned = pos_pst + current->cmt_size-1;
        cmt_pst_aligned -= cmt_pst_aligned%current->cmt_size;
        u64 cmt_pst_clamped = clampTop(cmt_pst_aligned, current->res);
        u64 cmt_size = cmt_pst_clamped - current->cmt;
        u8 *cmt_ptr = (u8 *)current + current->cmt;
        if(current->flags & ArenaFlag_LargePages)
        {
            // TODO: Large Pages
            // os_commit_large(cmt_ptr, cmt_size);
        }
        else
        {
            platform_commit_memory(cmt_ptr, cmt_size);
        }
        current->cmt = cmt_pst_clamped;
    }
  
    // rjf: push onto current block
    void *result = 0;
    if(current->cmt >= pos_pst)
    {
        result = (u8 *)current+pos_pre;
        current->pos = pos_pst;
        // AsanUnpoisonMemoryRegion(result, size);
    }
  
    // handle failure
  
    return result;
}
#pragma warning(pop) // Re-enable C5045


proc inline
u64 arena_pos(Arena *arena)
{
    Arena *current = arena->current;
    u64 pos = current->base_pos + current->pos;
    return pos;
}

proc inline
void arena_pop_to(Arena *arena, u64 pos)
{
    u64 big_pos = clampBot(ARENA_HEADER_SIZE, pos);
    Arena *current = arena->current;
  
#if ARENA_FREE_LIST
    for(Arena *prev = 0; current->base_pos >= big_pos; current = prev)
    {
        prev = current->prev;
        current->pos = ARENA_HEADER_SIZE;
        arena->free_size += current->res_size;
        SLLStackPush_N(arena->free_last, current, prev);
        AsanPoisonMemoryRegion((U8*)current + ARENA_HEADER_SIZE, current->res_size - ARENA_HEADER_SIZE);
    }
#else
    for(Arena *prev = 0; current->base_pos >= big_pos; current = prev)
    {
        prev = current->prev;
        platform_free_memory(current, current->res);
    }
#endif
    arena->current = current;
    u64 new_pos = big_pos - current->base_pos;
    // AssertAlways(new_pos <= current->pos);
    // AsanPoisonMemoryRegion((U8*)current + new_pos, (current->pos - new_pos));
    current->pos = new_pos;
}


proc inline
void arena_clear(Arena *arena)
{
  arena_pop_to(arena, 0);
}

proc inline
void arena_pop(Arena *arena, u64 amt)
{
  u64 pos_old = arena_pos(arena);
  u64 pos_new = pos_old;
  if(amt < pos_old)
  {
    pos_new = pos_old - amt;
  }
  arena_pop_to(arena, pos_new);
}

//- rjf: temporary arena scopes

proc inline
Temp temp_begin(Arena *arena)
{
  u64 pos = arena_pos(arena);
  Temp temp = {arena, pos};
  return temp;
}

internal void
temp_end(Temp temp)
{
  arena_pop_to(temp.arena, temp.pos);
}
