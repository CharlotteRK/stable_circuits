#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "circuit.h"
#include "logic.h"

int _AND(Wire* in_one, Wire* in_two) {
  if ((in_one->name == NULL) || (in_two->name == NULL)) {
    printf("Warning: insufficient input wires to operator AND\n");
    return 0;
  }
  return (in_one->previous_value && in_two->previous_value);
}

int _OR(Wire* in_one, Wire* in_two) {
  if ((in_one->name == NULL) || (in_two->name == NULL)) {
    printf("Warning: insufficient input wires to operator OR\n");
    return 0;
  }
  return (in_one->previous_value || in_two->previous_value);
}

int _NAND(Wire* in_one, Wire* in_two) {
  if ((in_one->name == NULL) || (in_two->name == NULL)) {
    printf("Warning: insufficient input wires to operator NAND\n");
    return 0;
  }
  return !(_AND(in_one, in_two));
}

int _NOR(Wire* in_one, Wire* in_two) {
  if ((in_one->name == NULL) || (in_two->name == NULL)) {
    printf("Warning: insufficient input wires to operator NOR\n");
    return 0;
  }
  return !(_OR(in_one, in_two));
}

int _XOR(Wire* in_one, Wire* in_two) {
  if ((in_one->name == NULL) || (in_two->name == NULL)) {
    printf("Warning: insufficient input wires to operator XOR\n");
    return 0;
  }
  return (_OR(in_one, in_two) && (_NAND(in_one, in_two)));
}

int _NOT(Wire* in) {
  if (in->name == NULL) {
    printf("Warning: insufficient input wires to operator NOT\n");
    return 0;
  }
  return (!(in->previous_value));
}

int _IN(int input) {
  return input;
}
