// Let's use our Plugin.Make functor to make a plugin!

// This is the definition of our plugin, which defines the type [t] (in our case, it's a function that returns a string),
// as well as a default implementation.
module PluginDefinition = {
  // The type is a unit that returns a string
  type t = unit => string;

  let default = () => "Default implementation, not dynlinked!";
};

// Use our functor to create some modules - both the [dynlib] and the [app] have access to this module.
// The [dynlib] will use TestPlugin.Instance.register to notify that it's loaded, and the application ([bin])
// will use [get] and [watch] to get the latest implementation.
include Plugin.Make(PluginDefinition);
