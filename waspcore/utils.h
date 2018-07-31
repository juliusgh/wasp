#ifndef WASP_UTILS_H
#define WASP_UTILS_H
#include "waspcore/wasp_bug.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <stdio.h>  // windows exponent configure
#include "waspcore/decl.h"

namespace wasp
{
/**
 * @brief strip_quotes removes single or double quotes
 * @param s the string to have the quotes removes from.
 * @return a string with 1 set of single or double quotes removed
 */
WASP_PUBLIC std::string strip_quotes(const std::string& s);

/**
 * @brief trim removes the given character set leading and trailing from the
 * given string
 * @param s the string to trim potential prefix and suffix characters
 * @param char_set the set of characters to be remove
 * @return the string with all leading and trailing character set removed
 */
WASP_PUBLIC std::string trim(std::string s, const std::string& char_set);

/**
 * @brief dir_name convenience method to acquire the directory name for the
 * given path
 * @param path the path from which the directory name will be acquired
 * @return path minus the last '/name' or \\name
 */
WASP_PUBLIC std::string dir_name(const std::string& path);
/**
 * @brief xml_escape_data replaces string with escaped versions of the five
 * characters that must be escaped in XML document data ( &, \, ", <, > )
 * to be used on xml data
 * @param src the string in which to have the characters escaped
 * @return a string with characters escaped
 */
WASP_PUBLIC std::string xml_escape_data(const std::string& src);
/**
 * @brief json_escape_string replaces string with escaped versions of the two
 * characters that must be escaped in JSON document string ( \, " )
 * to be used on json data
 * @param src the string in which to have the characters escaped
 * @return a string with characters escaped
 */
WASP_PUBLIC std::string json_escape_string(const std::string& src);
/**
 * @brief xml_escape_name replaces string with versions that will parse
 * as node names in xml
 * to be used on xml node names
 * @param src the string in which to have the characters replaced
 * @return a string with characters replaced
 */
WASP_PUBLIC std::string xml_escape_name(const std::string& src);

/**
 * @brief to_type acquire the data typed as the requested type
 * @param result the typed result
 * @param ok optional boolean return value indicating if conversion was
 * successful.
 */
template<typename T>
WASP_PUBLIC void to_type(T& result, const std::string& data, bool* ok = nullptr)
{
    std::stringstream str;
    str << data;
    str >> result;
    if (ok)
        *ok = !(str.bad() || str.fail());
}
/**
 *Specialization on string so as to avoid whitespace issues in stream operator>>
 */
template<>
inline WASP_PUBLIC void
to_type<std::string>(std::string& result, const std::string& data, bool* ok)
{
    result = data;
    if (ok)
        *ok = true;
}
template<>
inline WASP_PUBLIC void
to_type<int>(int& result, const std::string& data, bool* ok)
{
    try
    {
        result = std::stoi(data.c_str());
        if (ok)
            *ok = true;
    }
    catch (...)
    {
        result = 0;
        if (ok)
            *ok = false;
    }
}
template<>
inline WASP_PUBLIC void
to_type<double>(double& result, const std::string& data, bool* ok)
{
    try
    {
        result = std::stod(data.c_str());
        if (ok)
            *ok = true;
    }
    catch (...)
    {
        result = 0.0;
        if (ok)
            *ok = false;
    }
}

template<typename T>
static std::string to_string(T v, bool* ok = nullptr)
{
    std::stringstream s;
    s << v;
    if (ok)
        *ok = !(s.bad() || s.fail());
    return s.str();
}

/**
 * @brief to_xml walk the given node and emit xml elements into out
 * @param node the node to convert to xml
 * @param out the stream to emit the xml
 * @param emit_decorative indicates whether to emit decorative nodes to xml
 * stream
 * @param space amount of whitespace to prefix to a line
 */
template<class TAdapter>
inline WASP_PUBLIC void to_xml(const TAdapter& node,
                               std::ostream&   out,
                               bool            emit_decorative = true,
                               std::string     space           = "")
{
    bool decorative = node.is_decorative();
    if (decorative && !emit_decorative)
        return;
    size_t child_count = node.child_count();
    // print element name and location
    out << space << "<" << xml_escape_name(node.name());
    // capture location if it is a leaf
    if (child_count == 0)
        out << " loc=\"" << node.line() << "." << node.column() << "\"";
    if (decorative)
        out << " dec=\"" << std::boolalpha << decorative << "\"";
    out << ">";
    if (child_count == 0)
        out << xml_escape_data(node.data());
    else
        out << std::endl;
    // recurse into each child
    for (size_t i = 0; i < child_count; ++i)
    {
        to_xml(node.child_at(i), out, emit_decorative, space + "  ");
    }

    // close the element
    if (child_count > 0)
        out << space;
    out << "</" << xml_escape_name(node.name()) << ">" << std::endl;
}

/**
 * @brief wildcard_string_match
 * @param first wildcarded pattern to match
 * @param second literal string to determine match
 * @return true, iff, second matches first
 * NOTE: gratefully acquired from :
 * http://www.geeksforgeeks.org/wildcard-character-matching/
 */
inline WASP_PUBLIC bool wildcard_string_match(const char* first,
                                              const char* second)
{
    // If we reach at the end of both strings, we are done
    if (*first == '\0' && *second == '\0')
        return true;

    // Make sure that the characters after '*' are present
    // in second string. This function assumes that the first
    // string will not contain two consecutive '*'
    if (*first == '*' && *(first + 1) != '\0' && *second == '\0')
        return false;

    // If the first string contains '?', or current characters
    // of both strings match
    if (*first == '?' || *first == *second)
        return wildcard_string_match(first + 1, second + 1);

    // If there is *, then there are two possibilities
    // a) We consider current character of second string
    // b) We ignore current character of second string.
    if (*first == '*')
        return wildcard_string_match(first + 1, second) ||
               wildcard_string_match(first, second + 1);
    return false;
}
template<class TV>
WASP_PUBLIC std::string info(const TV& view)
{
    if (view.is_null())
        return "@TreeView(null)";
    else
        return "@TreeView(l=" + std::to_string(view.line()) + "," +
               std::to_string(view.column()) +
               ": t=" + std::to_string(view.node_index()) + ", i" +
               std::to_string(view.type()) + ", n'" + view.name() + "'" + ")";
}

template<class Pool>
WASP_PUBLIC void print_from(std::ostream& stream,
                            const Pool&   node_pool,
                            size_t        node_index,
                            size_t&       last_line,
                            size_t&       last_column)
{
    size_t child_count = node_pool.child_count(node_index);
    if (child_count == 0)  // fast check
    {
        if (node_pool.is_leaf(node_index) == false)
            return;
        //
        // determine distance from previous
        //
        size_t line = node_pool.line(node_index);
        wasp_check(line != size_t(-1));
        size_t column = node_pool.column(node_index);
        wasp_check(column != size_t(-1));
        size_t ldiff;
        if (line >= last_line)
            ldiff = line - last_line;
        else
            ldiff = 0;

        size_t cdiff;
        if (ldiff > 0)
            cdiff = column - 1;
        else if (column >= last_column)
            cdiff = column - last_column;
        else
            cdiff = 1;

        //        if( cdiff <= column || cdiff <= lastColumn )
        // write preceeding newlines
        for (size_t i = 0; i < ldiff; i++)
            stream << std::endl;
        //
        if (cdiff > 0)
            stream << std::string(cdiff, ' ');
        const std::string& data = node_pool.data(node_index);
        if (!(data.length() == 1 && data[0] == '\n'))
            stream << data;

        int newLinePrintedCount = 0;
        if (!(data.length() == 1 && data[0] == '\n'))
        {
            newLinePrintedCount = std::count(data.begin(), data.end(), '\n');
        }
        last_line = line + newLinePrintedCount;
        // todo - data could span newlines making column some remainder
        last_column = column + data.size();
        return;
    }
    for (size_t i = 0, cc = child_count; i < cc; i++)
    {
        size_t child_index = node_pool.child_at(node_index, i);
        print_from(stream, node_pool, child_index, last_line, last_column);
    }
}
template<class TAdapter>
WASP_PUBLIC void print_from(std::ostream&   out,
                            const TAdapter& node_view,
                            size_t&         last_line,
                            size_t&         last_column)
{
    print_from(out, *node_view.node_pool(), node_view.node_index(), last_line,
               last_column);
}

template<class TAdapter>
WASP_PUBLIC void print(std::ostream& out, const TAdapter& node_view)
{
    size_t l = node_view.line(), c = node_view.column();
    print_from(out, *node_view.node_pool(), node_view.node_index(), l, c);
}

template<class TAdapter>
WASP_PUBLIC typename TAdapter::Collection
non_decorative_children(const TAdapter& node)
{
    typename TAdapter::Collection results;
    for (std::size_t i = 0, count = node.child_count(); i < count; ++i)
    {
        const auto& child = node.child_at(i);
        if (!child.is_decorative())
            results.push_back(child);
    }
    return results;
}
template<class TAdapter>
WASP_PUBLIC TAdapter first_non_decorative_child_by_name(const TAdapter& node,
                                            const std::string& name)
{
    for (std::size_t i = 0, count = node.child_count(); i < count; ++i)
    {
        const auto& child = node.child_at(i);
        if (!child.is_decorative())
        {
            if (name == child.name())
            {
                return child;
            }
        }
    }
    return TAdapter();  // null node
}

template<class TAdapter>
WASP_PUBLIC size_t non_decorative_children_count(const TAdapter& node)
{
    size_t result = 0;
    for (std::size_t i = 0, count = node.child_count(); i < count; ++i)
    {
        const auto& child = node.child_at(i);
        if (!child.is_decorative())
            ++result;
    }
    return result;
}

template<class TAdapter>
WASP_PUBLIC std::string last_as_string(const TAdapter& node, bool* ok)
{
    size_t count = node.child_count();
    if (count > 0)
    {
        return last_as_string(node.child_at(count - 1),ok);
    }
    return node.to_string(ok);
}
}
#endif
