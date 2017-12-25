/*
 * initial stack is used primarily by dumping application,
 * if it is, malloc is always from heap, and _stksize is init
 * from initial_stack (to preserve the value in the undumped run)
 */

/* leave this as an uninitialized variable, so it goes in the bss */
long _initial_stack;
