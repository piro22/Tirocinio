# @ingroup ifw-templates
# @copyright ESO - European Southern Observatory
# @brief IFW Templates - CCF Initialisation Setup

!cfg.include config/ccfSetup.schema.yaml:

dcs: !cfg.type:DcsEx

  expo:
    mode:                          'Continuous' # Continuous, Finite
    nb:                            100
    time:                          0.2
    frame_rate:                    1000.0
    win_start_x:                   0
    win_start_y:                   0
    win_width:                     640
    win_height:                    640
    bin_x:                         1
    bin_y:                         1

  sim:
    type:                          'File'  # File, Pattern
    file:                          'image/ccf/control/testCube16Bit.fits'
    max_shift_pr_frame:            5    # Max shift in pixels per read out
    max_shift:                     32   # Total max shift in pixels in one direction
    noise:                         10   # Max noise to add per read out (0=off)

  pipeline1:
    enabled:                       true

    proc1_recipe1:
      enabled:                     true
      delay:                       0.0

    proc1_recipe2:
      enabled:                     true
      delay:                       0.0

    proc1_recipe3:
      enabled:                     true
      delay:                       0.0

    ddt_pub:
      enabled:                     true
      ddt_id:                      'CcfTest11'
      
      ddt_broker:                  'zpb.rr://{{ range service "tdmbroker" }}{{ .Address }}:{{ .Port }}{{ end }}/broker/Broker1'
      
      ddt_max_rate:                1000

    fits_pub:
      enabled:                     true
      basename:                    'PubFitsTest'
      max_size:                    100 # MB
      nb_of_frames:                1
      format:                      'Single' # Single, Cube, MEF
      overwrite:                   false # true
      rec_mode:                    'All' # All, I:<nb>, P:<period>

  pipeline2:
    enabled:                       true

    proc2_recipe1:
      enabled:                     true
      delay:                       0.0

    proc2_recipe2:
      enabled:                     true
      delay:                       0.0

    test_pub21:
      enabled:                     true
      delay:                       0.0

    test_pub22:
      enabled:                     true
