#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "circuit.h"
#include "logic.h"

Circuit add_component(Component* component, Circuit circuit) {
  Component* ptr = circuit.start;
  component->next = NULL;
  if (circuit.start != NULL) {
    while (ptr->next != NULL) {
      ptr = ptr->next;
    }
    ptr->next = component;
  }
  else {
    circuit.start = component;
  }
  return circuit;
}

void free_atom(Atom* atom) {
  if (atom->next == NULL) {
    free(atom);
  }
  else {
    free_atom(atom->next);
  }
}

void free_component(Component* component) {
  if (component->next == NULL) {
    free(component->first_input);
    free(component->second_input);
    free(component->output);
    free(component->operator);
    free(component);
  }
  else {
    free_component(component->next);
  }
}

void free_circuit(Circuit circuit) {
  if (circuit.start == NULL) {
    return;
  }
  free_component(circuit.start);
}

Wire* add_wire(Wire* wire_strt, Wire* wire) {
  Wire* wire_ptr = wire_strt;
  int already_there = 0;
  if (wire_ptr == NULL) {
    wire_strt = wire;
    wire_strt->next = wire_strt;
  }
  else {
    while (wire_ptr->next != wire_strt) {
      if (!strcmp(wire_ptr->name, wire->name)) {
        free(wire);
        already_there = 1;
        return wire_ptr;
      }
      wire_ptr = wire_ptr->next;
    }
    if (!strcmp(wire_ptr->name, wire->name)) {
      free(wire);
      return wire_ptr;
    }
    if (!(already_there)) {
      if (!strcmp(wire->name, "one")) {
        wire->value = 1;
      }
      wire_ptr->next = wire;
      wire_ptr = wire_ptr->next;
      wire->next = wire_strt;
    }
  }
  return wire;
}

Circuit read_input(Circuit circuit) {
  Wire* wire_strt = NULL;
  char string[50];
  while (fgets(string, 50, stdin) != NULL) {
    if (string[0] == '\n' || string[0] == EOF || string[0] == '\r') {
      continue;
    }
    int i = 0;
    int s = 0;
    Component* component = malloc(sizeof(Component));
    char* ptr = string;
    Wire* output_wire = malloc(sizeof(Wire));
    output_wire->value = 0;
    output_wire->name = NULL;
    char* str_two;
    Wire* input_wire_one = malloc(sizeof(Wire));
    input_wire_one->value = 0;
    input_wire_one->name = NULL;
    Wire* input_wire_two = malloc(sizeof(Wire));
    input_wire_two->value = 0;
    input_wire_two->name = NULL;
    int n = 0;
    while (string[i] != '\n' && string[i] != EOF && string[i] != '\r') {
      n++;
      if (string[i] == ' ') {
        if (s == 0) {
          output_wire->name = strndup(ptr, n - 1);
        }
        else if (s == 1) {
          str_two = strndup(ptr, n - 1);
        }
        else if (s == 2) {
          input_wire_one->name = strndup(ptr, n - 1);
        }
        else if (s == 3) {
          input_wire_two->name = strndup(ptr, n - 1);
        }
        n = 0;
        s++;
        ptr = &string[i + 1];
      }
      i++;
    }
    if (s == 1) {
      str_two = strndup(ptr, n);
    }
    else if (s == 2) {
      input_wire_one->name = strndup(ptr, n);
    }
    else if (s == 3) {
      input_wire_two->name = strndup(ptr, n);
    }
    if (s == 1) {
      component->no_of_input = 0;
      component->first_input = input_wire_one;
      component->second_input = input_wire_two;
      component->operator = str_two;
      component->output = add_wire(wire_strt, output_wire);
      component->output->input = 1;
      if (wire_strt == NULL) {
        wire_strt = component->output;
      }
    }
    else if (s == 2) {
      component->no_of_input = 1;
      component->first_input = add_wire(wire_strt, input_wire_one);
      if (wire_strt == NULL) {
        wire_strt = component->first_input;
      }
      component->output = add_wire(wire_strt, output_wire);
      component->second_input = input_wire_two;
      component->operator = str_two;
    }
    else if (s == 3) {
      component->no_of_input = 2;
      component->first_input = add_wire(wire_strt, input_wire_one);
      if (wire_strt == NULL) {
        wire_strt = component->first_input;
      }
      component->second_input = add_wire(wire_strt, input_wire_two);
      component->output = add_wire(wire_strt, output_wire);
      component->operator = str_two;
    }
    circuit = add_component(component, circuit);
  }
  Wire* wire_ptr = wire_strt;
  int q = 1;
  while (1) {
    if (!strcmp(wire_ptr->name, "out")) {
      circuit.out = wire_ptr;
    }
    if (wire_ptr->next != wire_strt) {
      q++;
      wire_ptr = wire_ptr->next;
    }
    else {
      break;
    }
  }
  return circuit;
}

