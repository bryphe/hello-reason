Console.log("Running Test Program:");
let () = print_endline("Not dynlinked yet!");

// Call into test plugin with the default implementation

let defaultInstance = Lib.TestPlugin.get();
prerr_endline (defaultInstance());

let onChange = (newInstance) => prerr_endline ("Got a new instance: " ++ newInstance());

Lib.TestPlugin.watch(onChange, Sys.getcwd() ++ "/_build/install/default/bin/dynamic_lib.cmxs");
