#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "cifras.h"

#define TABLE_SIZE 67

const double probabilities [TABLE_SIZE] = 
{ 0.000160079, 0.000554985, 0.000381091, 0.000306662, 0.000263793, 0.000235223, //0-5
  0.000214441, 0.00019842, 0.000185551, 0.000174894, 0.002049944, 0.00037499, //6-11
  0.000699981, 0.00107497, 0.003174913, 0.000549985, 0.000499986, 0.001524958, //12-17
  0.001749952, 3.7499E-05, 0.000192495, 0.000999973, 0.000599984, 0.001674954, //18-23
  0.001874949, 0.000474987, 2.37493E-05, 0.001499959, 0.001574957, 0.002274938, //24-29
  0.000699981, 0.000244993, 0.000599984, 3.7499E-05, 0.000499986, 1.84995E-05, //30-35
  0.061498311, 0.011249691, 0.020999423, 0.032249114, 0.095247384, 0.016499547, //36-41
  0.014999588, 0.045748744, 0.052498558, 0.001124969, 0.005774841, 0.029999176, //42-47
  0.017999506, 0.05024862, 0.056248455, 0.014249609, 0.00071248, 0.044998764, //48-53
  0.047248702, 0.068248126, 0.020999423, 0.007349798, 0.017999506, 0.001124969, //54-59
  0.014999588, 0.000554985, 0.190494768, 0.014249609, 0.014999588, 0.000554985, //60-65
  0.001124969 }; //66

int attack2(FILE *output_stream, double *freq)
{
    int min_offset = 0;
    double error[TABLE_SIZE + 1] = {0};

    for (int offset = 0; offset < TABLE_SIZE; offset++) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            error[offset] +=((pow((freq[i] - probabilities[(i + offset) % 67]), 2)) / probabilities[(i + offset) % 67]);
        }
    }

    double min_error = error[0];
    for (int i = 1; i < TABLE_SIZE; i++) {
        if (error[i] < min_error) {
            min_error = error[i];
            min_offset = i;
        }
    }

    fprintf(output_stream, "offset com menor erro %d, letra '%c', "
                       "cifrado com letra '%c', "
                       "erro quadrático médio: %f.\n",
                       min_offset, cipher_table[min_offset],
                       cipher_table[(TABLE_SIZE - min_offset) % TABLE_SIZE], min_error);

    return min_offset;
}