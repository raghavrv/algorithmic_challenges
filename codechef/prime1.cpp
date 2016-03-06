#include<iostream>
#include<math.h>
#include<vector>

#define MAX_SIZE 1000000000

using namespace std;

void update_is_prime(vector<bool> &is_prime,
                     unsigned long int upper_limit,
                     unsigned long int previous_upper_limit) {

    

    // Check if the is_prime was previously initialized
    if (previous_upper_limit==0) {
        // Mark the base primes
        is_prime[1] = false;
        is_prime[2] = true;
        is_prime[3] = true;
        is_prime[5] = true;
        previous_upper_limit = 1;
    }

    else if (upper_limit <= previous_upper_limit)
        return;

    /* According to atkin's theorem,
     *
     * A number, n, is prime if 1-the number of solutions to the quadratic is
     * odd, 2-it meets the criterion for that quadratic and 3-if n does not
     * have a perfect square factor.
     * 
     * As for the quadratic, we have 3 as defined by atkins,
     *
     * Q1, Quadratic 1 --> 4 * x^2 + y^2; criterion n % 4 == 1
     * Q2, Quadratic 2 --> 3 * x^2 + y^2; criterion n % 6 == 1
     * Q3, Quadratic 3 --> 3 * x^2 - y^2; criterion n % 12 == 11
     *
     * These three quadratics are sufficient to cover all the prime numbers.
     *
     */ 

    // We try all possible solution pairs (x, y),
    // now if the quadratic evaluates to n, we have one solution to n = Q1/Q2/Q3
    // so we flip the is_prime entry for that number (n)
    // At the end, the number of flips correspond to the number of solution
    // pairs that satisfy n = Q1/Q2/Q3

    // So for any number n, if the number of solutions (to n = Q1/Q2/Q3) is
    // odd --> number of flips is odd --> is_prime entry changes from initial
    // state (composite) to prime.
    
    unsigned long n;
    unsigned long n_mod_60;

    //  We will check for primes in the range (sqrt(previous_upper_limit), upper_limit]
    // range of x for all 3 quadratics is [sqrt(previous_upper_limit), sqrt(upper_limit)]

    unsigned long sqrt_upper_limit = sqrt(upper_limit);
    unsigned long sqrt_previous_upper_limit = sqrt(previous_upper_limit);

    unsigned long x_y_upper_limit = sqrt_upper_limit;

    unsigned long x_y_lower_limit = sqrt_previous_upper_limit;
    unsigned long x_y_lower_limit_odd =  x_y_lower_limit;
    unsigned long x_y_lower_limit_even = x_y_lower_limit;

    if (x_y_lower_limit % 2 == 0)
        x_y_lower_limit_odd++;
    else
        x_y_lower_limit_even++;

    // Quadratic 1 : 4 * x^2 + y^2;
    // All x's odd y's
    for (unsigned long int x=x_y_lower_limit; x <= x_y_upper_limit; x++) {
        for (unsigned long int y=x_y_lower_limit_odd; y <= x_y_upper_limit; y+=2) {
            // Quadratic 1 : 4 * x^2 + y^2;
            n = 4 * x * x + y * y;
            n_mod_60 = n % 60;

            // Eleminating composites based on the criterion (n % 4 == 1)
            // We check for n % 60 == {1, 13, 17, 29, 37, 41, 49, 55}
            // to eleminate more composites.

            // Flip the is_prime entry if n is a prime candidate for
            // the new range ((previous_upper_limit, upper_limit])
            if ((n > previous_upper_limit) && (n <= upper_limit) &&
                    (n_mod_60 == 1 || n_mod_60 == 13 || n_mod_60 == 17 ||
                     n_mod_60 == 29 || n_mod_60 == 37 || n_mod_60 == 41 ||
                     n_mod_60 == 49 || n_mod_60 == 53)) {
                is_prime[n] = !is_prime[n];
            }
        }
    }

    // Quadratic 2 : 3 * x^2 + y^2;
    // Odd x's even y's
    for (long long int x=x_y_lower_limit_odd; x <= x_y_upper_limit; x += 2) {
        for (long long int y=x_y_lower_limit_even; y <= x_y_upper_limit; y += 2) {
            // Quadratic 2 : 3 * x^2 + y^2;
            n = 3 * x * x + y * y;
            n_mod_60 = n % 60;

            // Eleminating composites based on the criterion (n % 3 == 1)
            // We check for n % 60 == {7, 19, 31, 43} to eleminate more
            // composites.

            // Flip the is_prime entry if n is a prime candidate for
            // the new range ((previous_upper_limit, upper_limit])
            if ((n > previous_upper_limit) && (n <= upper_limit) &&
                    (n_mod_60 == 7 || n_mod_60 == 19 || n_mod_60 == 31 ||
                     n_mod_60 == 43)) {
                is_prime[n] = !is_prime[n];
            }
        }
    }

    // Quadratic 3 : 3 * x^2 - y^2;
    // All even/odd combinations
    // To avoid underflow checks, lets use signed int itself
    for (long long int x = x_y_lower_limit; x <= x_y_upper_limit; x++) {
        for (long long int y = x-1; y >= 0; y -= 2) {
            // Quadratic 3 : 3 * x^2 - y^2;
            n = 3 * x * x - y * y;
            n_mod_60 = n % 60;

            // Eleminating composites based on the criterion (n % 12 == 11)
            // We check for n % 60 == {11, 23, 47, 59} to eleminate more
            // composites.

            // Flip the is_prime entry if n is a prime candidate for
            // the new range ((previous_upper_limit, upper_limit])
            if ((n > previous_upper_limit) && (n <= upper_limit) &&
                    (n_mod_60 == 11 || n_mod_60 == 23 || n_mod_60 == 47 ||
                     n_mod_60 == 59)) {
                is_prime[n] = !is_prime[n];
            }
        }
    }

    // Eliminate composites with squared primes as factors.
    // (Those incorrectly marked prime by us in the previous steps)
    unsigned long int twice_of_factor_squared;
    for (unsigned long int factor = 5; factor <=sqrt_upper_limit; factor++) {

        // Skip if the factor is not prime.
        if (is_prime[factor])    {
            // Strike off the multiples of the factor_squared
            twice_of_factor_squared = 2 * factor * factor;

            for (unsigned long int multiple=0.5*twice_of_factor_squared;
                     multiple <= upper_limit;
                     // Skip even multiples
                     multiple += twice_of_factor_squared) {

                if (is_prime[multiple])
                    is_prime[multiple] = false;
            }
        }
    }
}

int main() {
    // Lets use a common sieve (is_prime) and cache it for all the n_tries
    // Initialize the sieve (is_prime) to all 0s (Setting all numbers to composite)

    // This will force it to store the data on the heap
    vector<bool> *is_prime_ref = new vector<bool>(MAX_SIZE);

    // This will make my life a bit easier
    vector<bool> &is_prime = *is_prime_ref;

    unsigned long int previous_upper_limit=0;

    unsigned long int lower_limit, upper_limit;

    unsigned int n_testcases;
    cin>>n_testcases;

    while(n_testcases--) {
        cin>>lower_limit>>upper_limit;

        update_is_prime(is_prime, upper_limit, previous_upper_limit);
        if (upper_limit > previous_upper_limit)
            previous_upper_limit = upper_limit;

        for (unsigned long int no=lower_limit; no <= upper_limit; no++)
            if (is_prime[no] == true)
                cout<<no<<endl;
    }
    return 0;
}
