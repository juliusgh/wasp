#ifndef WASPLSP_SERVER_H
#define WASPLSP_SERVER_H

#include "wasplsp/LSP.h"
#include "wasplsp/Connection.h"
#include "waspcore/Object.h"
#include "waspcore/decl.h"
#include <iostream>

namespace wasp {
namespace lsp  {

template <class T>
class WASP_PUBLIC Server
{
  public:

    Server(){}

    ~Server(){}

    bool run()
    {
        return Impl.run();
    }

    bool handleInitializeRequest(
                    const DataObject & initializeRequest  ,
                          DataObject & initializeResponse )
    {
        return Impl.handleInitializeRequest( initializeRequest  ,
                                             initializeResponse );
    }

    bool handleInitializedNotification(
                    const DataObject & initializedNotification )
    {
        return Impl.handleInitializedNotification( initializedNotification );
    }

    bool handleDidOpenNotification(
                    const DataObject & didOpenNotification            ,
                          DataObject & publishDiagnosticsNotification )
    {
        return Impl.handleDidOpenNotification( didOpenNotification            ,
                                               publishDiagnosticsNotification );
    }

    bool handleDidChangeNotification(
                    const DataObject & didChangeNotification          ,
                          DataObject & publishDiagnosticsNotification )
    {
        return Impl.handleDidChangeNotification( didChangeNotification          ,
                                                 publishDiagnosticsNotification );
    }

    bool handleCompletionRequest(
                    const DataObject & completionRequest  ,
                          DataObject & completionResponse )
    {
        return Impl.handleCompletionRequest( completionRequest  ,
                                             completionResponse );
    }

    bool handleDefinitionRequest(
                    const DataObject & definitionRequest  ,
                          DataObject & definitionResponse )
    {
        return Impl.handleDefinitionRequest( definitionRequest  ,
                                             definitionResponse );
    }

    bool handleReferencesRequest(
                    const DataObject & referencesRequest  ,
                          DataObject & referencesResponse )
    {
        return Impl.handleReferencesRequest( referencesRequest  ,
                                             referencesResponse );
    }

    bool handleRangeFormattingRequest(
                    const DataObject & rangeFormattingRequest  ,
                          DataObject & rangeFormattingResponse )
    {
        return Impl.handleRangeFormattingRequest( rangeFormattingRequest  ,
                                                  rangeFormattingResponse );
    }

    bool handleDidCloseNotification(
                    const DataObject & didCloseNotification )
    {
        return Impl.handleDidCloseNotification( didCloseNotification  );
    }

    bool handleShutdownRequest(
                    const DataObject & shutdownRequest  ,
                          DataObject & shutdownResponse )
    {
        return Impl.handleShutdownRequest( shutdownRequest  ,
                                           shutdownResponse );
    }

    bool handleExitNotification(
                    const DataObject & exitNotification )
    {
        return Impl.handleExitNotification( exitNotification  );
    }

    std::shared_ptr<Connection> getConnection()
    {
        return Impl.getConnection();
    }

    bool isRunning()
    {
        return Impl.isRunning();
    }

    std::string getErrors()
    {
        return Impl.getErrors();
    }

  private:

      T Impl;
};

} // namespace lsp
} // namespace wasp

#endif // WASPLSP_SERVER_H
