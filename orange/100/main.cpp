#include <iostream>
#include <string>
#include <math.h>
#include <algorithm>
using namespace std;
int main() {
    int size, loop;
    printf("請輸入執行次數：");
    scanf("%d", &loop);
    while(loop--){
        printf("請輸入橘子總數：");
        scanf("%d", &size);
        int org[size];
        printf("請輸入擠橘子時間：");
        for (int i = 0; i < size; i++) {
            scanf("%d", &org[i]);
        }
        sort(org, org + size);
        for (int i =0; i < size; i++) {
            printf("%d, ", org[i]);
        }
        int s = 0, i = size-1;
        int d = org[i];
        for (; i > 0 ; i--) {
            printf(" iter = %d, s = %d\n", i, s);
            d = abs(d) - org[i-1];
            if (d >= 0) {
                s = s + org[i-1];
            } else {
                s = s + org[i-1] - abs(d);
            }
            if (i==1) {
                s = s + abs(d);
            }
        }
        printf("solution s = %d \n", s);
    }
}


