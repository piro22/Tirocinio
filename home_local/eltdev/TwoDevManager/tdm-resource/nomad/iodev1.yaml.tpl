!cfg.include config/fcf/devmgr/definitions/sensor.yaml:


iodev1: !cfg.type:Sensor
  identifier: PLC1
  prefix: MAIN.Iodev1
  mapfile: "config/fcf/devmgr/mapping/sensor.yaml"
  simulated: true
  dev_endpoint: not_defined
  
  sim_endpoint: opc.tcp://{{ range service "iodev1sim" }}{{ .Address }}:{{ .Port }}{{ end }}
  
  fits_prefix: "SENSOR1"
  channels: [
  {
  name: ch1,
  description: "This is the channel 1",
  fits_prefix: "CH1 STAT",
  type: DI,
  unit: mm,
  map: di1,
  named_values: [
  {
        name: 'HIGH',
        value: '1'
       },
       {
        name: 'LOW',
        value: '0'
        }
       ]
  },
  {
  name: ch2,
  description: "channel2",
  fits_prefix: "CH2 STAT",
  type: DI,
  unit: dd,
  map: di2,
  named_values: [
  {
        name: 'ON',
        value: '1'
       },
       {
        name: 'OFF',
        value: '0'
        }
       ]
  },
  {
  name: ch3,
  description: "channel3",
  fits_prefix: "CH3 STAT",
  type: DI,
  unit: df,
  map: di3
  },
  {
  name: ch4,
  description: "channel4",
  fits_prefix: "CH4 VAL",
  type: AI,
  unit: volts,
  map: ai1
  },
  {
  name: ch5,
  description: "channel5",
  fits_prefix: "CH5 VAL",
  type: AI,
  unit: degrees,
  map: ai2
  },
  {
  name: ch6,
  description: "channel6",
  fits_prefix: "CH6 VAL",
  type: AI,
  map: ai3
  },
  ]

