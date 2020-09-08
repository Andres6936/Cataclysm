# Exceptions

Defines several standard classes used for reporting exceptions. The classes form a derivation hierarchy all 
derived from class exception and include two general types of exceptions: logical errors and run-time errors.

- The logical errors are caused programmer mistakes. They derive from the base class logic_error:

- The run-time errors occur because of mistakes in either the library functions or in the run-time system.
 They derive from the base class runtime_error:

### domain_error

The class serves as the base class for all exceptions thrown to report a domain error.

### invalid_argument

The class serves as the base class for all exceptions thrown to report an invalid argument.

### length_error

The class serves as the base class for all exceptions thrown to report an attempt to generate an object too long to be specified.

### logic_error

The class serves as the base class for all exceptions thrown to report errors presumably detectable
before the program executes, such as violations of logical preconditions.

### out_of_range

The class serves as the base class for all exceptions thrown to report an argument that is out of its valid range.

----

### overflow_error

The class serves as the base class for all exceptions thrown to report an arithmetic overflow.

### range_error

The class serves as the base class for all exceptions thrown to report a range error.

### runtime_error

The class serves as the base class for all exceptions thrown to report errors presumably detectable only when the program executes.

### underflow_error

The class serves as the base class for all exceptions thrown to report an arithmetic underflow.