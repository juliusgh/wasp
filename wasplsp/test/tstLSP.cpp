#include "wasplsp/LSP.h"
#include "gtest/gtest.h"
#include <sstream>
#include <vector>

namespace wasp {
namespace lsp  {

TEST(lsp, bad_ranges)
{
    {
        DataObject        object;
        std::stringstream errors;

        int               request_id      =  91;
        std::string       uri             = "test/document/uri/string";
        int               tab_size        =  04;
        bool              insert_spaces   =  true;

        int               start_line      =  17;
        int               start_character = -34;
        int               end_line        =  13;
        int               end_character   = -39;
        std::string       expected_errors = "Error:: Range start "
                                            "( line:17 column:-34 ) "
                                            "must be less than range end "
                                            "( line:13 column:-39 )\n"
                                            "Error:: Column number must be "
                                            "non-negative - received: -34\n"
                                            "Error:: Column number must be "
                                            "non-negative - received: -39\n";

        ASSERT_FALSE(buildFormattingRequest( object           ,
                                             errors           ,
                                             request_id       ,
                                             uri              ,
                                             start_line       ,
                                             start_character  ,
                                             end_line         ,
                                             end_character    ,
                                             tab_size         ,
                                             insert_spaces    ));

        ASSERT_EQ( errors.str() , expected_errors );
    }
    {
        DataObject        object;
        std::stringstream errors;

        int               request_id      =  92;
        std::string       uri             = "test/document/uri/string";
        int               tab_size        =  04;
        bool              insert_spaces   =  true;

        int               start_line      =  17;
        int               start_character =  39;
        int               end_line        =  17;
        int               end_character   =  34;
        std::string       expected_errors = "Error:: Range start "
                                            "( line:17 column:39 ) "
                                            "must be less than range end "
                                            "( line:17 column:34 )\n";

        ASSERT_FALSE(buildFormattingRequest( object           ,
                                             errors           ,
                                             request_id       ,
                                             uri              ,
                                             start_line       ,
                                             start_character  ,
                                             end_line         ,
                                             end_character    ,
                                             tab_size         ,
                                             insert_spaces    ));

        ASSERT_EQ( errors.str() , expected_errors );
    }
}

TEST(lsp, initialize_request)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id =  1;
    int               process_id =  12345;
    std::string       root_uri   = "test/root/uri/string";
    DataObject        client_capabilities;

    ASSERT_TRUE(buildInitializeRequest( object              ,
                                        errors              ,
                                        request_id          ,
                                        process_id          ,
                                        root_uri            ,
                                        client_capabilities ));

    ASSERT_EQ  ( object.size() , (size_t) 3 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_method].is_string()                       );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_initialize );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 3 );

    ASSERT_TRUE( object[m_params][m_process_id].is_int()              );
    ASSERT_EQ  ( object[m_params][m_process_id].to_int() , process_id );

    ASSERT_TRUE( object[m_params][m_root_uri].is_string()            );
    ASSERT_EQ  ( object[m_params][m_root_uri].to_string() , root_uri );

    ASSERT_TRUE( object[m_params][m_capabilities].is_object()         );
    ASSERT_EQ  ( object[m_params][m_capabilities].size() , (size_t) 0 );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 171\r\n\r\n" << R"INPUT({
  "id" : 1
  ,"jsonrpc" : "2.0"
  ,"method" : "initialize"
  ,"params" : {
    "capabilities" : {}
    ,"processId" : 12345
    ,"rootUri" : "test/root/uri/string"
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                       );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_initialize );

    int         tst_request_id;
    int         tst_process_id;
    std::string tst_root_uri;
    DataObject  tst_client_capabilities;

    ASSERT_TRUE(dissectInitializeRequest( tst_object              ,
                                          errors                  ,
                                          tst_request_id          ,
                                          tst_process_id          ,
                                          tst_root_uri            ,
                                          tst_client_capabilities ));

    ASSERT_EQ( tst_request_id                 , request_id );
    ASSERT_EQ( tst_process_id                 , process_id );
    ASSERT_EQ( tst_root_uri                   , root_uri   );
    ASSERT_EQ( tst_client_capabilities.size() , (size_t) 0 );
}

TEST(lsp, initialized_notification)
{
    DataObject        object;
    std::stringstream errors;

    ASSERT_TRUE(buildInitializedNotification( object ,
                                              errors ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_method].is_string()                        );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_initialized );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 0 );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 68\r\n\r\n" << R"INPUT({
  "jsonrpc" : "2.0"
  ,"method" : "initialized"
  ,"params" : {}
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                        );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_initialized );

    ASSERT_TRUE(dissectInitializedNotification( object ,
                                                errors ));
}

TEST(lsp, didopen_notification)
{
    DataObject        object;
    std::stringstream errors;

    std::string       uri         = "test/document/uri/string";
    std::string       language_id = "test_language_id_string";
    int               version     =  1;
    std::string       text        = "test text string";

    ASSERT_TRUE(buildDidOpenNotification( object       ,
                                          errors       ,
                                          uri          ,
                                          language_id  ,
                                          version      ,
                                          text         ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_method].is_string()                    );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_didopen );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document].is_object()         );
    ASSERT_EQ  ( object[m_params][m_text_document].size() , (size_t) 4 );

    ASSERT_TRUE( object[m_params][m_text_document][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_text_document][m_uri].to_string() , uri );

    ASSERT_TRUE( object[m_params][m_text_document][m_language_id].is_string()               );
    ASSERT_EQ  ( object[m_params][m_text_document][m_language_id].to_string() , language_id );

    ASSERT_TRUE( object[m_params][m_text_document][m_version].is_int()           );
    ASSERT_EQ  ( object[m_params][m_text_document][m_version].to_int() , version );

    ASSERT_TRUE( object[m_params][m_text_document][m_text].is_string()        );
    ASSERT_EQ  ( object[m_params][m_text_document][m_text].to_string() , text );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 244\r\n\r\n" << R"INPUT({
  "jsonrpc" : "2.0"
  ,"method" : "textDocument/didOpen"
  ,"params" : {
    "textDocument" : {
    "languageId" : "test_language_id_string"
    ,"text" : "test text string"
    ,"uri" : "test/document/uri/string"
    ,"version" : 1
  }
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                    );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_didopen );

    std::string tst_uri;
    std::string tst_language_id;
    int         tst_version;
    std::string tst_text;

    ASSERT_TRUE(dissectDidOpenNotification( object          ,
                                            errors          ,
                                            tst_uri         ,
                                            tst_language_id ,
                                            tst_version     ,
                                            tst_text        ));

    ASSERT_EQ( tst_uri         , uri         );
    ASSERT_EQ( tst_language_id , language_id );
    ASSERT_EQ( tst_version     , version     );
    ASSERT_EQ( tst_text        , text        );

}

TEST(lsp, didchange_notification)
{
    DataObject        object;
    std::stringstream errors;

    std::string       uri             = "test/document/uri/string";
    int               version         =  14;
    int               start_line      =  10;
    int               start_character =  16;
    int               end_line        =  62;
    int               end_character   =  12;
    int               range_length    =  37;
    std::string       text            = "test text replacement string";

    ASSERT_TRUE(buildDidChangeNotification( object          ,
                                            errors          ,
                                            uri             ,
                                            version         ,
                                            start_line      ,
                                            start_character ,
                                            end_line        ,
                                            end_character   ,
                                            range_length    ,
                                            text            ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_method].is_string()                      );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_didchange );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_text_document].is_object()         );
    ASSERT_EQ  ( object[m_params][m_text_document].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_text_document][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_text_document][m_uri].to_string() , uri );

    ASSERT_TRUE( object[m_params][m_text_document][m_version].is_int()           );
    ASSERT_EQ  ( object[m_params][m_text_document][m_version].to_int() , version );

    ASSERT_TRUE( object[m_params][m_content_changes].is_object()         );
    ASSERT_EQ  ( object[m_params][m_content_changes].size() , (size_t) 3 );

    ASSERT_TRUE( object[m_params][m_content_changes][m_range].is_object()         );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_content_changes][m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_content_changes][m_range][m_start][m_line].is_int()              );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_range][m_start][m_line].to_int() , start_line );

    ASSERT_TRUE( object[m_params][m_content_changes][m_range][m_start][m_character].is_int()                   );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_range][m_start][m_character].to_int() , start_character );

    ASSERT_TRUE( object[m_params][m_content_changes][m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_content_changes][m_range][m_end][m_line].is_int()            );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_range][m_end][m_line].to_int() , end_line );

    ASSERT_TRUE( object[m_params][m_content_changes][m_range][m_end][m_character].is_int()                 );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_range][m_end][m_character].to_int() , end_character );

    ASSERT_TRUE( object[m_params][m_content_changes][m_range_length].is_int()                );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_range_length].to_int() , range_length );

    ASSERT_TRUE( object[m_params][m_content_changes][m_text].is_string()        );
    ASSERT_EQ  ( object[m_params][m_content_changes][m_text].to_string() , text );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 415\r\n\r\n" << R"INPUT({
  "jsonrpc" : "2.0"
  ,"method" : "textDocument/didChange"
  ,"params" : {
    "contentChanges" : {
    "range" : {
    "end" : {
    "character" : 12
    ,"line" : 62
  }
    ,"start" : {
      "character" : 16
      ,"line" : 10
    }
  }
    ,"range_length" : 37
    ,"text" : "test text replacement string"
  }
    ,"textDocument" : {
      "uri" : "test/document/uri/string"
      ,"version" : 14
    }
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                      );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_didchange );

    std::string tst_uri;
    int         tst_version;
    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    int         tst_range_length;
    std::string tst_text;

    ASSERT_TRUE(dissectDidChangeNotification( object              ,
                                              errors              ,
                                              tst_uri             ,
                                              tst_version         ,
                                              tst_start_line      ,
                                              tst_start_character ,
                                              tst_end_line        ,
                                              tst_end_character   ,
                                              tst_range_length    ,
                                              tst_text            ));

    ASSERT_EQ( tst_uri             , uri             );
    ASSERT_EQ( tst_version         , version         );
    ASSERT_EQ( tst_start_line      , start_line      );
    ASSERT_EQ( tst_start_character , start_character );
    ASSERT_EQ( tst_end_line        , end_line        );
    ASSERT_EQ( tst_end_character   , end_character   );
    ASSERT_EQ( tst_range_length    , range_length    );
    ASSERT_EQ( tst_text            , text            );
}

TEST(lsp, completion_request)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id =  2;
    std::string       uri        = "test/document/uri/string";
    int               line       =  87;
    int               character  =  48;

    ASSERT_TRUE(buildCompletionRequest( object     ,
                                        errors     ,
                                        request_id ,
                                        uri        ,
                                        line       ,
                                        character  ));

    ASSERT_EQ  ( object.size() , (size_t) 3 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_method].is_string()                       );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_completion );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_text_document].is_object()         );
    ASSERT_EQ  ( object[m_params][m_text_document].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_text_document][m_uri].to_string() , uri );

    ASSERT_TRUE( object[m_params][m_position].is_object()         );
    ASSERT_EQ  ( object[m_params][m_position].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_position][m_line].is_int()       );
    ASSERT_EQ  ( object[m_params][m_position][m_line].to_int() , line );

    ASSERT_TRUE( object[m_params][m_position][m_character].is_int()             );
    ASSERT_EQ  ( object[m_params][m_position][m_character].to_int() , character );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 227\r\n\r\n" << R"INPUT({
  "id" : 2
  ,"jsonrpc" : "2.0"
  ,"method" : "textDocument/completion"
  ,"params" : {
    "position" : {
    "character" : 48
    ,"line" : 87
  }
    ,"textDocument" : {
      "uri" : "test/document/uri/string"
    }
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                       );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_completion );

    int         tst_request_id ;
    std::string tst_uri;
    int         tst_line;
    int         tst_character;

    ASSERT_TRUE(dissectCompletionRequest( object         ,
                                          errors         ,
                                          tst_request_id ,
                                          tst_uri        ,
                                          tst_line       ,
                                          tst_character  ));

    ASSERT_EQ( tst_request_id , request_id );
    ASSERT_EQ( tst_uri        , uri        );
    ASSERT_EQ( tst_line       , line       );
    ASSERT_EQ( tst_character  , character  );

}

