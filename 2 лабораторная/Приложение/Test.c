#include <stdio.h>

int main() { int a[13]={1, 2, 3, 4, 4, 2, 3, 4, 2, 5, 0, 1, 0};

for (int i = 0; i < 13; i++) {

      for(int k = 13; k > i; k--){

        if(a[i]==a[k]){
            printf("%d %d\n", a[i], a[k]);
        }
     }
  }
  return 0;
}
