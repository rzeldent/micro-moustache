// micro-moustache basic example
//
// Replace strings and conditional include / exclude text
//
// For examples can be found in the original documentation: http://mustache.github.io/mustache.5.html

#include <Arduino.h>
#include <moustache.h>

void setup()
{
    const char *taxes =
        "Hello {{name}}\n"
        "You have just won {{value}} dollars!\n"
        "{{#in_ca}}"
        "Well, {{taxed_value}} dollars, after taxes.\n"
        "{{/in_ca}}"
        "{{^in_ca}}"
        "And no taxes paid!\n"
        "{{/in_ca}}";

    moustache_variable_t substitutions[] = {
        {"name", "Chris"},
        {"value", String(10000)},
        {"taxed_value", String(10000 - (10000 * 0.4))},
        {"in_ca", String(true)}};

    auto result = moustache_render(taxes, substitutions);
    Serial.println(result);

    substitutions[3].value = String(false);
    result = moustache_render(taxes, substitutions);
    Serial.println(result);
}

void loop()
{
}