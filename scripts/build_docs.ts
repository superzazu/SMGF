/// Deno 1.40+ script that generates an "api.md" file in Markdown format
/// from lua-language-server definitions (see docs-api folder)
/// usage: deno run --allow-run --allow-read --allow-write scripts/build_docs.ts
import { join } from "https://deno.land/std@0.216.0/path/mod.ts";

const cwd = Deno.cwd();
const DOC_DIR_PATH = join(cwd, "docs-api");
const CONFIG_PATH = join(cwd, "scripts/llsConf.json");
const OUTPUT_PATH = join(cwd, "website/docs/api.md");
const tempDirPath = await Deno.makeTempDir();

console.log(`Temporary dir path: ${tempDirPath}`);
console.log(`Docs dir path: ${DOC_DIR_PATH}`);
console.log(`Config path: ${CONFIG_PATH}`);
console.log(`Output path: ${OUTPUT_PATH}`);

// ensure config file exists
try {
  await Deno.lstat(CONFIG_PATH);
} catch (_e) {
  throw new Error(`Cannot find configuration file "llsConf.lua"`);
}

// getting "doc.json" file from "lua-language-server"
const command = new Deno.Command("lua-language-server", {
  args: [
    `--configpath="${CONFIG_PATH}"`,
    `--logpath="${tempDirPath}"`,
    `--doc="${DOC_DIR_PATH}"`,
    // `--loglevel=trace`,
  ],
});
const { code } = await command.output();
if (code !== 0) {
  throw new Error(`while running lua-language-server: ${code}`);
}

const docJsonPath = join(tempDirPath, "doc.json");
const decoder = new TextDecoder("utf-8");
const docJsonData = await Deno.readFile(docJsonPath);
// deno-lint-ignore no-explicit-any
const docJson: Record<string, any>[] = JSON.parse(decoder.decode(docJsonData));

// formating doc.json into markdown
// deno-lint-ignore no-explicit-any
const isMainModule = (node: any): boolean => node["name"] === "smgf";

docJson.sort(
  (a, b) =>
    // we want the "main" module (the one will all callbacks) first:
    (isMainModule(b) ? 1 : 0) - (isMainModule(a) ? 1 : 0) ||
    a.name.localeCompare(b.name)
);

/** Regex to replace all Markdown links in file */
const linksRe = /\[([^\[\]]*)\]\((.*?)\)/gm;
const moduleRegex = /^smgf\.(\w*)$/;
const functionRegex = /^smgf\.(\w*)\.(\w*)$/;

let markdownContent = "";

markdownContent += "---\n";
markdownContent += "title: API reference\n";
markdownContent += "sidebar_position: 5\n";
markdownContent += "---\n\n";

for (const jsonClass of docJson) {
  // ignore empty modules defines "smgf.audio"
  if (moduleRegex.test(jsonClass["name"]) && jsonClass["type"] === "variable") {
    continue;
  }

  // ignore complete function definitions
  if (functionRegex.test(jsonClass["name"])) {
    continue;
  }

  // ignore empty "smgf" module
  if (jsonClass["name"] === "smgf" && jsonClass["type"] === "type") {
    continue;
  }

  markdownContent += `## ${jsonClass["name"]}\n`;
  markdownContent += "\n";
  if ("desc" in jsonClass) {
    markdownContent += jsonClass["desc"].trim();
    markdownContent += "\n";
  }
  markdownContent += "\n";

  // deno-lint-ignore no-explicit-any
  const fields = (jsonClass["fields"] ?? []) as any[];
  fields.sort();

  for (const jsonField of fields) {
    const heading = `${jsonClass["name"]}.${jsonField["name"]}`;
    const headingId = heading;

    markdownContent += `### ${heading} {#${headingId}}\n`;
    markdownContent += "\n";
    markdownContent += `\`\`\`lua\n${jsonField["extends"]["view"]}\n\`\`\`\n`;
    markdownContent += "\n";
    if ("desc" in jsonField) {
      const description = (jsonField["desc"] as string)
        .trim()
        .replaceAll(linksRe, "[$1](#$1)");
      markdownContent += description;
      markdownContent += "\n";
    }
    markdownContent += "\n";
  }

  // deno-lint-ignore no-explicit-any
  const defines = jsonClass["defines"] as any[];
  for (const jsonDefine of defines) {
    const desc = jsonDefine.extends?.view;

    if (desc) {
      markdownContent += "\n";
      markdownContent += `\`\`\`lua\n${desc}\n\`\`\`\n`;
      markdownContent += "\n";
    }
  }

  markdownContent += "\n---\n";
}

await Deno.writeTextFile(OUTPUT_PATH, markdownContent);
