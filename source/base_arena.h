/* ========================================================================
   $File: base_arena.h
   $Date: 2025-03-14
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/


#ifndef BASE_ARENA_H
#define BASE_ARENA_H


#define ARENA_HEADER_SIZE 128

enum ArenaFlags : u64
{
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
  U64 free_size;
  Arena *free_last;
#endif
};


#endif BASE_ARENA_H
