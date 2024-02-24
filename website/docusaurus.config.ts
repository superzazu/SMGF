import type { Config } from "@docusaurus/types";
import type * as Preset from "@docusaurus/preset-classic";
import type { PluginOptions as SearchPluginOptions } from "@easyops-cn/docusaurus-search-local";
import { themes } from "prism-react-renderer";

const lightTheme = themes.github;
const darkTheme = themes.dracula;

const config: Config = {
  title: "SMGF",
  tagline: "A small framework to make crossplatform 2D games in Lua.",

  // favicon: 'img/favicon.ico',

  // Set the production url of your site here
  url: "https://superzazu.github.io",
  // Set the /<baseUrl>/ pathname under which your site is served
  // For GitHub pages deployment, it is often '/<projectName>/'
  baseUrl: "/SMGF/",

  // GitHub pages deployment config.
  // If you aren't using GitHub pages, you don't need these.
  organizationName: "superzazu", // Usually your GitHub org/user name.
  projectName: "SMGF", // Usually your repo name.

  onBrokenLinks: "throw",
  onBrokenMarkdownLinks: "warn",

  // Even if you don't use internalization, you can use this field to set useful
  // metadata like html lang. For example, if your site is Chinese, you may want
  // to replace "en" with "zh-Hans".
  i18n: {
    defaultLocale: "en",
    locales: ["en"],
  },

  presets: [
    [
      "classic",
      {
        docs: {
          sidebarPath: require.resolve("./sidebars.js"),
          editUrl: "https://github.com/superzazu/SMGF/tree/master/docs/",
        },
        blog: false,
        theme: {
          customCss: require.resolve("./src/css/custom.css"),
        },
      } satisfies Preset.Options,
    ],
  ],

  themeConfig: {
    navbar: {
      title: "SMGF",
      // logo: {
      //   alt: "My Site Logo",
      //   src: "img/logo.svg",
      // },
      items: [
        {
          type: "doc",
          docId: "introduction",
          position: "left",
          label: "Documentation",
        },
        {
          label: "Download",
          href: "https://github.com/superzazu/SMGF/releases/latest",
        },
        // { to: "/blog", label: "Blog", position: "left" },
        {
          href: "https://github.com/superzazu/SMGF",
          label: "GitHub",
          position: "right",
        },
      ],
    },
    footer: {
      style: "dark",
      links: [
        {
          title: "Docs",
          items: [
            {
              label: "API reference",
              to: "/docs/api",
            },
            {
              label: "Download",
              href: "https://github.com/superzazu/SMGF/releases/latest",
            },
          ],
        },
        // {
        //   title: "Community",
        //   items: [
        //     {
        //       label: "Stack Overflow",
        //       href: "https://stackoverflow.com/questions/tagged/docusaurus",
        //     },
        //     {
        //       label: "Discord",
        //       href: "https://discordapp.com/invite/docusaurus",
        //     },
        //     {
        //       label: "Twitter",
        //       href: "https://twitter.com/docusaurus",
        //     },
        //   ],
        // },
        // {
        //   title: "More",
        //   items: [
        //     {
        //       label: "Blog",
        //       to: "/blog",
        //     },
        //     {
        //       label: "GitHub",
        //       href: "https://github.com/facebook/docusaurus",
        //     },
        //   ],
        // },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} SMGF, Inc. Built with Docusaurus.`,
    },
    docs: {
      sidebar: {
        hideable: true,
      },
    },
    colorMode: {
      // defaultMode: "light",
      // disableSwitch: false,
      respectPrefersColorScheme: true,
    },
    // announcementBar: {
    //   id: "first_release",
    //   content:
    //     'Version 1.0 available, <a target="_blank" rel="noopener noreferrer" href="#">check it out</a>!',
    //   backgroundColor: "#fafbfc",
    //   textColor: "#091E42",
    //   isCloseable: true,
    // },
    prism: {
      theme: lightTheme,
      darkTheme: darkTheme,
      additionalLanguages: ["lua"],
    },
  } satisfies Preset.ThemeConfig,

  themes: [
    [
      require.resolve("@easyops-cn/docusaurus-search-local"),
      {
        hashed: true,
        highlightSearchTermsOnTargetPage: true,
      } satisfies SearchPluginOptions,
    ],
  ],

  trailingSlash: true,
};

export default config;
