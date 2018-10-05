#include <eosio/table_entry_plugin/table_entry_plugin.hpp>

namespace eosio {
	static appbase::abstract_plugin& _table_entry_plugin = app().register_plugin<table_entry_plugin>();

	class table_entry_plugin_impl {
	};

	table_entry_plugin::table_entry_plugin() {}
	table_entry_plugin::~table_entry_plugin() {}

	void table_entry_plugin::set_program_options(options_description&, options_description& cfg) {}

	void table_entry_plugin::plugin_initialize(const variables_map& options) {}

	void table_entry_plugin::plugin_startup() {
		ilog("starting table_entry_plugin");
		auto& chain_plugin_ref = app().get_plugin<chain_plugin>();
		auto& db = chain_plugin_ref.chain().db();
		auto time = chain_plugin_ref.get_abi_serializer_max_time();
		app().get_plugin<http_plugin>().add_api({
			{"/v1/chain-ext/get_table_entry", [&](std::string url, std::string body, url_response_callback callback) {
				try {
					if (body.empty()) body = "{}";
					std::vector<fc::variant> results;
					fc::variant var = fc::json::from_string(body).get_object();
					eosio::name code(var["code"].as_string());
					eosio::name scope(var["scope"].as_string());
					eosio::name table(var["table"].as_string());
					uint64_t key = var["key"].as_uint64();

					abi_def abi;
					const eosio::chain::account_object *code_accnt = db.find<eosio::chain::account_object, eosio::chain::by_name>(code);
					EOS_ASSERT(code_accnt != nullptr, chain::account_query_exception, "Fail to retrieve account for ${account}", ("account", code));
					abi_serializer::to_abi(code_accnt->abi, abi);
					abi_serializer abis;
					abis.set_abi(abi, time);

					const auto* tab = db.find<chain::table_id_object, chain::by_code_scope_table>(boost::make_tuple(code, scope, table));
					EOS_ASSERT(tab != nullptr, chain::database_exception, "Table not found");

					const chain::key_value_object* obj = db.find<chain::key_value_object, chain::by_scope_primary>(boost::make_tuple(tab->id, key));
					EOS_ASSERT(obj != nullptr, chain::database_exception, "Entry not found");

					std::vector<char> data;
					chain_plugin_ref.get_read_only_api().copy_inline_row(*obj, data);

					auto variant = abis.binary_to_variant(abis.get_table_type(table), data, time);
					callback(200, fc::json::to_string(variant));
				} catch (...) {
					http_plugin::handle_exception("chain-ext", "get_table_entry", body, callback);
				}
			}}
		});
	}

	void table_entry_plugin::plugin_shutdown() {}
}
