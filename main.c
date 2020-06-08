#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "circuit.h"
#include "logic.h"

int main() {

  Circuit circuit = {NULL, 0};
  circuit = read_input(circuit);
  if (circuit.start == NULL) {
    printf("Error: Empty input.\n");
    exit(0);
  }
  int no_inputs = get_no_inputs(circuit);
  int inputs[no_inputs];
  int i;
  for (i = 0; i < no_inputs; i++) {
    inputs[i] = 0;
  }
  print_headers(circuit);
  circuit = run(circuit, no_inputs, inputs, 1);
  int r;
  int j;
  for (r = 0; r < pow((double)2, (double)no_inputs) - 1; r++) {
    for (i = no_inputs - 1; i >= 0; i--) {
      if (inputs[i] != 1) {
        inputs[i] = 1;
        for (j = no_inputs - 1; j > i; j--) {
          inputs[j] = 0;
        }
        break;
      }
    }
    circuit = run(circuit, no_inputs, inputs, 1);
  }
  printf("\n");
  find_expression(circuit, no_inputs);
  free_circuit(circuit);
}
