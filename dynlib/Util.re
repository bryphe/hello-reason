let timeStr = string_of_float(Unix.gettimeofday());

// Call our functorized TestPlugin.Instance.register to let everyone know we're loaded!
Lib.TestPlugin.Instance.register(() =>
  "I'm dynamically linked, loaded at time: " ++ timeStr
);
