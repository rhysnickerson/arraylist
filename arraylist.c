#include "arraylist.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define GROWTH_RATE 2
void resize_grow(ArrayList* array_list){
    array_list->array_size *= GROWTH_RATE;
    array_list->array = reallocarray(array_list->array, array_list->array_size, array_list->elem_size);
}
void resize_shrink(ArrayList* array_list){
    array_list->array_size /=GROWTH_RATE;
    array_list->array = reallocarray(array_list->array, array_list->array_size, array_list->elem_size);
}
void arrayError(ArrayList *array_list, char* error_message){
    fprintf(stderr, "ArrayListError: %s\n", error_message);
    destroyArrayList(array_list);
    exit(1);
}
char* getIndex(ArrayList *array_list, size_t index){
    return array_list->array + (index * array_list->elem_size);
}


//Externally Accessible Functions

ArrayList* initArrayList(size_t elem_size){
    ArrayList *array_list = malloc(sizeof(ArrayList));
    if(array_list == NULL){
        return NULL;
    }
    array_list->array =(char*) malloc(elem_size);
    if(array_list->array == NULL){
        return NULL;
    }
    array_list->elem_size = elem_size;
    array_list->array_size = 1;
    array_list->size = 0;
    array_list->holder_elem.elem_size = elem_size; 
    array_list->holder_elem.bytes = malloc(elem_size);

    return array_list;
}

void destroyArrayList(ArrayList* array_list){
    if(array_list == NULL){
        return;
    }
    free(array_list->array);
    free(array_list->holder_elem.bytes);
    free(array_list);
}
size_t arrayListSize(ArrayList* array_list){
    return array_list->size;
}

//HIDDEN FUNCTIONS - ONLY ACCESSIBLE FROM PREPROCESSOR MACROS

ArrayElement (_getElement)(ArrayList* array_list, size_t elem_size, size_t index){
    if(index >= array_list->size || index < 0){arrayError(array_list, "index out of range");}
    else if(elem_size != array_list->elem_size){arrayError(array_list, "mismatched element sizes");}

    ArrayElement element;
    element.elem_size = elem_size;
    element.bytes =  getIndex(array_list, index);
    return element;
}



void (_appendElement)(ArrayList* array_list, ArrayElement element){
    if(element.elem_size != array_list->elem_size){arrayError(array_list, "mismatched element sizes");}
    
    char* target = getIndex(array_list, array_list->size);
    memcpy(target, element.bytes, array_list->elem_size);

    array_list->size ++;
    if(array_list->size >= array_list->array_size){
        resize_grow(array_list);
    }
    return;
}

void (_insertElement)(ArrayList *array_list, ArrayElement element, size_t index){
    if(index >= array_list->size || index < 0){arrayError(array_list, "index out of range");}
    else if(element.elem_size != array_list->elem_size){arrayError(array_list, "mismatched element sizes");}


    char* at_index=getIndex(array_list, index);
    char* after_index = getIndex(array_list, index+1);
    //NOTE: does this need to be memmove to avoid issues? - likely note but alternative implementations could mess this up
    memcpy(after_index,at_index, ((array_list->size - index) ) * array_list->elem_size);
    memcpy(at_index, element.bytes, array_list->elem_size);

    array_list->size ++; 
    if(array_list->size >= array_list->array_size){
        resize_grow(array_list);
    }
    return;
}
void (_replaceElement)(ArrayList *array_list, ArrayElement element, size_t index){
    if(index >= array_list->size || index < 0){arrayError(array_list, "index out of range");}
    else if(element.elem_size != array_list->elem_size){arrayError(array_list, "mismatched element sizes");}

    char* target = getIndex(array_list, index);
    memcpy(target, element.bytes, array_list->elem_size);
    return;
}

//NOTE: horrendously slow :(
ArrayElement (_removeElement)(ArrayList *array_list, size_t index){
    if(index >= array_list->size || index < 0){arrayError(array_list, "index out of range");}
    //THE MEMORY ADDRESS GETS REWRITTEN - NEED TO TEMPORARILY STORE MEMORY ELSEWHERE TO WRITE TO ELEMENT

    char* target = getIndex(array_list, index);
    char* after_target = getIndex(array_list, index+1);

    memcpy(array_list->holder_elem.bytes, target, array_list->elem_size);
    memmove(target, after_target, (((array_list->size-index)-1))*array_list->elem_size);

    array_list->size --;
    if(array_list->size < array_list->array_size /GROWTH_RATE){
        resize_shrink(array_list);
    }
    return array_list->holder_elem;
}

