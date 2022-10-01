# Micro Moustache
Minimal moustache implementation for Arduino and other micro controllers.

## Introduction
Moustache is a logic-less template processor. This makes it easy to replace text or in- or exclude parts of text.
This is especially handy for creating dynamic HTML!

This library is a minimal implementation of the original library. The original library is available for many platforms but, to my knowledge, not Arduino... Until now!

The original project can be found at [http://mustache.github.io/](http://mustache.github.io/) and the manual at [http://mustache.github.io/mustache.5.html](http://mustache.github.io/mustache.5.html).
However, in this library only a few functionalities are implemented.

## Passing variables to the template

Variables need to be present in an array of the structure ```moustache_variable_t```. The original implementation of Moustache uses json but this is overkill for micro controllers. Micro Moustache uses an array of a structure that holds a key and a value.
```
typedef struct moustache_variable
{
    const char *key;
    const String &value;
} moustache_variable_t;
```

Some flexibility like looping is lost but keeps the data structure simple.

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