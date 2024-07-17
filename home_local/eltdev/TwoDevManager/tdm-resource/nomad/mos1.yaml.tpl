!cfg.include config/fcs/definitions/mos.yaml:

mos1: !cfg.type:Mos
  identifier: PLC1                             # OPCUA Object Identifier
  prefix: MAIN.Mos1                       # OPCUA attribute prefix
  simulated: true
  dev_endpoint: undefined                            # To be set if a PLC is available
  sim_endpoint: opc.tcp://{{ range service "mos1sim" }}{{ .Address }}:{{ .Port }}{{ end }}
  fits_prefix: "OPTI1"
  mapfile: "config/fcs/mapping/mos.yaml"
  ctrl_config:
    timeout:       2000