TEST(lsp, definition_request)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id =  3;
    std::string       uri        = "test/document/uri/string";
    int               line       =  25;
    int               character  =  52;

    ASSERT_TRUE(buildDefinitionRequest( object     ,
                                        errors     ,
                                        request_id ,
                                        uri        ,
                                        line       ,
                                        character  ));

    ASSERT_EQ  ( object.size() , (size_t) 3 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_method].is_string()                       );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_definition );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_text_document].is_object()         );
    ASSERT_EQ  ( object[m_params][m_text_document].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_text_document][m_uri].to_string() , uri );

    ASSERT_TRUE( object[m_params][m_position].is_object()         );
    ASSERT_EQ  ( object[m_params][m_position].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_position][m_line].is_int()       );
    ASSERT_EQ  ( object[m_params][m_position][m_line].to_int() , line );

    ASSERT_TRUE( object[m_params][m_position][m_character].is_int()             );
    ASSERT_EQ  ( object[m_params][m_position][m_character].to_int() , character );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 227\r\n\r\n" << R"INPUT({
  "id" : 3
  ,"jsonrpc" : "2.0"
  ,"method" : "textDocument/definition"
  ,"params" : {
    "position" : {
    "character" : 52
    ,"line" : 25
  }
    ,"textDocument" : {
      "uri" : "test/document/uri/string"
    }
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                       );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_definition );

    int         tst_request_id ;
    std::string tst_uri;
    int         tst_line;
    int         tst_character;

    ASSERT_TRUE(dissectDefinitionRequest( object         ,
                                          errors         ,
                                          tst_request_id ,
                                          tst_uri        ,
                                          tst_line       ,
                                          tst_character  ));

    ASSERT_EQ( tst_request_id , request_id );
    ASSERT_EQ( tst_uri        , uri        );
    ASSERT_EQ( tst_line       , line       );
    ASSERT_EQ( tst_character  , character  );
}

TEST(lsp, references_request)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id          =  4;
    std::string       uri                 = "test/document/uri/string";
    int               line                =  25;
    int               character           =  52;
    bool              include_declaration = false;

    ASSERT_TRUE(buildReferencesRequest( object              ,
                                        errors              ,
                                        request_id          ,
                                        uri                 ,
                                        line                ,
                                        character           ,
                                        include_declaration ));

    ASSERT_EQ  ( object.size() , (size_t) 3 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_method].is_string()                       );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_references );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 3 );

    ASSERT_TRUE( object[m_params][m_text_document].is_object()         );
    ASSERT_EQ  ( object[m_params][m_text_document].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_text_document][m_uri].to_string() , uri );

    ASSERT_TRUE( object[m_params][m_position].is_object()         );
    ASSERT_EQ  ( object[m_params][m_position].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_position][m_line].is_int()       );
    ASSERT_EQ  ( object[m_params][m_position][m_line].to_int() , line );

    ASSERT_TRUE( object[m_params][m_position][m_character].is_int()             );
    ASSERT_EQ  ( object[m_params][m_position][m_character].to_int() , character );

    ASSERT_TRUE( object[m_params][m_context].is_object()         );
    ASSERT_EQ  ( object[m_params][m_context].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_context][m_include_declaration].is_bool()                       );
    ASSERT_EQ  ( object[m_params][m_context][m_include_declaration].to_bool() , include_declaration );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 289\r\n\r\n" << R"INPUT({
  "id" : 4
  ,"jsonrpc" : "2.0"
  ,"method" : "textDocument/references"
  ,"params" : {
    "context" : {
    "includeDeclaration" : false
  }
    ,"position" : {
      "character" : 52
      ,"line" : 25
    }
    ,"textDocument" : {
      "uri" : "test/document/uri/string"
    }
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                       );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_references );

    int         tst_request_id ;
    std::string tst_uri;
    int         tst_line;
    int         tst_character;
    bool        tst_include_declaration;

    ASSERT_TRUE(dissectReferencesRequest( object                  ,
                                          errors                  ,
                                          tst_request_id          ,
                                          tst_uri                 ,
                                          tst_line                ,
                                          tst_character           ,
                                          tst_include_declaration ));

    ASSERT_EQ( tst_request_id          , request_id           );
    ASSERT_EQ( tst_uri                 , uri                  );
    ASSERT_EQ( tst_line                , line                 );
    ASSERT_EQ( tst_character           , character            );
    ASSERT_EQ( tst_include_declaration , include_declaration  );
}

TEST(lsp, formatting_request)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id      =  5;
    std::string       uri             = "test/document/uri/string";
    int               start_line      =  54;
    int               start_character =  65;
    int               end_line        =  82;
    int               end_character   =  12;
    int               tab_size        =  04;
    bool              insert_spaces   =  true;

    ASSERT_TRUE(buildFormattingRequest( object          ,
                                        errors          ,
                                        request_id      ,
                                        uri             ,
                                        start_line      ,
                                        start_character ,
                                        end_line        ,
                                        end_character   ,
                                        tab_size        ,
                                        insert_spaces   ));

    ASSERT_EQ  ( object.size() , (size_t) 3 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_method].is_string()                        );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_rangeformat );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 3 );

    ASSERT_TRUE( object[m_params][m_text_document].is_object()         );
    ASSERT_EQ  ( object[m_params][m_text_document].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_text_document][m_uri].to_string() , uri );

    ASSERT_TRUE( object[m_params][m_range].is_object()         );
    ASSERT_EQ  ( object[m_params][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_params][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_range][m_start][m_line].is_int()              );
    ASSERT_EQ  ( object[m_params][m_range][m_start][m_line].to_int() , start_line );

    ASSERT_TRUE( object[m_params][m_range][m_start][m_character].is_int()                   );
    ASSERT_EQ  ( object[m_params][m_range][m_start][m_character].to_int() , start_character );

    ASSERT_TRUE( object[m_params][m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_params][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_range][m_end][m_line].is_int()            );
    ASSERT_EQ  ( object[m_params][m_range][m_end][m_line].to_int() , end_line );

    ASSERT_TRUE( object[m_params][m_range][m_end][m_character].is_int()                 );
    ASSERT_EQ  ( object[m_params][m_range][m_end][m_character].to_int() , end_character );

    ASSERT_TRUE( object[m_params][m_options].is_object()         );
    ASSERT_EQ  ( object[m_params][m_options].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_options][m_tab_size].is_int() );
    ASSERT_EQ  ( object[m_params][m_options][m_tab_size].to_int() , tab_size );

    ASSERT_TRUE( object[m_params][m_options][m_insert_spaces].is_bool()                 );
    ASSERT_EQ  ( object[m_params][m_options][m_insert_spaces].to_bool() , insert_spaces );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 398\r\n\r\n" << R"INPUT({
  "id" : 5
  ,"jsonrpc" : "2.0"
  ,"method" : "textDocument/rangeFormatting"
  ,"params" : {
    "options" : {
    "insertSpaces" : true
    ,"tabSize" : 4
  }
    ,"range" : {
      "end" : {
      "character" : 12
      ,"line" : 82
    }
      ,"start" : {
        "character" : 65
        ,"line" : 54
      }
    }
    ,"textDocument" : {
      "uri" : "test/document/uri/string"
    }
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                        );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_rangeformat );

    int         tst_request_id ;
    std::string tst_uri;
    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    int         tst_tab_size;
    bool        tst_insert_spaces;

    ASSERT_TRUE(dissectFormattingRequest( object              ,
                                          errors              ,
                                          tst_request_id      ,
                                          tst_uri             ,
                                          tst_start_line      ,
                                          tst_start_character ,
                                          tst_end_line        ,
                                          tst_end_character   ,
                                          tst_tab_size        ,
                                          tst_insert_spaces   ));

    ASSERT_EQ( tst_request_id      , request_id      );
    ASSERT_EQ( tst_uri             , uri             );
    ASSERT_EQ( tst_start_line      , start_line      );
    ASSERT_EQ( tst_start_character , start_character );
    ASSERT_EQ( tst_end_line        , end_line        );
    ASSERT_EQ( tst_end_character   , end_character   );
    ASSERT_EQ( tst_tab_size        , tab_size        );
    ASSERT_EQ( tst_insert_spaces   , insert_spaces   );
}

TEST(lsp, symbols_request)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id      =  6;
    std::string       uri             = "test/document/uri/string";

    ASSERT_TRUE(buildSymbolsRequest( object     ,
                                     errors     ,
                                     request_id ,
                                     uri        ));

    ASSERT_EQ  ( object.size() , (size_t) 3 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_method].is_string()                           );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_documentsymbol );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document].is_object()         );
    ASSERT_EQ  ( object[m_params][m_text_document].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_text_document][m_uri].to_string() , uri );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 165\r\n\r\n" << R"INPUT({
  "id" : 6
  ,"jsonrpc" : "2.0"
  ,"method" : "textDocument/documentSymbol"
  ,"params" : {
    "textDocument" : {
    "uri" : "test/document/uri/string"
  }
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                           );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_documentsymbol );

    int         tst_request_id ;
    std::string tst_uri;

    ASSERT_TRUE(dissectSymbolsRequest( object         ,
                                       errors         ,
                                       tst_request_id ,
                                       tst_uri        ));

    ASSERT_EQ( tst_request_id      , request_id      );
    ASSERT_EQ( tst_uri             , uri             );
}

TEST(lsp, didclose_notification)
{
    DataObject        object;
    std::stringstream errors;

    std::string       uri             = "test/document/uri/string";

    ASSERT_TRUE(buildDidCloseNotification( object ,
                                           errors ,
                                           uri    ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_method].is_string()                     );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_didclose );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document].is_object()         );
    ASSERT_EQ  ( object[m_params][m_text_document].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_params][m_text_document][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_text_document][m_uri].to_string() , uri );



    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 147\r\n\r\n" << R"INPUT({
  "jsonrpc" : "2.0"
  ,"method" : "textDocument/didClose"
  ,"params" : {
    "textDocument" : {
    "uri" : "test/document/uri/string"
  }
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                     );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_didclose );

    std::string tst_uri;

    ASSERT_TRUE(dissectDidCloseNotification( object  ,
                                             errors  ,
                                             tst_uri ));

    ASSERT_EQ( tst_uri , uri );
}

TEST(lsp, shutdown_request)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id = 7;

    ASSERT_TRUE(buildShutdownRequest( object     ,
                                      errors     ,
                                      request_id ));

    ASSERT_EQ  ( object.size() , (size_t) 3 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_method].is_string()                     );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_shutdown );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 0 );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 77\r\n\r\n" << R"INPUT({
  "id" : 7
  ,"jsonrpc" : "2.0"
  ,"method" : "shutdown"
  ,"params" : {}
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                     );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_shutdown );

    int tst_request_id;

    ASSERT_TRUE(dissectShutdownRequest( object         ,
                                        errors         ,
                                        tst_request_id ));

    ASSERT_EQ( tst_request_id , request_id );
}

TEST(lsp, exit_notification)
{
    DataObject        object;
    std::stringstream errors;

    ASSERT_TRUE(buildExitNotification( object ,
                                       errors ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_method].is_string()                 );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_exit );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 0 );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 61\r\n\r\n" << R"INPUT({
  "jsonrpc" : "2.0"
  ,"method" : "exit"
  ,"params" : {}
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                 );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_exit );

    ASSERT_TRUE(dissectExitNotification( object ,
                                         errors ));
}

