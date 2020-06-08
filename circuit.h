typedef struct Wires {
  char* name;
  int value;
  int previous_value;
  int input;
  struct Wires* next;
} Wire;

typedef struct Components {
  int no_of_input;
  Wire* first_input;
  Wire* second_input;
  Wire* output;
  char* operator;
  struct Components* next;
} Component;

typedef struct Circuits {
  Component* start;
  Wire* out;
  int length;
} Circuit;

typedef struct Atom {
  char* name;
  int _not;
  struct Atom* next;
} Atom;

void free_atom(Atom* atom);

Wire* add_wire(Wire* wire_strt, Wire* wire);

Circuit read_input(Circuit circuit);

void free_circuit(Circuit circuit);

int get_no_inputs(Circuit circuit);

Circuit add_component(Component* component, Circuit circuit);

void print_state(Circuit circuit, int stable);

void print_headers(Circuit circuit);

Circuit run_circuit(int no_inputs, int inputs[no_inputs], Circuit circuit);

Circuit run(Circuit circuit, int no_inputs, int inputs[no_inputs], int print);

int factorial(int n);

void find_expression(Circuit circuit, int no_inputs);
