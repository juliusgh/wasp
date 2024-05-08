#ifndef WASPLSP_SERVER_IMPL_H
#define WASPLSP_SERVER_IMPL_H

#include <string>
#include <sstream>
#include <map>
#include "wasplsp/LSP.h"
#include "wasplsp/Connection.h"
#include "waspcore/Object.h"
#include "waspcore/decl.h"

namespace wasp {
namespace lsp  {

class WASP_PUBLIC ServerImpl
{
  public:

    ServerImpl() : is_initialized(false) , client_snippet_support(false) {}

    virtual ~ServerImpl() = default;

    /** read from the server connection and handle packets until exit or error
     * @return - true if returning on exit call / false if returning on error
     */
    bool run();

    /** handle initialize request creating response in provided reference
     * @param initializeRequest - const reference to request to be handled
     * @param initializeResponse - reference to object that will hold response
     * @return - true if request was successfully handle and response built
     */
    bool handleInitializeRequest(
                    const wasp::DataObject & initializeRequest  ,
                          wasp::DataObject & initializeResponse );

    /** handle initialized notification - no response expected
     * @param initializedNotification - const reference to notification
     * @return - true if notification was successfully handled
     */
    bool handleInitializedNotification(
                    const wasp::DataObject & initializedNotification );

    /** handle didopen notification creating diagnostics in provided reference
     * @param didOpenNotification - const reference to notification
     * @param publishDiagnosticsNotification - reference to diagnostics to fill
     * @return - true if packet was successfully handled and diagnostics filled
     */
    bool handleDidOpenNotification(
                    const wasp::DataObject & didOpenNotification            ,
                          wasp::DataObject & publishDiagnosticsNotification );

    /** handle didchange notification creating diagnostics in provided reference
     * @param didOpenNotification - const reference to notification
     * @param publishDiagnosticsNotification - reference to diagnostics to fill
     * @return - true if packet was successfully handled and diagnostics filled
     */
    bool handleDidChangeNotification(
                    const wasp::DataObject & didChangeNotification          ,
                          wasp::DataObject & publishDiagnosticsNotification );

    /** handle completion request creating response in provided reference
     * @param completionRequest - const reference to request to be handled
     * @param completionResponse - reference to object that will hold response
     * @return - true if request was successfully handle and response built
     */
    bool handleCompletionRequest(
                    const wasp::DataObject & completionRequest  ,
                          wasp::DataObject & completionResponse );

    /** handle definition request creating response in provided reference
     * @param definitionRequest - const reference to request to be handled
     * @param definitionResponse - reference to object that will hold response
     * @return - true if request was successfully handle and response built
     */
    bool handleDefinitionRequest(
                    const wasp::DataObject & definitionRequest  ,
                          wasp::DataObject & definitionResponse );

    /** handle hover request building response in provided object reference
     * @param hoverRequest - const reference to object for hover parameters
     * @param hoverResponse - object reference where response will be built
     * @return - true if request was handled successfully building response
     */
    bool handleHoverRequest(
                    const wasp::DataObject & hoverRequest  ,
                          wasp::DataObject & hoverResponse );

    /** handle references request creating response in provided reference
     * @param referencesRequest - const reference to request to be handled
     * @param referencesResponse - reference to object that will hold response
     * @return - true if request was successfully handle and response built
     */
    bool handleReferencesRequest(
                    const wasp::DataObject & referencesRequest  ,
                          wasp::DataObject & referencesResponse );

    /** handle formatting request creating response in provided reference
     * @param formattingRequest - const reference to request to be handled
     * @param formattingRequest - reference to object that will hold response
     * @return - true if request was successfully handle and response built
     */
    bool handleFormattingRequest(
                    const wasp::DataObject & formattingRequest  ,
                          wasp::DataObject & formattingResponse );

    /** handle symbols request creating response in provided reference
     * @param symbolsRequest - const reference to request to be handled
     * @param symbolsResponse - reference to object that will hold response
     * @return - true if request was successfully handle and response built
     */
    bool handleSymbolsRequest(
                    const wasp::DataObject & symbolsRequest  ,
                          wasp::DataObject & symbolsResponse );

