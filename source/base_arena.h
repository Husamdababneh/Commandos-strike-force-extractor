/* ========================================================================
   $File: base_arena.h
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/


#ifndef BASE_ARENA_H
#define BASE_ARENA_H


// This is heavily copy&pasted from https://github.com/EpicGamesExt/raddebugger/blob/master/src/base/base_arena.h
// Shoutout to ryanfleury

enum ArenaFlags : u64;

constexpr u64 ARENA_HEADER_SIZE                 =  128;
constexpr global u64 arena_default_reserve_size = MB(64);
constexpr global u64 arena_default_commit_size  = KB(64);
constexpr global ArenaFlags arena_default_flags = (ArenaFlags)0;

enum ArenaFlags : u64
{
    ArenaFlag_None       = 0,
    ArenaFlag_NoChain    = (1<<0),
    ArenaFlag_LargePages = (1<<1),
};

typedef struct ArenaParams ArenaParams;
struct ArenaParams
{
  ArenaFlags flags;
  size64     reserve_size;
  size64     commit_size;
  vptr       optional_backing_buffer;
};

typedef struct Arena Arena;
struct Arena
{
  Arena *prev;    // previous arena in chain
  Arena *current; // current arena in chain
  ArenaFlags flags;
  u64 cmt_size;
  u64 res_size;
  u64 base_pos;
  u64 pos;
  u64 cmt;
  u64 res;
#if ARENA_FREE_LIST
  u64 free_size;
  Arena *free_last;
#endif
};

static_assert(sizeof(Arena) <= ARENA_HEADER_SIZE, "Arena Size must be 128");

typedef struct Temp Temp;
struct Temp
{
  Arena *arena;
  u64 pos;
};

// global ArenaParams defaultArenaParams;
////////////////////////////////
//~ rjf: Arena Functions

//- rjf: arena creation/destruction
internal Arena* arena_alloc_(const ArenaParams *params);
#define arena_alloc(...) arena_alloc_(&(ArenaParams){.reserve_size = arena_default_reserve_size, .commit_size = arena_default_commit_size, .flags = arena_default_flags, __VA_ARGS__})
internal void arena_release(Arena *arena);

//- rjf: arena push/pop/pos core functions
internal void *arena_push(Arena *arena, u64 size, u64 align);
internal u64   arena_pos(Arena *arena);
internal void  arena_pop_to(Arena *arena, u64 pos);

//- rjf: arena push/pop helpers
internal void arena_clear(Arena *arena);
internal void arena_pop(Arena *arena, u64 amt);

//- rjf: temporary arena scopes
internal Temp temp_begin(Arena *arena);
internal void temp_end(Temp temp);

//- rjf: push helper macros
#define push_array_no_zero_aligned(a, T, c, align) (T *)arena_push((a), sizeof(T)*(c), (align))
#define push_array_aligned(a, T, c, align) (T *)MemoryZero(push_array_no_zero_aligned(a, T, c, align), sizeof(T)*(c))
#define push_array_no_zero(a, T, c) push_array_no_zero_aligned(a, T, c, max(8, alignOf(T)))
#define push_array(a, T, c) push_array_aligned(a, T, c, max(8, alignOf(T)))


#endif BASE_ARENA_H
