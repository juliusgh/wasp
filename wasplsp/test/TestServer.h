#ifndef WASPLSP_TEST_SERVER_H
#define WASPLSP_TEST_SERVER_H

#include <string>
#include "wasplsp/LSP.h"
#include "waspcore/Object.h"
#include "waspcore/decl.h"

namespace wasp {
namespace lsp  {

class WASP_PUBLIC TestServer
{
  public:

    TestServer() : is_initialized(false) , is_document_open(false) {}

    ~TestServer(){}

    template <typename I, typename O>
    bool run( I            & input_stream  ,
              O            & output_stream ,
              std::ostream & error_stream  );

    bool handleInitializeRequest(
                    const DataObject   & initializeRequest  ,
                          DataObject   & initializeResponse ,
                          std::ostream & errors             );

    bool handleInitializedNotification(
                    const DataObject   & initializedNotification ,
                          std::ostream & errors                  );

    bool handleDidOpenNotification(
                    const DataObject   & didOpenNotification            ,
                          DataObject   & publishDiagnosticsNotification ,
                          std::ostream & errors                         );

    bool handleDidChangeNotification(
                    const DataObject   & didChangeNotification          ,
                          DataObject   & publishDiagnosticsNotification ,
                          std::ostream & errors                         );

    bool handleCompletionRequest(
                    const DataObject   & completionRequest  ,
                          DataObject   & completionResponse ,
                          std::ostream & errors             );

    bool handleDefinitionRequest(
                    const DataObject   & definitionRequest  ,
                          DataObject   & definitionResponse ,
                          std::ostream & errors             );

    bool handleReferencesRequest(
                    const DataObject   & referencesRequest  ,
                          DataObject   & referencesResponse ,
                          std::ostream & errors             );

    bool handleRangeFormattingRequest(
                    const DataObject   & rangeFormattingRequest  ,
                          DataObject   & rangeFormattingResponse ,
                          std::ostream & errors                  );

    bool handleShutdownRequest(
                    const DataObject   & shutdownRequest  ,
                          DataObject   & shutdownResponse ,
                          std::ostream & errors           );

    bool handleExitNotification(
                    const DataObject   & exitNotification ,
                          std::ostream & errors           );

    bool parseDocumentForDiagnostics(
                          DataArray    & diagnosticsList ,
                          std::ostream & errors          );

    bool updateDocumentTextChanges(
                    const std::string  & replacement_text ,
                          int            start_line       ,
                          int            start_character  ,
                          int            end_line         ,
                          int            end_character    ,
                          int            range_length     ,
                          std::ostream & errors           );

    bool gatherDocumentCompletionItems(
                          DataArray    & completionItems  ,
                          bool         & is_incomplete    ,
                          int            line             ,
                          int            character        ,
                          std::ostream & errors           );

    bool gatherDocumentDefinitionLocations(
                          DataArray    & definitionLocations ,
                          int            line                ,
                          int            character           ,
                          std::ostream & errors              );

    bool gatherDocumentReferencesLocations(
                          DataArray    & referencesLocations ,
                          int            line                ,
                          int            character           ,
                          bool           include_declaration ,
                          std::ostream & errors              );

    bool gatherDocumentFormattingTextEdits(
                          DataArray    & formattingTextEdits ,
                          int            start_line          ,
                          int            start_character     ,
                          int            end_line            ,
                          int            end_character       ,
                          int            tab_size            ,
                          bool           insert_spaces       ,
                          std::ostream & errors              );

  private:

      bool        is_initialized;
      bool        is_document_open;

      DataObject  server_capabilities;
      DataObject  client_capabilities;
      std::string client_root_path;
      int         client_process_id;
      int         client_request_id;

      std::string document_path;
      std::string document_language_id;
      std::string document_text;
      int         document_version;
};

#include "TestServer.i.h"

} // namespace lsp
} // namespace wasp

#endif // WASPLSP_TEST_SERVER_H
