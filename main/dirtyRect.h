#pragma once
#include "globals.h"
#include <Arduino.h>
#include <stdint.h>

#define MAX_RECTS 16

struct FBRect {
    int x0;
    int y0;
    int x1;//width added should be exclusive
    int y1;//height added should be exclusive
};


struct FBDirtyRect {
    bool valid; 

    int x0;
    int y0;
    int x1;
    int y1;

    FBDirtyRect()
        : valid(false),
          x0(0),
          y0(0),
          x1(0),
          y1(0)
    {
    }
};

struct DirtyList{
    FBDirtyRect d_list[MAX_RECTS];
    int count;
    DirtyList()
        : count(0)
    {
    }
};




//Dirty rect main functions
void fb_mark_dirty_wrapper(int x, int y, int x1, int y1);
void fb_mark_dirty(FBRect r);
void fb_mark_full_dirty();
void fb_clear_dirty();
int fb_dirty_area();



//helper functions
void fb_clip_rect(FBRect r);
bool fb_dirty_intersect(FBRect r, FBRect z);
bool fb_dirty_contains(FBRect r, FBRect z);
//bool fb_dirty_rect_valid(FBRect r);
bool fb_has_dirty();
bool fb_rect_is_valid(FBRect rect);
FBRect fb_dirty_union(FBRect r, FBRect z);
bool fb_is_full_dirty(DirtyList dl);


