#pragma once
#include <appbase/application.hpp>
#include <eosio/http_plugin/http_plugin.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>
#include <fc/io/json.hpp>

namespace eosio {
	using namespace appbase;

	class table_entry_plugin : public appbase::plugin<table_entry_plugin> {
	public:
		table_entry_plugin();
		virtual ~table_entry_plugin();
	 
		APPBASE_PLUGIN_REQUIRES()
		virtual void set_program_options(options_description&, options_description& cfg) override;
	 
		void plugin_initialize(const variables_map& options);
		void plugin_startup();
		void plugin_shutdown();

	private:
		class table_entry_plugin_impl* my;
	};
}
