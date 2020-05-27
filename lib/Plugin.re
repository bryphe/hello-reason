
// Our plugin can specify a type t, and a default implementation.
module type Sig = {
	type t;

	let default: t;
};

module Make = (Sig: Sig) => {
	
	// Tuck the current 'instance' - the latest plugin we know about - in a mutable value.
	module Mutable = {
		 let instance: ref(Sig.t) = ref(Sig.default);
	}

	// API to be called from the dynlinked module upon registration. Both the main app
	// and the plugin have access to this module.
	module Instance = {
		let register = (pluginInstance: Sig.t) => Mutable.instance := pluginInstance;
	};

	let watch = (onChange: Sig.t => unit, path: string) => {
		Dynlink.loadfile_private(path);
		// TODO: File watching
		// Whenever we see the 'path' change, we should call 'onChange'.
		onChange(Mutable.instance^);

		// We don't have file watching now... but let's pretend by just calling this every few seconds
		// We shouldn't actually need threads for watching - we can just use libuvs file watcher.
		let thread = Thread.create(() => {
			for (i in 0 to 10) {
				Unix.sleepf(2.0);
				Dynlink.loadfile_private(path);
				onChange(Mutable.instance^);
			}
		}, ());

		Thread.join(thread);
	};

	// Just get the latest value. If [watch] hasn't been called, this will always be the default.
	let get = () => Mutable.instance^;
};
