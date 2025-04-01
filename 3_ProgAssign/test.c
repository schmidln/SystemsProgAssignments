#include "fp_analyzer.h"

int main() {
    FP_TYPE num = 5.75; // Test value

    printf("Testing floating-point analysis for: %g\n", num);
    print_components(num);
    
    Converter conv;
    conv.floating_point = num;
    print_reconstitution(num, conv.components);

    return 0;
}
