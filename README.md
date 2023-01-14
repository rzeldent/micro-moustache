# Micro Moustache

![status badge](https://github.com/rzeldent/micro-moustache/actions/workflows/main.yml/badge.svg?event=push)

Minimal moustache implementation for Arduino and other micro controllers.

## Introduction
Moustache is a logic-less template processor. This makes it easy to replace text or in- or exclude parts of text.
This is especially handy for creating dynamic HTML!

This library is a minimal implementation of the original library. The original library is available for many platforms but, to my knowledge, not Arduino... Until now!

The original project can be found at [http://mustache.github.io/](http://mustache.github.io/) and the manual at [http://mustache.github.io/mustache.5.html](http://mustache.github.io/mustache.5.html).
However, in this library only a few functionalities are implemented.

Extensions have been added without changing the original interface and this is now Version 1.1.0.

There is a memory overhead in working with this code as compared to direct outputs.

## Passing variables to the template

Variables need to be present in an array of the structure ```moustache_variable_t```. The original implementation of Moustache uses json but this is overkill for micro controllers. Micro Moustache uses an array of a structure that holds a key and a value.
```
typedef struct moustache_variable
{
    const char *key;
    const String &value;
} moustache_variable_t;
```

Some features like looping are not possible using this data structure but keeps it simple.

The value is defined as a string. This makes it easy to convert values to string as a lot of conversions are already implemented in the String class.

A typical definition of the variables will look like this. The immediate values below can of course be replaced by variables.
```
  const moustache_variable_t substitutions[] = {
      {"Version", "1.0"},
      {"LoggedIn", String(false)},
      {"UserName", String("Jake")},
      ...
      ...
    };
```

## Implemented features
The following sections contain a list of implemented features. The examples are take from the official Moustache documentation.

### Replacing variables
Variables can be replaced using the syntax: ```{{variable}}```.

example:
```
Hello {{name}}
You have just won {{value}} dollars!
```
with variables:
```
  const moustache_variable_t substitutions[] = {
    {"name", "Chris"},
    {"value", String(10000)}
  };
```
will render
```
Hello Chris
You have just won 10000 dollars!
```

### Include Section
Sections can be included using the syntax: ```{{#expr}}Text to be included{/expr}```.

The expression needs to be a boolean expression and, as the String class is used, booleans convert to "0" or "1".

example:
```
{{#in_ca}}
Well, {{taxed_value}} dollars, after taxes.
{{/in_ca}}
```
with variables:
```
  const moustache_variable_t substitutions[] = {
    {"taxed_value", String(10000 - (10000 * 0.4))},
    {"in_ca", String(true)}
  };
```
will render
```
Well, 6000.0 dollars, after taxes.
```

### Inverted Section
Sections can be included using the syntax: ```{{^expr}}Text to be included{/expr}```.

The expression needs to be a boolean expression and, as the String class is used, booleans convert to "0" or "1".

example:
```
{{#in_ca}}
Well, {{taxed_value}} dollars, after taxes.
{{/in_ca}}
{{^in_ca}}
And no taxes paid!
{{/in_ca}}
```
with variables:
```
  const moustache_variable_t substitutions[] = {
    {"taxed_value", String(10000 - (10000 * 0.4))},
    {"in_ca", String(false)}
  };
```
will render
```
And no taxes paid!
```

## Extensions

What follows is a series of extensions to micro-moustache with an extra example to show how to use them.

The idea of the extensions has been to allow the rendering of values calculated at run time in the code.

In addition there are some declarations for error messages which are used internally by some of the new functions and also available for users to build their own error messages.

All the extension functions are in the form ```moustache_xxxx```.

To use the extensions add this include statement:
```
#include <moustache_code.h>
```
The key idea of the extensions is that every declaration of a ```moustache_variable_t``` is an array of pairs of values.
```
  const moustache_variable_t something[] = { { "a", "one" } {"b", "two"} };
```

### moustache_size and changing values

The size of the array can be found using this function:
```
size_t n = moustache_size(something);
```
Using this it is possible to change one of the values in the declared variable.
Consider these declarations:
```
const char *number_display = " {{number}}";
int number = 0;    // variable to store the number
const moustache_variable_t number_value[] = { { "number", String(number) } };
```
In code it is possible to change the value held in number at run time:
```
  number = 2;
  number_value[0].value = String(number);
  Serial.println(moustache_render(number_display,number_value));
```
There is a new function moustache_value which does the value change as long as the conversion to String works.
```
  number = 3;
  moustache_value(number_value,0,number);
  Serial.println(moustache_render(number_display,number_value));
```
The value change and render are combined in one function using moustache_render_value.
```
  number = 4;
  Serial.println(moustache_render_value(number_display,number_value,0,number));
```
The integer index value is checked in the function and the incorrect value 1 will give an error message. Seel the later section on error messages.
```
  Serial.println("Test of value range error");
  Serial.println(moustache_render_value(number_display,number_value,1,number));
```

### Moustache Arrays

Moustache can be extended to have arrays of arrays of type ```moustache_variable_t```

```
// Definitions for each element
const moustache_variable_t array0[] = { {"array", "0, zero"} };
const moustache_variable_t array1[] = { {"array", "1, one"} };
const moustache_variable_t array2[] = { {"array", "2, two"} };
// Array of elements (optional)  
const moustache_variable_t *array[]= { array0, array1, array2 };
```
The elements of the array can be accessed like this:
```
 size_t n = moustache_size(array);
  Serial.println("List of available values");
  for (size_t i = 0; i < n; i++ ) {
    const moustache_variable_t what[] =  { *array[i] };
    Serial.println(moustache_render(available_value,what)); 
  }
```
The elements of the arrag can be accessed individually using ```moustache_render_array ```

```
Serial.println(moustache_render_array(current_value,array,2));
```

Expressions such as this will print an error for values such as 3 which is not a valid index value.
```
Serial.println(moustache_render_array(current_value,array,3));
```

### Error messages

Error messages are implemented as a series of declarations in ```moustache_code.h```

```
const char *moustache_error   = "** moustache error : {{error}} **";
const char *moustache_error_i = "** moustache error : {{error}} {{i}} **";
const char *moustache_error_j = "** moustache error : {{error}} {{j}} **";
const char *moustache_error_k = "** moustache error : {{error}} {{k}} **";
size_t moustache_i = 0; // default error number does not pollute simple namespace
```

There are also some declarations which are used internally in some of the functions to report index range errors.

```
const moustache_variable_t array_error_i[] = { { "error", "array range error, i ="}, { "i", String(moustache_i)} };
const moustache_variable_t value_error_i[] = { { "error", "value range error, i ="}, { "i", String(moustache_i)} };
const moustache_variable_t value_error_j[] = { { "error", "value range error, j ="}, { "j", String(moustache_i)} };
const moustache_variable_t value_error_k[] = { { "error", "value range error, k ="}, { "k", String(moustache_i)} };
```

### Examples

basic has examples of the original moustache functions.

moustache_values has examples of the extensions in ```moustache_values.h```

## Collaboration
Collaboration is appreciated and bug reports or feature requests are welcome!
