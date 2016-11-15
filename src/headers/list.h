//
// Created by Harrison on 11/15/2016.
//

#ifndef CS430_PROJ4_RAYCASTING_LIST_H
#define CS430_PROJ4_RAYCASTING_LIST_H

typedef struct {
    double value;
    void* tag;
} DblListElem;
typedef DblListElem* DblListElemRef;

typedef struct {
    DblListElem* elements;
    int length;
    int maxSize;
} DblList;
typedef DblList* DblListRef;

void dbllist_init(DblListRef list, int initialSize);
void dbllist_dispose(DblListRef list);

DblListElem dbllist_elem_at(DblListRef list, int index);
void dbllist_update(DblListRef list, int index, DblListElem value);
void dbllist_push(DblListRef list, DblListElem value);
DblListElem dbllist_pop(DblListRef list);
DblListElem dbllist_peek(DblListRef list);
void dbllist_insert(DblListRef list, int index, DblListElem value);
int dbllist_remove(DblListRef list, DblListElem value);
DblListElem dbllist_removeAt(DblListRef list, int index);
int dbllist_indexOf(DblListRef list, DblListElem value);

#endif //CS430_PROJ4_RAYCASTING_LIST_H
