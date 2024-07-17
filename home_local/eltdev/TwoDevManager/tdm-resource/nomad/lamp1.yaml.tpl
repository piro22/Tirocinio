!cfg.include config/fcf/devmgr/definitions/lamp.yaml:

lamp1: !cfg.type:Lamp
  id: EIN-1001932
  overview: 'Sample of Lamp Overview'
  description: "Sample description for Lamp.
             For more information, :
             <html> <a href ='http://www.google.com'>click here</a> </html>"
  identifier: PLC1
  prefix: MAIN.Lamp1
  simulated: true
  dev_endpoint: "undefined"
  
  sim_endpoint: "opc.tcp://{{ range service "lamp1sim" }}{{ .Address }}:{{ .Port }}{{ end }}"
  
  fits_prefix: "LAMP1"