TEST(lsp, diagnostic_object)
{
    DataObject        object;
    std::stringstream errors;

    int               start_line      = 36;
    int               start_character = 57;
    int               end_line        = 36;
    int               end_character   = 63;
    int               severity        = 01;
    std::string       code            = "test.syntax.error.code";
    std::string       source          = "test_source";
    std::string       message         = "Couldn't convert 'test' to double.";

    ASSERT_TRUE(buildDiagnosticObject( object          ,
                                       errors          ,
                                       start_line      ,
                                       start_character ,
                                       end_line        ,
                                       end_character   ,
                                       severity        ,
                                       code            ,
                                       source          ,
                                       message         ));

    ASSERT_EQ  ( object.size() , (size_t) 5 );

    ASSERT_TRUE( object[m_range].is_object()         );
    ASSERT_EQ  ( object[m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_start][m_line].is_int()              );
    ASSERT_EQ  ( object[m_range][m_start][m_line].to_int() , start_line );

    ASSERT_TRUE( object[m_range][m_start][m_character].is_int()                   );
    ASSERT_EQ  ( object[m_range][m_start][m_character].to_int() , start_character );

    ASSERT_TRUE( object[m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_end][m_line].is_int()            );
    ASSERT_EQ  ( object[m_range][m_end][m_line].to_int() , end_line );

    ASSERT_TRUE( object[m_range][m_end][m_character].is_int()                 );
    ASSERT_EQ  ( object[m_range][m_end][m_character].to_int() , end_character );

    ASSERT_TRUE( object[m_severity].is_int()            );
    ASSERT_EQ  ( object[m_severity].to_int() , severity );

    ASSERT_TRUE( object[m_code].is_string()        );
    ASSERT_EQ  ( object[m_code].to_string() , code );

    ASSERT_TRUE( object[m_source].is_string()          );
    ASSERT_EQ  ( object[m_source].to_string() , source );

    ASSERT_TRUE( object[m_message].is_string()           );
    ASSERT_EQ  ( object[m_message].to_string() , message );

    std::stringstream  json;
    object.format_json(json);

    std::stringstream json_expected;
    json_expected << R"INPUT({
  "code" : "test.syntax.error.code"
  ,"message" : "Couldn't convert 'test' to double."
  ,"range" : {
    "end" : {
    "character" : 63
    ,"line" : 36
  }
    ,"start" : {
      "character" : 57
      ,"line" : 36
    }
  }
  ,"severity" : 1
  ,"source" : "test_source"
})INPUT";

    ASSERT_EQ( json.str() , json_expected.str() );

    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    int         tst_severity;
    std::string tst_code;
    std::string tst_source;
    std::string tst_message;

    ASSERT_TRUE(dissectDiagnosticObject( object              ,
                                         errors              ,
                                         tst_start_line      ,
                                         tst_start_character ,
                                         tst_end_line        ,
                                         tst_end_character   ,
                                         tst_severity        ,
                                         tst_code            ,
                                         tst_source          ,
                                         tst_message         ));

    ASSERT_EQ( tst_start_line      , start_line      );
    ASSERT_EQ( tst_start_character , start_character );
    ASSERT_EQ( tst_end_line        , end_line        );
    ASSERT_EQ( tst_end_character   , end_character   );
    ASSERT_EQ( tst_severity        , severity        );
    ASSERT_EQ( tst_code            , code            );
    ASSERT_EQ( tst_source          , source          );
    ASSERT_EQ( tst_message         , message         );
}

TEST(lsp, publishdiagnostics_notification)
{
    DataObject        object;
    std::stringstream errors;

    std::string       uri = "test/document/uri/string";
    DataObject        diagnostic;
    DataArray         diagnostics;

    ASSERT_TRUE(buildDiagnosticObject( diagnostic         ,
                                       errors             ,
                                       20                 ,
                                       16                 ,
                                       20                 ,
                                       27                 ,
                                       1                  ,
                                       "test.code.11"     ,
                                       "test_source_11"   ,
                                       "Test message 11." ));

    diagnostics.push_back(diagnostic);

    ASSERT_TRUE(buildDiagnosticObject( diagnostic         ,
                                       errors             ,
                                       28                 ,
                                       56                 ,
                                       34                 ,
                                       27                 ,
                                       2                  ,
                                       "test.code.22"     ,
                                       "test_source_22"   ,
                                       "Test message 22." ));

    diagnostics.push_back(diagnostic);

    ASSERT_TRUE(buildDiagnosticObject( diagnostic         ,
                                       errors             ,
                                       47                 ,
                                       36                 ,
                                       47                 ,
                                       43                 ,
                                       3                  ,
                                       "test.code.33"     ,
                                       "test_source_33"   ,
                                       "Test message 33." ));

    diagnostics.push_back(diagnostic);

    ASSERT_TRUE(buildDiagnosticObject( diagnostic         ,
                                       errors             ,
                                       67                 ,
                                       45                 ,
                                       68                 ,
                                       16                 ,
                                       4                  ,
                                       "test.code.44"     ,
                                       "test_source_44"   ,
                                       "Test message 44." ));

    diagnostics.push_back(diagnostic);

    ASSERT_TRUE(buildDiagnosticObject( diagnostic         ,
                                       errors             ,
                                       87                 ,
                                       17                 ,
                                       88                 ,
                                       12                 ,
                                       1                  ,
                                       "test.code.55"     ,
                                       "test_source_55"   ,
                                       "Test message 55." ));

    diagnostics.push_back(diagnostic);

    ASSERT_TRUE(buildPublishDiagnosticsNotification( object      ,
                                                     errors      ,
                                                     uri         ,
                                                     diagnostics ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_method].is_string()                           );
    ASSERT_EQ  ( object[m_method].to_string() , m_method_pubdiagnostics );

    ASSERT_TRUE( object[m_params].is_object()         );
    ASSERT_EQ  ( object[m_params].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_uri].is_string()       );
    ASSERT_EQ  ( object[m_params][m_uri].to_string() , uri );

    ASSERT_TRUE( object[m_params][m_diagnostics].is_array()          );
    ASSERT_EQ  ( object[m_params][m_diagnostics].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_params][m_diagnostics][0].is_object()         );
    ASSERT_EQ  ( object[m_params][m_diagnostics][0].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_params][m_diagnostics][1].is_object()         );
    ASSERT_EQ  ( object[m_params][m_diagnostics][1].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_params][m_diagnostics][2].is_object()         );
    ASSERT_EQ  ( object[m_params][m_diagnostics][2].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_params][m_diagnostics][3].is_object()         );
    ASSERT_EQ  ( object[m_params][m_diagnostics][3].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4].is_object()         );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_range].is_object()         );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_range][m_start][m_line].is_int()      );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_range][m_start][m_line].to_int() , 87 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_range][m_start][m_character].is_int()      );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_range][m_start][m_character].to_int() , 17 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_range][m_end][m_line].is_int()      );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_range][m_end][m_line].to_int() , 88 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_range][m_end][m_character].is_int()      );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_range][m_end][m_character].to_int() , 12 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_severity].is_int()     );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_severity].to_int() , 1 );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_code].is_string()                  );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_code].to_string() , "test.code.55" );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_source].is_string()                    );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_source].to_string() , "test_source_55" );

    ASSERT_TRUE( object[m_params][m_diagnostics][4][m_message].is_string()                      );
    ASSERT_EQ  ( object[m_params][m_diagnostics][4][m_message].to_string() , "Test message 55." );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 1596\r\n\r\n" << R"INPUT({
  "jsonrpc" : "2.0"
  ,"method" : "textDocument/publishDiagnostics"
  ,"params" : {
    "diagnostics" : [
    {
    "code" : "test.code.11"
    ,"message" : "Test message 11."
    ,"range" : {
      "end" : {
      "character" : 27
      ,"line" : 20
    }
      ,"start" : {
        "character" : 16
        ,"line" : 20
      }
    }
    ,"severity" : 1
    ,"source" : "test_source_11"
  }
    ,{
    "code" : "test.code.22"
    ,"message" : "Test message 22."
    ,"range" : {
      "end" : {
      "character" : 27
      ,"line" : 34
    }
      ,"start" : {
        "character" : 56
        ,"line" : 28
      }
    }
    ,"severity" : 2
    ,"source" : "test_source_22"
  }
    ,{
    "code" : "test.code.33"
    ,"message" : "Test message 33."
    ,"range" : {
      "end" : {
      "character" : 43
      ,"line" : 47
    }
      ,"start" : {
        "character" : 36
        ,"line" : 47
      }
    }
    ,"severity" : 3
    ,"source" : "test_source_33"
  }
    ,{
    "code" : "test.code.44"
    ,"message" : "Test message 44."
    ,"range" : {
      "end" : {
      "character" : 16
      ,"line" : 68
    }
      ,"start" : {
        "character" : 45
        ,"line" : 67
      }
    }
    ,"severity" : 4
    ,"source" : "test_source_44"
  }
    ,{
    "code" : "test.code.55"
    ,"message" : "Test message 55."
    ,"range" : {
      "end" : {
      "character" : 12
      ,"line" : 88
    }
      ,"start" : {
        "character" : 17
        ,"line" : 87
      }
    }
    ,"severity" : 1
    ,"source" : "test_source_55"
  }
  ]
    ,"uri" : "test/document/uri/string"
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_TRUE( tst_object[m_method].is_string()                           );
    ASSERT_EQ  ( tst_object[m_method].to_string() , m_method_pubdiagnostics );

    std::string tst_uri;
    DataArray   tst_diagnostics;

    ASSERT_TRUE(dissectPublishDiagnosticsNotification( object          ,
                                                       errors          ,
                                                       tst_uri         ,
                                                       tst_diagnostics ));

    ASSERT_EQ( tst_uri                , uri        );
    ASSERT_EQ( tst_diagnostics.size() , (size_t) 5 );

    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    int         tst_severity;
    std::string tst_code;
    std::string tst_source;
    std::string tst_message;

    ASSERT_TRUE(dissectDiagnosticObject( *(tst_diagnostics[0].to_object()) ,
                                           errors                          ,
                                           tst_start_line                  ,
                                           tst_start_character             ,
                                           tst_end_line                    ,
                                           tst_end_character               ,
                                           tst_severity                    ,
                                           tst_code                        ,
                                           tst_source                      ,
                                           tst_message                     ));

    ASSERT_TRUE(dissectDiagnosticObject( *(tst_diagnostics[1].to_object()) ,
                                           errors                          ,
                                           tst_start_line                  ,
                                           tst_start_character             ,
                                           tst_end_line                    ,
                                           tst_end_character               ,
                                           tst_severity                    ,
                                           tst_code                        ,
                                           tst_source                      ,
                                           tst_message                     ));

    ASSERT_TRUE(dissectDiagnosticObject( *(tst_diagnostics[2].to_object()) ,
                                           errors                          ,
                                           tst_start_line                  ,
                                           tst_start_character             ,
                                           tst_end_line                    ,
                                           tst_end_character               ,
                                           tst_severity                    ,
                                           tst_code                        ,
                                           tst_source                      ,
                                           tst_message                     ));

    ASSERT_TRUE(dissectDiagnosticObject( *(tst_diagnostics[3].to_object()) ,
                                           errors                          ,
                                           tst_start_line                  ,
                                           tst_start_character             ,
                                           tst_end_line                    ,
                                           tst_end_character               ,
                                           tst_severity                    ,
                                           tst_code                        ,
                                           tst_source                      ,
                                           tst_message                     ));

    ASSERT_TRUE(dissectDiagnosticObject( *(tst_diagnostics[4].to_object()) ,
                                           errors                          ,
                                           tst_start_line                  ,
                                           tst_start_character             ,
                                           tst_end_line                    ,
                                           tst_end_character               ,
                                           tst_severity                    ,
                                           tst_code                        ,
                                           tst_source                      ,
                                           tst_message                     ));

    ASSERT_EQ( tst_start_line      , 87                 );
    ASSERT_EQ( tst_start_character , 17                 );
    ASSERT_EQ( tst_end_line        , 88                 );
    ASSERT_EQ( tst_end_character   , 12                 );
    ASSERT_EQ( tst_severity        , 1                  );
    ASSERT_EQ( tst_code            , "test.code.55"     );
    ASSERT_EQ( tst_source          , "test_source_55"   );
    ASSERT_EQ( tst_message         , "Test message 55." );
}

TEST(lsp, initialize_response)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id = 8;
    DataObject        server_capabilities;

    ASSERT_TRUE(buildInitializeResponse( object              ,
                                         errors              ,
                                         request_id          ,
                                         server_capabilities ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_result].is_object()         );
    ASSERT_EQ  ( object[m_result].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_result][m_capabilities].is_object()         );
    ASSERT_EQ  ( object[m_result][m_capabilities].size() , (size_t) 0 );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 79\r\n\r\n" << R"INPUT({
  "id" : 8
  ,"jsonrpc" : "2.0"
  ,"result" : {
    "capabilities" : {}
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_FALSE( tst_object[m_method].is_string() );

    int        tst_request_id;
    DataObject tst_server_capabilities;

    ASSERT_TRUE(dissectInitializeResponse( object                  ,
                                           errors                  ,
                                           tst_request_id          ,
                                           tst_server_capabilities ));

    ASSERT_EQ( tst_request_id                 , request_id );
    ASSERT_EQ( tst_server_capabilities.size() , (size_t) 0 );
}

TEST(lsp, completion_object)
{
    DataObject        object;
    std::stringstream errors;

    std::string       label           = "test-label";
    int               start_line      = 73;
    int               start_character = 12;
    int               end_line        = 73;
    int               end_character   = 12;
    std::string       new_text        = "test-insert-text";
    int               kind            = 3;
    std::string       detail          = "test type info";
    std::string       documentation   = "test documentation";
    bool              deprecated      = false;
    bool              preselect       = true;

    ASSERT_TRUE(buildCompletionObject( object          ,
                                       errors          ,
                                       label           ,
                                       start_line      ,
                                       start_character ,
                                       end_line        ,
                                       end_character   ,
                                       new_text        ,
                                       kind            ,
                                       detail          ,
                                       documentation   ,
                                       deprecated      ,
                                       preselect       ));

    ASSERT_EQ  ( object.size() , (size_t) 7 );

    ASSERT_TRUE( object[m_label].is_string()         );
    ASSERT_EQ  ( object[m_label].to_string() , label );

    ASSERT_TRUE( object[m_text_edit].is_object()         );
    ASSERT_EQ  ( object[m_text_edit].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_text_edit][m_range].is_object()         );
    ASSERT_EQ  ( object[m_text_edit][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_text_edit][m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_text_edit][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_text_edit][m_range][m_start][m_line].is_int()              );
    ASSERT_EQ  ( object[m_text_edit][m_range][m_start][m_line].to_int() , start_line );

    ASSERT_TRUE( object[m_text_edit][m_range][m_start][m_character].is_int()                   );
    ASSERT_EQ  ( object[m_text_edit][m_range][m_start][m_character].to_int() , start_character );

    ASSERT_TRUE( object[m_text_edit][m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_text_edit][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_text_edit][m_range][m_end][m_line].is_int()            );
    ASSERT_EQ  ( object[m_text_edit][m_range][m_end][m_line].to_int() , end_line );

    ASSERT_TRUE( object[m_text_edit][m_range][m_end][m_character].is_int()                 );
    ASSERT_EQ  ( object[m_text_edit][m_range][m_end][m_character].to_int() , end_character );

    ASSERT_TRUE( object[m_text_edit][m_new_text].is_string()            );
    ASSERT_EQ  ( object[m_text_edit][m_new_text].to_string() , new_text );

    ASSERT_TRUE( object[m_kind].is_int()        );
    ASSERT_EQ  ( object[m_kind].to_int() , kind );

    ASSERT_TRUE( object[m_detail].is_string()          );
    ASSERT_EQ  ( object[m_detail].to_string() , detail );

    ASSERT_TRUE( object[m_documentation].is_string()                 );
    ASSERT_EQ  ( object[m_documentation].to_string() , documentation );

    ASSERT_TRUE( object[m_deprecated].is_bool()              );
    ASSERT_EQ  ( object[m_deprecated].to_bool() , deprecated );

    ASSERT_TRUE( object[m_preselect].is_bool()             );
    ASSERT_EQ  ( object[m_preselect].to_bool() , preselect );

    std::stringstream  json;
    object.format_json(json);

    std::stringstream json_expected;
    json_expected << R"INPUT({
  "deprecated" : false
  ,"detail" : "test type info"
  ,"documentation" : "test documentation"
  ,"kind" : 3
  ,"label" : "test-label"
  ,"preselect" : true
  ,"textEdit" : {
    "newText" : "test-insert-text"
    ,"range" : {
      "end" : {
      "character" : 12
      ,"line" : 73
    }
      ,"start" : {
        "character" : 12
        ,"line" : 73
      }
    }
  }
})INPUT";

    ASSERT_EQ( json.str() , json_expected.str() );

    std::string tst_label;
    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    std::string tst_new_text;
    int         tst_kind;
    std::string tst_detail;
    std::string tst_documentation;
    bool        tst_deprecated;
    bool        tst_preselect;

    ASSERT_TRUE(dissectCompletionObject( object              ,
                                         errors              ,
                                         tst_label           ,
                                         tst_start_line      ,
                                         tst_start_character ,
                                         tst_end_line        ,
                                         tst_end_character   ,
                                         tst_new_text        ,
                                         tst_kind            ,
                                         tst_detail          ,
                                         tst_documentation   ,
                                         tst_deprecated      ,
                                         tst_preselect       ));

    ASSERT_EQ( tst_label           , label           );
    ASSERT_EQ( tst_start_line      , start_line      );
    ASSERT_EQ( tst_start_character , start_character );
    ASSERT_EQ( tst_end_line        , end_line        );
    ASSERT_EQ( tst_end_character   , end_character   );
    ASSERT_EQ( tst_new_text        , new_text        );
    ASSERT_EQ( tst_kind            , kind            );
    ASSERT_EQ( tst_detail          , detail          );
    ASSERT_EQ( tst_documentation   , documentation   );
    ASSERT_EQ( tst_deprecated      , deprecated      );
    ASSERT_EQ( tst_preselect       , preselect       );
}

