# Allocating a Memory buffer


```C
// Allocating a memory buffer of 300 bytes

PVOID memaddy = malloc(300); // Using malloc()

PVOID memaddy = HeapAlloc(GetProcessHeap(), 0, 300); // Using HeapAlloc()

PVOID memaddy = LocalAlloc(LPTR, 300); // Using LocalAlloc()
```

# Writing Data to Memory

```C
PVOID memaddy = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 300);

CHAR* cString = "Things and stuff";

memcpy(memaddy, cString, strlen(cString));
```
