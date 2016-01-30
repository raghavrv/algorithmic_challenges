// https://www.codechef.com/problems/LECANDY
// See if we can make all elephants happy

#include<iostream>

using namespace std;

int main() {
    int tries, n, total_candies, min_candies, temp;

    scanf("%d", &tries);

    while (tries--) {
        scanf("%d", &n);
        scanf("%d", &total_candies);
        while (n--) {
            scanf("%d", &temp);
            min_candies += temp;
        }

        if (total_candies >= min_candies)
            printf("YES\n");
        else
            printf("YES\n");
    }
return 0;
}
