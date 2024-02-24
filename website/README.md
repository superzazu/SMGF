# SMGF website

Made with [Docusaurus 3](https://docusaurus.io/).

```sh
npm install
npm run start # for local development (with live reload)
npm run build # builds site to build/ dir
npm run serve # to test build

# to deploy, either one:
USE_SSH=true npm run deploy
GIT_USER=superzazu npm run deploy
```

File `api.md` is generated automatically by `build-docs.ts` script.
