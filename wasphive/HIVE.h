/*
 * File:   HIVE.h
 * Author: Brandon Langley, Robert A. Lefebvre (Migrated to WASP)
 *
 * Created on September 17, 2014, 8:19 PM
 */

#ifndef WASP_HIVE_H
#define WASP_HIVE_H
#include <algorithm>
#include <atomic>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <memory>
#include <numeric>
#include <cstdlib>
#include <string>
#include <sstream>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <vector>
#include "waspsiren/SIRENInterpreter.h"
#include "waspsiren/SIRENResultSet.h"

#include "waspcore/decl.h"

namespace wasp
{
/**
 * @brief The HIVE class contain the Hierachical Input Validation Engine
 * algorithms
 * HIVE requires the following APIs be followed.
 *
 * ==== Required InputAdapter API====
 * TAdapter TAdapter::parent()const - acquire parent tree node
 * bool TAdapter::is_null()const - indicates if the node is null (not backed by
 * document)
 * std::string TAdapter::path()const - acquire the path of the node in the tree
 * std::string TAdapter::name()const - acquire the quoteless name of the node
 * size_t TAdapter::line()const - acquire the line on which the node occurrs in
 * the document
 * size_t TAdapter::column()const - acquire the column on which the node occurrs
 * in the document
 * TAdaper& TAdapter::operator=(const TAdapter & orig) - node's assignment
 * operator
 * bool TAdapter::operator==(const TAdapter & orig) - node's equality operator
 * bool TAdapter::operator<(const TAdapter & orig) - node's less than operator
 * using lexical order
 * std::vector<TAdapter> TAdapter::child_by_name(const std::string & name,
 * size_t limit)const
 * int TAdapter::to_int()const - converts the node's data to an integer
 * double TAdapter::to_double()const - converts the node's data to a double
 * std::string TAdapter::to_string()const - converts the node's data to a
 * quoteless string
 * size_t TAdapter::child_count()const - the child node count for the current
 * node
 * size_t TAdapter::child_count_by_name(const std::string& name)const - the
 * child node count where the children have the given name
 * std::string data()const - the raw data of the node
 * std::vector<TAdapter> TAdapter::non_decorative_children()const - acquires a
 * vector of all children that are non-decorative
 * TAdapter TAdaper::first_non_decorative_child_by_name(const std::string &
 * name) - acquires the first non decorative child node with the given name
 * size_t TAdapter::non_decorative_children_count()const - acquires the count of
 * the non decorative children
 * bool TAdapter::is_decorative()const - determine if the node is decorative
 * ==== Required SchemaAdapter API ====
 *  !! All the above and the following !!
 * std::string TAdapter::id()const - acquire the id as a quoteless string
 * TAdapter TAdapter::id_child()const - acquire the child tree node deemed the
 * current node's identifier
 */
class WASP_PUBLIC HIVE
{
  public:
    HIVE();
    HIVE(const std::atomic<bool>& stop);
    ~HIVE();
    template<class SchemaAdapter, class InputAdapter>
    bool validate(SchemaAdapter&            schema_node,
                  InputAdapter&             input_node,
                  std::vector<std::string>& errors);
    enum class MessagePrintType
    {
        NORMAL,
        XML,
        JSON
    };
    void printMessages(bool                      pass,
                       std::vector<std::string>& errors,
                       MessagePrintType msgType = MessagePrintType::NORMAL,
                       std::string      file    = "",
                       std::ostream&    output  = std::cout);

    static void sort_errors(std::vector<std::string>& errors);
    static std::string combine(std::vector<std::string>& errors)
    {
        std::stringstream str;
        for (const std::string& s : errors)
        {
            str << s << std::endl;
        }
        return str.str();
    }

  private:
    // TODO document these attributes
    const float SUMERROR          = .01;
    const float PRODERROR         = .01;
    const int   MAXERRORS         = 100;
    const int   MAXENUMERRORCOUNT = 6;