TEST(lsp, completion_response)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id    = 9;
    bool              is_incomplete = false;
    DataObject        completion_item;
    DataArray         completion_items;

    ASSERT_TRUE(buildCompletionObject( completion_item        ,
                                       errors                 ,
                                       "test-label-1"         ,
                                       11                     ,
                                       11                     ,
                                       11                     ,
                                       11                     ,
                                       "test-insert-text-1"   ,
                                       1                      ,
                                       "test type info 1"     ,
                                       "test documentation 1" ,
                                       false                  ,
                                       true                   ));

    completion_items.push_back(completion_item);

    ASSERT_TRUE(buildCompletionObject( completion_item        ,
                                       errors                 ,
                                       "test-label-2"         ,
                                       22                     ,
                                       22                     ,
                                       22                     ,
                                       22                     ,
                                       "test-insert-text-2"   ,
                                       2                      ,
                                       "test type info 2"     ,
                                       "test documentation 2" ,
                                       false                  ,
                                       false                  ));

    completion_items.push_back(completion_item);

    ASSERT_TRUE(buildCompletionObject( completion_item        ,
                                       errors                 ,
                                       "test-label-3"         ,
                                       33                     ,
                                       33                     ,
                                       33                     ,
                                       33                     ,
                                       "test-insert-text-3"   ,
                                       3                      ,
                                       "test type info 3"     ,
                                       "test documentation 3" ,
                                       false                  ,
                                       false                  ));

    completion_items.push_back(completion_item);

    ASSERT_TRUE(buildCompletionObject( completion_item        ,
                                       errors                 ,
                                       "test-label-4"         ,
                                       44                     ,
                                       44                     ,
                                       44                     ,
                                       44                     ,
                                       "test-insert-text-4"   ,
                                       4                      ,
                                       "test type info 4"     ,
                                       "test documentation 4" ,
                                       false                  ,
                                       false                  ));

    completion_items.push_back(completion_item);

    ASSERT_TRUE(buildCompletionObject( completion_item        ,
                                       errors                 ,
                                       "test-label-5"         ,
                                       55                     ,
                                       55                     ,
                                       55                     ,
                                       55                     ,
                                       "test-insert-text-5"   ,
                                       5                      ,
                                       "test type info 5"     ,
                                       "test documentation 5" ,
                                       false                  ,
                                       false                  ));

    completion_items.push_back(completion_item);

    ASSERT_TRUE(buildCompletionResponse( object           ,
                                         errors           ,
                                         request_id       ,
                                         is_incomplete    ,
                                         completion_items ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_result].is_object()         );
    ASSERT_EQ  ( object[m_result].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][m_is_incomplete].is_bool()         );
    ASSERT_EQ  ( object[m_result][m_is_incomplete].to_bool() , false );

    ASSERT_TRUE( object[m_result][m_items].is_array()          );
    ASSERT_EQ  ( object[m_result][m_items].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_result][m_items][0].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][0].size() , (size_t) 7 );

    ASSERT_TRUE( object[m_result][m_items][1].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][1].size() , (size_t) 7 );

    ASSERT_TRUE( object[m_result][m_items][2].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][2].size() , (size_t) 7 );

    ASSERT_TRUE( object[m_result][m_items][3].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][3].size() , (size_t) 7 );

    ASSERT_TRUE( object[m_result][m_items][4].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][4].size() , (size_t) 7 );

    ASSERT_TRUE( object[m_result][m_items][4][m_label].is_string()                  );
    ASSERT_EQ  ( object[m_result][m_items][4][m_label].to_string() , "test-label-5" );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit][m_range].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit][m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit][m_range][m_start][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit][m_range][m_start][m_line].to_int() , 55 );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit][m_range][m_start][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit][m_range][m_start][m_character].to_int() , 55 );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit][m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit][m_range][m_end][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit][m_range][m_end][m_line].to_int() , 55 );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit][m_range][m_end][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit][m_range][m_end][m_character].to_int() , 55 );

    ASSERT_TRUE( object[m_result][m_items][4][m_text_edit][m_new_text].is_string()                        );
    ASSERT_EQ  ( object[m_result][m_items][4][m_text_edit][m_new_text].to_string() , "test-insert-text-5" );

    ASSERT_TRUE( object[m_result][m_items][4][m_kind].is_int()     );
    ASSERT_EQ  ( object[m_result][m_items][4][m_kind].to_int() , 5 );

    ASSERT_TRUE( object[m_result][m_items][4][m_detail].is_string()                      );
    ASSERT_EQ  ( object[m_result][m_items][4][m_detail].to_string() , "test type info 5" );

    ASSERT_TRUE( object[m_result][m_items][4][m_documentation].is_string()                           );
    ASSERT_EQ  ( object[m_result][m_items][4][m_documentation].to_string() , "test documentation 5"  );

    ASSERT_TRUE( object[m_result][m_items][4][m_deprecated].is_bool()         );
    ASSERT_EQ  ( object[m_result][m_items][4][m_deprecated].to_bool() , false );

    ASSERT_TRUE( object[m_result][m_items][4][m_preselect].is_bool()         );
    ASSERT_EQ  ( object[m_result][m_items][4][m_preselect].to_bool() , false );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 2478\r\n\r\n" << R"INPUT({
  "id" : 9
  ,"jsonrpc" : "2.0"
  ,"result" : {
    "isIncomplete" : false
    ,"items" : [
      {
      "deprecated" : false
      ,"detail" : "test type info 1"
      ,"documentation" : "test documentation 1"
      ,"kind" : 1
      ,"label" : "test-label-1"
      ,"preselect" : true
      ,"textEdit" : {
        "newText" : "test-insert-text-1"
        ,"range" : {
          "end" : {
          "character" : 11
          ,"line" : 11
        }
          ,"start" : {
            "character" : 11
            ,"line" : 11
          }
        }
      }
    }
      ,{
      "deprecated" : false
      ,"detail" : "test type info 2"
      ,"documentation" : "test documentation 2"
      ,"kind" : 2
      ,"label" : "test-label-2"
      ,"preselect" : false
      ,"textEdit" : {
        "newText" : "test-insert-text-2"
        ,"range" : {
          "end" : {
          "character" : 22
          ,"line" : 22
        }
          ,"start" : {
            "character" : 22
            ,"line" : 22
          }
        }
      }
    }
      ,{
      "deprecated" : false
      ,"detail" : "test type info 3"
      ,"documentation" : "test documentation 3"
      ,"kind" : 3
      ,"label" : "test-label-3"
      ,"preselect" : false
      ,"textEdit" : {
        "newText" : "test-insert-text-3"
        ,"range" : {
          "end" : {
          "character" : 33
          ,"line" : 33
        }
          ,"start" : {
            "character" : 33
            ,"line" : 33
          }
        }
      }
    }
      ,{
      "deprecated" : false
      ,"detail" : "test type info 4"
      ,"documentation" : "test documentation 4"
      ,"kind" : 4
      ,"label" : "test-label-4"
      ,"preselect" : false
      ,"textEdit" : {
        "newText" : "test-insert-text-4"
        ,"range" : {
          "end" : {
          "character" : 44
          ,"line" : 44
        }
          ,"start" : {
            "character" : 44
            ,"line" : 44
          }
        }
      }
    }
      ,{
      "deprecated" : false
      ,"detail" : "test type info 5"
      ,"documentation" : "test documentation 5"
      ,"kind" : 5
      ,"label" : "test-label-5"
      ,"preselect" : false
      ,"textEdit" : {
        "newText" : "test-insert-text-5"
        ,"range" : {
          "end" : {
          "character" : 55
          ,"line" : 55
        }
          ,"start" : {
            "character" : 55
            ,"line" : 55
          }
        }
      }
    }
    ]
  }
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_FALSE( tst_object[m_method].is_string() );

    int       tst_request_id;
    bool      tst_is_incomplete;
    DataArray tst_completion_items;

    ASSERT_TRUE(dissectCompletionResponse( object               ,
                                           errors               ,
                                           tst_request_id       ,
                                           tst_is_incomplete    ,
                                           tst_completion_items ));

    ASSERT_EQ( tst_request_id              , tst_request_id    );
    ASSERT_EQ( tst_is_incomplete           , tst_is_incomplete );
    ASSERT_EQ( tst_completion_items.size() , (size_t) 5        );

    std::string tst_label;
    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    std::string tst_new_text;
    int         tst_kind;
    std::string tst_detail;
    std::string tst_documentation;
    bool        tst_deprecated;
    bool        tst_preselect;

    ASSERT_TRUE(dissectCompletionObject( *(tst_completion_items[0].to_object()) ,
                                           errors                               ,
                                           tst_label                            ,
                                           tst_start_line                       ,
                                           tst_start_character                  ,
                                           tst_end_line                         ,
                                           tst_end_character                    ,
                                           tst_new_text                         ,
                                           tst_kind                             ,
                                           tst_detail                           ,
                                           tst_documentation                    ,
                                           tst_deprecated                       ,
                                           tst_preselect                        ));

    ASSERT_TRUE(dissectCompletionObject( *(tst_completion_items[1].to_object()) ,
                                           errors                               ,
                                           tst_label                            ,
                                           tst_start_line                       ,
                                           tst_start_character                  ,
                                           tst_end_line                         ,
                                           tst_end_character                    ,
                                           tst_new_text                         ,
                                           tst_kind                             ,
                                           tst_detail                           ,
                                           tst_documentation                    ,
                                           tst_deprecated                       ,
                                           tst_preselect                        ));

    ASSERT_TRUE(dissectCompletionObject( *(tst_completion_items[2].to_object()) ,
                                           errors                               ,
                                           tst_label                            ,
                                           tst_start_line                       ,
                                           tst_start_character                  ,
                                           tst_end_line                         ,
                                           tst_end_character                    ,
                                           tst_new_text                         ,
                                           tst_kind                             ,
                                           tst_detail                           ,
                                           tst_documentation                    ,
                                           tst_deprecated                       ,
                                           tst_preselect                        ));

    ASSERT_TRUE(dissectCompletionObject( *(tst_completion_items[3].to_object()) ,
                                           errors                               ,
                                           tst_label                            ,
                                           tst_start_line                       ,
                                           tst_start_character                  ,
                                           tst_end_line                         ,
                                           tst_end_character                    ,
                                           tst_new_text                         ,
                                           tst_kind                             ,
                                           tst_detail                           ,
                                           tst_documentation                    ,
                                           tst_deprecated                       ,
                                           tst_preselect                        ));

    ASSERT_TRUE(dissectCompletionObject( *(tst_completion_items[4].to_object()) ,
                                           errors                               ,
                                           tst_label                            ,
                                           tst_start_line                       ,
                                           tst_start_character                  ,
                                           tst_end_line                         ,
                                           tst_end_character                    ,
                                           tst_new_text                         ,
                                           tst_kind                             ,
                                           tst_detail                           ,
                                           tst_documentation                    ,
                                           tst_deprecated                       ,
                                           tst_preselect                        ));

    ASSERT_EQ( tst_label           , "test-label-5"         );
    ASSERT_EQ( tst_start_line      , 55                     );
    ASSERT_EQ( tst_start_character , 55                     );
    ASSERT_EQ( tst_end_line        , 55                     );
    ASSERT_EQ( tst_end_character   , 55                     );
    ASSERT_EQ( tst_new_text        , "test-insert-text-5"   );
    ASSERT_EQ( tst_kind            , 5                      );
    ASSERT_EQ( tst_detail          , "test type info 5"     );
    ASSERT_EQ( tst_documentation   , "test documentation 5" );
    ASSERT_EQ( tst_deprecated      , false                  );
    ASSERT_EQ( tst_preselect       , false                  );
}

TEST(lsp, location_object)
{
    DataObject        object;
    std::stringstream errors;

    std::string       uri             = "test/document/uri/string";
    int               start_line      = 78;
    int               start_character = 44;
    int               end_line        = 82;
    int               end_character   = 35;

    ASSERT_TRUE(buildLocationObject( object          ,
                                     errors          ,
                                     uri             ,
                                     start_line      ,
                                     start_character ,
                                     end_line        ,
                                     end_character   ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_uri].is_string()       );
    ASSERT_EQ  ( object[m_uri].to_string() , uri );

    ASSERT_TRUE( object[m_range].is_object()         );
    ASSERT_EQ  ( object[m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_start][m_line].is_int()              );
    ASSERT_EQ  ( object[m_range][m_start][m_line].to_int() , start_line );

    ASSERT_TRUE( object[m_range][m_start][m_character].is_int()                   );
    ASSERT_EQ  ( object[m_range][m_start][m_character].to_int() , start_character );

    ASSERT_TRUE( object[m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_end][m_line].is_int()            );
    ASSERT_EQ  ( object[m_range][m_end][m_line].to_int() , end_line );

    ASSERT_TRUE( object[m_range][m_end][m_character].is_int()                 );
    ASSERT_EQ  ( object[m_range][m_end][m_character].to_int() , end_character );

    std::stringstream  json;
    object.format_json(json);

    std::stringstream json_expected;
    json_expected << R"INPUT({
  "range" : {
  "end" : {
  "character" : 35
  ,"line" : 82
}
  ,"start" : {
    "character" : 44
    ,"line" : 78
  }
}
  ,"uri" : "test/document/uri/string"
})INPUT";

    ASSERT_EQ( json.str() , json_expected.str() );

    std::string tst_uri;
    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;

    ASSERT_TRUE(dissectLocationObject( object              ,
                                       errors              ,
                                       tst_uri             ,
                                       tst_start_line      ,
                                       tst_start_character ,
                                       tst_end_line        ,
                                       tst_end_character   ));

    ASSERT_EQ( tst_uri             , uri             );
    ASSERT_EQ( tst_start_line      , start_line      );
    ASSERT_EQ( tst_start_character , start_character );
    ASSERT_EQ( tst_end_line        , end_line        );
    ASSERT_EQ( tst_end_character   , end_character   );
}

