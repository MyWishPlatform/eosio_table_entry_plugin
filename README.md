Clone repo:
```bash
eos/plugins]$ git clone https://github.com/MyWishPlatform/eosio_table_entry_plugin/
eos/plugins]$ mv eosio_table_entry_plugin table_entry_plugin
```

<br />

Modify eos/plugins/CMakeLists.txt:
```
...
add_subdirectory(table_entry_plugin)
...
```

<br />

Modify eos/programs/nodeos/CMakeLists.txt:
```
...
PRIVATE -Wl,${whole_archive_flag} table_entry_plugin  -Wl,${no_whole_archive_flag}
...
```

<br />

Compile:
```bash
eos/build]$ make
eos/build]$ sudo make install
```

<br />

Add to config.ini:
```
plugin = eosio::table_entry_plugin
```
