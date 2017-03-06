#ifndef WASP_EXPRINTERPRETER_H
#define WASP_EXPRINTERPRETER_H

#include "waspcore/TreeNodePool.h"
#include "waspcore/Interpreter.h"
#include "waspcore/wasp_bug.h"
#include "waspexpr/ExprParser.hpp"
#include <cmath>
#include <sstream>
#include <map>

namespace wasp{
template<class S = TreeNodePool<> >
class ExprInterpreter : public Interpreter<S>
{
public:
    ExprInterpreter(std::ostream & error_stream=std::cerr);
    ~ExprInterpreter();

    /**
     * @brief parse parser the given input stream
     * @param input the stream of the input data
     * @param startLine the first line of the input stream
     * @param startColumn the first column of the input stream
     * @return true, iff no input processing errors were encountered
     */
     bool parse(std::istream &input
                       , std::size_t m_start_line=1u
                       , std::size_t m_start_column=1u);

private:
     // type of the result
     enum Type : unsigned char{
         BOOLEAN
         ,INTEGER
         ,REAL
         ,STRING
         ,ERROR
     };
     template<typename T>
     static std::string to_string(T v, bool * ok=nullptr){
         std::stringstream s;
         s<<v;
         if( ok ) *ok = !(s.bad() || s.fail());
         return s.str();
     }
public: // variables

     /**
      * @brief The Context class provides variable capability
      */
     class Context{
     public:
        Context(){}
        Context(const Context& orig){}
        ~Context(){clear();}

        /**
         * @brief add_default_variables add set of default variables
         * @return this Context with addition of variables
         * Default variables are :
         * 'pi' - pi ~3.14 or (2 * std::acos(0.0))
         * 'e' - 2.7182818284590452353602874713527
         */
        Context & add_default_variables();
        /**
         * @brief add_default_variables add set of default functions
         * @return this Context with addition of functions
         * Default functions are :
         * sin(x) - the sine is a trigonometric function of an angle (radians)
         */
        Context & add_default_functions();
        /**
         * @brief clear deletes all associated variables and function
         */
        void clear();

        bool exists(const std::string & name)const{
            // TODO - ensure it is not nullptr
            return m_variables.find(name) != m_variables.end();
        }
        bool store_ref(const std::string & name, bool & v ){
            return store_ref<VarRefBool>(name, v);
        }
        bool store_ref(const std::string & name, int & v ){
            return store_ref<VarRefInt>(name, v);
        }
        bool store_ref(const std::string & name, double & v ){
            return store_ref<VarRefReal>(name, v);
        }
        bool store_ref(const std::string & name, std::string & v ){
            return store_ref<VarRefString>(name, v);
        }
        bool store( const std::string & name, const bool& v ){
            return store_ref<VarBool>(name,v);
        }
        bool store( const std::string & name, const int& v ){
            return store_ref<VarInt>(name,v);
        }
        bool store( const std::string & name, const double& v ){
            return store_ref<VarReal>(name,v);
        }
        bool store( const std::string & name, const std::string& v ){
            return store_ref<VarString>(name,v);
        }

        bool function_exists(const std::string & name)const;

        /**
         * @brief add_function adds the given function to this context
         * @param name the name of the function (e.g., 'sin', 'cos')
         * @param f the function pointer to reference upon evaluation
         * @return this context with addition of function
         * If the function f already exists the
         */
        Context& add_function(const std::string & name
                              , class ExprInterpreter::Function* f)
        {
            wasp_require( function_exists(name) == false );
            m_functions[name] = f;
            return *this;
        }

        class ExprInterpreter::Function * function(const std::string & name)const
        {
            auto itr = m_functions.find(name);
            if( itr == m_functions.end() ) return nullptr;
            return itr->second;
        }

