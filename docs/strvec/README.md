## Strvec

This library only does single memory allocation for all strings instead of calling malloc() every time,

## How it works ?
```
data array*:
-----------------------------
| "string1\0" | "string2\0" |
-----------------------------
   |             |
   offset[0]     offset[1]
```

The offset array keeps track of the index of the string in the memory,
for that there is actualy two dynamic arrays used for doing single
memory allocation. This improves it's performance and has less failure
chance since most of the libraries call malloc() everytime.

Example program can be found at /examples

## Be aware

Note that this library is not finished, I did not find any implementation
similar to this altough there may be, there may be better ones. I do not
claim this library to be the fastest and the most secure one.

## Contribute

If you find any bugs please submit a pull request.
