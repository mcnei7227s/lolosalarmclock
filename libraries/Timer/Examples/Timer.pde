#include

// This is a nasty hack to resolve some weirdo errors in where we
// redefine some system types and functions, for some unknown reason. Rather than
// modify the header file to resolve this conflict, we remove the redefinitions
// here.
#undef int
#undef abs
#undef double
#undef float
#undef round
// End Hack

Timer RTOS = Timer();
timer_h rep;

void setup() {
Serial.begin(9600);

RTOS.once(5, t_once);
rep = RTOS.repeat(1, t_rep);
}

void t_once(void) {
Serial.println("World!");
RTOS.destroy(rep);
}

void t_rep(void) {
Serial.print("\nHello ");
}

void loop() { /* nil */ }
