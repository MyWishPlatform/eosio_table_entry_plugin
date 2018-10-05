file(GLOB HEADERS "include/eosio/table_entry_plugin/*.hpp")
add_library( table_entry_plugin
             table_entry_plugin.cpp
             ${HEADERS} )

target_link_libraries( table_entry_plugin chain_plugin http_plugin appbase fc )
target_include_directories( table_entry_plugin PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
