# @ingroup ifw-templates
# @copyright ESO - European Southern Observatory
# @brief IFW Templates - CCF Configuration

!cfg.include schema/ccf/common/server.schema.yaml:

server: !cfg.type:CcfServerTypeDef
  server_id:                   'dcs'

  req_endpoint:                'zpb.rr://{{ range service "tdmccf2-req" }}{{ .Address }}:{{ .Port }}{{ end }}/'
  pub_endpoint:                'zpb.ps://{{ range service "tdmccf2-pub" }}{{ .Address }}:{{ .Port }}{{ end }}/'

  db_timeout:                  2000
  scxml:                       'config/ccf/control/sm.xml'
  log_properties:              'config/ccf/control/log.properties'
  req_timeout:                 60
  dictionaries:                ['dictionary/dit/stddid/primary.did.yaml',
                                'dictionary/ccf/common/ccf.did.yaml']
  oldb_prefix:                 'elt/tdm'
  fits_prefix:                 'DET1'
  simulation:                  true
  init_setup:                  'local/ccfSetup.yaml'

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
        pipeline:              'pipeline1',
        output_queue_size:     20,
        allow_frame_skipping:  false,
        recipes:               [
          {
            name:              'proc1_recipe1',
            adapter:           'ccf::common::RecipeBase'
          },
          {
            name:              'proc1_recipe2',
            adapter:           'ccf::common::RecipeBase'
          },
          {
            name:              'proc1_recipe3',
            adapter:           'ccf::common::RecipeBase'
          }
        ],
        publishers:             [
          {
            name:              'ddt_pub',
            adapter:           'ccf::stdpub::PubDdt'
          },
          {
            name:              'fits_pub',
            adapter:           'ccf::stdpub::PubFits'
          }
        ]
      },

      {
        pipeline:              'pipeline2',
        output_queue_size:     20,
        allow_frame_skipping:  true,
        recipes:               [
          {
            name:              'proc2_recipe1',
            adapter:           'ccf::common::RecipeBase'
          },
          {
            name:              'proc2_recipe2',
            adapter:           'ccf::common::RecipeBase'
          }
        ],
        publishers:             [
          {
            name:              'test_pub21',
            adapter:           'ccf::common::PubBase'
          },
          {
            name:              'test_pub22',
            adapter:           'ccf::common::PubBase'
          }
        ]
      }
    ]

  devices:                   [
    {
      name:                  'TestSimCamera1',
      id:                    'TestCamera1Id',
      type:                  'CCD',
      model:                 'Simulator',
      manufacturer:          'Simulator',
      width:                 1620,
      height:                1220,
      max_resolution:        12,
      resolution:            12,
      protocol:              'Simulator',
      mapping:               'config/ccf/protocols/aravis/avt.mapping.yaml',
      sdk:                   'Aravis',
      address:               '134.171.12.243',
      sim_address:           'InternalSimulator',
      properties:            [
        {
          name:              'StreamBytesPerSecondMax',
          value:             '124000000'
        },
        {
          name:              'DelayStartAfterStop',
          value:             '0.100'
        },
        {
          name:              'GevSCPSPacketSize',
          value:             '1500'
        },
        {
          name:              'ChunkModeActive',
          value:             '0'
        }
      ]
    }
  ]
