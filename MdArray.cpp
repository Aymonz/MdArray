#include <iostream>
using namespace std;


/**
 * Multi dimensional array (4D in this ex) in contiguous memory (1 malloc) with prepared pointer tables to have freedom of access
 * using indexing through (arr) or with linear access and index calculations through (larr)
 * 
 * linear access equations:
 * idx -> global linear idx
 * iw, ix, iy, iz -> idx per dimension (the example considers iw the slowest changing idx through iz which the fastest)
 * 
 * >> idx = iz + iy * (zs) + ix * (ys * zs) + iw * (xs * ys * zs)
 * 
 * >> iw = (idx / (xs * ys * zs)) % ws
 * >> ix = (idx / (ys * zs)) % xs
 * >> iy = (idx / (zs)) % ys
 * >> iz = idx % zs
 * */

// data type for the array element
typedef unsigned int dtype;

int main()
{
    // indexing variables
    unsigned int i,j,k,l,idx;
    
    // sizes for the 4 dimensions (pointers overhead is reduced by using zs for the dimension with biggest size)
    unsigned char ws=2;
    unsigned char xs=3;
    unsigned char ys=4;
    unsigned char zs=5;
    
    //access variables
    // access by indexing (ex: arr[i][j][k][l])
    dtype**** arr;
    // linear access (ex: larr[idx] .. where idx is calculated from dimension indices)
    // ex:  
    dtype*    larr; 

    // calculate size with pointer table overhead
    unsigned int num_bytes = (sizeof(dtype*) * (ws + (ws * xs) + (ws * xs * ys))) + (sizeof(dtype) * (ws * xs * ys * zs));  
    
    // allocating contiguous memory
    arr = (dtype****)malloc(num_bytes);

    // helper pointer
    dtype*** ptr= (dtype***)arr;

    // test array used only for debugging (view in debugger for byte per byte analysis)
    unsigned char *test_arr = (unsigned char *)arr;
    
    
    // preparing the pointer tables
    idx=0;

    ptr += ws;
    for (i=0; i<ws; i++)
    {
        arr[idx++] = (ptr + i*xs);
    }
    
    ptr += (ws*xs);
    for (i=0; i<(ws*xs); i++)
    {
        arr[idx++] = (ptr + i*ys);
    }
        
    ptr += (ws*xs*ys);
    larr = (dtype*)ptr; //set larr at the last dimension also used for correct pointer arithmetic
    for (i=0; i<(ws*xs*ys); i++)
    {
        arr[idx++] = (dtype***)(larr + i*zs);
    }
    

    // filling the array 
    for (i=0; i<ws; i++)
    {
        for (j=0; j<xs; j++)
        {
            for (k=0; k<ys; k++)
            {
                for (l=0; l<zs; l++)
                {
                    arr[i][j][k][l] = 1;
                }
            }
        }
    }
    

    // byte-wise printing of the contiguous memory
    for(i=0; i<num_bytes; i++)
    {
        cout<< i <<": " << ((unsigned long int)test_arr[i]) <<endl;
    }
}