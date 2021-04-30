programming checklist
=====================

1. Did you return?

2. Did you initialize before use?

3. Do parameters and arguments match in types?

carelessness
------------
1. was it `.` or `->`?

string
------
1. Did you reserve one extra space for the '\0'?

2. How do you compute the size of a c string? you don't do this, right?

```c
void func(char *str)
{
  // ...
  int len = sizeof(str);
  // ...
}
```

3. Did you consider that you need to use an `int` instead of just a `char` to store the return value of `getchar()`?

*Because `getchar()` might return an `EOF`, which couldn't be represented by any `char`.*

stack memory
------------
1. When you read or write with a pointer, are you sure that there is really an array behind it and you move it within the legal range?

heap memory
-----------
1. Did you `free`?

2. When you use `malloc()` did you realize that the argument is in bytes?

*That is, if you want a space for 4 `int`s, then you need to call `malloc(sizeof(int) * 4)` instead of just `malloc(4)`*

