# @ingroup ccf_control
# @copyright ESO - European Southern Observatory
# @brief CCF Control Application example configuration.

!cfg.include schema/ccf/common/server.schema.yaml:

server: !cfg.type:CcfServerTypeDef
  server_id:                   'TestDcs'
  req_endpoint:                'zpb.rr://{{ range service "exccfsim-req" }}{{ .Address }}:{{ .Port }}{{ end }}/'
  pub_endpoint:                'zpb.ps://{{ range service "exccfsim-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/'
  db_timeout:                  5000
  scxml:                       'config/ccf/control/sm.xml'
  log_properties:              'config/ccf/control/log.properties'
  req_timeout:                 60
  dictionaries:                ['dictionary/dit/stddid/primary.did.yaml',
                                'dictionary/ccf/common/ccf.did.yaml']
  oldb_prefix:                 'CcfTest'
  fits_prefix:                 'DET1'
  simulation:                  false
  init_setup:                  'local/exccfsimSetup.yaml'

  recording:
    metadata_map:              'config/ccf/control/metaData.mapping.yaml'
    rec_hist_size:             1000
    rec_hist_expiration:       2147483647

  tasks:
    monitoring:
      period:                  1
      nb_of_samples:           100

    acquisition:
      max_rate:                100
      input_queue_size:        10
      allow_lost_frames:       false
      allow_frame_skipping:    false

    processing:                [
      {
        pipeline:              'proc_1',
        output_queue_size:     20,
        allow_frame_skipping:  false,
        recipes:               [
          {
            name:              'recipe_1_1_centroid',
            adapter:           'ccf::stdrecipe::RecipeCentroid'
          },
          {
            name:              'recipe_1_2',
            adapter:           'ccf::common::RecipeBase'
          },
          {
            name:              'recipe_1_3',
            adapter:           'ccf::common::RecipeBase'
          }
        ],
        publishers:             [
          {
            name:                'pub_1_1_ddt',
            adapter:             'ccf::stdpub::PubDdt'
           },
          {
            name:                'pub_1_2_fits',
            adapter:             'ccf::stdpub::PubFits'
          },
          {
            name:                'pub_1_3_rtms',
            adapter:             'ccf::stdpub::PubRtms'
          }
        ]
      },

      {
        pipeline:              'proc_2',
        output_queue_size:     20,
        allow_frame_skipping:  true,
        recipes:               [
          {
            name:              'recipe_2_1',
            adapter:           'ccf::common::RecipeBase'
          },
          {
            name:              'recipe_2_2',
            adapter:           'ccf::common::RecipeBase'
          }
        ],
        publishers:             [
          {
            name:                'pub_2_1_fits',
            adapter:             'ccf::stdpub::PubFits'
          },
          {
            name:                'pub_2_2_test',
            adapter:             'ccf::common::PubBase'
          }
        ]
      }
    ]

  devices:                   [
    {
      name:                  'TestSimCamera1',
      id:                    'TestCamera1Id',
      type:                  'CCD',
      model:                 'SimulatorCamera',
      manufacturer:          'SimManufacturer',
      width:                 1620,
      height:                1220,
      max_resolution:        12,
      resolution:            12,
      protocol:              'GigE Vision',
      mapping:               'config/ccf/protocols/aravis/avt.mapping.yaml',
      sdk:                   'Aravis',
      address:               '134.171.12.196',
      sim_address:           'InternalSimulator',
      properties:            [
        {
          name:              'StreamBytesPerSecond',
          value:             '115000000',
          write_on_init:      true
        },
        {
          name:              'StreamBytesPerSecondMax',
          value:             '124000000',
          write_on_init:      true
        },
        {
          name:              'DelayStartAfterStop',
          value:             '0.100',
          write_on_init:      true
        },
        {
          name:              'GevSCPSPacketSize',
          value:             '1500',
          write_on_init:      true
        },
        {
          name:              'ChunkModeActive',
          value:             '0',
          write_on_init:      true
        },
        {
          name:              'PixelFormat',
          value:             'Mono16',
          write_on_init:      true
        }
      ]
    }
  ]
