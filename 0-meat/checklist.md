programming checklist
=====================

1. Did you return?

2. Did you initialize before use?

3. Do parameters and arguments match in types?

carelessness
------------
想当然

1. Was it `.` or `->`?

2. After writing the body for loop, did you forget the increment?

3. Did you declare a function before calling it?

4. Is it reading from stdin or reading from a string?

basics
------
1. Do you know that the unit of sizeof operator is bytes?

```c
int a[] = {1,2,3};
```
`sizeof a` is not 3, but 12 (given `int` has 4 bytes)!

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

`string.h`
----------
1. Did you notice that functions in `string.h` put `dest` in front of `src`?

For example, `strcpy(p, q)` copies the contents from `q` to `p`, instead of the other way round.
