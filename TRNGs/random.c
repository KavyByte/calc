#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define BITS_TO_GENERATE 1024
#define LOOP_ITERATIONS 500000

int main() {
    /*
    * Step 1: Generating Raw Random Bits
    */
    printf("Step 1: Generating Raw Random Bits\n");
    int raw_bitstream[BITS_TO_GENERATE];

    for (int idx = 0; idx < BITS_TO_GENERATE; idx++) {
        struct timespec start, end;

        clock_gettime(CLOCK_MONOTONIC, &start);

        for (long j = 0; j < LOOP_ITERATIONS; j++) {
            int dummy = 0;
            dummy++;
        }

        clock_gettime(CLOCK_MONOTONIC, &end);

        int64_t duration_ns = (int64_t)(end.tv_sec - start.tv_sec) * 1000000000LL +
                               (int64_t)(end.tv_nsec - start.tv_nsec);

        raw_bitstream[idx] = (int)(duration_ns & 1);

        printf("%d", raw_bitstream[idx]);
    }
    printf("\n\n");

    /*
    * Step 2: Post-Processing with Von Neumann Extractor
    */
    printf("Step 2: Post-Processing with Von Neumann Extractor\n");
    int post_processed_bitstream[BITS_TO_GENERATE-1];

    int x = 0, y = 0;
    while(x < BITS_TO_GENERATE && y < BITS_TO_GENERATE - 1){
        int first = raw_bitstream[y], second = raw_bitstream[y+1];

        if ( first == 0 && second == 1) {
            post_processed_bitstream[x] = 0;
            printf("%d", 0);
            x++;
        } 
        else if ( first == 1 && second == 0) {
            post_processed_bitstream[x] = 1;
            printf("%d", 1);
            x++;
        }

        y+=2;
    }
    printf("\n\n");

    /*
    * Step 3: Statistical Analysis
    */
    printf("Step 3: Statistical Analysis\n");

    long long cnt_0 = 0, cnt_1 = 0; 
    long long total = x;

    for(int i = 0 ; i < x ; i++){
        if( post_processed_bitstream[i] ) cnt_1++;
        else cnt_0++;
    }

    printf("Shannon Entropy Formula = ");

    long double P0 = (long double)cnt_0 / total;;
    long double P1 = (long double)cnt_1 / total;

    long double term0 = 0.0L;
    long double term1 = 0.0L;

    if (P0 > 0.0L) {
        term0 = P0 * log2l(P0);
    }

    if (P1 > 0.0L) {
        term1 = P1 * log2l(P1);
    }

    long double H = -( term0 + term1 );

    printf("%Lf\n", H);

    printf("Chi-Square Test = ");

    long double O0 = cnt_0;
    long double O1 = cnt_1;
    long double E0 = total / 2.0;
    long double E1 = total / 2.0;

    long double exp1 = ( (O0 - E0) * (O0 - E0)) / E0;
    long double exp2 = ( (O1 - E1) * (O1 - E1)) / E1;

    long double Chi = exp1 + exp2;
    printf("%Lf\n", Chi);

    printf("\n");

    /*
    * Step 4: Comparison with PRNG
    */
    printf("Step 4: Comparison with PRNG\n");

    int pseudo_random_bitstream[total];
    for ( int i = 0 ; i < total ; i++){
        pseudo_random_bitstream[i] = rand() % 2;
        printf("%d",pseudo_random_bitstream[i]);
    }
    printf("\n\n");

    {
        long long cnt_0 = 0, cnt_1 = 0; 
        long long total = x;

        for(int i = 0 ; i < x ; i++){
            if( pseudo_random_bitstream[i] ) cnt_1++;
            else cnt_0++;
        }

        printf("PRNGs Shannon Entropy Formula = ");

        long double P0 = (long double)cnt_0 / total;;
        long double P1 = (long double)cnt_1 / total;

        long double term0 = 0.0L;
        long double term1 = 0.0L;

        if (P0 > 0.0L) {
            term0 = P0 * log2l(P0);
        }

        if (P1 > 0.0L) {
            term1 = P1 * log2l(P1);
        }

        long double H = -( term0 + term1 );

        printf("%Lf\n", H);

        printf("PRNGs Chi-Square Test = ");

        long double O0 = cnt_0;
        long double O1 = cnt_1;
        long double E0 = total / 2.0;
        long double E1 = total / 2.0;

        long double exp1 = ( (O0 - E0) * (O0 - E0)) / E0;
        long double exp2 = ( (O1 - E1) * (O1 - E1)) / E1;

        long double Chi = exp1 + exp2;
        printf("%Lf\n", Chi);
    }
    
    return 0;

}