Circuit run_circuit(int no_inputs, int inputs[no_inputs], Circuit circuit) {
  Component* comp_ptr = circuit.start;
  if (comp_ptr == NULL) {
    puts("NO COMPONENTS IN CIRCUIT");
    return circuit;
  }

  else {
    int i = 0;
    while (1) {
      if (!strcmp(comp_ptr->output->name, "zero") || !strcmp(comp_ptr->output->name, "one")) {
        printf("Error: Malformatted input.");
        exit(0);
      }
      else if (!strcmp(comp_ptr->operator, "IN")) {
        comp_ptr->output->value = _IN(inputs[i]);
        i++;
      }
      else if (!strcmp(comp_ptr->operator, "NAND")) {
        comp_ptr->output->value = _NAND(comp_ptr->first_input, comp_ptr->second_input);
      }
      else if (!strcmp(comp_ptr->operator, "AND")) {
        comp_ptr->output->value = _AND(comp_ptr->first_input, comp_ptr->second_input);
      }
      else if (!strcmp(comp_ptr->operator, "NOR")) {
        comp_ptr->output->value = _NOR(comp_ptr->first_input, comp_ptr->second_input);
      }
      else if (!strcmp(comp_ptr->operator, "XOR")) {
        comp_ptr->output->value = _XOR(comp_ptr->first_input, comp_ptr->second_input);
      }
      else if (!strcmp(comp_ptr->operator, "OR")) {
        comp_ptr->output->value = _OR(comp_ptr->first_input, comp_ptr->second_input);
      }
      else if (!strcmp(comp_ptr->operator, "NOT")) {
        comp_ptr->output->value = _NOT(comp_ptr->first_input);
      }
      else {
        printf("Error: Invalid gate operator: %s\n", comp_ptr->operator);
        exit(0);
      }
      if (comp_ptr->next != NULL) {
        comp_ptr = comp_ptr->next;
      }
      else {
        break;
      }
    }
  }
  Wire* wire_strt = circuit.start->output;
  Wire* wire_ptr = wire_strt;
  while (1) {
    wire_ptr->previous_value = wire_ptr->value;
    if (wire_ptr->next != wire_strt) {
      wire_ptr = wire_ptr->next;
    }
    else {
      break;
    }
  }
  return circuit;
}

void print_headers(Circuit circuit) {
  Wire* wire_ptr = circuit.start->output;
  do {
    if (wire_ptr->input || !strcmp(wire_ptr->name, "out")) {
      printf("%s ", wire_ptr->name);
    }
    wire_ptr = wire_ptr->next;
  } while (wire_ptr != circuit.start->output);
  printf("\n");
}

void print_state(Circuit circuit, int stable) {
  Wire* wire_ptr = circuit.start->output;
  do {
    if (wire_ptr->input || !strcmp(wire_ptr->name, "out")) {
      if (!strcmp(wire_ptr->name, "out") && !stable) {
        printf("? ");
      }
      else {
        printf("%d ", wire_ptr->value);
      }
    }
    wire_ptr = wire_ptr->next;
  } while (wire_ptr != circuit.start->output);
  printf("\n");
}

int get_no_inputs(Circuit circuit) {
  int no_inputs;
  Component* component = circuit.start;
  while (1) {
    if (component->operator == NULL) {
      printf("Error: Malformatted Input.\n");
      exit(0);
    }
    if (!strcmp(component->operator, "IN")) {
      no_inputs++;
    }
    if (component->next == NULL) {
      break;
    }
    component = component->next;
  }
  return no_inputs;
}

int factorial(int n) {
  if (n == 1) {
    return 1;
  }
  else {
    return factorial(n-1)*n;
  }
}

Circuit run(Circuit circuit, int no_inputs, int inputs[no_inputs], int print) {
  int no_wires = 1;
  Wire* wire_ptr = circuit.start->output;
  while (wire_ptr->next != circuit.start->output) {
    wire_ptr->previous_value = 0;
    wire_ptr = wire_ptr->next;
    no_wires++;
  }
  wire_ptr = circuit.start->output;
  int values[no_wires];
  int w;
  for (w = 0; w < no_wires; w++) {
    values[w] = wire_ptr->value;
    wire_ptr = wire_ptr->next;
  }
  int stable = 0;
  //Use factorial of no_wires as upper bound
  int i;
  for (i = 0; i < factorial(no_wires); i++) {
    if (stable) {
      break;
    }
    circuit = run_circuit(no_inputs, inputs, circuit);
    stable = 1;
    for (w = 0; w < no_wires; w++) {
      if (values[w] != wire_ptr->value) {
        stable = 0;
      }
      values[w] = wire_ptr->value;
      wire_ptr = wire_ptr->next;
    }
  }
  if (print){
    print_state(circuit, stable);
  }
  return circuit;
}

