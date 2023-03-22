#include <stdio.h>
#include "AdptArray.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>


typedef struct AdptArray_
{
    PElement *array; // array of PElements
    int array_size; // counter that represents the size of the array
    COPY_FUNC copyFunc; // pointer to a copy function
    DEL_FUNC delFunc; // pointer to a delete function
    PRINT_FUNC printFunc; // pointer to a print function

}AdptArray_, *PAdptArray;

//  ##################################################################################

PAdptArray CreateAdptArray(COPY_FUNC copyF, DEL_FUNC deleteF, PRINT_FUNC printF)
{
    if( (copyF != NULL) && (deleteF != NULL) && (printF != NULL) )
    {
        // create a pointer(PAdptArray) to the allocated memory of AdptArray struct
        PAdptArray new_PAdptArray = (PAdptArray)malloc(sizeof(AdptArray_));

        // making sure that the allocated memory isn't NULL
        if(new_PAdptArray == NULL)
        {
            printf("Couldn't create memory allocation for the AdptArray\n");
            return NULL;
        }

        // initialize the AdptArray_ struct 
        new_PAdptArray->array = NULL;
        new_PAdptArray->array_size = 0;
        new_PAdptArray->copyFunc = copyF;
        new_PAdptArray->delFunc = deleteF;
        new_PAdptArray-> printFunc = printF;

        return new_PAdptArray;
    }

    // if one of the parameters is NULL
    else
    {
        printf("one of the parameters is NULL\n");
        return NULL;
    }
}

//  ##################################################################################

void DeleteAdptArray(PAdptArray pAA)
{
    // checking if the pointer points to NULL, if not then continue with the code
    if( (pAA != NULL) && (pAA->array != NULL) )
    {
        // the current size of the array
        int arrSize = pAA->array_size;

        // loop through the array
        for (int i = 0; i < arrSize; i++)
        {
            // the current element in the array
            PElement currPElem = (pAA->array)[i];

            // if the element isn't a NULL delete it using the delete function pointer
            if(currPElem != NULL)
            {
                pAA->delFunc(currPElem);
            }
        }
            // clear the memory allocated fot the array
            free(pAA->array);
            // clear the memory allocated fot the AdptArray
            free(pAA);        
    }

    // if the pointer points to NULL 
    else
    {
        printf("The given parameter is NULL\n");
        return;
    }

}

//  ##################################################################################

Result SetAdptArrayAt(PAdptArray pAA, int index, PElement elem)
{
    // create new pointer to the array
    PElement *newPelemArray = NULL;

    if( (pAA != NULL) && (index >= 0) && (elem != NULL) )
    {

        // if the array is NULL create an array with size as the index+1
        if(pAA->array == NULL)
        {
            // create a pointer to the allocated memory of the array
            pAA->array = (PElement*)malloc( (index+1) * sizeof(PElement) );
            // making sure that the allocated memory isn't NULL
            if(pAA->array == NULL)
            {
                printf("Couldn't create memory allocation for the array\n");
                return FAIL;
            }

            // raise the size of the array to index+1
            pAA->array_size = index+1;

            // reset all the array to NULL
            for (int i = 0; i < pAA->array_size; i++)
            {
                (pAA->array)[i] = NULL;
            }
            
            // add the element to the array
            (pAA->array)[index] = pAA->copyFunc(elem);
            
        }
        else
        {   
            // if there is such an index in the array 
            if(index < pAA->array_size)
            {
                // delete if there is something there
                pAA->delFunc( (pAA->array)[index] );
                // add a copy of the desire element to the array
                (pAA->array)[index] = pAA->copyFunc(elem);
            }
            // if there isn't such an index in the array 
            else
            {
                // create a new array with the size of index and set it all to NULL
                newPelemArray = (PElement*)calloc((index+1), sizeof(PElement));
                
                // making sure that the allocated memory isn't NULL
                if(newPelemArray == NULL)
                {
                    printf("Couldn't create memory reallocation for the array\n");
                    return FAIL;
                }

                // copy the old array to the new array
                memcpy(newPelemArray, pAA->array, (pAA->array_size) * sizeof(PElement));
                // clear the memory of the old array
                free(pAA->array);
                // set the array of the struct to be the new array
                pAA->array = newPelemArray;

                // raise the size of the array to index+1
                pAA->array_size = index+1;   

                // delete if there is something there
                if( (pAA->array)[index] != NULL)
                {
                    pAA->delFunc( (pAA->array)[index] );
                }
                
                // add the element to the array
                (pAA->array)[index] = pAA->copyFunc(elem);
            }

        }
        
       return SUCCESS;
    }

    // if one of the parameters is invalid
    else
    {
        printf("one of the parameters is invalid\n");
        return FAIL;
    }

}

//  ##################################################################################

PElement GetAdptArrayAt(PAdptArray pAA, int index)
{
    // the return element - desire PElement or NULL
    PElement retElem = NULL;

    if( (pAA != NULL) && (index >= 0) )
    {
        // if the array is NULL 
        if(pAA->array == NULL)
        {
            printf("the array is NULL\n");
        }
        // if there is such an index, return the element
        else if(index < pAA->array_size)
        {
            if( (pAA->array)[index] != NULL )
            {
                // retElem = pAA->copyFunc( (pAA->array)[index] );
                retElem = pAA->copyFunc( (pAA->array)[index] );
            }
        }
        // if the given index doesn't exist
        else
        {
            printf("index out of bound\n");
        }  
    }
    // if one of the parameters is invalid
    else
    {
        printf("one of the parameters is invalid\n");
    }
    
    return retElem;
}

//  ##################################################################################

int GetAdptArraySize(PAdptArray pAA)
{
    // checking if the pointer points to NULL, if not then continue with the code
    if(pAA == NULL)
    {
        printf("the given parameter is NULL\n");
        return -1;
    }
    // checking if the array is NULL
    if(pAA->array == NULL)
    {
        printf("the array is NULL\n");
        return -1;
    }

    return pAA->array_size;

}

//  ##################################################################################

void PrintDB(PAdptArray pAA)
{
    // checking if the pointer points to NULL, if not then continue with the code
    if(pAA == NULL)
    {
        printf("the given parameter is NULL\n");
        return;
    }

    // checking if the array is NULL
    if(pAA->array == NULL)
    {
        printf("the array is NULL\n");
        return;
    }

    // print the elements in the array 
    printf("Print the array: \n");
    for (int i = 0; i < pAA->array_size; i++)
    {
        PElement currPElem = (pAA->array)[i];
        if(currPElem != NULL)
        {
            pAA->printFunc(currPElem);
        }
    }
    
}

//  ##################################################################################



// ***********************************
// Tools that helpd me:              *
// ADT - Tirgul 11                   *
// Exam 1 - solution for question 4  *
// ***********************************

