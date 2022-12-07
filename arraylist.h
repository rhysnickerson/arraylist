#ifndef __ARRAYLIST_H__
#define __ARRAYLIST_H__

#include <stddef.h>
#include <stdlib.h>

typedef struct{
    size_t elem_size;
    char* bytes;
}ArrayElement;

typedef struct{
    size_t elem_size; //the length in bytes of an element in the arry
    size_t array_size; //the actual size of the underlying array. Note! Not the same as the size of the list, the underlying array grows logarithmically
    size_t size; //the actual number of elements in the array
    char* array; //the underlying byte array
    ArrayElement holder_elem; //temporary element to hold recently deleted elements
}ArrayList;



extern ArrayList* initArrayList(size_t elem_size);
extern void destroyArrayList(ArrayList* array_list);
extern size_t arrayListSize(ArrayList* array_list);

extern ArrayElement _getElement(ArrayList* array_list, size_t elem_size, size_t index);
extern void  _appendElement(ArrayList* array_list, ArrayElement element);
extern void _insertElement(ArrayList *array_list, ArrayElement element, size_t index);
extern void _replaceElement(ArrayList *array_list, ArrayElement element, size_t index);
extern ArrayElement _removeElement(ArrayList *array_list, size_t index);

#define ERR_USE_MACRO error_use_the_macro_not_the_function
#define _getElement(...) ERR_USE_MACRO
#define getElementVal(array_list, type, index) *(type*)((_getElement)(array_list, sizeof(type), index).bytes)
#define getElementPtr(array_list, type, index) (type*)((_getElement)(array_list, sizeof(type), index).bytes)

#define _removeElement(...) ERR_USE_MACRO 
#define removeElement(array_list, index)  {(_removeElement)(array_list, index);}
#define removeElementVal(array_list, type, index) *(type*)((_removeElement)(array_list,  index).bytes)
#define removeElementPtr(array_list, type, index) (type*)((_removeElement)(array_list,  index).bytes)

#define _appendElement(...) ERR_USE_MACRO
#define appendElement(array_list, type, element) {type _ = element; (_appendElement)(array_list, (ArrayElement){sizeof(type), (char*)&_});}

#define _insertElement(...) ERR_USE_MACRO
#define insertElement(array_list, type, element, index) {type _ = element; (_insertElement)(array_list, (ArrayElement){sizeof(type), (char*)&_}, index);}

#define _replaceElement(...) ERR_USE_MACRO
#define replaceElement(array_list, type, element, index) {type _ = element; (_replaceElement)(array_list, (ArrayElement){sizeof(type), (char*)&_}, index);}

#endif