     private:
        template<class T, class V>
        bool store_ref(const std::string & name, V & v){
            if( exists(name) ) return false;
            auto * ptr = new T(v);
            m_variables[name] = ptr;
            return ptr != nullptr;
        }
    public:
        /**
         * @brief The Variable class is an abstract interface for dealing with
         */
        class Variable{
        public:
            virtual Type type()const=0;
            virtual ~Variable(){}
            virtual int integer(bool * ok=nullptr)const{
                if( ok != nullptr ) *ok = false;
                // not implemented
                return std::numeric_limits<int>::quiet_NaN();
            }
            virtual double real(bool * ok=nullptr)const{
                if( ok != nullptr ) *ok = false;
                // not implemented
                return std::numeric_limits<double>::quiet_NaN();
            }
            virtual bool boolean(bool * ok=nullptr)const{
                if( ok != nullptr ) *ok = false;
                // not implemented
                return std::numeric_limits<bool>::quiet_NaN();
            }
            virtual std::string string(bool * ok=nullptr)const{
                if( ok != nullptr ) *ok = false;
                // not implemented
                return std::string();
            }
            virtual void store(bool v){
                // not implemented
            }
            virtual void store(int v){
                // not implemented
            }
            virtual void store(double v){
                // not implemented
            }
            virtual void store(const std::string& v){
                // not implemented
            }
        };
        Variable * variable(const std::string & name)const{
            auto itr = m_variables.find(name);
            if( itr == m_variables.end() ) return nullptr;
            return itr->second;
        }


     private:
        class VarRefInt : public Variable{
        public:
            VarRefInt(int & i):v(i){}
            Type type() const{return INTEGER;}
            bool boolean(bool * ok)const{ if( ok ) *ok = true; return bool(v);}
            int integer(bool * ok)const{ if( ok ) *ok = true; return v;}
            double real(bool * ok)const{ if( ok ) *ok = true; return double(v);}
            std::string string(bool * ok)const{ return to_string(v,ok);}

            void store(bool v) { this->v = v;}
            void store(int v) { this->v = v;}
            void store(double v) { this->v = v;}
        private:
            int & v;
        };
        class VarInt : public VarRefInt{
        public:
            VarInt(int i):VarRefInt(v),v(i){}
        private:
            int v;
        };

        class VarRefBool : public Variable{
        public:
            VarRefBool(bool & b):v(b){}
            Type type() const{return BOOLEAN;}
            bool boolean(bool * ok)const{ if( ok ) *ok = true; return bool(v);}
            int integer(bool * ok)const{ if( ok ) *ok = true; return v;}
            double real(bool * ok)const{ if( ok ) *ok = true; return double(v);}
            std::string string(bool * ok)const{ return to_string(v,ok);}

            void store(bool v) { this->v = v;}
            void store(int v) { this->v = v;}
            void store(double v) { this->v = v;}
        private:
            bool & v;
        };
        class VarBool : public VarRefBool{
        public:
            VarBool(bool b):VarRefBool(v),v(b){}
        private:
            bool v;
        };
        class VarRefReal : public Variable{
        public:
            VarRefReal(double & d):v(d){}
            Type type() const{return REAL;}
            bool boolean(bool * ok)const{ if( ok ) *ok = true; return bool(v);}
            int integer(bool * ok)const{ if( ok ) *ok = true; return int(v);}
            double real(bool * ok)const{ if( ok ) *ok = true; return double(v);}
            std::string string(bool * ok)const{ return to_string(v,ok);}

            void store(bool v) { this->v = v;}
            void store(int v) { this->v = v;}
            void store(double v) { this->v = v;}
        private:
            double & v;
        };
        class VarReal : public VarRefReal{
        public:
            VarReal(double d):VarRefReal(v),v(d){}
        private:
            double v;
        };
        class VarRefString : public Variable{
        public:
            VarRefString(std::string & s):v(s){}
            Type type() const{return STRING;}
            std::string string(bool * ok)const{ if( ok ) *ok = true; return v;}

            void store(bool v) { this->v = to_string(v);}
            void store(int v) { this->v = to_string(v);}
            void store(double v){ this->v = to_string(v);}
            void store(const std::string & v){this->v = v;}
        private:
            std::string & v;
        };
        class VarString : public VarRefString{
        public:
            VarString(const std::string & s):VarRefString(v),v(s){}
        private:
            std::string v;
        };

        std::map<std::string,Variable*> m_variables;
        std::map<std::string,class ExprInterpreter::Function*> m_functions;
     };// end of class Context

     /**
      * @brief The Result class simple class for evaluating an expression
      */
     class Result{
     public:
         Result():m_type(Type::INTEGER){}
         Result(const Result& orig)
             :m_type(orig.m_type)
         , m_value(orig.m_value)
         , m_string(orig.m_string){

         }
         ~Result(){}

         template<class T>
         Result & evaluate( const T & tree_view, Context & context);

