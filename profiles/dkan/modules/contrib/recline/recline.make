core: 7.x
api: '2'
libraries:
  recline:
    download:
      type: git
      url: 'https://github.com/NuCivic/recline.js.git'
      branch: master
  lodash:
    download:
      type: git
      url: 'https://github.com/lodash/lodash.git'
      revision: e21e993729861a2bc1d01c858cfabce7a27d2861
  backbone:
    download:
      type: git
      url: 'https://github.com/jashkenas/backbone.git'
      revision: e109f6d3e7a366f933f1f34405ca97effecae6c5
  csv:
    download:
      type: git
      url: 'https://github.com/okfn/csv.js.git'
      revision: 7150de4c8d5e02626ac6a7fb9c178e955c964faf
  slickgrid:
    download:
      type: git
      url: 'https://github.com/mleibman/SlickGrid.git'
      revision: e004912b5ce29ac0d0cb04df50fe66db5e3af9ea
  mustache:
    download:
      type: git
      url: 'https://github.com/janl/mustache.js.git'
      revision: d4ba5a19d4d04b139bbf7840fe342bb43930aee3
  moment:
    download:
      type: git
      url: 'https://github.com/moment/moment.git'
      tag: 2.11.2
  leaflet:
    download:
      type: git
      url: 'https://github.com/NuCivic/Leaflet.git'
      tag: v1.0.2-alt-marker-shadow-5258
  flot:
    download:
      type: git
      url: 'https://github.com/flot/flot.git'
      revision: 7f5f90384ed6d6c30b232580d358c84e355919ab
  deep_diff:
    download:
      type: git
      url: 'https://github.com/flitbit/diff.git'
      revision: 07e91c624e5016be5c5c6560a9eabe49ef3ba2d0
    directory_name: deep_diff
  recline_deeplink:
    download:
      type: git
      url: 'https://github.com/NuCivic/recline-deeplink.git'
      revision: c1695d669f8314ed8b66e5907eb4f1bc4a8a9495
    directory_name: recline_deeplink
  leaflet_markercluster:
    download:
      type: git
      url: 'https://github.com/Leaflet/Leaflet.markercluster.git'
      revision: eb922a3646d2e1ef9ed9de99e20200709f1f9bb5
    directory_name: leaflet_markercluster
  xls:
    download:
      type: git
      url: 'https://github.com/NuCivic/recline.backend.xlsx'
      revision: a5d49f7305895f147c3e9a12115c2456a897f941
    directory_name: xls
  jsxlsx:
    download:
      type: git
      url: 'https://github.com/SheetJS/js-xlsx'
      revision: 53f7f6d9446ccd680c9b13992d6dcdccde49a8f6
    directory_name: jsxlsx
  jsonview:
    download:
      type: git
      url: 'https://github.com/yesmeck/jquery-jsonview.git'
      revision: 84fb68cc25e4749d2adf5af8dc3adefd80b4c430
    directory_name: jsonview