    /** handle didclose notification - no response expected
     * @param didCloseNotification - const reference to notification
     * @return - true if notification was successfully handled
     */
    bool handleDidCloseNotification(
                    const wasp::DataObject & didCloseNotification );

    /** handle shutdown request creating response in provided reference
     * @param shutdownRequest - const reference to request to be handled
     * @param shutdownResponse - reference to object that will hold response
     * @return - true if request was successfully handle and response built
     */
    bool handleShutdownRequest(
                    const wasp::DataObject & shutdownRequest  ,
                          wasp::DataObject & shutdownResponse );

    /** handle exit notification - no response expected
     * @param exitNotification - const reference to notification
     * @return - true if notification was successfully handled
     */
    bool handleExitNotification(
                    const wasp::DataObject & exitNotification );

    /** check if the server is initialized and reading from the connection
     * @return - true if server is initialized and reading from the connection
     */
    bool isRunning()
    {
        return this->is_initialized;
    }

    /** get all errors that have been stored on the server for any reason
     * @return - error string of all errors that have been stored on the server
     */
    std::string getErrors()
    {
        return errors.str();
    }

    /** check if client for this server communicated snippet syntax support
     * @return - true if client for this server supports snippet completion
     */
    bool clientSupportsSnippets()
    {
        return this->client_snippet_support;
    }

    /** get this server's connection - to be implemented on derived servers
     * @return - shared pointer to the server's read / write connection
     */
    virtual std::shared_ptr<wasp::lsp::Connection> getConnection() = 0;

  protected:

    /** parse document for diagnostics - to be implemented on derived servers
     * @param diagnosticsList - data array of diagnostics data objects to fill
     * @return - true if completed successfully - does not indicate parse fail
     */
    virtual bool parseDocumentForDiagnostics(
                          wasp::DataArray  & diagnosticsList ) = 0;

    /** update document text changes - to be implemented on derived servers
     * @param replacement_text - text to be replaced over the provided range
     * @param start_line - starting replace line number ( zero-based )
     * @param start_character - starting replace column number ( zero-based )
     * @param end_line - ending replace line number ( zero-based )
     * @param end_character - ending replace column number ( zero-based )
     * @param range_length - length of replace range - server specific
     * @return - true if the document text was updated successfully
     */
    virtual bool updateDocumentTextChanges(
                    const std::string & replacement_text ,
                          int           start_line       ,
                          int           start_character  ,
                          int           end_line         ,
                          int           end_character    ,
                          int           range_length     ) = 0;

    /** gather document completion items - to be implemented on derived servers
     * @param completionItems - data array of completion item objects to fill
     * @param is_incomplete - flag indicating if the completions are complete
     * @param line - line to be used for completions gathering logic
     * @param character - column to be used for completions gathering logic
     * @return - true if the gathering of items completed successfully
     */
    virtual bool gatherDocumentCompletionItems(
                          wasp::DataArray & completionItems  ,
                          bool      & is_incomplete    ,
                          int         line             ,
                          int         character        ) = 0;

    /** gather definition locations - to be implemented on derived servers
     * @param definitionLocations - data array of locations objects to fill
     * @param line - line to be used for locations gathering logic
     * @param character - column to be used for locations gathering logic
     * @return - true if the gathering of locations completed successfully
     */
    virtual bool gatherDocumentDefinitionLocations(
                          wasp::DataArray & definitionLocations ,
                          int         line                ,
                          int         character           ) = 0;

    /** get hover display text - details are implemented on derived servers
     ** rather than being pure virtual like similar server specific methods
     ** this base implementation leaves display text empty and returns true
     ** so derived servers do not require method for backward compatibility
     * @param displayText - string reference to add text displayed on hover
     * @param line - zero-based line to use for finding node and hover text
     * @param character - zero-based column for finding node and hover text
     * @return - true if display text was added or unmodified without error
     */
    virtual bool getHoverDisplayText(
                          std::string & /* displayText */ ,
                          int           /* line        */ ,
                          int           /* character   */ )  { return true; }