TEST(lsp, locations_response)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id = 10;
    DataObject        location;
    DataArray         locations;

    ASSERT_TRUE(buildLocationObject( location              ,
                                     errors                ,
                                     "test/document/uri/1" ,
                                     11                    ,
                                     11                    ,
                                     11                    ,
                                     11                    ));

    locations.push_back(location);

    ASSERT_TRUE(buildLocationObject( location              ,
                                     errors                ,
                                     "test/document/uri/2" ,
                                     22                    ,
                                     22                    ,
                                     22                    ,
                                     22                    ));

    locations.push_back(location);

    ASSERT_TRUE(buildLocationObject( location              ,
                                     errors                ,
                                     "test/document/uri/3" ,
                                     33                    ,
                                     33                    ,
                                     33                    ,
                                     33                    ));

    locations.push_back(location);

    ASSERT_TRUE(buildLocationObject( location              ,
                                     errors                ,
                                     "test/document/uri/4" ,
                                     44                    ,
                                     44                    ,
                                     44                    ,
                                     44                    ));

    locations.push_back(location);

    ASSERT_TRUE(buildLocationObject( location              ,
                                     errors                ,
                                     "test/document/uri/5" ,
                                     55                    ,
                                     55                    ,
                                     55                    ,
                                     55                    ));

    locations.push_back(location);

    ASSERT_TRUE(buildLocationsResponse( object     ,
                                        errors     ,
                                        request_id ,
                                        locations  ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_result].is_array()          );
    ASSERT_EQ  ( object[m_result].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_result][0].is_object()         );
    ASSERT_EQ  ( object[m_result][0].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][1].is_object()         );
    ASSERT_EQ  ( object[m_result][1].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][2].is_object()         );
    ASSERT_EQ  ( object[m_result][2].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][3].is_object()         );
    ASSERT_EQ  ( object[m_result][3].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4].is_object()         );
    ASSERT_EQ  ( object[m_result][4].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4][m_uri].is_string()                         );
    ASSERT_EQ  ( object[m_result][4][m_uri].to_string() , "test/document/uri/5" );

    ASSERT_TRUE( object[m_result][4][m_range].is_object()         );
    ASSERT_EQ  ( object[m_result][4][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4][m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_result][4][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4][m_range][m_start][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][4][m_range][m_start][m_line].to_int() , 55 );

    ASSERT_TRUE( object[m_result][4][m_range][m_start][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][4][m_range][m_start][m_character].to_int() , 55 );

    ASSERT_TRUE( object[m_result][4][m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_result][4][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4][m_range][m_end][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][4][m_range][m_end][m_line].to_int() , 55 );

    ASSERT_TRUE( object[m_result][4][m_range][m_end][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][4][m_range][m_end][m_character].to_int() , 55 );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 990\r\n\r\n" << R"INPUT({
  "id" : 10
  ,"jsonrpc" : "2.0"
  ,"result" : [
    {
    "range" : {
    "end" : {
    "character" : 11
    ,"line" : 11
  }
    ,"start" : {
      "character" : 11
      ,"line" : 11
    }
  }
    ,"uri" : "test/document/uri/1"
  }
    ,{
    "range" : {
    "end" : {
    "character" : 22
    ,"line" : 22
  }
    ,"start" : {
      "character" : 22
      ,"line" : 22
    }
  }
    ,"uri" : "test/document/uri/2"
  }
    ,{
    "range" : {
    "end" : {
    "character" : 33
    ,"line" : 33
  }
    ,"start" : {
      "character" : 33
      ,"line" : 33
    }
  }
    ,"uri" : "test/document/uri/3"
  }
    ,{
    "range" : {
    "end" : {
    "character" : 44
    ,"line" : 44
  }
    ,"start" : {
      "character" : 44
      ,"line" : 44
    }
  }
    ,"uri" : "test/document/uri/4"
  }
    ,{
    "range" : {
    "end" : {
    "character" : 55
    ,"line" : 55
  }
    ,"start" : {
      "character" : 55
      ,"line" : 55
    }
  }
    ,"uri" : "test/document/uri/5"
  }
  ]
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_FALSE( tst_object[m_method].is_string() );

    int       tst_request_id;
    DataArray tst_locations;

    ASSERT_TRUE(dissectLocationsResponse( object         ,
                                          errors         ,
                                          tst_request_id ,
                                          tst_locations  ));

    ASSERT_EQ( tst_request_id       , tst_request_id    );
    ASSERT_EQ( tst_locations.size() , (size_t) 5        );

    std::string tst_uri;
    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;

    ASSERT_TRUE(dissectLocationObject( *(tst_locations[0].to_object()) ,
                                         errors                        ,
                                         tst_uri                       ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ));

    ASSERT_TRUE(dissectLocationObject( *(tst_locations[1].to_object()) ,
                                         errors                        ,
                                         tst_uri                       ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ));

    ASSERT_TRUE(dissectLocationObject( *(tst_locations[2].to_object()) ,
                                         errors                        ,
                                         tst_uri                       ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ));

    ASSERT_TRUE(dissectLocationObject( *(tst_locations[3].to_object()) ,
                                         errors                        ,
                                         tst_uri                       ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ));

    ASSERT_TRUE(dissectLocationObject( *(tst_locations[4].to_object()) ,
                                         errors                        ,
                                         tst_uri                       ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ));

    ASSERT_EQ( tst_uri             , "test/document/uri/5" );
    ASSERT_EQ( tst_start_line      ,  55                   );
    ASSERT_EQ( tst_start_character ,  55                   );
    ASSERT_EQ( tst_end_line        ,  55                   );
    ASSERT_EQ( tst_end_character   ,  55                   );
}

TEST(lsp, textedit_object)
{
    DataObject        object;
    std::stringstream errors;

    int               start_line      = 10;
    int               start_character = 01;
    int               end_line        = 14;
    int               end_character   = 03;
    std::string       new_text        = "test\n  new\n  text\n  format\n  0";

    ASSERT_TRUE(buildTextEditObject( object          ,
                                     errors          ,
                                     start_line      ,
                                     start_character ,
                                     end_line        ,
                                     end_character   ,
                                     new_text        ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range].is_object()         );
    ASSERT_EQ  ( object[m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_start][m_line].is_int()              );
    ASSERT_EQ  ( object[m_range][m_start][m_line].to_int() , start_line );

    ASSERT_TRUE( object[m_range][m_start][m_character].is_int()                   );
    ASSERT_EQ  ( object[m_range][m_start][m_character].to_int() , start_character );

    ASSERT_TRUE( object[m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_range][m_end][m_line].is_int()            );
    ASSERT_EQ  ( object[m_range][m_end][m_line].to_int() , end_line );

    ASSERT_TRUE( object[m_range][m_end][m_character].is_int()                 );
    ASSERT_EQ  ( object[m_range][m_end][m_character].to_int() , end_character );

    ASSERT_TRUE( object[m_new_text].is_string()            );
    ASSERT_EQ  ( object[m_new_text].to_string() , new_text );

    std::stringstream  json;
    object.format_json(json);

    std::stringstream json_expected;
    json_expected << R"INPUT({
  "newText" : "test\n  new\n  text\n  format\n  0"
  ,"range" : {
    "end" : {
    "character" : 3
    ,"line" : 14
  }
    ,"start" : {
      "character" : 1
      ,"line" : 10
    }
  }
})INPUT";

    ASSERT_EQ( json.str() , json_expected.str() );

    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    std::string tst_new_text;

    ASSERT_TRUE(dissectTextEditObject( object              ,
                                       errors              ,
                                       tst_start_line      ,
                                       tst_start_character ,
                                       tst_end_line        ,
                                       tst_end_character   ,
                                       tst_new_text        ));

    ASSERT_EQ( tst_start_line      , start_line      );
    ASSERT_EQ( tst_start_character , start_character );
    ASSERT_EQ( tst_end_line        , end_line        );
    ASSERT_EQ( tst_end_character   , end_character   );
    ASSERT_EQ( tst_new_text        , new_text        );
}

