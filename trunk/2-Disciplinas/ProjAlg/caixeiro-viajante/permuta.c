#include <stdio.h>


void print(const int *v, const int size)
{
  if (v != 0) {
        int i;
    for ( i = 0; i < size; i++) {
      printf("%4d", v[i]-1 );
    }
    printf("\n");
  }
} // print


void visit(int *Value, int N, int k)
{
  static level = 0;
  level = level+1; Value[k] = level;

  if (level == N)
    print(Value, N);
  else
  {
      int i;
    for ( i = 0; i < N; i++)
      if (Value[i] == 0)
        visit(Value, N, i);
}
  level = level-1; Value[k] = 0;
}


main()
{
  const int N = 4;
  int Value[N];
  int i;
  for (i = 0; i < N; i++) {
    Value[i] = 0;
  }
  visit(Value, N, 0);
}
