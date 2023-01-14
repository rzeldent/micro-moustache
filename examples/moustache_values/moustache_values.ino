// moustache_values
// This shows how to use new moustache functions now in moustache_code.h

#include <moustache.h>
#include <moustache_code.h>

const char *available_value = "{{number}} is available";
const char *current_value = "Current value is {{number}}";
const char *number_display = " {{number}}";
const char *point_display  = " ( {{x}} , {{y}} ) ";

// Declaration of the user's own error string - test error can be whatever is needed.
const moustache_variable_t test_error[] ={ {"error", "test_error" } };
// Definitions for each element
const moustache_variable_t array0[] = { {"array", "0, zero"} };
const moustache_variable_t array1[] = { {"array", "1, one"} };
const moustache_variable_t array2[] = { {"array", "2, two"} };
// Array of elements (optional)  
const moustache_variable_t *array[]= { array0, array1, array2 };

int number = 0;    // variable to store the number
const moustache_variable_t number_value[] = { { "number", String(number) } };

double x = 0, y = 0;
const moustache_variable_t point_value[] = { { "x", String(x) }, { "y", String(y) } };

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial) { }
 
  Serial.println("Moustache code examples");
  // Demonstration of an array of moustache_variable_t arrays.
  size_t n = moustache_size(array);
  Serial.println("List of available values");
  for (size_t i = 0; i < n; i++ ) {
    const moustache_variable_t what[] =  { *array[i] };
    Serial.println(moustache_render(available_value,what)); 
  }
  Serial.println("Demo of range error");
  Serial.println(moustache_render_array(current_value,array,3));

  Serial.println(moustache_render(number_display,number_value));
  
  number = 2;
  // This changes the value in the next rendered output.
  number_value[0].value = String(number);
  Serial.println(moustache_render(number_display,number_value));
  number = 3;
  // The change can also be made like this:
  moustache_value(number_value,0,number);
  Serial.println(moustache_render(number_display,number_value));
  number = 4;
  // The change can be made in one step like this:
  Serial.println(moustache_render_value(number_display,number_value,0,number));
  // If the index value is out of range an error will be reported.
  Serial.println("Test of value range error");
  Serial.println(moustache_render_value(number_display,number_value,1,number));

  // In this example two values, x and y can be changed.
  Serial.println(moustache_render(point_display,point_value));
  x = 3;
  y = 4;  
  point_value[0].value = String(x);
  point_value[1].value = String(y);
  Serial.println(moustache_render(point_display,point_value));
  moustache_value(point_value,0,y);
  moustache_value(point_value,1,x);
  Serial.println(moustache_render(point_display,point_value));
 
  Serial.println(moustache_render_value2(point_display,point_value,0,x,1,y));

  Serial.println(moustache_render_value2(point_display,point_value,2,x,1,y));
  Serial.println(moustache_render_value2(point_display,point_value,0,x,3,y));

  Serial.println(moustache_render(moustache_error,test_error));

 }

void loop() {
  // put your main code here, to run repeatedly:
}