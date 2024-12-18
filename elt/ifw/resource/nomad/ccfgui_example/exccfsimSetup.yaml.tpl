# @ingroup ccf_control
# @copyright ESO - European Southern Observatory
# @brief CCF Control Application example setup.

!cfg.include schema/ccf/control/setupExample.schema.yaml:

dcs: !cfg.type:DcsExTypeDef

  expo:
    mode:                          'Continuous' # Continuous, Finite
    nb:                            10
    time:                          1.123
    frame_rate:                    100.0
    win_start_x:                   0
    win_start_y:                   0
    win_width:                     512
    win_height:                    512
    bin_x:                         1
    bin_y:                         1

  sim:
    type:                          'File'  # File, Pattern
    file:                          'image/ccf/control/testCube16Bit.fits'
    max_shift_pr_frame:            5       # Max shift in pixels per read out
    max_shift:                     32      # Total max shift in pixels in one direction
    noise:                         10      # Max noise to add per read out (0=off)

  proc_1:
    enabled:                       true

    recipe_1_1_centroid:
      enabled:                     true
      delay:                       0.0
      max_centre_error:            0.5
      max_sigma_error:             3
      robustness:                  7

    recipe_1_2:
      enabled:                     true
      delay:                       0.0

    recipe_1_3:
      enabled:                     true
      delay:                       0.0

    pub_1_1_ddt:
      enabled:                     true
      delay:                       0.0
      ddt_id:                      'CcfTest11'
      ddt_broker:                  'zpb.rr://{{ range service "exccfsimbroker" }}{{ .Address }}:{{ .Port }}{{ end }}/broker/Broker1'
      ddt_max_rate:                1000

    pub_1_2_fits:
      enabled:                     true
      basename:                    'PubFitsTest1'
      max_size:                    100 # MB
      nb_of_frames:                7
      format:                      'Single' # Single, Cube, MEF
      overwrite:                   true # true
      rec_mode:                    'All' # All, I:<nb>, P:<period>

    pub_1_3_rtms:
      enabled:                     true
      id:                          'RtmsPub'
      max_rate:                    220
      bpp:                         2
      interface:                   ''
      address:                     '127.0.0.1'
      port:                        59000
      topic_id:                    10
      sample_id:                   500
      pkt_send_delay:              0.001

  proc_2:
    enabled:                     true

    recipe_2_1:
      enabled:                     true
      delay:                       0.0

    recipe_2_2:
      enabled:                     true
      delay:                       0.0

    pub_2_1_fits:
      enabled:                     true
      basename:                    'PubFitsTest2'
      max_size:                    100 # MB
      nb_of_frames:                5
      format:                      'Single' # Single, Cube, MEF
      overwrite:                   true
      rec_mode:                    'All' # All, I:<nb>, P:<period>

    pub_2_2_test:
      enabled:                      true
      delay:                        0.0
