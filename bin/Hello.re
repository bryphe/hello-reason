Console.log("Running Test Program:");
let () = print_endline("Not dynlinked yet!");

Dynlink.loadfile(Sys.getcwd() ++ "/_build/install/default/bin/lib.cmxs");
