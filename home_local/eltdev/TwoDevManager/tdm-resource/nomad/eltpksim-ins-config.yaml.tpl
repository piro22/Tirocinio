# HLCC_DEPLOYMENT_DEPENDENT_FILE
# - Eltpksim configuration for instruments deployment

# eltpksim config for Nomad deployment for instruments (single machine).
# See also OneNote CCS High Level ... / HLCC Documentation / Telescope Simulator / eltpksim documentation
cfg:

  
  req_endpoint           : "zpb.rr://{{ env "NOMAD_ADDR_eltpksim_zmq" }}/" # IP address and port used to accept requests
  

  sm_scxml                   : "config/eltpksim/sm.xml" # SCXML state machine model
  log_properties             : "config/eltpksim/log.properties"
  oldb_uri_prefix            : "cii.oldb:///elt/hlcc/" # CII OLDB prefix
  oldb_conn_timeout : 1  # CII OLDB connection timeout in sec
  trs_health_enabled : 0  # disabled TRS health check to avoid continuous notifications on non-PTP machines
  pub:
    determ:
      
      nic                    : {{ env "NOMAD_IP_eltpksim_mudpi" }} # iface for mcast, default: 127.0.0.1
      
    dds:
      profile                : Localhost_Only
 #    profile_file           : # ignored if profile not set. default: config/hlcc/dds/hlccDdsQosProfiles.xml
  params:
    moving_min_delta         : 0.003 #arcsec/s, minimum change of position to consider telescope moving
    tracking_error           : 0.5  #arcsec, tolerable distance (error) between actual and commanded position
    alt_speed_deg_per_s      : 5.0  #degree/s. Real system max 1 deg/s.
    az_speed_deg_per_s       : 5.0  #degree/s. Real system max 2 deg/s.
  altaz:
    alt_upper_limit_op       : 1.544616  #operational limit in Radians
    alt_lower_limit_op       : 0.349066  #operational limit in Radians
    az_upper_limit_op        : 6.2832    #operational limit in Radians (2PI rounded up)
    az_lower_limit_op        : -3.141593 #operational limit in Radians
    alt_upper_limit_fct      : 1.570796  #functional limit in Radians
    alt_lower_limit_fct      : 0.0       #functional limit in Radians
    az_upper_limit_fct       : 6.2832    #functional limit in Radians (2PI rounded up)
    az_lower_limit_fct       : -3.141593 #functional limit in Radians
  site:
    info:
      elevation              : 3046.0    # meters
      latitude               : -0.429164 # Radians
      longitude              : 1.225075  # Radians
      id                     : "ELT"