void print_union(Atom* root_atom) {
  Atom* atom_ptr = root_atom;
  printf("(");
  while (atom_ptr->next != NULL) {
    if (atom_ptr->_not) {
      printf("¬");
    }
    printf("%s ∨ ", atom_ptr->name);
    atom_ptr = atom_ptr->next;
  }
  if (atom_ptr->_not) {
    printf("¬");
  }
  printf("%s)", atom_ptr->name);
}

void find_expression(Circuit circuit, int no_inputs) {
  int inputs[no_inputs];
  char* input_names[no_inputs];
  Component* comp_ptr = circuit.start;
  int i = 0;
  while (1) {
    if (!strcmp(comp_ptr->operator, "IN")) {
      input_names[i] = comp_ptr->output->name;
      i++;
    }
    if (comp_ptr->next != NULL) {
      comp_ptr = comp_ptr->next;
    }
    else {
      break;
    }
  }
  for (i = 0; i < no_inputs; i++) {
    inputs[i] = 0;
  }
  int a = 0;
  Atom* atoms[(no_inputs*no_inputs)];
  circuit = run(circuit, no_inputs, inputs, 0);
  i = 0;
  if (!circuit.out->value) {
    Atom* root_atom = (Atom*)malloc(sizeof(Atom));
    atoms[a] = root_atom;
    a++;
    Atom* atom_ptr = root_atom;
    if ((inputs[i])) {
      root_atom->_not = 1;
    }
    else {
      root_atom->_not = 0;
    }
    root_atom->name = input_names[i];
    i++;
    while (i < no_inputs - 2) {
      Atom* atom = (Atom*)malloc(sizeof(Atom));
      if ((inputs[i])) {
        atom->_not = 1;
      }
      else {
        atom->_not = 0;
      }
      atom->name = input_names[i];
      atom_ptr->next = atom;
      atom_ptr = atom_ptr->next;
      i++;
    }
    Atom* last_atom = (Atom*)malloc(sizeof(Atom));
    if ((inputs[i])) {
      last_atom->_not = 1;
    }
    else {
      last_atom->_not = 0;
    }
    last_atom->name = input_names[i];
    atom_ptr->next = last_atom;
  }
  int r;
  int j;
  for (r = 0; r < (no_inputs*no_inputs) - 1; r++) {
    for (i = no_inputs - 1; i >= 0; i--) {
      if (inputs[i] != 1) {
        inputs[i] = 1;
        for (j = no_inputs - 1; j > i; j--) {
          inputs[j] = 0;
        }
        break;
      }
    }
    circuit = run(circuit, no_inputs, inputs, 0);
    i = 0;
    if (!circuit.out->value) {
      Atom* root_atom = (Atom*)malloc(sizeof(Atom));
      atoms[a] = root_atom;
      a++;
      Atom* atom_ptr = root_atom;
      if ((inputs[i])) {
        root_atom->_not = 1;
      }
      else {
        root_atom->_not = 0;
      }
      root_atom->name = input_names[i];
      i++;
      while (i < no_inputs - 2) {
        Atom* atom = (Atom*)malloc(sizeof(Atom));
        if ((inputs[i])) {
          atom->_not = 1;
        }
        else {
          atom->_not = 0;
        }
        atom->name = input_names[i];
        atom_ptr->next = atom;
        atom_ptr = atom_ptr->next;
        i++;
      }
      Atom* last_atom = (Atom*)malloc(sizeof(Atom));
      if ((inputs[i])) {
        last_atom->_not = 1;
      }
      else {
        last_atom->_not = 0;
      }
      last_atom->name = input_names[i];
      atom_ptr->next = last_atom;
    }
  }
  if (atoms[0]->name != NULL) {
    printf("out = ");
    print_union(atoms[0]);
    free_atom(atoms[0]);
  }
  for (int f = 1; f <= a - 1; f++) {
    if (atoms[f]->name == NULL) {
      break;
    }
    printf(" ∧ ");
    print_union(atoms[f]);
    free_atom(atoms[f]);
  }
  printf("\n");
}



/*
typedef struct Phrases {
  struct Pharses* phrase_one;
  struct Phrases* phrase_two;
  char* atom_one;
  char* atom_two;
  int not_one;
  int not_two;
} Phrase;
*/
