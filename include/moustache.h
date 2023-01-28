#pragma once

#include <WString.h>

typedef struct moustache_variable
{
    const char *key;
    String value;
} moustache_variable_t;

template <size_t n>
inline String moustache_render(const String &format, const moustache_variable_t (&values)[n])
{
    auto s = String(format);
    // Conditional sections
    for (size_t i = 0; i < n; i++)
    {
        // Include Section {{#expr}}
        auto match_section_begin = "{{#" + String(values[i].key) + "}}";
        // Inverted section {{^expr}}
        auto match_section_inverted_begin = "{{^" + String(values[i].key) + "}}";
        // End section {{/expr}}
        auto match_section_end = "{{/" + String(values[i].key) + "}}";
        while (true)
        {
            bool inverted = false;
            auto first = s.indexOf(match_section_begin);
            if (first < 0)
            {
                inverted = true;
                first = s.indexOf(match_section_inverted_begin);
                if (first < 0)
                    break;
            }

            auto second = s.indexOf(match_section_end, first + match_section_begin.length());
            if (second < 0)
                break;

            // Arduino returns 0 and 1 for bool.toString()
            if ((!inverted && (values[i].value == "1")) || (inverted && (values[i].value == "0")))
                s = s.substring(0, first) + s.substring(first + match_section_begin.length(), second) + s.substring(second + match_section_end.length());
            else
                s = s.substring(0, first) + s.substring(second + match_section_end.length());
        }
    }

    // Replace variables {{variable}}
    for (size_t i = 0; i < n; i++)
        s.replace("{{" + String(values[i].key) + "}}", values[i].value);

    return s;
}

// Update one value at index  and render
template <size_t n>
inline String moustache_render(const String &format, const moustache_variable_t (&values)[n], String updates, size_t index = 0)
{
#if DEBUG
    if (new_index >= n)
        return "Index out of range";
#endif

    values[index] = updates;
    return moustache_render(format, values);
}

// Update one value at key and render
template <size_t n>
inline String moustache_render(const String &format, const moustache_variable_t (&values)[n], String update, const char *key)
{
    size_t index = 0;
    while (index < n && strcmp(values[index].key, key))
        index++;

    return moustache_render(format, values, update, index);
}

// Update an array of values starting at start_index and render
template <size_t n, size_t m>
inline String moustache_render(const String &format, const moustache_variable_t (&values)[n], String updates[m], size_t start_index = 0)
{
#if DEBUG
    if (start_index + m >= n)
        return "More values than original array values";
#endif

    for (size_t i = 0; i < m; ++i)
        values[i + start_index] = updates[i];

    return moustache_render(format, values);
}

// Update an array of values starting at key and render
template <size_t n, size_t m>
inline String moustache_render(const String &format, const moustache_variable_t (&values)[n], String updates[m], const char *key)
{
    size_t index = 0;
    while (index < n && strcmp(values[index].key, key))
        index++;

    return moustache_render(format, values, updates, index);
}