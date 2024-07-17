# HLCC_DEPLOYMENT_DEPENDENT_FILE
# - Telmon configuration for instruments deployment

cfg:

  
  req_endpoint                 : "zpb.rr://{{ env "NOMAD_ADDR_telmon_zmq" }}/" # IP address and port used to accept requests
  

  sm_scxml                     : "config/telmon/sm.xml" # SCXML state machine model
  log_properties               : "config/telmon/log.properties"
  oldb_uri_prefix              : "cii.oldb:///elt/hlcc/" # CII OLDB prefix
  pub:
    dds:
      profile                  : Localhost_Only
 #    profile_file             : # ignored if profile not set. default: config/hlcc/dds/hlccDdsQosProfiles.xml
  estimation_period_ms         : 1000 # milliseconds
  ready_for_handover_inertia_ms: 850 # milliseconds
  estim_scripts                :
                                - MonScripts.ReadyForHandoverEstimation
                                - MonScripts.CcsStateEstimation
                                - MonScripts.TrackingEstimation
                                - MonScripts.SegmentExchangeModeEstimation
                                - MonScripts.NightTimeEstimation
  
  monitored_apps               :
                                - name: telifsim
                                  oldb: cii.oldb:///elt/hlcc/telifsim/mon/state
                                  stdcmds: zpb.rr://{{ range service "telifsim|passing,warning" }}{{ .Address }}:{{ .Port }}{{ end }}/telifsim/StdCmds
                                  pubsub:
                                  ignore: false

                                - name: eltpksim
                                  oldb: cii.oldb:///elt/hlcc/eltpksim/mon/state
                                  stdcmds: zpb.rr://{{ range service "eltpksim|passing,warning" }}{{ .Address }}:{{ .Port }}{{ end }}/eltpksim/StdCmds
                                  pubsub:
                                  ignore: false

                                - name: telmon
                                  oldb: cii.oldb:///elt/hlcc/telmon/mon/state
                                  stdcmds: zpb.rr://{{ env "NOMAD_ADDR_telmon_zmq" }}/telmon/StdCmds
                                  pubsub:
                                  ignore: false
  
  night_time_cfg:
    needed_subsystems:
     - "eltpksim"
     - "telifsim"
     - "telmon"
    not_needed_subsystems: []
    not_needed_critical_subsystems: []

