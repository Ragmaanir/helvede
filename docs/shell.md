
cd
ls
kill
cat
grep
rm
mkdir
mv
ln

```
Terminal.Environment.extend do
  def cd := Terminal.workdir=(*)
  def list := System.Dir(".").list
  def ls = list(format: "human", *)
  def signal := System.Process.signal
  def read := System.File.read
  def select/pick/filter
  def delete := System.File.delete(:trash, *)
  def trash := System.File.delete(:trash, *)
  def shred := System.File.delete(:shred, *)
  def undo := Terminal.CommandHistory.last.prompt_undo
end
```

Are those functions/methods or executables?
What about non-builtin executables like git, ssh?
- Those could provide their own command scripts
- A command registry could provide automatic namespacing and collision detection

- autocompletion for paths
- autocompletion / documentation for parameters
- syntax highlighting
- favorite commands (gather usage stats, favorite parameters)
- have named environments with standard aliases for scripts (like build scripts etc)
- multiple named screens by default (+ tabs)
- directory completion / directory history/favorites
- multiline prompt on demand / long command (shift+enter to start, shift+enter to submit)
- prompt-placeholder
- progress indicator (command has to support that)
- output format: table/(j/c)son/syaml; Stream: ?
- everything is in color, but colors can easily be ignored when output piped (color-channel)
- prioritized history: score calculated by frequency/recency/age/success
- commands do suggestions/autocompletion by themselves
- atomic/transactional commands
- globally log failed and completed commands + time + user
- restrictive filenames (?) only /\p{Alnum}0-9_-./
- verbose mode for commands
- should commands behave differently when run from a script or interactively?
- do not execute/log commands with syntax error
- do not execute/log missing commands
- syntax highlight command red when it does not exist, yellow when there is a command with that prefix, pink if you don't have necessary permissions
- notify user automatically (graphically+sound) when long running command finished or failed
- show an indicator that displays wether a command is currently running and wether it displayed new output the user didn't see yet
- highlight output the user didn't see yet
- collapse long output
- _ (or $ ?) is placeholder for last command result
- search for commands via tags/keywords and maybe categorize them
- filename start symbol unicode (?)
- select, copy, paste


Terminal
-----------

[√] ~/downloads: rm "~/downloads/test.txt"


Use Cases
--------------

1. sort files by size
2. get path/name of biggest file in directory
3. show directory tree to level N with max M entries
4. navigate in directory tree with keyboard without needing to type pathnames

--------------

3.
```
~: Dir.tree(depth: 2, max: 3)
.
├─ app
|  ├─ models
|  └─ views
├─ config
├─ logs
├─ ...
```
