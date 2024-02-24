/// Deno 1.28+ script that generates a "smgf_key_names" array from SDL
/// "SDL_scancode_names" to use in smgf. Works with SDL_keyboard.c file from
/// SDL 2.26.1.

if (Deno.args.length !== 1) {
  throw new Error(
    "usage: deno run --allow-run --allow-read --allow-write get_keyboard_array.ts path/to/SDL_keyboard.c"
  );
}

const sdlKeyboardFilePath = Deno.args[0];
const sdlKeyboardFile = await Deno.readFile(sdlKeyboardFilePath);
const decoder = new TextDecoder("utf-8");
const fileContents = decoder.decode(sdlKeyboardFile);

const ARRAY_START = "static const char *SDL_scancode_names[SDL_NUM_SCANCODES]";
const ARRAY_END = "};";

const startIndex = fileContents.indexOf(ARRAY_START);
const endIndex = fileContents.indexOf(ARRAY_END, startIndex);

if (startIndex === -1 || endIndex === -1) {
  throw new Error("Invalid input file");
}

const outputStr = fileContents
  .substring(startIndex, endIndex)
  .toLowerCase()
  .replaceAll(/\/\* \d+ \*\//g, "")
  .replaceAll("\n", "")
  .replaceAll("     ", " ")
  .replaceAll(
    ARRAY_START.toLocaleLowerCase(),
    "static const char* smgf_key_names[]"
  )
  .replaceAll("null", "NULL");

console.log(outputStr + ARRAY_END);