TEST(lsp, formatting_response)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id = 11;
    DataObject        textedit;
    DataArray         textedits;

    ASSERT_TRUE(buildTextEditObject( textedit                             ,
                                     errors                               ,
                                     10                                   ,
                                     01                                   ,
                                     14                                   ,
                                     03                                   ,
                                     "new\n  \"text\"\n  f\tor\\mat\n  1" ));

    textedits.push_back(textedit);

    ASSERT_TRUE(buildTextEditObject( textedit                             ,
                                     errors                               ,
                                     20                                   ,
                                     01                                   ,
                                     24                                   ,
                                     03                                   ,
                                     "new\n  \"text\"\n  f\tor\\mat\n  2" ));

    textedits.push_back(textedit);

    ASSERT_TRUE(buildTextEditObject( textedit                             ,
                                     errors                               ,
                                     30                                   ,
                                     01                                   ,
                                     34                                   ,
                                     03                                   ,
                                     "new\n  \"text\"\n  f\tor\\mat\n  3" ));

    textedits.push_back(textedit);

    ASSERT_TRUE(buildTextEditObject( textedit                             ,
                                     errors                               ,
                                     40                                   ,
                                     01                                   ,
                                     44                                   ,
                                     03                                   ,
                                     "new\n  \"text\"\n  f\tor\\mat\n  4" ));

    textedits.push_back(textedit);

     ASSERT_TRUE(buildTextEditObject( textedit                             ,
                                      errors                               ,
                                      50                                   ,
                                      01                                   ,
                                      54                                   ,
                                      03                                   ,
                                      "new\n  \"text\"\n  f\tor\\mat\n  5" ));

    textedits.push_back(textedit);

    ASSERT_TRUE(buildFormattingResponse( object     ,
                                         errors     ,
                                         request_id ,
                                         textedits  ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_result].is_array()          );
    ASSERT_EQ  ( object[m_result].size() , (size_t) 5 );

    ASSERT_TRUE( object[m_result][0].is_object()         );
    ASSERT_EQ  ( object[m_result][0].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][1].is_object()         );
    ASSERT_EQ  ( object[m_result][1].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][2].is_object()         );
    ASSERT_EQ  ( object[m_result][2].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][3].is_object()         );
    ASSERT_EQ  ( object[m_result][3].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4].is_object()         );
    ASSERT_EQ  ( object[m_result][4].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4][m_range].is_object()         );
    ASSERT_EQ  ( object[m_result][4][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4][m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_result][4][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4][m_range][m_start][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][4][m_range][m_start][m_line].to_int() , 50 );

    ASSERT_TRUE( object[m_result][4][m_range][m_start][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][4][m_range][m_start][m_character].to_int() , 01 );

    ASSERT_TRUE( object[m_result][4][m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_result][4][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][4][m_range][m_end][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][4][m_range][m_end][m_line].to_int() , 54 );

    ASSERT_TRUE( object[m_result][4][m_range][m_end][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][4][m_range][m_end][m_character].to_int() , 03 );

    ASSERT_TRUE( object[m_result][4][m_new_text].is_string()                                        );
    ASSERT_EQ  ( object[m_result][4][m_new_text].to_string() , "new\n  \"text\"\n  f\tor\\mat\n  5" );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 1165\r\n\r\n" << R"INPUT({
  "id" : 11
  ,"jsonrpc" : "2.0"
  ,"result" : [
    {
    "newText" : "new\n  \"text\"\n  f\tor\\mat\n  1"
    ,"range" : {
      "end" : {
      "character" : 3
      ,"line" : 14
    }
      ,"start" : {
        "character" : 1
        ,"line" : 10
      }
    }
  }
    ,{
    "newText" : "new\n  \"text\"\n  f\tor\\mat\n  2"
    ,"range" : {
      "end" : {
      "character" : 3
      ,"line" : 24
    }
      ,"start" : {
        "character" : 1
        ,"line" : 20
      }
    }
  }
    ,{
    "newText" : "new\n  \"text\"\n  f\tor\\mat\n  3"
    ,"range" : {
      "end" : {
      "character" : 3
      ,"line" : 34
    }
      ,"start" : {
        "character" : 1
        ,"line" : 30
      }
    }
  }
    ,{
    "newText" : "new\n  \"text\"\n  f\tor\\mat\n  4"
    ,"range" : {
      "end" : {
      "character" : 3
      ,"line" : 44
    }
      ,"start" : {
        "character" : 1
        ,"line" : 40
      }
    }
  }
    ,{
    "newText" : "new\n  \"text\"\n  f\tor\\mat\n  5"
    ,"range" : {
      "end" : {
      "character" : 3
      ,"line" : 54
    }
      ,"start" : {
        "character" : 1
        ,"line" : 50
      }
    }
  }
  ]
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_FALSE( tst_object[m_method].is_string() );

    int       tst_request_id;
    DataArray tst_textedits;

    ASSERT_TRUE(dissectFormattingResponse( object         ,
                                           errors         ,
                                           tst_request_id ,
                                           tst_textedits  ));

    ASSERT_EQ( tst_request_id       , tst_request_id    );
    ASSERT_EQ( tst_textedits.size() , (size_t) 5        );

    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    std::string tst_new_text;

    ASSERT_TRUE(dissectTextEditObject( *(tst_textedits[0].to_object()) ,
                                         errors                        ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ,
                                         tst_new_text                  ));

    ASSERT_TRUE(dissectTextEditObject( *(tst_textedits[1].to_object()) ,
                                         errors                        ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ,
                                         tst_new_text                  ));

    ASSERT_TRUE(dissectTextEditObject( *(tst_textedits[2].to_object()) ,
                                         errors                        ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ,
                                         tst_new_text                  ));

    ASSERT_TRUE(dissectTextEditObject( *(tst_textedits[3].to_object()) ,
                                         errors                        ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ,
                                         tst_new_text                  ));

    ASSERT_TRUE(dissectTextEditObject( *(tst_textedits[4].to_object()) ,
                                         errors                        ,
                                         tst_start_line                ,
                                         tst_start_character           ,
                                         tst_end_line                  ,
                                         tst_end_character             ,
                                         tst_new_text                  ));

    ASSERT_EQ( tst_start_line      , 50                                   );
    ASSERT_EQ( tst_start_character , 01                                   );
    ASSERT_EQ( tst_end_line        , 54                                   );
    ASSERT_EQ( tst_end_character   , 03                                   );
    ASSERT_EQ( tst_new_text        , "new\n  \"text\"\n  f\tor\\mat\n  5" );
}

TEST(lsp, documentsymbol_object)
{
    std::stringstream errors;
    DataObject        p0_object;

    std::string p0_name                      = "symbol_name_parent_0";
    std::string p0_detail                    = "symbol::detail::parent::0";
    int         p0_kind                      = 15;
    bool        p0_deprecated                = false;
    int         p0_start_line                = 10;
    int         p0_start_character           = 11;
    int         p0_end_line                  = 10;
    int         p0_end_character             = 17;
    int         p0_selection_start_line      = 10;
    int         p0_selection_start_character = 13;
    int         p0_selection_end_line        = 10;
    int         p0_selection_end_character   = 15;

    std::string c1_name                      = "symbol_name_child_1";
    std::string c1_detail                    = "symbol::detail::child::1";
    int         c1_kind                      = 20;
    bool        c1_deprecated                = false;
    int         c1_start_line                = 20;
    int         c1_start_character           = 21;
    int         c1_end_line                  = 20;
    int         c1_end_character             = 27;
    int         c1_selection_start_line      = 20;
    int         c1_selection_start_character = 23;
    int         c1_selection_end_line        = 20;
    int         c1_selection_end_character   = 25;

    std::string c2_name                      = "symbol_name_child_2";
    std::string c2_detail                    = "symbol::detail::child::2";
    int         c2_kind                      = 22;
    bool        c2_deprecated                = false;
    int         c2_start_line                = 30;
    int         c2_start_character           = 31;
    int         c2_end_line                  = 30;
    int         c2_end_character             = 37;
    int         c2_selection_start_line      = 30;
    int         c2_selection_start_character = 33;
    int         c2_selection_end_line        = 30;
    int         c2_selection_end_character   = 35;

    ASSERT_TRUE(buildDocumentSymbolObject( p0_object                    ,
                                           errors                       ,
                                           p0_name                      ,
                                           p0_detail                    ,
                                           p0_kind                      ,
                                           p0_deprecated                ,
                                           p0_start_line                ,
                                           p0_start_character           ,
                                           p0_end_line                  ,
                                           p0_end_character             ,
                                           p0_selection_start_line      ,
                                           p0_selection_start_character ,
                                           p0_selection_end_line        ,
                                           p0_selection_end_character   ));

    ASSERT_TRUE(buildDocumentSymbolObject( addDocumentSymbolChild( p0_object ) ,
                                           errors                              ,
                                           c1_name                             ,
                                           c1_detail                           ,
                                           c1_kind                             ,
                                           c1_deprecated                       ,
                                           c1_start_line                       ,
                                           c1_start_character                  ,
                                           c1_end_line                         ,
                                           c1_end_character                    ,
                                           c1_selection_start_line             ,
                                           c1_selection_start_character        ,
                                           c1_selection_end_line               ,
                                           c1_selection_end_character          ));

    ASSERT_TRUE(buildDocumentSymbolObject( addDocumentSymbolChild( p0_object ) ,
                                           errors                              ,
                                           c2_name                             ,
                                           c2_detail                           ,
                                           c2_kind                             ,
                                           c2_deprecated                       ,
                                           c2_start_line                       ,
                                           c2_start_character                  ,
                                           c2_end_line                         ,
                                           c2_end_character                    ,
                                           c2_selection_start_line             ,
                                           c2_selection_start_character        ,
                                           c2_selection_end_line               ,
                                           c2_selection_end_character          ));

    ASSERT_EQ  ( p0_object.size() , (size_t) 7 );

    ASSERT_TRUE( p0_object[m_name].is_string()           );
    ASSERT_EQ  ( p0_object[m_name].to_string() , p0_name );

    ASSERT_TRUE( p0_object[m_detail].is_string()             );
    ASSERT_EQ  ( p0_object[m_detail].to_string() , p0_detail );

    ASSERT_TRUE( p0_object[m_kind].is_int()           );
    ASSERT_EQ  ( p0_object[m_kind].to_int() , p0_kind );

    ASSERT_TRUE( p0_object[m_deprecated].is_bool()                 );
    ASSERT_EQ  ( p0_object[m_deprecated].to_bool() , p0_deprecated );

    ASSERT_TRUE( p0_object[m_range].is_object()         );
    ASSERT_EQ  ( p0_object[m_range].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_range][m_start].is_object()         );
    ASSERT_EQ  ( p0_object[m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_range][m_start][m_line].is_int()                 );
    ASSERT_EQ  ( p0_object[m_range][m_start][m_line].to_int() , p0_start_line );

    ASSERT_TRUE( p0_object[m_range][m_start][m_character].is_int()                      );
    ASSERT_EQ  ( p0_object[m_range][m_start][m_character].to_int() , p0_start_character );

    ASSERT_TRUE( p0_object[m_range][m_end].is_object()         );
    ASSERT_EQ  ( p0_object[m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_range][m_end][m_line].is_int()               );
    ASSERT_EQ  ( p0_object[m_range][m_end][m_line].to_int() , p0_end_line );

    ASSERT_TRUE( p0_object[m_range][m_end][m_character].is_int()                    );
    ASSERT_EQ  ( p0_object[m_range][m_end][m_character].to_int() , p0_end_character );

    ASSERT_TRUE( p0_object[m_selection_range].is_object()         );
    ASSERT_EQ  ( p0_object[m_selection_range].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_selection_range][m_start].is_object()         );
    ASSERT_EQ  ( p0_object[m_selection_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_selection_range][m_start][m_line].is_int()                           );
    ASSERT_EQ  ( p0_object[m_selection_range][m_start][m_line].to_int() , p0_selection_start_line );

    ASSERT_TRUE( p0_object[m_selection_range][m_start][m_character].is_int()                                );
    ASSERT_EQ  ( p0_object[m_selection_range][m_start][m_character].to_int() , p0_selection_start_character );

    ASSERT_TRUE( p0_object[m_selection_range][m_end].is_object()         );
    ASSERT_EQ  ( p0_object[m_selection_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_selection_range][m_end][m_line].is_int()                         );
    ASSERT_EQ  ( p0_object[m_selection_range][m_end][m_line].to_int() , p0_selection_end_line );

    ASSERT_TRUE( p0_object[m_selection_range][m_end][m_character].is_int()                              );
    ASSERT_EQ  ( p0_object[m_selection_range][m_end][m_character].to_int() , p0_selection_end_character );

    ASSERT_TRUE( p0_object[m_children].is_array()          );
    ASSERT_EQ  ( p0_object[m_children].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][0].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][0].size() , (size_t) 7 );

    ASSERT_TRUE( p0_object[m_children][0][m_name].is_string()           );
    ASSERT_EQ  ( p0_object[m_children][0][m_name].to_string() , c1_name );

    ASSERT_TRUE( p0_object[m_children][0][m_detail].is_string()             );
    ASSERT_EQ  ( p0_object[m_children][0][m_detail].to_string() , c1_detail );

    ASSERT_TRUE( p0_object[m_children][0][m_kind].is_int()           );
    ASSERT_EQ  ( p0_object[m_children][0][m_kind].to_int() , c1_kind );

    ASSERT_TRUE( p0_object[m_children][0][m_deprecated].is_bool()                 );
    ASSERT_EQ  ( p0_object[m_children][0][m_deprecated].to_bool() , c1_deprecated );

    ASSERT_TRUE( p0_object[m_children][0][m_range].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][0][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][0][m_range][m_start].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][0][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][0][m_range][m_start][m_line].is_int()                 );
    ASSERT_EQ  ( p0_object[m_children][0][m_range][m_start][m_line].to_int() , c1_start_line );

    ASSERT_TRUE( p0_object[m_children][0][m_range][m_start][m_character].is_int()                      );
    ASSERT_EQ  ( p0_object[m_children][0][m_range][m_start][m_character].to_int() , c1_start_character );

    ASSERT_TRUE( p0_object[m_children][0][m_range][m_end].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][0][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][0][m_range][m_end][m_line].is_int()               );
    ASSERT_EQ  ( p0_object[m_children][0][m_range][m_end][m_line].to_int() , c1_end_line );

    ASSERT_TRUE( p0_object[m_children][0][m_range][m_end][m_character].is_int()                    );
    ASSERT_EQ  ( p0_object[m_children][0][m_range][m_end][m_character].to_int() , c1_end_character );

    ASSERT_TRUE( p0_object[m_children][0][m_selection_range].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][0][m_selection_range].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][0][m_selection_range][m_start].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][0][m_selection_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][0][m_selection_range][m_start][m_line].is_int()                           );
    ASSERT_EQ  ( p0_object[m_children][0][m_selection_range][m_start][m_line].to_int() , c1_selection_start_line );

    ASSERT_TRUE( p0_object[m_children][0][m_selection_range][m_start][m_character].is_int()                                );
    ASSERT_EQ  ( p0_object[m_children][0][m_selection_range][m_start][m_character].to_int() , c1_selection_start_character );

    ASSERT_TRUE( p0_object[m_children][0][m_selection_range][m_end].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][0][m_selection_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][0][m_selection_range][m_end][m_line].is_int()                         );
    ASSERT_EQ  ( p0_object[m_children][0][m_selection_range][m_end][m_line].to_int() , c1_selection_end_line );

    ASSERT_TRUE( p0_object[m_children][0][m_selection_range][m_end][m_character].is_int()                              );
    ASSERT_EQ  ( p0_object[m_children][0][m_selection_range][m_end][m_character].to_int() , c1_selection_end_character );

    ASSERT_TRUE( p0_object[m_children][1].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][1].size() , (size_t) 7 );

    ASSERT_TRUE( p0_object[m_children][1][m_name].is_string()           );
    ASSERT_EQ  ( p0_object[m_children][1][m_name].to_string() , c2_name );

    ASSERT_TRUE( p0_object[m_children][1][m_detail].is_string()             );
    ASSERT_EQ  ( p0_object[m_children][1][m_detail].to_string() , c2_detail );

    ASSERT_TRUE( p0_object[m_children][1][m_kind].is_int()           );
    ASSERT_EQ  ( p0_object[m_children][1][m_kind].to_int() , c2_kind );

    ASSERT_TRUE( p0_object[m_children][1][m_deprecated].is_bool()                 );
    ASSERT_EQ  ( p0_object[m_children][1][m_deprecated].to_bool() , c2_deprecated );

    ASSERT_TRUE( p0_object[m_children][1][m_range].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][1][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][1][m_range][m_start].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][1][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][1][m_range][m_start][m_line].is_int()                 );
    ASSERT_EQ  ( p0_object[m_children][1][m_range][m_start][m_line].to_int() , c2_start_line );

    ASSERT_TRUE( p0_object[m_children][1][m_range][m_start][m_character].is_int()                      );
    ASSERT_EQ  ( p0_object[m_children][1][m_range][m_start][m_character].to_int() , c2_start_character );

    ASSERT_TRUE( p0_object[m_children][1][m_range][m_end].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][1][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][1][m_range][m_end][m_line].is_int()               );
    ASSERT_EQ  ( p0_object[m_children][1][m_range][m_end][m_line].to_int() , c2_end_line );

    ASSERT_TRUE( p0_object[m_children][1][m_range][m_end][m_character].is_int()                    );
    ASSERT_EQ  ( p0_object[m_children][1][m_range][m_end][m_character].to_int() , c2_end_character );

    ASSERT_TRUE( p0_object[m_children][1][m_selection_range].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][1][m_selection_range].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][1][m_selection_range][m_start].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][1][m_selection_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][1][m_selection_range][m_start][m_line].is_int()                           );
    ASSERT_EQ  ( p0_object[m_children][1][m_selection_range][m_start][m_line].to_int() , c2_selection_start_line );

    ASSERT_TRUE( p0_object[m_children][1][m_selection_range][m_start][m_character].is_int()                                );
    ASSERT_EQ  ( p0_object[m_children][1][m_selection_range][m_start][m_character].to_int() , c2_selection_start_character );

    ASSERT_TRUE( p0_object[m_children][1][m_selection_range][m_end].is_object()         );
    ASSERT_EQ  ( p0_object[m_children][1][m_selection_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( p0_object[m_children][1][m_selection_range][m_end][m_line].is_int()                         );
    ASSERT_EQ  ( p0_object[m_children][1][m_selection_range][m_end][m_line].to_int() , c2_selection_end_line );

    ASSERT_TRUE( p0_object[m_children][1][m_selection_range][m_end][m_character].is_int()                              );
    ASSERT_EQ  ( p0_object[m_children][1][m_selection_range][m_end][m_character].to_int() , c2_selection_end_character );

    std::stringstream json;
    p0_object.format_json(json);

    std::stringstream json_expected;
    json_expected << R"INPUT({
  "children" : [
  {
  "children" : []
  ,"deprecated" : false
  ,"detail" : "symbol::detail::child::1"
  ,"kind" : 20
  ,"name" : "symbol_name_child_1"
  ,"range" : {
    "end" : {
    "character" : 27
    ,"line" : 20
  }
    ,"start" : {
      "character" : 21
      ,"line" : 20
    }
  }
  ,"selectionRange" : {
    "end" : {
    "character" : 25
    ,"line" : 20
  }
    ,"start" : {
      "character" : 23
      ,"line" : 20
    }
  }
}
  ,{
  "children" : []
  ,"deprecated" : false
  ,"detail" : "symbol::detail::child::2"
  ,"kind" : 22
  ,"name" : "symbol_name_child_2"
  ,"range" : {
    "end" : {
    "character" : 37
    ,"line" : 30
  }
    ,"start" : {
      "character" : 31
      ,"line" : 30
    }
  }
  ,"selectionRange" : {
    "end" : {
    "character" : 35
    ,"line" : 30
  }
    ,"start" : {
      "character" : 33
      ,"line" : 30
    }
  }
}
]
  ,"deprecated" : false
  ,"detail" : "symbol::detail::parent::0"
  ,"kind" : 15
  ,"name" : "symbol_name_parent_0"
  ,"range" : {
    "end" : {
    "character" : 17
    ,"line" : 10
  }
    ,"start" : {
      "character" : 11
      ,"line" : 10
    }
  }
  ,"selectionRange" : {
    "end" : {
    "character" : 15
    ,"line" : 10
  }
    ,"start" : {
      "character" : 13
      ,"line" : 10
    }
  }
})INPUT";

    ASSERT_EQ( json.str() , json_expected.str() );

    std::string tst_p0_name;
    std::string tst_p0_detail;
    int         tst_p0_kind;
    bool        tst_p0_deprecated;
    int         tst_p0_start_line;
    int         tst_p0_start_character;
    int         tst_p0_end_line;
    int         tst_p0_end_character;
    int         tst_p0_selection_start_line;
    int         tst_p0_selection_start_character;
    int         tst_p0_selection_end_line;
    int         tst_p0_selection_end_character;

    std::string tst_c1_name;
    std::string tst_c1_detail;
    int         tst_c1_kind;
    bool        tst_c1_deprecated;
    int         tst_c1_start_line;
    int         tst_c1_start_character;
    int         tst_c1_end_line;
    int         tst_c1_end_character;
    int         tst_c1_selection_start_line;
    int         tst_c1_selection_start_character;
    int         tst_c1_selection_end_line;
    int         tst_c1_selection_end_character;

    std::string tst_c2_name;
    std::string tst_c2_detail;
    int         tst_c2_kind;
    bool        tst_c2_deprecated;
    int         tst_c2_start_line;
    int         tst_c2_start_character;
    int         tst_c2_end_line;
    int         tst_c2_end_character;
    int         tst_c2_selection_start_line;
    int         tst_c2_selection_start_character;
    int         tst_c2_selection_end_line;
    int         tst_c2_selection_end_character;

    ASSERT_TRUE(dissectDocumentSymbolObject( p0_object                        ,
                                             errors                           ,
                                             tst_p0_name                      ,
                                             tst_p0_detail                    ,
                                             tst_p0_kind                      ,
                                             tst_p0_deprecated                ,
                                             tst_p0_start_line                ,
                                             tst_p0_start_character           ,
                                             tst_p0_end_line                  ,
                                             tst_p0_end_character             ,
                                             tst_p0_selection_start_line      ,
                                             tst_p0_selection_start_character ,
                                             tst_p0_selection_end_line        ,
                                             tst_p0_selection_end_character   ));

    ASSERT_EQ ( tst_p0_name                      , p0_name                      );
    ASSERT_EQ ( tst_p0_detail                    , p0_detail                    );
    ASSERT_EQ ( tst_p0_kind                      , p0_kind                      );
    ASSERT_EQ ( tst_p0_deprecated                , p0_deprecated                );
    ASSERT_EQ ( tst_p0_start_line                , p0_start_line                );
    ASSERT_EQ ( tst_p0_start_character           , p0_start_character           );
    ASSERT_EQ ( tst_p0_end_line                  , p0_end_line                  );
    ASSERT_EQ ( tst_p0_end_character             , p0_end_character             );
    ASSERT_EQ ( tst_p0_selection_start_line      , p0_selection_start_line      );
    ASSERT_EQ ( tst_p0_selection_start_character , p0_selection_start_character );
    ASSERT_EQ ( tst_p0_selection_end_line        , p0_selection_end_line        );
    ASSERT_EQ ( tst_p0_selection_end_character   , p0_selection_end_character   );

    ASSERT_EQ ( getDocumentSymbolChildren( p0_object )->size() , (size_t) 2 );

    const DataObject & child_object_1 =
                *(getDocumentSymbolChildren( p0_object )->at(0).to_object());

    ASSERT_TRUE(dissectDocumentSymbolObject( child_object_1                   ,
                                             errors                           ,
                                             tst_c1_name                      ,
                                             tst_c1_detail                    ,
                                             tst_c1_kind                      ,
                                             tst_c1_deprecated                ,
                                             tst_c1_start_line                ,
                                             tst_c1_start_character           ,
                                             tst_c1_end_line                  ,
                                             tst_c1_end_character             ,
                                             tst_c1_selection_start_line      ,
                                             tst_c1_selection_start_character ,
                                             tst_c1_selection_end_line        ,
                                             tst_c1_selection_end_character   ));

    ASSERT_EQ ( tst_c1_name                      , c1_name                      );
    ASSERT_EQ ( tst_c1_detail                    , c1_detail                    );
    ASSERT_EQ ( tst_c1_kind                      , c1_kind                      );
    ASSERT_EQ ( tst_c1_deprecated                , c1_deprecated                );
    ASSERT_EQ ( tst_c1_start_line                , c1_start_line                );
    ASSERT_EQ ( tst_c1_start_character           , c1_start_character           );
    ASSERT_EQ ( tst_c1_end_line                  , c1_end_line                  );
    ASSERT_EQ ( tst_c1_end_character             , c1_end_character             );
    ASSERT_EQ ( tst_c1_selection_start_line      , c1_selection_start_line      );
    ASSERT_EQ ( tst_c1_selection_start_character , c1_selection_start_character );
    ASSERT_EQ ( tst_c1_selection_end_line        , c1_selection_end_line        );
    ASSERT_EQ ( tst_c1_selection_end_character   , c1_selection_end_character   );

    const DataObject & child_object_2 =
                *(getDocumentSymbolChildren( p0_object )->at(1).to_object());

    ASSERT_TRUE(dissectDocumentSymbolObject( child_object_2                   ,
                                             errors                           ,
                                             tst_c2_name                      ,
                                             tst_c2_detail                    ,
                                             tst_c2_kind                      ,
                                             tst_c2_deprecated                ,
                                             tst_c2_start_line                ,
                                             tst_c2_start_character           ,
                                             tst_c2_end_line                  ,
                                             tst_c2_end_character             ,
                                             tst_c2_selection_start_line      ,
                                             tst_c2_selection_start_character ,
                                             tst_c2_selection_end_line        ,
                                             tst_c2_selection_end_character   ));

    ASSERT_EQ ( tst_c2_name                      , c2_name                      );
    ASSERT_EQ ( tst_c2_detail                    , c2_detail                    );
    ASSERT_EQ ( tst_c2_kind                      , c2_kind                      );
    ASSERT_EQ ( tst_c2_deprecated                , c2_deprecated                );
    ASSERT_EQ ( tst_c2_start_line                , c2_start_line                );
    ASSERT_EQ ( tst_c2_start_character           , c2_start_character           );
    ASSERT_EQ ( tst_c2_end_line                  , c2_end_line                  );
    ASSERT_EQ ( tst_c2_end_character             , c2_end_character             );
    ASSERT_EQ ( tst_c2_selection_start_line      , c2_selection_start_line      );
    ASSERT_EQ ( tst_c2_selection_start_character , c2_selection_start_character );
    ASSERT_EQ ( tst_c2_selection_end_line        , c2_selection_end_line        );
    ASSERT_EQ ( tst_c2_selection_end_character   , c2_selection_end_character   );
}