    std::map<std::string, std::set<std::string>> enumRef;
    /**
     * @brief select_nodes selects nodes.
     * @param results the result set to populate with the nodes selected.
     * @param input_node the input node from which to make the selection.
     * @param selection_path the selection query path.
     * @return true, iff no selection path errors were encountered.
     * An error message is placed on the error.
     */
    template<class InputAdapter>
    bool select_nodes(SIRENResultSet<InputAdapter>& results,
                      InputAdapter&                 input_node,
                      const std::string&            selection_path,
                      std::vector<std::string>&     errors);

    // TODO document algorithm logic, runtime complexity, expected result
    template<class SchemaAdapter, class InputAdapter>
    bool traverse_schema(SchemaAdapter&            schema_node,
                         InputAdapter&             input_node,
                         std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateMinOccurs(SchemaAdapter&            schema_node,
                           InputAdapter&             input_node,
                           std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateMaxOccurs(SchemaAdapter&            schema_node,
                           InputAdapter&             input_node,
                           std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateValType(SchemaAdapter&            schema_node,
                         InputAdapter&             input_node,
                         std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateValEnums(SchemaAdapter&            schema_node,
                          InputAdapter&             input_node,
                          std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateMinValInc(SchemaAdapter&            schema_node,
                           InputAdapter&             input_node,
                           std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateMaxValInc(SchemaAdapter&            schema_node,
                           InputAdapter&             input_node,
                           std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateMinValExc(SchemaAdapter&            schema_node,
                           InputAdapter&             input_node,
                           std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateMaxValExc(SchemaAdapter&            schema_node,
                           InputAdapter&             input_node,
                           std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateExistsIn(SchemaAdapter&            schema_node,
                          InputAdapter&             input_node,
                          std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateNotExistsIn(SchemaAdapter&            schema_node,
                             InputAdapter&             input_node,
                             std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateSumOver(SchemaAdapter&            schema_node,
                         InputAdapter&             input_node,
                         std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateSumOverGroup(SchemaAdapter&            schema_node,
                              InputAdapter&             input_node,
                              std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateIncreaseOver(SchemaAdapter&            schema_node,
                              InputAdapter&             input_node,
                              std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateDecreaseOver(SchemaAdapter&            schema_node,
                              InputAdapter&             input_node,
                              std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateChildAtMostOne(SchemaAdapter&            schema_node,
                                InputAdapter&             input_node,
                                std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateChildExactlyOne(SchemaAdapter&            schema_node,
                                 InputAdapter&             input_node,
                                 std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateChildAtLeastOne(SchemaAdapter&            schema_node,
                                 InputAdapter&             input_node,
                                 std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateChildCountEqual(SchemaAdapter&            schema_node,
                                 InputAdapter&             input_node,
                                 std::vector<std::string>& errors);
    template<class SchemaAdapter, class InputAdapter>
    bool validateChildUniqueness(SchemaAdapter&            schema_node,
                                 InputAdapter&             input_node,
                                 std::vector<std::string>& errors);
    std::string getFullRuleName(const std::string& shortName)
    {
        static std::map<std::string, std::string> fullNames = {
            {"MinOccurs", "minimum occurrence"},
            {"MaxOccurs", "maximum occurrence"},
            {"ValType", "value type"},
            {"ValEnums", "enumerations"},
            {"MinValInc", "minimum inclusive value"},
            {"MaxValInc", "maximum inclusive value"},
            {"MinValExc", "minimum exclusive value"},
            {"MaxValExc", "maximum exclusive value"},
            {"ExistsIn", "exists in"},
            {"NotExistsIn", "not exists in"},
            {"SumOver", "sum over"},
            {"SumOverGroup", "sum over group"},
            {"ProdOver", "product over"},
            {"ProdOverGroup", "product over group"},
            {"IncreaseOver", "increasing"},
            {"DecreaseOver", "decreasing"},
            {"ChildAtMostOne", "at most one"},
            {"ChildExactlyOne", "exactly one"},
            {"ChildAtLeastOne", "at least one"},
            {"ChildCountEqual", "an equal number"},
            {"ChildUniqueness", "uniqueness"}};
        auto fullNamesIter = fullNames.find(shortName);
        if (fullNamesIter == fullNames.end())
            return shortName;
        else
            return fullNamesIter->second;
    }
    static bool schema_element(const std::string& name)
    {
        static std::set<std::string> element_names = {
            "MinOccurs",       "MaxOccurs",       "ValType",
            "ValEnums",        "MinValInc",       "MaxValInc",
            "MinValExc",       "MaxValExc",       "ExistsIn",
            "NotExistsIn",     "SumOver",         "SumOverGroup",
            "ProdOver",        "ProdOverGroup",   "IncreaseOver",
            "DecreaseOver",    "ChildAtMostOne",  "ChildExactlyOne",
            "ChildAtLeastOne", "ChildCountEqual", "ChildUniqueness",
            "InputTmpl"};
        return element_names.find(name) != element_names.end();
    }
    static bool is_valid_expression(std::string cpy)
    {
        transform(cpy.begin(), cpy.end(), cpy.begin(), ::tolower);
        const static std::vector<std::string> valid_function_names = {
            "abs",   "acos",  "acosh", "arg",  "asin", "asinh", "atan",
            "atan2", "atanh", "cbrt",  "conj", "ceil", "cos",   "cosh",
            "cot",   "csc",   "eval",  "exp",  "exp2", "floor", "hypot",
            "if",    "imag",  "int",   "log",  "log2", "log10", "max",
            "min",   "polar", "pow",   "real", "sec",  "sin",   "sinh",
            "sqrt",  "tan",   "tanh",  "trunc"};
        std::string::size_type i;
        for (auto& func : valid_function_names)
        {
            if ((i = cpy.find(func)) != std::string::npos)
                cpy.erase(i, func.size());
        }
        auto valid_function_char = [](char character) {
            const static std::set<char> valid_characters = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'e', 'x',
                'y', 'z', 't', '.', ',', '(', ')', '^', '+', '-', '*', '/'};
            return valid_characters.find(character) != valid_characters.end();
        };
        cpy.erase(std::remove_if(cpy.begin(), cpy.end(), valid_function_char),
                  cpy.end());
        return cpy.empty();
    }

    const std::atomic<bool>& stop;

  public:
    template<class D, class V>
    static bool create_definition(D*            definition_model,
                                  const V&      schema_view,
                                  std::ostream& errors,
                                  bool          ambig_error = true)
    {
        wasp_require(definition_model);
        for (size_t i = 0; i < schema_view.child_count(); ++i)
        {
            const auto& child_view = schema_view.child_at(i);
            // skip decorative elements
            if (child_view.is_decorative())
                continue;
            const std::string child_view_name = child_view.name();
            if (child_view_name == "InputAliases")
            {
                for (size_t a = 0; a < child_view.child_count(); ++a)
                {
                    const auto& alias_view = child_view.child_at(a);
                    if (alias_view.is_decorative())
                        continue;
                    // must be a scalar value - i.e. a string
                    wasp_check(alias_view.child_count() == 0);
                    const std::string& alias_name = alias_view.to_string();
                    definition_model->parent()->create_aliased(
                        alias_name, definition_model);
                }
                continue;  // skip aliases
            }
            // skip schema elements
            else if (schema_element(child_view_name))
                continue;
            D* child_definition = definition_model->create(child_view_name);
            //
            if (child_definition == nullptr &&
                definition_model->has(child_view_name) && ambig_error)
            {
                errors << "***Error : Ambiguous definition for '"
                       << child_view_name << "' found at line "
                       << child_view.line() << " and column "
                       << child_view.column() << std::endl;
                return false;
            }

            else if (child_definition != nullptr &&
                     !create_definition(child_definition, child_view, errors,
                                        ambig_error))
            {
                return false;
            }
        }
        return true;
    }

    // json type of singleton or array to switch on
    enum class JsonType
    {
        SINGLETON,
        ARRAY
    };

    // check if this node could occur just one time ( singleton )
    // or occur multiple times ( array ) according to the schema
    template<class SR, class CN>
    static bool get_json_type(SR            schema_root,
                              const CN&     current_node,
                              JsonType&     type,
                              std::ostream& err)
    {
        // if this is the document_root - then it is a singleton
        if (current_node.type() == wasp::DOCUMENT_ROOT)
        {
            type = JsonType::SINGLETON;
            return true;
        }

        // look up this current node's path in the schema
        DefaultSIRENInterpreter selector(err);
        selector.parseString(current_node.path());
        SIRENResultSet<SR> results;
        selector.evaluate(schema_root, results);

        // error if there are more than one of these nodes in the schema
        if (results.size() > 1)
        {
            err << "***ERROR: " << current_node.path()
                << " exists multiple times in schema." << std::endl;
            return false;
        }

        // error if this does not exist in the schema and not a value node
        else if (results.size() == 0)
        {
            if (current_node.type() == wasp::VALUE)
            {
                type = JsonType::SINGLETON;
            }
            else
            {
                err << "***ERROR: " << current_node.path()
                    << " does not exist in schema." << std::endl;
                return false;
            }
        }

        // if maxoccurs=1 - then singleton, otherwise it can occur more than
        // once
        else
        {
            if (!results.adapted(0)
                     .first_child_by_name("MaxOccurs")
                     .is_null() &&
                results.adapted(0)
                        .first_child_by_name("MaxOccurs")
                        .to_string() == "1")
            {
                type = JsonType::SINGLETON;
            }
            else
            {
                type = JsonType::ARRAY;
            }
        }

        return true;
    }

    // recursive method to traverse dom and save json conversion to out stream
    // uses schema to determine if components should be objects or arrays
    template<class SR, class CN>
    static bool input_to_json(const SR&     schema_root,
                              const CN&     current_node,
                              int           level,
                              int&          last_level_printed,
                              std::ostream& out,
                              std::ostream& err)
    {
        // deterime if this is a json object or array and if it is a "value"
        // node
        JsonType json_type;
        if (!get_json_type(schema_root, current_node, json_type, err))
            return false;
        const bool is_value = (current_node.type() == wasp::VALUE);

        // lambda returning the indentation spaces string depending on the level
        auto spaces = [](int level) {
            int tabsize = 2;
            return std::string(tabsize * level, ' ');
        };

        // if this component can occur just a single time
        if (json_type == JsonType::SINGLETON)
        {
            // print a comma if needed
            if (level == last_level_printed)
                out << "," << std::endl;

            // if this is a value node, then print simple
            if (is_value)
            {
                std::string escape_string = current_node.last_as_string();
                std::replace(escape_string.begin(), escape_string.end(), '"',
                             '\'');
                out << spaces(level) << "\"" << current_node.name() << "\":\""
                    << escape_string << "\"";
            }

            // if this is not a value node, then print json object and recurse
            else
            {
                // if this is the document root, do not print the node name -
                // otherwise do
                if (current_node.type() == wasp::DOCUMENT_ROOT)
                {
                    out << std::endl << spaces(level) << "{" << std::endl;
                }
                else
                {
                    out << spaces(level) << "\"" << current_node.name()
                        << "\":{" << std::endl;
                }

                // increment level
                level++;

                // print id tag if present
                if (!current_node.id().empty())
                {
                    out << spaces(level) << "\"_id\":\"" << current_node.id()
                        << "\"," << std::endl;
                }

                // get the non decorative children and recurse further
                auto children = current_node.non_decorative_children();
                for (size_t i = 0, count = children.size(); i < count; i++)
                {
                    if (!input_to_json(schema_root, children[i], level,
                                       last_level_printed, out, err))
                        return false;
                }

                // decrement level
                level--;

                // print close of this json object
                out << std::endl << spaces(level) << "}";

                // if this closes the document root, then print an extra newline
                if (current_node.type() == wasp::DOCUMENT_ROOT)
                {
                    out << std::endl;
                }
            }

            // save the last level printed to know if commas need to be printed
            // later
            last_level_printed = level;
        }

        // if this component can occur multiple times
        else if (json_type == JsonType::ARRAY)
        {
            // get the parent and all children of the same name
            auto parent = !current_node.parent().is_null()
                              ? current_node.parent()
                              : current_node;
            auto children_by_name = parent.child_by_name(current_node.name());

            // only operate on all of the same named children one time
            if (current_node == children_by_name[0])
            {
                // print a comma if needed
                if (level == last_level_printed)
                    out << "," << std::endl;

                // print name and beginning of json array
                out << spaces(level) << "\"" << current_node.name() << "\":["
                    << (is_value ? "" : "\n");

                // increment level
                level++;

                // loop over all collected children of the same name
                for (size_t i = 0, out_count = children_by_name.size();
                     i < out_count; i++)
                {
                    // if this is a value node, then print simple
                    if (is_value)
                    {
                        std::string escape_string =
                            children_by_name[i].last_as_string();
                        std::replace(escape_string.begin(), escape_string.end(),
                                     '"', '\'');
                        out << " \"" << escape_string << "\"";
                    }

                    // if this is not a value node, then print json object and
                    // recurse
                    else
                    {
                        // print start of this json object
                        out << spaces(level) << "{" << std::endl;

                        // increment level
                        level++;

                        // print id tag if present
                        if (!children_by_name[i].id().empty())
                        {
                            out << spaces(level) << "\"_id\":\""
                                << children_by_name[i].id() << "\","
                                << std::endl;
                        }

                        // get the non decorative children and recurse further
                        auto children =
                            children_by_name[i].non_decorative_children();
                        for (size_t j = 0, in_count = children.size();
                             j < in_count; j++)
                        {
                            if (!input_to_json(schema_root, children[j], level,
                                               last_level_printed, out, err))
                                return false;
                        }

                        // decrement level
                        level--;

                        // print close of this json object
                        out << std::endl << spaces(level) << "}";

                        // save the last level printed to know if commas need to
                        // be printed later
                        last_level_printed = level;
                    }

                    // print a comma if needed
                    if (i + 1 != out_count)
                        out << "," << (is_value ? "" : "\n");
                }

                // decrement level
                level--;

                // print close of this json array
                out << (is_value ? " " : "\n" + spaces(level)) << "]";

                // save the last level printed to know if commas need to be
                // printed later
                last_level_printed = level;
            }
        }

        return true;
    }

    class Error
    {
      public:
        //// Invalid Schema Rule Errors ////

        static std::string SirenParseError(const std::string& message)
        {
            return "Validation Error: Invalid Schema Rule: " + message;
        }

        static std::string
        BadSchemaRule(const std::string& ruleName, int line, int col)
        {
            return "Validation Error: Invalid Schema Rule: \"" + ruleName +
                   "\" line:" + std::to_string(line) +
                   " column:" + std::to_string(col);
        }

        static std::string BadSchemaPath(const std::string& ruleName,
                                         const std::string& lookupPath,
                                         int                line,
                                         int                col)
        {
            return "Validation Error: Invalid Schema Rule: Bad " + ruleName +
                   " Path \"" + lookupPath +
                   "\" at line:" + std::to_string(line) +
                   " column:" + std::to_string(col);
        }

        static std::string BadOption(const std::string& ruleName,
                                     const std::string& badOption,
                                     int                line,
                                     int                col,
                                     const std::string& validOptions)
        {
            return "Validation Error: Invalid Schema Rule: Bad " + ruleName +
                   " Option \"" + badOption +
                   "\" at line:" + std::to_string(line) +
                   " column:" + std::to_string(col) + " - Expected [ " +
                   validOptions + " ]";
        }

        static std::string MissingArgument(const std::string& ruleName,
                                           const std::string& argument,
                                           int                line,
                                           int                col)
        {
            return "Validation Error: Invalid Schema Rule: " + ruleName +
                   " missing " + argument + " at line:" + std::to_string(line) +
                   " column:" + std::to_string(col);
        }

        static std::string
        BadEnumReference(const std::string& refName, int line, int col)
        {
            return "Validation Error: Invalid Schema Rule: Enum Reference \"" +
                   refName + "\" at line:" + std::to_string(line) +
                   " column:" + std::to_string(col) + " not found in schema";
        }

        static std::string RangeNotTwoVals(int line, int col)
        {
            return "Validation Error: Invalid Schema Rule: Range does not have "
                   "exactly two values at line:" +
                   std::to_string(line) + " column:" + std::to_string(col);
        }

        static std::string
        RangeNotValidNum(const std::string& value, int line, int col)
        {
            return "Validation Error: Invalid Schema Rule: " + value +
                   " range value not a valid number at line:" +
                   std::to_string(line) + " column:" + std::to_string(col);
        }

        static std::string RangeInvalid(const std::string& startValue,
                                        const std::string& endValue,
                                        int                line,
                                        int                col)
        {
            return "Validation Error: Invalid Schema Rule: " + startValue +
                   "\" start of range is greater than or equal to \"" +
                   endValue + "\" end of range at" +
                   " line:" + std::to_string(line) +
                   " column:" + std::to_string(col);
        }

        //// Input Errors ////

        static std::string
        NotExistInSchema(int line, int col, const std::string& path)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + path +
                   " is not a valid piece of input";
        }

        static std::string MoreThanOneValue(int                line,
                                            int                col,
                                            const std::string& nodeName,
                                            const std::string& ruleName,
                                            const std::string& ruleValue)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " " + ruleName +
                   " checks against \"" + ruleValue + "\"" +
                   " which returns more than one value";
        }

        static std::string NotAValidNumber(int                line,
                                           int                col,
                                           const std::string& nodeName,
                                           const std::string& ruleName,
                                           const std::string& ruleValue)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " " + ruleName +
                   " checks against \"" + ruleValue + "\"" +
                   " which does not return a valid number";
        }

        static std::string WrongTypeForRule(int                line,
                                            int                col,
                                            const std::string& nodeName,
                                            const std::string& value,
                                            const std::string& ruleName)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " value \"" + value +
                   "\" is wrong value type for " + ruleName;
        }

        static std::string ErrorLimit(int                line,
                                      int                col,
                                      const std::string& nodePath,
                                      const std::string& ruleName,
                                      int                maxErrors)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + " --- ERROR LIMIT FOR RULE \"" +
                   nodePath + "/" + ruleName + "\" REACHED ... OVER " +
                   std::to_string(maxErrors) + " ERRORS FOUND";
        }

        static std::string Occurance(int                line,
                                     int                col,
                                     const std::string& nodeName,
                                     int                realOccurs,
                                     const std::string& childName,
                                     const std::string& ruleName,
                                     const std::string& expectedOccurs,
                                     const std::string& lookupPath = "")
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " has " +
                   std::to_string(realOccurs) + " \"" + childName +
                   "\" occurrences - when there should be a " + ruleName +
                   " of " +
                   (lookupPath == "" ? expectedOccurs
                                     : "\"" + expectedOccurs + "\" from \"" +
                                           lookupPath + "\"");
        }

        static std::string BadValType(int                line,
                                      int                col,
                                      const std::string& nodeName,
                                      const std::string& value,
                                      const std::string& valType)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " value \"" + value +
                   "\" is not of type " + valType;
        }

        static std::string BadEnum(int                line,
                                   int                col,
                                   const std::string& nodeName,
                                   const std::string& value,
                                   const std::string& closestEnums)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " value \"" + value +
                   "\" is not one of the allowed values: [ " + closestEnums +
                   " ]";
        }

        static std::string MinMax(int                line,
                                  int                col,
                                  const std::string& nodeName,
                                  const std::string& value,
                                  const std::string& ruleName,
                                  const std::string& minMaxValue,
                                  const std::string& lookupPath = "")
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " value \"" + value +
                   "\" is " +
                   (ruleName == "minimum inclusive value" ? "less than" : "") +
                   (ruleName == "maximum inclusive value" ? "greater than"
                                                          : "") +
                   (ruleName == "minimum exclusive value"
                        ? "less than or equal to"
                        : "") +
                   (ruleName == "maximum exclusive value"
                        ? "greater than or equal to"
                        : "") +
                   " the allowed " + ruleName + " of " +
                   (lookupPath == "" ? minMaxValue
                                     : "\"" + minMaxValue + "\" from \"" +
                                           lookupPath + "\"");
        }

        static std::string NotExistsIn(int                line,
                                       int                col,
                                       const std::string& nodeName,
                                       const std::string& value,
                                       const std::string& lookupPaths)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " value \"" + value +
                   "\" does not exist in set: [ " + lookupPaths + " ]";
        }

        static std::string AlsoExistsAt(int                line,
                                        int                col,
                                        const std::string& nodeName,
                                        const std::string& value,
                                        const std::string& lookupPath,
                                        int                breakline,
                                        int                breakcol,
                                        const std::string& ruleName)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " value \"" + value +
                   "\" also exists at \"" + lookupPath +
                   "\" on line:" + std::to_string(breakline) +
                   " column:" + std::to_string(breakcol);
        }

        static std::string SumProd(int                line,
                                   int                col,
                                   const std::string& nodeName,
                                   const std::string& childName,
                                   const std::string& actualValue,
                                   const std::string& ruleName,
                                   const std::string& expectedValue,
                                   const std::string& contextPath)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " children \"" +
                   childName + "\" " + (ruleName == "sum over" ? "sum" : "") +
                   (ruleName == "product over" ? "multiply" : "") + " to " +
                   actualValue + " - instead of the required " +
                   (ruleName == "sum over" ? "sum" : "") +
                   (ruleName == "product over" ? "product" : "") + " of " +
                   expectedValue;
        }

        static std::string SumProdGroup(int                line,
                                        int                col,
                                        const std::string& nodeName,
                                        const std::string& childName,
                                        const std::string& actualValue,
                                        const std::string& comparePath,
                                        int                groupDivide,
                                        int                groupDivideValue,
                                        const std::string& ruleName,
                                        const std::string& expectedValue,
                                        const std::string& contextPath)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " children \"" +
                   childName + "\" " +
                   (ruleName == "sum over group" ? "sum" : "") +
                   (ruleName == "product over group" ? "multiply" : "") +
                   " to " + actualValue + " for " +
                   std::to_string(groupDivide * groupDivideValue) +
                   " group - instead of the required " +
                   (ruleName == "sum over group" ? "sum" : "") +
                   (ruleName == "product over group" ? "product" : "") +
                   " of " + expectedValue;
        }

        static std::string IncreaseDecrease(int                line,
                                            int                col,
                                            const std::string& nodeName,
                                            const std::string& childName,
                                            const std::string& type,
                                            const std::string& ruleName,
                                            const std::string& contextPath,
                                            int                breakline,
                                            int                breakcol)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " children \"" +
                   childName + "\" are not " +
                   (type == "Mono" ? "monotonically " : "") +
                   (type == "Strict" ? "strictly " : "") + ruleName +
                   " at line:" + std::to_string(breakline) +
                   " column:" + std::to_string(breakcol);
        }

        static std::string ChildMostExactLeast(int                line,
                                               int                col,
                                               const std::string& nodeName,
                                               const std::string& childNames,
                                               const std::string& ruleName,
                                               const std::string& howMany = "")
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " has " +
                   (ruleName == "at most one" ? "more than one" : "") +
                   (ruleName == "exactly one" ? howMany : "") +
                   (ruleName == "at least one" ? "zero" : "") + " of: [ " +
                   childNames + " ] - " + ruleName + " must occur";
        }

        static std::string ChildCountEqual(int                line,
                                           int                col,
                                           const std::string& nodeName,
                                           const std::string& ruleName,
                                           const std::string& option,
                                           const std::string& childNames)
        {
            return "line:" + std::to_string(line) +
                   " column:" + std::to_string(col) +
                   " - Validation Error: " + nodeName + " does not have " +
                   ruleName + " of" +
                   (option == "IfExists" ? " existing" : "") + ": [ " +
                   childNames + " ]";
        }
    };
};
#include "wasphive/HIVE.i.h"
}  // namespace wasp

#endif /* WASP_HIVE_H */
