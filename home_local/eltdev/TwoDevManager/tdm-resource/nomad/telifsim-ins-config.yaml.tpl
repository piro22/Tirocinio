# HLCC_DEPLOYMENT_DEPENDENT_FILE
# - Telifsim configuration for instruments deployment

cfg:
  
  req_endpoint               : "zpb.rr://{{ env "NOMAD_ADDR_telifsim_zmq" }}/" # IP address and port used to accept requests
  req_eltpksim               : "zpb.rr://{{ range service "eltpksim|passing,warning" }}{{ .Address }}:{{ .Port }}{{ end }}/" # IP address and port used to connect eltpksim
  

  sm_scxml                   : "config/telifsim/sm.xml" # SCXML state machine model
  log_properties             : "config/telifsim/log.properties"
  oldb_uri_prefix            : "cii.oldb:///elt/hlcc/" # CII OLDB prefix
  oldb_conn_timeout : 1  # CII OLDB connection timeout in sec
  trs_health_enabled : 0  # disabled TRS health check to avoid continuous notifications on non-PTP machines
  pub:
    dds:
      profile                : Localhost_Only
 #    profile_file           : # ignored if profile not set. default: config/hlcc/dds/hlccDdsQosProfiles.xml
  altaz:
    alt_upper_limit_op       : 1.544616  #operational limit in Radians
    alt_lower_limit_op       : 0.349066  #operational limit in Radians
    az_upper_limit_op        : 6.283185  #operational limit in Radians
    az_lower_limit_op        : -3.141593 #operational limit in Radians
    alt_upper_limit_fct      : 1.570796  #functional limit in Radians
    alt_lower_limit_fct      : 0.0       #functional limit in Radians
    az_upper_limit_fct       : 6.283185  #functional limit in Radians
    az_lower_limit_fct       : -3.141593 #functional limit in Radians
  rous:
    timer_period_s           : 300       #period in seconds
  ready_for_handover:
    estimation_period_ms     : 1000 #period in milliseconds
  tel_named_pos              : !cfg.type:TelNamedPositions
    - pos_string: PARK
      alt_pos: 0.0
      az_pos: 0.0

    - pos_string: ZENITH
      alt_pos: 1.570796
      az_pos: 0.0

    - pos_string: SEGEXCH
      alt_pos: 1.570796
      az_pos: 0.523599
