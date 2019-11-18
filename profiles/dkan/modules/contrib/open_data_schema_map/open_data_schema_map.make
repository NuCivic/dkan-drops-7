core: 7.x
api: '2'
projects:
  fast_token_browser:
    version: '1.5'
libraries:
  symfonyserializer:
    type: libraries
    download:
      type: git
      url: 'https://github.com/symfony/serializer.git'
      tag: v3.4.15
  json-schema:
    type: libraries
    download:
      type: git
      url: 'https://github.com/justinrainbow/json-schema.git'
      tag: 5.2.7