TEST(lsp, symbols_response)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id = 12;
    DataObject        document_symbol;
    DataArray         document_symbols;

    ASSERT_TRUE(buildDocumentSymbolObject( document_symbol            ,
                                           errors                     ,
                                           "document_symbol_name"     ,
                                           "document::symbol::detail" ,
                                           13                         ,
                                           true                       ,
                                           30                         ,
                                           15                         ,
                                           60                         ,
                                           45                         ,
                                           40                         ,
                                           25                         ,
                                           50                         ,
                                           35                         ));

    document_symbols.push_back(document_symbol);

    ASSERT_TRUE(buildSymbolsResponse( object           ,
                                      errors           ,
                                      request_id       ,
                                      document_symbols ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_result].is_array()          );
    ASSERT_EQ  ( object[m_result].size() , (size_t) 1 );

    ASSERT_TRUE( object[m_result][0].is_object()         );
    ASSERT_EQ  ( object[m_result][0].size() , (size_t) 7 );

    ASSERT_TRUE( object[m_result][0][m_name].is_string()                          );
    ASSERT_EQ  ( object[m_result][0][m_name].to_string() , "document_symbol_name" );

    ASSERT_TRUE( object[m_result][0][m_detail].is_string()                              );
    ASSERT_EQ  ( object[m_result][0][m_detail].to_string() , "document::symbol::detail" );

    ASSERT_TRUE( object[m_result][0][m_kind].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_kind].to_int() , 13 );

    ASSERT_TRUE( object[m_result][0][m_deprecated].is_bool()        );
    ASSERT_EQ  ( object[m_result][0][m_deprecated].to_bool() , true );

    ASSERT_TRUE( object[m_result][0][m_range].is_object()         );
    ASSERT_EQ  ( object[m_result][0][m_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][0][m_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_result][0][m_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][0][m_range][m_start][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_range][m_start][m_line].to_int() , 30 );

    ASSERT_TRUE( object[m_result][0][m_range][m_start][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_range][m_start][m_character].to_int() , 15 );

    ASSERT_TRUE( object[m_result][0][m_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_result][0][m_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][0][m_range][m_end][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_range][m_end][m_line].to_int() , 60 );

    ASSERT_TRUE( object[m_result][0][m_range][m_end][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_range][m_end][m_character].to_int() , 45 );

    ASSERT_TRUE( object[m_result][0][m_selection_range].is_object()         );
    ASSERT_EQ  ( object[m_result][0][m_selection_range].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][0][m_selection_range][m_start].is_object()         );
    ASSERT_EQ  ( object[m_result][0][m_selection_range][m_start].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][0][m_selection_range][m_start][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_selection_range][m_start][m_line].to_int() , 40 );

    ASSERT_TRUE( object[m_result][0][m_selection_range][m_start][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_selection_range][m_start][m_character].to_int() , 25 );

    ASSERT_TRUE( object[m_result][0][m_selection_range][m_end].is_object()         );
    ASSERT_EQ  ( object[m_result][0][m_selection_range][m_end].size() , (size_t) 2 );

    ASSERT_TRUE( object[m_result][0][m_selection_range][m_end][m_line].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_selection_range][m_end][m_line].to_int() , 50 );

    ASSERT_TRUE( object[m_result][0][m_selection_range][m_end][m_character].is_int()      );
    ASSERT_EQ  ( object[m_result][0][m_selection_range][m_end][m_character].to_int() , 35 );

    ASSERT_TRUE( object[m_result][0][m_children].is_array()          );
    ASSERT_EQ  ( object[m_result][0][m_children].size() , (size_t) 0 );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 537\r\n\r\n" << R"INPUT({
  "id" : 12
  ,"jsonrpc" : "2.0"
  ,"result" : [
    {
    "children" : []
    ,"deprecated" : true
    ,"detail" : "document::symbol::detail"
    ,"kind" : 13
    ,"name" : "document_symbol_name"
    ,"range" : {
      "end" : {
      "character" : 45
      ,"line" : 60
    }
      ,"start" : {
        "character" : 15
        ,"line" : 30
      }
    }
    ,"selectionRange" : {
      "end" : {
      "character" : 35
      ,"line" : 50
    }
      ,"start" : {
        "character" : 25
        ,"line" : 40
      }
    }
  }
  ]
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_FALSE( tst_object[m_method].is_string() );

    int       tst_request_id;
    DataArray tst_document_symbols;

    ASSERT_TRUE(dissectSymbolsResponse( object                ,
                                        errors                ,
                                        tst_request_id        ,
                                        tst_document_symbols  ));

    ASSERT_EQ( tst_request_id              , tst_request_id    );
    ASSERT_EQ( tst_document_symbols.size() , (size_t) 1        );

    std::string tst_name;
    std::string tst_detail;
    int         tst_kind;
    bool        tst_deprecated;
    int         tst_start_line;
    int         tst_start_character;
    int         tst_end_line;
    int         tst_end_character;
    int         tst_selection_start_line;
    int         tst_selection_start_character;
    int         tst_selection_end_line;
    int         tst_selection_end_character;

    ASSERT_TRUE(dissectDocumentSymbolObject( *(tst_document_symbols[0].to_object()) ,
                                             errors                                 ,
                                             tst_name                               ,
                                             tst_detail                             ,
                                             tst_kind                               ,
                                             tst_deprecated                         ,
                                             tst_start_line                         ,
                                             tst_start_character                    ,
                                             tst_end_line                           ,
                                             tst_end_character                      ,
                                             tst_selection_start_line               ,
                                             tst_selection_start_character          ,
                                             tst_selection_end_line                 ,
                                             tst_selection_end_character            ));

    ASSERT_EQ( tst_name                      , "document_symbol_name"     );
    ASSERT_EQ( tst_detail                    , "document::symbol::detail" );
    ASSERT_EQ( tst_kind                      , 13                         );
    ASSERT_EQ( tst_deprecated                , true                       );
    ASSERT_EQ( tst_start_line                , 30                         );
    ASSERT_EQ( tst_start_character           , 15                         );
    ASSERT_EQ( tst_end_line                  , 60                         );
    ASSERT_EQ( tst_end_character             , 45                         );
    ASSERT_EQ( tst_selection_start_line      , 40                         );
    ASSERT_EQ( tst_selection_start_character , 25                         );
    ASSERT_EQ( tst_selection_end_line        , 50                         );
    ASSERT_EQ( tst_selection_end_character   , 35                         );
}

TEST(lsp, shutdown_response)
{
    DataObject        object;
    std::stringstream errors;

    int               request_id = 13;

    ASSERT_TRUE(buildShutdownResponse( object     ,
                                       errors     ,
                                       request_id ));

    ASSERT_EQ  ( object.size() , (size_t) 2 );

    ASSERT_TRUE( object[m_id].is_int()              );
    ASSERT_EQ  ( object[m_id].to_int() , request_id );

    ASSERT_TRUE( object[m_result].is_object()         );
    ASSERT_EQ  ( object[m_result].size() , (size_t) 0 );

    std::string rpcstr;
    ASSERT_TRUE( objectToRPCString( object ,
                                    rpcstr ,
                                    errors ));

    std::stringstream rpc_expected;
    rpc_expected << "Content-Length: 53\r\n\r\n" << R"INPUT({
  "id" : 13
  ,"jsonrpc" : "2.0"
  ,"result" : {}
})INPUT";

    ASSERT_EQ( rpcstr , rpc_expected.str() );

    DataObject tst_object;
    ASSERT_TRUE( RPCStringToObject( rpcstr     ,
                                    tst_object ,
                                    errors     ));

    ASSERT_FALSE( tst_object[m_method].is_string() );

    int tst_request_id;

    ASSERT_TRUE(dissectShutdownResponse( object         ,
                                         errors         ,
                                         tst_request_id ));

    ASSERT_EQ( tst_request_id , request_id );
}

} // namespace lsp
} // namespace wasp
