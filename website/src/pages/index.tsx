import React, { FC, ReactElement } from "react";
import { Redirect } from "@docusaurus/router";
import useDocusaurusContext from "@docusaurus/useDocusaurusContext";
import Layout from "@theme/Layout";
import CodeBlock from "@theme/CodeBlock";
import Link from "@docusaurus/Link";
import clsx from "clsx";

const REDIRECT_TO_DOCS_MODE = false;

import styles from "./index.module.css";

const features: { title: string; description: string | ReactElement }[] = [
  {
    title: "Code once",
    description: (
      <>
        Games are written in Lua and can be run on macOS, Linux, Windows and on
        the web.
      </>
    ),
  },
  {
    title: "Easy to use",
    description: (
      <>
        Minimal set of APIs to handle drawing, playing audio, handling user
        input (mouse, keyboard, gamepads) and file I/O in a sandboxed
        environment.
      </>
    ),
  },
  {
    title: "Free",
    description: (
      <>
        Open source under the zlib license, which means you can use it and
        distribute your games freely and free of charge.
      </>
    ),
  },
];

const Feature: FC<{ title: string; description: string | ReactElement }> = ({
  title,
  description,
}) => {
  return (
    <div className={clsx("col col--4")}>
      <div className="text--center">
        {/* <Svg className={styles.featureSvg} role="img" /> */}
      </div>
      <div className="text--center padding-horiz--md">
        <h3>{title}</h3>
        <p>{description}</p>
      </div>
    </div>
  );
};

const HomepageFeatures: FC = () => {
  return (
    <section className={styles.features}>
      {features.map((props, idx) => (
        <Feature key={idx} {...props} />
      ))}
    </section>
  );
};

const HomepageHeader: FC = (props) => {
  const { siteConfig } = useDocusaurusContext();
  return (
    <header className={clsx("hero", styles.heroBanner)}>
      <div className="container">
        <h1 className="hero__title">{siteConfig.title}</h1>
        <p className="hero__subtitle">{siteConfig.tagline}</p>
        <div className={styles.heroBannerButtons}>
          <Link
            className="button button--primary button--lg"
            to="/docs/getting-started"
          >
            Get started
          </Link>

          <Link
            className="button button--secondary button--lg"
            to="/docs/introduction"
          >
            Introduction
          </Link>
        </div>
      </div>
    </header>
  );
};

const Homepage: FC = () => {
  const { siteConfig } = useDocusaurusContext();

  if (REDIRECT_TO_DOCS_MODE) {
    return <Redirect to="/docs" />;
  }

  return (
    <Layout title="Homepage" description={siteConfig.tagline}>
      <HomepageHeader />
      <main>
        <HomepageFeatures />

        <h1>Examples</h1>
        <CodeBlock
          language="lua"
          title="Display an image on screen"
          showLineNumbers
        >
          {`function smgf.init()
  my_texture = smgf.graphics.new("my_image.png")
end

function smgf.draw()
  smgf.graphics.clear()
  smgf.graphics.draw(my_texture, 5, 5)
end`}
        </CodeBlock>

        <CodeBlock
          language="lua"
          title="Play a sound on key press"
          showLineNumbers
        >
          {`function smgf.init()
  sound = smgf.audio.new("sfx.wav")
end

function smgf.key_down(key)
  if key == "return" then
    sound:play()
  end
end`}
        </CodeBlock>
      </main>
    </Layout>
  );
};

export default Homepage;