     private:
         bool and_expr(const Result & a){

             if( is_number() && a.is_number() )
             {
                 m_value.m_bool = number() && a.number();
             }
             else if( is_bool() && a.is_bool() )
             {
                 m_value.m_bool = boolean() && a.boolean();
             }
             else {
                 // NOT IMPLEMENTED
                 m_value.m_bool = false;
             }
             m_type = BOOLEAN;
             return m_value.m_bool;
         }
         bool or_expr(const Result & a){

             if( is_number() && a.is_number() )
             {
                 m_value.m_bool = number() || a.number();
             }
             else if( is_bool() && a.is_bool() )
             {
                 m_value.m_bool = boolean() || a.boolean();
             }
             else {
                 // NOT IMPLEMENTED
                 m_value.m_bool = false;
             }
             m_type = BOOLEAN;
             return m_value.m_bool;
         }
         bool not_equal(const Result& a){
             m_value.m_bool = !equal(a);
             m_type = BOOLEAN;
            return m_value.m_bool;

         }
         bool equal(const Result& a) {
             if( is_number() && a.is_number() )
             {
                 m_value.m_bool = number() == a.number();
             }
             else if( is_string() && a.is_string() )
             {
                 m_value.m_bool = string() == a.string();
             }
             else if( is_bool() && a.is_bool() )
             {
                 m_value.m_bool = boolean() == a.boolean();
             }
             else {
                 m_value.m_bool = false;
             }

             m_type = BOOLEAN;
             return m_value.m_bool;
         }
         bool less (const Result & a) {

             if( is_number() && a.is_number() )
             {
                 m_value.m_bool = number() < a.number();
                 m_type = BOOLEAN;
                 return m_value.m_bool;
             }
             if( is_string() && a.is_string() )
             {
                 m_value.m_bool = string() < a.string();
                 m_type = BOOLEAN;
                 return m_value.m_bool;
             }

             return false;
         }
         bool less_or_equal(const Result & a)
         {
             if( is_number() && a.is_number() )
             {
                 m_value.m_bool = number() <= a.number();
                 m_type = BOOLEAN;
                 return m_value.m_bool;
             }
             if( is_string() && a.is_string() )
             {
                 m_type = BOOLEAN;
                 m_value.m_bool = string() <= a.string();
                 return m_value.m_bool;
             }
             return false;
         }
         bool greater (const Result & a) {
             if( is_number() && a.is_number() )
             {
                 m_value.m_bool = number() > a.number();
                 m_type = BOOLEAN;
                 return m_value.m_bool;
             }
             if( is_string() && a.is_string() )
             {
                 m_value.m_bool = string() > a.string();
                 m_type = BOOLEAN;
                 return m_value.m_bool;
             }
             return false;
         }
         bool greater_or_equal(const Result & a){
             if( is_number() && a.is_number() )
             {
                 m_value.m_bool = number() >= a.number();
                 m_type = BOOLEAN;
                 return m_value.m_bool;
             }
             if( is_string() && a.is_string() )
             {
                 m_value.m_bool = string() >= a.string();
                 m_type = BOOLEAN;
                 return m_value.m_bool;
             }
             return false;
         }
         Result& pow (const Result & a){

             if( is_integer() )
             {
                 // integer ^ interger
                 if( a.is_integer() ){
                     m_value.m_int
                             = std::pow(integer(),a.integer());
                 }
                 // integer ^ real
                 else if( a.is_real() ){
                     m_type = REAL;
                     m_value.m_real = std::pow(integer(),a.real());
                 }
                 // integer ^ string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(integer())
                             + a.string();
                 }
             }
             else if( is_real() )
             {
                 // real ^ integer
                 if( a.is_integer() ){
                     m_value.m_real
                             = std::pow(real(),a.integer());
                 }
                 // real ^ real
                 else if( a.is_real() ){
                     m_value.m_real = std::pow(real(),a.real());
                 }
                 // real ^ string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(real())
                             + a.string();
                 }
             }else if( is_string() ){
                 // string ^ integer
                 if( a.is_integer() ){
                     string()
                             += std::to_string(a.integer());
                 }
                 // string ^ real
                 else if( a.is_real() ){
                     string()
                             += std::to_string(a.real());
                 }
                 // string ^ string
                 else if( a.is_string() )
                 {
                    string()
                            += a.string();
                 }
             }
             return *this;
         }

