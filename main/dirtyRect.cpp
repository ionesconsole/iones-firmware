#include "dirtyRect.h"
#include "framebuffer.h"

// ----- HELPER FUNCTIONS -----

void fb_clip_rect(FBRect &r){
    if (r.x0 < 0)
    {
        r.x0 = 0;
    }
    if (r.x1 > fb.width)
    {
        r.x1 = fb.width;
    }
    if (r.y0 < 0)
    {
        r.y0 = 0;
    }
    if (r.y1 > fb.height)
    {
        r.y1 = fb.height;
    }
}

bool fb_rect_is_valid(const FBRect& r) {
    return r.x0 < r.x1 && r.y0 < r.y1;
}

bool fb_has_dirty(){
    return fb.dirty.count;
}

bool fb_dirty_contains(FBRect r, FBRect z) {
    if (r.x0 <= z.x0 && r.y0 <= z.y0) {
        if (r.x1 >= z.x1 && r.y1 >= z.y1) {
            return true;
        }
    }

    return false;
}

bool fb_dirty_intersect(FBRect r, FBRect z) {
    if (r.x0 >= z.x1) {
        return false; 
    }

    if (r.x1 <= z.x0) {
        return false; 
    }

    if (r.y0 >= z.y1) {
        return false; 
    }

    if (r.y1 <= z.y0) {
        return false; 
    }

    return true;
}

FBRect fb_dirty_union(FBRect r, FBRect z){
    if (fb_dirty_intersect(r, z))
    {
        Serial.printf("Intersection makes sense!");
        FBRect newRect;

        newRect.x0 = min(r.x0,z.x0);
        newRect.y0 = min(r.y0,z.y0);
        newRect.x1 = max(r.x1,z.x1);
        newRect.y1 = min(r.y1,z.y1);

        return newRect;
    }
    Serial.printf("Intersection does not make sense!");
    return;

}

bool fb_is_full_dirty(const DirtyList& dl) {
    if (dl.count == 1) {
        if (dl.d_list[0].x0 == 0 && dl.d_list[0].y0 == 0) {
            if (dl.d_list[0].x1 == fb.width && dl.d_list[0].y1 == fb.height) {
                return true;
            }
        }
    }

    return false;
}

// ----- MAIN FUNCTIONS -----

void fb_clear_dirty(){
    fb.dirty.count = 0;
    //reinitlize????
}

void fb_mark_full_dirty(){
    // case where the whole screen is one rect
    fb.dirty.count = 1;
    fb.dirty.d_list[0] = {0,0,fb.width,fb.height};
}

void fb_mark_dirty_wrapper(int x, int y, int x1, int y1){
    //if rectangulation needed add code here, make the wrapper do it
    //fb_mark_dirty(int x, int y, int x1, int y1);
}

void fb_mark_dirty(FBRect r){
    fb_clip_rect(r);

    if(!fb_rect_is_valid(r)){
        Serial.printf("Rect is not valid!");
        return;
    }

    if(fb_is_full_dirty(fb.dirty)){
        return;
    }

    for (int i = 0; i < fb.dirty.count; i++)
    {
        if (fb_dirty_contains(r,fb.dirty.d_list[i]))
        {
            fb.dirty.d_list[i] = r;
            return;
        }
        if (fb_dirty_contains(fb.dirty.d_list[i],r))
        {
            return;
        }

        if (fb_dirty_intersect(r,fb.dirty.d_list[i]))
        {
            fb.dirty.d_list[i] = fb_dirty_union(r,fb.dirty.d_list[i]);
            return;
        }
    
    }
    
    if (fb.dirty.count < MAX_RECTS) {
        fb.dirty.d_list[fb.dirty.count] = r;
        fb.dirty.count++;
        return;
    }

    fb_mark_full_dirty();

}

/*int fb_dirty_area() {
    int area = 0;

    for (int i = 0; i < fb.dirty.count; i++) {
        area += fb_rect_area(fb.dirty.d_list[i]);
    }

    return area;
}*/