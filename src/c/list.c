
#include<stdlib.h>
#include "../headers/list.h"

void dbllist_init(DblListRef list, int initialSize) {
    list->elements = malloc(sizeof(DblListElem) * initialSize);
    list->length = 0;
    list->maxSize = initialSize;
}
void dbllist_dispose(DblListRef list) {
    free(list->elements);
}

void dbllist_resize(DblListRef list, int newSize) {
    list->elements = realloc(list->elements, sizeof(DblListElem) * newSize);
    list->maxSize = newSize;
}
void dbllist_shift_left(DblListRef list, int startIndex, int shiftAmount) {
    int i;
    for (i = startIndex; i < list->length; i++) {
        dbllist_update(list, i - shiftAmount, dbllist_elem_at(list, i));
    }
    list->length -= shiftAmount;
}
void dbllist_shift_right(DblListRef list, int startIndex, int shiftAmount) {

    if (list->length + shiftAmount > list->maxSize) {
        dbllist_resize(list, list->maxSize * 2);
    }

    int i;
    for (i = list->length-1; i >= startIndex; i--) {
        dbllist_update(list, i + shiftAmount, dbllist_elem_at(list, i));
    }
    list->length += shiftAmount;
}

DblListElem dbllist_elem_at(DblListRef list, int index) {
    return list->elements[index];
}
void dbllist_update(DblListRef list, int index, DblListElem value) {
    list->elements[index] = value;
}

void dbllist_push(DblListRef list, DblListElem value) {
    if (list->length + 1 > list->maxSize) {
        dbllist_resize(list, list->maxSize * 2);
    }
    list->elements[list->length++] = value;
}
DblListElem dbllist_pop(DblListRef list) {
    return list->elements[--list->length];
}
DblListElem dbllist_peek(DblListRef list) {
    return list->elements[list->length - 1];
}
void dbllist_insert(DblListRef list, int index, DblListElem value) {
    dbllist_shift_right(list, index, 1);
    dbllist_update(list, index, value);
}
int dbllist_remove(DblListRef list, DblListElem value) {
    int index = dbllist_indexOf(list, value);
    if (index >= 0) {
        dbllist_removeAt(list, index);
        return 1;
    }
    return 0;
}
DblListElem dbllist_removeAt(DblListRef list, int index) {
    DblListElem out = dbllist_elem_at(list, index);
    dbllist_shift_left(list, index + 1, 1);
    return out;
}
int dbllist_indexOf(DblListRef list, DblListElem value) {
    int i;
    DblListElem e;
    for (i = 0; i < list->length; i++) {
        e = dbllist_elem_at(list, i);
        if (e.value == value.value && e.tag == value.tag) {
            return i;
        }
    }
    return -1;
}