         Result& plus(const Result&a)
         {
             if( is_integer() )
             {
                 // integer + interger
                 if( a.is_integer() ){
                     m_value.m_int += a.integer();
                 }
                 // integer + real
                 else if( a.is_real() ){
                     m_type = REAL;
                     m_value.m_real = integer()+a.real();
                 }
                 // integer + string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(integer())
                             + a.string();
                 }
             }
             else if( is_real() )
             {
                 // real + integer
                 if( a.is_integer() ){
                     m_value.m_real += a.integer();
                 }
                 // real + real
                 else if( a.is_real() ){
                     m_value.m_real += a.real();
                 }
                 // real + string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(real())
                             + a.string();
                 }
             }else if( is_string() ){
                 // string + integer
                 if( a.is_integer() ){
                     string()
                             += std::to_string(a.integer());
                 }
                 // string + real
                 else if( a.is_real() ){
                     string()
                             += std::to_string(a.real());
                 }
                 // string + string
                 else if( a.is_string() )
                 {
                    string()
                            += a.string();
                 }
             }
             return *this;
         }

         Result& minus(const Result&a)
         {
             if( is_integer() )
             {
                 // integer - interger
                 if( a.is_integer() ){
                     m_value.m_int -= a.integer();
                 }
                 // integer - real
                 else if( a.is_real() ){
                     m_type = REAL;
                     m_value.m_real = integer()-a.real();
                 }
                 // integer - string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(integer())
                             +"-"+ a.string();
                 }
             }
             else if( is_real() )
             {
                 // real - integer
                 if( a.is_integer() ){
                     m_value.m_real -= a.integer();
                 }
                 // real - real
                 else if( a.is_real() ){
                     m_value.m_real -= a.real();
                 }
                 // real - string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(real())
                             + a.string();
                 }
             }else if( is_string() ){
                 // string - integer
                 if( a.is_integer() ){
                     string()
                             += "-"+std::to_string(a.integer());
                 }
                 // string - real
                 else if( a.is_real() ){
                     string()
                             += "-"+std::to_string(a.real());
                 }
                 // string - string
                 else if( a.is_string() )
                 {
                    string()
                            += "-"+a.string();
                 }
             }
             return *this;
         }
         Result& div(const Result&a)
         {
             if( is_integer() )
             {
                 // integer / interger
                 if( a.is_integer() ){
                     m_value.m_int /= a.integer();
                 }
                 // integer / real
                 else if( a.is_real() ){
                     m_type = REAL;
                     m_value.m_real = integer()/a.real();
                 }
                 // integer / string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(integer())
                             + "/"+a.string();
                 }
             }
             else if( is_real() )
             {
                 // real / integer
                 if( a.is_integer() ){
                     m_value.m_real /= a.integer();
                 }
                 // real / real
                 else if( a.is_real() ){
                     m_value.m_real /= a.real();
                 }
                 // real / string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(real())
                             + +"/"+a.string();
                 }
             }else if( is_string() ){
                 // string / integer
                 if( a.is_integer() ){
                     string()
                             += std::to_string(a.integer());
                 }
                 // string / real
                 else if( a.is_real() ){
                     string()
                             += std::to_string(a.real());
                 }
                 // string / string
                 else if( a.is_string() )
                 {
                    string()
                            += "/"+a.string();
                 }
             }
             return *this;
         }
         Result& mult(const Result&a)
         {
             if( is_integer() )
             {
                 // integer * interger
                 if( a.is_integer() ){
                     m_value.m_int *= a.integer();
                 }
                 // integer * real
                 else if( a.is_real() ){
                     m_type = REAL;
                     m_value.m_real = integer()*a.real();
                 }
                 // integer * string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(integer())
                             + a.string();
                 }
             }
             else if( is_real() )
             {
                 // real * integer
                 if( a.is_integer() ){
                     m_value.m_real *= a.integer();
                 }
                 // real * real
                 else if( a.is_real() ){
                     m_value.m_real *= a.real();
                 }
                 // real * string
                 else if( a.is_string() )
                 {
                     m_type = STRING;
                     string()
                             = std::to_string(real())
                             + a.string();
                 }
             }else if( is_string() ){
                 // string * integer
                 if( a.is_integer() ){
                     string()
                             += std::to_string(a.integer());
                 }
                 // string * real
                 else if( a.is_real() ){
                     string()
                             += std::to_string(a.real());
                 }
                 // string * string
                 else if( a.is_string() )
                 {
                    string()
                            += a.string();
                 }
             }
             return *this;
         }
         Result& unary_not()
         {

             if( is_bool() )
             {
                 m_value.m_bool = !m_value.m_bool;
                 m_type = BOOLEAN;
             }
             else if( is_integer() )
             {
                 m_value.m_bool = !bool(integer());
                 m_type = BOOLEAN;
             }
             else if( is_real() )
             {
                m_value.m_bool = !bool(real());
                m_type = BOOLEAN;
             }
             else if( is_string() )
             {
                string() = "!"+string();
                m_type = STRING;
             }

             return *this;
         }
         Result& unary_minus(){
             if( is_integer() )
             {
                 m_value.m_int = -(integer());
             }
             else if( is_real() )
             {
                m_value.m_real = -(real());
             }
             else if( is_string() )
             {
                string() = "-"+string();
             }
             return *this;
         }

     public:
         int integer()const{return m_value.m_int;}
         double real()const{return m_value.m_real;}
         double number()const{
             switch( m_type )
             {
                case INTEGER:
                 return m_value.m_int;
                case REAL:
                 return m_value.m_real;
                default:
                 break;
             }
             return std::numeric_limits<double>::quiet_NaN();
         }
         bool boolean()const{return m_value.m_bool;}
         const std::string& string()const{
             return (m_string);}

         std::string& string(){
             return (m_string);}

         bool is_bool()const{
             return m_type == BOOLEAN;
         }
         bool is_integer()const{
             return m_type == INTEGER;
         }
         bool is_real()const{
             return m_type == REAL;
         }
         bool is_number()const{
             return m_type == INTEGER
                     || m_type == REAL;
         }
         bool is_error()const{
             return m_type == ERROR;
         }
         bool is_string()const{
             return m_type == STRING;
         }
     private:
         Type m_type;
         union Value{
             bool m_bool;
             int m_int;
             double m_real;
         }m_value;
         std::string m_string;

         template<class T>
         std::string error_msg( const T & tree_view, const std::string & msg_str)
         {
             std::stringstream msg;
             msg<<"***Error : '"<<tree_view.name()<<"' at line "
               <<tree_view.line()<<" and column "<<tree_view.column()
              <<" - "<<msg_str<<std::endl;
             return msg.str();
         }
     }; // end of Result
     /**
      * @brief The Function class wraps Context accessible functions
      */
     class Function{
     public:
         typedef std::vector<Result> Args;
         virtual Type type()const=0;
         virtual ~Function(){}
         virtual int integer( const Args& args
                              , std::ostream & err
                              , bool * ok=nullptr)const{
             if( ok != nullptr ) *ok = false;
             wasp_not_implemented("Function::integer");
         }
         virtual double real(const Args& args
                             , std::ostream & err
                             , bool * ok=nullptr)const{
             if( ok != nullptr ) *ok = false;
             wasp_not_implemented("Function::real");
         }
         virtual bool boolean(const Args& args
                              , std::ostream & err
                              , bool * ok=nullptr)const{
             if( ok != nullptr ) *ok = false;
             wasp_not_implemented("Function::boolean");
         }
         virtual std::string string(const Args& args
                                    , std::ostream & err
                                    , bool * ok=nullptr)const{
             if( ok != nullptr ) *ok = false;
             wasp_not_implemented("Function::string");
         }
     };
     class FSin: public Function
     {
     public:
         typedef std::vector<Result> Args;
         virtual Type type()const{return REAL;}
         virtual ~FSin(){}
         virtual int integer( const Args& args
                              , std::ostream & err
                              , bool * ok=nullptr)const{
             return real(args,err,ok);
         }
         virtual double real(const Args& args
                             , std::ostream & err
                             , bool * ok=nullptr)const{
             bool l_ok = true;
             if( args.size() != 1 )
             {   l_ok = false;
                 err<<"function expects 1 argument, given "<<args.size()<<".";
             }
             else if( !args.front().is_number() )
             {
                 l_ok = false;
                 err<<"function expects a number";
             }
             if( ok != nullptr ) *ok = l_ok;
             const auto & a = args.front();
             if( a.is_integer() ){
                return std::sin(a.integer());
             }else if ( a.is_real() ){
                 return std::sin(a.real());
             }
             return std::numeric_limits<double>::quiet_NaN();
         }
         virtual bool boolean(const Args& args
                              , std::ostream & err
                              , bool * ok=nullptr)const{
             return real(args,err,ok);
         }
         virtual std::string string(const Args& args
                                    , std::ostream & err
                                    , bool * ok=nullptr)const{
             return to_string(real(args,err,ok));
         }
     };

private :
     Context m_context;
public:
     Context & context(){return m_context;}
     const Context & context()const{return m_context;}
     Result evaluate(){
         Result r;
         auto root_view = this->root();
         return r.evaluate(root_view,m_context);
     }
};
#include "waspexpr/ExprInterpreter.i.h"
} // end of namespace

#endif
