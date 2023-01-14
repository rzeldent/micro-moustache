// moustache_code.h

// Extra codes for moustache.
// These extend what can be done with Arduino moustache within the limitations of the Arduino compiler.
// Some of these functions will not compile in an *.ino file for some reason.
// That is why they have been moved into a header file where they work.

#ifndef MOUSTACHE_CODE_H
#define MOUSTACHE_CODE_H

// First attempt at moustache reporting run time errors.
const char *moustache_error   = "** moustache error : {{error}} **";
const char *moustache_error_i = "** moustache error : {{error}} {{i}} **";
const char *moustache_error_j = "** moustache error : {{error}} {{j}} **";
const char *moustache_error_k = "** moustache error : {{error}} {{k}} **";

size_t moustache_i = 0; // default error number does not pollute simple namespace
const moustache_variable_t array_error_i[] = { { "error", "array range error, i ="}, { "i", String(moustache_i)} };
const moustache_variable_t value_error_i[] = { { "error", "value range error, i ="}, { "i", String(moustache_i)} };
const moustache_variable_t value_error_j[] = { { "error", "value range error, j ="}, { "j", String(moustache_i)} };
const moustache_variable_t value_error_k[] = { { "error", "value range error, k ="}, { "k", String(moustache_i)} };
/* moustache error reporting:
   Define errors like this:
   const moustache_variable_t test_error[] ={ {"error", "test_error" } };
   At run time do this:
   Serial.println(moustache_render(moustache_error,test_error));
   
   See examples below for error reporting when a range error occures for value or array.
*/

// This returns the size of a moustache_variable_t array and also of an array of arrays.
// It can be used to control output from rendering the array of arrays
template <typename T, size_t n>
size_t moustache_size(T (&values)[n])
{
  return n;
}

// Note: all value changes are checked for the size of values.
template <typename T, typename V, size_t n>
void moustache_value(T (&values)[n], size_t i, const V &v)
{
  if (i < n)
    values[i].value = String(v);
}

// This will not compile in the *.ino file with format as the first argument - why is not clear to me.
// It works in a header file.
// This will change one value and render the file in one go.
template <typename T, typename V, size_t n>
String moustache_render_value(const String &format, T (&values)[n], size_t i, const V &v)
{
    if (i >= n) { 
      moustache_value(value_error_i, 1, i);     
      return moustache_render(moustache_error_i,value_error_i);
    }

    values[i].value = String(v);
    return moustache_render(format,values);
}

// This will change two values and render the file in one go. The variable types need not be the same.
template <typename T, typename V, typename W, size_t n>
String moustache_render_value2(const String &format, T (&values)[n], size_t i, const V &v, size_t j, const W &w)
{
    if (i >= n) { 
      moustache_value(value_error_i, 1, i);     
      return moustache_render(moustache_error_i, value_error_i);
    }

    values[i].value = String(v);
    if (j >= n) {
      moustache_value(value_error_j, 1, j);     
      return moustache_render(moustache_error_j, value_error_j);
    }

     values[j].value = String(w);
     return moustache_render(format,values);
}

// This works on an array of values with the same keyword.
template <typename T, size_t n>
String moustache_render_array(const String &format, T (&values)[n], size_t i)
{
    if (i >= n) { 
      moustache_value(value_error_i, 1, i);     
      return moustache_render(moustache_error_i, value_error_i);
    }

    const moustache_variable_t what[] =  { *values[i] };    
    return moustache_render(format, what);
}

// This works on an array of values with the same keyword and also changes a value in the chosen output
template <typename T, typename V, size_t n>
String moustache_render_array_value(const String &format, T (&values)[n], size_t i, size_t j, const V &v)
{
    if (i >= n) { 
      moustache_value(value_error_i, 1, i);     
      return moustache_render(moustache_error_i, value_error_i);
    }

    const moustache_variable_t what[] =  { *values[i] };    
    return moustache_render_value(format,what, j, v);
}
#endif

