#ifndef WASPLSP_CLIENT_IMPL_H
#define WASPLSP_CLIENT_IMPL_H

#include <string>
#include <sstream>
#include "wasplsp/LSP.h"
#include "wasplsp/Connection.h"
#include "wasplsp/SymbolIterator.h"
#include "waspcore/Object.h"
#include "waspcore/decl.h"

namespace wasp {
namespace lsp  {

class WASP_PUBLIC ClientImpl
{
  public:

    ClientImpl() :
        is_connected(false)      ,
        is_initialized(false)    ,
        is_document_open(false)  ,
        already_in_call(false)   ,
        request_id(0)            ,
        document_version(0)      ,
        response_type(NONE){}

    ~ClientImpl(){}

    typedef std::shared_ptr<ClientImpl> SP;

    bool connect( Connection::SP connection );

    bool startUpCleanly();

    bool wrapUpCleanly();

    bool doInitialize();

    bool doInitialized();

    bool doDocumentOpen( const std::string & document_path        ,
                         const std::string & document_language_id ,
                         const std::string & document_text        );

    bool doDocumentChange( int                 start_line        ,
                           int                 start_character   ,
                           int                 end_line          ,
                           int                 end_character     ,
                           int                 range_length      ,
                           const std::string & new_document_text );

    bool doDocumentCompletion( int line      ,
                               int character );

    bool doDocumentDefinition( int line      ,
                               int character );

    bool doDocumentReferences( int  line                ,
                               int  character           ,
                               bool include_declaration );

    bool doDocumentFormatting( int  start_line      ,
                               int  start_character ,
                               int  end_line        ,
                               int  end_character   ,
                               int  tab_size        ,
                               bool insert_spaces   );

    bool doDocumentSymbols();

    bool doDocumentClose();

    bool doShutdown();

    bool doExit();

    int getDiagnosticSize();

    int getCompletionSize();

    int getDefinitionSize();

    int getReferencesSize();

    int getFormattingSize();

    bool getDiagnosticAt( int           index           ,
                          int         & start_line      ,
                          int         & start_character ,
                          int         & end_line        ,
                          int         & end_character   ,
                          int         & severity        ,
                          std::string & code            ,
                          std::string & source          ,
                          std::string & message         );

    bool getCompletionAt( int           index           ,
                          std::string & label           ,
                          int         & start_line      ,
                          int         & start_character ,
                          int         & end_line        ,
                          int         & end_character   ,
                          std::string & new_text        ,
                          int         & kind            ,
                          std::string & detail          ,
                          std::string & documentation   ,
                          bool        & deprecated      ,
                          bool        & preselect       );

    bool getDefinitionAt( int           index           ,
                          int         & start_line      ,
                          int         & start_character ,
                          int         & end_line        ,
                          int         & end_character   );

    bool getReferencesAt( int           index           ,
                          int         & start_line      ,
                          int         & start_character ,
                          int         & end_line        ,
                          int         & end_character   );

    bool getFormattingAt( int           index           ,
                          int         & start_line      ,
                          int         & start_character ,
                          int         & end_line        ,
                          int         & end_character   ,
                          std::string & new_text        );

    bool isConnected()
    {
        return this->is_connected;
    }

    Connection::SP getConnection()
    {
        return this->connection;
    }

    std::string getErrors() const
    {
        return this->errors.str();
    }

    int getPreviousRequestID() const
    {
        return this->request_id;
    }

    bool isDocumentOpen() const
    {
        return this->is_document_open;
    }

    std::string getDocumentPath() const
    {
        return this->document_path;
    }

    int getCurrentDocumentVersion() const
    {
        return this->document_version;
    }

    SymbolIterator::SP getSymbolIterator()
    {
        wasp_check( verifySymbolsResponse( *(this->response) ) );

        return std::make_shared<SymbolIterator>( this->response );
    }

  private:

      struct InsideClientCall
      {
          InsideClientCall( bool & in_call ) : in_call( in_call )
          {
              this->in_call = true;
          }
          ~InsideClientCall()
          {
              this->in_call = false;
          }
          bool & in_call;
      };

      Connection::SP    connection;
      std::stringstream errors;

      bool              is_connected;
      bool              is_initialized;
      bool              is_document_open;
      bool              already_in_call;
      int               request_id;
      int               document_version;
      std::string       document_path;

      enum {
        INITIALIZE ,
        DIAGNOSTIC ,
        COMPLETION ,
        DEFINITION ,
        REFERENCES ,
        FORMATTING ,
        SYMBOLS    ,
        SHUTDOWN   ,
        NONE
      } response_type;

      DataObject::SP response;

      void incrementRequestID()
      {
          this->request_id++;
      }

      void incrementDocumentVersion()
      {
          this->document_version++;
      }
};

} // namespace lsp
} // namespace wasp

#endif // WASPLSP_CLIENT_IMPL_H
