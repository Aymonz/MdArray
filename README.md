# MdArray
Multi dimensional array (4D in this ex) in contiguous memory (1 malloc) with prepared pointer tables


In this small study, the various ways to build a multidimensional array in C/C++ are investigated. [this](https://www.geeksforgeeks.org/multidimensional-arrays-c-cpp/) page already lists the most famous techniques but it was desired to experiment with higher dimensions with focus on contiguous memory allocation. following is a summary of the 3 methods identified.

## Method1 - Explicit math for indexing (no memory overhead):
```c++
/**
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
 
void* arr = malloc(sizeof(dtype) * ws * xs * ys * zs);

// use array
for (iw=0; iw<ws; iw++)
{
  for (ix=0; ix<xs; ix++)
  {
    for (iy=0; iy<ys; iy++)
    {
      for (iz=0; iz<zs; iz++)
      {
        idx = iz + iy * (zs) + ix * (ys * zs) + iw * (xs * ys * zs)
        arr[idx] = value;
      }
    }
  }
}
```

## Method2 - Implicit math for indexing by liveraging sizing info and casting (no memory overhead):
```c++
/**
 * define memory normally but before access cast to data type with sizing info included (then the compiler will handle the linear access equations)
 * */
 
void* arr = malloc(sizeof(dtype) * ws * xs * ys * zs);
// OR
dtype (*myarr)[xs][ys][zs] = (dtype(*)[xs][ys][zs]) arr;

// use array
for (iw=0; iw<ws; iw++)
{
  for (ix=0; ix<xs; ix++)
  {
    for (iy=0; iy<ys; iy++)
    {
      for (iz=0; iz<zs; iz++)
      {
        ((dtype(*)[xs][ys][zs])arr)[iw][ix][iy][iz] = value;
        // OR
        myarr[iw][ix][iy][iz] = value;
      }
    }
  }
}
```
## Method3 - allocate more memory and prepare pointer tables (memory overhead):
```c++
/**
 * define additional memory and prepare pointer tables here you can also access the data linearly like in method1
 * */
 
//access variables
// access by indexing (ex: arr[i][j][k][l])
dtype**** arr;
// linear access (ex: larr[idx] .. where idx is calculated from dimension indices)
dtype*    larr; 

// calculate size with pointer table overhead
unsigned int num_bytes = (sizeof(dtype*) * (ws + (ws * xs) + (ws * xs * ys))) + (sizeof(dtype) * (ws * xs * ys * zs));  

// allocating contiguous memory
arr = (dtype****)malloc(num_bytes);

// helper pointer
dtype*** ptr= (dtype***)arr;

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

// use array
for (iw=0; iw<ws; iw++)
{
  for (ix=0; ix<xs; ix++)
  {
    for (iy=0; iy<ys; iy++)
    {
      for (iz=0; iz<zs; iz++)
      {
        arr[iw][ix][iy][iz] = value;
      }
    }
  }
}
```
**Note:** the code in the file only tests method3
