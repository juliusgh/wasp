
#ifndef WASP_HALITEINTERPRETER_H
#define WASP_HALITEINTERPRETER_H

#include <string>
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>

#include "waspcore/Interpreter.h"
#include "wasphalite/SubStringIndexer.h"
#include "waspexpr/ExprInterpreter.h"
#include "waspcore/wasp_bug.h"

namespace wasp {

    template<class S = TreeNodePool<unsigned short, unsigned short
                                    ,TokenPool<unsigned short,unsigned short, unsigned short>> >
    class HaliteInterpreter : public Interpreter<S> {
    public:
        typedef S Storage_type;
        typedef std::shared_ptr<HaliteInterpreter> SharedPtr;
        HaliteInterpreter();
        HaliteInterpreter(std::ostream & err);
        virtual ~HaliteInterpreter();

        /** Invoke the lexer and parser for a stream.
         * @param in        input stream
         * @param sname     stream name for error messages
         * @return          true if successfully parsed
         */
        bool parseStream(std::istream& in,
                const std::string& sname = "stream input", size_t startLine=1u, size_t startColumn=1u);
        bool parse(std::istream &input, size_t startLine=1u, size_t startColumn=1u);
        /** Invoke the lexer and parser on an input string.
         * @param input     input string
         * @param sname     stream name for error messages
         * @return          true if successfully parsed
         */
        bool parseString(const std::string& input,
                const std::string& sname = "string stream", size_t startLine=1u, size_t startColumn=1u);
        /** Invoke the lexer and parser on a file. Use parse_stream with a
         * std::ifstream if detection of file reading errors is required.
         * @param filename  input file name
         * @return          true if successfully parsed
         */
        bool parseFile(const std::string& filename, size_t startLine=1u);

        /**
         * @brief evaluate evaluates the template emitting the expansion into out stream
         * @param out the stream on which to emit the expanded template
         * @param activity_log an optional activity log to emit template activity on
         * @return true, iff the template expanded with no errors.
         */
        bool evaluate(std::ostream & out, std::ostream * activity_log=nullptr);
    private:

        bool parse_content(std::istream& in);

        bool parse_line(const std::string & line );

        void capture_leaf(const std::string& node_name
                          , size_t node_type
                          , const std::string& data
                          , size_t token_type
                          , size_t file_offset);

        /**
         * @brief capture captures the given attributes into a tree structure
         * @param current_column_index the line's column to begin capture
         * @param current_attribute_index
         * @param attribute_indices
         * @param limit default of 0, meaning no limit.
         */
        void capture(const std::string& data
                     ,size_t& current_column_index
                     ,size_t& current_attribute_index
                     ,const SubStringIndexer::IndexPairs_type& attribute_indices
                     ,size_t limit=0);

        /**
         * @brief print_attribute prints attribute to output stream
         * @param attr_view tree view containing attribute structure
         * @param out the stream to emit the attribute substitution result
         * @param line the line on which the substitution is taking place
         * @param column the column at which the substitution is starting, will be updated
         * @return true, iff no errors occurred during the substitution/printing of the attribute
         * TODO - implement nested attributes and attribute formatting
         */
        bool print_attribute(const TreeNodeView<S> & attr_view
                             ,std::ostream& out, size_t& line, size_t& column  );

        /**
         * @brief import_file imports the file represented by the given tree view
         * @param import_view the tree view containing the directive and potentially parameterized text
         * @param out the output stream on which to emit the file contents
         * @param line the line on which the import is taking place
         * @param column the column at which the import starts
         * @return true, iff the file and its contents were successfully imported and emitted
         */
        bool import_file(const TreeNodeView<S> & import_view
                         ,std::ostream& out, size_t& line, size_t & column);
    public: // public variables

        /**
         * @brief setStreamName sets the name of this stream and indicates whether the stream/name is a file[path]
         * @param name the name of the stream or file[path]
         * @param isFile [default=false] indicates whether the name is a file path
         */
        void setStreamName(const std::string & name, bool isFile=false)
        {Interpreter<S>::stream_name() = name; m_has_file = isFile;}


        /**
         * @brief hasFile indicates whether this parser's stream name is a file path
         * @return
         */
        bool hasFile()const{return m_has_file;}
    private: // private variables

        /**
         * @brief attribute_start_delim attribute substitution declarator
         */
        std::string m_attribute_start_delim;
        /**
         * @brief attribute_end_delim attribute substitution terminator
         */
        std::string m_attribute_end_delim;

        size_t m_current_line_count;
        size_t m_file_offset;
    private: // private methods

        /**
         * @brief mHasFile indicates whether this parser was instantiated via a file
         */
        bool m_has_file;
    }; // end of HaliteInterpreter class
#include "wasphalite/HaliteInterpreter.i.h"
} // namespace wasp
#endif // WASPHaliteIntERPRETER_H