    /** gather references locations - to be implemented on derived servers
     * @param referencesLocations - data array of locations objects to fill
     * @param line - line to be used for locations gathering logic
     * @param character - column to be used for locations gathering logic
     * @param include_declaration - flag indicating declaration inclusion
     * @return - true if the gathering of locations completed successfully
     */
    virtual bool gatherDocumentReferencesLocations(
                          wasp::DataArray & referencesLocations ,
                          int         line                ,
                          int         character           ,
                          bool        include_declaration ) = 0;

    /** gather formatting text edits - to be implemented on derived servers
     * @param formattingTextEdits - data array of text edit objects to fill
     * @param tab_size - value of the size of a tab in spaces for formatting
     * @param insert_spaces - flag indicating whether to use spaces for tabs
     * @return - true if the gathering of text edits completed successfully
     */
    virtual bool gatherDocumentFormattingTextEdits(
                          wasp::DataArray & formattingTextEdits ,
                          int         tab_size            ,
                          bool        insert_spaces       ) = 0;

    /** gather document symbols - to be implemented on derived servers
     * @param documentSymbols - data array of symbols data objects to fill
     * @return - true if the gathering of symbols completed successfully
     */
    virtual bool gatherDocumentSymbols(
                          wasp::DataArray & documentSymbols ) = 0;


    /** read from connection into object - to be implemented on derived servers
     * @param object - reference to object to be read into
     * @return - true if the read from the connection completed successfully
     */
    virtual bool connectionRead( wasp::DataObject & object ) = 0;

    /** write object json to connection - to be implemented on derived servers
     * @param object - reference to object with contents to write to connection
     * @return - true if the write to the connection completed successfully
     */
    virtual bool connectionWrite( wasp::DataObject & object ) = 0;

    /** enable full document sync capability for server */
    void enableFullSync()
    {
        server_capabilities[m_text_doc_sync] = DataObject();
        this->server_capabilities[m_text_doc_sync][m_open_close] = true;
        this->server_capabilities[m_text_doc_sync][m_change] = m_change_full;
    }

    /** enable completion capability for server */
    void enableCompletion()
    {
        this->server_capabilities[m_completion_provider] = DataObject();
        this->server_capabilities[m_completion_provider][m_resolve_provider] = false;
    }

    /** enable definition capability for server */
    void enableDefinition()
    {
        this->server_capabilities[m_definition_provider] = true;
    }

    /** enable references capability for server */
    void enableReferences()
    {
        this->server_capabilities[m_references_provider] = true;
    }

    /** enable formatting capability for server */
    void enableFormatting()
    {
        this->server_capabilities[m_doc_format_provider] = true;
    }

    /** enable hover capability for server */
    void enableHover()
    {
        this->server_capabilities[m_hover_provider] = true;
    }

    /** enable document symbols capability for server */
    void enableSymbols()
    {
        this->server_capabilities[m_doc_symbol_provider] = true;
    }

    /**
     * @brief errors - all errors stored by the server for any reason
     */
    std::stringstream errors;

    /**
     * @brief is_initialized - is the server initialized and waiting to read
     */
    bool is_initialized;

    /**
     * @brief client_capabilities - capabilities that client sent in initialize
     */
    wasp::DataObject client_capabilities;

    /**
     * @brief server_capabilities - capabilities this server claims to support
     */
    wasp::DataObject server_capabilities;

    /**
     * @brief client_root_path - root workspace URI of client ( if applicable )
     */
    std::string client_root_path;

    /**
     * @brief client_process_id - id of the client process ( if applicable )
     */
    int client_process_id;

    /**
     * @brief client_request_id - request id of most recent request by client
     */
    int client_request_id;

    /**
     * @brief document_path - document URI for the current document operation
     */
    std::string document_path;

    /**
     * @brief document_language_id - language id of the open document
     */
    std::string document_language_id;

    /**
     * @brief document_text - full current document text of the open document
     */
    std::string document_text;

    /**
     * @brief document_versions - open document paths to current versions map
     */
    std::map<std::string, int> document_versions;

    /**
     * @brief client_snippet_support - client supports snippets as completion
     */
    bool client_snippet_support;
};

} // namespace lsp
} // namespace wasp

#endif // WASPLSP_SERVER_IMPL_H
