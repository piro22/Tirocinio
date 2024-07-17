!cfg.include config/fcf/devmgr/definitions/shutter.yaml:

shutter1: !cfg.type:Shutter
  id: EIN-1001930
  overview: 'Sample of Shutter Overview'
  description: "Sample description for Shutter.
               For more information, :
               <html> <a href ='http://www.google.com'>click here</a> </html>"
  identifier: PLC1                             # OPCUA Object Identifier
  prefix: MAIN.Shutter1                       # OPCUA attribute prefix
  simulated: true
  dev_endpoint: undefined				# To be set, if a PLC is available
  
  sim_endpoint: opc.tcp://{{ range service "shutter1sim" }}{{ .Address }}:{{ .Port }}{{ end }}
  
  fits_prefix: "SHUT1"
  ctrl_config:
    initial_state:  false


