!cfg.include config/fcf/devmgr/definitions/motor.yaml:

motor1: !cfg.type:Motor
  id: EIN-1001933
  overview: 'Sample of Motor Overview'
  description: "Sample description for Motor.
           For more information, :
           <html> <a href ='http://www.google.com'>click here</a> </html>"
  identifier: PLC1
  prefix: MAIN.Motor1
  simulated: true
  dev_endpoint: undefined                            # To be set if a PLC is available
  
  sim_endpoint: opc.tcp://{{ range service "motor1sim" }}{{ .Address }}:{{ .Port }}{{ end }}
  
  fits_prefix: "MOT1"
  initialisation: [
         {
          step: 'FIND_LHW',
          value1: 4.0,
          value2: 4.0
         },
         {
          step: 'FIND_UHW',
          value1: 4.0,
          value2: 4.0
         },
         {
          step: 'CALIB_ABS',
          value1: 0.0,
          value2: 0.0
         },
         {
          step: 'END',
          value1: 0.0,
          value2: 0.0
         }
       ]
  positions: [
       {
        name: 'ON_NAME',
        value: 30
       },
       {
        name: 'OFF_NAME',
        value: 100
        }
       ]

  ctrl_config:
    velocity:              3.0
    min_pos:               0.0
    max_pos:               359.0
    active_low_ref:        true
    active_low_uhw:        true

